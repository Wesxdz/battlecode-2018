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
#include <math.h>
#include "AsteroidPattern.h"
#include "PolicyOverlord.h"

std::map<uint16_t, std::vector<uint16_t>> BuilderOverlord::buildProjects;
std::map<uint16_t, std::vector<uint16_t>> BuilderOverlord::rockets;
std::vector<bc_UnitType> BuilderOverlord::rocketLoadType;
std::map<Section*, FlowChart> BuilderOverlord::findKarbonite;
std::map<uint16_t, int> BuilderOverlord::miningSuccess;

BuilderOverlord::BuilderOverlord()
{
	rocketLoadType.push_back(Worker);
	rocketLoadType.push_back(Knight);
	rocketLoadType.push_back(Ranger);
	rocketLoadType.push_back(Mage);
	rocketLoadType.push_back(Healer);
	CreateKarboniteFlows();
}

void BuilderOverlord::Update()
{
	if (GameController::Planet() == Earth) {
		for (auto section : Section::earthSections) {
			if (section->status == StartStatus::Team || section->status == StartStatus::Mixed) {
				if (section->karboniteDeposits.size() > 0) {
					auto mined = std::remove_if(section->karboniteDeposits.begin(), section->karboniteDeposits.end(), [](MapLocation& location) {
						return location.IsVisible() && location.Karbonite() == 0;
					});
					if (mined != section->karboniteDeposits.end()) {
						section->karboniteDeposits.erase(mined);
					}
				}
			}
		}
	}
	else {
		if (GameController::Round() > 100) {
			for (auto section : Section::marsSections) {
				if (section->karboniteDeposits.size() > 0) {
					section->karboniteDeposits.erase(std::remove_if(section->karboniteDeposits.begin(), section->karboniteDeposits.end(), [](MapLocation& location) {
						return location.IsVisible() && location.Karbonite() == 0;
					}));
				}
			}
		}
		if (AsteroidPattern::WillAsteroidStrike(GameController::Round())) {
			AsteroidStrike strike = AsteroidPattern::Strike(GameController::Round());
			MapLocation landing = strike.Loc();
			if (landing.IsPassable()) { // Otherwise not in section!
				Section* sectionHit = Section::Get(landing);
				auto deposit = std::find(sectionHit->karboniteDeposits.begin(), sectionHit->karboniteDeposits.end(), landing);
				if (deposit == sectionHit->karboniteDeposits.end()) {
					//std::cout << "Adding asteroid hit " << landing.X() << ", " << landing.Y() << std::endl;
					sectionHit->karboniteDeposits.push_back(landing);
				}
			}
		}
	}
	if (GameController::Round() % 5 == 0) {
		CreateKarboniteFlows();
	}
	DesireUnits();
}

