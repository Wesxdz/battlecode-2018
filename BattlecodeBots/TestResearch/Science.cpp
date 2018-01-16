#include "Science.h"

#include <iostream>
#include <algorithm>

#include "GameController.h"
#include "Research.h"
#include "GlobalData.h"
#include "Constants.h"

void Science::Update()
{
	if (researchNextTurn) { // Determine what upgrade to research
		// Upgrades are removed from paths once they are researched
		paths.erase(std::remove_if(std::begin(paths), std::end(paths), [](Upgrade& upgrade) {
			return Research::GetLevel(upgrade.branch) >= upgrade.level;
		}), std::end(paths));
		for (auto& upgrade : paths) {
			upgrade.evaluationScore = upgrade.Evaluate(&upgrade);
		}
		for (int i = 1; i < 4; i++) {
			for (auto& upgrade : paths) {
				if (upgrade.level == i) {
					// Add previous level evaluation scores
					for (auto& lowerUpgrade : paths) {
						if (lowerUpgrade.level == i - 1 && lowerUpgrade.branch == upgrade.branch) {
							upgrade.evaluationScore += lowerUpgrade.evaluationScore; // Too many indentations? Not enough.
						}
					}
				}
			}
		}
		auto max = std::max_element(std::begin(paths), std::end(paths), [](Upgrade& a, Upgrade& b) {
			return a.evaluationScore/a.TurnsToResearch() < b.evaluationScore/b.TurnsToResearch();
		});
		(*max).Research();
		researchNextTurn = false;
	}

	uint32_t roundsLeft = Research::RoundsLeft();
	if (roundsLeft == 1) {
		researchNextTurn = true;
	}
}

