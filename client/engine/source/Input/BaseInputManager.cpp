#include "Input/BaseInputManager.hpp"

namespace Input {

BaseInputManager::BaseInputManager(Events::EventManager<size_t>& eventManager) : eventManager(eventManager) {}

Events::EventManager<size_t>& BaseInputManager::GetEventManager() const {
  return eventManager;
}

}  // namespace Input
