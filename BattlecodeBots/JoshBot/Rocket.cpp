#include "Rocket.h"

#include "GameController.h"

namespace units {

	Rocket::Rocket()
	{
	}

	Rocket::~Rocket()
	{
	}

	uint8_t Rocket::IsUsed()
	{
		return bc_Unit_rocket_is_used(self);
	}

	uint32_t Rocket::BlastDamage()
	{
		return bc_Unit_rocket_blast_damage(self);
	}

	uint32_t Rocket::ExtraSpeed()
	{
		return bc_Unit_rocket_travel_time_decrease(self);
	}

	uint8_t Rocket::CanLaunch(MapLocation location)
	{
		return bc_GameController_can_launch_rocket(GameController::gc, id, location.self);
	}

	void Rocket::Launch(MapLocation location)
	{
		bc_GameController_launch_rocket(GameController::gc, id, location.self);
		destination = location;
		hasLaunched = 1;
	}

}

