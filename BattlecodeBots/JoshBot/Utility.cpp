#include "Utility.h"

#include <iostream>

namespace Utility{

	bool CheckError() {
		if (bc_has_err())
		{
			char* err;
			int8_t code = bc_get_last_err(&err);
			std::cout << "Engine error code " << code << ": " << err << std::endl;
			bc_free_string(err);
			return true;
		}
		else
		{
			return false;
		}
	}

	bc_Team GetOtherTeam(bc_Team team) {
		return team == bc_Team::Red ? bc_Team::Blue : bc_Team::Red;
	}

	bool IsRobot(bc_UnitType type) {
		return type < bc_UnitType::Rocket;
	}

	MapLocation GetRandomPosition(GameInfo* info, MapLocation* temp) {
		auto canPassTerrainAt = bc_PlanetMap_is_passable_terrain_at(info->planetMap.self, temp->self);
		std::cout << "WE " << (canPassTerrainAt ? "can" : "can not") << " pass this terrain" << std::endl;
		std::cout << "It is at " << temp->X() << ", " << temp->Y() << std::endl;

		int width = info->planetMap.width;
		int height = info->planetMap.height;
		bc_Planet type = info->planetMap.planetType;

		int x = rand() % width;
		int y = rand() % height;
		MapLocation m(type, x, y);
		auto canPass = bc_PlanetMap_is_passable_terrain_at(info->planetMap.self, m.self);
		std::cout << m.self << " " << info->planetMap.self << "  -The location " << x << ", " << y << " is " << (canPass ? "passable" : "not passable") << std::endl;
		while (!canPass) {
			x++;
			if (x >= width) {
				x = 0;
				y++;
				if (y >= height) {
					y = 0;
				}
			}
			m = MapLocation(type, x, y);
			canPass = bc_PlanetMap_is_passable_terrain_at(info->planetMap.self, m.self);
			std::cout << m.self << " " << info->planetMap.self << "  -The location " << x << ", " << y << " is " << (canPass ? "passable" : "not passable") << std::endl;
		}
		return m;
	}

}