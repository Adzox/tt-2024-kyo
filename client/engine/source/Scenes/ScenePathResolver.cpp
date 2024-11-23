#include "Scenes/ScenePathResolver.hpp"

#include <vector>

namespace Scenes {

namespace {

static const char kPathDelimiter = '/';
static const char kTagToken = '#';
static const char kComponentToken = '@';
static const char kWildCardToken = '*';

bool MatchesPathPart(SceneObject& object, std::string_view part) {
  char op = part[0];
  if (op == kTagToken) {
    return object.HasTag({part.begin() + 1, part.end()});
  } else if (op == kComponentToken) {
    return object.HasComponentNamed({part.begin() + 1, part.end()});
  } else if (op == kWildCardToken) {
    return true;
  } else {
    return object.GetName() == part;
  }
}

}  // namespace

SceneObject* ParseScenePathFirst(SceneObject& sceneObject, std::string_view scenePath) {
  if (scenePath == "") {
    return {};
  }

  auto pos = scenePath.find_first_of("/");
  pos = pos == std::string::npos ? scenePath.length() : pos;

  if (!MatchesPathPart(sceneObject, {scenePath.data(), pos})) {
    return {};
  }

  if (pos == scenePath.length()) {
    return &sceneObject;
  }
  for (auto& child : sceneObject.GetChildren()) {
    if (auto a = child.lock()) {
      auto result = ParseScenePathFirst(*a, {scenePath.begin() + pos + 1, scenePath.end()});
      if (result != nullptr) {
        return result;
      }
    }
  }
  return nullptr;
}

};  // namespace Scenes
