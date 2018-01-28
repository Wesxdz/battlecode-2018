#include "UnitPolicies.h"

#include "Policy.h"
#include "Pathfind.h"

#include <iostream>
#include <algorithm>

#include "Log.h"
#include "PolicyOverlord.h"
#include "UnitPolicies.h"
#include <algorithm>

bc_Direction PolicyOverlord::storeDirection;
uint16_t PolicyOverlord::storeId;
MapLocation PolicyOverlord::storeLocation;
bc_UnitType PolicyOverlord::storeUnitType;

void PolicyOverlord::Update()
{
	int maxLoops = 5;
	if (bc_GameController_get_time_left_ms(GameController::gc) < 2000) maxLoops = 3;
	int loops = 0;
	//std::cout << HighestPriority() << " is highest priority" << std::endl;
	bool policyTaken = false;
	do { // Loop through all units evaluating policies until no policy is executed
		loops++;
		if (loops == maxLoops) break; // :(
		policyTaken = false;
		bc_VecUnit* units = bc_GameController_my_units(GameController::gc);
		for (uintptr_t i = 0; i < bc_VecUnit_len(units); i++) {
			units::Unit unit = bc_VecUnit_index(units, i);
			if (unit.Loc().IsInGarrison()) continue;
			if (unit.type == Ranger && bc_Unit_ranger_is_sniping(unit.self)) continue;
			std::sort(policies[unit.type].begin(), policies[unit.type].end(), [&unit](std::shared_ptr<Policy>& a, std::shared_ptr<Policy>& b) {
				return a->DebugEvaluate(unit.self) > b->DebugEvaluate(unit.self); // reverse sort
			});
			auto policyChosen = policies[unit.type].begin(); // Choose the max policy
			while (true) { // Loop through the policies until one is successfully executed or there are no more left
				bool isValidPolicy = policyChosen != policies[unit.type].end();
				if (!isValidPolicy) {
					break;
				}
				if ((*policyChosen)->DebugEvaluate(unit.self) <= 0.0f) { // Eval stores variables!!
					break;
				}
				if ((*policyChosen)->Execute(unit.self)) {
					policyTaken = false;
					break;
				}
				policyChosen++;
			}
		}
		delete_bc_VecUnit(units);
	} while (policyTaken);

	//if (GameController::Round() % 25 == 0) {
	//	for (auto& pair : Policy::times) {
	//		std::cout << pair.first << ": " << pair.second << std::endl;
	//	}
	//}
	//std::cout << "Finished evaluating policies" << std::endl;
}

bc_UnitType PolicyOverlord::HighestProductionPriority()
{
	bc_UnitType priority = Knight;
	float highest = 0.0f;
	for (auto& unitPrio : PlayerData::pd->unitPriority) {
		if (unitPrio.first == Worker || unitPrio.first == Factory || unitPrio.first == Rocket) continue;
		if (unitPrio.second > highest) {
			priority = unitPrio.first;
			highest = unitPrio.second;
		}
	}
	return priority;
}

bc_UnitType PolicyOverlord::HighestPriority()
{
	bc_UnitType priority;
	float highest = 0.0f;
	for (auto& unitPrio : PlayerData::pd->unitPriority) {
		//std::cout << unitPrio.first << " unit type has priority " << unitPrio.second << "\n";
		if (unitPrio.second > highest) {
			priority = unitPrio.first;
			highest = unitPrio.second;
		}
	}
	return priority;
}

