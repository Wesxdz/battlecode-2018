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

#include "Strategist.h"
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

Strategist strategist;
CombatOverlord combatLord;
BuilderOverlord builderLord;
PolicyOverlord policyLord;


int main()
{
	srand(0);
	std::chrono::duration<double> roundTime;
	while (true)
	{
		uint32_t round = GameController::Round();
		auto timeLeft = bc_GameController_get_time_left_ms(GameController::gc);
		if (round % 10 == 0) {
			std::cout << "Round: " << round << " Time Left: " << timeLeft << std::endl;
		}
		if (timeLeft < 1000) {
			std::cout << "ENDING TURN EARLY (Time remaining limited)" << std::endl;
			GameController::EndTurn();
			continue;
		}
		playerData.Update();

		auto start = std::chrono::system_clock::now();
		builderLord.Update();
		auto end = std::chrono::system_clock::now();
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

		combatLord.LateUpdate();

		GameController::EndTurn();
	}
}


