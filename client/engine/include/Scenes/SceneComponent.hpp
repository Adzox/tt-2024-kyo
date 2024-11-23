#pragma once

#include <concepts>
#include <functional>
#include <memory>
#include <optional>
#include <string_view>

namespace Scenes {

class SceneObject;
class IWorldApi;

template<typename T>
concept Component = requires(T /* t */) {
  { T::GetType() } -> std::convertible_to<std::string_view>;
};

class SceneComponent {
 public:
  SceneComponent(IWorldApi& world);
  virtual ~SceneComponent() = default;

  virtual void OnAttached(SceneObject& object);
  virtual void OnDetached();

  virtual void OnEnabled() {}
  virtual void OnDisabled() {}

  virtual void OnNewParent(size_t parentId) {}
  virtual void OnParentRemoved() {}

  virtual void OnChildAdded(std::shared_ptr<SceneObject> child) {}
  virtual void OnChildRemoved(std::shared_ptr<SceneObject> child) {}

  std::optional<size_t> GetOwnerId();
  std::weak_ptr<SceneObject> GetOwner();
  IWorldApi& GetWorldApi();
  
 private:
  IWorldApi& worldApi;
  std::optional<size_t> ownerId;
};

}  // namespace Scenes
