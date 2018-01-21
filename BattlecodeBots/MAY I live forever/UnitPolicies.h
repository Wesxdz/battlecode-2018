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
#include "Pegboard.h"

namespace policy {

	float AvoidDamageEvaluate(bc_Unit* unit) {
		units::Robot robot{ bc_Unit_clone(unit) };
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

	bool AvoidDamageExecute(bc_Unit* unit) {
		units::Robot robot{ bc_Unit_clone(unit) };
		robot.Move(PolicyOverlord::storeDirection);
		return true;
	}

	float LoadRocketEvaluate(bc_Unit* unit) {
		float score = 0.0f;
		units::Robot robot = bc_Unit_clone(unit);
		MapLocation robotLocation = robot.Loc().ToMapLocation();
		if (!robot.IsMoveReady()) return 0.0f;
		bc_VecUnit* nearbyRockets = bc_GameController_sense_nearby_units_by_type(GameController::gc, robotLocation.self, 2, Rocket);
		for (uintptr_t i = 0; i < bc_VecUnit_len(nearbyRockets); i++) {
			units::Rocket rocket{ bc_VecUnit_index(nearbyRockets, i) };
			if (rocket.CanLoad(robot)) {
				PolicyOverlord::storeId = bc_Unit_id(rocket.self);
				score += 40.0f;
				break;
			}
		}
		delete_bc_VecUnit(nearbyRockets);
		return score;
	}

	bool LoadRocketExecute(bc_Unit* unit) {
		units::Robot robot = bc_Unit_clone(unit);
		units::Rocket rocket = bc_GameController_unit(GameController::gc, PolicyOverlord::storeId);
		rocket.Load(robot);
		return true;
	}

	float WanderEvaluate(bc_Unit* unit) {
		if (!bc_GameController_is_move_ready(GameController::gc, bc_Unit_id(unit))) return 0.0f;
		return 0.1f;
	}

	bool WanderExecute(bc_Unit* unit) {
		units::Robot robot = bc_Unit_clone(unit);
		return Pathfind::MoveRandom(robot);
	}

	float WorkerHarvestKarboniteEvaluate(bc_Unit* unit) {
		if (bc_Unit_worker_has_acted(unit)) return 0.0f;
		for (bc_Direction direction : constants::directions_all) {
			if (bc_GameController_can_harvest(GameController::gc, bc_Unit_id(unit), direction)) {
				PolicyOverlord::storeDirection = direction;
				return 1.0f;
			}
		}
		return 0.0f;
	}

	bool WorkerHarvestKarboniteExecute(bc_Unit* unit) {
		bc_GameController_harvest(GameController::gc, bc_Unit_id(unit), PolicyOverlord::storeDirection);
		return true;
	}

	float WorkerBlueprintEvaluate(bc_Unit* unit) {
		units::Worker worker = bc_Unit_clone(unit);
		if (worker.HasActed()) return 0.0f;
		bc_UnitType priority = PolicyOverlord::HighestPriority();
		if (!(priority == Factory || priority == Rocket)) return 0.0f;
		if (GameController::Karbonite() < bc_UnitType_blueprint_cost(priority)) return 0.0f;
		for (bc_Direction direction : constants::directions_adjacent) {
			if (worker.CanBlueprint(priority, direction)) {
				PolicyOverlord::storeDirection = direction;
				PolicyOverlord::storeUnitType = priority;
				return 3.0f;
			}
		}
		return 0.0f;
	}

	bool WorkerBlueprintExecute(bc_Unit* unit) {
		units::Worker worker = bc_Unit_clone(unit);
		worker.Blueprint(PolicyOverlord::storeUnitType, PolicyOverlord::storeDirection);
		MapLocation buildLocation = MapLocation::Neighbor(worker.Loc().ToMapLocation(), PolicyOverlord::storeDirection);
		units::Structure build = buildLocation.Occupant();
		BuilderOverlord::buildProjects[build.id].push_back(worker.id);
		return true;
	}

	float WorkerReplicateEvaluate(bc_Unit* unit) {
		units::Worker worker = bc_Unit_clone(unit);
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

	bool WorkerReplicateExecute(bc_Unit* unit) {
		units::Worker worker = bc_Unit_clone(unit);
		worker.Replicate(PolicyOverlord::storeDirection);
		return true;
	}

	float WorkerSeekKarboniteEvaluate(bc_Unit* unit) {
		units::Worker worker = bc_Unit_clone(unit);
		MapLocation workerLocation = worker.Loc().ToMapLocation();
		if (!worker.IsMoveReady()) return 0.0f;
		for (auto& project : BuilderOverlord::buildProjects) {
			auto self = std::find(project.second.begin(), project.second.begin(), worker.id);
			bool workingOnProject = self != project.second.end();
			if (workingOnProject) return 0.0f;
		}
		for (bc_Direction direction : constants::directions_all) { // If Karbonite is already nearby, don't seek it!
			if (MapLocation::Neighbor(workerLocation, direction).Karbonite() > 0) return 0.0f;
		}
		auto closest = std::min_element(PlayerData::pd->karboniteDeposits.begin(), PlayerData::pd->karboniteDeposits.end(), [&workerLocation](MapLocation& a, MapLocation& b) {
			return workerLocation.DistanceTo(a) < workerLocation.DistanceTo(b);
		});
		if (closest != PlayerData::pd->karboniteDeposits.end()) {
			PolicyOverlord::storeLocation = *closest;
			return 5.0f;
		}
		return 0.0f;
	}

	bool WorkerSeekKarboniteExecute(bc_Unit* unit) {
		units::Robot robot = bc_Unit_clone(unit);
		return Pathfind::MoveFuzzyFlow(robot, PolicyOverlord::storeLocation);
	}

	float WorkerBuildEvaluate(bc_Unit* unit) {
		units::Worker worker = bc_Unit_clone(unit);
		if (worker.HasActed()) return 0.0f;
		for (auto& project : BuilderOverlord::buildProjects) {
			auto self = std::find(project.second.begin(), project.second.begin(), worker.id);
			if (self != project.second.end()) {
				if (bc_GameController_can_build(GameController::gc, worker.id, project.first)) {
					PolicyOverlord::storeId = project.first;
					return 10.0f;
				}
			}
		}
		return 0.0f;
	}

	bool WorkerBuildExecute(bc_Unit* unit) {
		units::Worker worker = bc_Unit_clone(unit);
		units::Structure build = bc_GameController_unit(GameController::gc, PolicyOverlord::storeId);
		worker.Build(build);
		units::Structure updatedBuild = bc_GameController_unit(GameController::gc, PolicyOverlord::storeId);
		if (updatedBuild.IsBuilt()) {
			BuilderOverlord::buildProjects.erase(BuilderOverlord::buildProjects.find(updatedBuild.id)); //Remove build project
		}
		return true;
	}

	float WorkerSeekBuildEvaluate(bc_Unit* unit) {
		if (BuilderOverlord::buildProjects.size() == 0) return 0.0f;
		units::Worker worker = bc_Unit_clone(unit);
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

	bool WorkerSeekBuildExecute(bc_Unit* unit) {
		units::Robot robot = bc_Unit_clone(unit);
		return Pathfind::MoveFuzzy(robot, PolicyOverlord::storeDirection);
	}

	float FactoryProduceEvaluate(bc_Unit* unit) {
		units::Factory factory = bc_Unit_clone(unit);
		if (!factory.IsBuilt()) return 0.0f;
		bc_UnitType priority = PolicyOverlord::HighestPriority();
		if (!Utility::IsRobot(priority)) return 0.0f;
		if (priority == Worker && PlayerData::pd->teamUnitCounts[Worker] > 2) return 0.0f;
		if (factory.CanProduce(priority)) {
			PolicyOverlord::storeUnitType = priority;
			return PlayerData::pd->unitPriority[priority];
		}
		return 0.0f;
	}

	bool FactoryProduceExecute(bc_Unit* unit) {
		units::Factory factory = bc_Unit_clone(unit);
		factory.Produce(PolicyOverlord::storeUnitType);
		return true;
	}

	float FactoryUnloadEvaluate(bc_Unit* unit) {
		units::Factory factory = bc_Unit_clone(unit);
		if (!factory.IsBuilt()) return 0.0f;
		for (bc_Direction direction : constants::directions_adjacent) {
			if (factory.CanUnload(direction)) {
				PolicyOverlord::storeDirection = direction;
				return 1.0f;
			}
		}
		return 0.0f;
	}

	bool FactoryUnloadExecute(bc_Unit* unit) {
		units::Factory factory = bc_Unit_clone(unit);
		factory.Unload(PolicyOverlord::storeDirection);
		return true;
	}

	float KnightAttackEvaluate(bc_Unit* unit) {
		units::Knight robot = bc_Unit_clone(unit);
		if (robot.IsAttackReady()) { // Attack before Javelin in case you kill adjacent units
			auto nearby = CombatOverlord::EnemiesInRange(robot, robot.AttackRange());
			auto best = std::max_element(nearby.begin(), nearby.end(), [&robot](units::Unit& a, units::Unit& b) {
				return CombatOverlord::AttackValue(robot, a) < CombatOverlord::AttackValue(robot, b);
			});
			if (best != nearby.end()) {
				if (robot.CanAttack(*best)) {
					PolicyOverlord::storeId = (*best).id;
					return CombatOverlord::AttackValue(robot, *best);
				}
			}
		}
		return 0.0f;
	}

	float KnightJavelinEvaluate(bc_Unit* unit) {
		units::Knight knight = bc_Unit_clone(unit);
		if (knight.IsActiveUnlocked() && knight.IsJavelinReady()) {
			auto nearby = CombatOverlord::EnemiesInRange(knight, knight.AbilityRange());
			auto best = std::max_element(nearby.begin(), nearby.end(), [&knight](units::Unit& a, units::Unit& b) {
				return CombatOverlord::AttackValue(knight, a) < CombatOverlord::AttackValue(knight, b);
			});
			if (best != nearby.end()) {
				if (knight.CanJavelin(*best)) {
					PolicyOverlord::storeId = (*best).id;
					return CombatOverlord::AttackValue(knight, *best) - 0.1f; // Attack before Javelin 
				}
			}
		}
		return 0.0f;
	}

	bool KnightJavelinExecute(bc_Unit* unit) {
		units::Knight knight = bc_Unit_clone(unit);
		units::Unit target = bc_GameController_unit(GameController::gc, PolicyOverlord::storeId);
		knight.Javelin(target);
		return true;
	}

	float MageAttackEvaluate(bc_Unit* unit) {
		units::Mage mage = bc_Unit_clone(unit);
		if (mage.IsAttackReady()) { // Attack before Javelin in case you kill adjacent units
			auto nearby = CombatOverlord::EnemiesInRange(mage, mage.AttackRange());
			auto best = std::max_element(nearby.begin(), nearby.end(), [&mage](units::Unit& a, units::Unit& b) {
				return CombatOverlord::SplashValue(mage, a) < CombatOverlord::SplashValue(mage, b);
			});
			if (best != nearby.end()) {
				if (mage.CanAttack(*best)) {
					PolicyOverlord::storeId= (*best).id;
					return CombatOverlord::AttackValue(mage, *best);
				}
			}
		}
		return 0.0f;
	}

	float RangerAttackEvaluate(bc_Unit* unit) {
		units::Ranger ranger = bc_Unit_clone(unit);
		if (!ranger.IsAttackReady()) return 0.0f;
		auto nearby = ranger.Loc().ToMapLocation().NearbyUnits(ranger.AttackRange(), Utility::GetOtherTeam(ranger.Team()));
		if (nearby.size() == 0) return 0.0f;
		auto best = std::max_element(nearby.begin(), nearby.end(), [&ranger](units::Unit& a, units::Unit& b) {
			return CombatOverlord::AttackValue(ranger, a) < CombatOverlord::AttackValue(ranger, b);
		});
		while (true) {
			if (best == nearby.end()) break; // Probably too close D:
			if (ranger.CanAttack(*best)) {
				PolicyOverlord::storeId = (*best).id;
				return CombatOverlord::AttackValue(ranger, *best);
			}
			else {
				best++;
			}
		}
		return 0.0f;
	}

	bool AttackExecute(bc_Unit* unit) {
		units::Robot robot = bc_Unit_clone(unit);
		units::Robot toAttack = bc_GameController_unit(GameController::gc, PolicyOverlord::storeId);
		robot.Attack(toAttack);
		return true;
	}

	float SeekEnemyEvaluate(bc_Unit* unit) {
		units::Robot robot = bc_Unit_clone(unit);
		MapLocation robotLocation = robot.Loc().ToMapLocation();
		if (!robot.IsMoveReady()) return 0.0f;
		bc_Team otherTeam = Utility::GetOtherTeam(GameController::Team());
		bc_VecUnit* nearbyEnemies = bc_GameController_sense_nearby_units_by_team(GameController::gc, robotLocation.self, robot.AttackRange() + 2, otherTeam);
		if (bc_VecUnit_len(nearbyEnemies) > 0) {
			units::Unit seek = bc_VecUnit_index(nearbyEnemies, 0);
			PolicyOverlord::storeDirection = robotLocation.DirectionTo(seek.Loc().ToMapLocation());
			delete_bc_VecUnit(nearbyEnemies);
			return 20.0f;
		}
		delete_bc_VecUnit(nearbyEnemies);
		return 0.0f;
	}

	bool SeekEnemyExecute(bc_Unit* unit) {
		units::Robot robot = bc_Unit_clone(unit);
		return Pathfind::MoveFuzzy(robot, PolicyOverlord::storeDirection);
	}

	float SeekControlEvaluate(bc_Unit* unit) {
		units::Robot robot = bc_Unit_clone(unit);
		MapLocation robotLocation = robot.Loc().ToMapLocation();
		if (CombatOverlord::controlPoints.size() > 0) { // Move towards the closest control point
			auto move = std::min_element(CombatOverlord::controlPoints.begin(), CombatOverlord::controlPoints.end(), [&robotLocation](MapLocation& a, MapLocation& b) {
				return a.DistanceTo(robotLocation) < b.DistanceTo(robotLocation);
			});
			PolicyOverlord::storeLocation = *move;
			return 1.0f;
		}
		return 0.0f;
	}

	bool SeekControlExecute(bc_Unit* unit) {
		units::Robot robot = bc_Unit_clone(unit);
		return Pathfind::MoveFuzzyFlow(robot, PolicyOverlord::storeLocation);
	}

	float HealerHealEvaluate(bc_Unit* unit) {
		units::Healer healer = bc_Unit_clone(unit);
		if (!healer.IsHealReady()) return 0.0f;
		auto nearby = healer.Loc().ToMapLocation().NearbyUnits(healer.AttackRange(), healer.Team());
		if (nearby.size() == 0) return 0.0f;
		for (uint16_t id : CombatOverlord::requestHeal) {
			auto found = std::find_if(nearby.begin(), nearby.end(), [&healer, &id](units::Unit& toHeal) {
				if (id == toHeal.id) {
					units::Robot healing = bc_Unit_clone(toHeal.self);
					if (healer.CanHeal(healing)) {
						return true;
					}
				}
				return false;
			});
			if (found != nearby.end()) {
				PolicyOverlord::storeId = (*found).id;
				return 30.0f;
			}
		}
		return 0.0f;
	}

	bool HealerHealExecute(bc_Unit* unit) {
		units::Healer healer = bc_Unit_clone(unit);
		units::Robot toHeal = bc_GameController_unit(GameController::gc, PolicyOverlord::storeId);
		healer.Heal(toHeal);
		return true;
	}

	float HealerOverchargeEvaluate(bc_Unit* unit) {
		units::Healer healer = bc_Unit_clone(unit);
		if (!healer.IsActiveUnlocked() || !healer.IsAttackReady()) return 0.0f;
		auto nearby = healer.Loc().ToMapLocation().NearbyUnits(healer.AbilityRange(), healer.Team());
		if (nearby.size() == 0) return 0.0f;
		auto best = std::max_element(nearby.begin(), nearby.end(), [&healer](units::Unit& a, units::Unit& b) {
			return CombatOverlord::OverchargeValue(healer, bc_Unit_clone(a.self)) < CombatOverlord::OverchargeValue(healer, bc_Unit_clone(b.self));
		});
		units::Robot toOvercharge = bc_Unit_clone((*best).self);
		if (healer.CanOvercharge(toOvercharge.self)) {
			PolicyOverlord::storeId = bc_Unit_id(toOvercharge.self);
			return CombatOverlord::OverchargeValue(healer, bc_Unit_clone((*best).self));
		}
		return 0.0f;

	}

	bool HealerOverchargeExecute(bc_Unit* unit) {
		units::Healer healer = bc_Unit_clone(unit);
		units::Robot toOvercharge = bc_GameController_unit(GameController::gc, PolicyOverlord::storeId);
		healer.Overcharge(toOvercharge);
		return true;
	}

	float RocketUnloadEvaluate(bc_Unit* unit) {
		units::Rocket rocket = bc_Unit_clone(unit);
		if (!rocket.IsBuilt()) return 0.0f;
		for (bc_Direction direction : constants::directions_adjacent) {
			if (rocket.CanUnload(direction)) {
				PolicyOverlord::storeDirection = direction;
				return 1.0f;
			}
		}
		return 0.0f;
	}

	bool RocketUnloadExecute(bc_Unit* unit) {
		units::Rocket rocket = bc_Unit_clone(unit);
		rocket.Unload(PolicyOverlord::storeDirection);
		// TODO Push the unit
		return true;
	}

	float RocketLaunchEvaluate(bc_Unit* unit) {
		units::Rocket rocket = bc_Unit_clone(unit);
		if (!rocket.IsBuilt()) return 0.0f;
		if (GameController::Round() == 749 || rocket.Health() < 100 || rocket.Garrison().size() == rocket.MaxCapacity()) {
			// TODO Consider units damaged and push them away
			PolicyOverlord::storeLocation = bc_MapLocation_clone(MapUtil::marsPassableLocations[rand() % MapUtil::marsPassableLocations.size()]);
			return 1.0f;
		}
		return 0.0f;
	}

	bool RocketLaunchExecute(bc_Unit* unit) {
		units::Rocket rocket = bc_Unit_clone(unit);
		rocket.Launch(PolicyOverlord::storeLocation);
		return true;
	}

	float KiteEvaluate(bc_Unit* unit) {
		units::Robot robot = bc_Unit_clone(unit);
		if (!robot.IsMoveReady()) return 0.0f;
		MapLocation robotLocation = robot.Loc().ToMapLocation();
		auto nearbyUnits = robotLocation.NearbyUnits(robot.AttackRange(), Utility::GetOtherTeam(GameController::Team()));
		if (nearbyUnits.size() == 0) return 0.0f;
		Force f;
		for (units::Unit& nearby : nearbyUnits) {
			MapLocation nearbyLocation = nearby.Loc().ToMapLocation();
			if ((robotLocation.DistanceTo(nearbyLocation) + 10) < robot.AttackRange()) {
				bc_Direction run = nearbyLocation.DirectionTo(robotLocation);
				if (Utility::IsAttackRobot(nearby.type)) {
					f = f + Force((float)bc_Direction_dx(run) * 3, (float)bc_Direction_dy(run) * 3);
				}
				else {
					f = f + Force((float)bc_Direction_dx(run), (float)bc_Direction_dy(run));
				}
			}
		}
		if (f.Power() >= 1) {
			bc_Direction move = f.Direction();
			if (move != Center) {
				PolicyOverlord::storeDirection = move;
				return 1000.0f;
			}
		}
		return 0.0f;
	}

	bool KiteExecute(bc_Unit* unit) {
		units::Robot robot = bc_Unit_clone(unit);
		return Pathfind::MoveFuzzy(robot, PolicyOverlord::storeDirection);
	}

}

#endif
