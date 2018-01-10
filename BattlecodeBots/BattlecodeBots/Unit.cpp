#include "Unit.h"

Unit::Unit()
{
}

void Unit::Init(bc_Unit* unit)
{
	self = unit;
	id = bc_Unit_id(self);
	type = bc_Unit_unit_type(self);
}

Unit::~Unit()
{
	delete_bc_Unit(self);
}

uint32_t Unit::Value()
{
	return bc_UnitType_value(type);
}

bc_Team Unit::Team()
{
	return bc_Unit_team(self);
}

uintptr_t Unit::ResearchLevel()
{
	return bc_Unit_research_level(self);
}

Location Unit::Loc()
{
	Location location;
	location.self = bc_Unit_location(self);
	return location;
}

uint32_t Unit::MaxHealth()
{
	return bc_Unit_max_health(self);
}

uint32_t Unit::Health()
{
	return bc_Unit_health(self);
}

uint32_t Unit::VisionRange()
{
	return bc_Unit_vision_range(self);
}
