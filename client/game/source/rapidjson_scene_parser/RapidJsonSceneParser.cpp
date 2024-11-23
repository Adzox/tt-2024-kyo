#include "rapidjson_scene_parser/RapidJsonSceneParser.hpp"

#include <Log/Log.hpp>
#include <SceneParser/ComponentTokens.hpp>

#include <rapidjson/document.h>

RapidJsonSceneParser::RapidJsonSceneParser(Assets::ITextFileLoader& textFileLoader) : textFileLoader(textFileLoader) {};

std::optional<SceneParser::SceneObjectToken> RapidJsonSceneParser::ParseSceneData(std::string_view resourcePath) {
  std::optional<std::string> fileContents = textFileLoader.LoadTextFile(resourcePath);
  if (!fileContents.has_value()) {
    return std::nullopt;
  }

  rapidjson::Document document;
  document.Parse(fileContents.value().c_str());

  // Todo: Replace checking values with JSON Schema validation
  if (!document.IsObject()) {
    Log::LogError("Failed parsing scene file '{}': Root is not an object!", resourcePath);
    return std::nullopt;
  }
  auto rootObject = document.GetObject();
  return GetSceneObjectToken(rootObject, resourcePath);
};

std::optional<SceneParser::SceneObjectToken> RapidJsonSceneParser::GetSceneObjectToken(
    rapidjson::GenericObject<false, rapidjson::Value>& object,
    std::string_view resourcePath,
    bool supressErrors) {
  SceneParser::SceneObjectToken token;

  // Parsing name
  if (!object.HasMember("name") || !object["name"].IsString()) {
    if (!supressErrors) {
      Log::LogError("Failed parsing scene file '{}': SceneObject is missing the string property 'name'!", resourcePath);
    }
    return std::nullopt;
  }
  token.name = object["name"].GetString();

  // Parsing tags
  if (object.HasMember("tags")) {
    if (!object["tags"].IsArray()) {
      Log::LogError("Failed parsing scene file '{}': SceneObject is missing the array property 'tags'!", resourcePath);
      return std::nullopt;
    }
    for (auto& value : object["tags"].GetArray()) {
      if (!value.IsString()) {
        Log::LogError("Failed parsing scene file '{}': SceneObject contains non-string tags!", resourcePath);
        return std::nullopt;
      }
      token.tags.insert(value.GetString());
    }
  }

  // Parsing Components
  if (object.HasMember("components")) {
    for (auto& value : object["components"].GetObject()) {
      if (!value.value.IsObject()) {
        Log::LogError("Failed parsing scene file '{}': SceneObject contains non-object component!", resourcePath);
        return std::nullopt;
      }
      auto componentObject = value.value.GetObject();
      auto component = GetSceneComponentToken(value.name.GetString(), componentObject);
      if (!component.has_value()) {
        Log::LogError("Failed parsing scene file '{}': SceneObject contains non-parseable component!", resourcePath);
        return std::nullopt;
      }
      token.components.push_back(component.value());
    }
  }

  // Parsing Children
  if (object.HasMember("children")) {
    for (auto& value : object["children"].GetArray()) {
      if (!value.IsObject()) {
        Log::LogError("Failed parsing scene file '{}': SceneObject contains non-valid child!", resourcePath);
        return std::nullopt;
      }
      auto childObject = value.GetObject();
      auto child = GetSceneObjectToken(childObject, resourcePath, true);
      if (child.has_value()) {
        token.children.push_back(child.value());
        continue;
      }

      auto include = GetIncludeToken(childObject, resourcePath);
      if (include.has_value()) {
        token.children.push_back(include.value());
        continue;
      }

      Log::LogError("Failed parsing scene file '{}': SceneObject contains non-parseable child include!", resourcePath);
      return std::nullopt;
    }
  }

  return token;
}

