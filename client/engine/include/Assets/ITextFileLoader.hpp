#pragma once

#include <string>
#include <string_view>
#include <optional>

namespace Assets {

class ITextFileLoader {
 public:
  virtual ~ITextFileLoader() = default;
  virtual std::optional<std::string> LoadTextFile(std::string_view resourcePath) = 0;
};

}  // namespace Assets
