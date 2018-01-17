#include "RocketLanding.h"


RocketLanding::RocketLanding() {
}
RocketLanding::RocketLanding(bc_RocketLanding* self) {
	this->self = self;
}
RocketLanding::~RocketLanding() {
	delete_bc_RocketLanding(self);
}

MapLocation RocketLanding::GetDestination() {
	return MapLocation(bc_RocketLanding_destination_get(self));
}
int RocketLanding::GetRocketID() {
	return bc_RocketLanding_rocket_id_get(self);
}