#include "RocketLandingInfo.h"

bc_RocketLandingInfo* RocketLandingInfo::self = nullptr;

RocketLandingInfo::RocketLandingInfo() {
	self = new_bc_RocketLandingInfo();
}
RocketLandingInfo::~RocketLandingInfo() {
	delete_bc_RocketLandingInfo(self);
	self = nullptr;
}

std::vector<RocketLanding> RocketLandingInfo::GetRocketLandings(int round) {
	auto rocketLandVecPtr = bc_RocketLandingInfo_landings_on(self, round);
	int length = bc_VecRocketLanding_len(rocketLandVecPtr);
	std::vector<RocketLanding> rocketLandings(length);
	for (int i = 0; i < length; i++) {
		rocketLandings.push_back(RocketLanding(bc_VecRocketLanding_index(rocketLandVecPtr, i)));
	}
	return rocketLandings;
}