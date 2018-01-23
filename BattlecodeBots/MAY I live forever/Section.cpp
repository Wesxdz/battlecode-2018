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

std::list<std::shared_ptr<Deposit>> Deposit::marsDeposits;
std::list<std::shared_ptr<Deposit>> Deposit::earthDeposits;

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
				std::cout << "Found false section!" << std::endl;
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
			std::cout << "Adding: " << loc.X() << ", " << loc.Y() << std::endl;
			sectionMap[Key(loc)] = section;
			std::cout << sectionMap[Key(loc)]->karboniteDeposits.size();
		}
	}
	std::cout << sections.size() << " sections found" << std::endl;
	return sections;
}

void Section::FindEarthSectionsStatus()
{
	auto team = PlayerData::pd->teamSpawnPositions;
	auto enemy = PlayerData::pd->enemySpawnPositions;
	for (auto section : earthSections) {
		bool enemyHere = false;
		bool teamHere = false;
		for (bc_MapLocation* location : section->locations) {
			MapLocation loc{ bc_MapLocation_clone(location) };
			auto findTeam = std::find(team.begin(), team.end(), loc);
			auto findEnemy = std::find(enemy.begin(), enemy.end(), loc);
			if (findTeam != team.end()) {
				teamHere = true;
				team.erase(findTeam);
			}
			if (findEnemy != enemy.end()) {
				enemyHere = true;
				enemy.erase(findEnemy);
			}
			if (teamHere && enemyHere) {
				section->status = StartStatus::Mixed;
				break;
			}
		}
		if (teamHere) {
			section->status = StartStatus::Team;
		}
		else if (enemyHere) {
			section->status = StartStatus::Enemy;
		}
		else {
			section->status = StartStatus::None;
		}
	}
}

int Section::Key(MapLocation location)
{
	return location.Planet() * 3000 + location.Y() * 51 + location.X();
}

bool Section::InSame(MapLocation & a, MapLocation & b)
{
	return false;
	//return sectionMap[a] == sectionMap[b];
}

std::list<std::shared_ptr<Deposit>> Deposit::GenDeposits(std::vector<bc_MapLocation*>& locations)
{
	bc_PlanetMap* map = GameController::PlanetMap(bc_MapLocation_planet_get(locations[0]));
	std::list<std::shared_ptr<Deposit>> deposits;
	for (auto planetLocation : locations) {
		auto depositToJoin = std::find_if(std::begin(deposits), std::end(deposits), [&map, &planetLocation](std::shared_ptr<Deposit> deposit) {
			for (auto location : deposit->locations) {
				if ((bc_MapLocation_is_adjacent_to(planetLocation, location) && bc_PlanetMap_is_passable_terrain_at(map, location)) && 
					bc_PlanetMap_initial_karbonite_at(map, planetLocation) > 0) {
					return true; // MapLocation is in deposit
				}
			}
			return false; // MapLocation is the first tile of another section
		});
		if (depositToJoin != std::end(deposits)) {
			(*depositToJoin)->karbonite += bc_PlanetMap_initial_karbonite_at(map, planetLocation);
			(*depositToJoin)->locations.push_back(planetLocation);
		}
		else if (bc_PlanetMap_is_passable_terrain_at(map, planetLocation) && bc_PlanetMap_initial_karbonite_at(map, planetLocation) > 0) {
			auto newDeposit = std::make_shared<Deposit>();
			newDeposit->locations.push_back(planetLocation);
			newDeposit->karbonite += bc_PlanetMap_initial_karbonite_at(map, planetLocation);
			newDeposit->landing = planetLocation;
			deposits.push_back(newDeposit);
		}
	}
	delete_bc_PlanetMap(map);
	return deposits;
}
