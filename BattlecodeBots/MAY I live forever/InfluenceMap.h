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
	float GetInfluence(MapLocation& location);
	// diffuseEq @return 
	void SetInfluence(MapLocation& location, float amount, int diffuse, std::function<float(float)> DiffuseEq = [](float distance) {
		return 1 / (distance + 1);
	});
	void SetInfluence(int xLoc, int yLoc, float amount, int diffuse, std::function<float(float)> DiffuseEq = [](float distance) {
		return 1 / (distance + 1);
	});
	void Reset();
	void Print();
	void FindMax();
	MapLocation max;
private:
	void Diffuse(int startIndex, float amount, int diffuse , std::function<float(float)> DiffuseEq);
	float* influence;
	bool IsValid(int index);
	bool Overflow(int index, int x);
	uintptr_t width;
	uintptr_t height;
	bc_Planet planet;
};

#endif

