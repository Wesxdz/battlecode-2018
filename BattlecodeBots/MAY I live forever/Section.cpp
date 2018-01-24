#include "Section.h"

#include "MapUtil.h"
#include <algorithm>
#include "PlanetMap.h"
#include "GameController.h"
#include "PlayerData.h"
#include <iostream>

std::list<Section*> Section::marsSections;
std::list<Section*> Section::earthSections;
std::map<int, Section*> Section::sectionMap;

Section::~Section()
{
	for (auto location : locations) {
		delete_bc_MapLocation(location);
	}
}

void Section::Add(Section* section)
{
	locations.insert(locations.end(), section->locations.begin(), section->locations.end());
}

std::list<Section*> Section::GenSections(std::vector<bc_MapLocation*>& passables, bc_PlanetMap* planetMap)
{
	std::list<Section*> sections;

	for (auto passsableLocation : passables) {
		auto sectionToJoin = std::find_if(std::begin(sections), std::end(sections), [&passsableLocation](Section* section) {
			for (auto location : section->locations) {
				if (bc_MapLocation_is_adjacent_to(passsableLocation, location)) {
					return true; // MapLocation is in section
				}
			}
			return false; // MapLocation is the first tile of another section
		});
		if (sectionToJoin != std::end(sections)) { // We found a section to join!

			(*sectionToJoin)->locations.push_back(passsableLocation);

			auto nextSec = sectionToJoin;
			nextSec++;
			auto anotherSection = std::find_if(nextSec, sections.end(), [&passsableLocation](Section* section) {
				for (auto location : section->locations) {
					if (bc_MapLocation_is_adjacent_to(passsableLocation, location)) {
						return true; // MapLocation is in section
					}
				}
				return false; // MapLocation is the first tile of another section
			});
			if (anotherSection != sections.end()) { // Oh dear, this location is adjacent to two sections. We need to merge them
				(*sectionToJoin)->Add(*anotherSection);
				sections.remove(*anotherSection);
			}
		}	
		else {
			auto newSection = new Section;
			newSection->locations.push_back(passsableLocation);
			sections.push_back(newSection);
		}
	}
	for (Section* section : sections) {
		for (bc_MapLocation* location : section->locations) {
			if (bc_PlanetMap_initial_karbonite_at(planetMap, location) > 0) section->karboniteDeposits.push_back(MapLocation{ bc_MapLocation_clone(location) });
			MapLocation loc(bc_MapLocation_clone(location));
			sectionMap[Key(loc)] = section;
		}
	}
	return sections;
}

void Section::FindEarthSectionsStatus()
{
	for (MapLocation& location : PlayerData::pd->teamSpawnPositions) {
		Section* section = Get(location);
		if (section->status == StartStatus::None) {
			section->status = StartStatus::Team;
		}
		else if (section->status == StartStatus::Enemy) {
			section->status = StartStatus::Mixed;
		}
	}
	for (MapLocation& location : PlayerData::pd->enemySpawnPositions) {
		Section* section = Get(location);
		if (section->status == StartStatus::None) {
			section->status = StartStatus::Enemy;
		}
		else if (section->status == StartStatus::Team) {
			section->status = StartStatus::Mixed;
		}
	}
}

int Section::Key(MapLocation& location)
{
	return location.Planet() * 3000 + location.Y() * 51 + location.X();
}

int Section::Key(bc_Planet type, int x, int y)
{
	return type * 3000 + y * 51 + x;
}

Section* Section::Get(MapLocation & location)
{
	return sectionMap[Key(location)];
}

Section* Section::Get(bc_Planet type, int x, int y)
{
	return sectionMap[Key(type, x, y)];
}

bool Section::InSame(MapLocation & a, MapLocation & b)
{
	return Get(a) == Get(b);
}

