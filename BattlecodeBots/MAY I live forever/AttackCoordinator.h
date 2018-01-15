#ifndef ATTACK_COORDINATOR_H
#define ATTACK_COORDINATOR_H

#include "bc.h"
#include "Unit.h"
#include "Robot.h"
#include <map>

struct DamageInstance {
	uint16_t sourceId;
	uint32_t damage;
	bool isSplash = false;
};

class AttackCoordinator
{
public:
	static std::map<bc_UnitType, float> multipliers;
	static std::map<uint16_t, std::vector<DamageInstance>> damageCombinations;
	/*
	Add damageCombinations of particular robot
	*/
	static void Consider(units::Robot& fighter);
	/*
	@return The value of dealing #damage to #enemy
	*/
	static float Value(uint32_t damage, units::Unit& enemy);
};

#endif

