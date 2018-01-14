#ifndef STRUCTURE_H 
#define STRUCTURE_H

#include "Unit.h"

#include <vector>
#include <memory>

namespace units {

class Robot;

/*
To spawn a structure in the game, a Worker must blueprint the structure on an occupiable square adjacent to the Worker. 
This action deducts the cost of the blueprint from the team's resource pool. 
The blueprint starts with 1/4 the maximum health of the structure. 
The Worker then builds the blueprint, increasing the blueprint’s health by a fixed amount each turn until it reaches its maximum health. 
Then the blueprint becomes a structure, and the structure can be used.
Both structures have a garrison, which is a holding space for up to 8 robots. (Research may increase the capacity of the garrison.) 
Robots are loaded into a rocket’s garrison by the rocket, which requires the robots to be ready to move. 
Robots are loaded into a Factory’s garrison automatically after the Factory has produced the robot. 
Rockets and factories both unload a robot from their garrison by placing the robot in an occupiable square adjacent to the structure. 
Loading is treated like a robot’s standard move action, though unloading has no effect or dependency on the movement heat.
Structures cannot be blueprinted or built on Mars.
*/
	class Structure : public Unit 
	{
	public:
		Structure(bc_Unit* unit);
		~Structure();
		virtual uint32_t Cost() override;
		/*
		@return Whether this structure has been built.
		*/
		uint8_t IsBuilt();
		/*
		@return The max capacity of a structure.
		*/
		uintptr_t MaxCapacity();
		/*
		@note The bc function can only get unit ids, which is why you need to pass in #teamRobots
		@return The robots in the structure's garrison.
		*/
		std::vector<Robot> Garrison();
		/*
		@return Whether the robot can be loaded into the given structure's garrison. The robot must be ready to move and must be adjacent to the structure. The structure and the robot must be on the same team, and the structure must have space.
		*/
		uint8_t CanLoad(bc_Unit* robot);
		/*
		@execute Loads the robot into the garrison of the structure.
		*/
		void Load(bc_Unit* robot);
		/*
		@return Whether the given structure is able to unload a unit in the given direction. There must be space in that direction, and the unit must be ready to move.
		*/
		uint8_t CanUnload(bc_Direction direction);
		/*
		@execute Unloads a robot from the garrison of the specified structure into an adjacent space. Robots are unloaded in the order they were loaded.
		*/
		void Unload(bc_Direction direction);
	};

}
#endif