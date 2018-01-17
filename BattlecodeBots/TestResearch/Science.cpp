#include "Science.h"

#include <iostream>
#include <algorithm>
#include <math.h>

#include "GameController.h"
#include "Constants.h"
#include "MapUtil.h"
#include "PlayerData.h"

void Science::Update()
{
	auto researchInfoPtr = bc_GameController_research_info(GameController::gc);

	if (researchNextTurn) { // Determine what upgrade to research
		// Upgrades are removed from paths once they are researched
		paths.erase(std::remove_if(std::begin(paths), std::end(paths), 
			[researchInfoPtr, this](Upgrade& upgrade) {
			auto val = bc_ResearchInfo_get_level(researchInfoPtr, upgrade.branch);

			if (max_level(upgrade.branch) == val) {
				if (upgrade.branch == bc_UnitType::Knight) {
					this->hasJavelin = true;
				}
				if (upgrade.branch == bc_UnitType::Mage) {
					this->hasBlink = true;
				}
				if (upgrade.branch == bc_UnitType::Ranger) {
					this->hasSnipe = true;
				}
				if (upgrade.branch == bc_UnitType::Healer) {
					this->hasOverCharge = true;
				}
			} 
			// Determine if Ultimate

			return val >= upgrade.level;
		}), std::end(paths));

		for (auto& upgrade : paths) {
			upgrade.evaluationScore = upgrade.Evaluate(&upgrade);
		}

		// Upgrade calls it's Evaluate method, storing it's Evaluation
		// Then it goes into previous upgrades and adds those to its score
		// IT's evaluation is devided by how long it would take to reseach that and previous upgrades
		// Changes???
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
		CHECK_ERRORS();
	} else {
		uint32_t roundsLeft = bc_ResearchInfo_rounds_left(researchInfoPtr);
		if (roundsLeft == 1) {
			researchNextTurn = true;
		}

		std::cout << "Rounds left for Research " << roundsLeft << std::endl;
		CHECK_ERRORS();
	}

	delete_bc_ResearchInfo(researchInfoPtr);
}

