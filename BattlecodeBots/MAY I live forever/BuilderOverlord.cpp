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
std::list<std::shared_ptr<Deposit>> BuilderOverlord::sortedLandings;
std::map<uint16_t, MapLocation> BuilderOverlord::seekKarbonite;

BuilderOverlord::BuilderOverlord()
{
	//if (GameController::Planet() == Earth) {
	//	sortedLandings = Deposit::earthDeposits;
	//	MapLocation start = PlayerData::pd->teamSpawnPositions[0];
	//	std::sort(sortedLandings.begin(), sortedLandings.end(), [&start](std::shared_ptr<Deposit>& a, std::shared_ptr<Deposit>& b) {
	//		MapLocation aloc = bc_MapLocation_clone(a->landing);
	//		MapLocation bloc = bc_MapLocation_clone(b->landing);
	//		return start.DistanceTo(aloc) < start.DistanceTo(bloc);
	//	});
	//}
}

void BuilderOverlord::Update()
{
	if (PlayerData::pd->karboniteDeposits.size() > 0) {
		PlayerData::pd->karboniteDeposits.erase(std::remove_if(PlayerData::pd->karboniteDeposits.begin(), PlayerData::pd->karboniteDeposits.end(), [](MapLocation& location) {
			return location.IsVisible() && location.Karbonite() == 0;
		}));
	}
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
		if (GameController::Round() > 749) {
			PlayerData::pd->unitPriority[Worker] = 1.0f;
		}
		else {
			PlayerData::pd->unitPriority[Knight] = 1.0f; // We can't actually build them...
		}
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
		if (round < 25 || workerAmo < 10) workerPriority += 1.0f;
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
		uintptr_t width = bc_PlanetMap_width_get(GameController::earth);
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


		float ratio = (knightAmo + mageAmo) / (rangerAmo + 1);
		if (ratio > 1) {
			rangerPriority = 1.0f;
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
