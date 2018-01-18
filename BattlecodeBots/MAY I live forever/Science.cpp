#include "Science.h"

#include <iostream>
#include <algorithm>
#include <math.h>

#include "GameController.h"
#include "OrbitPattern.h"
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

		//std::cout << "Rounds left for Research " << roundsLeft << std::endl;
		CHECK_ERRORS();
	}

	delete_bc_ResearchInfo(researchInfoPtr);
}

void Science::Init(PlayerData* playerData)
{
	this->playerData = playerData;

	// Damage = 2 Points

	/*
	25
	Workers may harvest an additional 1 Karbonite from a deposit at a time.
	*/
	/// ??? - 0, Linear through Ratio and Workers
	paths.push_back({ "Gimme some of that Black Stuff", Worker, 1, [playerData](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 750
		auto currRound = GameController::Round();

		float score = .0f;
		if (currRound > 1000 - constants::WorkerUpgrade1) {
			score = .0f;
		} else {
			float ourShare = playerData->earthStartingKarbonite / 2.0f;
			if(ourShare < 1) { ourShare = 1; }
			float predictedTurns = ourShare / (3 * playerData->teamUnitCounts[bc_UnitType::Worker]);
			float possibleTurns = ourShare / (4 * playerData->teamUnitCounts[bc_UnitType::Worker]);
			score = (possibleTurns / predictedTurns) * (ourShare / 10.0f); // 5000 karbonite = Very worth = 500 Points * Ratio
		} 

		//std::cout << "Gimme some of that Black Stuff has a value of " << score << std::endl;
		return score;
	} });

	/*
	75
	Workers add 1 more health when repairing or constructing a building.
	*/
	/// 1 - 0, Straight
	paths.push_back({ "Time is of the Essence", Worker, 2, [playerData](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 750
		auto currRound = GameController::Round();

		float score = .0f;
		if (currRound > 1000 - constants::WorkerUpgrade2) {
			score = .0f;
		} else {
			float hasUnits = playerData->teamUnitCounts[bc_UnitType::Worker] > 0.0f ? 1.0f : 0.0f;
			float willHaveUnits = playerData->unitPriority[bc_UnitType::Worker] > 0.0f ? 1.0f : 0.0f;
			float tempScore = 1.0f;
			score = tempScore * hasUnits + tempScore * willHaveUnits;
		} 

		//std::cout << "Time is of the Essence I has a value of " << score << std::endl;
		return score;
	} });

	/*
	75
	Workers add another 1 more health (2 more total) when repairing or constructing a building.
	*/
	/// 1 - 0, Straight
	paths.push_back({ "Time is of the Essence II", Worker, 3, [playerData](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 750
		auto currRound = GameController::Round();

		float score = .0f;
		if (currRound > 1000 - constants::WorkerUpgrade3) {
			score = .0f;
		} else {
			float hasUnits = playerData->teamUnitCounts[bc_UnitType::Worker] > 0.0f ? 1.0f : 0.0f;
			float willHaveUnits = playerData->unitPriority[bc_UnitType::Worker] > 0.0f ? 1.0f : 0.0f;
			float tempScore = 1.0f;
			score = tempScore * hasUnits + tempScore * willHaveUnits;
		} 

		//std::cout << "Time is of the Essence II has a value of " << score << std::endl;
		return score;
	} });

	/*
	75
	Workers add another 3 more health (5 more total) when repairing or constructing a building.
	*/
	/// 5 - 0, Straight
	paths.push_back({ "Time is of the Essence III", Worker, 4, [playerData](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 750
		auto currRound = GameController::Round();

		float score = .0f;
		if (currRound > 1000 - constants::WorkerUpgrade3) {
			score = .0f;
		} else {
			float hasUnits = playerData->teamUnitCounts[bc_UnitType::Worker] > 0.0f ? 1.0f : 0.0f;
			float willHaveUnits = playerData->unitPriority[bc_UnitType::Worker] > 0.0f ? 1.0f : 0.0f;
			float tempScore = 5.0f;
			score = tempScore * hasUnits + tempScore * willHaveUnits;
		} 

		//std::cout << "Time is of the Essence III has a value of " << score << std::endl;
		return score;
	} });

	/*
	25
	Decreases the strength of an attack on a Knight by an additional 5HP.
	*/
	/// 30 - 0, Linear through Units
	paths.push_back({ "Armor", Knight, 1, [playerData](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GameController::Round();

		float score = .0f;
		if (currRound > 1000 - constants::KnightUpgrade1) {
			score = .0f;
		} else {
			float hasUnits = playerData->teamUnitCounts[bc_UnitType::Knight] > 0.0f ? 1.0f : 0.0f;
			float willHaveUnits = playerData->unitPriority[bc_UnitType::Knight] > 0.0f ? 1.0f : 0.0f;
			score = 30.0f * hasUnits + 30.0f * willHaveUnits;
		} 

		//std::cout << "Armor has a value of " << score << std::endl;
		return score;
	 } });

	/*
	75
	Decreases the strength of an attack on a Knight by another 5HP (10HP more total).
	*/
	/// 30 - 0, Straight
	paths.push_back({ "Even More Armor", Knight, 2, [playerData](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GameController::Round();

		float score = .0f;
		if (currRound > 1000 - constants::KnightUpgrade2) {
			score = .0f;
		} else {
			float hasUnits = playerData->teamUnitCounts[bc_UnitType::Knight] > 0.0f ? 1.0f : 0.0f;
			float willHaveUnits = playerData->unitPriority[bc_UnitType::Knight] > 0.0f ? 1.0f : 0.0f;
			score = 30.0f * hasUnits + 30.0f * willHaveUnits;
		} 

		//std::cout << "Even More Armor has a value of " << score << std::endl;
		return score;
	} });

	/*
	150
	Unlocks “Javelin” for Knights.
	*/
	/// 400 - 0, Slope through rounds
	paths.push_back({ "Javelin", Knight, 3, [playerData](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		float currRound = static_cast<float>(GameController::Round());
		// I need it as a float...

		float score = .0f;
		if (currRound > 1000 - constants::KnightUpgrade3) {
			score = .0f;
		} else {
			float hasUnits = playerData->teamUnitCounts[bc_UnitType::Knight] > 0.0f ? 1.0f : 0.0f;
			float willHaveUnits = playerData->unitPriority[bc_UnitType::Knight] > 0.0f ? 1.0f : 0.0f;
			float tempScore = -.0000007f * pow(currRound, 3.0f) - pow(currRound, 2.0f) + 400.0f;
			score = tempScore * hasUnits + tempScore * willHaveUnits;
		} 
		// 60 damage, every 10 rounds. +3~ range

		//std::cout << "Javelin has a value of " << score << std::endl;
		return score;
	} });

	/*
	25
	Decreases a Ranger’s movement cooldown by 5.
	*/
	/// 60 - 0, Straight
	paths.push_back({ "Get in Fast", Ranger, 1, [playerData](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GameController::Round();

		float score = .0f;
		if (currRound > 1000 - constants::RangerUpgrade1) {
			score = .0f;
		} else {
			float hasUnits = playerData->teamUnitCounts[bc_UnitType::Ranger] > 0.0f ? 1.0f : 0.0f;
			float willHaveUnits = playerData->unitPriority[bc_UnitType::Ranger] > 0.0f ? 1.0f : 0.0f;
			float tempScore = 60.0f;
			score = tempScore * hasUnits + tempScore * willHaveUnits;
		} 

		//std::cout << "Get in Fast has a value of " << score << std::endl;
		return score;
	} });

	/*
	100
	Increases a Ranger’s vision range by 30.
	*/
	/// 100 - 0, Straight
	paths.push_back({ "Scopes", Ranger, 2, [playerData](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GameController::Round();

		float score = .0f;
		if (currRound > 1000 - constants::RangerUpgrade2) {
			score = .0f;
		} else {
			float hasUnits = playerData->teamUnitCounts[bc_UnitType::Ranger] > 0.0f ? 1.0f : 0.0f;
			float willHaveUnits = playerData->unitPriority[bc_UnitType::Ranger] > 0.0f ? 1.0f : 0.0f;
			float tempScore = 100.0f;
			score = tempScore * hasUnits + tempScore * willHaveUnits;
		} 

		//std::cout << "Scopes has a value of " << score << std::endl;
		return score;
	} });

	/*
	200
	Unlocks "Snipe" for Rangers.
	*/
	/// 0 - 400 - 0, Straight, Slope through rounds
	paths.push_back({ "Snipe", Ranger, 3, [playerData](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GameController::Round();

		float score = .0f;
		if (currRound > 1000 - constants::RangerUpgrade3) {
			score = .0f;
		} else if(currRound < 400) {
			float hasUnits = playerData->teamUnitCounts[bc_UnitType::Ranger] > 0.0f ? 1.0f : 0.0f;
			float willHaveUnits = playerData->unitPriority[bc_UnitType::Ranger] > 0.0f ? 1.0f : 0.0f;
			float tempScore = 300.0f;
			score = tempScore * hasUnits + tempScore * willHaveUnits;
		} else {
			float hasUnits = playerData->teamUnitCounts[bc_UnitType::Ranger] > 0.0f ? 1.0f : 0.0f;
			float willHaveUnits = playerData->unitPriority[bc_UnitType::Ranger] > 0.0f ? 1.0f : 0.0f;
			float tempScore = pow(.99f, currRound - 1000.0f);
			score = tempScore * hasUnits + tempScore * willHaveUnits;
		}

		//std::cout << "Snipe has a value of " << score << std::endl;
		return score;
	} });

	/*
	25
	Increases standard attack damage by 15HP.
	*/
	/// 30 - 0, Straight through rounds
	paths.push_back({ "Glass Cannon", Mage, 1, [playerData](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GameController::Round();

		float score = .0f;
		if (currRound > 1000 - constants::MageUpgrade1) {
			score = .0f;
		} else {
			float hasUnits = playerData->teamUnitCounts[bc_UnitType::Mage] > 0.0f ? 1.0f : 0.0f;
			float willHaveUnits = playerData->unitPriority[bc_UnitType::Mage] > 0.0f ? 1.0f : 0.0f;
			float tempScore = 30.0f;
			score = tempScore * hasUnits + tempScore * willHaveUnits;
		}

		//std::cout << "Glass Cannon I has a value of " << score << std::endl;
		return score;
	} });

	/*
	75
	Increases standard attack damage by another 15HP (30HP more total).
	*/
	/// 30 - 0, Straight through rounds
	paths.push_back({ "Glass Cannon II", Mage, 2, [playerData](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GameController::Round();

		float score = .0f;
		if (currRound > 1000 - constants::MageUpgrade2) {
			score = .0f;
		} else {
			float hasUnits = playerData->teamUnitCounts[bc_UnitType::Mage] > 0.0f ? 1.0f : 0.0f;
			float willHaveUnits = playerData->unitPriority[bc_UnitType::Mage] > 0.0f ? 1.0f : 0.0f;
			float tempScore = 30.0f;
				score = tempScore * hasUnits + tempScore * willHaveUnits;
		}

		//std::cout << "Glass Cannon II has a value of " << score << std::endl;
		return score;
	} });

	/*
	100
	Glass Cannon III: Increases standard attack damage by another 15HP (45HP more total).
	*/
	/// 30 - 0, Straight through rounds
	paths.push_back({ "Glass Cannon III", Mage, 3, [playerData](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GameController::Round();

		float score = .0f;
		if (currRound > 1000 - constants::MageUpgrade3) {
			score = .0f;
		} else {
			float hasUnits = playerData->teamUnitCounts[bc_UnitType::Mage] > 0.0f ? 1.0f : 0.0f;
			float willHaveUnits = playerData->unitPriority[bc_UnitType::Mage] > 0.0f ? 1.0f : 0.0f;
			float tempScore = 30.0f;
			score = tempScore * hasUnits + tempScore * willHaveUnits;
		}

		//std::cout << "Glass Cannon III has a value of " << score << std::endl;
		return score;
	} });

	/*
	200
	Unlocks “Blink” for Mages.
	*/
	/// 0 - 500 - 0, Parabola through rounds
	paths.push_back({ "Blink", Mage, 4, [playerData](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GameController::Round();

		float score = .0f;
		if (currRound > 1000 - constants::MageUpgrade4) {
			score = .0f;
		} else {
			float hasUnits = playerData->teamUnitCounts[bc_UnitType::Mage] > 0.0f ? 1.0f : 0.0f;
			float willHaveUnits = playerData->unitPriority[bc_UnitType::Mage] > 0.0f ? 1.0f : 0.0f;
			float tempScore =  -.07f * pow(currRound - 600.0f, 2.0f) + 500.0f;
			score = tempScore * hasUnits + tempScore * willHaveUnits;
		}
	
		//std::cout << "Blink has a value of " << score << std::endl;
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
			float hasUnits = playerData->teamUnitCounts[bc_UnitType::Healer] > 0.0f ? 1.0f : 0.0f;
			float willHaveUnits = playerData->unitPriority[bc_UnitType::Healer] > 0.0f ? 1.0f : 0.0f;
			float tempScore = 4.0f;
			score = tempScore * hasUnits + tempScore * willHaveUnits;
		}

		//std::cout << "Spirit Water I has a value of " << score << std::endl;
		return score;
	} });

	/*
	100
	Increases Healer’s healing ability by an another 5HP (7HP more total).
	*/
	/// 10 - 0, Straight
	paths.push_back({ "Spirit Water II", Healer, 2, [playerData](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GameController::Round();

		float score = .0f;
		if (currRound > 1000 - constants::HealerUpgrade2) {
			score = .0f;
		} else {
			float hasUnits = playerData->teamUnitCounts[bc_UnitType::Healer] > 0.0f ? 1.0f : 0.0f;
			float willHaveUnits = playerData->unitPriority[bc_UnitType::Healer] > 0.0f ? 1.0f : 0.0f;
			float tempScore = 10.0f;
			score = tempScore * hasUnits + tempScore * willHaveUnits;
		}

		//std::cout << "Spirit Water II has a value of " << score << std::endl;
		return score;
	} });

	/*
	200
	Unlocks “Overcharge” for Healers.
	*/
	/// 0 - 600 - 0, Parabola through Rounds / Straight
	paths.push_back({ "Overcharge", Healer, 3, [this, playerData](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 1000
		auto currRound = GameController::Round();

		// The Healer Ultimate is most effective at mid - end game after we have gotten an existing ultimate

		float score = .0f;
		if (currRound > 1000 - constants::HealerUpgrade3) {
			score = .0f;
		} else if(this->hasJavelin || this->hasSnipe || this->hasBlink){
			float hasUnits = playerData->teamUnitCounts[bc_UnitType::Healer]> 0.0f ? 1.0f : 0.0f;
			float willHaveUnits = playerData->unitPriority[bc_UnitType::Healer] > 0.0f ? 1.0f : 0.0f;
			float tempScore = -.007f * pow(currRound - 500.0f, 2.0f) + 400.0f;
			score = tempScore * hasUnits + tempScore * willHaveUnits;
		} else {
			float hasUnits = playerData->teamUnitCounts[bc_UnitType::Healer] > 0.0f ? 1.0f : 0.0f;
			float willHaveUnits = playerData->unitPriority[bc_UnitType::Healer] > 0.0f ? 1.0f : 0.0f;
			float tempScore = 100.0f;
			score = tempScore * hasUnits + tempScore * willHaveUnits;
		}

		//std::cout << "Overcharge has a value of " << score << std::endl;
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

		float multiplier = static_cast<float>(currRound);
		

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

			auto totalMarsLocation = MapUtil::marsLocations.size();
			auto passableMarsLocation = MapUtil::marsPassableLocations.size();
			float passableMarsRatio = static_cast<float>(passableMarsLocation) / totalMarsLocation;

			float averageKarbPerRoundOnMars = passableMarsRatio * KarbPerRoundOnMars;
			// float averageKarbPerRoundOnMars = AsteroidPattern::GetTotalKarbOnMarsByRound(currRound+100);

			// 1000 is a hardcoded minimum to be competent on Earth at start
			// Checking for errors in Starting Karbonite
			float EarthToMarsRatio;
			if (playerData->earthStartingKarbonite < 1) {
				EarthToMarsRatio = 0.0f;
			} else {
				EarthToMarsRatio = 1000.0f / playerData->earthStartingKarbonite;
			}
			if (EarthToMarsRatio > multiplier) {
				multiplier = EarthToMarsRatio;
			}
		}
		float score = pow(1.014f, multiplier);

		//std::cout << "Rocketry has a value of " << score << std::endl;
		return score;
	} });

	/*
	100
	Reduces rocket travel time by 20 rounds compared to the travel time determined by the orbit of the planets.
	*/
	/// 650 - 0, Linear through rounds.
	paths.push_back({ "Rocket Boosters", Rocket, 2, [playerData](Upgrade* upgrade) {
		// This Upgrade is pointless if it can't be reached by round 750
		auto currRound = GameController::Round();

		float score = .0f;
		if (currRound > 750 - constants::RocketUpgrade2) {
			score = .0f;
		} else {
			score = static_cast<float>(playerData->optimalFlightTime) / playerData->optimalLaunchRounds.size();
		}
		// Ratio of Turns it takes and launch Amount. Then maybe we should research this?
		// LaunchTime / Launch Amo

		//std::cout << "Rocket Boosters has a value of " << score << std::endl;
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

		float score = .0f;
		if (currRound > 750 - constants::RocketUpgrade3) {
			score = .0f;
		} else {
			score = .0f;
		}
		// Never research this?

		//std::cout << "Increased Capacity has a value of " << score << std::endl;
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
// Change Science so that Each upgrad takes into account it's research time.

// Research should be based on current and future production quotas.
// Research should be based off current and future enemy production
// Research should be based off enemy research. Going a very specific path is vauable
// Ultimates should almost always be prioritized. *Blink and Overcharge, slight exceptions.
// Keep track of what has and hasn't been researched.

// Research should be influenced based on general strategy.

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
