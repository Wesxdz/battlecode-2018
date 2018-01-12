#ifndef WORKER_H 
#define WORKER_H

#include "Robot.h"

namespace units {

	class Structure;

	class Worker : public Robot
	{
	public:
		Worker();
		~Worker();
		uint32_t ReplicateCost();
		/*
		@return Whether the worker has already acted (harveted, blueprinted, built, or repaired) this round.
		*/
		uint8_t HasActed();
		/*
		@return The health restored when building a structure.
		*/
		uint32_t BuildSpeed();
		/*
		@return The health restored when repairing a structure.
		*/
		uint32_t RepairSpeed();
		/*
		@return The maximum amount of karbonite harvested from a deposit in one turn.
		*/
		uint32_t HarvestAmount();
		/*
		@return Whether the worker is ready to harvest, and the given direction contains karbonite to harvest. The worker cannot already have performed an action this round.
		*/
		uint8_t CanHarvest(bc_Direction direction);
		/*
		@execute Harvests up to the worker's harvest amount of karbonite from the given location, adding it to the team's resource pool.
		*/
		void Harvest(bc_Direction direction);
		/*
		@return Whether the worker can blueprint a unit of the given type. The worker can only blueprint factories, and rockets if Rocketry has been researched. The team must have sufficient karbonite in its resource pool. The worker cannot already have performed an action this round.
		*/
		uint8_t CanBlueprint(bc_UnitType structure, bc_Direction direction);
		/*
		@exectue Blueprints a unit of the given type in the given direction. Subtract cost of that unit from the team's resource pool.
		*/
		void Blueprint(bc_UnitType structure, bc_Direction direction);
		/*
		If raised to maximum health, the blueprint becomes a completed structure.
		@execute Builds a given blueprint, increasing its health by the worker's build amount. 
		*/
		void Build(std::shared_ptr<Structure> blueprint);
		/*
		@return Whether the given worker can repair the given strucutre. Tests that the worker is able to execute a worker action, that the structure is built, and that the structure is within range.
		*/
		uint8_t CanRepair(std::shared_ptr<Structure> structure);
		/*
		@execute Commands the worker to repair a structure, repleneshing health to it. This can only be done to structures which have been fully built.
		*/
		void Repair(std::shared_ptr<Structure> structure);
		/*
		@return Whether the worker is ready to replicate. Tests that the worker's ability heat is sufficiently low, that the team has sufficient karbonite in its resource pool, and that the square in the given direction is empty.
		*/
		uint8_t CanReplicate(bc_Direction direction);
		/*
		@execute Replicates a worker in the given direction. Subtracts the cost of the worker from the team's resource pool.
		*/
		void Replicate(bc_Direction direction);
	};

}

#endif