#include "AttackCoordinator.h"

#include <algorithm>
#include "MapLocation.h"
#include "Constants.h"
#include "Knight.h"
#include "Worker.h"
#include "Utility.h"

std::map<units::Unit, std::vector<DamageInstance>> AttackCoordinator::damageCombinations;
std::map<units::Unit, std::vector<DamageInstance>> AttackCoordinator::enemyDamageCombinations;
std::map<bc_UnitType, float> AttackCoordinator::multipliers = {
	{Factory, 1.5f}, {Healer, 1.1f}, {Worker, 1.0f}, {Knight, 10.0f}, {Mage, 1.0f}, {Ranger, 1.0f }, {Rocket, 1.0f}
};

void AttackCoordinator::Consider(units::Robot& fighter)
{
	if (std::find(constants::types_fighters.begin(), constants::types_fighters.end(), fighter.type) == constants::types_fighters.end()) return;
	std::vector<units::Unit> targets = fighter.Loc().ToMapLocation().NearbyUnits(fighter.AttackRange(), Utility::GetOtherTeam(fighter.Team()));
	if (fighter.type == Mage) {
		for (units::Unit& enemy : targets) {
			std::vector<units::Unit> splash;
			MapLocation enemyLocation = enemy.Loc().ToMapLocation();
			for (bc_Direction direction : constants::directions_adjacent) {
				MapLocation adjacent = MapLocation::Neighbor(enemyLocation, direction);
				if (adjacent.IsValid() && adjacent.IsVisible() && adjacent.IsOccupied()) {
					units::Unit hit = adjacent.Occupant();
					damageCombinations[hit].push_back(DamageInstance{ fighter, fighter.Damage(), true });
				}
			}
			damageCombinations[enemy].push_back(DamageInstance{ fighter, fighter.Damage() });
		}
	}
	else {
		for (units::Unit& enemy : targets) {
			damageCombinations[enemy].push_back(DamageInstance{ fighter, fighter.Damage() });
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
		if (enemy.type == Worker) {
			units::Worker worker = units::Worker(bc_Unit_clone(enemy.self));
			score += worker.ReplicateCost() / 2.0f * multipliers[enemy.type];
		}
		else {
			score += enemy.Cost() / 2.0f * multipliers[enemy.type];
		}
		uint32_t overkill = actualDamage - enemy.Health();
		score -= overkill/5.0f;
	}
	float percentDamage = (float)enemy.MaxHealth() / actualDamage;
	score += enemy.Cost() * percentDamage * multipliers[enemy.type];
	// TODO: If the attacking unit is in danger of death prioritize attacking fighter units
	return score;
}
