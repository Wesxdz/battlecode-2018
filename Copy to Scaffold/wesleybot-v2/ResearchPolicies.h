#ifndef RESEARCH_POLICIES_H
#define RESEARCH_POLICIES_H

#include <map>
#include <list>
#include <memory>
#include "bc.h"
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

class ResearchPolicies
{
public:
	void Update();
	std::list<Upgrade> paths;
	void Init();
private:
	bool researchNextTurn = true;
};

#endif
