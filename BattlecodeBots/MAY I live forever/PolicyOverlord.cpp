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
units::Unit PolicyOverlord::storeUnit;
MapLocation PolicyOverlord::storeLocation;

void PolicyOverlord::Update()
{
	//std::cout << HighestPriority() << " is highest priority\n";
	//std::map<uint16_t, std::vector<std::string>> failedPolicies;
	bool policyTaken = false;
	do { // Loop through all units evaluating policies until no policy is executed
		policyTaken = false;
		std::vector<units::Unit> units = GameController::Units(MyTeam);
		for (auto& unit : units) {
			if (unit.Loc().IsInGarrison()) continue;
			std::sort(policies[unit.type].begin(), policies[unit.type].end(), [&unit](std::shared_ptr<Policy>& a, std::shared_ptr<Policy>& b) {
				return a->Evaluate(unit) > b->Evaluate(unit); // reverse sort
			});
			auto policyChosen = policies[unit.type].begin(); // Choose the max policy
			while (true) { // Loop through the policies until one is successfully executed or there are no more left
				bool isValidPolicy = policyChosen != policies[unit.type].end();
				if (!isValidPolicy) {
					break;
				}
				if ((*policyChosen)->Evaluate(unit) <= 0.0f) { // Eval stores variables!!
					break;
				}
				if ((*policyChosen)->Command(unit)) {
					policyTaken = true;
					break;
				}
				else {
					//failedPolicies[unit.id].push_back((*policyChosen)->name);
				}
				policyChosen++;
			}
		}
	} while (policyTaken);
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
	auto robot1 = std::make_shared<Policy>("fear"); // TODO Run towards friendly units
	robot1->Evaluate = policy::AvoidDamageEvaluate;
	robot1->Execute = policy::AvoidDamageExecute;
	policies[Worker].push_back(robot1);
	policies[Ranger].push_back(robot1);

	auto robot2 = std::make_shared<Policy>("load_rocket");
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
	//policies[Worker].push_back(robot3);

	auto worker1 = std::make_shared<Policy>("harvest_karbonite");
	worker1->Evaluate = policy::WorkerHarvestKarboniteEvaluate;
	worker1->Execute = policy::WorkerHarvestKarboniteExecute;
	policies[Worker].push_back(worker1);

	auto worker2 = std::make_shared<Policy>("blueprint");
	worker2->Evaluate = policy::WorkerBlueprintEvaluate;
	worker2->Execute = policy::WorkerBlueprintExecute;
	policies[Worker].push_back(worker2);

	auto worker3 = std::make_shared<Policy>("replicate");
	worker3->Evaluate = policy::WorkerReplicateEvaluate;
	worker3->Execute = policy::WorkerReplicateExecute;
	policies[Worker].push_back(worker3);

	auto worker4 = std::make_shared<Policy>("seek_build");
	worker4->Evaluate = policy::WorkerSeekBuildEvaluate;
	worker4->Execute = policy::WorkerSeekBuildExecute;
	policies[Worker].push_back(worker4);

	// Move towards nearby Karbonite deposits
	auto worker5 = std::make_shared<Policy>("seek_karbonite");
	worker5->Evaluate = policy::WorkerSeekKarboniteEvaluate;
	worker5->Execute = policy::WorkerSeekKarboniteExecute;
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

	auto rocket1 = std::make_shared<Policy>("unload_rocket");
	auto rocket2 = std::make_shared<Policy>("launch");

	// Charge towards enemies if there are enough friendly units nearby
	auto fighter1 = std::make_shared<Policy>("courage");

	auto fighter2 = std::make_shared<Policy>("seek_enemy");
	fighter2->Evaluate = policy::SeekEnemyEvaluate;
	fighter2->Execute = policy::SeekEnemyExecute;
	policies[Knight].push_back(fighter2);
	policies[Mage].push_back(fighter2);
	policies[Ranger].push_back(fighter2);

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

	// Stay out of vision range of enemy units, avoid letting units into MinAttackRange
	auto ranger4 = std::make_shared<Policy>("kite");


	// Attack best target
	auto knight1 = std::make_shared<Policy>("knight_attack");
	knight1->Evaluate = policy::KnightAttackEvaluate;
	knight1->Execute = policy::AttackExecute;
	policies[Knight].push_back(knight1);

	auto knight2 = std::make_shared<Policy>("javelin");

	auto healer1 = std::make_shared<Policy>("heal");
	auto healer2 = std::make_shared<Policy>("overcharge");

}

