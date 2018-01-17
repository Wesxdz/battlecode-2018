#ifndef SECTION_H
#define SECTION_H

#include "bc.h"
#include <vector>
#include <list>
#include <memory>

/*
A group of bc_MapLocations that are connected in some way
There is always a way to pathfind between locations in the same section
Units should only care about movement within their section
*/
class Section
{
public:
	~Section();
	std::vector<bc_MapLocation*> locations;
	static std::list<std::shared_ptr<Section>> GenSections(std::vector<bc_MapLocation*>& passables);
	static std::list<std::shared_ptr<Section>> marsSections;
	static std::list<std::shared_ptr<Section>> earthSections;
};

#endif

