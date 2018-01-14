#include <iostream>
#include <random>
#include <chrono>

#include "bc.h"

#include "GameController.h"
#include "GameMap.h"
#include "GameInfo.h"
#include "Research.h"
#include "Utility.h"
#include "Location.h"
#include "MapLocation.h"


GameController gc;
GameMap gameMap;
GameInfo gameInfo;
Research research;

//PlanetMap planetMap;
//
//
//bc_Team thisTeam;
//bc_Planet thisPlanet;
//
//int currRound;
//int currKarbonite;

//std::vector<std::shared_ptr<units::Unit>> currUnits;
//int currWorkerAmo;
//int currKnightAmo;
//int currFactoryAmo;

MapLocation wayPoint;

enum WorkerPriority {
	KARB,
	FACTORY,
	ROCKET,
	REPLICATE,
};
WorkerPriority currWorkerPriority = WorkerPriority::KARB;
int workerPriorityAmo = 3;

enum FactoryPriority {
	IDLE,
	BUILD
};
FactoryPriority currFactoryPriority = FactoryPriority::BUILD;
bc_UnitType currFactoryBuild = bc_UnitType::Knight;

void SetWayPoint();

void UpdateWorkerEarth(units::Worker* worker) {
	// Initialize
	bool hasAction = true;
	
	auto location = worker->Loc();
	if (location.IsInSpace() || location.IsInGarrison()) {
		return;
	}

	MapLocation mapLocation = location.ToMapLocation();
	//int x = mapLocation.X();
	//int y = mapLocation.Y();


	// It will probably be smart to get all surrounding units and move, build based on that.
	// We couls also have a way to sort priorities... like building, loading, repairing, replicating, etc.

	//Factory
	{
		// Get all nearby factories within adjacent spaces and build.
		auto nearByFactories = worker->GetUnitsWithinRangeByType<units::Factory>(2, bc_UnitType::Factory);
		int factoryAmo = nearByFactories.size();
		for (int i = 0; i < factoryAmo; i++) {
			if (!nearByFactories[i]->IsBuilt() && nearByFactories[i]->Team() == gameInfo.thisTeam) {
				worker->Build(nearByFactories[i]->id);
				std::cout << "Worker " << worker->id << " helped build Factory " << nearByFactories[i]->id << std::endl;
				hasAction = false;
				break;
			}
		}

		// Search for closest factory thst isnt built
		// Keep track of structures that aren't built
		// Focus more on buidling. Don't replicate whenever we can

		for (int i = 0; i < 8; i++) {
			bc_Direction currDir = static_cast<bc_Direction>(i);
			if (worker->CanBlueprint(bc_UnitType::Factory, currDir)) {
				worker->Blueprint(bc_UnitType::Factory, currDir);
				std::cout << "Worker " << worker->id << " has blueprinted Factory in Direction " << currDir << std::endl;
				hasAction = false;
				break;
			}
		}
		//break;
	}
	//case ROCKET:
	{
		// Get all nearby Rockets within adjacent spaces and build.
		auto nearByRockets = worker->GetUnitsWithinRangeByType<units::Rocket>(2, bc_UnitType::Rocket);
		int rocketAmo = nearByRockets.size();
		for (int i = 0; i < rocketAmo; i++) {
			if (!nearByRockets[i]->IsBuilt() && nearByRockets[i]->Team() == gameInfo.thisTeam) {
				worker->Build(nearByRockets[i]->id);
				std::cout << "Worker " << worker->id << " helped build Rocket " << nearByRockets[i]->id << std::endl;
				hasAction = false;
				break;
			}
		}

		for (int i = 0; i < 8; i++) {
			bc_Direction currDir = static_cast<bc_Direction>(i);
			if (worker->CanBlueprint(bc_UnitType::Rocket, currDir)) {
				worker->Blueprint(bc_UnitType::Rocket, currDir);
				std::cout << "Worker " << worker->id << " has blueprinted Rocket in Direction " << currDir << std::endl;
				hasAction = false;
				break;
			}
		}
		//break;
	}
	//case REPLICATE:
	{
		// Only replicate if the map Ratio and Factory ratio are small
		int workerAmo = gameInfo.currWorkers.size();
		int factoryAmo = gameInfo.currFactorys.size();
		int MapToWorkerRatio = static_cast<int>(gameInfo.planetMap.width * gameInfo.planetMap.height * .125f);
		int FactoryToWorkerRatio = factoryAmo == 0 ? 100 : workerAmo / factoryAmo;
		if (workerAmo < MapToWorkerRatio && FactoryToWorkerRatio < 10) {

			for (int i = 0, e = rand() % bc_Direction::Center; i < 8; i++, e++) {
				if (e > bc_Direction::Northwest) {
					e = bc_Direction::North;
				}
				bc_Direction currDir = static_cast<bc_Direction>(e);
				if (worker->CanReplicate(currDir)) {
					worker->Replicate(currDir);
					std::cout << "Worker " << worker->id << " has replicated in Direction " << currDir << std::endl;
				}
			}
		}
		//break;
	}
	
	// We should hold all the initial Karbonite spots and check them
	// If we can't harvest from that location then remove and move on to the next.
	// When all are removed, we  should build Rockets and head to Mars then Harvest there

	// Karb
	{
		// Harvest curr Loc
		if (worker->CanHarvest(bc_Direction::Center)) {
			bc_GameController_harvest(gc.gc, worker->id, bc_Direction::Center);
			std::cout << "Worker " << worker->id << " is harvesting" << std::endl;
			hasAction = false;
			return;
		}

		// Harvest Surrounding Spaces
		for (int i = 0; i < 8; i++) {
			bc_Direction currDir = static_cast<bc_Direction>(i);
			if (worker->CanHarvest(currDir)) {
				bc_GameController_harvest(gc.gc, worker->id, currDir);
				std::cout << "Worker " << worker->id << " is harvesting" << std::endl;
				hasAction = false;
				return;
			}
		}
	}

	// If we could not Build or Harvest, we should move
	if (hasAction) {
		// Movement
		{
		// Check Heat
			if (worker->MovementHeat() > 9) {
				return;
			}

			// Find a different direction
			for (int i = 0, e = rand() % bc_Direction::Center; i < 8; i++, e++) {
				if (e > bc_Direction::Northwest) {
					e = bc_Direction::North;
				}
				auto dir = static_cast<bc_Direction>(e);
				if (worker->CanMove(dir)) {
					worker->Move(dir);
					std::cout << "Worker " << worker->id << " has moved in Direction " << e << std::endl;
					hasAction = false;
					return;
				}
			}
		}
	}	
}
void UpdateKnightEarth(units::Knight* knight) {
	// Initialize
	bool hasAction = true;

	// Check Location
	auto location = knight->Loc();
	if (location.IsInSpace() || location.IsInGarrison()) {
		return;
	}
	MapLocation mapLocation = location.ToMapLocation();

	// Attack enemy team
	auto nearByEnemies = knight->GetUnitsWithinRangeByTeam(knight->AttackRange(), Utility::GetOtherTeam(gameInfo.thisTeam));
	int enemyAmo = nearByEnemies.size();

	// If nearby Enemies, either attack, stay, or move away if Knights
	if (enemyAmo > 0) {
		if (knight->AttackHeat() < 10) {
			for (int i = 0; i < enemyAmo; i++) {
				if (Utility::IsRobot(nearByEnemies[i]->type)) {
					knight->Attack(nearByEnemies[i]->id);
					return;
				}
			}
		} 
		else {
			// Run away???
		}
	} 
	// Else Move to Waypoint
	else {
		// Check Heat
		if(knight->MovementHeat() > 9){ return; }

		if (mapLocation == wayPoint) {
			std::cout << "Changing the Waypoint" << std::endl;
			SetWayPoint();
		}

		// Move in Closest Direction
		auto dir = mapLocation.DirectionTo(wayPoint);
		if (knight->CanMove(dir)) {
			knight->Move(dir);
			std::cout << "Knight " << knight->id << " has moved in Direction " << dir << std::endl;
			hasAction = false;
			return;
		}

		// Move in different Direction
		for (int i = 1, y = dir + 1; i < 7; i++, y++) {
			if (y > bc_Direction::Northwest) {
				y = bc_Direction::North;
			}
			dir = static_cast<bc_Direction>(y);
			if (knight->CanMove(dir)) {
				knight->Move(dir);
				std::cout << "Knight " << knight->id << " has moved in Direction " << dir << std::endl;
				hasAction = false;
				break;
			}
		}
	}
	
	if(hasAction){
		//std::cout << "Knight did not do anything this turn" << std::endl;
	}
	
}
void UpdateRangerEarth(units::Ranger* ranger) {

}
void UpdateMageEarth(units::Mage* mage) {

}
void UpdateHealerEarth(units::Healer* healer) {
	 
}
void UpdateFactoryEarth(units::Factory* factory){
	if (factory->IsBuilt()) {
		if (factory->CanProduce(currFactoryBuild)) {
			factory->Produce(currFactoryBuild);
			std::cout << "Factory " << factory->id << " is producing UnitType " << currFactoryBuild << std::endl;
		}
		auto unitsInGarrison = factory->Garrison();
		auto unitsInGarrisonAmo = unitsInGarrison.size();
		MapLocation mapLocation = factory->Loc().ToMapLocation();
		for (int i = 0; i < 8 && unitsInGarrisonAmo > 0; i++) {
			bc_Direction currDir = static_cast<bc_Direction>(i);
			MapLocation loc = MapLocation::Neighbor(mapLocation, currDir);
			if (loc.Occupiable()) {
				factory->Unload(currDir);
				std::cout << "Factory " << factory->id << " is Unloading a unit in Direction " << currDir << std::endl;
				unitsInGarrisonAmo--;
			}
		}
	
	}
}
void UpdateRocketEarth(units::Rocket* rocket) {

}

