#include "Section.h"

#include "MapUtil.h"
#include "Player.h"
#include <algorithm>

Section::~Section()
{
	for (auto location : locations) {
		delete_bc_MapLocation(location);
	}
}

std::list<std::shared_ptr<Section>> Section::GenSections(bc_PlanetMap* map)
{
	std::list<std::shared_ptr<Section>> sections;
	auto all = MapUtil::AllLocations(map);
	auto passables = MapUtil::FilteredLocations(all, [&map](bc_MapLocation* location) {
		return bc_PlanetMap_is_passable_terrain_at(map, location);
	});
	for (auto location : all) {
		delete_bc_MapLocation(location);
	}
	for (auto passsableLocation : passables) {
		auto sectionToJoin = std::find_if(std::begin(sections), std::end(sections), [&passsableLocation](std::shared_ptr<Section> section) {
			for (auto location : section->locations) {
				if (bc_MapLocation_is_adjacent_to(passsableLocation, location)) {
					return true; // MapLocation is in section
				}
			}
			return false; // MapLocation is the first tile of another section
		});
		if (sectionToJoin != std::end(sections)) {
			(*sectionToJoin)->locations.push_back(passsableLocation);
		}
		else {
			auto newSection = std::make_shared<Section>();
			newSection->locations.push_back(passsableLocation);
			sections.push_back(newSection);
		}
	}
	return sections;
}
