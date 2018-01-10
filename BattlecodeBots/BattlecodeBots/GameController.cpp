#include "GameController.h"

bc_GameController* GameController::gc = nullptr;

void GameController::Initialize()
{
	gc = new_bc_GameController();
}

GameController::GameController()
{
}


GameController::~GameController()
{
}
