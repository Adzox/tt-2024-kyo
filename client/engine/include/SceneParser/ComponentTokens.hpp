#pragma once

#include <array>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <variant>
#include <vector>

namespace SceneParser {

struct SceneComponentEntry;

struct SceneComponentMapValue {
  std::map<std::string, SceneComponentEntry> entries;

  template <typename T>
  std::optional<T> ParseEntry(const std::string& name);
};

struct SceneComponentArrayValue {
  std::vector<SceneComponentEntry> entries;

  template <typename T, size_t D>
  std::optional<std::array<T, D>> ParseAsArray();

  template <typename T>
  std::optional<std::vector<T>> ParseAsList();
};

struct SceneComponentEntry {
  std::variant<std::string, int, float, bool, SceneComponentMapValue, SceneComponentArrayValue> value;

  template <typename T>
  std::optional<T> Parse();
};

template <typename T>
std::optional<T> SceneComponentEntry::Parse() {
  if (std::holds_alternative<T>(value)) {
    return std::get<T>(value);
  } else {
    return std::nullopt;
  }
}

template <typename T>
std::optional<T> SceneComponentMapValue::ParseEntry(const std::string& name) {
  if (!entries.contains(name)) {
    return std::nullopt;
  }
  return entries[name].Parse<T>();
}

template <typename T, size_t D>
std::optional<std::array<T, D>> SceneComponentArrayValue::ParseAsArray() {
  std::array<T, D> output;
  if (entries.size() != D) {
    return std::nullopt;
  }
  for (size_t i = 0; i < D; ++i) {
    if (auto value = entries[i].Parse<T>()) {
      output[i] = *value;
    } else {
      return std::nullopt;
    }
  }
  return output;
}

template <typename T>
std::optional<std::vector<T>> SceneComponentArrayValue::ParseAsList() {
  std::vector<T> output;
  for (auto& entry : entries) {
    if (auto value = entry.Parse<T>()) {
      output.push_back(*value);
    } else {
      return std::nullopt;
    }
  }
  return output;
}

}  // namespace SceneParser
