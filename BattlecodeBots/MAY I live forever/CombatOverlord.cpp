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

CombatOverlord::CombatOverlord()
{
}

CombatOverlord::~CombatOverlord()
{
}

void CombatOverlord::Update()
{
	DetermineDesiredUnits();
}

//void CombatOverlord::KnightAction(units::Knight knight)
//{
//	if (knight.IsMoveReady()) {
//		bool hasMoved = false;
//		for (int i = 1; i <= 100; i = (i + 1) * (i + 1)) {
//			auto nearbyEnemies = knight.Loc().ToMapLocation().NearbyUnits(i, Utility::GetOtherTeam(knight.Team()));
//			if (nearbyEnemies.size() > 0) {
//				MapLocation enemyLocation = nearbyEnemies[0].Loc().ToMapLocation();
//				Pathfind::MoveGreedy(knight, enemyLocation);
//				hasMoved = true;
//				break;
//			}
//		}
//		if (!hasMoved) {
//			Pathfind::MoveRandom(knight); // TODO Group movement
//		}
//	}
//	if (knight.IsAttackReady()) { // Attack before Javelin in case you kill adjacent units
//		auto nearby = EnemiesInRange(knight, knight.AttackRange());
//		auto best = std::max_element(nearby.begin(), nearby.end(), [&knight, this](units::Unit& a, units::Unit& b) {
//			return this->AttackValue(knight, a) < this->AttackValue(knight, b);
//		});
//		if (best != nearby.end()) {
//			if (knight.CanAttack(*best)) {
//				knight.Attack(*best);
//			}
//		}
//	}
//	if (knight.IsActiveUnlocked() && knight.IsJavelinReady()) {
//		auto nearby = EnemiesInRange(knight, knight.AbilityRange());
//		auto best = std::max_element(nearby.begin(), nearby.end(), [&knight, this](units::Unit& a, units::Unit& b) {
//			return this->AttackValue(knight, a) < this->AttackValue(knight, b);
//		});
//		if (best != nearby.end()) {
//			if (knight.CanJavelin(*best)) {
//				knight.Javelin(*best);
//			}
//		}
//	}
//}

//void CombatOverlord::RangerAction(units::Ranger ranger)
//{
//	if (!ranger.IsSniping()) {
//		if (ranger.IsAttackReady()) { // Attack before Javelin in case you kill adjacent units
//			auto nearby = EnemiesInRange(ranger,ranger.AttackRange());
//			nearby.erase(std::find_if(nearby.begin(), nearby.end(), [&ranger](units::Unit& unit) {
//				auto unitLocation = unit.Loc().ToMapLocation();
//				return ranger.Loc().ToMapLocation().DistanceTo(unitLocation) < ranger.AttackRangeMin();
//			}));
//			auto best = std::max_element(nearby.begin(), nearby.end(), [&ranger, this](units::Unit& a, units::Unit& b) {
//				return this->AttackValue(ranger, a) < this->AttackValue(ranger, b);
//			});
//			if (best != nearby.end()) {
//				if (ranger.CanAttack(*best)) {
//					ranger.Attack(*best);
//				}
//			}
//		}
//		// TODO Call down the thunder
//	}
//}
//
//void CombatOverlord::MageAction(units::Mage mage)
//{
//	if (mage.IsAttackReady()) { // Attack before Javelin in case you kill adjacent units
//		auto nearby = EnemiesInRange(mage, mage.AttackRange());
//		auto best = std::max_element(nearby.begin(), nearby.end(), [&mage, this](units::Unit& a, units::Unit& b) {
//			return this->SplashValue(mage, a) < this->SplashValue(mage, b);
//		});
//		if (best != nearby.end()) {
//			if (mage.CanAttack(*best)) {
//				mage.Attack(*best);
//			}
//		}
//	}
//}
//
//void CombatOverlord::HealerAction(units::Healer healer)
//{
//	if (healer.IsHealReady()) {
//		auto nearby = healer.Loc().ToMapLocation().NearbyUnits(healer.AttackRange(), healer.Team());
//		auto best = std::max_element(nearby.begin(), nearby.end(), [&healer, this](units::Unit& a, units::Unit& b) {
//			return this->AttackValue(healer, a) < this->AttackValue(healer, b);
//		});
//		if (best != nearby.end()) {
//			units::Robot toHeal{ bc_Unit_clone((*best).self) };
//			healer.Heal(toHeal);
//		}
//	}
//	if (healer.IsActiveUnlocked() && healer.IsOverchargeReady()) {
//		auto nearby = healer.Loc().ToMapLocation().NearbyUnits(healer.AbilityRange(), healer.Team());
//		auto best = std::max_element(nearby.begin(), nearby.end(), [&healer, this](units::Unit& a, units::Unit& b) {
//			return this->AttackValue(healer, a) < this->AttackValue(healer, b);
//		});
//		if (best != nearby.end()) {
//			units::Robot overcharging{ bc_Unit_clone((*best).self) };
//			healer.Overcharge(overcharging);
//			if (overcharging.type == Mage) {
//				MageAction(units::Mage(bc_Unit_clone(overcharging.self)));
//			}
//			else if (overcharging.type == Knight) {
//				KnightAction(units::Knight(bc_Unit_clone(overcharging.self)));
//			}
//			else if (overcharging.type == Ranger) {
//				RangerAction(units::Ranger(bc_Unit_clone(overcharging.self)));
//			}
//		}
//	}
//}

