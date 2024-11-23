#pragma once

#include "Scenes/SceneComponent.hpp"

#include "Math/Point2d.hpp"
#include "Scenes/WorldApi.hpp"

#include <memory>
#include <vector>

namespace Components {

class Transform : public Scenes::SceneComponent {
 public:
  Transform(Scenes::IWorldApi& world);
  virtual ~Transform() = default;
  
  void SetGlobalPosition(Math::FloatPoint2d pos);
  void SetGlobalScale(Math::FloatPoint2d);
  void SetGlobalRotation(float angle);

  Math::FloatPoint2d GetGlobalPosition();
  Math::FloatPoint2d GetGlobalScale();
  float GetGlobalRotation();

  void SetLocalPosition(Math::FloatPoint2d pos);
  void SetLocalScale(Math::FloatPoint2d);
  void SetLocalRotation(float angle);

  Math::FloatPoint2d GetLocalPosition();
  Math::FloatPoint2d GetLocalScale();
  float GetLocalRotation();

  void OnAttached(Scenes::SceneObject& object) override;
  void OnDetached() override;

  void OnNewParent(size_t parentId) override;
  void OnParentRemoved() override;

  static std::string_view GetType() {
    return "Transform";
  }

 private:

  Math::FloatPoint2d localPosition;
  Math::FloatPoint2d localScale;
  float localRotation;

  std::weak_ptr<Transform> parentTransform;
};

}  // namespace Components
