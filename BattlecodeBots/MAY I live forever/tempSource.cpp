//#include <iostream>
//#include <memory>
//#include <math.h>
//
//#include "bc.h"
//
//#include "PlayerData.h"
//#include "GameController.h"
//
//#include "Unit.h"
//#include "Worker.h"
//#include "MapLocation.h"
//#include "Location.h"
//
//#include "OrbitPattern.h"
//#include "AsteroidPattern.h"
//#include "RocketLandingInfo.h"
//#include "TeamArray.h"
//#include "Utility.h"
//#include "MapUtil.h"
//
//#include "Science.h"
//#include "BuilderOverlord.h"
//
//GameController gc;
//OrbitPattern orbitPattern;
//AsteroidPattern asteroidPattern;
//RocketLandingInfo rocketLandingInfo;
//TeamArray teamArray;
//MapUtil mapUtil;
//PlayerData playerData;
//
//
//Science science;
//BuilderOverlord builder;
//
//int main()
//{
//	srand(0);
//
//	// Init Science
//	science.Init(&playerData);
//
//	while (true)
//	{
//		uint32_t round = GameController::Round();
//		if (round % 10 == 0) 
//		{
//			std::cout << "Round: " << round << std::endl;
//		}
//
//		playerData.Update();
//		if (GameController::Planet() == bc_Planet::Earth) {
//			science.Update();
//		} else if(round > 749){
//			science.Update();
//		}
//		
//
//		builder.Update(round);
//		GameController::EndTurn();
//	}
//}