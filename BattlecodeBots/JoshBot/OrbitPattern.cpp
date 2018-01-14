#include "OrbitPattern.h"

#include "GameController.h"

OrbitPattern::OrbitPattern()
{
	self = bc_GameController_orbit_pattern(GameController::gc);
}

OrbitPattern::~OrbitPattern()
{
	delete_bc_OrbitPattern(self);
}

uint32_t OrbitPattern::Amplitude()
{
	return bc_OrbitPattern_amplitude_get(self);
}

uint32_t OrbitPattern::Period()
{
	return bc_OrbitPattern_period_get(self);
}

uint32_t OrbitPattern::Center()
{
	return bc_OrbitPattern_center_get(self);
}

uint32_t OrbitPattern::Duration(uint32_t round)
{
	return bc_OrbitPattern_duration(self, round);
}
