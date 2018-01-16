#ifndef UNIT_H 
#define UNIT_H

#include "bc.h"

#include "Location.h"
#include "Log.h"

namespace units {

	class Unit
	{
	public:
		bc_Unit * self;
		uint16_t id;
		bc_UnitType type;
	public:
		// Use bc_Unit_clone if you don't want your #unit to be deleted
		Unit(bc_Unit* unit);
		Unit(const Unit& other);
		~Unit();
		/*
		@return The cost to blueprint or build in factory
		*/
		virtual uint32_t Cost();
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
		Disintegrates the unit and removes it from the map. If the unit is a factory or a rocket, also disintegrates any units garrisoned inside it.
		*/
		void Disintegrate();

	};

}

#endif