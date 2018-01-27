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
#include "Strategist.h"
#include "CombatOverlord.h"

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
					//section->karboniteDeposits.erase(std::remove_if(section->karboniteDeposits.begin(), section->karboniteDeposits.end(), [](MapLocation& location) {
					//	return location.IsVisible() && location.Karbonite() == 0;
					//}));
					auto mined = std::remove_if(section->karboniteDeposits.begin(), section->karboniteDeposits.end(), [](MapLocation& location) {
						return location.IsVisible() && location.Karbonite() == 0;
					});
					if (mined != section->karboniteDeposits.end()) {
						section->karboniteDeposits.erase(mined);
					}
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
			else {
				for (bc_Direction direction : constants::directions_adjacent) {
					MapLocation neighbor = MapLocation::Neighbor(landing, direction);
					if (neighbor.IsValid() && neighbor.IsPassable()) {
						Section* nearbySection = Section::Get(neighbor);
						auto deposit = std::find(nearbySection->karboniteDeposits.begin(), nearbySection->karboniteDeposits.end(), neighbor);
						if (deposit == nearbySection->karboniteDeposits.end()) {
							nearbySection->karboniteDeposits.push_back(neighbor);
						}
						break;
					}
				}
			}
		} 
	}
	if (GameController::Round() % 5 == 0) {
		CreateKarboniteFlows();
	}
	DesireUnits();
	ManageProduction();
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
	
	// Worker Priority
	{
		float workerPriority = .0f;
		if (round < 25 || workerAmo < 10) workerPriority += 1.0f;
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
			rocketPriority += (round / 450.0f) / (rocketAmo + 1); // More rocket amounts signify they have not been loaded
			int availableSpace = 0;
			for (Section* section : Section::earthSections) {
				if (section->status == Team) {
					availableSpace += section->locations.size();
				}
				else if (section->status == Mixed) {
					availableSpace += section->locations.size() / 2;
				}
			}
			float spaceTaken = totalAmo / availableSpace;
			if (spaceTaken > 0.5f) {
				rocketPriority += 1000;
			}
			PlayerData::pd->unitPriority[bc_UnitType::Rocket] = rocketPriority;
		}
		delete_bc_ResearchInfo(info);
	}

	// Factory Priority
	{
		float factoryPriority = .0f;

		// Always want to be producing factories. Compare to Karb reserves
		factoryPriority = GameController::Karbonite() / 200.0f;
		float mapSize = (MapUtil::EARTH_MAP_WIDTH * MapUtil::EARTH_MAP_HEIGHT) / 2500; // .16 to 1
		if (round > 3 && round < 30) {
			factoryPriority += (1 / mapSize) / (factoryAmo + 1)/((workerAmo + 1)/8);
		}
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

		// Rangers can never go wrong (RANGERS NERFED AFTER SPRINT AND SEEDING)
		// Good health, long range, good damage

		float tilesToEnemy = PlayerData::pd->teamSpawnPositions[0].TilesTo(PlayerData::pd->enemySpawnPositions[0]) + 1;
		float actualTilesToEnemy = Pathfind::GetFuzzyFlowTurns(PlayerData::pd->teamSpawnPositions[0], PlayerData::pd->enemySpawnPositions[0]);
		float extraTravel = actualTilesToEnemy / tilesToEnemy;
		if (extraTravel > 1.5) { // If we had to walk a maze, better do it with rangers...
			rangerPriority = 1.0f;
		}
		float ratio = (knightAmo + mageAmo) / (rangerAmo + 1);
		if (ratio > 1) {
			rangerPriority = 0.9f;
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
		if (PlayerData::pd->enemyUnitCounts[Ranger] > 1) magePriority = 0.0f; // TODO, unless suicide bomber or sending to Mars

		PlayerData::pd->unitPriority[bc_UnitType::Mage] = magePriority;
	}

	// Healer Priority
	{
		float healerPriority = .0f;

		float ratio = (knightAmo + mageAmo + rangerAmo) / (healerAmo + healerProductionAmo + 1);
		healerPriority = ratio / 3;

		PlayerData::pd->unitPriority[bc_UnitType::Healer] = healerPriority;
	}

	if (Strategist::strategy == Strategy::ShieldFormation) {
		PlayerData::pd->unitPriority[Knight] += 0.1f;
	}
	else if (Strategist::strategy == Strategy::TerroristOvercharge) {
		PlayerData::pd->unitPriority[Knight] = 0.0f;
		PlayerData::pd->unitPriority[Ranger] = 0.9f;
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
		if (GameController::Karbonite() < bc_UnitType_blueprint_cost(highestPriority)) return;
		float highestScore = -1000000.0f;
		uint16_t toBuild;
		bc_Direction buildDirection;
		for (units::Worker& worker : workers) {
			if (!worker.Loc().IsOnMap()) continue;
			for (bc_Direction direction : constants::directions_adjacent) {
				if (worker.CanBlueprint(highestPriority, direction) && MapLocation::Neighbor(worker.Loc().ToMapLocation(), direction).IsValid()) {
					float placementScore = highestPriority == Factory ? 
						FactoryPlacementScore(MapLocation::Neighbor(worker.Loc().ToMapLocation(), direction)) :
						RocketPlacementScore(MapLocation::Neighbor(worker.Loc().ToMapLocation(), direction));
					if (placementScore > highestScore) {
						highestScore = placementScore;
						toBuild = worker.id;
						buildDirection = direction;
					}
				}
			}
		}
		units::Worker builder = bc_GameController_unit(GameController::gc, toBuild);
		builder.Blueprint(highestPriority, buildDirection);
		MapLocation buildLocation = MapLocation::Neighbor(builder.Loc().ToMapLocation(), buildDirection);
		units::Structure build = buildLocation.Occupant();
		BuilderOverlord::buildProjects[build.id].push_back(builder.id);
		if (highestPriority == bc_UnitType::Rocket) {
			BuilderOverlord::rockets[build.id]; // Add rockets
		}
	}
	//std::vector<units::Factory> factories = VecUnit::Wrap<units::Factory>(bc_GameController_my_units(GameController::gc));
	//std::sort(factories.begin(), factories.end(), [](units::Factory& a, units::Factory& b) {
	//	return ProductionScore(a) > ProductionScore(b);
	//});
	//for (units::Factory& factory : factories) {
	//	if (ProductionScore(factory) <= 0) break;
	//	bc_UnitType prio = Priority(factory);
	//	if (factory.CanProduce(prio)) {
	//		factory.Produce(prio);
	//	}
	//}

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
	float score = 0.0f;
	for (MapLocation& point : CombatOverlord::controlPoints) {
		score -= location.DistanceTo(point)/30;
	}
	int adjacentImpassable = 0;
	for (bc_Direction direction : constants::directions_adjacent) {
		MapLocation neighbor = MapLocation::Neighbor(location, direction);
		if (!neighbor.IsPassable()) {
			adjacentImpassable++;
		}
		else if (neighbor.IsOccupied()) {
			units::Unit occupant = neighbor.Occupant();
			if (!Utility::IsRobot(occupant.type)) {
				adjacentImpassable++;
			}
		}
	}
	if (adjacentImpassable == 2) {
		score -= 10000.0f; // This might block a chokepoint
	}
	else {
		score += adjacentImpassable * 10.0f;
	}
	std::vector<units::Unit> nearby = location.NearbyUnits(16);
	score -= CombatOverlord::fear.GetInfluence(location);
	for (units::Unit& unit : nearby) {
		if (unit.type == Worker) {
			score += 10.0f;
		}
	}
	return score;
}

