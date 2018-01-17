#ifndef UNIT_POLICIES_H
#define UNIT_POLICIES_H

#include "GameController.h"
#include "Rocket.h"
#include "PolicyOverlord.h"
#include "Pathfind.h"
#include "Worker.h"
#include "Constants.h"
#include "PlayerData.h"

namespace policy {

	float AvoidDamageEval(units::Unit unit) {
		return 0.0f;
	}

	bool AvoidDamageExecute(units::Unit unit) {
		return true;
	}

	float LoadRocketEval(units::Unit unit) {
		float score = 0.0f;
		bc_Location* location = bc_Unit_location(unit.self);
		bool isOnMap = bc_Location_is_on_map(location);
		if (!isOnMap) return 0.0f;
		bc_MapLocation* mapLocation = bc_Location_map_location(location);
		delete_bc_Location(location);
		bc_VecUnit* nearbyRockets = bc_GameController_sense_nearby_units_by_type(GameController::gc, mapLocation, 1, Rocket);
		for (uintptr_t i = 0; i < bc_VecUnit_len(nearbyRockets); i++) {
			auto rocket = bc_VecUnit_index(nearbyRockets, 0);
			if (bc_Unit_team(rocket) == GameController::Team() && bc_Unit_structure_is_built(rocket)) {
				PolicyOverlord::storeUnit = bc_Unit_clone(rocket);
				score += 40.0f;
			}
			delete_bc_Unit(rocket);
		}
		delete_bc_VecUnit(nearbyRockets);
		delete_bc_MapLocation(mapLocation);
		return score;
	}

	bool LoadRocketExecute(units::Unit unit) {
		units::Rocket rocket = bc_Unit_clone(PolicyOverlord::storeUnit.self);
		units::Robot robot = bc_Unit_clone(robot.self);
		if (rocket.CanLoad(robot)) {
			rocket.Load(robot);
			return true;
		}
		return false;
	}

	float WanderEvaluate(units::Unit unit) {
		units::Robot robot{ bc_Unit_clone(unit.self) };
		if (robot.IsMoveReady()) {
			return 0.1f;
		}
		return 0.0f;
	}

	bool WanderExecute(units::Unit unit) {
		units::Robot robot{ bc_Unit_clone(unit.self) };
		return Pathfind::MoveRandom(robot);
	}

	float WorkerHarvestKarboniteEvaluate(units::Unit unit) {
		units::Worker worker = bc_Unit_clone(unit.self);
		if (worker.HasActed()) {
			return 0.0f;
		}
		for (bc_Direction direction : constants::directions_all) {
			if (worker.CanHarvest(direction)) {
				PolicyOverlord::storeDirection = direction;
				return 1.0f;
			}
		}
		return 0.0f;
	}

	bool WorkerHarvestKarboniteExecute(units::Unit unit) {
		units::Worker worker = bc_Unit_clone(unit.self);
		worker.Harvest(PolicyOverlord::storeDirection);
		return true;
	}

	//float WorkerSeekKarboniteEvaluate(units::Unit unit) {
	//	units::Worker worker = bc_Unit_clone(unit.self);
	//	if (!worker.IsMoveReady()) return 0.0f;
	//}

	//bool WorkerSeekKarboniteExecute(units::Unit unit) {
	//	units::Worker worker = bc_Unit_clone(unit.self);

	//}

	float WorkerBlueprintFactoryEvaluate(units::Unit unit) {
		units::Worker worker = bc_Unit_clone(unit.self);
		if (worker.HasActed()) return 0.0f;
		if (GameController::Karbonite() >= bc_UnitType_blueprint_cost(Factory) &&
			PlayerData::pd->desiredUnitCounts[Factory] > PlayerData::pd->teamUnitCounts[Factory]) {
			for (bc_Direction direction : constants::directions_adjacent) {
				if (worker.CanBlueprint(Factory, direction)) {
					PolicyOverlord::storeDirection = direction;
					return 3.0f;
				}
			}
		}
		return 0.0f;
	}

	bool WorkerBlueprintFactoryExecute(units::Unit unit) {
		units::Worker worker = bc_Unit_clone(unit.self);
		worker.Blueprint(Factory, PolicyOverlord::storeDirection);
		return true;
	}

	float WorkerBlueprintRocketEvaluate(units::Unit unit) {
		units::Worker worker = bc_Unit_clone(unit.self);
		if (worker.HasActed()) return 0.0f;
		if (GameController::Karbonite() >= bc_UnitType_blueprint_cost(Factory) &&
			PlayerData::pd->desiredUnitCounts[Factory] > PlayerData::pd->teamUnitCounts[Factory]) {
			for (bc_Direction direction : constants::directions_adjacent) {
				if (worker.CanBlueprint(Factory, direction)) {
					PolicyOverlord::storeDirection = direction;
					return 2.0f;
				}
			}
		}
		return 0.0f;
	}

	bool WorkerBlueprintRocketExecute(units::Unit unit) {
		units::Worker worker = bc_Unit_clone(unit.self);
		worker.Blueprint(Rocket, PolicyOverlord::storeDirection);
		return true;
	}


}

#endif
