#ifndef INFLUENCE_MAP_H
#define INFLUENCE_MAP_H

#include "MapLocation.h"
#include <functional>

class InfluenceMap
{
public:
	InfluenceMap();
	void Init(bc_PlanetMap* map);
	~InfluenceMap();
	float GetInfluence(MapLocation location);
	void SetInfluence(MapLocation location, float amount, int diffuse);
	void Reset();
	void Print();
private:
	void Diffuse(int startIndex, float amount, int diffuse);
	float* influence;
	bool IsValid(int index);
	uintptr_t width;
	uintptr_t height;
	bc_Planet planet;
};

#endif

