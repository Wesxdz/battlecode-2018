#include "Structure.h"

#include "Robot.h"
#include "GameController.h"
#include "Log.h"

namespace units {

	Structure::Structure()
	{
	}

	Structure::~Structure()
	{
	}

	uint32_t Structure::Cost()
	{
		return bc_UnitType_blueprint_cost(type);
	}

	uint8_t Structure::IsBuilt()
	{
		return bc_Unit_structure_is_built(self);
	}

	uintptr_t Structure::MaxCapacity()
	{
		return bc_Unit_structure_max_capacity(self);
	}

	std::vector<std::shared_ptr<Robot>> Structure::Garrison()
	{
		auto garrison = std::vector<std::shared_ptr<units::Robot>>();
		bc_VecUnitID* inside = bc_Unit_structure_garrison(self);
		for (uintptr_t i = 0; i < bc_VecUnitID_len(inside); i++) {
			// Use static_pointer_cast to make types more specific
			garrison.push_back(std::static_pointer_cast<units::Robot>(GameController::Unit((GameController::gc, bc_VecUnitID_index(inside, i)))));
		}
		delete_bc_VecUnitID(inside);
		return garrison;
	}

	uint8_t Structure::CanLoad(std::shared_ptr<Robot> robot)
	{
		return bc_GameController_can_load(GameController::gc, id, robot->id);
	}

	void Structure::Load(std::shared_ptr<Robot> robot)
	{
		bc_GameController_load(GameController::gc, id, robot->id);
		CHECK_ERRORS();
	}

	uint8_t Structure::CanUnload(bc_Direction direction)
	{
		return bc_GameController_can_unload(GameController::gc, id, direction);
	}

	void Structure::Unload(bc_Direction direction)
	{
		bc_GameController_unload(GameController::gc, id, direction);
		CHECK_ERRORS();
	}

}

