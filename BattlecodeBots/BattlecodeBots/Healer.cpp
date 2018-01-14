#include "Healer.h"

#include "GameController.h"
#include "Log.h"

namespace units {

	Healer::Healer(bc_Unit* unit) :
		Robot(unit)
	{
	}

	Healer::~Healer()
	{
	}

	uint32_t Healer::SelfHeal()
	{
		return bc_Unit_healer_self_heal_amount(self);
	}

	uint8_t Healer::IsHealReady()
	{
		return bc_GameController_is_heal_ready(GameController::gc, id);
	}

	uint8_t Healer::CanHeal(Robot& target)
	{
		return bc_GameController_can_heal(GameController::gc, id, target.id);
	}

	void Healer::Heal(Robot& target)
	{
		bc_GameController_heal(GameController::gc, id, target.id);
		CHECK_ERRORS();
	}

	uint8_t Healer::IsOverchargeReady()
	{
		return bc_GameController_is_overcharge_ready(GameController::gc, id);
	}

	uint8_t Healer::CanOvercharge(bc_Unit* target)
	{
		return bc_GameController_can_overcharge(GameController::gc, id, bc_Unit_id(target));
	}

	void Healer::Overcharge(Robot& target)
	{
		bc_GameController_overcharge(GameController::gc, id, target.id);
		CHECK_ERRORS();
	}

	void Healer::Attack(units::Unit& target)
	{
		// Healer can't attack! D:
	}

}

