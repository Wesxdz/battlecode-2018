#include "Science.h"

#include <iostream>
#include <algorithm>
#include "GameController.h"

void Science::Update()
{
	bc_ResearchInfo* info = bc_GameController_research_info(GameController::gc);
	if (researchNextTurn) { // Determine what upgrade to research
		// Upgrades are removed from paths once they are researched
		paths.erase(std::remove_if(std::begin(paths), std::end(paths), [&info](Upgrade& upgrade) {
			return bc_ResearchInfo_get_level(info, upgrade.branch) >= upgrade.level;
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
	uint32_t roundsLeft = bc_ResearchInfo_rounds_left(info);
	if (roundsLeft == 1) {
		researchNextTurn = true;
	}
	delete_bc_ResearchInfo(info);
}

void Science::Init()
{
	/*
	25
	Workers may harvest an additional 1 Karbonite from a deposit at a time.
	*/
	paths.push_back({ "Gimme some of that Black Stuff", Worker, 1, [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	} });

	/*
	75
	Workers add 1 more health when repairing or constructing a building.
	*/
	paths.push_back({ "Time is of the Essence", Worker, 2, [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	} });

	/*
	75
	Workers add another 1 more health (2 more total) when repairing or constructing a building.
	*/
	paths.push_back({ "Time is of the Essence II", Worker, 3, [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	} });

	/*
	75
	Workers add another 3 more health (5 more total) when repairing or constructing a building.
	*/
	paths.push_back({ "Time is of the Essence III", Worker, 4, [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	} });

	/*
	25
	Decreases the strength of an attack on a Knight by an additional 5HP.
	*/
	paths.push_back({ "Armor", Knight, 1, [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	 } });

	/*
	75
	Decreases the strength of an attack on a Knight by another 5HP (10HP more total).
	*/
	paths.push_back({ "Even More Armor", Knight, 2, [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	} });

	/*
	150
	Unlocks “Javelin” for Knights.
	*/
	paths.push_back({ "Javelin", Knight, 3, [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	} });

	/*
	25
	Decreases a Ranger’s movement cooldown by 5.
	*/
	paths.push_back({ "Get in Fast", Ranger, 1, [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	} });

	/*
	100
	Increases a Ranger’s vision range by 30.
	*/
	paths.push_back({ "Scopes", Ranger, 2, [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	} });

	/*
	200
	Unlocks "Snipe" for Rangers.
	*/
	paths.push_back({ "Snipe", Ranger, 3, [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	} });

	/*
	25
	Increases standard attack damage by 15HP.
	*/
	paths.push_back({ "Glass Cannon", Mage, 1, [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	} });

	/*
	75
	Increases standard attack damage by another 15HP (30HP more total).
	*/
	paths.push_back({ "Glass Cannon II", Mage, 2, [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	} });

	/*
	100
	Glass Cannon III: Increases standard attack damage by another 15HP (45HP more total).
	*/
	paths.push_back({ "Glass Cannon III", Mage, 3, [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	} });

	/*
	200
	Unlocks “Blink” for Mages.
	*/
	paths.push_back({ "Blink", Mage, 4, [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	} });

	/*
	25
	Increases Healer’s healing ability by 2HP.
	*/
	paths.push_back({ "Spirit Water", Healer, 1, [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	} });

	/*
	100
	Increases Healer’s healing ability by an another 5HP (7HP more total).
	*/
	paths.push_back({ "Spirit Water II", Healer, 2, [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	} });

	/*
	200
	Unlocks “Overcharge” for Healers.
	*/
	paths.push_back({ "Overcharge", Healer, 3, [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	} });

	/*
	100
	Unlocks rocket technology. Workers can now blueprint and build rockets.
	*/
	paths.push_back({ "Rocketry", Rocket, 1, [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	} });

	/*
	100
	Reduces rocket travel time by 20 rounds compared to the travel time determined by the orbit of the planets.
	*/
	paths.push_back({ "Rocket Boosters", Rocket, 2, [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	} });

	/*
	100
	Allows rockets to garrison 4 more units per rocket.
	*/
	paths.push_back({ "Increased Capacity", Rocket, 3, [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	} });

}

uint32_t Upgrade::TurnsToResearch()
{
	bc_ResearchInfo* info = bc_GameController_research_info(GameController::gc);
	uintptr_t startLevel = bc_ResearchInfo_get_level(info, branch);
	uint32_t turnsToResearch = 0;
	for (uintptr_t i = startLevel; i <= level; i++) {
		turnsToResearch += cost_of(branch, i);
	}
	delete_bc_ResearchInfo(info);
	return turnsToResearch;
}

void Upgrade::Research()
{
	std::cout << "Researching " << name << "\n";
	bc_GameController_reset_research(GameController::gc);
	bc_GameController_queue_research(GameController::gc, branch);
}
