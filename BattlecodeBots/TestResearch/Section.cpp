#include "Section.h"

#include <algorithm>

std::list<Section> Section::marsSections;
std::list<Section> Section::earthSections;

Section::~Section()
{
}

std::list<Section> Section::GenSections(std::vector<MapLocation>& passables)
{
	std::list<Section> sections;
	for (auto passsableLocation : passables) {
		auto sectionToJoin = std::find_if(std::begin(sections), std::end(sections), [&passsableLocation](Section& section) {
			for (auto location : section.locations) {
				if (passsableLocation.IsAdjacentTo(location)) {
					return true;
				}
			}
			return false; // MapLocation is the first tile of another section
		});
		if (sectionToJoin != std::end(sections)) {
			(*sectionToJoin).locations.push_back(passsableLocation);
		}
		else {
			Section newSection;
			newSection.locations.push_back(passsableLocation);
			sections.push_back(newSection);
		}
	}
	return sections;
}
