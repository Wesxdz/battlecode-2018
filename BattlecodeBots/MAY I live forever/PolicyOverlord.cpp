#include "UnitPolicies.h"

#include "Policy.h"
#include "Pathfind.h"
#include "MapUtil.h"

#include <iostream>
#include <algorithm>

#include "Log.h"
#include "PolicyOverlord.h"
#include "UnitPolicies.h"

bc_Direction PolicyOverlord::storeDirection;
units::Unit PolicyOverlord::storeUnit;

void PolicyOverlord::Update()
{
	bool policyTaken = false;
	do { // Loop through all units evaluating policies until no policy is executed
		policyTaken = false;
		auto units = GameController::Units(MyTeam);
		for (auto& unit : units) {
			std::sort(policies[unit.type].begin(), policies[unit.type].end(), [&unit](std::shared_ptr<Policy>& a, std::shared_ptr<Policy>& b) {
				return a->Evaluate(unit) > b->Evaluate(unit); // reverse sort
			});
			auto policyChosen = policies[unit.type].begin(); // Choose the max policy
			bool searchingForPolicyChoice = policyChosen != policies[unit.type].end();
			while (searchingForPolicyChoice) { // Loop through the policies until one is successfully executed or there are no more left
				if ((*policyChosen)->Evaluate(unit) <= 0.0f) { // Eval stores variables!!
					break;
				}
				if ((*policyChosen)->Command(unit)) {
					policyTaken = true;
					break;
				}
				policyChosen++;
				bool isValidPolicy = policyChosen != policies[unit.type].end();
				if (!isValidPolicy) {
					searchingForPolicyChoice = false;
				}
			}
		}
	} while (policyTaken);
}

PolicyOverlord::PolicyOverlord()
{
	auto robot1 = std::make_shared<Policy>( "avoid_damage" );
	robot1->Evaluate = policy::AvoidDamageEval;
	robot1->Execute = policy::AvoidDamageExecute;
	policies[Worker].push_back(robot1);

	auto robot2 = std::make_shared<Policy>( "load_rocket" );
	robot2->Evaluate = policy::LoadRocketEval;
	robot2->Execute = policy::LoadRocketExecute;
	policies[Knight].push_back(robot2);
	policies[Ranger].push_back(robot2);
	policies[Mage].push_back(robot2);
	policies[Worker].push_back(robot2);

	auto robot3 = std::make_shared<Policy>("wander");
	robot3->Evaluate = policy::WanderEvaluate;
	robot3->Execute = policy::WanderExecute;
	policies[Worker].push_back(robot3);
	policies[Knight].push_back(robot3);
	policies[Ranger].push_back(robot3);
	policies[Mage].push_back(robot3);
	policies[Worker].push_back(robot3);

	auto worker1 = std::make_shared<Policy>("harvest_karbonite");
	worker1->Evaluate = policy::WorkerHarvestKarboniteEvaluate;
	worker1->Execute = policy::WorkerHarvestKarboniteExecute;
	policies[Worker].push_back(worker1);

}

