#include "BuilderOverlord.h"
#include "GameController.h"
#include "Worker.h"
#include "Unit.h"
#include "Structure.h"
#include "Factory.h"
#include "Rocket.h"
#include <iostream>

std::array<int, 5> tryRotate = { 0, -1, 1, -2, 2 };


void BuilderOverlord::Update(uint32_t round)
{
	m_workerCount = 0;
	m_factoryCount = 0;
	m_rocketCount = 0;
	//round specifics and triggers
	switch (round)
	{
	case 1:
		m_desiredWorkers = 4;
		m_desiredFactories = 1;
		break;

	case 15:
		m_desiredWorkers = 8;
		break;

	
	}

	std::vector<units::Unit> units = GameController::Units(bc_Selection::MyTeam);

	std::vector<int> workerIndexes;
	std::vector<int> factoryIndexes;
	std::vector<int> rocketIndexes;

	for (int i = 0; i < units.size(); i++)
	{
		switch (units[i].type)
		{
		case bc_UnitType::Worker: workerIndexes.push_back(i); m_workerCount++; break;
		case bc_UnitType::Factory: factoryIndexes.push_back(i); m_factoryCount++; break;
		case bc_UnitType::Rocket: rocketIndexes.push_back(i); m_rocketCount++; break;

		}
	}

	std::cout << m_workerCount << " / " << m_desiredWorkers << " workers." << std::endl;
	std::cout << m_factoryCount << " / " << m_desiredFactories << " factories." << std::endl;
	std::cout << m_rocketCount << " / " << m_desiredRockets << " rockets." << std::endl;



	for (int i = 0; i < workerIndexes.size(); ++i)
	{
		bool replicate;
		bool build;
		bool acted = false;
		m_workerCount < m_desiredWorkers ? replicate = true : replicate = false;
		m_factoryCount < m_desiredFactories || m_rocketCount < m_desiredRockets ? build = true : build = false;


		units::Worker worker = bc_Unit_clone(units[workerIndexes[i]].self);
		//Check that worker is not in space

		if (!worker.Loc().IsInSpace() && !worker.Loc().IsInGarrison())
		{
			//If we want to replicate, check that we have enough resources and then find a spot to replicate to.
			if (replicate && GameController::Karbonite() >= 15)
			{
				std::cout << "Replicating!" << std::endl;
				for (int dir : tryRotate)
				{
					bc_Direction replicateDirection = static_cast<bc_Direction>(South + dir);
					if (worker.CanReplicate(replicateDirection))
					{
						worker.Replicate(replicateDirection);
						m_workerCount++;
						std::cout << "Done replicating." << std::endl;

						break;
					}

				}




			}

			if (m_unfinishedFactories || m_unfinishedRockets)
			{
				std::cout << "Working on a project!" << std::endl;

				build = false;
				acted = true;
				std::cout << "0" << std::endl;

				MapLocation locationOfStructure = worker.Loc().ToMapLocation();
				int structureIndex;
				bool isFactory = false;
				bool foundAdjacentProject = false;
				uint32_t closestDistance = UINT32_MAX;
				//Iterate through factories to see if one is adjacent
				for (int fIndex : factoryIndexes)
				{
					units::Factory factory = bc_Unit_clone(units[fIndex].self);
					MapLocation loc = factory.Loc().ToMapLocation();

					if (!factory.IsBuilt())
					{
						std::cout << "1" << std::endl;

						MapLocation loc = factory.Loc().ToMapLocation();

						std::cout << "2" << std::endl;

						uint32_t tempDistance = worker.Loc().ToMapLocation().DirectionTo(loc);

						//Check if this distance is closer than the last closest, if so set the location to it and check to see if it is adjacent.
						if (tempDistance < closestDistance)
						{
							locationOfStructure = loc;
							isFactory = true;
							structureIndex = fIndex;
							std::cout << "3" << std::endl;

							if (worker.Loc().ToMapLocation().DistanceTo(loc) < 2)
							{
								foundAdjacentProject = true;
								break;
							}
						}
					}
				}

				//If we did not find an adjacent factory, iterate through rockets to find one.
				if (!foundAdjacentProject)
				{
					for (int rIndex : rocketIndexes)
					{
						units::Rocket rocket = bc_Unit_clone(units[rIndex].self);
						if (!rocket.IsBuilt())
						{
							std::cout << "4" << std::endl;

							MapLocation loc = rocket.Loc().ToMapLocation();
							std::cout << "5" << std::endl;

							uint32_t tempDistance = worker.Loc().ToMapLocation().DistanceTo(loc);

							//Check if this distance is closer than the last closest, if so set the location to it and check to see if it is adjacent.
							if (tempDistance < closestDistance)
							{
								locationOfStructure = loc;
								isFactory = false;
								structureIndex = rIndex;
								std::cout << "6" << std::endl;

								if (worker.Loc().ToMapLocation().IsAdjacentTo(loc))
								{
									foundAdjacentProject = true;
									break;
								}
							}
						}
					}
				}

				//If we found an adjacent project, work on it.
				if (foundAdjacentProject)
				{

					units::Structure structure = bc_Unit_clone(units[factoryIndexes[structureIndex]].self);
					if (!isFactory)
					{
						structure = bc_Unit_clone(units[rocketIndexes[structureIndex]].self);
					}
			

					if (worker.CanBuild(structure))
					{
						worker.Build(structure);
						std::cout << "Contributed to project." << std::endl;

					}
				}
				else //Move towards project.
				{
					std::cout << "7" << std::endl;

					bc_Direction directionTo = worker.Loc().ToMapLocation().DirectionTo(locationOfStructure);

					for (int d : tryRotate)
					{
						bc_Direction dir = static_cast<bc_Direction>(directionTo + d);
						if (worker.CanMove(dir))
						{
							worker.Move(dir);
							std::cout << "Moved towards project." << std::endl;

							break;
						}
						
					}
					std::cout << "Tried moving towards project." << std::endl;

				}

			}

			if (build)
			{
				std::cout << "Blueprinting a project!" << std::endl;

				acted = true;
				//Find clear spot to build
				bool foundOpenSpot = false;
				int spotX = 0;
				int spotY = 0;


				for (int x = -1; x < 2; x++)
				{
					for (int y = -1; y < 2; y++)
					{
						spotX = x + worker.Loc().ToMapLocation().X();
						spotY = y + worker.Loc().ToMapLocation().Y();
						bool isMars = GameController::Planet();
						MapLocation ml = MapLocation(GameController::Planet(), spotX, spotY);

						foundOpenSpot = ml.IsOccupiable();
						if (foundOpenSpot)
						{						
							break;
						}
					}
					if (foundOpenSpot)
					{
						break;
					}
				}


				//if clear spot exists, build with priority given to rockets.
				if (foundOpenSpot)
				{
					MapLocation buildLocation = MapLocation(GameController::Planet(), spotX, spotY);
					bc_Direction buildDirection = worker.Loc().ToMapLocation().DirectionTo(buildLocation);
					if (m_rocketCount < m_desiredRockets && GameController::Karbonite() >= 75)
					{
						if(worker.CanBlueprint(bc_UnitType::Rocket, buildDirection))
						{
							worker.Blueprint(bc_UnitType::Rocket, buildDirection);
							m_unfinishedRockets = true;
							std::cout << "Blueprinted a rocket." << std::endl;
						}
					}
					else if (m_factoryCount < m_desiredFactories && GameController::Karbonite() >= 100)
					{
						if (worker.CanBlueprint(bc_UnitType::Factory, buildDirection))
						{
							worker.Blueprint(bc_UnitType::Factory, buildDirection);
							m_unfinishedRockets = true;
							std::cout << "Blueprinted a factory." << std::endl;
						}
					}
					else
					{
						std::cout << "Not enough karbonite to blueprint!" << std::endl;
					}

				}
			}

			if (!acted)
			{
				bool foundKarboniteDeposit = false;
				int spotX = 0;
				int spotY = 0;

				for (int x = -1; x < 2; x++)
				{
					for (int y = -1; y < 2; y++)
					{
						if (GameController::Planet())
						{
							foundKarboniteDeposit = bc_GameController_karbonite_at(GameController::gc, new_bc_MapLocation(GameController::Planet(), spotX, spotY));
						}
					
						if (foundKarboniteDeposit)
						{
							spotX = x + worker.Loc().ToMapLocation().X();
							spotY = y + worker.Loc().ToMapLocation().Y();
							break;
						}
					}
					if (foundKarboniteDeposit)
					{
						break;
					}
				}

				if (foundKarboniteDeposit)
				{
					MapLocation harvestLocation = MapLocation(GameController::Planet(), spotX, spotY);
					bc_Direction harvestDirection = worker.Loc().ToMapLocation().DirectionTo(harvestLocation);

					if (worker.CanHarvest(harvestDirection))
					{
						worker.Harvest(harvestDirection);

					}
				}
			}

		}
	}

	for (int fIndex : factoryIndexes)
	{
		m_unfinishedFactories = true;
		units::Factory factory = bc_Unit_clone(units[fIndex].self);

		if (!factory.IsBuilt())
		{
			m_unfinishedFactories = true;
		}
	}

	for (int rIndex : rocketIndexes)
	{
		m_unfinishedRockets = false;
		units::Rocket rocket = bc_Unit_clone(units[rIndex].self);

		if (!rocket.IsBuilt())
		{
			m_unfinishedRockets = true;
		}
	}
}
