#include "Ranger.h"

#include "GameController.h"
#include "Log.h"

namespace units {

	Ranger::Ranger()
	{
	}

	Ranger::~Ranger()
	{
	}

	uint32_t Ranger::AttackRangeMin()
	{
		return bc_Unit_ranger_cannot_attack_range(self);
	}

	uint32_t Ranger::SnipeSpeed()
	{
		return bc_Unit_ranger_is_sniping(self);
	}

	uint8_t Ranger::IsSniping()
	{
		return bc_Unit_ranger_is_sniping(self);
	}

	MapLocation Ranger::SnipeTarget()
	{
		auto location = MapLocation(bc_Unit_ranger_target_location(self));
		CHECK_ERRORS();
		return location;
	}

	uint8_t Ranger::IsSnipeReady()
	{
		return bc_GameController_is_begin_snipe_ready(GameController::gc, id);
	}

	uint8_t Ranger::CanBeginSnipe(MapLocation location)
	{
		return bc_GameController_can_begin_snipe(GameController::gc, id, location.self);
	}

	void Ranger::BeginSnipe(MapLocation location)
	{
		bc_GameController_begin_snipe(GameController::gc, id, location.self);
		CHECK_ERRORS();
	}

}

