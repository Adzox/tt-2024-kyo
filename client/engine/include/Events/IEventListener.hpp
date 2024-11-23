#pragma once

#include "Event.hpp"

namespace Events {

template <EventType T>
class IEventListener {
 public:
  virtual ~IEventListener() = default;
  virtual void OnEvent(const T& event) = 0;
};

}  // namespace Events
