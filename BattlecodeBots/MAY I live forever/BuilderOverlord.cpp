#include "BuilderOverlord.h"
#include "GameController.h"
#include "Worker.h"
#include "Unit.h"
#include "Structure.h"
#include "Factory.h"
#include "Rocket.h"
#include <iostream>
#include "MapUtil.h"
#include "Constants.h"
#include "Pathfind.h"
#include <algorithm>
#include "VecUnit.h"

std::map<uint16_t, std::vector<uint16_t>> BuilderOverlord::buildProjects;

BuilderOverlord::BuilderOverlord()
{
}

void BuilderOverlord::Update()
{
	// TODO Fix
	//std::remove_if(PlayerData::pd->karboniteDeposits.begin(), PlayerData::pd->karboniteDeposits.end(), [](MapLocation& location) {
	//	return location.IsVisible() && location.Karbonite() > 0;
	//});
	//std::cout << PlayerData::pd->karboniteDeposits.size() << " karbonite deposits\n";
	DesireUnits();
}

void BuilderOverlord::DesireUnits() {
	uint32_t round = GameController::Round();

	// Cant build anything, so replicate workers
	if (GameController::Planet() == Mars) {
		for (auto& priority : PlayerData::pd->unitPriority) {
			priority.second = 0.0f;
		}
		PlayerData::pd->unitPriority[bc_UnitType::Worker] = 1.0f;
		return;
	}

	int workerAmo = PlayerData::pd->teamUnitCounts[bc_UnitType::Worker];
	int knightAmo = PlayerData::pd->teamUnitCounts[bc_UnitType::Knight];
	int mageAmo = PlayerData::pd->teamUnitCounts[bc_UnitType::Mage];
	int rangerAmo = PlayerData::pd->teamUnitCounts[bc_UnitType::Ranger];
	int healerAmo = PlayerData::pd->teamUnitCounts[bc_UnitType::Healer];
	int factoryAmo = PlayerData::pd->teamUnitCounts[bc_UnitType::Factory];
	int rocketAmo = PlayerData::pd->teamUnitCounts[bc_UnitType::Rocket];
	int totalAmo = workerAmo + knightAmo + mageAmo + rangerAmo + healerAmo + factoryAmo + rocketAmo;

	int workerEnemyAmo = PlayerData::pd->enemyUnitCounts[bc_UnitType::Worker];
	int knightEnemyAmo = PlayerData::pd->enemyUnitCounts[bc_UnitType::Knight];
	int mageEnemyAmo = PlayerData::pd->enemyUnitCounts[bc_UnitType::Mage];
	int rangerEnemyAmo = PlayerData::pd->enemyUnitCounts[bc_UnitType::Ranger];
	int healerEnemyAmo = PlayerData::pd->enemyUnitCounts[bc_UnitType::Healer];
	int factoryEnemyAmo = PlayerData::pd->enemyUnitCounts[bc_UnitType::Factory];
	int rocketEnemyAmo = PlayerData::pd->enemyUnitCounts[bc_UnitType::Rocket];
	int totalEnemyAmo = workerEnemyAmo + knightEnemyAmo + mageEnemyAmo + rangerEnemyAmo + healerEnemyAmo + factoryEnemyAmo + rocketEnemyAmo;
	if(totalEnemyAmo < 1){ totalEnemyAmo = 1; }

	int workerProductionAmo = PlayerData::pd->inProductionCounts[bc_UnitType::Worker];
	int knightProductionAmo = PlayerData::pd->inProductionCounts[bc_UnitType::Knight];
	int mageProductionAmo = PlayerData::pd->inProductionCounts[bc_UnitType::Mage];
	int rangerProductionAmo = PlayerData::pd->inProductionCounts[bc_UnitType::Ranger];
	int healerProductionAmo = PlayerData::pd->inProductionCounts[bc_UnitType::Healer];
	int factoryProductionAmo = PlayerData::pd->inProductionCounts[bc_UnitType::Factory];
	int rocketProductionAmo = PlayerData::pd->inProductionCounts[bc_UnitType::Rocket];
	int totalProductionAmo = workerProductionAmo + knightProductionAmo + mageProductionAmo + rangerProductionAmo + healerProductionAmo + factoryProductionAmo + rocketProductionAmo;

	// < 0 = Too many
	// 0 = Don't need
	// 0 - 1 = Could use
	//  > 1 = Need (highest priority)

	// Workers are quite valuable early on for factory production, karb gathering, and / or scouting.
	// However, they can replicate, do we need to buid them?
	// Maybe if we have no workers. High priority then depending on round.

	// Worker Priority
	{
		float workerPriority = .0f;

		/// We only need workers for Karb and Structures
		/*
			I think we don't need too many for Structures actually. After we finish one, we can move the previous to the new.
			CurrTeamSize should only lower worker count, never increase.
			Adjust Karb gather ratio. Keep track of Karb Gather Amo
			Workers shoul never exceed Other Units priority if possible
		*/

		// Determine how much karb is left to mine
		// Determine how many structures need to be built
		// Determine ratio to team. Don't make too many 

		float currKarbAmo = PlayerData::pd->earthStartingKarbonite;
		float currUnbuiltStructureAmo = static_cast<float>(buildProjects.size());
		float currTeamSize = totalAmo - workerAmo;

		// A worker can harvest 3 at a time
		// 8 workers can work on a project

		int unworkingWorkers = workerAmo - currUnbuiltStructureAmo * 8;
		
		// Enough Workers for Structures - Prioritize
		if (unworkingWorkers < 0) {
			workerPriority = .9f;
		} 
		// Enough Workers for Karb - Suggestion
		else {
			// 3 or 4 per turn

			// What turn amount is acceptable for worker to karb gathering
			// Lets say 100

			// 500 / 3 * ? = 100
			// 100 / (3 * ? / 5) = 100

			// ASSUMING 3 KARB PER GATHER
			float turnRecommend = 100.0f;
			float currKarbGather = 3.0f;
			float percent = currKarbAmo / currKarbGather; // Ratio of Karb to Turn with 1 unit

			float currTotalWorkers = (workerAmo + workerProductionAmo);
			float reccTotalWorkers = percent / turnRecommend;
			if (reccTotalWorkers < currTotalWorkers) {
				workerPriority = 1.0f;
			}
			workerPriority = reccTotalWorkers < currTotalWorkers ? .9f : .0f;

			workerPriority -= currTotalWorkers / currTeamSize;
			// 16 workers - 30 rangers = .5333
			// 8 workers - 30 rangers = .2666
			// Don't need that many workers
		}

		//// Unbuilt Structure Ratio
		//float structureAmo = static_cast<float>(buildProjects.size());
		//if(structureAmo < 1) { structureAmo = 1; }
		//float workerToStructure = workerAmo / structureAmo;

		//// Unmined Karb ratio
		//float structureAmo = static_cast<float>(buildProjects.size());
		//if(structureAmo < 1) { structureAmo = 1; }
		//float workerToStructure = workerAmo / structureAmo;

		//float currWorkers = static_cast<float>(workerAmo + workerProductionAmo);
		//float workerToTeam = currWorkers / (totalAmo + totalProductionAmo);

		//float rocketAmof = static_cast<float>(rocketAmo + rocketProductionAmo);
		//if(rocketAmof < 1) { rocketAmof = 1; }
		//float workerToRockets = currWorkers / rocketAmof;

		//// 100 = Bountiful
		//if (currRound < 100) {
		//	// 10ish per structure
		//	workerPriority = 2.0f - (workerToStructure / 10.0f);
		//	if (workerPriority < .3f) {
		//		workerPriority = .3f;
		//	}
		//}
		//// 600 = Rockets
		//else if (currRound > 600) {
		//	// 8 per rocket
		//	workerPriority = 1.0f - (workerToRockets / 8.0f);
		//}
		//// Meh
		//else {
		//	// Need
		//	if (workerToTeam < .1f) {
		//		// 10 per 100...
		//		workerPriority = .1f / workerToTeam;
		//	}
		//	// Could use
		//	else if (workerToTeam < .20f) {
		//		// .1 - .2 = 1 - 0
		//		// x - .1 * 10 = 0 - 1
		//		workerPriority = 1.0f - ((workerToTeam - .1f) * 10.0f);
		//		if (workerPriority > 1.0f) {
		//			workerPriority = 1.0f;
		//		}
		//	}
		//	// Not Needed
		//	else {
		//		workerPriority = .0f;
		//	}
		//}
		PlayerData::pd->unitPriority[bc_UnitType::Worker] = workerPriority;
	}

	// Rocket Priority
	{
		float rocketPriority = .0f;

		// If lacking in Initial karbonite and Early
		//if (round > 40 && round < 200) {
		//	// NEED
		//	if (PlayerData::pd->earthStartingKarbonite < 1.0f) {
		//		rocketPriority = 10000.0f;
		//	}
		//	// Need or could use
		//	else if (PlayerData::pd->earthStartingKarbonite < 1000.0f) {
		//		rocketPriority = 2000.0f / PlayerData::pd->earthStartingKarbonite + 1;
		//	}
		//	else {
		//		// Linear to Round
		//	}
			rocketPriority += (round / 450.0f)/(rocketAmo + 1); // More rocket amounts signify they have not been loaded
			PlayerData::pd->unitPriority[bc_UnitType::Rocket] = rocketPriority;
	}

	// Factory Priority
	{
	float factoryToTeam = static_cast<float>(factoryAmo) / (totalAmo + totalProductionAmo + 1);

	float factoryPriority = .0f;

	// Always want to be producing factories. Compare to Karb reserves
	factoryPriority = (1.0f - (factoryToTeam * 10.0f)) * (GameController::Karbonite() / 100.0f);

	PlayerData::pd->unitPriority[bc_UnitType::Factory] = factoryPriority;
	}

	// Knight Priority
	{
		float knightPriority = .0f;

		// Knights will always be somewhat valuable
		// They do good damage, limited range, high HP, good Defense
		// If we lack a "Defense", aka Knights, then we should get a minimum in for sure.
		// After that, we should only build more to recuperate our defense, rush, or strategise
		uintptr_t width = bc_PlanetMap_width_get(GameController::PlanetMap(bc_Planet::Earth));
		if (width < 1) { width = 0; }
		float knightToMap = static_cast<float>(knightAmo) / width;

		if (knightAmo < width) {
			float knightAmof = static_cast<float>(knightAmo);
			knightPriority = .5f - (width / knightAmof + 1);
			if (knightPriority < .5f) { knightPriority = .5f; }
			if (knightPriority > width / 10.0f) { knightPriority = width / 10.0f; }
		}
		else {
			knightPriority = .3f;
		}

		PlayerData::pd->unitPriority[bc_UnitType::Knight] = knightPriority;
	}

	// Ranger Priority
	{
		float rangerPriority = .0f;

		// Rangers can never go wrong (RANGERS NERFED IN SPRINT)
		// Good health, long range, good damage

		if (rangerAmo + rangerProductionAmo < 0) { // Build a scout first unless map is tiny
			rangerPriority = .7f;
		}
		else {
			rangerPriority = .1f;
		}


		PlayerData::pd->unitPriority[bc_UnitType::Ranger] = rangerPriority;
	}

	// Mage Priority
	{
		float magePriority = .0f;

		float mapSize = MapUtil::earthPassableLocations.size() / 2.0f;
		float enemyToMap = (totalEnemyAmo / mapSize) * 10;

		// If enemy is grouped up, then we should use mages to deal a lot of damage;
		magePriority = enemyToMap;

		PlayerData::pd->unitPriority[bc_UnitType::Mage] = magePriority;
	}

	// Healer Priority
	{
		float healerPriority = .0f;

		float ratio = (knightAmo + mageAmo + rangerAmo) / (healerAmo + 1);
		healerPriority = ratio / 5;

		PlayerData::pd->unitPriority[bc_UnitType::Healer] = healerPriority;
	}

	if (factoryAmo + factoryProductionAmo == 0) {
		PlayerData::pd->unitPriority[Knight] = 0.0f;
		PlayerData::pd->unitPriority[Healer] = 0.0f;
		PlayerData::pd->unitPriority[Ranger] = 0.0f;
		PlayerData::pd->unitPriority[Mage] = 0.0f;
	}

	bc_ResearchInfo* info = bc_GameController_research_info(GameController::gc);
	if (bc_ResearchInfo_get_level(info, Rocket) == 0) {
		PlayerData::pd->unitPriority[Rocket] = 0.0f;
	}
	delete_bc_ResearchInfo(info);

}
