#include "Structure.h"

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
