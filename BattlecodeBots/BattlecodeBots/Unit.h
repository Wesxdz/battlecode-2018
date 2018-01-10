#pragma once

#include "bc.h"

#include "Location.h"

class Unit
{
public:
	bc_Unit* self;
	uint16_t id;
	bc_UnitType type;
private:
	Unit();
public:
	void Init(bc_Unit* unit); // Must be initialized before use
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
};

