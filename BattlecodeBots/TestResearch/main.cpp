#include <iostream>
#include <memory>
#include <math.h>

#include "bc.h"

#include "GameController.h"
#include "Research.h"
#include "Science.h"
#include "Log.h"

GameController gc;

/*
Instead of micromanaging individual unit behavior, prioritize goals based on available data and
then determine best execution strategy
*/
int main()
{
	srand(0);

	std::cout << "A* test initialize" << std::endl;

	Science science;
	science.Init();

	while (true)
	{
		int currRound = GameController::Round();
		std::cout << "Round " << currRound << std::endl;
		CHECK_ERRORS();

		//science.researchNextTurn = true;
		if (bc_GameController_planet(GameController::gc) == bc_Planet::Earth) {
			science.Update();
		}
		GameController::EndTurn();
	}
}