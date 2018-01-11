#pragma once

#include "Robot.h"

namespace units {

	class Healer : public Robot
	{
	public:
		Healer();
		~Healer();
		/*
		@return The amount of health passively restored to itself each round.
		*/
		uint32_t SelfHeal();
		/*
		@return Whether the healer is ready to heal. Tests whether the healer's attack heat is sufficiently low.
		*/
		uint8_t IsHealReady();
		/*
		@return Whether the healer can heal the given robot, without taking into account the healer's attack heat. Takes into account only the healer's attack range, and the location of the robot.
		*/
		uint8_t CanHeal(std::shared_ptr<Robot> target);
		/*
		@execute Commands the healer to heal the target robot.
		*/
		void Heal(std::shared_ptr<Robot> target);
		/*
		@return Whether the healer is ready to overcharge. Tests whether the healer's ability heat is sufficiently low.
		*/
		uint8_t IsOverchargeReady();
		/*
		@return Whether the healer can overcharge the given robot, without taking into account the healer's ability heat. Takes into account only the healer's ability range, and the location of the robot.
		*/
		uint8_t CanOvercharge(std::shared_ptr<Robot> target);
		/*
		@execute Overcharges the robot, resetting the robot's cooldowns. The robot must be on the same team as you.
		*/
		void Overcharge(std::shared_ptr<Robot> target);
	private:
		void Attack(units::Unit& target) override;
	};

}