float BuilderOverlord::RocketPlacementScore(MapLocation location)
{
	float score = 0.0f;
	int adjacentImpassable = 0;
	for (bc_Direction direction : constants::directions_adjacent) {
		MapLocation neighbor = MapLocation::Neighbor(location, direction);
		if (!neighbor.IsPassable()) {
			adjacentImpassable++;
		}
	}
	return (8 - adjacentImpassable);
}


void BuilderOverlord::CreateKarboniteFlows()
{
	if (GameController::Planet() == Earth) {
		for (Section* section : Section::earthSections) {
			if (section->status == StartStatus::Mixed || section->status == StartStatus::Team) {
				if (section->karboniteDeposits.size() > 0) {
					std::vector<MapLocation> safeKarbonite = section->karboniteDeposits;
					auto unsafeKarbonite = std::remove_if(safeKarbonite.begin(), safeKarbonite.end(), [](MapLocation& a) {
						return CombatOverlord::damage.GetInfluence(a) > 1.0f;
					});
					if (unsafeKarbonite != safeKarbonite.end()) {
						safeKarbonite.erase(unsafeKarbonite);
					}
					findKarbonite[section] = Pathfind::CreateFlowChart(safeKarbonite);
				}
			}
		}
	}
	else {
		for (Section* section : Section::marsSections) {
			if (section->karboniteDeposits.size() > 0) {
				std::vector<MapLocation> safeKarbonite = section->karboniteDeposits;
				auto unsafeKarbonite = std::remove_if(safeKarbonite.begin(), safeKarbonite.end(), [](MapLocation& a) {
					return CombatOverlord::damage.GetInfluence(a) > 0.5f;
				});
				if (unsafeKarbonite != safeKarbonite.end()) {
					safeKarbonite.erase(unsafeKarbonite);
				}
				findKarbonite[section] = Pathfind::CreateFlowChart(safeKarbonite);
			}
		}
	}
}
