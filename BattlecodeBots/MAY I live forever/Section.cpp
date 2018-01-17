#include "Section.h"

#include "MapUtil.h"
#include <algorithm>

std::list<std::shared_ptr<Section>> Section::marsSections;
std::list<std::shared_ptr<Section>> Section::earthSections;

Section::~Section()
{
	for (auto location : locations) {
		delete_bc_MapLocation(location);
	}
}

std::list<std::shared_ptr<Section>> Section::GenSections(std::vector<bc_MapLocation*>& passables)
{
	std::list<std::shared_ptr<Section>> sections;
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
