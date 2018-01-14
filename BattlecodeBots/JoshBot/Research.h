#ifndef RESEARCH_H
#define RESEARCH_H

#include "bc.h"

class Research
{
public:
	bc_ResearchInfo* self;
public:
	Research();
	~Research();
	/*
	@return Resets the research queue to be empty. Returns true if the queue was not empty before, and false otherwise.
	*/
	uint8_t Reset();
	/*
	Adds a branch to the back of the queue, if it is a valid upgrade, and starts research if it is the first in the queue.
	@return Whether the branch was successfully added.
	*/
	uint8_t Queue(bc_UnitType branch);
	/*
	@return Returns the number of rounds left until the upgrade at the front of the research queue is applied.
	*/
	uint32_t RoundsLeft();
	/*
	@return The max level of a unit branch
	*/
	static uintptr_t MaxLevel(bc_UnitType branch);
	/*
	@return The rounds required to research a level of a unit branch
	*/
	static uint32_t TimeToResearch(bc_UnitType branch, uintptr_t level);
};

#endif // !RESEARCH_H

