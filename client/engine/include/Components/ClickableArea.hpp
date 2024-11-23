#pragma once

#include "Components/Transform.hpp"
#include "Events/IEventListener.hpp"
#include "Input/InputEvents.hpp"
#include "Math/Point2d.hpp"
#include "Scenes/SceneComponent.hpp"

#include <memory>

namespace Components {

struct AreaPressed {
  size_t id;
};

struct AreaReleased {
  size_t id;
};

class ClickableArea : public Scenes::SceneComponent,
                      public Events::IEventListener<Input::MousePress>,
                      public Events::IEventListener<Input::MouseRelease> {
 public:
  ClickableArea(Scenes::IWorldApi& world);
  void SetArea(Math::IntPoint2d size);
  Math::IntPoint2d GetArea();

  void OnAttached(Scenes::SceneObject& owner) override;
  void OnDetached() override;

  void OnEvent(const Input::MousePress& event) override;
  void OnEvent(const Input::MouseRelease& event) override;

  bool Inside(Math::FloatPoint2d point) const;

  static std::string_view GetType() {
    return "ClickableArea";
  }

 private:
  std::weak_ptr<Components::Transform> transform;
  Math::IntPoint2d area;
  std::shared_ptr<size_t> idToken;
};

}  // namespace Components
