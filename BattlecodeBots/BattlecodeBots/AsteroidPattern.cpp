#include "AsteroidPattern.h"

#include "GameController.h"

AsteroidPattern::AsteroidPattern()
{
	self = bc_GameController_asteroid_pattern(GameController::gc);
}

AsteroidPattern::~AsteroidPattern()
{
	delete_bc_AsteroidPattern(self);
}

uint8_t AsteroidPattern::WillAsteroidStrike(uint32_t round)
{
	return bc_AsteroidPattern_has_asteroid(self, round);
}

AsteroidStrike AsteroidPattern::Strike(uint32_t round)
{
	return AsteroidStrike(bc_AsteroidPattern_asteroid(self, round));
}
