#ifndef ROBOT_H 
#define ROBOT_H

#include "Unit.h"

namespace units {

	class Robot : public Unit
	{
	public:
		Robot(bc_Unit* unit);
		~Robot();
		virtual uint32_t Cost() override;
		/*
		@return The damage inflicted by the robot during a normal attack.
		*/
		uint32_t Damage();
		/*
		@return The attack range.
		*/
		uint32_t AttackRange();
		/*
		@return The movement heat.
		*/
		uint32_t MovementHeat();
		/*
		@return The attack heat.
		*/
		uint32_t AttackHeat();
		/*
		@return The movement cooldown.
		*/
		uint32_t MovementCooldown();
		/*
		@return The attack cooldown.
		*/
		uint32_t AttackCooldown();
		/*
		@return Whether the active ability is unlocked.
		*/
		uint8_t IsActiveUnlocked();
		/*
		@return The active ability heat.
		*/
		uint32_t AbilityHeat();
		/*
		@return The active ability cooldown.
		*/
		uint32_t AbilityCooldown();
		/*
		@return  The active ability range. This is the range in which: workers can replicate, knights can javelin, rangers can snipe, mages can blink, and healers can overcharge.
		*/
		uint32_t AbilityRange();
		/*
		@return Whether the robot is ready to move. Tests whether the robot's move heat is sufficiently low.
		*/
		uint8_t IsMoveReady();
		/*
		@return Whether the robot can move in the given direction, without taking into account the unit's movement heat. Takes into account only the map terrain, positions of other robots, and the edge of the game map.
		*/
		uint8_t CanMove(bc_Direction direction);
		/*
		Moves the robot in the given direction.
		*/
		void Move(bc_Direction direction);
		/*
		Whether the robot is ready to attack. Tests whether the robot's attack heat is sufficiently low.
		*/
		uint8_t IsAttackReady();
		/*
		Whether the robot can attack the given unit, without taking into account the robot's attack heat. Takes into account only the robot's attack range, and the location of the robot and target.
		*/
		uint8_t CanAttack(Unit& target);
		/*
		Commands a robot to attack a unit, dealing the robot's standard amount of damage.
		*/
		virtual void Attack(Unit& target);
	};

}


#endif