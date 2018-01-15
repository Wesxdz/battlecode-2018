#include "AttackCoordinator.h"

#include "MapLocation.h"
#include "Constants.h"
#include "Knight.h"

void AttackCoordinator::Consider(units::Robot& fighter)
{
	if (std::find(std::begin(constants::types_fighters), std::end(constants::types_fighters), fighter.type) == std::end(constants::types_fighters)) return;
	std::vector<units::Unit> targets = fighter.Loc().ToMapLocation().NearbyUnits(fighter.AttackRange());
	if (fighter.type == Mage) {
		for (units::Unit& enemy : targets) {
			std::vector<units::Unit> splash;
			MapLocation enemyLocation = enemy.Loc().ToMapLocation();
			for (bc_Direction direction : constants::directions_adjacent) {
				MapLocation adjacent = MapLocation::Neighbor(enemyLocation, direction);
				if (adjacent.IsValid() && adjacent.IsVisible()) {
					adjacent.Occupant();
					// Occupant
				}
			}
			damageCombinations[enemy.id].push_back(DamageInstance{ fighter.id, fighter.Damage() });
		}
	}
	else {
		for (units::Unit& enemy : targets) {
			damageCombinations[enemy.id].push_back(DamageInstance{ fighter.id, fighter.Damage() });
		}
	}
}

float AttackCoordinator::Value(uint32_t damage, units::Unit& enemy)
{
	float score = 0.0f;
	uint32_t actualDamage = damage;
	if (enemy.type == Knight) {
		units::Knight knight = units::Knight(bc_Unit_clone(enemy.self));
		actualDamage -= knight.Defense();
	}
	if (actualDamage > enemy.Health()) {
		score += 100;
		uint32_t overkill = actualDamage - enemy.Health();
		score -= overkill;
	}
	
}
