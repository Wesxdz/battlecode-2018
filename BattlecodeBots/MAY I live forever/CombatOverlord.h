#ifndef COMBAT_OVERLORD_H
#define COMBAT_OVERLORD_H

#include "bc.h"

#include "Knight.h"
#include "Ranger.h"
#include "Mage.h"
#include "Healer.h"
#include "PlayerData.h"

class CombatOverlord
{
public:
	CombatOverlord();
	~CombatOverlord();
	void Update();
	void KnightAction(units::Knight knight);
	void RangerAction(units::Ranger ranger);
	void MageAction(units::Mage mage);
	void HealerAction(units::Healer healer);
	/*
	@return The enemy robots that this robot could attack
	*/
	std::vector<units::Unit> EnemiesInRange(units::Robot& robot, uint32_t range);
	float AttackValue(units::Robot& attacker, units::Unit& target);
	float SplashValue(units::Mage& mage, units::Unit& target);
	float HealValue(units::Healer& healer, units::Unit& target);
	float OverchargeValue(units::Healer& healer, units::Unit& target);
	/*
	Doesn't take into account mage splash, knight jav, or possible movement of enemies, only a heuristic
	@return Potential damage that could be taken
	*/
	uint32_t Danger(units::Unit& unit);
	void DetermineDesiredUnits();
	bc_UnitType MostDesiredType();
};

#endif

