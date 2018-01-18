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

BuilderOverlord::BuilderOverlord()
{
	//PlayerData::pd->desiredUnitCounts[Worker] = 8;
}

void BuilderOverlord::Update()
{
	//auto units = GameController::Units(MyTeam);
	//if (GameController::Round() > 50) {
	//	PlayerData::pd->desiredUnitCounts[Factory] = 6;
	//}
	DetermineDesiredUnits();
}

void BuilderOverlord::DetermineDesiredUnits() {
	// TODO Determine desired Workers, Factories, and Rockets
	//PlayerData::pd->desiredUnitCounts[Worker] = ((749.0f - GameController::Round()) * - bc_UnitType_replicate_cost(Worker) * 2) / 10;

	uint32_t currRound = GameController::Round();

	// Cant build
	if (currRound > 749 || GameController::Planet() == bc_Planet::Mars) {
		PlayerData::pd->unitPriority[bc_UnitType::Worker] = 1.0f; // Maybe change Worker Priority?
		PlayerData::pd->unitPriority[bc_UnitType::Rocket] = .0f;
		PlayerData::pd->unitPriority[bc_UnitType::Factory] = .0f;
		return;
	}

	// Need to take into account Current, Future, and Enemy

	// Knights are countered by Mage and Rangers...
	// Mages are countered by Rangers
	// 

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
	/////////////// BUILDER

	// < 0 = Too many
	// 0 = Don't need
	// 0 - 1 = Could use
	//  > 1 = Need

	// Workers are quite valuable early on for factory production, karb gathering, and / or scouting.
	// However, they can replicate, do we need to buid them?
	// Maybe if we have no workers. High priority then depending on round.

	// Worker Priority
	{
		float workerPriority = .0f;

		float currWorkers = static_cast<float>(workerAmo + workerProductionAmo);
		float workerToTeam = currWorkers / (totalAmo + totalProductionAmo);

		float structureAmo = static_cast<float>(factoryAmo + rocketAmo + factoryProductionAmo + rocketProductionAmo);
		if(structureAmo < 1) { structureAmo = 1; }
		float workerToStructure = currWorkers / structureAmo;

		float rocketAmof = static_cast<float>(rocketAmo + rocketProductionAmo);
		if(rocketAmof < 1) { rocketAmof = 1; }
		float workerToRockets = currWorkers / rocketAmof;

		// 100 = Bountiful
		if (currRound < 100) {
			// 10ish per structure
			workerPriority = 1.0f - (workerToStructure / 10.0f);
			if (workerPriority < .3f) {
				workerPriority = .3f;
			}
		}
		// 600 = Rockets
		else if (currRound > 600) {
			// 8 per rocket
			workerPriority = 1.0f - (workerToRockets / 8.0f);
		}
		// Meh
		else {
			// Need
			if (workerToTeam < .1f) {
				// 10 per 100...
				workerPriority = .1f / workerToTeam;
			}
			// Could use
			else if (workerToTeam < .20f) {
				// .1 - .2 = 1 - 0
				// x - .1 * 10 = 0 - 1
				workerPriority = 1.0f - ((workerToTeam - .1f) * 10.0f);
				if (workerPriority > 1.0f) {
					workerPriority = 1.0f;
				}
			}
			// Not Needed
			else {
				workerPriority = .0f;
			}
		}
		PlayerData::pd->unitPriority[bc_UnitType::Worker] = workerPriority;
	}

	// Rocket Priority
	{
		float rocketPriority = .0f;

		// If lacking in Initial karbonite and Early
		if (currRound < 200) {
			// NEED
			if (PlayerData::pd->earthStartingKarbonite < 1.0f) {
				rocketPriority = 10000.0f;
			}
			// Need or could use
			else if (PlayerData::pd->earthStartingKarbonite < 1000.0f) {
				int startKarb = PlayerData::pd->earthStartingKarbonite;
				if(startKarb < 1) { startKarb = 1; }
				rocketPriority = 1000.0f / startKarb;
			} 
			
		} else {
			// Linear to Round
			rocketPriority = (currRound / 450.0f);
		}

		PlayerData::pd->unitPriority[bc_UnitType::Rocket] = rocketPriority;
	}
	
	// Factory Priority
	{
		float factoryToTeam = static_cast<float>(factoryAmo) / (totalAmo + totalProductionAmo);

		float factoryPriority = .0f;
		
		// Always want to be producing factories. Compare to Karb reserves
		factoryPriority = (1.0f - (factoryToTeam * 10.0f)) * (GameController::Karbonite() / 100.0f);

		PlayerData::pd->unitPriority[bc_UnitType::Factory] = factoryPriority;
	}

}

