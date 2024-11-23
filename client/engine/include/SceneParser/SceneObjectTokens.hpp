#pragma once

#include "SceneParser/ComponentTokens.hpp"

#include <set>
#include <string>
#include <variant>
#include <vector>

namespace SceneParser {

struct SceneComponentToken {
  std::string id;
  SceneComponentMapValue data;
};

struct SceneObjectIncludeToken {
  std::string path;
  std::optional<std::string> nameOverride; 
  bool enableOverride = true;
  std::set<std::string> extraTags;
};

struct SceneObjectToken {
  std::string name;
  bool enabled = true;
  std::set<std::string> tags;
  std::vector<SceneComponentToken> components;
  std::vector<std::variant<SceneObjectToken, SceneObjectIncludeToken>> children;
};

}  // namespace SceneParser
