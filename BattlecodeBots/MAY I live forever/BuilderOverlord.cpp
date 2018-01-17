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
	PlayerData::pd->desiredUnitCounts[Worker] = 8;
}

void BuilderOverlord::Update()
{
	auto units = GameController::Units(MyTeam);
	if (GameController::Round() > 50) {
		PlayerData::pd->desiredUnitCounts[Factory] = 6;
	}
	for (auto& unit : units) {
		if (unit.Loc().IsOnMap()) {
			if (unit.type == Worker) {
				WorkerAction(units::Worker(bc_Unit_clone(unit.self)));
			}
			else if (unit.type == Factory) {
				FactoryAction(units::Factory(bc_Unit_clone(unit.self)));
			}
			else if (unit.type == Rocket) {
				RocketAction(units::Rocket(bc_Unit_clone(unit.self)));
			}
		}
	}
}

void BuilderOverlord::WorkerAction(units::Worker worker)
{
	if (PlayerData::pd->desiredUnitCounts[Worker] > PlayerData::pd->teamUnitCounts[Worker]) {
		for (auto direction : constants::directions_adjacent) {
			if (worker.CanReplicate(direction)) {
				worker.Replicate(direction);
			}
		}
	}
	if (PlayerData::pd->desiredUnitCounts[Factory] > PlayerData::pd->teamUnitCounts[Factory]) {
		for (auto direction : constants::directions_adjacent) {
			if (worker.CanBlueprint(Factory, direction)) {
				worker.Blueprint(Factory, direction);
			}
		}
	}
	if (PlayerData::pd->desiredUnitCounts[Rocket] > PlayerData::pd->teamUnitCounts[Rocket]) {
		for (auto direction : constants::directions_adjacent) {
			if (worker.CanBlueprint(Rocket, direction)) {
				worker.Blueprint(Rocket, direction);
			}
		}
	}
	MapLocation workerLocation = worker.Loc().ToMapLocation();
	bc_VecMapLocation* nearby = bc_GameController_all_locations_within(GameController::gc, workerLocation.self, 25);
	auto nearbyDeposits = MapUtil::FilteredLocations(nearby, [](bc_MapLocation* potentialDeposit) {
		return bc_GameController_karbonite_at(GameController::gc, potentialDeposit) > 0;
	});
	delete_bc_VecMapLocation(nearby);
	if (nearbyDeposits.size() == 0) {
		Pathfind::MoveRandom(worker);
	}
	for (int i = 1; i <= 100; i = (i + 1) * (i + 1)) {
		auto nearbyFactories = VecUnit::Wrap<units::Factory>(bc_GameController_sense_nearby_units_by_type(GameController::gc, workerLocation.self, i, Factory));
		if (nearbyFactories.size() > 0) {
			if (worker.CanBuild(nearbyFactories[0])) {
				worker.Build(nearbyFactories[0]);
			}
			MapLocation buildLocation = nearbyFactories[0].Loc().ToMapLocation();
			Pathfind::MoveGreedy(worker, buildLocation);
			break;
		}
	}
	//std::sort(nearbyDeposits.begin(), nearbyDeposits.end(), [&workerLocation, this](bc_MapLocation* a, bc_MapLocation* b) {
	//	return
	//		abs(bc_MapLocation_x_get(workerLocation.self) - bc_MapLocation_x_get(a)) + abs(bc_MapLocation_y_get(workerLocation.self) - bc_MapLocation_y_get(a)) <
	//		abs(bc_MapLocation_x_get(workerLocation.self) - bc_MapLocation_x_get(b)) + abs(bc_MapLocation_y_get(workerLocation.self) - bc_MapLocation_y_get(b));
	//});
	//bc_Direction probe = bc_MapLocation_direction_to(workerLocation.self, nearbyDeposits[0]);
	//if (worker.CanHarvest(probe)) {
	//	worker.Harvest(probe);
	//}
	//MapLocation toSeek{ bc_MapLocation_clone(nearbyDeposits[0]) };
	MapLocation test = MapLocation(Earth, 10, 10);
	Pathfind::MoveGreedy(worker, test);
}

void BuilderOverlord::FactoryAction(units::Factory factory)
{
	if (factory.IsBuilt()) {
		for (auto direction : constants::directions_adjacent) {
			if (factory.CanUnload(direction)) {
				factory.Unload(direction);
			}
		}
		if (PlayerData::pd->desiredUnitCounts[Factory] <= PlayerData::pd->teamUnitCounts[Factory] && // Don't spend Karbonite if structures need to be build
			PlayerData::pd->desiredUnitCounts[Rocket] <= PlayerData::pd->teamUnitCounts[Rocket] &&
			factory.CanProduce(Knight)) {
			std::cout << "Actual Knights: " << PlayerData::pd->teamUnitCounts[Knight] << "\n";
			std::cout << "Desired Knights: " << PlayerData::pd->desiredUnitCounts[Knight] << "\n";
			if (PlayerData::pd->desiredUnitCounts[Knight] > PlayerData::pd->teamUnitCounts[Knight]) {
				std::cout << "Producing Knight\n";
				factory.Produce(Knight);
				PlayerData::pd->inProductionCounts[Knight]++;
			}
			else if (PlayerData::pd->desiredUnitCounts[Ranger] > PlayerData::pd->teamUnitCounts[Ranger]) {
				factory.Produce(Ranger);
			}
			else if (PlayerData::pd->desiredUnitCounts[Mage] > PlayerData::pd->teamUnitCounts[Mage]) {
				factory.Produce(Mage);
			}
			else if (PlayerData::pd->desiredUnitCounts[Healer] > PlayerData::pd->teamUnitCounts[Healer]) {
				factory.Produce(Healer);
			}
		}
	}
}

void BuilderOverlord::RocketAction(units::Rocket rocket)
{
	if (rocket.IsBuilt()) {
		if (GameController::Planet() == Earth) {
			MapLocation random{ MapUtil::marsPassableLocations[rand() % MapUtil::marsPassableLocations.size()] };
			if (rocket.CanLaunch(random)) {
				if (GameController::Round() == 749 || rocket.Garrison().size() == rocket.MaxCapacity()) {
					rocket.Launch(random);
				}
			}
		}
		else {
			for (auto direction : constants::directions_adjacent) {
				if (rocket.CanUnload(direction)) {
					rocket.Unload(direction);
					// TODO Push unloaded unit and try unload again
				}
			}
		}
	}
}