void Science::Init(PlayerData* playerData)
{
	this->playerData = playerData;

	/*
	25
	Workers may harvest an additional 1 Karbonite from a deposit at a time.
	*/
	/// 725 - 0, Linear through rounds
	paths.push_back({ "Gimme some of that Black Stuff", Worker, 1, [playerData](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 750
		auto currRound = GameController::Round();

		float score = .0f;
		if (currRound > 1000 - constants::WorkerUpgrade1) {
			score = .0f;
		} else {
			int ourShare = playerData->earthStartingKarbonite / 2.0f;
			int predictedTurns = ourShare / (3 * playerData->teamUnitCounts[bc_UnitType::Worker]);
			int possibleTurns = ourShare / (4 * playerData->teamUnitCounts[bc_UnitType::Worker]);
			score = (possibleTurns / predictedTurns) * ourShare;
		} 

		std::cout << "Gimme some of that Black Stuff has a value of " << score << std::endl;
		return score;
	} });

	/*
	75
	Workers add 1 more health when repairing or constructing a building.
	*/
	/// 675 - 0, Linear through rounds
	paths.push_back({ "Time is of the Essence", Worker, 2, [](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 750
		auto currRound = GameController::Round();

		float score = .0f;
		if (currRound > 1000 - constants::WorkerUpgrade2) {
			score = .0f;
		} else {
			score = 1.0f;
		} 

		std::cout << "Time is of the Essence I has a value of " << score << std::endl;
		return score;
	} });

	/*
	75
	Workers add another 1 more health (2 more total) when repairing or constructing a building.
	*/
	/// 675 - 0, Linear through rounds
	paths.push_back({ "Time is of the Essence II", Worker, 3, [](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 750
		auto currRound = GameController::Round();

		float score = .0f;
		if (currRound > 1000 - constants::WorkerUpgrade3) {
			score = .0f;
		} else {
			score = 1.0f;
		} 

		std::cout << "Time is of the Essence II has a value of " << score << std::endl;
		return score;
	} });

	/*
	75
	Workers add another 3 more health (5 more total) when repairing or constructing a building.
	*/
	/// 675 - 0, Linear through rounds
	paths.push_back({ "Time is of the Essence III", Worker, 4, [](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 750
		auto currRound = GameController::Round();

		float score = .0f;
		if (currRound > 1000 - constants::WorkerUpgrade4) {
			score = .0f;
		} else {
			score = 5.0f;
		} 

		std::cout << "Time is of the Essence III has a value of " << score << std::endl;
		return score;
	} });

	/*
	25
	Decreases the strength of an attack on a Knight by an additional 5HP.
	*/
	/// 975 - 0, Linear through rounds
	paths.push_back({ "Armor", Knight, 1, [](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GameController::Round();

		float score = .0f;
		if (currRound > 1000 - constants::KnightUpgrade1) {
			score = .0f;
		} else {
			score = 30.0f;
		} 

		std::cout << "Armor has a value of " << score << std::endl;
		return score;
	 } });

	/*
	75
	Decreases the strength of an attack on a Knight by another 5HP (10HP more total).
	*/
	/// 30 - 0, Straight
	paths.push_back({ "Even More Armor", Knight, 2, [](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GameController::Round();

		float score = .0f;
		if (currRound > 1000 - constants::KnightUpgrade2) {
			score = .0f;
		} else {
			score = 30.0f;
		} 

		std::cout << "Even More Armor has a value of " << score << std::endl;
		return score;
	} });

	/*
	150
	Unlocks “Javelin” for Knights.
	*/
	/// 400 - 0, Slope through rounds
	paths.push_back({ "Javelin", Knight, 3, [](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GameController::Round();

		float score = .0f;
		if (currRound > 1000 - constants::KnightUpgrade3) {
			score = .0f;
		} else {
			score = -.0000007 * pow(currRound, 3) - pow(currRound, 2) + 400;
		} 
		// 60 damage, every 10 rounds. +3~ range

		std::cout << "Javelin has a value of " << score << std::endl;
		return score;
	} });

	/*
	25
	Decreases a Ranger’s movement cooldown by 5.
	*/
	/// 60 - 0, Straight
	paths.push_back({ "Get in Fast", Ranger, 1, [](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GameController::Round();

		float score = .0f;
		if (currRound > 1000 - constants::RangerUpgrade1) {
			score = .0f;
		} else {
			score = 60.0f;
		} 

		std::cout << "Get in Fast has a value of " << score << std::endl;
		return score;
	} });

	/*
	100
	Increases a Ranger’s vision range by 30.
	*/
	/// 100 - 0, Straight
	paths.push_back({ "Scopes", Ranger, 2, [](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GameController::Round();

		float score = .0f;
		if (currRound > 1000 - constants::RangerUpgrade2) {
			score = .0f;
		} else {
			score = 100.0f;
		} 

		std::cout << "Scopes has a value of " << score << std::endl;
		return score;
	} });

	/*
	200
	Unlocks "Snipe" for Rangers.
	*/
	/// 0 - 400 - 0, Straight, Slope through rounds
	paths.push_back({ "Snipe", Ranger, 3, [](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GameController::Round();

		float score = .0f;
		if (currRound > 1000 - constants::RangerUpgrade3) {
			score = .0f;
		} else if(currRound < 400) {
			score = 300.0f;
		} else {
			score = pow(.99f, currRound - 1000);
		}

		std::cout << "Snipe has a value of " << score << std::endl;
		return score;
	} });

	/*
	25
	Increases standard attack damage by 15HP.
	*/
	/// 30 - 0, Straight through rounds
	paths.push_back({ "Glass Cannon", Mage, 1, [](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GameController::Round();

		float score = .0f;
		if (currRound > 1000 - constants::MageUpgrade1) {
			score = .0f;
		} else {
			score = 30.0f;
		}

		std::cout << "Glass Cannon I has a value of " << score << std::endl;
		return score;
	} });

	/*
	75
	Increases standard attack damage by another 15HP (30HP more total).
	*/
	/// 30 - 0, Straight through rounds
	paths.push_back({ "Glass Cannon II", Mage, 2, [](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GameController::Round();

		float score = .0f;
		if (currRound > 1000 - constants::MageUpgrade2) {
			score = .0f;
		} else {
			score = 30.0f;
		}

		std::cout << "Glass Cannon II has a value of " << score << std::endl;
		return score;
	} });

	/*
	100
	Glass Cannon III: Increases standard attack damage by another 15HP (45HP more total).
	*/
	/// 30 - 0, Straight through rounds
	paths.push_back({ "Glass Cannon III", Mage, 3, [](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GameController::Round();

		float score = .0f;
		if (currRound > 1000 - constants::MageUpgrade3) {
			score = .0f;
		} else {
			score = 30.0f;
		}

		std::cout << "Glass Cannon III has a value of " << score << std::endl;
		return score;
	} });

	/*
	200
	Unlocks “Blink” for Mages.
	*/
	/// 0 - 500 - 0, Parabola through rounds
	paths.push_back({ "Blink", Mage, 4, [](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GameController::Round();

		float score = .0f;
		if (currRound > 1000 - constants::MageUpgrade4) {
			score = .0f;
		} else {
			score = -.07 * pow(currRound - 600, 2) + 500;
		}
	
		std::cout << "Blink has a value of " << score << std::endl;
		return score;
	} });

	/*
	25
	Increases Healer’s healing ability by 2HP.
	*/
	/// 4 - 0, Straight
	paths.push_back({ "Spirit Water", Healer, 1, [playerData](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GameController::Round();

		float score = .0f;
		if (currRound > 1000 - constants::HealerUpgrade1) {
			score = .0f;
		} else {
			score = 4.0f;
		}

		std::cout << "Spirit Water I has a value of " << score << std::endl;
		return score;
	} });

	/*
	100
	Increases Healer’s healing ability by an another 5HP (7HP more total).
	*/
	/// 10 - 0, Straight
	paths.push_back({ "Spirit Water II", Healer, 2, [](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GameController::Round();

		float score = .0f;
		if (currRound > 1000 - constants::HealerUpgrade2) {
			score = .0f;
		} else {
			score = 10.0f;
		}

		std::cout << "Spirit Water II has a value of " << score << std::endl;
		return score;
	} });

	/*
	200
	Unlocks “Overcharge” for Healers.
	*/
	/// 0 - 600 - 0, Parabola through Rounds / Straight
	paths.push_back({ "Overcharge", Healer, 3, [this](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GameController::Round();

		// The Healer Ultimate is most effective at mid - end game after we have gotten an existing ultimate

		float score = .0f;
		if (currRound > 1000 - constants::HealerUpgrade3) {
			score = .0f;
		} else if(this->hasJavelin || this->hasSnipe || this->hasBlink){
			score = -.007 * pow(currRound - 500, 2) + 400;
		} else {
			score = 100.0f;
		}

		std::cout << "Overcharge has a value of " << score << std::endl;
		return score;
	} });

	/*
	100
	Unlocks rocket technology. Workers can now blueprint and build rockets.
	*/
	/// 0 - 6000, Exponential through rounds
	paths.push_back({ "Rocketry", Rocket, 1, [playerData](Upgrade* upgrade) {
		int currRound = GameController::Round();

		// Must have by 600 Garanteed!!!
		// 0 - 100, not very effective unless karbonite
		// 300 - 500 is optimal

		// Exponential model with rounds. Normal
		// Plus bonus depending on initial karbonite on earth and ratio

		float multiplier = currRound;
		

		// Average of 15 rounds per strike
		// Average of 60 karb
		// Each round 60 Karb / 15 Rounds = 4 Karb per round
		// Width * Height  = Total
		// CurrPassbleLocation Amount on Mars = CurrLocAmo
		// CurrLocAmo / Total = Avaible Locations Ratio
		// Avaliable Location Ratio * 4 Karb Per Round = Total average of Karb Gain Per round

		// Check if Avaible Researchs on Earth before round
		if (currRound < 200) {
			int KarbPerRoundOnMars = 4;

			float totalMarsLocation = MapUtil::marsLocations.size();
			float passableMarsLocation = MapUtil::marsPassableLocations.size();
			float passableMarsRatio = passableMarsLocation / totalMarsLocation;

			float averageKarbPerRoundOnMars = passableMarsRatio * KarbPerRoundOnMars;
			// float averageKarbPerRoundOnMars = AsteroidPattern::GetTotalKarbOnMarsByRound(currRound+100);

			// 1000 is a hardcoded minimum to be competent on Earth at start
			float EarthToMarsRatio = 1000 / playerData->earthStartingKarbonite;
			if (EarthToMarsRatio > multiplier) {
				multiplier = EarthToMarsRatio;
			}
		}
		float score = pow(1.014, multiplier);

		std::cout << "Rocketry has a value of " << score << std::endl;
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
		auto currRound = GameController::Round();

		float score = 1.0f * (750 - currRound - constants::RocketUpgrade2);

		std::cout << "Rocket Boosters has a value of " << score << std::endl;
		return score;
	} });

	/*
	100
	Allows rockets to garrison 4 more units per rocket.
	*/
	/// 650 - 0, Linear through rounds
	paths.push_back({ "Increased Capacity", Rocket, 3, [](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 750
		auto currRound = GameController::Round();

		float score = 1.0f * (750 - currRound - constants::RocketUpgrade3);

		std::cout << "Increased Capacity has a value of " << score << std::endl;
		return score;
	} });
	CHECK_ERRORS();
}

