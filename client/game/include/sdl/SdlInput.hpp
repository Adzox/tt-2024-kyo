#pragma once

#include <Input/BaseInputManager.hpp>

#include <Events/Event.hpp>
#include <Events/EventManager.hpp>

#include <SDL3/SDL_events.h>

#include <memory>
#include <vector>

class SdlInputManager : public Input::BaseInputManager {
 public:
  SdlInputManager(Events::EventManager<size_t>& eventManager);
  void UpdateInput(float dt) override {};

  void AddEvent(SDL_Event* event);

 private:
  std::vector<std::shared_ptr<Events::BaseEvent>> events;
};
