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
		auto timeLeft = bc_GameController_get_time_left_ms(GameController::gc);
		if (timeLeft < roundTime.count() * 3) {
			std::cout << "ENDING TURN EARLY: " << timeLeft << std::endl;
			GameController::EndTurn();
			continue;
		}
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
	
		// Pathfind Test
		/*
		{
			start = std::chrono::system_clock::now();
			auto units = GameController::Units(bc_Selection::MyTeam);
			for (int i = 0; i < units.size(); i++) {
				units::Unit* unit = &units[i];
				if (Utility::IsAttackRobot(unit->type)) {
					Location loc = unit->Loc();
					if (loc.IsOnMap()) {
						MapLocation mapLoc = loc.ToMapLocation();
						units::Robot robot(bc_Unit_clone(unit->self));

						// Change pathfind to the specific place
						// For example, a karb deposit or enemy position

						Pathfind::MoveFuzzyFlow(robot, PlayerData::pd->enemySpawnPositions[0]);
					}
					
				}
			}
			end = std::chrono::system_clock::now();
			roundTime = end - start;
			std::cout << "Path Finding Time: " << roundTime.count() << std::endl;
		}
		//*/

		start = std::chrono::system_clock::now();
		policyLord.Update();
		end = std::chrono::system_clock::now();
		roundTime = end - start;
		//std::cout << "Policy Time: " << roundTime.count() << std::endl;

		
		GameController::EndTurn();
	}
}

// Worker and Attackers should all use Flow
// Workers should search for karb within dist, or find it using the initial Karb
// Workers should make sure karb is reachable before moving
// Balance out rangers and knight.
// Group
// Flow might not be working?