#pragma once

#include "SceneParser/SceneObjectTokens.hpp"

#include <optional>

namespace SceneParser {

class ISceneParser {
 public:
  ~ISceneParser() = default;

  virtual std::optional<SceneObjectToken> ParseSceneData(std::string_view resourcePath) = 0;
};

}  // namespace SceneParser
