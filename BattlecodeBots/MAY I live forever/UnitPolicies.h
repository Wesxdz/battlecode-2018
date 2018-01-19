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
#include "MapUtil.h"

namespace policy {

	float AvoidDamageEvaluate(units::Unit unit) {
		units::Robot robot{ bc_Unit_clone(unit.self) };
		if (!robot.IsMoveReady()) return 0.0f;
		MapLocation location = robot.Loc().ToMapLocation();
		bc_Team enemyTeam = Utility::GetOtherTeam(robot.Team());
		float currentDanger = CombatOverlord::Danger(location, enemyTeam);
		if (currentDanger == 0) return 0.0f;
		auto run = constants::directions_adjacent;
		std::sort(run.begin(), run.end(), [&location, &enemyTeam](bc_Direction a, bc_Direction b) {
			return CombatOverlord::Danger(MapLocation::Neighbor(location, a), enemyTeam) < CombatOverlord::Danger(MapLocation::Neighbor(location, b), enemyTeam);
		});
		for (bc_Direction direction : run) {
			float dangerReduction = (float)currentDanger - CombatOverlord::Danger(MapLocation::Neighbor(location, direction), enemyTeam);
			if (robot.CanMove(direction)) {
				PolicyOverlord::storeDirection = direction;
				return dangerReduction / 20;
			}
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

	float WorkerSeekKarboniteEvaluate(units::Unit unit) {
		units::Worker worker = bc_Unit_clone(unit.self);
		MapLocation workerLocation = worker.Loc().ToMapLocation();
		if (!worker.IsMoveReady()) return 0.0f;
		for (auto& project : BuilderOverlord::buildProjects) {
			auto self = std::find(project.second.begin(), project.second.begin(), worker.id);
			bool workingOnProject = self != project.second.end();
			if (workingOnProject) return 0.0f;
		}
		std::vector<bc_MapLocation*> nearbyDeposits;
		for (uintptr_t i = 0; i < 100; i = (i + 1) * (i + 1)) { // TODO Optimize with PlayerData karboniteDeposits
			bc_VecMapLocation* nearby = bc_GameController_all_locations_within(GameController::gc, workerLocation.self, i);
			nearbyDeposits = MapUtil::FilteredLocations(nearby, [](bc_MapLocation* potentialDeposit) {
				return bc_GameController_can_sense_location(GameController::gc, potentialDeposit) && bc_GameController_karbonite_at(GameController::gc, potentialDeposit) > 0;
			});
			delete_bc_VecMapLocation(nearby);
			if (nearbyDeposits.size() > 0) break;
		}
		if (nearbyDeposits.size() == 0) return 0.0f;
		auto closestDeposit = std::min_element(std::begin(nearbyDeposits), std::end(nearbyDeposits), [&workerLocation](bc_MapLocation* a, bc_MapLocation* b) {
			return
				abs(bc_MapLocation_x_get(workerLocation.self) - bc_MapLocation_x_get(a)) + abs(bc_MapLocation_y_get(workerLocation.self) - bc_MapLocation_y_get(a)) <
				abs(bc_MapLocation_x_get(workerLocation.self) - bc_MapLocation_x_get(b)) + abs(bc_MapLocation_y_get(workerLocation.self) - bc_MapLocation_y_get(b));
		});
		PolicyOverlord::storeLocation = MapLocation(bc_MapLocation_clone(*closestDeposit));
		for (bc_MapLocation* location : nearbyDeposits) {
			delete_bc_MapLocation(location);
		}
		return 5.0f;
	}

	bool WorkerSeekKarboniteExecute(units::Unit unit) {
		units::Robot robot = bc_Unit_clone(unit.self);
		return Pathfind::MoveFuzzy(robot, robot.Loc().ToMapLocation().DirectionTo(PolicyOverlord::storeLocation));
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
		delete_bc_Unit(build.self);
		build.self = bc_GameController_unit(GameController::gc, PolicyOverlord::storeUnit.id);
		if (build.IsBuilt()) {
			BuilderOverlord::buildProjects.erase(BuilderOverlord::buildProjects.find(build.id)); //Remove build project
		}
		return true;
	}

	float WorkerSeekBuildEvaluate(units::Unit unit) {
		if (BuilderOverlord::buildProjects.size() == 0) return 0.0f;
		units::Worker worker = bc_Unit_clone(unit.self);
		if (!worker.IsMoveReady()) return 0.0f;
		MapLocation workerLocation = worker.Loc().ToMapLocation();
		bool workingOnProject = false;
		uint16_t projId = 0;
		for (auto& project : BuilderOverlord::buildProjects) {
			auto self = std::find(project.second.begin(), project.second.begin(), worker.id);
			workingOnProject = self != project.second.end();
			if (workingOnProject) {
				projId = project.first;
				break;
			}
		}
		if (!workingOnProject) { // Do we want to work on a project?
			auto startWork = std::min_element(BuilderOverlord::buildProjects.begin(), BuilderOverlord::buildProjects.end(), [&workerLocation](auto& a, auto& b) { // std::pair<uint16_t, std::vector<uint16_t>>
				units::Structure buildA = bc_GameController_unit(GameController::gc, a.first);
				units::Structure buildB = bc_GameController_unit(GameController::gc, b.first);
				return workerLocation.DistanceTo(buildA.Loc().ToMapLocation()) < workerLocation.DistanceTo(buildA.Loc().ToMapLocation());
			});
			if (startWork != BuilderOverlord::buildProjects.end() && (*startWork).second.size() < 4) {
				std::cout << "Joining project with " << (*startWork).second.size() << " workers" << std::endl;
				BuilderOverlord::buildProjects[(*startWork).first].push_back(worker.id); // Join the closest project if it has less than 4 workers!
				workingOnProject = true;
			}
		}
		if (workingOnProject) {
			units::Structure build = bc_GameController_unit(GameController::gc, projId);
			if (worker.Loc().ToMapLocation().IsAdjacentTo(build.Loc().ToMapLocation())) {
				return 0.0f; // We don't need to move if already next to the build location
			}
			else {
				PolicyOverlord::storeDirection = worker.Loc().ToMapLocation().DirectionTo(build.Loc().ToMapLocation());
				return 10.0f;
			}
		}
		return 0.0f;
	}

	bool WorkerSeekBuildExecute(units::Unit unit) {
		units::Robot robot = bc_Unit_clone(unit.self);
		return Pathfind::MoveFuzzy(robot, PolicyOverlord::storeDirection);
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
					PolicyOverlord::storeUnit.id = (*best).id;
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
					PolicyOverlord::storeUnit.id = (*best).id;
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
		auto best = std::max_element(nearby.begin(), nearby.end(), [&ranger](units::Unit& a, units::Unit& b) {
			return CombatOverlord::AttackValue(ranger, a) < CombatOverlord::AttackValue(ranger, b);
		});
		while (true) {
			if (best == nearby.end()) break; // Probably too close D:
			if (ranger.CanAttack(*best)) {
				PolicyOverlord::storeUnit.id = (*best).id;
				return CombatOverlord::AttackValue(ranger, *best);
			}
			else {
				best++;
			}
		}
		return 0.0f;
	}

	bool AttackExecute(units::Unit unit) {
		units::Robot robot = bc_Unit_clone(unit.self);
		units::Robot toAttack = bc_GameController_unit(GameController::gc, PolicyOverlord::storeUnit.id);
		robot.Attack(PolicyOverlord::storeUnit);
		return true;
	}

	float SeekEnemyEvaluate(units::Unit unit) {
		units::Robot robot = bc_Unit_clone(unit.self);
		if (!robot.IsMoveReady()) return 0.0f;
		std::vector<units::Unit> nearbyEnemies;
		for (int i = 1; i <= 1000; i = (i + 1) * (i + 1)) {
			nearbyEnemies = robot.Loc().ToMapLocation().NearbyUnits(i, Utility::GetOtherTeam(robot.Team()));
			if (nearbyEnemies.size() > 0) {
				PolicyOverlord::storeDirection = robot.Loc().ToMapLocation().DirectionTo(nearbyEnemies[0].Loc().ToMapLocation());
				return 10.0f;
			}
		}
		return 0.0f;
	}

	bool SeekEnemyExecute(units::Unit unit) {
		units::Robot robot = bc_Unit_clone(unit.self);
		return Pathfind::MoveFuzzy(robot, PolicyOverlord::storeDirection);
	}

}

#endif
