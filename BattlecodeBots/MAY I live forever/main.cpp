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
BuilderOverlord builderLord;
CombatOverlord combatLord;
PolicyOverlord policyLord;


int main()
{
	srand(0);

	// Init Science
	science.Init(&playerData);
	std::chrono::duration<double> totalTime;
	std::chrono::duration<double> roundTime;

	while (true)
	{
		uint32_t round = GameController::Round();
		if (round % 10 == 0) {
			std::cout << "Round: " << round << std::endl;
		}
		auto timeLeft = std::chrono::duration<double>(10 + .05 * round) - totalTime;
		if (timeLeft < roundTime * 3) {
			std::cout << "Almost out of time: Halt turn" << std::endl;
			GameController::EndTurn(); // last round;
			continue;
		}
		playerData.Update();
		if (GameController::Planet() == bc_Planet::Earth) {
			science.Update();
		}
		else if (round > 749) {
			science.Update();
		}
		builderLord.Update();
		combatLord.Update();
		auto start = std::chrono::system_clock::now();
		policyLord.Update();
		auto end = std::chrono::system_clock::now();
		roundTime = end - start;
		totalTime += roundTime;
		//std::cout << "Total time used: " << totalTime.count() << "\nPolicy time used: " << roundTime.count() << "\n";
		GameController::EndTurn();
	}
}