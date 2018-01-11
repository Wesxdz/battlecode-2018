#include "GameController.h"
#include "GameMap.h"
#include "Unit.h"
#include "Worker.h"
#include <iostream>

/*
Instead of micromanaging individual unit behavior, prioritize goals based on available data and
then determine best execution strategy
*/
int main()
{
	std::cout << "WesleyBot initialize" << std::endl;
	GameController player;
	GameMap spaceMap;
	PlanetMap planetMap;
	if (player.Planet() == bc_Planet::Earth) {
		planetMap = spaceMap.Earth();
	}
	else {
		planetMap = spaceMap.Mars();
	}
	auto locations = planetMap.Locations();
	for (MapLocation& location : locations) {

	}
	while (true)
	{
		uint32_t round = player.Round();
		std::cout << "Round: " << round << std::endl;
		auto units = player.Units(bc_Selection::MyTeam);
		player.EndTurn();
	}
}