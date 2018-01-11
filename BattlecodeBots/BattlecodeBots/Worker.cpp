#include "Worker.h"

#include "GameController.h"
#include "Structure.h"
#include "Log.h"

namespace units {

	Worker::Worker()
	{
	}

	Worker::~Worker()
	{
	}

	uint32_t Worker::ReplicateCost()
	{
		return bc_UnitType_replicate_cost(type);
	}

	uint8_t Worker::HasActed()
	{
		return bc_Unit_worker_has_acted(self);
	}

	uint32_t Worker::BuildSpeed()
	{
		return bc_Unit_worker_build_health(self);
	}

	uint32_t Worker::RepairSpeed()
	{
		return bc_Unit_worker_repair_health(self);
	}

	uint32_t Worker::HarvestAmount()
	{
		return bc_Unit_worker_harvest_amount(self);
	}

	uint8_t Worker::CanHarvest(bc_Direction direction)
	{
		return bc_GameController_can_harvest(GameController::gc, id, direction);
	}

	uint8_t Worker::CanBlueprint(bc_UnitType structure, bc_Direction direction)
	{
		return bc_GameController_can_blueprint(GameController::gc, id, structure, direction);
	}

	void Worker::Blueprint(bc_UnitType structure, bc_Direction direction)
	{
		bc_GameController_blueprint(GameController::gc, id, structure, direction);
		CHECK_ERRORS();
	}

	void Worker::Build(std::shared_ptr<Structure> blueprint)
	{
		bc_GameController_build(GameController::gc, id, blueprint->id);
		CHECK_ERRORS();
	}

	uint8_t Worker::CanRepair(std::shared_ptr<Structure> structure)
	{
		return bc_GameController_can_repair(GameController::gc, id, structure->id);
	}

	void Worker::Repair(std::shared_ptr<Structure> structure)
	{
		bc_GameController_repair(GameController::gc, id, structure->id);
		CHECK_ERRORS();
	}

	uint8_t Worker::CanReplicate(bc_Direction direction)
	{
		return bc_GameController_can_replicate(GameController::gc, id, direction);
	}

	void Worker::Replicate(bc_Direction direction)
	{
		bc_GameController_replicate(GameController::gc, id, direction);
		CHECK_ERRORS();
	}

}
