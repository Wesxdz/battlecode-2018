#pragma once

#include "bc.h"

class Unit
{
public:
	bc_Unit* self;
	uint16_t id;
	bc_UnitType type;
public:
	Unit();
	~Unit();
};

