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
void events::Events::notifyTileEventObservers(const ObjectIdentifier& objectId, const std::string& eventName, std::vector<core::GameTile>& tiles, int firstLayerMoveSize) const
{
	for (const auto [id, obs] : m_observers) {
		if (id == objectId)
			obs->handleTileEvent(eventName, tiles, firstLayerMoveSize);
	}
}
