#include "PlayerData.h"



PlayerData::PlayerData()
{
	if (pd == nullptr)
	{
		pd = new PlayerData();
	}
}

void PlayerData::ClearUnitCounts()
{
	teamUnits.clear();
	enemyUnits.clear();
}



