#pragma once

#include "data_types.h"
#include "game_command.h"

#include <string>
#include <vector>
#include <variant>
//#include <ranges>

namespace core {
	struct GameTile;
}

namespace events {

	template<typename Notifier>
	class Observer
	{
	public:
		virtual void newUnitSelected(const UnitSelectedInfo& unitInfo) = 0;
		virtual void informationMsgRecieved(const GameInfoMessage& msgInfo) = 0;
		virtual void moveAreaRecieved(const MoveAreaInfo& moveArea) = 0;
		virtual void moveUnitRecieved(const MoveUnitInfo& moveUnit) = 0;
		void operator ()(const UnitSelectedInfo& unitInfo) { newUnitSelected(unitInfo); }
		void operator ()(const GameInfoMessage& msgInfo) { informationMsgRecieved(msgInfo); }
		void operator ()(const MoveAreaInfo& moveArea) { moveAreaRecieved(moveArea); }
		void operator ()(const MoveUnitInfo& moveUnit) { moveUnitRecieved(moveUnit); }
		void operator ()(std::monostate) { }
		virtual ~Observer() = default;
	};


	/**
	* This is the class is a part of Observer pattern,
	* notifies the observers and maintains a list of them.
	*/
	template<typename Notifier>
	class Events
	{
	public:
		using GameInfoVariant = std::variant<UnitSelectedInfo, GameInfoMessage, MoveAreaInfo, MoveUnitInfo>;
		void subscribe(Observer<Notifier>* obs)
		{
			m_observers.push_back(obs);
		}
		void unsubscribe(Observer<Notifier>* obs)
		{
			std::erase(m_observers, obs);
		}
	protected:
		void notify(const GameInfoVariant& info)
		{
			for (auto observer : m_observers)
			{
				std::visit(*observer, info);
				//observer->newUnitSelected(unitId, eventName);
			}
		}

	private:
		std::string m_name;
		std::vector<Observer<Notifier>*> m_observers;
		//std::map < UnitIdentifier, ObserverPtr, Comparator<UnitIdentifier>> m_observers;
	};
}