void BuilderOverlord::DesireUnits() {
	uint32_t round = GameController::Round();

	// TODO: Set Rocket Desred Types

	// Cant build anything, so replicate workers
	if (GameController::Planet() == Mars) {
		for (auto& priority : PlayerData::pd->unitPriority) {
			priority.second = 0.0f;
		}
		if (GameController::Round() > 749) {
			PlayerData::pd->unitPriority[Worker] = 1.0f;
		}
		else {
			PlayerData::pd->unitPriority[Knight] = 1.0f; // Dont' build anything
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

	int workerProductionAmo = PlayerData::pd->inProductionCounts[bc_UnitType::Worker];
	int knightProductionAmo = PlayerData::pd->inProductionCounts[bc_UnitType::Knight];
	int mageProductionAmo = PlayerData::pd->inProductionCounts[bc_UnitType::Mage];
	int rangerProductionAmo = PlayerData::pd->inProductionCounts[bc_UnitType::Ranger];
	int healerProductionAmo = PlayerData::pd->inProductionCounts[bc_UnitType::Healer];
	int factoryProductionAmo = PlayerData::pd->inProductionCounts[bc_UnitType::Factory];
	int rocketProductionAmo = PlayerData::pd->inProductionCounts[bc_UnitType::Rocket];
	int totalProductionAmo = workerProductionAmo + knightProductionAmo + mageProductionAmo + rangerProductionAmo + healerProductionAmo + factoryProductionAmo + rocketProductionAmo;

	{
		float workerPriority = .0f;
		if (round < 25 || workerAmo < 10) workerPriority += 1.1f;
		PlayerData::pd->unitPriority[bc_UnitType::Worker] = workerPriority;
	}

	// Rocket Priority
	{
		bc_ResearchInfo* info = bc_GameController_research_info(GameController::gc);
		if (bc_ResearchInfo_get_level(info, Rocket) == 0) {
			PlayerData::pd->unitPriority[Rocket] = 0.0f;
		}
		else {
			float rocketPriority = .0f;
			int reachableLocations = 0;
			for (Section* section : Section::earthSections) {
				if (section->status == StartStatus::Mixed || section->status == StartStatus::Team) {
					reachableLocations += section->locations.size();
				}
			}
			//std::cout << totalAmo << "/" << reachableLocations << std::endl;
			float filled = totalAmo / reachableLocations;
			rocketPriority += filled * 6;
			rocketPriority += (round / 450.0f)/(rocketAmo + 1); // More rocket amounts signify they have not been loaded
			PlayerData::pd->unitPriority[bc_UnitType::Rocket] = rocketPriority;
			//std::cout << rocketPriority << " rocket prio" << std::endl;
		}
		delete_bc_ResearchInfo(info);
	}

	// Factory Priority
	{
	float factoryToTeam = static_cast<float>(factoryAmo) / (totalAmo + totalProductionAmo + 1);

	float factoryPriority = .0f;

	// Always want to be producing factories. Compare to Karb reserves
	factoryPriority = (1.0f - (factoryToTeam * 10.0f)) * (GameController::Karbonite() / 100.0f);
	if (factoryAmo == 0 && round > 3) {
		uintptr_t mapSize = MapUtil::EARTH_MAP_HEIGHT * MapUtil::EARTH_MAP_WIDTH;
		float mapRatio = mapSize/2500.0f; // If the map is small, we should build factories earlier
		float roundRatio = round / 50.0f;
		float timeBonus = roundRatio/mapRatio/(factoryAmo + 2);
		factoryPriority += timeBonus;
	}
	PlayerData::pd->unitPriority[bc_UnitType::Factory] = factoryPriority;
	}

	// Knight Priority
	{
		float knightPriority = 0.0f;

		// Knights will always be somewhat valuable
		// They do good damage, limited range, high HP, good Defense
		// If we lack a "Defense", aka Knights, then we should get a minimum in for sure.
		// After that, we should only build more to recuperate our defense, rush, or strategise
		//uintptr_t width = bc_PlanetMap_width_get(GameController::earth);
		//if (width < 1) { width = 0; }
		//float knightToMap = static_cast<float>(knightAmo) / width;

		//if (knightAmo < width) {
		//	float knightAmof = static_cast<float>(knightAmo);
		//	knightPriority = .5f - (width / knightAmof + 1);
		//	if (knightPriority < .5f) { knightPriority = .5f; }
		//	if (knightPriority > width / 10.0f) { knightPriority = width / 10.0f; }
		//}
		//else {
		//	knightPriority = .3f;
		//}

		PlayerData::pd->unitPriority[bc_UnitType::Knight] = knightPriority;
	}

	// Ranger Priority
	{
		float rangerPriority = .0f;

		// Rangers can never go wrong (RANGERS NERFED IN SPRINT)
		// Good health, long range, good damage

		//for (Section* section : Section::earthSections) {
		//	if (section->status == Team) {

		//	}
		//}
		float tilesToEnemy = PlayerData::pd->teamSpawnPositions[0].TilesTo(PlayerData::pd->enemySpawnPositions[0]) + 1;
		float actualTilesToEnemy = Pathfind::GetFuzzyFlowTurns(PlayerData::pd->teamSpawnPositions[0], PlayerData::pd->enemySpawnPositions[0]);
		float extraTravel = actualTilesToEnemy / tilesToEnemy;
		if (extraTravel > 1.5) { // If we had to walk a maze, better do it with rangers...
			rangerPriority = 0.9f;
		}
		else {
			rangerPriority = 0.8f;
		}
		if (PlayerData::pd->teamUnitCounts[Ranger] < 4) rangerPriority += .1f;
		PlayerData::pd->unitPriority[bc_UnitType::Ranger] = rangerPriority;
	}

	// Mage Priority
	{
		float magePriority = .0f;

		float mapSize = MapUtil::earthPassableLocations.size() / 2.0f;
		float enemyToMap = (totalEnemyAmo / mapSize) * 10;

		// If enemy is grouped up, then we should use mages to deal a lot of damage;
		if (enemyToMap > 1.0f) {
			magePriority = 1.0f;
		}

		PlayerData::pd->unitPriority[bc_UnitType::Mage] = magePriority;
	}

	// Healer Priority
	{
		float healerPriority = .0f;

		float ratio = (knightAmo + mageAmo + rangerAmo) / (healerAmo + 1);
		if (ratio / 5 > 1) {
			healerPriority = 1.0f;
		}

		PlayerData::pd->unitPriority[bc_UnitType::Healer] = healerPriority;
	}

	if (factoryAmo + factoryProductionAmo == 0) {
		PlayerData::pd->unitPriority[Knight] = 0.0f;
		PlayerData::pd->unitPriority[Healer] = 0.0f;
		PlayerData::pd->unitPriority[Ranger] = 0.0f;
		PlayerData::pd->unitPriority[Mage] = 0.0f;
	}

}

void BuilderOverlord::ManageProduction()
{
	auto units = GameController::Units(MyTeam);
	std::vector<units::Worker> workers = VecUnit::Wrap<units::Worker>(bc_GameController_my_units(GameController::gc));
	bc_UnitType highestPriority = PolicyOverlord::HighestPriority();
	if (highestPriority == Factory || highestPriority == Rocket) {

	}
	std::vector<units::Factory> factories = VecUnit::Wrap<units::Factory>(bc_GameController_my_units(GameController::gc));
	std::sort(factories.begin(), factories.end(), [](units::Factory& a, units::Factory& b) {
		return ProductionScore(a) > ProductionScore(b);
	});
	for (units::Factory& factory : factories) {
		if (ProductionScore(factory) <= 0) break;
		bc_UnitType prio = Priority(factory);
		if (factory.CanProduce(prio)) {
			factory.Produce(prio);
		}
	}

}

float BuilderOverlord::ProductionScore(units::Factory & factory)
{
	if (!factory.IsBuilt() || factory.IsProducing()) return 0.0f;
	return 0.0f;
}

bc_UnitType BuilderOverlord::Priority(units::Factory & factory)
{
	return bc_UnitType();
}

float BuilderOverlord::FactoryPlacementScore(MapLocation location)
{
	return 0.0f;
}

float BuilderOverlord::RocketPlacementScore(MapLocation location)
{
	return 0.0f;
}

std::list<Section*> BuilderOverlord::PrioritizeSections(std::list<Section*> sections)
{
	//if (bc_MapLocation_planet_get(sections.front()->locations[0]) == Earth) {
	//	std::sort(sections.begin(), sections.end(), [](Section* a, Section* b) {
	//		if (a->status == StartStatus::Enemy || a->status == StartStatus::None) return false;
	//		if (a->status == StartStatus::Mixed && b->status != StartStatus::Mixed) return false; // Mixed sections always take priority
	//		return a->TotalKarbonite() > b->TotalKarbonite();
	//	});
	//}
	//else {
	//	std::sort(sections.begin(), sections.end(), [](Section* a, Section* b) {
	//		return a->TotalKarbonite() > b->TotalKarbonite();
	//	});
	//}
	return sections;
}

void BuilderOverlord::CreateKarboniteFlows()
{
	if (GameController::Planet() == Earth) {
		for (Section* section : Section::earthSections) {
			if (section->status == StartStatus::Mixed || section->status == StartStatus::Team) {
				if (section->karboniteDeposits.size() > 0) {
					findKarbonite[section] = Pathfind::CreateFlowChart(section->karboniteDeposits);
				}
			}
		}
	}
	else {
		for (Section* section : Section::marsSections) {
			if (section->karboniteDeposits.size() > 0) {
				findKarbonite[section] = Pathfind::CreateFlowChart(section->karboniteDeposits);
			}
		}
	}
}
