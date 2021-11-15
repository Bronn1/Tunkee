#include "events.h"

#include <ranges>

void events::Events::addObserver(const ObjectIdentifier& objectId, ObserverPtr& obs)
{
	m_observers.insert({ objectId, obs });
}

void events::Events::removeObserver(const ObjectIdentifier& objectId)
{
	m_observers.erase(objectId);
}

// TODO try catch maybe
void events::Events::notifyObservers(const ObjectIdentifier& objectId, const std::string& eventName) const
{
	for (const auto [id, obs] : m_observers) {
		if (id == objectId)
			obs->update(eventName);
	}
}