void SetWayPoint() {
	// Look for specific areas we saw enemies at
	// WE should average the known enemies out?
	if (gameInfo.currEnemys.size() > 0) {
		wayPoint = gameInfo.currEnemys[0]->Loc().ToMapLocation();
		return;
	}

	// Identify areas that the opponent spawned at
	if (gameInfo.enemySpawnAmo > 0) {
		wayPoint = gameInfo.enemySpawns[--gameInfo.enemySpawnAmo];
		return;
	} else {
		delete[] gameInfo.enemySpawns;
		gameInfo.enemySpawns = nullptr; // Delete after we have found all of them
	}
	wayPoint = MapLocation(gameInfo.planetMap.planetType, 10, 10);// SomePoint Outside of Vision Range...
}

void UpdateEarth() {
	gameInfo.thisTeam = gc.Team();
	gameInfo.planetMap = gameMap.Earth();

	// Enemy Spawns
	{
		//std::vector<std::shared_ptr<units::Unit>> initialUnits =
		//	GameController::Wrap<units::Unit>(bc_PlanetMap_initial_units_get(gameInfo.planetMap.self));
		//gameInfo.enemySpawnAmo = initialUnits.size()/2;
		//gameInfo.enemySpawns = new MapLocation[gameInfo.enemySpawnAmo];

		//for (int i = 0; i < gameInfo.enemySpawnAmo; i++) {
		//	if (initialUnits[i]->Team() == gameInfo.thisTeam) { continue;}
		//	gameInfo.enemySpawns[i] = initialUnits[i]->Loc().ToMapLocation();
		//}
		SetWayPoint();
	}
	
	std::cout << "This bot is on Earth" << std::endl;

	// Research...
	{
	// Queue all Research for Worker
		research.Queue(bc_UnitType::Worker);
		research.Queue(bc_UnitType::Worker);
		research.Queue(bc_UnitType::Worker);
		research.Queue(bc_UnitType::Worker);
		std::cout << "Queuing research for Worker 4 times" << std::endl;
		Utility::CheckError();
	}

	std::chrono::duration<double> totalTime;
	while (true) {
		auto start = std::chrono::system_clock::now();
		Utility::CheckError();

		gameInfo.currKarbonite = gc.Karbonite();
		gameInfo.currRound = gc.Round();
		std::cout << "Round: " << gameInfo.currRound << ", " << gameInfo.currKarbonite << " Karbonite" << std::endl;
		
		auto allUnits = gc.Units(bc_Selection::Visible);
		auto allUnitAmo = allUnits.size();
		gameInfo.currEnemys.clear();
		gameInfo.currWorkers.clear();
		gameInfo.currKnights.clear();
		gameInfo.currRangers.clear();
		gameInfo.currMages.clear();
		gameInfo.currHealers.clear();
		gameInfo.currFactorys.clear();
		gameInfo.currRockets.clear();

		// Loops through all units and stores them.
		for (int i = 0; i < allUnitAmo; i++) {
			if (allUnits[i]->Team() != gameInfo.thisTeam) {
				gameInfo.currEnemys.push_back(allUnits[i]);
			} 
			else {
				switch (allUnits[i]->type) {
					case Worker:
					{
						std::shared_ptr<units::Worker> w = std::make_shared<units::Worker>();
						w->Init(allUnits[i]->self);
						gameInfo.currWorkers.push_back(w);
						break;
					}
					case Knight:
					{
						std::shared_ptr<units::Knight> k = std::make_shared<units::Knight>();
						k->Init(allUnits[i]->self);
						gameInfo.currKnights.push_back(k);
						break;
					}
					case Ranger:
					{
						std::shared_ptr<units::Ranger> r = std::make_shared<units::Ranger>();
						r->Init(allUnits[i]->self);
						gameInfo.currRangers.push_back(r);
						break;
					}
					case Mage:
					{
						std::shared_ptr<units::Mage> m = std::make_shared<units::Mage>();
						m->Init(allUnits[i]->self);
						gameInfo.currMages.push_back(m);
						break;
					}
					case Healer:
					{
						std::shared_ptr<units::Healer> h = std::make_shared<units::Healer>();
						h->Init(allUnits[i]->self);
						gameInfo.currHealers.push_back(h);
						break;
					}
					case Factory:
					{
						std::shared_ptr<units::Factory> f = std::make_shared<units::Factory>();
						f->Init(allUnits[i]->self);
						gameInfo.currFactorys.push_back(f);
						break;
					}
					case Rocket:
					{
						std::shared_ptr<units::Rocket> r = std::make_shared<units::Rocket>();
						r->Init(allUnits[i]->self);
						gameInfo.currRockets.push_back(r);
						break;
					}
				}
			}
			
		}
		Utility::CheckError();

		// Update Worker
		{
			int workerAmo = gameInfo.currWorkers.size();
			for (int i = 0; i < workerAmo; i++) {
				UpdateWorkerEarth(gameInfo.currWorkers[i].get());
			}
			Utility::CheckError();
		}

		// Update Knight
		{
			int knightAmo = gameInfo.currKnights.size();
			for (int i = 0; i < knightAmo; i++) {
				UpdateKnightEarth(gameInfo.currKnights[i].get());
			}
			Utility::CheckError();
		}

		// Update Ranger
		{
			int rangerAmo = gameInfo.currRangers.size();
			for (int i = 0; i < rangerAmo; i++) {
				UpdateRangerEarth(gameInfo.currRangers[i].get());
			}
			Utility::CheckError();
		}

		// Update Mages
		{
			int mageAmo = gameInfo.currMages.size();
			for (int i = 0; i < mageAmo; i++) {
				UpdateMageEarth(gameInfo.currMages[i].get());
			}
			Utility::CheckError();
		}
		
		// Update Healer
		{
			int healerAmo = gameInfo.currHealers.size();
			for (int i = 0; i < healerAmo; i++) {
				UpdateHealerEarth(gameInfo.currHealers[i].get());
			}
			Utility::CheckError();
		}

		// Update Factory
		{
			if(currFactoryPriority != FactoryPriority::IDLE) { 
				int factoryAmo = gameInfo.currFactorys.size();
				for (int i = 0; i < factoryAmo; i++) {
					UpdateFactoryEarth(gameInfo.currFactorys[i].get());
				}
			}
			Utility::CheckError();
		}

		// Update Rockets
		{
			int rocketAmo = gameInfo.currRockets.size();
			for (int i = 0; i < rocketAmo; i++) {
				UpdateRocketEarth(gameInfo.currRockets[i].get());
			}
			Utility::CheckError();
		}
		

		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> roundTime = end - start;
		totalTime += roundTime;
		std::cout << "This round took " << roundTime.count() << " seconds and " << totalTime.count() << " overall" << std::endl;
		gc.EndTurn();
	}
}

