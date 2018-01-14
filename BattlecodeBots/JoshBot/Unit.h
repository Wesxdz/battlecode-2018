#ifndef UNIT_H 
#define UNIT_H

#include "bc.h"

#include "GameController.h"
#include "Location.h"
#include <vector>
#include <memory>

namespace units {

	class Unit
	{
	public:
		bc_Unit* self = nullptr;
		uint16_t id;
		bc_UnitType type;
	public:
		Unit();
		void Init(bc_Unit* unit); // Must be initialized before use!
		~Unit();
		
		/*
		@return The cost to blueprint or build in factory
		*/
		virtual uint32_t Cost() = 0;
		/*
		@return The value of a unit, as relevant to tiebreakers.
		*/
		uint32_t Value();
		/*
		The team the unit belongs to.
		*/
		bc_Team Team();
		/*
		@return The current research level
		*/
		uintptr_t ResearchLevel();
		/*
		@return Current Location
		*/
		Location Loc();
		/*
		@return Maximum health
		*/
		uint32_t MaxHealth();
		/*
		@return Current health
		*/
		uint32_t Health();
		/*
		@return Vision range
		*/
		uint32_t VisionRange();
		/*
		;)
		Disintegrates the unit and removes it from the map. If the unit is a factory or a rocket, also disintegrates any units garrisoned inside it.
		*/
		void Disintegrate();

		bool Exists();
	
		std::vector<std::shared_ptr<units::Unit>> GetUnitsWithinRange(uint32_t radius);
		std::vector<std::shared_ptr<units::Unit>> GetUnitsWithinRangeByTeam(uint32_t radius, bc_Team team);

		template<class T>
		std::vector<std::shared_ptr<T>> GetUnitsWithinRangeByType(uint32_t radius, bc_UnitType type);

	};

	template<class T>
	inline std::vector<std::shared_ptr<T>> Unit::GetUnitsWithinRangeByType(uint32_t radius, bc_UnitType type) {
		MapLocation mapLoc = Loc().ToMapLocation();
		return GameController::Wrap<T>(
			bc_GameController_sense_nearby_units_by_type(GameController::gc, mapLoc.self, radius, type));
	}

}

#endif