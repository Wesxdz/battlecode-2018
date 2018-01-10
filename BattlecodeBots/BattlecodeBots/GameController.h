#pragma once

#include "bc.h"

class GameController
{
public:
	static void Initialize();
	static bc_GameController* gc;
public:
	GameController();
	~GameController();
};

