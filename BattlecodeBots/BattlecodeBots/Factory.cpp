#include "Factory.h"

#include "GameController.h"

namespace units {

	Factory::Factory()
	{
	}

	Factory::~Factory()
	{
	}

	uint8_t Factory::IsProducing()
	{
		return bc_Unit_is_factory_producing(self);
	}

	bc_UnitType Factory::ProductionUnit()
	{
		return bc_Unit_factory_unit_type(self);
	}

	uint32_t Factory::RoundsLeft()
	{
		return bc_Unit_factory_rounds_left(self);
	}

	uint32_t Factory::ProductionSpeed()
	{
		return bc_Unit_factory_max_rounds_left(self);
	}

	uint8_t Factory::CanProduce(bc_UnitType robot)
	{
		return bc_GameController_can_produce_robot(GameController::gc, id, robot);
	}

	void Factory::Produce(bc_UnitType robot)
	{
		bc_GameController_produce_robot(GameController::gc, id, robot);
	}

}

