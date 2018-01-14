#include "Mage.h"

#include "GameController.h"
#include "Log.h"

namespace units {

	Mage::Mage(bc_Unit* unit) :
		Robot(unit)
	{
	}

	Mage::~Mage()
	{
	}

	uint8_t Mage::IsBlinkReady()
	{
		return bc_GameController_is_blink_ready(GameController::gc, id);
	}

	uint8_t Mage::CanBlink(MapLocation& location)
	{
		return bc_GameController_can_blink(GameController::gc, id, location.self);
	}

	void Mage::Blink(MapLocation& location)
	{
		bc_GameController_blink(GameController::gc, id, location.self);
		CHECK_ERRORS();
	}

}

