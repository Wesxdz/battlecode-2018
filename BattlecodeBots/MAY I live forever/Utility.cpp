#include "Utility.h"

#include <iostream>
#include "GameMap.h"
#include "GameController.h"

namespace Utility{

	std::array<int, 5> Utility::tryRotate = { 0, -1, 1, -2, 2 };

	bc_Team GetOtherTeam(bc_Team team) {
		return team == bc_Team::Red ? bc_Team::Blue : bc_Team::Red;
	}

	bc_Planet GetOtherPlanet(bc_Planet type) {
		return type == bc_Planet::Earth ? bc_Planet::Mars : bc_Planet::Earth;
	}

	bool IsRobot(bc_UnitType type) {
		return type < bc_UnitType::Rocket;
	}

	bool Utility::IsSpotOpen(int x, int y, bool mars)
	{
		bool returnBool = false;
		if (mars)
		{
			bc_MapLocation* mp = new_bc_MapLocation(GameMap::Mars().Planet(), x, y);

			returnBool = bc_PlanetMap_is_passable_terrain_at(GameMap::Mars().self, mp);

			if (returnBool)
			{
				returnBool = bc_GameController_has_unit_at_location(GameController::gc, mp);
			}

			delete_bc_MapLocation(mp);
		}
		else
		{
			bc_MapLocation* mp = new_bc_MapLocation(GameMap::Earth().Planet(), x, y);

			returnBool = bc_PlanetMap_is_passable_terrain_at(GameMap::Earth().self, mp);

			if (returnBool)
			{
				returnBool = bc_GameController_has_unit_at_location(GameController::gc, mp);
			}

			delete_bc_MapLocation(mp);
		}


		return returnBool;
	}

	//MapLocation GetRandomPosition(GameInfo* info, MapLocation* temp) {
	//	auto canPassTerrainAt = bc_PlanetMap_is_passable_terrain_at(info->planetMap.self, temp->self);
	//	std::cout << "WE " << (canPassTerrainAt ? "can" : "can not") << " pass this terrain" << std::endl;
	//	std::cout << "It is at " << temp->X() << ", " << temp->Y() << std::endl;

	//	int width = info->planetMap.width;
	//	int height = info->planetMap.height;
	//	bc_Planet type = info->planetMap.planetType;

	//	int x = rand() % width;
	//	int y = rand() % height;
	//	MapLocation m(type, x, y);
	//	auto canPass = bc_PlanetMap_is_passable_terrain_at(info->planetMap.self, m.self);
	//	std::cout << m.self << " " << info->planetMap.self << "  -The location " << x << ", " << y << " is " << (canPass ? "passable" : "not passable") << std::endl;
	//	while (!canPass) {
	//		x++;
	//		if (x >= width) {
	//			x = 0;
	//			y++;
	//			if (y >= height) {
	//				y = 0;
	//			}
	//		}
	//		m = MapLocation(type, x, y);
	//		canPass = bc_PlanetMap_is_passable_terrain_at(info->planetMap.self, m.self);
	//		std::cout << m.self << " " << info->planetMap.self << "  -The location " << x << ", " << y << " is " << (canPass ? "passable" : "not passable") << std::endl;
	//	}
	//	return m;
	//}

}