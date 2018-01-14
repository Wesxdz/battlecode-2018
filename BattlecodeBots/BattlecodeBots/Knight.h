#ifndef KNIGHT_H 
#define KNIGHT_H

#include "Robot.h"

namespace units {
	
	class Knight : public Robot
	{
	public:
		Knight(bc_Unit* unit);
		~Knight();
		/*
		@return The amount of damage resisted by a knight when attacked.
		*/
		uint32_t Defense();
		/*
		@return Whether the knight is ready to javelin. Tests whether the knight's ability heat is sufficiently low.
		*/
		uint8_t IsJavelinReady();
		/*
		@return Whether the knight can javelin the given robot, without taking into account the knight's ability heat. Takes into account only the knight's ability range, and the location of the robot.
		*/
		uint8_t CanJavelin(Unit& target);
		/*
		@execute Javelins the robot, dealing the knight's standard damage.
		*/
		void Javelin(Unit& target);
	};

}

#endif