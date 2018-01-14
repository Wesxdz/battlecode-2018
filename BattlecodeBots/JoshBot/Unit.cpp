#include "Unit.h"

#include "GameController.h"

namespace units {

	Unit::Unit()
	{
	}

	void Unit::Init(bc_Unit* unit)
	{
		self = bc_Unit_clone(unit);
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

	void Unit::Disintegrate()
	{
		if (Team() == GameController::Team()) {
			bc_GameController_disintegrate_unit(GameController::gc, id);
		}
	}

	bool Unit::Exists() {
		return bc_GameController_can_sense_unit(GameController::gc, id);
	}

	std::vector<std::shared_ptr<units::Unit>> Unit::GetUnitsWithinRange(uint32_t radius) {
		MapLocation mapLoc = Loc().ToMapLocation();
		return GameController::Wrap<units::Unit>(bc_GameController_sense_nearby_units(GameController::gc, mapLoc.self, radius));
	}
	std::vector<std::shared_ptr<units::Unit>> Unit::GetUnitsWithinRangeByTeam(uint32_t radius, bc_Team team) {
		MapLocation mapLoc = Loc().ToMapLocation();
		return GameController::Wrap<units::Unit>(
			bc_GameController_sense_nearby_units_by_team(GameController::gc, mapLoc.self, radius, team));
	}
}

