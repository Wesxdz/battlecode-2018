#include <iostream>
#include <memory>
#include <math.h>

#include "bc.h"

#include "GameController.h"
#include "OrbitPattern.h"
#include "PlayerData.h"
#include "MapUtil.h"

#include "Research.h"
#include "Science.h"
#include "Log.h"

GameController gc;
OrbitPattern orbitPattern;
PlayerData data;
MapUtil mapUtil;

/*
Instead of micromanaging individual unit behavior, prioritize goals based on available data and
then determine best execution strategy
*/
int main()
{
	srand(0);

	std::cout << "A* test initialize" << std::endl;

	Science science;
	science.Init(&data);
	CHECK_ERRORS();

	while (true)
	{
		int currRound = GameController::Round();
		std::cout << "Round " << currRound << std::endl;
		CHECK_ERRORS();

		data.Update();
		CHECK_ERRORS();

		if (bc_GameController_planet(GameController::gc) == bc_Planet::Earth) {
			science.Update(); // Upgrade on Earth
		} else if(currRound > 749) {
			science.Update(); // Upgrade on Mars
		}
		CHECK_ERRORS();
		GameController::EndTurn();
	}
}