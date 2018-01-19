#ifndef UNIT_POLICIES_H
#define UNIT_POLICIES_H

#include "GameController.h"
#include "Rocket.h"
#include "PolicyOverlord.h"
#include "Pathfind.h"
#include "Worker.h"
#include "Constants.h"
#include "PlayerData.h"
#include "CombatOverlord.h"
#include "Utility.h"
#include <algorithm>
#include <iostream>
#include "Factory.h"
#include "BuilderOverlord.h"

namespace policy {

	float AvoidDamageEval(units::Unit unit) {
		units::Robot robot{ bc_Unit_clone(unit.self) };
		if (!robot.IsMoveReady()) return 0.0f;
		MapLocation location = robot.Loc().ToMapLocation();
		bc_Team enemyTeam = Utility::GetOtherTeam(robot.Team());
		uint32_t currentDanger = CombatOverlord::Danger(location, enemyTeam);
		if (currentDanger == 0) return 0.0f;
		auto run = constants::directions_adjacent;
		std::remove_if(run.begin(), run.end(), [&robot](bc_Direction direction) {
			return !robot.CanMove(direction);
		});
		auto escape = std::min_element(run.begin(), run.end(), [&location, &enemyTeam](bc_Direction a, bc_Direction b) {
			return CombatOverlord::Danger(MapLocation::Neighbor(location, a), enemyTeam) < CombatOverlord::Danger(MapLocation::Neighbor(location, b), enemyTeam);
		});
		if (escape != run.end()) {
			float dangerReduction = (float)currentDanger - CombatOverlord::Danger(MapLocation::Neighbor(location, *escape), enemyTeam);
			PolicyOverlord::storeDirection = *escape;
			return dangerReduction / 10;
		}
		return 0.0f;
	}

