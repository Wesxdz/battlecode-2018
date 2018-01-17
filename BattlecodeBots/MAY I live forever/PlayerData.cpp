#include "PlayerData.h"
#include "GameController.h"
#include "OrbitPattern.h"

#include <iostream>
#include "Utility.h"
#include "Log.h"

PlayerData::PlayerData()
{

	int period = OrbitPattern::Period();
	int optimalRounds = 749 / period;
	optimalLaunchTime = OrbitPattern::Center() - OrbitPattern::Amplitude();
	for (int i = 0; i < optimalRounds; i++)
	{
		optimalLaunchRounds.push_back((period - period / 4) * i);
		std::cout << OrbitPattern::Duration((period - period / 4) * i) << std::endl;
	}


	/////////////////////////////////////////////////

	//int centerPoint = Map(GameController::Planet()).;

	//int allLocationsSize = centerPoint * centerPoint * 4;
	//MapLocation centerPointEarth(GameMap::OurPlanet()->planetType, centerPoint, centerPoint);
	//MapLocation centerPointMars(Utility::GetOtherPlanet(GameMap::OurPlanet()->planetType), centerPoint, centerPoint);

	//int playAreaSqr = centerPoint * centerPoint + centerPoint * centerPoint;

	//bc_VecMapLocation* allLocationsEarth = bc_GameController_all_locations_within(GameController::gc, centerPointEarth.self, playAreaSqr);
	//bc_VecMapLocation* allLocationsMars = bc_GameController_all_locations_within(GameController::gc, centerPointMars.self, playAreaSqr);
	//uintptr_t actualAmo = bc_VecMapLocation_len(allLocationsEarth);
	//if (actualAmo != allLocationsSize) {
	//	std::cout << "We expected " << allLocationsSize << " Locations but got " << actualAmo << std::endl;
	//	std::cout << "The center point on earth is at " << centerPoint << ", " << centerPoint << std::endl;
	//	std::cout << "Planet Height: " << GameMap::OurPlanet()->height << ", Width: " << GameMap::OurPlanet()->width << std::endl;
	//	std::cout << "We are searching all areas with " << playAreaSqr << "radius squared" << std::endl;
	//	CHECK_ERRORS();
	//}

	//bc_PlanetMap* planetEarthPtr = earth;
	//bc_PlanetMap* planetMarsPtr = mars;
	//std::cout << "The pointers are " << planetEarthPtr << " " << planetMarsPtr << std::endl;
	//for (int i = 0; i < allLocationsSize; i++) 
	//{
	//	bc_MapLocation* earthLocPtr = bc_VecMapLocation_index(allLocationsEarth, i);
	//	bc_MapLocation* marsLocPtr = bc_VecMapLocation_index(allLocationsMars, i);
	//	std::cout << "The pointers are " << earthLocPtr << " " << marsLocPtr << std::endl;
	//	bc_MapLocation* thisPlanetLocPtr = GameMap::OurPlanet()->planetType == bc_Planet::Earth ? earthLocPtr : marsLocPtr;

	//	bool canPass = bc_PlanetMap_is_passable_terrain_at(planetEarthPtr, earthLocPtr);
	//	MapLocation earthLoc(earthLocPtr);
	//	if (canPass) 
	//	{
	//		passableEarthTerrain.push_back(earthLoc);
	//	}
	//	CHECK_ERRORS();
	//	std::cout << "Earth " << earthLoc.X() << ", " << earthLoc.Y() << ". Type " << earthLoc.Planet() << std::endl;

	//	canPass = bc_PlanetMap_is_passable_terrain_at(planetMarsPtr, marsLocPtr);
	//	MapLocation marsLoc(marsLocPtr);
	//	if (canPass) 
	//	{
	//		passableMarsTerrain.push_back(marsLoc);
	//	}
	//	CHECK_ERRORS();
	//	std::cout << "Mars " << marsLoc.X() << ", " << marsLoc.Y() << ". Type " << marsLoc.Planet() << std::endl;

	//	uint32_t initialKarb = bc_PlanetMap_initial_karbonite_at(Map(GameController::Planet()), thisPlanetLocPtr);
	//	MapLocation karbLoc(bc_MapLocation_clone(thisPlanetLocPtr));
	//	if (initialKarb)
	//	{
	//		initialKarboniteLocations.push_back(karbLoc);
	//		earthStartingKarbonite += initialKarb;
	//	}
	//	CHECK_ERRORS();
	//	std::cout << "Karbonite " << karbLoc.X() << ", " << karbLoc.Y() << ". Type " << karbLoc.Planet() << std::endl;
	//}
	//delete_bc_VecMapLocation(allLocationsEarth);
	//delete_bc_VecMapLocation(allLocationsMars);

	//std::cout << "There are " << passableEarthTerrain.size() << " passable tiles on earth." << std::endl;
	//std::cout << "There are " << initialKarboniteLocations.size() << " initial Karbonite locations" << std::endl;
	//std::cout << "There is " << earthStartingKarbonite << " total karbonite available." << std::endl;
	//CHECK_ERRORS();



	////////////////////////////////////////////////



}

void PlayerData::Update()
{
	GatherUnitData();
}

void PlayerData::GatherUnitData()
{
	ClearUnitCounts();
	std::vector<units::Unit> units = GameController::Units(bc_Selection::Visible);
	for (auto unit : units)
	{
		bc_Team uTeam = unit.Team();
		bc_UnitType uType = unit.type;

		uTeam == GameController::Team() ? teamUnitCounts[uType]++ : enemyUnitCounts[uType]++;

	}
}

void PlayerData::ClearUnitCounts()
{
	teamUnitCounts.clear();
	enemyUnitCounts.clear();
}



