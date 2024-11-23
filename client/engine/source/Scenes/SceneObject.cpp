#include "Scenes/SceneObject.hpp"
#include "Scenes/SceneComponent.hpp"
#include "Scenes/WorldApi.hpp"

#include <algorithm>
#include <ranges>

namespace Scenes {

SceneObject::SceneObject(IWorldApi& worldApi,
                         size_t id,
                         std::string_view name,
                         std::set<std::string> tags,
                         std::optional<size_t> parentId)
    : worldApi(worldApi), id(id), name(name), tags(tags), enabled(true), parentId(parentId), children(), components() {}

void SceneObject::SetParent(size_t newParentId) {
  RemoveFromParent();
  if (auto newParentObject = worldApi.GetSceneObject(newParentId).lock()) {
    newParentObject->AddChild(worldApi.GetSceneObject(id).lock());
    for (auto& [_, component] : components) {
      component->OnNewParent(newParentId);
    }
  }
  parentId = newParentId;
}

void SceneObject::RemoveFromParent() {
  if (parentId.has_value()) {
    if (auto currentParent = worldApi.GetSceneObject(parentId.value()).lock()) {
      currentParent->RemoveChild(id);
      for (auto& [_, component] : components) {
        component->OnParentRemoved();
      }
    }
    parentId = std::nullopt;
  }
}

std::weak_ptr<SceneObject> SceneObject::GetParent() {
  return parentId.has_value() ? worldApi.GetSceneObject(parentId.value()) : std::weak_ptr<SceneObject>();
};

void SceneObject::AddChild(std::shared_ptr<SceneObject>&& child) {
  if (!child) {
    return;
  }
  children.emplace_back(std::move(child));
  children.back()->parentId = id;
  for (auto& [_, component] : components) {
    component->OnChildAdded(children.back());
  }
}

std::weak_ptr<SceneObject> SceneObject::GetChild(size_t index) {
  if (index < children.size()) {
    return std::weak_ptr<SceneObject>(children[index]);
  }
  return {};
}

std::vector<std::weak_ptr<SceneObject>> SceneObject::GetChildren() {
  return children | std::views::transform([](auto& child) { return std::weak_ptr<SceneObject>(child); }) |
         std::ranges::to<std::vector<std::weak_ptr<SceneObject>>>();
}

void SceneObject::RemoveChild(size_t id) {
  auto toBeRemoved = std::find_if(children.begin(), children.end(), [id](auto& child) { return child->id == id; });
  if (toBeRemoved == children.end()) {
    return;
  }
  toBeRemoved->get()->parentId = std::nullopt;
  for (auto& [_, component] : components) {
    component->OnChildRemoved(std::shared_ptr<SceneObject>(*toBeRemoved));
  }
}

void SceneObject::AddComponent(std::string_view type, std::unique_ptr<SceneComponent> component) {
  if (!component) {
    return;
  }
  std::string typeString(type);
  if (!components.contains(type)) {
    components.emplace(typeString, nullptr);
  } else {
    auto old = components[typeString];
    old->OnDetached();
  }
  components[typeString] = std::shared_ptr<SceneComponent>(component.release());
  components[typeString]->OnAttached(*this);
}

std::vector<std::reference_wrapper<Scenes::IUpdateable>> SceneObject::GetUpdateables() {
  if (!enabled) {
    return {};
  }

  std::vector<std::reference_wrapper<Scenes::IUpdateable>> res{};
  for (auto& [_, component] : components) {
    if (auto a = dynamic_pointer_cast<Scenes::IUpdateable>(component)) {
      if (a) {
        res.emplace_back(*a.get());
      }
    }
  }
  return res;
}

std::vector<std::reference_wrapper<Scenes::IRenderable>> SceneObject::GetRenderables() {
  if (!enabled) {
    return {};
  }

  std::vector<std::reference_wrapper<Scenes::IRenderable>> res{};
  for (auto& [_, component] : components) {
    if (auto a = dynamic_pointer_cast<Scenes::IRenderable>(component)) {
      if (a) {
        res.emplace_back(*a.get());
      }
    }
  }
  return res;
}

bool SceneObject::HasComponentNamed(std::string_view name) const {
  return components.contains(name);
}

void SceneObject::RequestDestruction() {
  destructionRequested = true;
}
bool SceneObject::DestructionRequested() {
  return destructionRequested;
}

size_t SceneObject::GetId() const {
  return id;
}

void SceneObject::SetName(std::string newName) {
  name = newName;
}

std::string_view SceneObject::GetName() const {
  return name;
}

void SceneObject::AddTag(std::string_view tag) {
  tags.insert(std::string(tag));
}

bool SceneObject::HasTag(std::string_view tag) const {
  return std::find(tags.cbegin(), tags.cend(), tag) != tags.cend();
}

void SceneObject::RemoveTag(std::string_view tag) {
  std::erase_if(tags, [tag](const auto& t) { return tag == t; });
}

void SceneObject::SetEnabled(bool newEnabledState) {
  if (newEnabledState == enabled) {
    return;
  }

  if (parentId.has_value()) {
    if (auto parentObject = worldApi.GetSceneObject(parentId.value()).lock()) {
      if (!parentObject->IsEnabled() && newEnabledState) {
        return;
      }
    }
  }

  enabled = newEnabledState;
  for (auto& [_, component] : components) {
    if (enabled) {
      component->OnEnabled();
    } else {
      component->OnDisabled();
    }
  }
  for (auto& child : children) {
    child->SetEnabled(newEnabledState);
  }
}

bool SceneObject::IsEnabled() {
  return enabled;
}

}  // namespace Scenes
