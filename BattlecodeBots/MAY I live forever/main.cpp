#include <iostream>
#include <memory>
#include <math.h>

#include "bc.h"

#include "PlayerData.h"
#include "GameController.h"

#include "Unit.h"
#include "Worker.h"
#include "MapLocation.h"
#include "Location.h"

#include "Research.h"
#include "OrbitPattern.h"
#include "AsteroidPattern.h"
#include "RocketLandingInfo.h"
#include "TeamArray.h"
#include "Utility.h"
#include "MapUtil.h"

#include "CombatOverlord.h"
#include "VecUnit.h"

#include "Factory.h"
#include "Constants.h"

GameController gc;
Research research;
OrbitPattern orbitPattern;
AsteroidPattern asteroidPattern;
RocketLandingInfo rocketLandingInfo;
TeamArray teamArray;
MapUtil mapUtil;
PlayerData playerData;

CombatOverlord combat;

int main()
{
	srand(0);

	while (true)
	{
		uint32_t round = GameController::Round();
		if (round % 10 == 0) {
			std::cout << "Round: " << round << std::endl;
		}
		playerData.Update();
		combat.Update();
		//auto units = GameController::Units(MyTeam);
		//for (auto& unit : units) {
		//	if (unit.type == Worker) {
		//		units::Worker worker{ bc_Unit_clone(unit.self) };
		//		MapLocation location = unit.Loc().ToMapLocation();
		//		auto toBuild = VecUnit::Wrap<units::Factory>(bc_GameController_sense_nearby_units_by_type(GameController::gc, location.self, 1, Factory));
		//		if (toBuild.size() > 0) {
		//			if (worker.CanBuild(toBuild[0])) {
		//				worker.Build(toBuild[0]);
		//			}
		//		}
		//		for (auto& direction : constants::directions_adjacent) {
		//			if (worker.CanBlueprint(Factory, direction)) {
		//				worker.Blueprint(Factory, direction);
		//			}
		//			MapLocation adjacent = MapLocation::Neighbor(location, direction);
		//		}
		//	}
		//	else if (unit.type == Factory) {
		//		units::Factory factory{ bc_Unit_clone(unit.self) };
		//		if (factory.CanProduce(Knight)) {
		//			factory.Produce(Knight);
		//		}
		//		for (auto& direction : constants::directions_adjacent) {
		//			if (factory.CanUnload(direction)) {
		//				factory.Unload(direction);
		//			}
		//		}
		//	}
		//}
		GameController::EndTurn();
	}
}