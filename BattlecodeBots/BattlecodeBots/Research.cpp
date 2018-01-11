#include "Research.h"

#include "GameController.h"

Research::Research()
{
	bc_GameController_research_info(GameController::gc);
}

Research::~Research()
{
	delete_bc_ResearchInfo(self);
}

uint8_t Research::Reset()
{
	return bc_GameController_reset_research(GameController::gc);
}

uint8_t Research::Queue(bc_UnitType branch)
{
	return bc_GameController_queue_research(GameController::gc);
}

uint32_t Research::RoundsLeft()
{
	return bc_ResearchInfo_rounds_left(self);
}

uintptr_t Research::MaxLevel(bc_UnitType branch)
{
	return max_level(branch);
}

uint32_t Research::TimeToResearch(bc_UnitType branch, uintptr_t level)
{
	return cost_of(branch, level);
}
