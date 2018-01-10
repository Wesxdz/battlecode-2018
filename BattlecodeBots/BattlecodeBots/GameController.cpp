#include "GameController.h"

GameController::GameController()
{
	self = new_bc_GameController();
}

GameController::~GameController()
{
	delete_bc_GameController(self);
}
