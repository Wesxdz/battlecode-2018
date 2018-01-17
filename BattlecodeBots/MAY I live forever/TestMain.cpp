#include <iostream>
#include <memory>
#include <math.h>

#include "bc.h"

#include "PlayerData.h"
#include "GameController.h"

#include "Unit.h"
#include "Worker.h"
#include "MapLocation.h"
#include "Location.h"

#include "GameMap.h"
#include "Research.h"
#include "OrbitPattern.h"
#include "AsteroidPattern.h"
#include "RocketLandingInfo.h"
#include "TeamArray.h"
#include "Utility.h"



GameController gc;
GameMap gameMap;

Research research;
OrbitPattern orbitPattern;
AsteroidPattern asteroidPattern;
RocketLandingInfo rocketLandingInfo;
TeamArray teamArray;
PlayerData playerData;

int main()
{
	srand(0);
	std::cout << "Playerdata test initialize" << std::endl;

	while (true)
	{
		uint32_t round = GameController::Round();
		std::cout << "Round: " << round << std::endl;
		playerData.Update();

		



		GameController::EndTurn();
	}
}

