#include "Knight.h"

#include "GameController.h"
#include "Log.h"

namespace units {

	Knight::Knight()
	{
	}

	Knight::~Knight()
	{
	}

	uint32_t Knight::Defense()
	{
		return bc_Unit_knight_defense(self);
	}

	uint8_t Knight::IsJavelinReady()
	{
		return bc_GameController_is_javelin_ready(GameController::gc, id);
	}

	uint8_t Knight::CanJavelin(std::shared_ptr<Unit> target)
	{
		return bc_GameController_can_javelin(GameController::gc, id, target->id);
	}

	void Knight::Javelin(std::shared_ptr<Unit> target)
	{
		bc_GameController_javelin(GameController::gc, id, target->id);
		CHECK_ERRORS();
	}

}

