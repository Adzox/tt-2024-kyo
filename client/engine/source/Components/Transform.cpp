#include "Components/Transform.hpp"

#include "Scenes/SceneObject.hpp"

#include <memory>

namespace Components {

Transform::Transform(Scenes::IWorldApi& worldApi)
    : SceneComponent(worldApi), localPosition(), localScale(1, 1), localRotation(0) {}

void Transform::SetGlobalPosition(Math::FloatPoint2d pos) {
  Math::FloatPoint2d parentPosition{};
  if (auto parent = parentTransform.lock()) {
    parentPosition = parent->GetGlobalPosition();
  }
  localPosition = parentPosition - pos;
};

void Transform::SetGlobalScale(Math::FloatPoint2d scale) {
  Math::FloatPoint2d parentScale{};
  if (auto parent = parentTransform.lock()) {
    parentScale = parent->GetGlobalScale();
  }
  localScale = parentScale * scale;
};

void Transform::SetGlobalRotation(float angle) {
  float parentRotation{};
  if (auto parent = parentTransform.lock()) {
    parentRotation = parent->GetGlobalRotation();
  }
  localRotation = parentRotation + angle;
};

Math::FloatPoint2d Transform::GetGlobalPosition() {
  Math::FloatPoint2d parentPosition{};
  if (auto parent = parentTransform.lock()) {
    parentPosition = parent->GetGlobalPosition();
  }
  return parentPosition + localPosition;
}

Math::FloatPoint2d Transform::GetGlobalScale() {
  Math::FloatPoint2d parentScale{1, 1};
  if (auto parent = parentTransform.lock()) {
    parentScale = parent->GetGlobalScale();
  }
  return parentScale * localScale;
}

float Transform::GetGlobalRotation() {
  float parentRotation{};
  if (auto parent = parentTransform.lock()) {
    parentRotation = parent->GetGlobalRotation();
  }
  return parentRotation + localRotation;
}

void Transform::SetLocalPosition(Math::FloatPoint2d pos) {
  localPosition = pos;
}

void Transform::SetLocalScale(Math::FloatPoint2d scale) {
  localScale = scale;
}

void Transform::SetLocalRotation(float angle) {
  localRotation = angle;
}

Math::FloatPoint2d Transform::GetLocalPosition() {
  return localPosition;
}

Math::FloatPoint2d Transform::GetLocalScale() {
  return localScale;
}

float Transform::GetLocalRotation() {
  return localRotation;
}

void Transform::OnAttached(Scenes::SceneObject& object) {
  if (auto parent = object.GetParent().lock()) {
    parentTransform = parent->GetComponent<Transform>();
  }
};

void Transform::OnDetached() {
  parentTransform.reset();
};

void Transform::OnNewParent(size_t parentId) {
  if (auto parent = GetWorldApi().GetSceneObject(parentId).lock()) {
    parentTransform = parent->GetComponent<Transform>();
  }
};

void Transform::OnParentRemoved() {
  parentTransform.reset();
};

}  // namespace Components
