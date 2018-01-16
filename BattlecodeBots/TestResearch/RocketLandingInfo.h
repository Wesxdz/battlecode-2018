#ifndef ROCKET_LANDING_INFO_H
#define ROCKET_LANDING_INFO_H

#include "bc.h"

#include <vector>

#include "RocketLanding.h"

class RocketLandingInfo {
public:
	static bc_RocketLandingInfo* self;
public:
	RocketLandingInfo();
	~RocketLandingInfo();

	/// Get the rocket landings on this round.
	std::vector<RocketLanding> GetRocketLandings(int round);
};

#endif // !ROCKET_LANDING_INFO_H
