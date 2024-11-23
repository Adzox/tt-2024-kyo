#pragma once

#include <concepts>

namespace Events {

using EventId = size_t;

template <typename T>
concept EventType = std::semiregular<T>;

class BaseEvent {
 public:
  virtual ~BaseEvent() {}

  static EventId GetNextId() {
    static EventId idCount = 0;
    return idCount++;
  };
};

template <EventType T>
class Event : public BaseEvent {
 public:
  static EventId GetEventId() {
    static EventId id = BaseEvent::GetNextId();
    return id;
  }

  Event(const T& event) : event(event) {}

  T event;
};

}  // namespace Events
