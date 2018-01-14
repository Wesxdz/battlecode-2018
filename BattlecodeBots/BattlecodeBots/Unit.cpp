#include "Unit.h"

#include "GameController.h"
#include "Log.h"

namespace units {

	Unit::Unit()
	{
	}

	Unit::Unit(const Unit& other)
	{
		self = bc_Unit_clone(other.self);
		id = bc_Unit_id(self);
		type = bc_Unit_unit_type(self);
	}

	void Unit::Init(bc_Unit* unit)
	{
		Init(bc_Unit_id(unit));
	}

	void Unit::Init(uint32_t unitId)
	{
		self = bc_GameController_unit(GameController::gc, unitId);
		id = unitId;
		type = bc_Unit_unit_type(self);
		CHECK_ERRORS();
	}

	Unit::~Unit()
	{
		delete_bc_Unit(self);
	}

	uint32_t Unit::Cost()
	{
		log_error(true, "Base unit class has no cost");
		return uint32_t();
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
		CHECK_ERRORS();
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

	void Unit::Disintegrate()
	{
		if (Team() == GameController::Team()) {
			bc_GameController_disintegrate_unit(GameController::gc, id);
		}
	}

}

