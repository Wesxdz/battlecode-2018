#include "Player.h"

bc_GameController* Player::gc = nullptr;
bc_PlanetMap* Player::earth = nullptr;
bc_PlanetMap* Player::mars = nullptr;
bc_Team Player::team;

void Player::Init()
{
	gc = new_bc_GameController();
	earth = bc_GameController_starting_map(gc, Earth);
	mars = bc_GameController_starting_map(gc, Mars);
	team = bc_GameController_team(gc);
}

bc_PlanetMap* Player::Map(bc_Planet planet)
{
	return planet == Earth ? earth : mars;
}

void Player::Shutdown()
{
	delete_bc_PlanetMap(earth);
	delete_bc_PlanetMap(mars);
	delete_bc_GameController(gc);
}