	bool AvoidDamageExecute(units::Unit unit) {
		units::Robot robot{ bc_Unit_clone(unit.self) };
		robot.Move(PolicyOverlord::storeDirection);
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

	float WorkerSeekKarboniteEvaluate(units::Unit unit) {
		units::Worker worker = bc_Unit_clone(unit.self);
		if (!worker.IsMoveReady()) return 0.0f;
		return 0.0f;
	}

	bool WorkerSeekKarboniteExecute(units::Unit unit) {
		units::Worker worker = bc_Unit_clone(unit.self);
		worker.Move(PolicyOverlord::storeDirection);
		return true;
	}

	float WorkerBlueprintEvaluate(units::Unit unit) {
		units::Worker worker = bc_Unit_clone(unit.self);
		if (worker.HasActed()) return 0.0f;
		bc_UnitType priority = PolicyOverlord::HighestPriority();
		if (!(priority == Factory || priority == Rocket)) return 0.0f;
		if (GameController::Karbonite() < bc_UnitType_blueprint_cost(priority)) return 0.0f;
		for (bc_Direction direction : constants::directions_adjacent) {
			if (worker.CanBlueprint(priority, direction)) {
				PolicyOverlord::storeDirection = direction;
				PolicyOverlord::storeUnit.type = priority;
				return 3.0f;
			}
		}
		return 0.0f;
	}

	bool WorkerBlueprintExecute(units::Unit unit) {
		units::Worker worker = bc_Unit_clone(unit.self);
		worker.Blueprint(PolicyOverlord::storeUnit.type, PolicyOverlord::storeDirection);
		MapLocation buildLocation = MapLocation::Neighbor(worker.Loc().ToMapLocation(), PolicyOverlord::storeDirection);
		units::Structure build = buildLocation.Occupant();
		BuilderOverlord::buildProjects[build.id].push_back(worker.id);
		return true;
	}

	float WorkerReplicateEvaluate(units::Unit unit) {
		units::Worker worker = bc_Unit_clone(unit.self);
		if (worker.HasActed()) return 0.0f;
		if (GameController::Karbonite() < worker.ReplicateCost()) return 0.0f;
		if (PolicyOverlord::HighestPriority() != Worker) return 0.0f;
		for (bc_Direction direction : constants::directions_adjacent) {
			if (worker.CanReplicate(direction)) {
				PolicyOverlord::storeDirection = direction;
				return 5.0f;
			}
		}
		return 0.0f;
	}

	bool WorkerReplicateExecute(units::Unit unit) {
		units::Worker worker = bc_Unit_clone(unit.self);
		worker.Replicate(PolicyOverlord::storeDirection);
		return true;
	}

	float WorkerBuildEvaluate(units::Unit unit) {
		units::Worker worker = bc_Unit_clone(unit.self);
		if (worker.HasActed()) return 0.0f;
		for (auto& project : BuilderOverlord::buildProjects) {
			auto self = std::find(project.second.begin(), project.second.begin(), worker.id);
			if (self != project.second.end()) {
				if (bc_GameController_can_build(GameController::gc, worker.id, project.first)) {
					PolicyOverlord::storeUnit.id = project.first;
					return 10.0f;
				}
			}
		}
		return 0.0f;
	}

	bool WorkerBuildExecute(units::Unit unit) {
		units::Worker worker = bc_Unit_clone(unit.self);
		units::Structure build = bc_GameController_unit(GameController::gc, PolicyOverlord::storeUnit.id);
		worker.Build(build);
		if (build.IsBuilt()) {
			BuilderOverlord::buildProjects.erase(BuilderOverlord::buildProjects.find(build.id)); //Remove build project
		}
		return true;
	}

	float WorkerSeekBuildEvaluate(units::Unit unit) { // TODO Finish
		units::Worker worker = bc_Unit_clone(unit.self);
		if (!worker.IsMoveReady()) return 0.0f;
		MapLocation workerLocation = worker.Loc().ToMapLocation();
		return 0.0f;
	}

	bool WorkerSeekBuildExecute(units::Unit unit) {
		return false;
	}

	float FactoryProduceEvaluate(units::Unit unit) {
		units::Factory factory = bc_Unit_clone(unit.self);
		if (!factory.IsBuilt()) return 0.0f;
		bc_UnitType priority = PolicyOverlord::HighestPriority();
		if (!Utility::IsRobot(priority)) return 0.0f;
		if (priority == Worker && PlayerData::pd->teamUnitCounts[Worker] > 2) return 0.0f;
		if (factory.CanProduce(priority)) {
			PolicyOverlord::storeUnit.type = priority;
			return PlayerData::pd->unitPriority[priority];
		}
		return 0.0f;
	}

	bool FactoryProduceExecute(units::Unit unit) {
		units::Factory factory = bc_Unit_clone(unit.self);
		factory.Produce(PolicyOverlord::storeUnit.type);
		return true;
	}

	float FactoryUnloadEvaluate(units::Unit unit) {
		units::Factory factory = bc_Unit_clone(unit.self);
		if (!factory.IsBuilt()) return 0.0f;
		for (bc_Direction direction : constants::directions_adjacent) {
			if (factory.CanUnload(direction)) {
				PolicyOverlord::storeDirection = direction;
				return 1.0f;
			}
		}
		return 0.0f;
	}

	bool FactoryUnloadExecute(units::Unit unit) {
		units::Factory factory = bc_Unit_clone(unit.self);
		factory.Unload(PolicyOverlord::storeDirection);
		return true;
	}

	float KnightAttackEvaluate(units::Unit unit) {
		units::Knight robot = bc_Unit_clone(unit.self);
		if (robot.IsAttackReady()) { // Attack before Javelin in case you kill adjacent units
			auto nearby = CombatOverlord::EnemiesInRange(robot, robot.AttackRange());
			auto best = std::max_element(nearby.begin(), nearby.end(), [&robot](units::Unit& a, units::Unit& b) {
				return CombatOverlord::AttackValue(robot, a) < CombatOverlord::AttackValue(robot, b);
			});
			if (best != nearby.end()) {
				if (robot.CanAttack(*best)) {
					PolicyOverlord::storeUnit = *best;
					return CombatOverlord::AttackValue(robot, *best);
				}
			}
		}
		return 0.0f;
	}

	float MageAttackEvaluate(units::Unit unit) {
		units::Mage mage = bc_Unit_clone(unit.self);
		if (mage.IsAttackReady()) { // Attack before Javelin in case you kill adjacent units
			auto nearby = CombatOverlord::EnemiesInRange(mage, mage.AttackRange());
			auto best = std::max_element(nearby.begin(), nearby.end(), [&mage](units::Unit& a, units::Unit& b) {
				return CombatOverlord::SplashValue(mage, a) < CombatOverlord::SplashValue(mage, b);
			});
			if (best != nearby.end()) {
				if (mage.CanAttack(*best)) {
					PolicyOverlord::storeUnit = *best;
					return CombatOverlord::AttackValue(mage, *best);
				}
			}
		}
		return 0.0f;
	}

	float RangerAttackEvaluate(units::Unit unit) {
		units::Ranger ranger = bc_Unit_clone(unit.self);
		if (!ranger.IsAttackReady()) return 0.0f;
		auto nearby = ranger.Loc().ToMapLocation().NearbyUnits(ranger.AttackRange(), Utility::GetOtherTeam(ranger.Team()));
		if (nearby.size() == 0) return 0.0f;
		std::remove_if(nearby.begin(), nearby.end(), [&ranger](units::Unit& unit) {
			auto unitLocation = unit.Loc().ToMapLocation();
			return ranger.Loc().ToMapLocation().DistanceTo(unitLocation) < ranger.AttackRangeMin();
		});
		auto best = std::max_element(nearby.begin(), nearby.end(), [&ranger](units::Unit& a, units::Unit& b) {
			return CombatOverlord::AttackValue(ranger, a) < CombatOverlord::AttackValue(ranger, b);
		});
		if (best != nearby.end()) {
			if (ranger.CanAttack(*best)) {
				PolicyOverlord::storeUnit = *best;
				std::cout << CombatOverlord::AttackValue(ranger, *best) << " attack value" << std::endl;
				return CombatOverlord::AttackValue(ranger, *best);
			}
		}
		return 0.0f;
	}

	bool AttackExecute(units::Unit unit) {
		units::Robot robot = bc_Unit_clone(unit.self);
		robot.Attack(PolicyOverlord::storeUnit);
		return true;
	}

}

#endif
