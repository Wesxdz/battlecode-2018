#include "Utility.h"

#include <iostream>
#include "GameController.h"

namespace Utility{

	

	bc_Team GetOtherTeam(bc_Team team) {
		return team == bc_Team::Red ? bc_Team::Blue : bc_Team::Red;
	}

	bc_Planet GetOtherPlanet(bc_Planet type) {
		return type == bc_Planet::Earth ? bc_Planet::Mars : bc_Planet::Earth;
	}

	bool IsRobot(bc_UnitType type) {
		return type < bc_UnitType::Rocket;
	}

	bool IsAttackRobot(bc_UnitType type) {
		return type > bc_UnitType::Worker && type < bc_UnitType::Healer;
	}

	bc_Direction DirectionTo(int sourceX, int sourceY, int destX, int destY) {
		int resultX = destX - sourceX;
		int resultY = destY - sourceY;
		
		// Right
		if (resultX > 0) {
			if (resultY > 0) {
				return bc_Direction::Northeast;
			} else if (resultY < 0) {
				return bc_Direction::Southeast;
			} else {
				return bc_Direction::East;
			}
		} 
		// Left
		else if (resultX < 0) {
			if (resultY > 0) {
				return bc_Direction::Northwest;
			} else if (resultY < 0) {
				return bc_Direction::Southwest;
			} else {
				return bc_Direction::West;
			}
		} 
		// Center
		else {
			if (resultY > 0) {
				return bc_Direction::North;
			} else if (resultY < 0) {
				return bc_Direction::South;
			} else {
				return bc_Direction::Center;
			}
		}
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