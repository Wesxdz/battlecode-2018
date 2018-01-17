#ifndef RESEARCH_POLICIES_H
#define RESEARCH_POLICIES_H

#include <map>
#include <list>
#include <memory>
#include "bc.h"
#include <functional>
#include <string>

#include "PlayerData.h"

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
	virtual void Init(PlayerData* playerData);
	void Update();
	std::list<Upgrade> paths;
private:
	PlayerData* playerData;  

	bool researchNextTurn = true;
	int turnsToResearch = 0;

	bool hasBlink;
	bool hasSnipe;
	bool hasJavelin;
	bool hasOverCharge;
	bool hasRockets;
};

#endif
