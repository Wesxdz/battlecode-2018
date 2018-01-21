#include "Section.h"

#include "MapUtil.h"
#include <algorithm>
#include "PlanetMap.h"
#include "GameController.h"

std::list<std::shared_ptr<Section>> Section::marsSections;
std::list<std::shared_ptr<Section>> Section::earthSections;

std::list<std::shared_ptr<Deposit>> Deposit::marsDeposits;
std::list<std::shared_ptr<Deposit>> Deposit::earthDeposits;

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
