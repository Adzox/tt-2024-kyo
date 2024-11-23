#pragma once

#include "Event.hpp"
#include "IEventListener.hpp"

#include <Log/Log.hpp>

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

namespace Events {

template <typename SubscriberIdType>
  requires std::integral<SubscriberIdType>
class EventManager {
 public:
  template <EventType T>
  using Callable = std::function<void(const T&)>;

  template <EventType T>
  void Subscribe(Callable<T> callable, std::weak_ptr<SubscriberIdType> idToken) {
    if (idToken.expired()) {
      return;
    }
    EventId id = Event<T>::GetEventId();
    if (!subscribers.contains(id)) {
      subscribers[id] = {};
    }
    subscribers[id].push_back({CallbackWrapper<T>(callable), idToken});
  }

  template <EventType T>
  void SubscribeListener(IEventListener<T>& callable, std::weak_ptr<SubscriberIdType> idToken) {
    Subscribe<T>([&callable](const T& event) { callable.OnEvent(event); }, idToken);
  }

  template <EventType T>
  void Unsubscribe(SubscriberIdType subscriberId) {
    EventId id = Event<T>::GetEventId();
    if (subscribers.contains(id)) {
      std::erase_if(subscribers[id], [&subscriberId](auto& entry) {
        if (auto tokenId = entry.idToken.lock()) {
          return *tokenId.get() == subscriberId;
        }
        return true;
      });
    }
  }

  template <EventType T>
  void NotifyNow(T event, std::unordered_set<SubscriberIdType> subscriberIds = {}) {
    EventId id = Event<T>::GetEventId();
    if (!subscribers.contains(id)) {
      subscribers[id] = {};
    }
    Event<T> eventWrapper(event);
    for (auto& [receiver, idToken] : subscribers[id]) {
      if (IsSubscriberTarget(idToken, subscriberIds)) {
        receiver(eventWrapper);
      }
    }
  }

  template <EventType T>
  void PushQueue(T event, std::unordered_set<SubscriberIdType> subscriberIds = {}) {
    EventId id = Event<T>::GetEventId();
    if (!queuedEvents.contains(id)) {
      queuedEvents[id] = {};
    }

    queuedEvents[id].push_back({std::make_shared<Event<T>>(event), subscriberIds});
  }

  void FlushQueue() {
    for (const auto& entry : queuedEvents) {
      const auto& [id, queueEntries] = entry;
      for (auto& queueEntry : queueEntries) {
        const auto& [event, subscriberIds] = queueEntry;
        for (auto& [receiver, idToken] : subscribers[id]) {
          if (IsSubscriberTarget(idToken, subscriberIds)) {
            receiver(*event);
          }
        }
      }
    }
    queuedEvents.clear();
  }

  void Cleanup() {
    for (auto& entry : subscribers) {
      auto& [_, subscriberEntries] = entry;
      std::erase_if(subscriberEntries, [](auto& entry) { return entry.idToken.expired(); });
    }
  }

  template <EventType T>
  struct CallbackWrapper {
    CallbackWrapper(Callable<T> callable) : callable(callable) {}

    void operator()(const BaseEvent& event) {
      callable(static_cast<const Event<T>&>(event).event);
    }

    Callable<T> callable;
  };

 private:
  struct SubscriberEntry {
    Callable<BaseEvent> receiver;
    std::weak_ptr<size_t> idToken;
  };

  struct QueueEntry {
    std::shared_ptr<BaseEvent> eventWrapper;
    std::unordered_set<size_t> subscriberIds;
  };

  bool IsSubscriberTarget(std::weak_ptr<SubscriberIdType> idToken, std::unordered_set<SubscriberIdType> subscriberIds) {
    if (auto subscriberId = idToken.lock()) {
      if (subscriberIds.empty() || (subscriberId && subscriberIds.contains(*subscriberId.get()))) {
        return true;
      }
    }
    return false;
  }

  std::map<EventId, std::vector<SubscriberEntry>> subscribers = {};
  std::map<EventId, std::vector<QueueEntry>> queuedEvents = {};
};

}  // namespace Events