PolicyOverlord::PolicyOverlord()
{
	auto holdTheLine = std::make_shared<Policy>("hold_the_line");
	holdTheLine->Evaluate = policy::HoldTheLineEvaluate;
	holdTheLine->Execute = policy::HoldTheLineExecute;
	policies[Knight].push_back(holdTheLine);
	policies[Ranger].push_back(holdTheLine);
	policies[Healer].push_back(holdTheLine);
	policies[Mage].push_back(holdTheLine);

	auto groupUp = std::make_shared<Policy>("group_up");
	groupUp->Evaluate = policy::GroupUpEvaluate;
	groupUp->Execute = policy::GroupUpExecute;
	policies[Knight].push_back(groupUp);
	policies[Ranger].push_back(groupUp);

	auto robot1 = std::make_shared<Policy>("fear"); // TODO Run towards friendly units
	robot1->Evaluate = policy::AvoidDamageEvaluate;
	robot1->Execute = policy::AvoidDamageExecute;
	policies[Worker].push_back(robot1);
	//policies[Knight].push_back(robot1);
	//policies[Healer].push_back(robot1);
	//policies[Ranger].push_back(robot1);
	//policies[Mage].push_back(robot1);

	auto wander = std::make_shared<Policy>("wander");
	wander->Evaluate = policy::WanderEvaluate;
	wander->Execute = policy::WanderExecute;
	policies[Knight].push_back(wander);
	policies[Ranger].push_back(wander);
	policies[Mage].push_back(wander);
	policies[Healer].push_back(wander);
	//policies[Worker].push_back(wander);

	auto worker1 = std::make_shared<Policy>("harvest_karbonite");
	worker1->Evaluate = policy::WorkerHarvestKarboniteEvaluate;
	worker1->Execute = policy::WorkerHarvestKarboniteExecute;
	policies[Worker].push_back(worker1);

	auto worker3 = std::make_shared<Policy>("replicate");
	worker3->Evaluate = policy::WorkerReplicateEvaluate;
	worker3->Execute = policy::WorkerReplicateExecute;
	policies[Worker].push_back(worker3);

	auto fighter2 = std::make_shared<Policy>("seek_enemy");
	fighter2->Evaluate = policy::SeekEnemyEvaluate;
	fighter2->Execute = policy::SeekEnemyExecute;
	policies[Knight].push_back(fighter2);
	policies[Mage].push_back(fighter2);
	policies[Ranger].push_back(fighter2);
	policies[Healer].push_back(fighter2); // TODO Custom healer seek behavior

	auto seekControl = std::make_shared<Policy>("seek_control_point");
	seekControl->Evaluate = policy::SeekControlEvaluate;
	seekControl->Execute = policy::SeekControlExecute;
	policies[Knight].push_back(seekControl);
	policies[Mage].push_back(seekControl);
	policies[Ranger].push_back(seekControl);
	policies[Healer].push_back(seekControl);

	// Determine if moving towards enemies before attacking would provide a higher value target
	auto fighter3 = std::make_shared<Policy>("move_attack_advantage");

	auto mage1 = std::make_shared<Policy>("mage_splash_attack");
	mage1->Evaluate = policy::MageAttackEvaluate;
	mage1->Execute = policy::AttackExecute;
	policies[Mage].push_back(mage1);

	// Request
	auto mage2 = std::make_shared<Policy>("blink");

	auto ranger1 = std::make_shared<Policy>("ranger_attack");
	ranger1->Evaluate = policy::RangerAttackEvaluate;
	ranger1->Execute = policy::AttackExecute;
	policies[Ranger].push_back(ranger1);

	auto ranger2 = std::make_shared<Policy>("scout");
	auto ranger3 = std::make_shared<Policy>("snipe");
	ranger3->Evaluate = policy::SnipeEvaluate;
	ranger3->Execute = policy::SnipeExecute;
	policies[Ranger].push_back(ranger3);

	// Attack best target
	auto knight1 = std::make_shared<Policy>("knight_attack");
	knight1->Evaluate = policy::KnightAttackEvaluate;
	knight1->Execute = policy::AttackExecute;
	policies[Knight].push_back(knight1);

	auto knight2 = std::make_shared<Policy>("javelin");
	knight2->Evaluate = policy::KnightJavelinEvaluate;
	knight2->Execute = policy::KnightJavelinExecute;
	policies[Knight].push_back(knight2);

	auto healer1 = std::make_shared<Policy>("heal");
	healer1->Evaluate = policy::HealerHealEvaluate;
	healer1->Execute = policy::HealerHealExecute;
	policies[Healer].push_back(healer1);

	auto healer2 = std::make_shared<Policy>("overcharge");
	healer2->Evaluate = policy::HealerOverchargeEvaluate;
	healer2->Execute = policy::HealerOverchargeExecute;
	policies[Healer].push_back(healer2);

	// Move towards nearby Karbonite deposits
	auto worker5 = std::make_shared<Policy>("seek_karbonite");
	worker5->Evaluate = policy::WorkerSeekKarboniteEvaluate;
	worker5->Execute = policy::WorkerSeekKarboniteExecute;
	policies[Worker].push_back(worker5);

	if (GameController::Planet() == Earth) {
		auto robot2 = std::make_shared<Policy>("load_rocket");
		robot2->Evaluate = policy::LoadRocketEvaluate;
		robot2->Execute = policy::LoadRocketExecute;
		policies[Knight].push_back(robot2);
		policies[Ranger].push_back(robot2);
		policies[Mage].push_back(robot2);
		policies[Worker].push_back(robot2);

		auto seekRocket = std::make_shared<Policy>("seek_rocket");
		seekRocket->Evaluate = policy::SeekRocketEvaluate;
		seekRocket->Execute = policy::SeekRocketExecute;
		policies[Worker].push_back(seekRocket);
		//policies[Knight].push_back(seekRocket);
		policies[Ranger].push_back(seekRocket);
		policies[Mage].push_back(seekRocket);
		policies[Healer].push_back(seekRocket);

		auto worker4 = std::make_shared<Policy>("seek_build");
		worker4->Evaluate = policy::WorkerSeekBuildEvaluate;
		worker4->Execute = policy::WorkerSeekBuildExecute;
		policies[Worker].push_back(worker4);

		auto worker5 = std::make_shared<Policy>("fear_built");
		worker5->Evaluate = policy::WorkerFearBuiltEvaluate;
		worker5->Execute = policy::WorkerFearBuiltExecute;
		policies[Worker].push_back(worker5);

		auto worker6 = std::make_shared<Policy>("build");
		worker6->Evaluate = policy::WorkerBuildEvaluate;
		worker6->Execute = policy::WorkerBuildExecute;
		policies[Worker].push_back(worker6);

		auto worker7 = std::make_shared<Policy>("repair");

		auto factory1 = std::make_shared<Policy>("unload_factory");
		factory1->Evaluate = policy::FactoryUnloadEvaluate;
		factory1->Execute = policy::FactoryUnloadExecute;
		policies[Factory].push_back(factory1);

		auto factory2 = std::make_shared<Policy>("produce_unit");
		factory2->Evaluate = policy::FactoryProduceEvaluate;
		factory2->Execute = policy::FactoryProduceExecute;
		policies[Factory].push_back(factory2);

		auto rocket2 = std::make_shared<Policy>("launch");
		rocket2->Evaluate = policy::RocketLaunchEvaluate;
		rocket2->Execute = policy::RocketLaunchExecute;
		policies[Rocket].push_back(rocket2);
	}
	else {
		auto rocket1 = std::make_shared<Policy>("unload_rocket");
		rocket1->Evaluate = policy::RocketUnloadEvaluate;
		rocket1->Execute = policy::RocketUnloadExecute;
		policies[Rocket].push_back(rocket1);
	}

}

