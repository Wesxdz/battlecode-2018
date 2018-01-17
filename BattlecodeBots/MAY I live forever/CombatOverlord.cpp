#include "CombatOverlord.h"

#include "Utility.h"
#include "GameController.h"
#include <algorithm>
#include "Worker.h"
#include "AttackCoordinator.h"
#include "Constants.h"
#include "Pathfind.h"

CombatOverlord::CombatOverlord()
{
}

CombatOverlord::~CombatOverlord()
{
}

void CombatOverlord::Update()
{
	auto units = GameController::Units(MyTeam);
	for (auto& unit : units) {
		if (unit.Loc().IsOnMap()) {
			if (unit.type == Mage) {
				MageAction(units::Mage(bc_Unit_clone(unit.self)));
			}
			else if (unit.type == Knight) {
				KnightAction(units::Knight(bc_Unit_clone(unit.self)));
			}
			else if (unit.type == Ranger) {
				RangerAction(units::Ranger(bc_Unit_clone(unit.self)));
			}
			else if (unit.type == Healer) {
				HealerAction(units::Healer(bc_Unit_clone(unit.self)));
			}
		}
	}
	DetermineDesiredUnits();
}

void CombatOverlord::KnightAction(units::Knight knight)
{
	if (knight.IsMoveReady()) {
		bool hasMoved = false;
		for (int i = 1; i <= 100; i = (i + 1) * (i + 1)) {
			auto nearbyEnemies = knight.Loc().ToMapLocation().NearbyUnits(i, Utility::GetOtherTeam(knight.Team()));
			if (nearbyEnemies.size() > 0) {
				MapLocation enemyLocation = nearbyEnemies[0].Loc().ToMapLocation();
				Pathfind::MoveGreedy(knight, enemyLocation);
				hasMoved = true;
				break;
			}
		}
		if (!hasMoved) {
			Pathfind::MoveRandom(knight); // TODO Group movement
		}
	}
	if (knight.IsAttackReady()) { // Attack before Javelin in case you kill adjacent units
		auto nearby = EnemiesInRange(knight, knight.AttackRange());
		auto best = std::max_element(nearby.begin(), nearby.end(), [&knight, this](units::Unit& a, units::Unit& b) {
			return this->AttackValue(knight, a) < this->AttackValue(knight, b);
		});
		if (best != nearby.end()) {
			if (knight.CanAttack(*best)) {
				knight.Attack(*best);
			}
		}
	}
	if (knight.IsActiveUnlocked() && knight.IsJavelinReady()) {
		auto nearby = EnemiesInRange(knight, knight.AbilityRange());
		auto best = std::max_element(nearby.begin(), nearby.end(), [&knight, this](units::Unit& a, units::Unit& b) {
			return this->AttackValue(knight, a) < this->AttackValue(knight, b);
		});
		if (best != nearby.end()) {
			if (knight.CanJavelin(*best)) {
				knight.Javelin(*best);
			}
		}
	}
}

void CombatOverlord::RangerAction(units::Ranger ranger)
{
	if (!ranger.IsSniping()) {
		if (ranger.IsAttackReady()) { // Attack before Javelin in case you kill adjacent units
			auto nearby = EnemiesInRange(ranger,ranger.AttackRange());
			nearby.erase(std::find_if(nearby.begin(), nearby.end(), [&ranger](units::Unit& unit) {
				auto unitLocation = unit.Loc().ToMapLocation();
				return ranger.Loc().ToMapLocation().DistanceTo(unitLocation) < ranger.AttackRangeMin();
			}));
			auto best = std::max_element(nearby.begin(), nearby.end(), [&ranger, this](units::Unit& a, units::Unit& b) {
				return this->AttackValue(ranger, a) < this->AttackValue(ranger, b);
			});
			if (best != nearby.end()) {
				if (ranger.CanAttack(*best)) {
					ranger.Attack(*best);
				}
			}
		}
		// TODO Call down the thunder
	}
}

