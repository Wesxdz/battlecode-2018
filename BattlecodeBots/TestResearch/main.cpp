#include <iostream>

#include "bc.h"

#include "GlobalData.h"

#include "Science.h"

GlobalData data;

int main()
{
	srand(0);

	Science science;
	while (true) {
		data.currRound = GameController::Round();

		science.researchNextTurn = true;
		science.Update();

		GameController::EndTurn();
	}
}