//void BuilderOverlord::WorkerAction(units::Worker worker)
//{
//	if (PlayerData::pd->desiredUnitCounts[Worker] > PlayerData::pd->teamUnitCounts[Worker]) {
//		for (auto direction : constants::directions_adjacent) {
//			if (worker.CanReplicate(direction)) {
//				worker.Replicate(direction);
//			}
//		}
//	}
//	if (PlayerData::pd->desiredUnitCounts[Factory] > PlayerData::pd->teamUnitCounts[Factory]) {
//		for (auto direction : constants::directions_adjacent) {
//			if (worker.CanBlueprint(Factory, direction)) {
//				worker.Blueprint(Factory, direction);
//			}
//		}
//	}
//	if (PlayerData::pd->desiredUnitCounts[Rocket] > PlayerData::pd->teamUnitCounts[Rocket]) {
//		for (auto direction : constants::directions_adjacent) {
//			if (worker.CanBlueprint(Rocket, direction)) {
//				worker.Blueprint(Rocket, direction);
//			}
//		}
//	}
//	MapLocation workerLocation = worker.Loc().ToMapLocation();
//	bc_VecMapLocation* nearby = bc_GameController_all_locations_within(GameController::gc, workerLocation.self, 25);
//	auto nearbyDeposits = MapUtil::FilteredLocations(nearby, [](bc_MapLocation* potentialDeposit) {
//		return bc_GameController_karbonite_at(GameController::gc, potentialDeposit) > 0;
//	});
//	delete_bc_VecMapLocation(nearby);
//	if (nearbyDeposits.size() == 0) {
//		Pathfind::MoveRandom(worker);
//	}
//	for (int i = 1; i <= 100; i = (i + 1) * (i + 1)) {
//		auto nearbyFactories = VecUnit::Wrap<units::Factory>(bc_GameController_sense_nearby_units_by_type(GameController::gc, workerLocation.self, i, Factory));
//		if (nearbyFactories.size() > 0) {
//			if (worker.CanBuild(nearbyFactories[0])) {
//				worker.Build(nearbyFactories[0]);
//			}
//			MapLocation buildLocation = nearbyFactories[0].Loc().ToMapLocation();
//			Pathfind::MoveGreedy(worker, buildLocation);
//			break;
//		}
//	}
//	//std::sort(nearbyDeposits.begin(), nearbyDeposits.end(), [&workerLocation, this](bc_MapLocation* a, bc_MapLocation* b) {
//	//	return
//	//		abs(bc_MapLocation_x_get(workerLocation.self) - bc_MapLocation_x_get(a)) + abs(bc_MapLocation_y_get(workerLocation.self) - bc_MapLocation_y_get(a)) <
//	//		abs(bc_MapLocation_x_get(workerLocation.self) - bc_MapLocation_x_get(b)) + abs(bc_MapLocation_y_get(workerLocation.self) - bc_MapLocation_y_get(b));
//	//});
//	//bc_Direction probe = bc_MapLocation_direction_to(workerLocation.self, nearbyDeposits[0]);
//	//if (worker.CanHarvest(probe)) {
//	//	worker.Harvest(probe);
//	//}
//	//MapLocation toSeek{ bc_MapLocation_clone(nearbyDeposits[0]) };
//	MapLocation test = MapLocation(Earth, 10, 10);
//	Pathfind::MoveGreedy(worker, test);
//}
//
//void BuilderOverlord::FactoryAction(units::Factory factory)
//{
//	if (factory.IsBuilt()) {
//		for (auto direction : constants::directions_adjacent) {
//			if (factory.CanUnload(direction)) {
//				factory.Unload(direction);
//			}
//		}
//		if (PlayerData::pd->desiredUnitCounts[Factory] <= PlayerData::pd->teamUnitCounts[Factory] && // Don't spend Karbonite if structures need to be build
//			PlayerData::pd->desiredUnitCounts[Rocket] <= PlayerData::pd->teamUnitCounts[Rocket] &&
//			factory.CanProduce(Knight)) {
//			std::cout << "Actual Knights: " << PlayerData::pd->teamUnitCounts[Knight] << "\n";
//			std::cout << "Desired Knights: " << PlayerData::pd->desiredUnitCounts[Knight] << "\n";
//			if (PlayerData::pd->desiredUnitCounts[Knight] > PlayerData::pd->teamUnitCounts[Knight]) {
//				std::cout << "Producing Knight\n";
//				factory.Produce(Knight);
//				PlayerData::pd->inProductionCounts[Knight]++;
//			}
//			else if (PlayerData::pd->desiredUnitCounts[Ranger] > PlayerData::pd->teamUnitCounts[Ranger]) {
//				factory.Produce(Ranger);
//			}
//			else if (PlayerData::pd->desiredUnitCounts[Mage] > PlayerData::pd->teamUnitCounts[Mage]) {
//				factory.Produce(Mage);
//			}
//			else if (PlayerData::pd->desiredUnitCounts[Healer] > PlayerData::pd->teamUnitCounts[Healer]) {
//				factory.Produce(Healer);
//			}
//		}
//	}
//}
//
//void BuilderOverlord::RocketAction(units::Rocket rocket)
//{
//	if (rocket.IsBuilt()) {
//		if (GameController::Planet() == Earth) {
//			MapLocation random{ MapUtil::marsPassableLocations[rand() % MapUtil::marsPassableLocations.size()] };
//			if (rocket.CanLaunch(random)) {
//				if (GameController::Round() == 749 || rocket.Garrison().size() == rocket.MaxCapacity()) {
//					rocket.Launch(random);
//				}
//			}
//		}
//		else {
//			for (auto direction : constants::directions_adjacent) {
//				if (rocket.CanUnload(direction)) {
//					rocket.Unload(direction);
//					// TODO Push unloaded unit and try unload again
//				}
//			}
//		}
//	}
//}
