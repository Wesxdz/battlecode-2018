#ifndef SECTION_H
#define SECTION_H

#include "bc.h"
#include <vector>
#include <list>
#include "MapLocation.h"

/*
A group of bc_MapLocations that are connected in some way
There is always a way to pathfind between locations in the same section
Units should only care about movement within their section
*/
class Section
{
public:
	~Section();
	std::vector<MapLocation> locations;
	static std::list<Section> GenSections(std::vector<MapLocation>& passables);
	static std::list<Section> marsSections;
	static std::list<Section> earthSections;
};

#endif