std::vector<units::Unit> CombatOverlord::EnemiesInRange(units::Robot& robot, uint32_t range)
{
	return robot.Loc().ToMapLocation().NearbyUnits(range, Utility::GetOtherTeam(robot.Team()));
}

float CombatOverlord::AttackValue(units::Robot& attacker, units::Unit& enemy)
{
	return 1.0f;
	//float score = 0.0f;
	//uint32_t actualDamage = attacker.Damage();
	//if (enemy.type == Knight) {
	//	units::Knight knight = units::Knight(bc_Unit_clone(enemy.self));
	//	actualDamage -= knight.Defense();
	//}
	//if (actualDamage > enemy.Health()) {
	//	if (enemy.type == Worker) {
	//		units::Worker worker = units::Worker(bc_Unit_clone(enemy.self));
	//		score += worker.ReplicateCost() / 2.0f * AttackCoordinator::multipliers[enemy.type];
	//	}
	//	else {
	//		score += enemy.Cost() / 2.0f * AttackCoordinator::multipliers[enemy.type];
	//	}
	//	uint32_t overkill = actualDamage - enemy.Health();
	//	score -= overkill / 5.0f;
	//}
	//float percentDamage = (float)enemy.MaxHealth() / actualDamage;

	//score += enemy.Cost() * percentDamage * AttackCoordinator::multipliers[enemy.type];
	//// TODO: If the attacking unit is in danger of death prioritize attacking fighter units
	//return score;
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

float CombatOverlord::HealValue(units::Healer& healer, units::Unit& target)
{
	float score = 0.0f;
	float danger = Danger(target.Loc().ToMapLocation(), Utility::GetOtherTeam(target.Team()));
	float deathChance = (float)danger/target.Health();
	float deathChanceWithHeal = (float)danger/target.Health();
	if (deathChanceWithHeal < 1 && deathChance > 1) {
		score += 10.0f;
	}
	float percentHealth = (float)target.Health() / target.MaxHealth();
	return score;
}

float CombatOverlord::OverchargeValue(units::Healer & healer, units::Unit & target)
{
	return 0.0f;
}

float CombatOverlord::Danger(MapLocation location, bc_Team damageSource)
{
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

void CombatOverlord::DetermineDesiredUnits()
{
	//PlayerData::pd->desiredUnitCounts[Knight]++;

	uint32_t currRound = GameController::Round();

	// Cant build
	if (currRound > 749 || GameController::Planet() == bc_Planet::Mars) {
		PlayerData::pd->unitPriority[bc_UnitType::Worker] = 1.0f; // Maybe change Worker Priority?
		PlayerData::pd->unitPriority[bc_UnitType::Rocket] = .0f;
		PlayerData::pd->unitPriority[bc_UnitType::Factory] = .0f;
		return;
	}

	// Need to take into account Current, Future, and Enemy

	// Knights are countered by Mage and Rangers...
	// Mages are countered by Rangers
	// 

	int workerAmo = PlayerData::pd->teamUnitCounts[bc_UnitType::Worker];
	int knightAmo = PlayerData::pd->teamUnitCounts[bc_UnitType::Knight];
	int mageAmo = PlayerData::pd->teamUnitCounts[bc_UnitType::Mage];
	int rangerAmo = PlayerData::pd->teamUnitCounts[bc_UnitType::Ranger];
	int healerAmo = PlayerData::pd->teamUnitCounts[bc_UnitType::Healer];
	int factoryAmo = PlayerData::pd->teamUnitCounts[bc_UnitType::Factory];
	int rocketAmo = PlayerData::pd->teamUnitCounts[bc_UnitType::Rocket];
	int totalAmo = workerAmo + knightAmo + mageAmo + rangerAmo + healerAmo + factoryAmo + rocketAmo;

	int workerEnemyAmo = PlayerData::pd->enemyUnitCounts[bc_UnitType::Worker];
	int knightEnemyAmo = PlayerData::pd->enemyUnitCounts[bc_UnitType::Knight];
	int mageEnemyAmo = PlayerData::pd->enemyUnitCounts[bc_UnitType::Mage];
	int rangerEnemyAmo = PlayerData::pd->enemyUnitCounts[bc_UnitType::Ranger];
	int healerEnemyAmo = PlayerData::pd->enemyUnitCounts[bc_UnitType::Healer];
	int factoryEnemyAmo = PlayerData::pd->enemyUnitCounts[bc_UnitType::Factory];
	int rocketEnemyAmo = PlayerData::pd->enemyUnitCounts[bc_UnitType::Rocket];
	int totalEnemyAmo = workerEnemyAmo + knightEnemyAmo + mageEnemyAmo + rangerEnemyAmo + healerEnemyAmo + factoryEnemyAmo + rocketEnemyAmo;

	int workerProductionAmo = PlayerData::pd->inProductionCounts[bc_UnitType::Worker];
	int knightProductionAmo = PlayerData::pd->inProductionCounts[bc_UnitType::Knight];
	int mageProductionAmo = PlayerData::pd->inProductionCounts[bc_UnitType::Mage];
	int rangerProductionAmo = PlayerData::pd->inProductionCounts[bc_UnitType::Ranger];
	int healerProductionAmo = PlayerData::pd->inProductionCounts[bc_UnitType::Healer];
	int factoryProductionAmo = PlayerData::pd->inProductionCounts[bc_UnitType::Factory];
	int rocketProductionAmo = PlayerData::pd->inProductionCounts[bc_UnitType::Rocket];
	int totalProductionAmo = workerProductionAmo + knightProductionAmo + mageProductionAmo + rangerProductionAmo + healerProductionAmo + factoryProductionAmo + rocketProductionAmo;
	/////////////// BUILDER

	// < 0 = Too many
	// 0 = Don't need
	// 0 - 1 = Could use
	//  > 1 = Need

	// Knight Priority
	{
		float knightPriority = .0f;

		// Knights will always be somewhat valuable
		// They do good damage, limited range, high HP, good Defense
		// If we lack a "Defense", aka Knights, then we should get a minimum in for sure.
		// After that, we should only build more to recuperate our defense, rush, or strategise
		uintptr_t width = bc_PlanetMap_width_get(GameController::PlanetMap(bc_Planet::Earth));
		if(width < 1) { width = 0; }
		float knightToMap = static_cast<float>(knightAmo) / width;

		if (knightAmo < width) {
			float knightAmof = static_cast<float>(knightAmo);
			if(knightAmof < 1) { knightAmof = .01f; }
			knightPriority = .5f - (width / knightAmof);
			if(knightPriority < .5f){knightPriority = .5f; }
			if(knightPriority > width / 10.0f) { knightPriority = width / 10.0f; }
		} else {
			knightPriority = .3f;
		}

		PlayerData::pd->unitPriority[bc_UnitType::Knight] = 0.5f;
		//PlayerData::pd->unitPriority[bc_UnitType::Knight] = knightPriority;
	}

	// Ranger Priority
	{
		float rangerPriority = .0f;

		// Rangers can never go wrong
		// Good health, long range, good damage

		// Unless another unit has a specific reason to be built, we should always build Ranger's
		rangerPriority = .7f;
		

		PlayerData::pd->unitPriority[bc_UnitType::Ranger] = rangerPriority;
	}

	// Mage Priority
	{
		float magePriority = .0f;

		float mapSize = MapUtil::earthPassableLocations.size() / 2.0f;
		if(mapSize < 1) { mapSize = .0001f; }
		float enemyToMap = totalEnemyAmo / mapSize;

		// If enemy is grouped up, then we should use mages to deal a lot of damage;
		magePriority = enemyToMap;

		PlayerData::pd->unitPriority[bc_UnitType::Mage] = magePriority;
	}

	// Healer Priority
	{
		float healerPriority = .0f;

		float totalEnemyAmof = static_cast<float>(totalEnemyAmo);
		if(totalEnemyAmof < 1.0f) { totalEnemyAmof = .001f; }
		//float teamToEnemy = totalAmo / totalEnemyAmo;

		// If we are winning, snowball but keeping our units alive with healers...?
		//healerPriority = (knightAmo + rangerAmo) / 5.0f;

		PlayerData::pd->unitPriority[bc_UnitType::Healer] = 0.1f;
		//PlayerData::pd->unitPriority[bc_UnitType::Healer] = healerPriority;
	}
}
