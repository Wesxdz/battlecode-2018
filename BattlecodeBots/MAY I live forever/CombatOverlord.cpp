#include "CombatOverlord.h"

#include "Utility.h"

#include <algorithm>

CombatOverlord::CombatOverlord()
{
}

CombatOverlord::~CombatOverlord()
{
}

void CombatOverlord::Update(GlobalData* data)
{
	//for (auto mage : data->currMages) { // Mage goes first because of splash damage
	//	MageAction(mage);
	//}
	//for (auto knight : data->currKnights) {
	//	KnightAction(knight);
	//}
	//for (auto ranger : data->currRangers) {
	//	RangerAction(ranger);
	//}
	//for (auto healer : data->currHealers) { // Healer overcharge makes unit act again
	//	HealerAction(healer);
	//}
}

void CombatOverlord::KnightAction(units::Knight & knight)
{
	//if (knight.IsAttackReady()) { // Attack before Javelin in case you kill adjacent units
	//	auto nearby = InAttackRange(knight, knight.AttackRange());
	//	auto best = std::max_element(nearby.begin(), nearby.end(), [&knight, this](units::Unit& unit) {
	//		return this->AttackValue(knight, unit);
	//	});
	//	if (best != nearby.end()) {
	//		if (knight.CanAttack(*best)) {
	//			knight.Attack(*best);
	//		}
	//	}
	//}
	//if (knight.IsJavelinReady()) {
	//	auto nearby = InAttackRange(knight, knight.AbilityRange());
	//	auto best = std::max_element(nearby.begin(), nearby.end(), [&knight, this](units::Unit& unit) {
	//		return this->AttackValue(knight, unit);
	//	});
	//	if (best != nearby.end()) {
	//		if (knight.CanJavelin(*best)) {
	//			knight.Javelin(*best);
	//		}
	//	}
	//}
}

void CombatOverlord::RangerAction(units::Ranger & ranger)
{
	//if (!ranger.IsSniping()) {
	//	if (ranger.IsAttackReady()) { // Attack before Javelin in case you kill adjacent units
	//		auto nearby = InAttackRange(ranger,ranger.AttackRange());
	//		auto underAttack = 
	//		auto best = std::max_element(nearby.begin(), nearby.end(), [&knight, this](units::Unit& unit) {
	//			return this->AttackValue(knight, unit);
	//		});
	//		if (best != nearby.end()) {
	//			if (knight.CanAttack(*best)) {
	//				knight.Attack(*best);
	//			}
	//		}
	//	}
	//	if (knight.IsJavelinReady()) {
	//		auto nearby = InAttackRange(knight, knight.AbilityRange());
	//		auto best = std::max_element(nearby.begin(), nearby.end(), [&knight, this](units::Unit& unit) {
	//			return this->AttackValue(knight, unit);
	//		});
	//		if (best != nearby.end()) {
	//			if (knight.CanJavelin(*best)) {
	//				knight.Javelin(*best);
	//			}
	//		}
	//	}
	//}
}

void CombatOverlord::MageAction(units::Mage & mage)
{
}

void CombatOverlord::HealerAction(units::Healer & healer)
{
}

std::vector<units::Unit> CombatOverlord::InAttackRange(units::Robot& robot, uint32_t range)
{
	return robot.Loc().ToMapLocation().NearbyUnits(range, Utility::GetOtherTeam(robot.Team()));
}

float CombatOverlord::AttackValue(units::Robot & attacker, units::Unit & target)
{
	return 0.0f;
}

float CombatOverlord::HealValue(units::Healer & healer, units::Unit & target)
{
	return 0.0f;
}
