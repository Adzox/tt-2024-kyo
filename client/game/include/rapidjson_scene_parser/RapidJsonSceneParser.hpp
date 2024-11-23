#pragma once

#include <Assets/ITextFileLoader.hpp>
#include <SceneParser/ISceneParser.hpp>
#include <SceneParser/SceneObjectTokens.hpp>

#include <rapidjson/document.h>

class RapidJsonSceneParser : public SceneParser::ISceneParser {
 public:
  RapidJsonSceneParser(Assets::ITextFileLoader& textFileLoader);

  std::optional<SceneParser::SceneObjectToken> ParseSceneData(std::string_view resourcePath) override;

  std::optional<SceneParser::SceneObjectToken> GetSceneObjectToken(
      rapidjson::GenericObject<false, rapidjson::Value>& object,
      std::string_view resourcePath,
      bool suppressErrors = false);

  std::optional<SceneParser::SceneObjectIncludeToken> GetIncludeToken(
      rapidjson::GenericObject<false, rapidjson::Value>& object,
      std::string_view resourcePath);

  std::optional<SceneParser::SceneComponentToken> GetSceneComponentToken(
      std::string id,
      rapidjson::GenericObject<false, rapidjson::Value>& componentObject);

  std::optional<SceneParser::SceneComponentMapValue> GetSceneComponentMapValue(
      rapidjson::GenericObject<false, rapidjson::Value>& componentObject);

  std::optional<SceneParser::SceneComponentArrayValue> GetSceneComponentArrayValue(
      rapidjson::GenericArray<false, rapidjson::Value>& componentArray);

  std::optional<SceneParser::SceneComponentEntry> GetSceneComponentEntry(rapidjson::Value& entryValue);

 private:
  Assets::ITextFileLoader& textFileLoader;
};