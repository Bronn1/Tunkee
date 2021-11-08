#pragma once

#include "data_types.h"

#include <string>
#include <vector>
#include <memory>
#include <map>

namespace core {
	struct GameTile;
}

namespace events {


	class Observer
	{
	public:
		//virtual void handleEvent( const std::string& eventName) = 0;
		/** should be notified about any event connected with tiles, last parameter is needed only for 
		* 'getMoveArea' query exclusively 
		*/
		virtual void handleTileEvent(const std::string& eventName, const std::vector<core::GameTile>& tiles, int firstLayerMoveSize = 0) = 0;
		virtual ~Observer() = default;
	};

	using ObserverPtr = std::shared_ptr<Observer>;

	/**
	* This is the class is a part of Observer pattern,
	* notifies the observers and maintains a list of them.
	*/
	class Events
	{
	public:
		void addObserver(const ObjectIdentifier& objectId, ObserverPtr& obs);
		void removeObserver(const ObjectIdentifier& objectId);
		void notifyTileEventObservers(const ObjectIdentifier& objectId, const std::string& eventName, std::vector<core::GameTile>& tiles, int firstLayerMoveSize = 0) const;

	private:
		std::string m_name;
		std::map < ObjectIdentifier, ObserverPtr, Comparator<ObjectIdentifier>> m_observers;
	};
}

