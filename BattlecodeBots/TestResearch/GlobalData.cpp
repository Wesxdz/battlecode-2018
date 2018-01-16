#include "Globaldata.h"

#include <iostream>

#include "Utility.h"

GlobalData* GlobalData::data = nullptr;

GlobalData::GlobalData() {
	data = this;
}
GlobalData::~GlobalData() {

}

void GlobalData::Init() {
	thisTeam = GameController::Team();
	thisPlanetMap = GameMap::OurPlanet();

	// Setting Up the Initial Enemy Spawn Points
	///*
	{
		//std::cout << "Getting the Initial Units" << std::endl;
		//std::cout << "The current PlanetMap Ptr is " << data->thisPlanetMap->self << std::endl;
		//auto initialUnitPtr = bc_PlanetMap_initial_units_get(data->thisPlanetMap->self);
		/*std::cout << "Got the initial Unit Ptrs." << std::endl;
		std::vector<units::Unit> initialUnits = GameController::Wrap<units::Unit>(initialUnitPtr);
		std::cout << "There are " << initialUnits.size() << " initial Units" << std::endl;
		data->enemySpawnAmo = initialUnits.size()/2;
		std::cout << "There are " << data->enemySpawnAmo << " enemies" << std::endl;

		data->enemySpawns = new MapLocation[data->enemySpawnAmo];
		std::cout << "We created an array of size " << data->enemySpawnAmo << std::endl;

		for (int i = 0; i < data->enemySpawnAmo; i++) {
		if (initialUnits[i].Team() == data->thisTeam) {
		std::cout << "We have a unit" << std::endl;
		continue;
		}
		data->enemySpawns[i] = initialUnits[i].Loc().ToMapLocation();
		std::cout << "Enemy at " << data->enemySpawns[i].X() << ", " << data->enemySpawns[i].Y() << std::endl;
		}*/
		CHECK_ERRORS();
	}
	//*/

	// Finding Optimal Launch Times
	{
		int lowest = 999;
		int curr = 0;
		int next = 0;
		for (int i = 0; i < 749; i++) {
			auto launchTime = bc_OrbitPattern_duration(OrbitPattern::self, i);
			if (launchTime < lowest) {
				lowest = launchTime;
				curr = i;
				next = 0;
			}
			if (launchTime == lowest && next == 0) {
				next = i;
				break;
			}
		}
		data->launchFrequency = next - curr;
		data->launchStart = curr;
		data->minLaunchTurns = lowest;
		// The period is actually the .157
		std::cout << "The launch time is " << lowest << " at " << curr << " and " << next << std::endl;
		CHECK_ERRORS();
	}

	// Getting Passable Terrain and Karb for this Map
	{
		int centerPoint = data->thisPlanetMap->height / 2;

		int allLocationsSize = centerPoint * centerPoint * 4;
		MapLocation centerPointEarth(data->thisPlanetMap->planetType, centerPoint, centerPoint);
		MapLocation centerPointMars(Utility::GetOtherPlanet(data->thisPlanetMap->planetType), centerPoint, centerPoint);

		int playAreaSqr = centerPoint * centerPoint + centerPoint * centerPoint;

		auto allLocationsEarth = bc_GameController_all_locations_within(GameController::gc, centerPointEarth.self, playAreaSqr);
		auto allLocationsMars = bc_GameController_all_locations_within(GameController::gc, centerPointMars.self, playAreaSqr);
		auto actualAmo = bc_VecMapLocation_len(allLocationsEarth);
		if(actualAmo != allLocationsSize){
			std::cout << "We expected " << allLocationsSize << " Locations but got " << actualAmo << std::endl;
			std::cout << "The center point on earth is at " << centerPoint << ", " << centerPoint << std::endl;
			std::cout << "Planet Height: " << data->thisPlanetMap->height << ", Width: " << data->thisPlanetMap->width << std::endl;
			std::cout << "We are searching all areas with " << playAreaSqr << "radius squared" << std::endl;
			CHECK_ERRORS();
		}

		auto planetEarthPtr = GameMap::earth.self;
		auto planetMarsPtr = GameMap::mars.self;
		std::cout << "The pointers are " << planetEarthPtr << " " << planetMarsPtr << std::endl;
		for (int i = 0; i < allLocationsSize; i++) {
			auto earthLocPtr = bc_VecMapLocation_index(allLocationsEarth, i);
			auto marsLocPtr = bc_VecMapLocation_index(allLocationsMars, i);
			std::cout << "The pointers are " << earthLocPtr << " " << marsLocPtr << std::endl;
			auto thisPlanetLocPtr = data->thisPlanetMap->planetType == bc_Planet::Earth ? earthLocPtr : marsLocPtr;

			auto canPass = bc_PlanetMap_is_passable_terrain_at(planetEarthPtr, earthLocPtr);
			MapLocation earthLoc(earthLocPtr);
			if (canPass) {
				data->passableEarthTerrain.push_back(earthLoc);
			}
			CHECK_ERRORS();
			std::cout << "Earth " << earthLoc.X() << ", " << earthLoc.Y() << ". Type " << earthLoc.Planet() << std::endl;

			canPass = bc_PlanetMap_is_passable_terrain_at(planetMarsPtr, marsLocPtr);
			MapLocation marsLoc(marsLocPtr);
			if (canPass) {
				data->passableMarsTerrain.push_back(marsLoc);
			}
			CHECK_ERRORS();
			std::cout << "Mars " << marsLoc.X() << ", " << marsLoc.Y() << ". Type " << marsLoc.Planet() << std::endl;

			auto initialKarb = bc_PlanetMap_initial_karbonite_at(data->thisPlanetMap->self, thisPlanetLocPtr);
			MapLocation karbLoc(bc_MapLocation_clone(thisPlanetLocPtr));
			if (initialKarb) {
				data->initialKarbLocations.push_back(karbLoc);
				data->totalInitialKarb += initialKarb;
			}
			CHECK_ERRORS();
			std::cout << "Karb " << karbLoc.X() << ", " << karbLoc.Y() << ". Type " << karbLoc.Planet() << std::endl;
		}
		delete_bc_VecMapLocation(allLocationsEarth);
		delete_bc_VecMapLocation(allLocationsMars);

		std::cout << "There are " << data->passableEarthTerrain.size() << " passable tiles on earth." << std::endl;
		std::cout << "There are " << data->initialKarbLocations.size() << " initial Karbonite locations" << std::endl;
		std::cout << "There is " << data->totalInitialKarb << " total karb available." << std::endl;
		CHECK_ERRORS();
	}

}
void GlobalData::Update() {
	currKarbonite = GameController::Karbonite();
	currRound =  GameController::Round();
	std::cout << "Round: " << currRound << ", " << currKarbonite << " Karbonite" << std::endl;

	auto allUnits = gc.Units(bc_Selection::Visible);
	auto allUnitAmo = allUnits.size();
	// All Enemy units
	currEnemys.clear();

	// Specifics
	currEnemyKnights.clear();
	currEnemyRangers.clear();
	currEnemyMages.clear();
	currEnemyHealers.clear();
	currEnemyOthers.clear();

	// All our units
	currUnits.clear();

	// Speecifics
	currWorkers.clear();
	currKnights.clear();
	currRangers.clear();
	currMages.clear();
	currHealers.clear();
	currFactorys.clear();
	currRockets.clear();

	unBuiltFactorys.clear();
	unBuiltRockets.clear();

	// Loops through all units and stores them.
	for (int i = 0; i < allUnitAmo; i++) {
		auto type = allUnits[i].type;
		auto team = allUnits[i].Team();
		if (team != thisTeam) {
			currEnemys.push_back(allUnits[i]);
			switch (type) {
				case Knight:
				{
					currEnemyKnights.push_back(bc_Unit_clone(allUnits[i].self));
					break;
				}
				case Ranger:
				{
					currEnemyRangers.push_back(bc_Unit_clone(allUnits[i].self));
					break;
				}
				case Mage:
				{
					currEnemyMages.push_back(bc_Unit_clone(allUnits[i].self));
					break;
				}
				case Healer:
				{
					currEnemyHealers.push_back(bc_Unit_clone(allUnits[i].self));
					break;
				}
				default:
				{
					currEnemyOthers.push_back(allUnits[i]);
				}
					break;
			}
		} 
		else {
			currUnits.push_back(allUnits[i]);
			switch (type) {
				case Worker:
				{
					currWorkers.push_back(bc_Unit_clone(allUnits[i].self));
					break;
				}
				case Knight:
				{
					currKnights.push_back(bc_Unit_clone(allUnits[i].self));
					break;
				}
				case Ranger:
				{
					currRangers.push_back(bc_Unit_clone(allUnits[i].self));
					break;
				}
				case Mage:
				{
					currMages.push_back(bc_Unit_clone(allUnits[i].self));
					break;
				}
				case Healer:
				{
					currHealers.push_back(bc_Unit_clone(allUnits[i].self));
					break;
				}
				case Factory:
				{
					if (bc_Unit_structure_is_built(allUnits[i].self)) {
						currFactorys.push_back(bc_Unit_clone(allUnits[i].self));
					} else {
						unBuiltFactorys.push_back(bc_Unit_clone(allUnits[i].self));
					}
					
					break;
				}
				case Rocket:
				{
					if (bc_Unit_structure_is_built(allUnits[i].self)) {
						currRockets.push_back(bc_Unit_clone(allUnits[i].self));
					} else {
						unBuiltRockets.push_back(bc_Unit_clone(allUnits[i].self));
					}
					break;
				}
			}
		}

	}
}
