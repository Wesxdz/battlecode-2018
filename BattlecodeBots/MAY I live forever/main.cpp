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

#include "Pathfind.h"

#include <chrono>

GameController gc;
OrbitPattern orbitPattern;
AsteroidPattern asteroidPattern;
RocketLandingInfo rocketLandingInfo;
TeamArray teamArray;
MapUtil mapUtil;
PlayerData playerData;

Science science;
BuilderOverlord builderLord;
CombatOverlord combatLord;
PolicyOverlord policyLord;


int main()
{
	//bc_GameController_get_time_left_ms(); This gets the time remaining supposedly. Header file doesnt have it
	srand(0);

	// Init Science
	science.Init(&playerData);
	//std::chrono::duration<double> totalTime;
	std::chrono::duration<double> roundTime;

	while (true)
	{
		uint32_t round = GameController::Round();
		if (round % 10 == 0) {
			std::cout << "Round: " << round << std::endl;
		}
		//auto timeLeft = bc_GameController_get_time_left_ms(GameController::gc);
		//if (timeLeft < 1000) {
		//	std::cout << "ENDING TURN EARLY (Time remaining limited)" << std::endl;
		//	GameController::EndTurn();
		//	continue;
		//}
		playerData.Update();

		auto start = std::chrono::system_clock::now();
		if (GameController::Planet() == bc_Planet::Earth) {
			science.Update();
		}
		else if (round > 749) {
			science.Update();
		}
		auto end = std::chrono::system_clock::now();
		roundTime = end - start;
		//std::cout << "Science Time: " << roundTime.count() << std::endl;

		start = std::chrono::system_clock::now();
		builderLord.Update();
		end = std::chrono::system_clock::now();
		roundTime = end - start;
		//std::cout << "Builder Time: " << roundTime.count() << std::endl;

		start = std::chrono::system_clock::now();
		combatLord.Update();

		end = std::chrono::system_clock::now();
		roundTime = end - start;
		//std::cout << "Combater Time: " << roundTime.count() << std::endl;

		start = std::chrono::system_clock::now();
		policyLord.Update();
		end = std::chrono::system_clock::now();
		roundTime = end - start;
		//std::cout << "Policy Time: " << roundTime.count() << std::endl;

		//std::cout << "Total time left: " << bc_GameController_get_time_left_ms(GameController::gc) << std::endl;
		GameController::EndTurn();
	}
}