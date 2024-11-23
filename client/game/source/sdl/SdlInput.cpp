#include "sdl/SdlInput.hpp"

#include <Input/InputEvents.hpp>

SdlInputManager::SdlInputManager(Events::EventManager<size_t>& eventManager)
    : BaseInputManager(eventManager), events() {}

void SdlInputManager::AddEvent(SDL_Event* event) {
  switch (event->type) {
    case SDL_EVENT_MOUSE_MOTION: {
      auto* motionEvent = (SDL_MouseMotionEvent*)event;
      GetEventManager().NotifyNow(Input::MouseMoved{motionEvent->x, motionEvent->y});
      break;
    }
    case SDL_EVENT_MOUSE_BUTTON_DOWN: {
      auto* buttonEvent = (SDL_MouseButtonEvent*)event;
      GetEventManager().NotifyNow(Input::MousePress{buttonEvent->x, buttonEvent->y,
          buttonEvent->button == 0 ? Input::MouseButton::Left : Input::MouseButton::None});

      break;
    }
    case SDL_EVENT_MOUSE_BUTTON_UP: {
      auto* buttonEvent = (SDL_MouseButtonEvent*)event;
      GetEventManager().NotifyNow(Input::MouseRelease{buttonEvent->x, buttonEvent->y,
          buttonEvent->button == 0 ? Input::MouseButton::Left : Input::MouseButton::None});
      break;
    }
  }
}
