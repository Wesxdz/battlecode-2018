#include "CombatOverlord.h"

#include "Utility.h"
#include "GameController.h"
#include <algorithm>
#include "Worker.h"
#include "AttackCoordinator.h"
#include "Constants.h"
#include "Pathfind.h"

#include "MapUtil.h"
#include <iostream>

std::vector<uint16_t> CombatOverlord::requestHeal;
std::vector<MapLocation> CombatOverlord::controlPoints;

CombatOverlord::CombatOverlord()
{
	for (MapLocation location : PlayerData::pd->enemySpawnPositions) {
		controlPoints.push_back(location);
	}
}

CombatOverlord::~CombatOverlord()
{
}

void CombatOverlord::Update()
{
	auto end = std::remove_if(controlPoints.begin(), controlPoints.end(), [](MapLocation& point) {
		if (point.IsVisible()) {
			auto enemies = point.NearbyUnits(25, Utility::GetOtherTeam(GameController::Team()));
			if (enemies.size() == 0) {
				return true;
			}
		}
		return false;
	});
	if (end != controlPoints.end()) {
		controlPoints.erase(end);
	}
	requestHeal.clear();
	auto team = GameController::Units(MyTeam);
	for (units::Unit& unit : team) {
		if (Utility::IsRobot(unit.type)) {
			if (unit.Health() < unit.MaxHealth()) {
				requestHeal.push_back(unit.id);
			}
		}
	}
}

std::vector<units::Unit> CombatOverlord::EnemiesInRange(units::Robot& robot, uint32_t range)
{
	return robot.Loc().ToMapLocation().NearbyUnits(range, Utility::GetOtherTeam(robot.Team()));
}

float CombatOverlord::AttackValue(units::Robot& attacker, units::Unit& enemy)
{
	float score = 1.0f;
	uint32_t actualDamage = attacker.Damage();
	if (enemy.type == Knight) {
		units::Knight knight = units::Knight(bc_Unit_clone(enemy.self));
		actualDamage -= knight.Defense();
	}
	if (actualDamage > enemy.Health()) {
		if (enemy.type == Worker) {
			units::Worker worker = units::Worker(bc_Unit_clone(enemy.self));
			score += worker.ReplicateCost() / 2.0f * AttackCoordinator::multipliers[enemy.type];
		}
		else {
			score += enemy.Cost() / 2.0f * AttackCoordinator::multipliers[enemy.type];
		}
	}
	float percentDamage = (float)enemy.MaxHealth() / actualDamage;

	score += enemy.Cost() * percentDamage * AttackCoordinator::multipliers[enemy.type];
	// TODO: If the attacking unit is in danger of death prioritize attacking fighter units
	return score;
}

float CombatOverlord::SplashValue(units::Mage & mage, units::Unit & target)
{
	float score = AttackValue(mage, target);
	for (bc_Direction splashDir : constants::directions_adjacent) {
		MapLocation targetLocation = target.Loc().ToMapLocation();
		MapLocation splashLand = MapLocation::Neighbor(targetLocation, splashDir);
		if (splashLand.IsOccupied()) {
			units::Unit hit(splashLand.Occupant());
			if (hit.Team() == mage.Team()) {
				score -= AttackValue(mage, target) * 2;
			}
			else {
				score += AttackValue(mage, target);
			}
		}
	}
	return score;
}

float CombatOverlord::HealValue(units::Healer& healer, units::Robot target)
{
	float score = 0.0f;
	float danger = Danger(target.Loc().ToMapLocation(), Utility::GetOtherTeam(target.Team()));
	float deathChance = (float)danger/target.Health();
	float deathChanceWithHeal = (float)danger/target.Health();
	if (deathChanceWithHeal < 1 && deathChance > 1) {
		score += 0.5f;
	}
	float percentHealth = (float)target.Health() / target.MaxHealth();
	score += 1 - percentHealth;
	return score;
}

float CombatOverlord::OverchargeValue(units::Healer & healer, units::Robot target)
{
	float score = 0.1f;
	if (target.IsActiveUnlocked()) {
		float activePercentage = (float)target.AbilityHeat()/ target.AbilityCooldown();
		score += (activePercentage - 1) * target.AbilityCooldown();
	}
	return score;
}

float CombatOverlord::Danger(MapLocation location, bc_Team damageSource)
{
	if (!location.IsValid()) return 1000000; // Non existent tiles are certainly dangerous
	float potentialDamage = 0;
	auto dmgSources = location.NearbyUnits(64, damageSource); // Max attack range, ranger
	for (auto& source : dmgSources) {
		if (source.type == Knight || source.type == Mage || source.type == Ranger) {
			units::Robot attacker{ bc_Unit_clone(source.self) };
			uint32_t attackRange = attacker.AttackRange();
			bool couldMove = attacker.MovementHeat() <= 10;
			uint32_t distance = attacker.Loc().ToMapLocation().DistanceTo(location);
			potentialDamage += 64 - distance;
			if (couldMove) attackRange += 10; // Heuristic :P
			if (attackRange < distance) {
				if (attacker.AttackHeat() <= 10) {
					potentialDamage += attacker.Damage();
				}
				else {
					potentialDamage += attacker.Damage() / 2;
				}
			}
		}
	}
	return potentialDamage;
}
