#include "Robot.h"

#include "GameController.h"
#include "Log.h"

namespace units {

	Robot::Robot()
	{
	}

	Robot::~Robot()
	{
	}

	uint32_t Robot::Cost()
	{
		return bc_UnitType_factory_cost(type);
	}

	uint32_t Robot::Damage()
	{
		return bc_Unit_damage(self);
	}

	uint32_t Robot::AttackRange()
	{
		return bc_Unit_attack_range(self);
	}

	uint32_t Robot::MovementHeat()
	{
		return bc_Unit_movement_heat(self);
	}

	uint32_t Robot::AttackHeat()
	{
		return bc_Unit_attack_heat(self);
	}

	uint32_t Robot::MovementCooldown()
	{
		return bc_Unit_movement_cooldown(self);
	}

	uint32_t Robot::AttackCooldown()
	{
		return bc_Unit_attack_cooldown(self);
	}

	uint8_t Robot::IsActiveUnlocked()
	{
		return bc_Unit_is_ability_unlocked(self);
	}

	uint32_t Robot::AbilityHeat()
	{
		return bc_Unit_ability_cooldown(self);
	}

	uint32_t Robot::AbilityCooldown()
	{
		return bc_Unit_ability_cooldown(self);
	}

	uint32_t Robot::AbilityRange()
	{
		return bc_Unit_ability_range(self);
	}

	uint8_t Robot::IsMoveReady()
	{
		return bc_GameController_is_move_ready(GameController::gc, id);
	}

	uint8_t Robot::CanMove(bc_Direction direction)
	{
		return bc_GameController_can_move(GameController::gc, id, direction);
	}

	void Robot::Move(bc_Direction direction)
	{
		bc_GameController_move_robot(GameController::gc, id, direction);
		CHECK_ERRORS();
	}

	uint8_t Robot::IsAttackReady()
	{
		return bc_GameController_is_attack_ready(GameController::gc, id);
	}

	uint8_t Robot::CanAttack(units::Unit& target)
	{
		return bc_GameController_can_attack(GameController::gc, id, target.id);
	}

	void Robot::Attack(units::Unit& target)
	{
		bc_GameController_attack(GameController::gc, id, target.id);
		CHECK_ERRORS();
	}

}

