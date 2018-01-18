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
	auto robot1 = std::make_shared<Policy>("fear");
	robot1->Evaluate = policy::AvoidDamageEval;
	robot1->Execute = policy::AvoidDamageExecute;
	policies[Worker].push_back(robot1);

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
	policies[Worker].push_back(robot3);

	auto robot4 = std::make_shared<Policy>("diversion");

	auto worker1 = std::make_shared<Policy>("harvest_karbonite");
	worker1->Evaluate = policy::WorkerHarvestKarboniteEvaluate;
	worker1->Execute = policy::WorkerHarvestKarboniteExecute;
	policies[Worker].push_back(worker1);

	auto worker2 = std::make_shared<Policy>("blueprint");
	worker2->Evaluate = policy::WorkerBlueprintEvaluate;
	worker2->Execute = policy::WorkerBlueprintExecute;
	policies[Worker].push_back(worker2);

	auto worker4 = std::make_shared<Policy>("replicate");
	auto worker5 = std::make_shared<Policy>("seek_karbonite");
	auto worker6 = std::make_shared<Policy>("build");
	auto worker7 = std::make_shared<Policy>("repair");

	auto factory1 = std::make_shared<Policy>("unload_factory");
	auto factory2 = std::make_shared<Policy>("produce_unit");

	auto rocket1 = std::make_shared<Policy>("unload_rocket");
	auto rocket2 = std::make_shared<Policy>("launch");

	auto fighter1 = std::make_shared<Policy>("courage"); // Move towards enemies
	auto fighter2 = std::make_shared<Policy>("attack");

	auto mage1 = std::make_shared<Policy>("splash_attack");
	auto mage2 = std::make_shared<Policy>("blink");

	auto ranger1 = std::make_shared<Policy>("scout");
	auto ranger2 = std::make_shared<Policy>("snipe");

	auto knight1 = std::make_shared<Policy>("javelin");

	auto healer1 = std::make_shared<Policy>("heal");
	auto healer2 = std::make_shared<Policy>("overcharge");

}