void Science::Init()
{
	/*
	25
	Workers may harvest an additional 1 Karbonite from a deposit at a time.
	*/
	/// 725 - 0, Linear through rounds
	paths.push_back({ "Gimme some of that Black Stuff", Worker, 1, [](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 750
		auto currRound = GlobalData::data->currRound;

		float score = 1.0f * (750 - currRound - constants::WorkerUpgrade1);
		return score;
	} });

	/*
	75
	Workers add 1 more health when repairing or constructing a building.
	*/
	/// 675 - 0, Linear through rounds
	paths.push_back({ "Time is of the Essence", Worker, 2, [](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 750
		auto currRound = GlobalData::data->currRound;

		float score = 0.1f * (750 - currRound - constants::WorkerUpgrade2);
		return score;
	} });

	/*
	75
	Workers add another 1 more health (2 more total) when repairing or constructing a building.
	*/
	/// 675 - 0, Linear through rounds
	paths.push_back({ "Time is of the Essence II", Worker, 3, [](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 750
		auto currRound = GlobalData::data->currRound;

		float score = 1.0f * (750 - currRound - constants::WorkerUpgrade3);
		return score;
	} });

	/*
	75
	Workers add another 3 more health (5 more total) when repairing or constructing a building.
	*/
	/// 675 - 0, Linear through rounds
	paths.push_back({ "Time is of the Essence III", Worker, 4, [](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 750
		auto currRound = GlobalData::data->currRound;

		float score = 1.0f * (750 - currRound - constants::WorkerUpgrade4);
		return score;
	} });

	/*
	25
	Decreases the strength of an attack on a Knight by an additional 5HP.
	*/
	/// 975 - 0, Linear through rounds
	paths.push_back({ "Armor", Knight, 1, [](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GlobalData::data->currRound;

		float score = 1.0f * (1000 - currRound - constants::KnightUpgrade1);
		return score;
	 } });

	/*
	75
	Decreases the strength of an attack on a Knight by another 5HP (10HP more total).
	*/
	/// 925 - 0, Linear through rounds
	paths.push_back({ "Even More Armor", Knight, 2, [](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GlobalData::data->currRound;

		float score = 1.0f * (1000 - currRound - constants::KnightUpgrade2);
		return score;
	} });

	/*
	150
	Unlocks “Javelin” for Knights.
	*/
	/// 850 - 0, Linear through rounds
	paths.push_back({ "Javelin", Knight, 3, [](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GlobalData::data->currRound;

		float score = 1.0f * (1000 - currRound - constants::KnightUpgrade3);
		return score;
	} });

	/*
	25
	Decreases a Ranger’s movement cooldown by 5.
	*/
	/// 975 - 0, Linear through rounds
	paths.push_back({ "Get in Fast", Ranger, 1, [](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GlobalData::data->currRound;

		float score = 1.0f * (1000 - currRound - constants::RangerUpgrade1);
		return score;
	} });

	/*
	100
	Increases a Ranger’s vision range by 30.
	*/
	/// 900 - 0, Linear through rounds
	paths.push_back({ "Scopes", Ranger, 2, [](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GlobalData::data->currRound;

		float score = 1.0f * (1000 - currRound - constants::RangerUpgrade2);
		return score;
	} });

	/*
	200
	Unlocks "Snipe" for Rangers.
	*/
	/// 800 - 0, Linear through rounds
	paths.push_back({ "Snipe", Ranger, 3, [](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GlobalData::data->currRound;

		float score = 1.0f * (1000 - currRound - constants::RangerUpgrade3);
		return score;
	} });

	/*
	25
	Increases standard attack damage by 15HP.
	*/
	/// 975 - 0, Linear through rounds
	paths.push_back({ "Glass Cannon", Mage, 1, [](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GlobalData::data->currRound;

		float score = 1.0f * (1000 - currRound - constants::MageUpgrade1);
		return score;
	} });

	/*
	75
	Increases standard attack damage by another 15HP (30HP more total).
	*/
	/// 925 - 0, Linear through rounds
	paths.push_back({ "Glass Cannon II", Mage, 2, [](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GlobalData::data->currRound;

		float score = 1.0f * (1000 - currRound - constants::MageUpgrade2);
		return score;
	} });

	/*
	100
	Glass Cannon III: Increases standard attack damage by another 15HP (45HP more total).
	*/
	/// 900 - 0, Linear through rounds
	paths.push_back({ "Glass Cannon III", Mage, 3, [](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GlobalData::data->currRound;

		float score = 1.0f * (1000 - currRound - constants::MageUpgrade3);
		return score;
	} });

	/*
	200
	Unlocks “Blink” for Mages.
	*/
	/// 800 - 0, Linear through rounds
	paths.push_back({ "Blink", Mage, 4, [](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GlobalData::data->currRound;

		float score = 1.0f * (1000 - currRound - constants::MageUpgrade4);
		return score;
	} });

	/*
	25
	Increases Healer’s healing ability by 2HP.
	*/
	/// 975 - 0, Linear through rounds
	paths.push_back({ "Spirit Water", Healer, 1, [](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GlobalData::data->currRound;

		float score = 1.0f * (1000 - currRound - constants::HealerUpgrade1);
		return score;
	} });

	/*
	100
	Increases Healer’s healing ability by an another 5HP (7HP more total).
	*/
	/// 900 - 0, Linear through rounds
	paths.push_back({ "Spirit Water II", Healer, 2, [](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GlobalData::data->currRound;

		float score = 1.0f * (1000 - currRound - constants::HealerUpgrade2);
		return score;
	} });

	/*
	200
	Unlocks “Overcharge” for Healers.
	*/
	/// 800 - 0, Linear through Rounds
	paths.push_back({ "Overcharge", Healer, 3, [](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GlobalData::data->currRound;

		float score = 1.0f * (1000 - currRound - constants::HealerUpgrade3);
		return score;
	} });

	/*
	100
	Unlocks rocket technology. Workers can now blueprint and build rockets.
	*/
	/// 0 - 6000, Linear through rounds
	paths.push_back({ "Rocketry", Rocket, 1, [](Upgrade* upgrade) {
		int currRound = GlobalData::data->currRound;
		int minRound = (750 - constants::RocketUpgrade1 - 50); // Round 600. We MUST research by now.

		float score = 6000 - (minRound - currRound) * 10;

		return score;
	} });

	/*
	100
	Reduces rocket travel time by 20 rounds compared to the travel time determined by the orbit of the planets.
	*/
	/// 650 - 0, Linear through rounds.
	paths.push_back({ "Rocket Boosters", Rocket, 2, [](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 750
		// Make it relevant with 
		auto currRound = GlobalData::data->currRound;

		float score = 1.0f * (750 - currRound - constants::RocketUpgrade2);
		return score;
	} });

	/*
	100
	Allows rockets to garrison 4 more units per rocket.
	*/
	/// 650 - 0, Linear through rounds
	paths.push_back({ "Increased Capacity", Rocket, 3, [](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 750
		auto currRound = GlobalData::data->currRound;

		float score = 1.0f * (750 - currRound - constants::RocketUpgrade3);
		return score;
	} });

}