void CombatOverlord::MageAction(units::Mage mage)
{
	if (mage.IsAttackReady()) { // Attack before Javelin in case you kill adjacent units
		auto nearby = EnemiesInRange(mage, mage.AttackRange());
		auto best = std::max_element(nearby.begin(), nearby.end(), [&mage, this](units::Unit& a, units::Unit& b) {
			return this->AttackValue(mage, a) < this->AttackValue(mage, b);
		});
		if (best != nearby.end()) {
			if (mage.CanAttack(*best)) {
				mage.Attack(*best);
			}
		}
	}
}

void CombatOverlord::HealerAction(units::Healer healer)
{
	if (healer.IsHealReady()) {
		auto nearby = healer.Loc().ToMapLocation().NearbyUnits(healer.AttackRange(), healer.Team());
		auto best = std::max_element(nearby.begin(), nearby.end(), [&healer, this](units::Unit& a, units::Unit& b) {
			return this->AttackValue(healer, a) < this->AttackValue(healer, b);
		});
		if (best != nearby.end()) {
			units::Robot toHeal{ bc_Unit_clone((*best).self) };
			healer.Heal(toHeal);
		}
	}
	if (healer.IsActiveUnlocked() && healer.IsOverchargeReady()) {
		auto nearby = healer.Loc().ToMapLocation().NearbyUnits(healer.AbilityRange(), healer.Team());
		auto best = std::max_element(nearby.begin(), nearby.end(), [&healer, this](units::Unit& a, units::Unit& b) {
			return this->AttackValue(healer, a) < this->AttackValue(healer, b);
		});
		if (best != nearby.end()) {
			units::Robot overcharging{ bc_Unit_clone((*best).self) };
			healer.Overcharge(overcharging);
			if (overcharging.type == Mage) {
				MageAction(units::Mage(bc_Unit_clone(overcharging.self)));
			}
			else if (overcharging.type == Knight) {
				KnightAction(units::Knight(bc_Unit_clone(overcharging.self)));
			}
			else if (overcharging.type == Ranger) {
				RangerAction(units::Ranger(bc_Unit_clone(overcharging.self)));
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
	float score = 0.0f;
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
		uint32_t overkill = actualDamage - enemy.Health();
		score -= overkill / 5.0f;
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

float CombatOverlord::HealValue(units::Healer& healer, units::Unit& target)
{
	float score = 0.0f;
	uint32_t danger = Danger(target);
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

uint32_t CombatOverlord::Danger(units::Unit & unit)
{
	uint32_t potentialDamage = 0;
	MapLocation unitLocation = unit.Loc().ToMapLocation();
	auto dmgSources = unitLocation.NearbyUnits(50, Utility::GetOtherTeam(unit.Team())); // Max attack range, ranger
	for (auto& source : dmgSources) {
		if (source.type == Knight || source.type == Mage || source.type == Ranger) {
			units::Robot attacker{ bc_Unit_clone(source.self) };
			if (attacker.AttackRange() < attacker.Loc().ToMapLocation().DistanceTo(unitLocation)) {
				potentialDamage += attacker.Damage();
			}
		}
	}
	return potentialDamage;
}

void CombatOverlord::DetermineDesiredUnits()
{
	PlayerData::pd->desiredUnitCounts[Knight] = PlayerData::pd->teamUnitCounts[Knight];
	PlayerData::pd->desiredUnitCounts[Ranger] = PlayerData::pd->teamUnitCounts[Ranger];
	PlayerData::pd->desiredUnitCounts[Healer] = PlayerData::pd->teamUnitCounts[Healer];
	PlayerData::pd->desiredUnitCounts[Mage] = PlayerData::pd->teamUnitCounts[Mage];
	PlayerData::pd->desiredUnitCounts[MostDesiredType()]++;
}

bc_UnitType CombatOverlord::MostDesiredType()
{
	return Knight;
}
