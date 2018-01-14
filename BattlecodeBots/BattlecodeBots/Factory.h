#ifndef FACTORY_H 
#define FACTORY_H

#include "Structure.h"

namespace units {

	class Factory : public Structure
	{
	public:
		Factory(bc_Unit* unit);
		~Factory();
		/*
		@return Whether the factory is currently producing a unit.
		*/
		uint8_t IsProducing();
		/*
		@return The unit type currently being produced by the factory.
		*/
		bc_UnitType ProductionUnit();
		/*
		@return The number of rounds left to produce a robot in this factory.
		*/
		uint32_t RoundsLeft();
		/*
		@return The maximum number of rounds left to produce a robot in this factory.
		*/
		uint32_t ProductionSpeed();
		/*
		@return Whether the factory can produce a robot of the given type. The factory must not currently be producing a robot, and the team must have sufficient resources in its resource pool.
		*/
		uint8_t CanProduce(bc_UnitType robot);
		/*
		@execute Starts producing the robot of the given type.
		*/
		void Produce(bc_UnitType robot);

	};

}


#endif