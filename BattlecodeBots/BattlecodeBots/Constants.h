#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "bc.h"
#include <array>

namespace constants
{
	const std::array<bc_Direction, 9> directions_all = { North, Northeast, East, Southeast, South, Southwest, West, Northwest, Center };
	const std::array<bc_Direction, 8> directions_adjacent = { North, Northeast, East, Southeast, South, Southwest, West, Northwest };
	const std::array<bc_UnitType, 7> types_units = { Factory, Rocket, Worker, Knight, Healer, Mage, Ranger };
	const std::array<bc_UnitType, 2> types_structures = { Factory, Rocket };
	const std::array<bc_UnitType, 5> types_robots = { Worker, Knight, Healer, Mage, Ranger };
	const std::array<bc_UnitType, 3> types_fighters = { Knight, Ranger, Mage };
	const uint32_t MAP_SIZE_MIN = 400;
	const uint32_t MAP_SIZE_MAX = 2500;
};

#endif
