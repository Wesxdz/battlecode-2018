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
	//bc_GameController_get_time_left_ms(); This gets the time remaining.
	srand(0);

	// Init Science
	science.Init(&playerData);
	//std::chrono::duration<double> totalTime;

	while (true)
	{
		uint32_t round = GameController::Round();
		if (round % 10 == 0) {
			std::cout << "Round: " << round << std::endl;
		}

		//auto start = std::chrono::system_clock::now();
		playerData.Update();
		//auto end = std::chrono::system_clock::now();
		//std::chrono::duration<double> roundTime = end - start;
		//std::cout << "Data Time: " << roundTime.count() << std::endl;

		//start = std::chrono::system_clock::now();
		if (GameController::Planet() == bc_Planet::Earth) {
			science.Update();
		}
		else if (round > 749) {
			science.Update();
		}
		//end = std::chrono::system_clock::now();
		//roundTime = end - start;
		//std::cout << "Science Time: " << roundTime.count() << std::endl;

		//start = std::chrono::system_clock::now();
		evan.Update();
		//end = std::chrono::system_clock::now();
		//roundTime = end - start;
		//std::cout << "Builder Time: " << roundTime.count() << std::endl;

		//start = std::chrono::system_clock::now();
		josh.Update();
		if (round < 5) playerData.unitPriority[Worker] = 10.0f; // TODO Temporary, make better unitPriority evaluations elsewhere :P
		if (round > 5 && round < 25) playerData.unitPriority[Factory] = 10.0f;
		if (round > 25) playerData.unitPriority[Ranger] = 10.0f;
		auto start = std::chrono::system_clock::now();
		//end = std::chrono::system_clock::now();
		//roundTime = end - start;
		//std::cout << "Combater Time: " << roundTime.count() << std::endl;
		
		//start = std::chrono::system_clock::now();
		wesley.Update();
		//end = std::chrono::system_clock::now();
		//roundTime = end - start;
		//std::cout << "Policy Time: " << roundTime.count() << std::endl;

		std::cout << "Total time used: " << bc_GameController_get_time_left_ms(GameController::gc) << std::endl;;
		GameController::EndTurn();
	}
}