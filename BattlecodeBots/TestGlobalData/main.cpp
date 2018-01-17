#include <iostream>
#include <memory>
#include <math.h>

#include "bc.h"

#include "GlobalData.h"

#include "Unit.h"
#include "Worker.h"
#include "MapLocation.h"
#include "Location.h"

#include "AStar.h"
#include "Utility.h"

GlobalData data;

//bc_Team thisTeam = data.gc.Team();
//bc_Planet thisPlanet = data.gc.Planet();
//PlanetMap* thisPlent = data.gameMap.OurPlanet();

void SetupInitalGlobalData() {
	std::cout << "Here" << std::endl;
	// Setting Up the Initial Enemy Spawn Points
	/*
	{
		std::cout << "Getting the Initial Units" << std::endl;
		std::cout << "The current PlanetMap Ptr is " << data.thisPlanetMap->self << std::endl;
		auto initialUnitPtr = bc_PlanetMap_initial_units_get(data.thisPlanetMap->self);
		std::cout << "Got the initial Unit Ptrs." << std::endl;
		std::vector<units::Unit> initialUnits = GameController::Wrap<units::Unit>(initialUnitPtr);
		std::cout << "There are " << initialUnits.size() << " initial Units" << std::endl;
		data.enemySpawnAmo = initialUnits.size()/2;
		std::cout << "There are " << data.enemySpawnAmo << " enemies" << std::endl;

		data.enemySpawns = new MapLocation[data.enemySpawnAmo];
		std::cout << "We created an array of size " << data.enemySpawnAmo << std::endl;
		
		for (int i = 0; i < data.enemySpawnAmo; i++) {
			if (initialUnits[i].Team() == data.thisTeam) {
				std::cout << "We have a unit" << std::endl;
				continue;
			}
			data.enemySpawns[i] = initialUnits[i].Loc().ToMapLocation();
			std::cout << "Enemy at " << data.enemySpawns[i].X() << ", " << data.enemySpawns[i].Y() << std::endl;
		}
		CHECK_ERRORS();
	}
	*/

	std::cout << "Here1" << std::endl;
	// Finding Optimal Launch Times
	///*
	{
		auto amp = OrbitPattern::Amplitude();
		auto center = OrbitPattern::Center();
		auto frequency = OrbitPattern::Period();
		auto period = 1.0f / frequency;
		data.launchFrequency = frequency * 2; // This is how often the launch time is at lowest
		data.launchStart = frequency * 4.7123889f; // 3 pi / 2
		data.minLaunchTurns = amp * sin(static_cast<long>(period * data.launchStart)) + center;
		
		std::cout << "The frequency is " << frequency << std::endl;
		std::cout << "The Sine function has A = " << amp << ", B = " << period << ", C = " << center << ". The minimum turn is " << data.minLaunchTurns << " at Round " << data.launchStart << " repeateding every " << data.launchFrequency << " rounds." << std::endl;
		CHECK_ERRORS();
	}
	//*/
	std::cout << "Here2" << std::endl;
	// Getting Passable Terrain for this Map
	/*
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
	*/
	std::cout << "Here3" << std::endl;
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
	//SetupInitalGlobalData();
	//CHECK_ERRORS();
	std::cout << "Here4" << std::endl;
	//AStar aStar = AStar();
	//bool pathed = false;

	//if (GameController::Planet() == Mars)
	//{
	//	pathed = true;
	//}

	//int pathNum = 0;
	while (true)
	{
		uint32_t round = GameController::Round();
		std::cout << "Round: " << round << std::endl;
		//auto units = GameController::Units(bc_Selection::MyTeam);
		//for (auto unit : units)
		//{
		//	if (!pathed)
		//	{
		//		int x = unit.Loc().ToMapLocation().X();
		//		int y = unit.Loc().ToMapLocation().Y();
		//		aStar.PathFind(x, y, GameMap::Earth().Width() - x, GameMap::Earth().Height() - y, true);
		//		pathed = true;
		//	}
		//	if (unit.type == bc_UnitType::Worker) {
		//		units::Worker worker{ bc_Unit_clone(unit.self) };
		//		if (worker.IsMoveReady() && worker.CanMove(aStar.m_pathDirections[pathNum]))
		//		{
		//			worker.Move(aStar.m_pathDirections[pathNum]);
		//			if (pathNum < aStar.m_pathDirections.size())
		//				pathNum++;
		//		}

		//	}
		//}
		GameController::EndTurn();
	}
}