void PolicyOverlord::Init()
{

	//auto worker1 = std::make_shared<Policy>("collect_karbonite");
	//worker1->Evaluate = [this]() {
	//	if (activeMapLocation) { return (float)bc_Unit_worker_harvest_amount(activeUnit); }
	//	return 0.0f;
	//};
	//worker1->Execute = [this](float value) {
	//	bc_VecMapLocation* nearby = bc_GameController_all_locations_within(Player::gc, activeMapLocation, 25);
	//	auto nearbyDeposits = MapUtil::FilteredLocations(nearby, [](bc_MapLocation* potentialDeposit) {
	//		return bc_GameController_karbonite_at(Player::gc, potentialDeposit) > 0;
	//	});
	//	delete_bc_VecMapLocation(nearby);
	//	if (nearbyDeposits.size() == 0) {
	//		Pathfind::MoveRandom(activeUnit);
	//		return false; // Calculate likelyhood of success of policy at runtime
	//	}
	//	std::sort(std::begin(nearbyDeposits), std::end(nearbyDeposits), [this](bc_MapLocation* a, bc_MapLocation* b) {
	//		return
	//			abs(bc_MapLocation_x_get(activeMapLocation) - bc_MapLocation_x_get(a)) + abs(bc_MapLocation_y_get(activeMapLocation) - bc_MapLocation_y_get(a)) <
	//			abs(bc_MapLocation_x_get(activeMapLocation) - bc_MapLocation_x_get(b)) + abs(bc_MapLocation_y_get(activeMapLocation) - bc_MapLocation_y_get(b));
	//	});
	//	bc_Direction probe = bc_MapLocation_direction_to(activeMapLocation, nearbyDeposits[0]);
	//	if (bc_GameController_can_harvest(Player::gc, bc_Unit_id(activeUnit), probe)) {
	//		bc_GameController_harvest(Player::gc, bc_Unit_id(activeUnit), probe);
	//		return true;
	//	}
	//	Pathfind::MoveGreedy(activeUnit, nearbyDeposits[0]);
	//	return true;
	//};
	//policies[bc_UnitType::Worker].push_back(worker1);


	//auto worker2 = std::make_shared<Policy>("blueprint_factory");
	//worker2->Evaluate = [this]() {
	//	bc_Location* location = bc_Unit_location(activeUnit);
	//	bool isOnMap = bc_Location_is_on_map(location);
	//	delete_bc_Location(location);
	//	if (!isOnMap) return 0.0f;
	//	if (bc_GameController_karbonite(Player::gc) >= bc_UnitType_blueprint_cost(Factory)) return 200.0f;
	//	return 0.0f;
	//};
	//worker2->Execute = [this](float value) {
	//	// TODO Determine where factory should go
	//	for (int i = 0; i < 8; i++) {
	//		if (bc_GameController_can_blueprint(Player::gc, bc_Unit_id(activeUnit), Factory, static_cast<bc_Direction>(i))) {
	//			bc_GameController_blueprint(Player::gc, bc_Unit_id(activeUnit), Factory, static_cast<bc_Direction>(i));
	//			return true;
	//		}
	//	}
	//	return true;
	//};
	//policies[bc_UnitType::Worker].push_back(worker2);

	//auto worker4 = std::make_shared<Policy>("build");
	//worker4->Evaluate = [this]() {
	//	float score = 0.0f;
	//	if (!activeMapLocation) { return 0.0f; }
	//	auto factories = bc_GameController_sense_nearby_units_by_type(Player::gc, activeMapLocation, 1, Factory);
	//	auto rockets = bc_GameController_sense_nearby_units_by_type(Player::gc, activeMapLocation, 1, Rocket);
	//	std::vector<bc_Unit*> nearbyBuilds;
	//	for (uintptr_t i = 0; i < bc_VecUnit_len(factories); i++) {
	//		nearbyBuilds.push_back(bc_VecUnit_index(factories, i));
	//	}
	//	for (uintptr_t i = 0; i < bc_VecUnit_len(rockets); i++) {
	//		nearbyBuilds.push_back(bc_VecUnit_index(rockets, i));
	//	}
	//	if (nearbyBuilds.size() > 0) {
	//		auto it = std::find_if(std::begin(nearbyBuilds), std::end(nearbyBuilds), [](bc_Unit* structure) {
	//			return !bc_Unit_structure_is_built(structure);
	//		});
	//		if (it != std::end(nearbyBuilds)) {
	//			if (bc_GameController_can_build(Player::gc, activeId, bc_Unit_id(*it))) {
	//				if (bc_Unit_unit_type(*it) == Rocket) {
	//					std::cout << "Evaluated should build rocket\n";
	//				}
	//				storeId = bc_Unit_id(*it);
	//				score = 300.0f;
	//			}
	//		}
	//	}
	//	delete_bc_VecUnit(factories);
	//	delete_bc_VecUnit(rockets);
	//	for (bc_Unit* unit : nearbyBuilds) {
	//		delete_bc_Unit(unit);
	//	}
	//	return score;
	//};
	//worker4->Execute = [this](float value) {
	//	if (bc_GameController_can_build(Player::gc, activeId, storeId)) {
	//		bc_GameController_build(Player::gc, activeId, storeId);
	//		return true;
	//	}
	//	return false;
	//};
	//policies[bc_UnitType::Worker].push_back(worker4);

	//auto worker3 = std::make_shared<Policy>("blueprint_rocket");
	//worker3->Evaluate = [this]() {
	//	auto rockets = bc_GameController_sense_nearby_units_by_type(Player::gc, activeMapLocation, 1, Rocket);
	//	if (bc_VecUnit_len(rockets) > 0) return 0.0f;
	//	delete_bc_VecUnit(rockets);
	//	float score = 0.0f;
	//	auto info = bc_GameController_research_info(Player::gc);
	//	if (bc_ResearchInfo_get_level(info, Rocket) > 0) {
	//		score += 500.0f;
	//	}
	//	delete_bc_ResearchInfo(info);
	//	bc_Location* location = bc_Unit_location(activeUnit);
	//	bool isOnMap = bc_Location_is_on_map(location);
	//	delete_bc_Location(location);
	//	if (!isOnMap) return 0.0f;
	//	return score;
	//};
	//worker3->Execute = [this](float value) {
	//	if (bc_GameController_karbonite(Player::gc) < bc_UnitType_blueprint_cost(Rocket)) return false;
	//	for (int i = 0; i < 8; i++) {
	//		if (bc_GameController_can_blueprint(Player::gc, bc_Unit_id(activeUnit), Rocket, static_cast<bc_Direction>(i))) {
	//			bc_GameController_blueprint(Player::gc, bc_Unit_id(activeUnit), Rocket, static_cast<bc_Direction>(i));
	//			return true;
	//		}
	//	}
	//	return true;
	//};
	//policies[bc_UnitType::Worker].push_back(worker3);


	//auto worker5 = std::make_shared<Policy>("repair");


	//auto worker6 = std::make_shared<Policy>("replicate");
	//worker6->Evaluate = [this]() {
	//	if (bc_GameController_karbonite(Player::gc) < bc_UnitType_replicate_cost(Worker)) return 0.0f;
	//	auto round = bc_GameController_round(Player::gc);
	//	if (round > 20 && round < 30) return 0.0f;
	//	if (Info::numTeamUnits[Worker] > 10) return 0.0f;
	//	return ((749.0f - round) * bc_Unit_worker_harvest_amount(activeUnit) - bc_UnitType_replicate_cost(Worker) * 2)/10;
	//};
	//worker6->Execute = [this](float value) {
	//	for (int i = 0; i < 8; i++) {
	//		if (bc_GameController_can_replicate(Player::gc, activeId, static_cast<bc_Direction>(i))) {
	//			bc_GameController_replicate(Player::gc, activeId, static_cast<bc_Direction>(i));
	//			return true;
	//		}
	//	}
	//	return false;
	//};
	//policies[bc_UnitType::Worker].push_back(worker6);

	//auto factory1 = std::make_shared<Policy>("produce_knight");
	//factory1->Evaluate = [this]() {
	//	if (bc_GameController_karbonite(Player::gc) < bc_UnitType_factory_cost(Knight) || bc_Unit_is_factory_producing(activeUnit)) return 0.0f;
	//	return 20.0f;
	//};
	//factory1->Execute = [this](float value) {
	//	if (bc_GameController_can_produce_robot(Player::gc, activeId, Knight)) {
	//		bc_GameController_produce_robot(Player::gc, activeId, Knight);
	//		return true;
	//	}
	//	return false;
	//};
	//policies[Factory].push_back(factory1);


	//auto factory2 = std::make_shared<Policy>("produce_worker");
	//factory2->Evaluate = []() {
	//	return 0.0f;
	//};
	//factory2->Execute = [](float value) {
	//	return false;
	//};
	////policies[Factory].push_back(factory2);


	//auto factory3 = std::make_shared<Policy>("produce_healer");
	//factory3->Evaluate = []() {
	//	return 0.0f;
	//};
	//factory3->Execute = [](float value) {
	//	return false;
	//};
	////policies[Factory].push_back(factory3);

	//auto factory4 = std::make_shared<Policy>("produce_ranger");
	//factory4->Evaluate = [this]() {
	//	if (bc_GameController_karbonite(Player::gc) < bc_UnitType_factory_cost(Ranger) || bc_Unit_is_factory_producing(activeUnit)) return 0.0f;
	//	return 10.0f;
	//};
	//factory4->Execute = [this](float value) {
	//	if (bc_GameController_can_produce_robot(Player::gc, activeId, Ranger)) {
	//		bc_GameController_produce_robot(Player::gc, activeId, Ranger);
	//		return true;
	//	}
	//	return false;
	//};
	//policies[Factory].push_back(factory4);


	//auto factory5 = std::make_shared<Policy>("produce_mage");
	//factory5->Evaluate = []() {
	//	if (bc_GameController_karbonite(Player::gc) < bc_UnitType_factory_cost(Mage)) return 0.0f;
	//	return (float)bc_GameController_round(Player::gc)/100;
	//};
	//factory5->Execute = [this](float value) {
	//	if (bc_GameController_can_produce_robot(Player::gc, activeId, Mage)) {
	//		bc_GameController_produce_robot(Player::gc, activeId, Mage);
	//		return true;
	//	}
	//	return false;
	//};
	//policies[Factory].push_back(factory5);
	//

	//auto factory6 = std::make_shared<Policy>("unload_units");
	//factory6->Evaluate = [this]() {
	//	float score = 0.0f;
	//	auto garrison = bc_Unit_structure_garrison(activeUnit);
	//	if (bc_VecUnitID_len(garrison) > 0) {
	//		for (int i = 0; i < 8; i++) {
	//			if (bc_GameController_can_unload(Player::gc, activeId, static_cast<bc_Direction>(i))) {
	//				storeDirection = static_cast<bc_Direction>(i);
	//				score = 100.0f;
	//			}
	//		}
	//	}
	//	delete_bc_VecUnitID(garrison);
	//	return score;
	//};
	//factory6->Execute = [this](float value) {
	//	bc_GameController_unload(Player::gc, activeId, storeDirection);
	//	return true;
	//};
	//policies[Factory].push_back(factory6);
	//if (Player::planet == Mars) {
	//	policies[Rocket].push_back(factory6);
	//}


	//auto factory7 = std::make_shared<Policy>("avoid_production"); // Use to make building units a negative value if factory is damaged
	//factory7->Evaluate = [this]() {
	//	auto round = bc_GameController_round(Player::gc);
	//	if (round > 450) {
	//		return 200.0f;
	//	}
	//	return 0.0f;
	//};
	//factory7->Execute = [this](float value) {
	//	return true;
	//};
	//policies[Factory].push_back(factory7);

	//auto rocket1 = std::make_shared<Policy>("launch");
	//rocket1->Evaluate = [this]() {
	//	if (!bc_Unit_structure_is_built(activeUnit)) return 0.0f;
	//	float score = 0.0f;
	//	auto round = bc_GameController_round(Player::gc);
	//	if (round == 749) score += 1000.0f;
	//	bc_VecUnitID* garrison = bc_Unit_structure_garrison(activeUnit);
	//	float filled = bc_VecUnitID_len(garrison)/(float)bc_Unit_structure_max_capacity(activeUnit);
	//	if (filled == 1.0f) score += 100;
	//	float health = (float)bc_Unit_health(activeUnit) / bc_Unit_max_health(activeUnit);
	//	if (health < 1.0f) score += (1.0f - health) * 10.0f;
	//	delete_bc_VecUnitID(garrison);
	//	return score;
	//};
	//rocket1->Execute = [this](float value) {
	//	std::cout << "Attempting Launch!\n";
	//	bc_GameController_launch_rocket(Player::gc, activeId, Pathfind::PickRandom(MapUtil::marsPassableLocations));
	//	CHECK_ERRORS();
	//	return true;
	//};
	//if (Player::planet == Earth) {
	//	policies[Rocket].push_back(rocket1);
	//}

	//auto hunt = std::make_shared<Policy>("hunt");
	//hunt->Evaluate = [this]() {
	//	float score = 0.0f;
	//	if (!activeMapLocation) return score;
	//	bc_Team team = bc_GameController_team(Player::gc);
	//	bc_Team otherTeam = team == bc_Team::Blue ? Red : Blue;
	//	for (int i = 1; i <= 100; i = (i + 1) * (i + 1)) {
	//		bc_VecUnit* nearbyEnemies = bc_GameController_sense_nearby_units_by_team(Player::gc, activeMapLocation, i*i, otherTeam);
	//		if (bc_VecUnit_len(nearbyEnemies) > 0) {
	//			storeUnit = bc_VecUnit_index(nearbyEnemies, 0);
	//			storeId = bc_Unit_id(storeUnit);
	//			score = 50.0f;
	//			delete_bc_VecUnit(nearbyEnemies);
	//			return score;
	//		}
	//		delete_bc_VecUnit(nearbyEnemies);
	//	}
	//	return score;
	//};
	//hunt->Execute = [this](float value) {
	//	if (bc_GameController_can_attack(Player::gc, activeId, storeId)) {
	//		bc_GameController_attack(Player::gc, activeId, storeId);
	//	}
	//	else {
	//		bc_Location* location = bc_Unit_location(storeUnit);
	//		if (!bc_Location_is_on_map(location)) {
	//			delete_bc_Location(location);
	//			return false;
	//		}
	//		bc_MapLocation* mapLocation = bc_Location_map_location(location);
	//		Pathfind::MoveGreedy(activeUnit, mapLocation);
	//		delete_bc_MapLocation(mapLocation);
	//		delete_bc_Location(location);
	//	}
	//	return true;
	//};
	//policies[Knight].push_back(hunt);
	//policies[Ranger].push_back(hunt);

}

