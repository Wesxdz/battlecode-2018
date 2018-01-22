#ifndef COMBAT_OVERLORD_H
#define COMBAT_OVERLORD_H

#include "bc.h"

#include <map>

#include "Knight.h"
#include "Ranger.h"
#include "Mage.h"
#include "Healer.h"
#include "PlayerData.h"
#include "InfluenceMap.h"

class CombatOverlord
{
public:
	CombatOverlord();
	~CombatOverlord();
	void Update();
	/*
	@return The enemy robots that this robot could attack
	*/
	static std::vector<units::Unit> EnemiesInRange(units::Robot& robot, uint32_t range);
	static float AttackValue(units::Robot& attacker, units::Unit& target);
	static float SplashValue(units::Mage& mage, units::Unit& target);
	static float HealValue(units::Healer& healer, units::Robot target);
	static float OverchargeValue(units::Healer& healer, units::Robot target);
	/*
	Doesn't take into account mage splash, knight jav, or possible movement of enemies, only a heuristic
	@return Potential damage that could be taken
	*/
	static float Danger(MapLocation location, bc_Team damageSource);

	// The units that require healing, sorted by order of most needed first
	static std::vector<uint16_t> requestHeal;
	// These are points that have concentrated enemies and must be destroyed
	static std::vector<MapLocation> controlPoints;
	static InfluenceMap fear;
	static void CalculateFearMap();
};

#endif