uint32_t Upgrade::TurnsToResearch()
{
	uintptr_t startLevel = Research::GetLevel(branch);
	uint32_t turnsToResearch = 0;
	for (uintptr_t i = startLevel; i <= level; i++) {
		turnsToResearch += Research::TimeToResearch(branch, i);
	}
	
	return turnsToResearch;
}

void Upgrade::Research()
{
	std::cout << "Researching " << name << "\n";
	Research::Reset();
	Research::Queue(branch);
}

// I dont think we should take into account Further Research... Or we should modify how it works...
// Research should be heavily determined by current and future predictions. For example, many research takes 100+ turns. The game can change drastically in those turns.
// If we upgrade based on the current then we have already lost. In fact, if we can research an ultimate, that should almost always take priority.
// Keep track of enemy over time?

// Knight and Mage.
// Research should be based on current and future production quotas.
// Research should be based off current and future enemy production
// Research should be based off enemy research. Going a very specific path is vauable
// Ultimates should almost always be prioritized. *Blink and Overcharge, slight exceptions.
// Keep track of what has and hasn't been researched.

// Because of how expensive it is to switch Paths, I think we should determine a general strategy to use and base our research off that.
// The strategy should be based off of the current enemy production and research.
// Or we could develop out own strategy and simply program our AI to react to counters...
// For example, we research Knight (basic), Ranger (Basic), Mage (Basic) and/ or Rocket (Basic). Then based off the Combat Manager's evaluation of the enemy and it's strategy, research those.

// I dont think we ned a boolean. Instead if we add the Turn Research Amount into Constants we could use that
// Maybe we choose which PATH we want to research, as opposed to what upgrade.
// Actually yes. Because If we research one thing and then check which is more effective. That previous upgrade will have less because the previous upgrade got removed. This does not mean we shouldn't upgrade it though.
// Removal can possibly be more simple / efficient... Keep track of what we are upgrading.

// Any research that can't be completed by the end of game has 0.
// Also Other research needs to take into account the base rocket research being completed by 700.

// Each of the workers upgrades aren't very effective unless in numbers. The last upgrade is a bit effective, but it takes a while to get there.
// Furthermore, they become useless endgame when workers cant do anything.

// Knight Upgrades are quite meaningful especially when knights are plentiful. They lose they're meaning however with knights having limited range.
// Therefore Javelin if available is very valuable

// Ranger first Upgrade is cheap and pretty useful
// Next one allows for enhanced scouting, but isn't particularly strong
// Snipe can be very useful for setting up defense with some scouting measures. The problem however, is wether the ranger can see the enemy unit. It can't attack a unit it can't see.

// On Paper, Mage research is quite good, however comared to its base attack and other units health, it isn't that strong.
// It can and is very effective against buffed up knights because they do more damage, however they are also squishy with limited range
// In general, the mage is good against many enemies.
// Blink seems good as well, however the range isn't very good, and cooldown is worst other than worker

// Healer has a small nice upgrades, however compared to how much damage other units do, they're only effective with Knights and Rangers
// In addition, they're completely pointless without other units and only effective in groups.
// The Overcharge is extremely strong, but only if another unit has an ultimate. Otherwise, pretty useless

// Rocket base is nessecary. At round 600, it is the most important upgrade.
// Next upgrades can be usefull for a rush to Mars Strategy.
