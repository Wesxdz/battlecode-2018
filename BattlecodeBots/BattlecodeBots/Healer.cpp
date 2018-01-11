#include "Healer.h"

#include "GameController.h"
#include "Log.h"

namespace units {

	Healer::Healer()
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

	uint8_t Healer::CanHeal(std::shared_ptr<Robot> target)
	{
		return bc_GameController_can_heal(GameController::gc, id, target->id);
	}

	void Healer::Heal(std::shared_ptr<Robot> target)
	{
		bc_GameController_heal(GameController::gc, id, target->id);
		CHECK_ERRORS();
	}

	uint8_t Healer::IsOverchargeReady()
	{
		return bc_GameController_is_overcharge_ready(GameController::gc, id);
	}

	uint8_t Healer::CanOvercharge(std::shared_ptr<Robot> target)
	{
		return bc_GameController_can_overcharge(GameController::gc, id, target->id);
	}

	void Healer::Overcharge(std::shared_ptr<Robot> target)
	{
		bc_GameController_overcharge(GameController::gc, id, target->id);
		CHECK_ERRORS();
	}

	void Healer::Attack(units::Unit& target)
	{
		// Healer can't attack! D:
	}

}