uint32_t Upgrade::TurnsToResearch()
{
	auto researchInfoPtr = bc_GameController_research_info(GameController::gc);

	// Our StartLevel is our Current Level, aka how what we have researched. WE start at 0 and go up by 1.
	uintptr_t startLevel = bc_ResearchInfo_get_level(researchInfoPtr, branch);

	// Therefore we don't want to get our current level, we want to get the next level.
	uint32_t turnsToResearch = 0;
	for (uintptr_t i = startLevel+1; i <= level; i++) {
		auto value = cost_of(branch, i);
		turnsToResearch += value;
	}
	
	delete_bc_ResearchInfo(researchInfoPtr);
	return turnsToResearch;
}

void Upgrade::Research()
{
	auto researchInfoPtr = bc_GameController_research_info(GameController::gc);

	bc_GameController_reset_research(GameController::gc);
	if (bc_GameController_queue_research(GameController::gc, branch)) {
		std::cout << "Researching " << name << ". It will take " << cost_of(branch, level) << " turns." << std::endl;
	} else {
		std::cout << "Failed to research " << name << std::endl;
	}

	delete_bc_ResearchInfo(researchInfoPtr);
	CHECK_ERRORS();
}
// Flight Time, by karb ratio to Earth and Mars

// Change Science so that Each upgrad takes into account it's research time.


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
