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

#include "OrbitPattern.h"
#include "AsteroidPattern.h"
#include "RocketLandingInfo.h"
#include "TeamArray.h"
#include "Utility.h"
#include "MapUtil.h"

#include "Science.h"
#include "CombatOverlord.h"
#include "BuilderOverlord.h"
#include "PolicyOverlord.h"

#include <chrono>

GameController gc;
OrbitPattern orbitPattern;
AsteroidPattern asteroidPattern;
RocketLandingInfo rocketLandingInfo;
TeamArray teamArray;
MapUtil mapUtil;
PlayerData playerData;

Science science;
BuilderOverlord evan;
CombatOverlord josh;
PolicyOverlord wesley;


int main()
{
	//bc_GameController_get_time_left_ms(); This gets the time remaining supposedly. Header file doesnt have it
	srand(0);

	// Init Science
	science.Init(&playerData);
	std::chrono::duration<double> totalTime;

	while (true)
	{
		uint32_t round = GameController::Round();
		if (round % 10 == 0) {
			std::cout << "Round: " << round << std::endl;
		}
		playerData.Update();
		if (GameController::Planet() == bc_Planet::Earth) {
			science.Update();
		}
		else if (round > 749) {
			science.Update();
		}
		evan.Update();
		josh.Update();
		auto start = std::chrono::system_clock::now();
		wesley.Update();
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> roundTime = end - start;
		totalTime += roundTime;
		//std::cout << "Total time used: " << totalTime.count() << "\nPolicy time used: " << roundTime.count() << "\n";
		GameController::EndTurn();
	}
}