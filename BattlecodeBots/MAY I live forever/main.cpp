#include <iostream>
#include <memory>
#include <math.h>

#include "bc.h"

#include "GlobalData.h"

#include "Unit.h"
#include "Worker.h"
#include "MapLocation.h"
#include "Location.h"

#include "Utility.h"

GlobalData data;

//bc_Team thisTeam = data.gc.Team();
//bc_Planet thisPlanet = data.gc.Planet();
//PlanetMap* thisPlent = data.gameMap.OurPlanet();

void SetupInitalGlobalData() {
	// Setting Up the Initial Enemy Spawn Points
	{
		std::vector<units::Unit> initialUnits = 
			GameController::Wrap<units::Unit>(bc_PlanetMap_initial_units_get(data.thisPlanetMap->self));
		data.enemySpawnAmo = initialUnits.size()/2;
		data.enemySpawns = new MapLocation[data.enemySpawnAmo];

		for (int i = 0; i < data.enemySpawnAmo; i++) {
			if (initialUnits[i].Team() == data.thisTeam) { continue;}
			data.enemySpawns[i] = initialUnits[i].Loc().ToMapLocation();
			std::cout << "Enemy at " << data.enemySpawns[i].X() << ", " << data.enemySpawns[i].Y() << std::endl;
		}
		CHECK_ERRORS();
	}

	// Finding Optimal Launch Times
	{
		auto amp = OrbitPattern::Amplitude();
		auto center = OrbitPattern::Center();
		auto period = OrbitPattern::Period();
		auto frequency = 1.0f / period;
		data.launchFrequency = frequency * 2; // This is how often the launch time is at lowest
		data.launchStart = frequency * 4.7123889; // 3 pi / 2
		data.minLaunchTurns = amp * sin(period * data.launchStart) + center;
		

		std::cout << "The Sine function has A = " << amp << ", B = " << period << ", C = " << center << ". The minimum turn is " << data.minLaunchTurns << " at Round " << data.launchStart << " repeateding every " << data.launchFrequency << " rounds." << std::endl;
		CHECK_ERRORS();
	}

	// Getting Passable Terrain for this Map
	{
		int centerPoint = data.thisPlanetMap->height / 2;

		int allLocationsSize = centerPoint * centerPoint * 4;
		MapLocation centerPointEarth(data.thisPlanetMap->planetType, centerPoint, centerPoint);
		MapLocation centerPointMars(Utility::GetOtherPlanet(data.thisPlanetMap->planetType), centerPoint, centerPoint);

		int playAreaSqr = centerPoint * centerPoint + centerPoint * centerPoint;

		auto allLocationsEarth = bc_GameController_all_locations_within(GameController::gc, centerPointEarth.self, playAreaSqr);
		auto allLocationsMars = bc_GameController_all_locations_within(GameController::gc, centerPointMars.self, playAreaSqr);
		auto actualAmo = bc_VecMapLocation_len(allLocationsEarth);
		if(actualAmo != allLocationsSize){
			std::cout << "We expected " << allLocationsSize << " Locations but got " << actualAmo << std::endl;
			std::cout << "The center point on earth is at " << centerPoint << ", " << centerPoint << std::endl;
			std::cout << "Planet Height: " << data.thisPlanetMap->height << ", Width: " << data.thisPlanetMap->width << std::endl;
			std::cout << "We are searching all areas with " << playAreaSqr << "radius squared" << std::endl;
			CHECK_ERRORS();
		}
		
		auto planetEarthPtr = GameMap::earth.self;
		auto planetMarsPtr = GameMap::mars.self;
		for (int i = 0; i < allLocationsSize; i++) {
			auto earthLocPtr = bc_VecMapLocation_index(allLocationsEarth, i);
			auto marsLocPtr = bc_VecMapLocation_index(allLocationsMars, i);
			auto thisPlanetLocPtr = data.thisPlanetMap->planetType == bc_Planet::Earth ? earthLocPtr : marsLocPtr;
			
			auto canPass = bc_PlanetMap_is_passable_terrain_at(planetEarthPtr, earthLocPtr);
			if (canPass) {
				data.passableEarthTerrain.push_back(MapLocation(earthLocPtr));
			}

			canPass = bc_PlanetMap_is_passable_terrain_at(planetMarsPtr, marsLocPtr);
			if (canPass) {
				data.passableMarsTerrain.push_back(MapLocation(marsLocPtr));
			}

			auto initialKarb = bc_PlanetMap_initial_karbonite_at(data.thisPlanetMap->self, thisPlanetLocPtr);
			if (initialKarb) {
				data.initialKarbLocations.push_back(MapLocation(bc_MapLocation_clone(thisPlanetLocPtr)));
				data.totalInitialKarb += initialKarb;
			}
		}
		delete_bc_VecMapLocation(allLocationsEarth);
		delete_bc_VecMapLocation(allLocationsMars);

		std::cout << "There are " << data.passableEarthTerrain.size() << " passable tiles on earth." << std::endl;
		std::cout << "There are " << data.initialKarbLocations.size() << " initial Karbonite locations" << std::endl;
		std::cout << "There is " << data.totalInitialKarb << " total karb available." << std::endl;
		CHECK_ERRORS();
	}
#include "AStar.h"


}

void SetupTurnGlobalData() {
	// Grab all units
}

/*
Instead of micromanaging individual unit behavior, prioritize goals based on available data and
then determine best execution strategy
*/
int main()
{
	srand(0);

	std::cout << "A* test initialize" << std::endl;
	AStar aStar = AStar();

	SetupInitalGlobalData();
	CHECK_ERRORS();

	while (true)
	{
		uint32_t round = GameController::Round();
		std::cout << "Round: " << round << std::endl;
		//auto units = player.Units(bc_Selection::MyTeam);
		//for (auto unit : units) {
		//	if (unit.type == bc_UnitType::Worker) {
		//		auto worker = std::make_shared<units::Worker>();
		//		worker.Init(unit.id);
		//		Location workerLocation = worker->Loc();
		//		MapLocation onMap = workerLocation.ToMapLocation();
		//		//bc_Direction directionToMove = Southwest;
		//		bc_Direction directionToMove = Pathfinding::PickGreedy(onMap, test);
		//		if (directionToMove != bc_Direction::Center && worker->IsMoveReady()) {
		//			worker->Move(directionToMove);
		//		}
		//	}
		//}
		GameController::EndTurn();
	}
}