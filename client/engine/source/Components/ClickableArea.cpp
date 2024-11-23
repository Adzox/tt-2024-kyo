#include "Components/ClickableArea.hpp"

#include "Math/Utils.hpp"
#include "Scenes/SceneObject.hpp"

namespace Components {

ClickableArea::ClickableArea(Scenes::IWorldApi& world) : SceneComponent(world) {}

void ClickableArea::OnAttached(Scenes::SceneObject& owner) {
  Scenes::SceneComponent::OnAttached(owner);
  idToken = std::make_shared<size_t>(owner.GetId());
  transform = owner.GetComponent<Transform>();
  GetWorldApi().GetEventManager().SubscribeListener<Input::MousePress>(*this, idToken);
  GetWorldApi().GetEventManager().SubscribeListener<Input::MouseRelease>(*this, idToken);
}

void ClickableArea::OnEvent(const Input::MouseRelease& press) {
  if (!GetOwnerId().has_value()) {
    return;
  }
  if (Inside({press.x, press.y})) {
    GetWorldApi().GetEventManager().PushQueue(AreaReleased{*GetOwnerId()});
  }
}

void ClickableArea::OnEvent(const Input::MousePress& press) {
  if (!GetOwnerId().has_value()) {
    return;
  }
  if (Inside({press.x, press.y})) {
    GetWorldApi().GetEventManager().PushQueue(AreaPressed{*GetOwnerId()});
  }
}

void ClickableArea::OnDetached() {
  idToken.reset();
}
void ClickableArea::SetArea(Math::IntPoint2d newArea) {
  area = newArea;
}

Math::IntPoint2d ClickableArea::GetArea() {
  return area;
};

bool ClickableArea::Inside(Math::FloatPoint2d point) const {
  auto t = transform.lock();
  if (!t) {
    return false;
  }
  Math::FloatPoint2d pos = t->GetGlobalPosition();
  Math::FloatPoint2d scale = t->GetGlobalScale();
  Math::FloatPoint2d size = Math::FloatPoint2d(area) * scale;
  float rotation = t->GetGlobalRotation();

  Math::FloatPoint2d testPoint;
  if (Math::ApproximatelyEqual(rotation, 0, 0.0001)) {
    testPoint = point;
  } else {
    Math::FloatPoint2d center = t->GetGlobalPosition() + Math::FloatPoint2d(0.5, 0.5) * size;
    Math::FloatPoint2d centeredPoint = point - center;
    testPoint = centeredPoint.Rotated(-rotation);
  }

  Math::FloatPoint2d topLeft = pos;
  Math::FloatPoint2d topRight = pos + Math::FloatPoint2d(1, 0) * size;
  Math::FloatPoint2d bottomLeft = pos + Math::FloatPoint2d(0, 1) * size;
  Math::FloatPoint2d bottomRight = pos + Math::FloatPoint2d(1, 1) * size;

  if (testPoint.GetX() >= topLeft.GetX() && testPoint.GetX() <= topRight.GetX()) {
    if (testPoint.GetY() >= topLeft.GetY() && testPoint.GetY() <= bottomLeft.GetY()) {
      return true;
    }
  }
  return false;
};

}  // namespace Components
