#include "Worker.h"



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
