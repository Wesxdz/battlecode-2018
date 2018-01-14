#include "Info.h"

#include "Player.h"
#include "Section.h"
#include "MapUtil.h"

std::map<bc_UnitType, int> Info::numTeamUnits;
std::map<bc_UnitType, int> Info::numEnemyUnits;

Info::Info()
{
	MapUtil::earthLocations = MapUtil::AllLocations(Player::earth);
	MapUtil::earthPassableLocations = MapUtil::FilteredLocations(MapUtil::earthLocations, [](bc_MapLocation* location) {
		return bc_PlanetMap_is_passable_terrain_at(Player::earth, location);
	});
	MapUtil::marsLocations = MapUtil::AllLocations(Player::mars);
	MapUtil::marsPassableLocations = MapUtil::FilteredLocations(MapUtil::marsLocations, [](bc_MapLocation* location) {
		return bc_PlanetMap_is_passable_terrain_at(Player::mars, location);
	});
	Section::earthSections = Section::GenSections(MapUtil::earthPassableLocations);
	Section::marsSections = Section::GenSections(MapUtil::marsPassableLocations);
}

Info::~Info()
{
}

void Info::Update()
{
	Info::numEnemyUnits.clear();
	Info::numTeamUnits.clear();
	bc_VecUnit* allUnits = bc_GameController_units(Player::gc);
	for (uintptr_t i = 0; i < bc_VecUnit_len(allUnits); i++) {
		bc_Unit* unit = bc_VecUnit_index(allUnits, i);
		bc_UnitType type = bc_Unit_unit_type(unit);
		bc_Team team = bc_Unit_team(unit);
		if (team == Player::team) {
			Info::numTeamUnits[type]++;
		}
		else {
			Info::numEnemyUnits[type]++;
		}
		delete_bc_Unit(unit);
	}
	delete_bc_VecUnit(allUnits);
}
