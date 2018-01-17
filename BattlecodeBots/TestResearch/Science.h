#ifndef RESEARCH_POLICIES_H
#define RESEARCH_POLICIES_H

#include "bc.h"

#include <map>
#include <list>
#include <memory>
#include <functional>
#include <string>

struct Upgrade
{
	std::string name;
	bc_UnitType branch;
	uintptr_t level;
	std::function<float(Upgrade*)> Evaluate;

	float evaluationScore;
	uint32_t TurnsToResearch();
	void Research();
};

class Science
{
public:
	virtual void Init();
	void Update();
	std::list<Upgrade> paths;
//private:
	bool researchNextTurn = true;
};

#endif