std::optional<SceneParser::SceneObjectIncludeToken> RapidJsonSceneParser::GetIncludeToken(
    rapidjson::GenericObject<false, rapidjson::Value>& object,
    std::string_view resourcePath) {
  SceneParser::SceneObjectIncludeToken token;

  // Parsing path
  if (!object.HasMember("path") || !object["path"].IsString()) {
    Log::LogError("Failed parsing included scene file in '{}': Include is missing the string property 'path'!",
                  resourcePath);
    return std::nullopt;
  }
  token.path = object["path"].GetString();

  // Parsing enableOverride
  bool enableOverride = true;
  if (object.HasMember("enableOverride")) {
    if (!object["enableOverride"].IsBool()) {
      Log::LogError(
          "Failed parsing included scene file '{}' in '{}': Property 'enableOverride' must be a boolean property!",
          token.path,
          resourcePath);
      return std::nullopt;
    }
    token.enableOverride = object["enableOverride"].GetBool();
    ;
  }

  // Parsing extraTags
  if (object.HasMember("extraTags")) {
    if (!object["extraTags"].IsArray()) {
      Log::LogError("Failed parsing included scene file '{}' in '{}': Property 'extraTags' must be an array property!",
                    token.path,
                    resourcePath);
      return std::nullopt;
    }
    for (auto& value : object["extraTags"].GetArray()) {
      if (!value.IsString()) {
        Log::LogError("Failed parsing included scene file '{}' in '{}': Include contains non-string extra tags!",
                      token.path,
                      resourcePath);
        return std::nullopt;
      }
      token.extraTags.insert(value.GetString());
    }
  }

  return token;
}

std::optional<SceneParser::SceneComponentToken> RapidJsonSceneParser::GetSceneComponentToken(
    std::string id,
    rapidjson::GenericObject<false, rapidjson::Value>& componentObject) {
  SceneParser::SceneComponentToken token;
  token.id = id;
  // Parsing arguments
  auto map = GetSceneComponentMapValue(componentObject);
  if (!map.has_value()) {
    Log::LogError("Failed parsing scene component: failed parsing parameters for component!");
    return std::nullopt;
  }
  token.data = map.value();
  return token;
}

std::optional<SceneParser::SceneComponentMapValue> RapidJsonSceneParser::GetSceneComponentMapValue(
    rapidjson::GenericObject<false, rapidjson::Value>& componentObject) {
  SceneParser::SceneComponentMapValue token;
  for (auto& member : componentObject) {
    auto fieldName = member.name.GetString();
    auto value = GetSceneComponentEntry(member.value);
    if (!value.has_value()) {
      Log::LogError("Failed parsing component entry: failed parsing map values");
      return std::nullopt;
    }
    token.entries[fieldName] = value.value();
  }
  return token;
}

std::optional<SceneParser::SceneComponentArrayValue> RapidJsonSceneParser::GetSceneComponentArrayValue(
    rapidjson::GenericArray<false, rapidjson::Value>& componentArray) {
  SceneParser::SceneComponentArrayValue token;
  for (auto& entry : componentArray) {
    auto value = GetSceneComponentEntry(entry);
    if (!value.has_value()) {
      Log::LogError("Failed parsing component entry: failed parsing array values");
      return std::nullopt;
    }
    token.entries.push_back(value.value());
  }
  return token;
}

std::optional<SceneParser::SceneComponentEntry> RapidJsonSceneParser::GetSceneComponentEntry(
    rapidjson::Value& entryValue) {
  SceneParser::SceneComponentEntry entry;

  if (entryValue.IsString()) {
    entry.value = entryValue.GetString();
  } else if (entryValue.IsInt()) {
    entry.value = entryValue.GetInt();
  } else if (entryValue.IsFloat()) {
    entry.value = entryValue.GetFloat();
  } else if (entryValue.IsBool()) {
    entry.value = entryValue.GetBool();
  } else if (entryValue.IsArray()) {
    auto array = entryValue.GetArray();
    if (auto arrayValue = GetSceneComponentArrayValue(array)) {
      entry.value = arrayValue.value();
    } else {
      return std::nullopt;
    }
  } else if (entryValue.IsObject()) {
    auto map = entryValue.GetObject();
    if (auto mapValue = GetSceneComponentMapValue(map)) {
      entry.value = mapValue.value();
    } else {
      return std::nullopt;
    }
  } else {
    Log::LogError("Failed parsing component entry: unsupported type!");
    return std::nullopt;
  }
  return entry;
}
