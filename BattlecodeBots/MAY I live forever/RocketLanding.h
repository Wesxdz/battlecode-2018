#ifndef ROCKET_LANDING_H
#define ROCKET_LANDING_H

#include "bc.h"

#include "MapLocation.h"

class RocketLanding {
public:
	bc_RocketLanding* self = nullptr;
public:
	RocketLanding();
	RocketLanding(bc_RocketLanding* self);
	~RocketLanding();

	MapLocation GetDestination();
	int GetRocketID();
};

#endif // !ROCKET_LANDING_H
