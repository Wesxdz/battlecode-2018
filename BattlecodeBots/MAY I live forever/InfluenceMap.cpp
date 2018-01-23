#include "InfluenceMap.h"

#include <stdlib.h>
#include "MapUtil.h"
#include <iostream>

InfluenceMap::InfluenceMap()
{
}

void InfluenceMap::Init(bc_PlanetMap * map)
{
	width = bc_PlanetMap_width_get(map);
	height = bc_PlanetMap_height_get(map);
	planet = bc_PlanetMap_planet_get(map);
	influence = new float[width * height];
}

InfluenceMap::~InfluenceMap()
{
	delete[] influence;
}

float InfluenceMap::GetInfluence(MapLocation& location)
{
	return influence[location.X() + location.Y() * width];
}

void InfluenceMap::SetInfluence(MapLocation& location, float amount, int diffuse, std::function<float(float)> DiffuseEq)
{
	Diffuse(location.X() + location.Y() * width, amount, diffuse, DiffuseEq);
}

void InfluenceMap::Reset()
{
	for (int i = 0; i < width * height; i++) {
		influence[i] = 0;
	}
}

void InfluenceMap::Print()
{
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			std::cout << (int)influence[x + y * height] << " ";
		}
		std::cout << std::endl;
	}
}

void InfluenceMap::FindMax()
{
	int maxIndex = 0;
	float maxInf = 0;
	for (int i = 0; i < width * height; i++) {
		if (influence[i] > maxInf) {
			maxIndex = i;
		}
	}
	int32_t y = maxIndex / width;
	int32_t x = maxIndex % width;
	max = MapLocation(new_bc_MapLocation(planet, x, y));
}

void InfluenceMap::Diffuse(int startIndex, float amount, int diffuse, std::function<float(float)> DiffuseEq)
{
	if (diffuse > 0) {
		for (int y = -diffuse; y < diffuse; y++) {
			for (int x = -diffuse; x < diffuse; x++) {
				int index = startIndex + x + y * width;
				if (IsValid(index) && !Overflow(startIndex, x)) {
					influence[index] += amount * DiffuseEq(abs(x) + abs(y));
				}
			}
		}
	}
}

bool InfluenceMap::IsValid(int index)
{
	return index > 0 && index < width * height;
}

bool InfluenceMap::Overflow(int index, int x)
{
	int curX = (index % width) + x;
	return curX > width && curX < 0;
}
