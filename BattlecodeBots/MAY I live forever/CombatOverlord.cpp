#include "CombatOverlord.h"

#include "Utility.h"
#include "GameController.h"
#include <algorithm>
#include "Worker.h"
#include "Constants.h"
#include "Pathfind.h"

#include "MapUtil.h"
#include <iostream>
#include <math.h>
#include "Section.h"

std::vector<uint16_t> CombatOverlord::requestHeal;
std::vector<MapLocation> CombatOverlord::controlPoints;
InfluenceMap CombatOverlord::fear;
InfluenceMap CombatOverlord::courage;
InfluenceMap CombatOverlord::damage;
std::map<bc_UnitType, float> CombatOverlord::multipliers = {
	{ Factory, 9.0f },{ Healer, 15.0f },{ Worker, 1.0f },{ Knight, 5.0f },{ Mage, 20.0f },{ Ranger, 10.0f },{ Rocket, 20.0f }
};
std::map<bc_UnitType, float> CombatOverlord::fearTolerance = {
	{Healer, -40.0f}, {Knight, 0.0f}, {Mage, -50.0f}, {Ranger, -30.0f}
};
std::map<uint16_t, HealthInstance> CombatOverlord::healthAmounts;

CombatOverlord::CombatOverlord()
{
	for (MapLocation location : PlayerData::pd->enemySpawnPositions) {
		if (Section::Get(location)->status == StartStatus::Mixed) {
			controlPoints.push_back(location);
		}
	}
	std::cout << "Control Points " << controlPoints.size() << std::endl;
	if (GameController::Planet() == Earth) {
		fear.Init(GameController::earth);
		courage.Init(GameController::earth);
		damage.Init(GameController::earth);
	}
	else {
		fear.Init(GameController::mars);
		courage.Init(GameController::mars);
		damage.Init(GameController::mars);
	}
}

CombatOverlord::~CombatOverlord()
{
}

void CombatOverlord::Update()
{
	if (GameController::Round() % 5) {
		auto end = std::remove_if(controlPoints.begin(), controlPoints.end(), [](MapLocation& point) {
			if (point.IsVisible()) {
				auto enemies = point.NearbyUnits(100, Utility::GetOtherTeam(GameController::Team()));
				auto team = point.NearbyUnits(100, GameController::Team());
				if (enemies.size() == 0 && team.size() > 5) {
					return true;
				}
			}
			return false;
		});
		if (end != controlPoints.end()) {
			controlPoints.erase(end);
		}
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
	if (GameController::Round() % 3) {
		CalculateInfluenceMaps();
	}
	//if (GameController::Round() % 10 == 0) {
	//}
}

void CombatOverlord::LateUpdate()
{
	// Need late update otherwise HealthInstance locations are incorrect if the unit moves
	auto team = GameController::Units(MyTeam);
	for (units::Unit& unit : team) {
		auto lastRoundHealth = healthAmounts.find(unit.id);
		if (lastRoundHealth != healthAmounts.end()) {
			uint32_t currentHealth = unit.Health();
			uint32_t lastHealth = (*lastRoundHealth).second.health;
			if (currentHealth < lastHealth) { // This unit has taken damage D:
				uint32_t damageTaken = lastHealth - currentHealth;
				//std::cout << "Unit took damage" << std::endl;
				damage.SetInfluence(lastRoundHealth->second.location, damageTaken/10.0f, 5);
			}
			healthAmounts.erase((*lastRoundHealth).first);
		}
	}

	std::vector<uint32_t> died;
	for (auto& lastRoundHealth : healthAmounts) { // Any healthAmount not removed at this point is a unit death
		uint32_t deathDamageTaken = lastRoundHealth.second.health; // At least
		//std::cout << "Unit died" << std::endl;
		damage.SetInfluence(lastRoundHealth.second.location, deathDamageTaken, 5);
		died.push_back(lastRoundHealth.first);
	}
	for (uint32_t deadUnit : died) {
		healthAmounts.erase(deadUnit);
	}

	for (units::Unit& unit : team) {
		if (!unit.Loc().IsOnMap()) continue;
		healthAmounts[unit.id] = HealthInstance{ unit.Health(), unit.Loc().ToMapLocation() };
	}

	for (int i = 0; i < damage.width * damage.height; i++) {
		damage.influence[i] *= 0.5f; // Damage slowly fades over time
	}

	//if (GameController::Round() % 10 == 0 && GameController::Planet() == Earth) {
	//	damage.Print();
	//}
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
			score += worker.ReplicateCost() / 2.0f * multipliers[enemy.type];
		}
		else {
			score += enemy.Cost() / 2.0f * multipliers[enemy.type];
		}
	}
	float percentDamage = (float)enemy.MaxHealth() / actualDamage;

	score += enemy.Cost() * percentDamage * CombatOverlord::multipliers[enemy.type];
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
	float score = 0.0f;
	if (target.IsActiveUnlocked() && (target.type == Mage || target.type == Knight)) {
		float activePercentage = (float)target.AbilityHeat()/ target.AbilityCooldown();
		score += (activePercentage - 1) * target.AbilityCooldown();
	}
	if (target.IsAttackReady()) {
		score += 1.0f;
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

void CombatOverlord::CalculateInfluenceMaps()
{
	fear.Reset();
	courage.Reset();
	auto units = GameController::Units(Visible);
	for (units::Unit& unit : units) {
		MapLocation location = unit.Loc().ToMapLocation();
		if (unit.Team() == GameController::Team()) {
			switch (unit.type) {
			case Mage:
				courage.SetInfluence(location, 5, 3, [](float distance) { return 1.0f; });
				break;
			case Knight:
				courage.SetInfluence(location, 10, 2, [](float distance) { return 1.0f; });
				break;
			case Healer:
				courage.SetInfluence(location, 5, 5, [](float distance) { return 1.0f; });
				break;
			case Ranger:
				courage.SetInfluence(location, 15, 7, [](float distance) { return 1.0f; });
				break;
			case Factory:
				courage.SetInfluence(location, 30, 3, [](float distance) { return 1.0f; });
			default:
				break;
			}
		}
		else {
			switch (unit.type) {
			case Mage:
				fear.SetInfluence(location, 15, 7, [](float distance) { return 1.0f; });
				break;
			case Knight:
				fear.SetInfluence(location, 10, 6, [](float distance) { return 1.0f; });
				break;
			case Ranger:
				fear.SetInfluence(location, 5, 10, [](float distance) { return 1.0f; });
				fear.SetInfluence(location, -10, 3, [](float distance) { return 1.0f; });
				break;
			default:
				break;
			}
		}
	}
	fear.FindMax();
	//if (GameController::Planet() == Earth) {
	//	fear.Print();
	//	courage.Print();
	//}
}
