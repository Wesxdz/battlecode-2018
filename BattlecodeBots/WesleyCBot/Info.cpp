#include "Info.h"

#include "Player.h"

std::map<bc_UnitType, int> Info::numTeamUnits;
std::map<bc_UnitType, int> Info::numEnemyUnits;

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
