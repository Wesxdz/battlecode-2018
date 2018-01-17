#include "AsteroidStrike.h"

AsteroidStrike::AsteroidStrike(bc_AsteroidStrike* strike)
{
	self = strike;
}

AsteroidStrike::AsteroidStrike(const AsteroidStrike& strike)
{
	self = bc_AsteroidStrike_clone(strike.self);
}

AsteroidStrike::~AsteroidStrike()
{
	delete_bc_AsteroidStrike(self);
}

uint32_t AsteroidStrike::Karbonite()
{
	return bc_AsteroidStrike_karbonite_get(self);
}

MapLocation AsteroidStrike::Loc()
{
	return MapLocation(bc_AsteroidStrike_location_get(self));
}
