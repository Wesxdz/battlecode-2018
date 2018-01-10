#include "GameMap.h"

GameMap::GameMap()
{
	self = new_bc_GameMap();
}

GameMap::~GameMap()
{
	delete_bc_GameMap(self);
}