void UpdateMars() {
	std::cout << "This bot is on Mars" << std::endl;
	while (true) {
		Utility::CheckError();

		//currUnits = gc.Units(bc_Selection::MyTeam);
		//if (currUnits.size() > 0) {

		//} 
		gc.EndTurn();
	}
}

int main()
{
	Utility::CheckError();
	try {
		std::cout << "Player C++ bot starting!" << std::endl;
		srand(0);

		if (gameInfo.planetMap.planetType == bc_Planet::Earth) {
			UpdateEarth();
		} else {
			UpdateMars();
		}

		Utility::CheckError();
	} catch (...) {
		Utility::CheckError();
		std::cout << "We have an error. What do we do?" << std::endl;
	}

	delete[] gameInfo.enemySpawns;
	gameInfo.enemySpawns = nullptr;
	gameInfo.enemySpawnAmo = 0;

	return 0;



	//while (true)
	//{
	//	uint32_t round = bc_GameController_round(gc);
	//	std::cout << "Round: "<< round << std::endl;

	//	bc_VecUnit *units = bc_GameController_my_units(gc);

	//	int len = bc_VecUnit_len(units);
	//	for (int i = 0; i < len; i++)
	//	{
	//		bc_Unit *unit = bc_VecUnit_index(units, i);

	//		uint16_t id = bc_Unit_id(unit);
	//		if (bc_GameController_can_move(gc, id, North) && bc_GameController_is_move_ready(gc, id)) 
	//		{
	//			bc_GameController_move_robot(gc, id, North);
	//			Utility::CheckError();
	//		}
	//		delete_bc_Unit(unit);

	//	}
	//	delete_bc_VecUnit(units);

	//	bc_GameController_next_turn(gc);
	//}
}