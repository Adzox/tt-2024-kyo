#pragma once

#include "Scenes/IRenderable.hpp"
#include "Scenes/IUpdateable.hpp"
#include "Scenes/SceneComponent.hpp"
#include "Scenes/WorldApi.hpp"

#include <Events/Event.hpp>

#include <concepts>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace Scenes {

class SceneObject {
 public:
  SceneObject(IWorldApi& worldApi,
              size_t id,
              std::string_view name,
              std::set<std::string> tags = {},
              std::optional<size_t> parentId = std::nullopt);
  SceneObject(const SceneObject&) = delete;
  SceneObject(const SceneObject&&) = delete;

  void SetEnabled(bool enabled);
  bool IsEnabled();

  void SetParent(size_t parentId);
  std::weak_ptr<SceneObject> GetParent();
  void RemoveFromParent();

  void AddChild(std::shared_ptr<SceneObject>&& child);
  std::weak_ptr<SceneObject> GetChild(size_t index);
  std::vector<std::weak_ptr<SceneObject>> GetChildren();
  void RemoveChild(size_t id);

  void AddComponent(std::string_view type, std::unique_ptr<SceneComponent> component);

  template <Component T>
  void AddComponent(std::unique_ptr<T> component) {
    std::string type = std::string(T::GetType());
    if (!components.contains(type)) {
      components.emplace(type, nullptr);
    } else {
      auto old = components[type];
      old->OnDetached();
    }
    components[type] = component.release();
    components[type]->OnAttached(*this);
  }

  bool HasComponentNamed(std::string_view name) const;

  template <Component T>
  std::weak_ptr<T> GetComponent() {
    if (components.contains(T::GetType())) {
      return std::dynamic_pointer_cast<T>(components[std::string(T::GetType())]);
    } else {
      return {};
    }
  }

  template <Component T>
  bool HasComponent() const {
    return components.contains(T::GetType());
  }

  template <Component T>
  void RemoveComponent() {
    if (components.contains(T::GetType())) {
      std::string type = std::string(T::GetType());
      auto component = components[type];
      components.erase(type);
      component.OnRemoved();
    }
  }

  template <Events::EventType T>
  void NotifyComponents(const T& event, bool recursive = false) {
    worldApi.GetEventManager().NotifyNow(event, {id});
    if (!recursive) {
      return;
    }
    for (auto child : children) {
      if (child) {
        child->NotifyComponents(event, true);
      }
    }
  }

  std::vector<std::reference_wrapper<Scenes::IUpdateable>> GetUpdateables();
  std::vector<std::reference_wrapper<Scenes::IRenderable>> GetRenderables();

  void RequestDestruction();
  bool DestructionRequested();
  size_t GetId() const;
  void SetName(std::string name);
  std::string_view GetName() const;
  void AddTag(std::string_view tag);
  bool HasTag(std::string_view tag) const;
  void RemoveTag(std::string_view tag);

 private:
  IWorldApi& worldApi;

  size_t id;
  std::string name;
  std::set<std::string> tags;

  bool enabled = true;
  bool destructionRequested = false;

  std::optional<size_t> parentId;
  std::vector<std::shared_ptr<SceneObject>> children;
  std::map<std::string, std::shared_ptr<SceneComponent>, std::less<>> components;
};

}  // namespace Scenes
