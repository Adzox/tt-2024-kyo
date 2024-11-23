#pragma once

#include "Events/EventManager.hpp"

namespace Input {

class BaseInputManager {
 public:
  BaseInputManager(Events::EventManager<size_t>& eventManager);
  virtual ~BaseInputManager() = default;

  virtual void UpdateInput(float dt) = 0;

  Events::EventManager<size_t>& GetEventManager() const;

 private:
  Events::EventManager<size_t>& eventManager;
};

}  // namespace Input
