#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "bc.h"
#include <array>

namespace constants
{
	const std::array<bc_Direction, 9> directions_all = { North, Northeast, East, Southeast, South, Southwest, West, Northwest, Center };
	const std::array<bc_Direction, 8> directions_adjacent = { North, Northeast, East, Southeast, South, Southwest, West, Northwest };
	const std::array<bc_Direction, 4> directions_knight_attack = { North, East, South, West };
	const std::array<bc_UnitType, 7> types_units = { Factory, Rocket, Worker, Knight, Healer, Mage, Ranger };
	const std::array<bc_UnitType, 2> types_structures = { Factory, Rocket };
	const std::array<bc_UnitType, 5> types_robots = { Worker, Knight, Healer, Mage, Ranger };
	const std::array<bc_UnitType, 3> types_fighters = { Knight, Ranger, Mage };
	const int RocketLoadAmo = 8;
	const uint32_t MAP_SIZE_MIN = 400;
	const uint32_t MAP_SIZE_MAX = 2500;

	const int WorkerUpgrade1 = 25;
	const int WorkerUpgrade2 = 75;
	const int WorkerUpgrade3 = 75;
	const int WorkerUpgrade4 = 75;

	const int KnightUpgrade1 = 25;
	const int KnightUpgrade2 = 75;
	const int KnightUpgrade3 = 100;

	const int RangerUpgrade1 = 25;
	const int RangerUpgrade2 = 100;
	const int RangerUpgrade3 = 200;

	const int MageUpgrade1 = 25;
	const int MageUpgrade2 = 75;
	const int MageUpgrade3 = 100;
	const int MageUpgrade4 = 75;

	const int HealerUpgrade1 = 25;
	const int HealerUpgrade2 = 100;
	const int HealerUpgrade3 = 100;

	const int RocketUpgrade1 = 50;
	const int RocketUpgrade2 = 100;
	const int RocketUpgrade3 = 100;
};

#endif
