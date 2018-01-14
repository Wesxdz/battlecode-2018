#include "ResearchPolicies.h"

#include <iostream>
#include <algorithm>
#include "Player.h"

void ResearchPolicies::Update()
{
	bc_ResearchInfo* info = bc_GameController_research_info(Player::gc);
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

void ResearchPolicies::Init()
{
	/*
	25
	Workers may harvest an additional 1 Karbonite from a deposit at a time.
	*/
	Upgrade worker1{ "Gimme some of that Black Stuff", Worker, 1 };
	worker1.Evaluate = [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	};
	paths.push_back(worker1);

	/*
	75
	Workers add 1 more health when repairing or constructing a building.
	*/
	Upgrade worker2{ "Time is of the Essence", Worker, 2 };
	worker2.Evaluate = [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	};
	paths.push_back(worker2);
	/*
	75
	Workers add another 1 more health (2 more total) when repairing or constructing a building.
	*/
	Upgrade worker3{ "Time is of the Essence II", Worker, 3 };
	worker3.Evaluate = [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	};
	paths.push_back(worker3);
	/*
	75
	Workers add another 3 more health (5 more total) when repairing or constructing a building.
	*/
	Upgrade worker4{ "Time is of the Essence III", Worker, 4 };
	worker4.Evaluate = [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	};
	paths.push_back(worker4);

	/*
	25
	Decreases the strength of an attack on a Knight by an additional 5HP.
	*/
	Upgrade knight1{ "Armor", Knight, 1 };
	knight1.Evaluate = [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	};
	paths.push_back(knight1);

	/*
	75
	Decreases the strength of an attack on a Knight by another 5HP (10HP more total).
	*/
	Upgrade knight2{ "Even More Armor", Knight, 2 };
	knight2.Evaluate = [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	};
	paths.push_back(knight2);

	/*
	150
	Unlocks “Javelin” for Knights.
	*/
	Upgrade knight3{ "Javelin", Knight, 3 };
	knight3.Evaluate = [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	};
	paths.push_back(knight3);

	/*
	25
	Decreases a Ranger’s movement cooldown by 5.
	*/
	Upgrade ranger1{ "Get in Fast", Ranger, 1 };
	ranger1.Evaluate = [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	};
	paths.push_back(ranger1);

	/*
	100
	Increases a Ranger’s vision range by 30.
	*/
	Upgrade ranger2{ "Scopes", Ranger, 2 };
	ranger2.Evaluate = [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	};
	paths.push_back(ranger2);

	/*
	200
	Unlocks "Snipe" for Rangers.
	*/
	Upgrade ranger3{ "Snipe", Ranger, 3 };
	ranger3.Evaluate = [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	};
	paths.push_back(ranger3);


	/*
	25
	Increases standard attack damage by 15HP.
	*/
	Upgrade mage1{ "Glass Cannon", Mage, 1 };
	mage1.Evaluate = [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	};
	paths.push_back(mage1);

	/*
	75
	Increases standard attack damage by another 15HP (30HP more total).
	*/
	Upgrade mage2{ "Glass Cannon II", Mage, 2 };
	mage2.Evaluate = [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	};
	paths.push_back(mage2);

	/*
	100
	Glass Cannon III: Increases standard attack damage by another 15HP (45HP more total).
	*/
	Upgrade mage3{ "Glass Cannon III", Mage, 3 };
	mage3.Evaluate = [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	};
	paths.push_back(mage3);

	/*
	200
	Unlocks “Blink” for Mages.
	*/
	Upgrade mage4{ "Blink", Mage, 4 };
	mage4.Evaluate = [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	};
	paths.push_back(mage4);

	/*
	25
	Increases Healer’s healing ability by 2HP.
	*/
	Upgrade healer1{ "Spirit Water", Healer, 1 };
	healer1.Evaluate = [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	};
	paths.push_back(healer1);

	/*
	100
	Increases Healer’s healing ability by an another 5HP (7HP more total).
	*/
	Upgrade healer2{ "Spirit Water II", Healer, 2 };
	healer2.Evaluate = [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	};
	paths.push_back(healer2);

	/*
	200
	Unlocks “Overcharge” for Healers.
	*/
	Upgrade healer3{ "Overcharge", Healer, 3 };
	healer3.Evaluate = [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	};
	paths.push_back(healer3);

	/*
	100
	Unlocks rocket technology. Workers can now blueprint and build rockets.
	*/
	Upgrade rocket1{ "Rocketry", Rocket, 1 };
	rocket1.Evaluate = [](Upgrade* upgrade) {
		float score = 0.1f;
		if (bc_GameController_round(Player::gc) >= 400) { // Always research rocket
			score += 100000.0f;
		}
		return score;
	};
	paths.push_back(rocket1);

	/*
	100
	Reduces rocket travel time by 20 rounds compared to the travel time determined by the orbit of the planets.
	*/
	Upgrade rocket2{ "Rocket Boosters", Rocket, 2 };
	rocket2.Evaluate = [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	};
	paths.push_back(rocket2);

	/*
	100
	Allows rockets to garrison 4 more units per rocket.
	*/
	Upgrade rocket3{ "Increased Capacity", Rocket, 3 };
	rocket3.Evaluate = [](Upgrade* upgrade) {
		float score = 0.1f;
		return score;
	};
	paths.push_back(rocket3);

}

uint32_t Upgrade::TurnsToResearch()
{
	bc_ResearchInfo* info = bc_GameController_research_info(Player::gc);
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
	bc_GameController_reset_research(Player::gc);
	std::cout << "Was added " << (bool)bc_GameController_queue_research(Player::gc, branch) << "\n";
}
