#include "BuilderOverlord.h"
#include "GameController.h"
#include "GameMap.h"

std::array<int, 5> tryRotate = { 0, -1, 1, -2, 2 };


void BuilderOverlord::Update(uint32_t round)
{
	//round specifics and triggers
	switch (round)
	{
	case 0:
		m_desiredWorkers = 4;
		m_desiredFactories = 1;
		break;

	case 15:
		m_desiredWorkers = 10;
		break;

	case 25:
		m_desiredFactories = 3;
		break;
	}

	bc_VecUnit* units = bc_GameController_my_units(GameController::gc);
	for (int i = 0; i < m_workerIDs.size(); ++i)
	{
		bool replicate;
		bool build;
		bool acted = false;
		m_workerCount < m_desiredWorkers ? replicate = true : replicate = false;
		m_factoryCount < m_desiredFactories || m_rocketCount < m_desiredRockets ? build = true : build = false;

		bc_Unit* worker = bc_VecUnit_index(units, m_workerIDs[i]);
		//Check that worker is not in space
		if (!bc_Location_is_in_space(bc_Unit_location(worker)))
		{
			//If we want to replicate, check that we have enough resources and then find a spot to replicate to.
			if (replicate && GameController::Karbonite() >= 15)
			{
				for (int dir : tryRotate)
				{
					bc_Direction replicateDirection = static_cast<bc_Direction>(South + dir);
					if (bc_GameController_can_replicate(GameController::gc, m_workerIDs[i], replicateDirection))
					{
						bc_GameController_replicate(GameController::gc, m_workerIDs[i], replicateDirection); //TODO: command this worker
						
						break;
					}

				}




			}

			if (m_unfinishedFactories || m_unfinishedRockets)
			{
				build = false;
				acted = true;
				bc_Location* locationOfStructure;
				int structureID;
				bool factory = false;
				bool foundAdjacentProject = false;
				uint32_t closestDistance = UINT32_MAX;
				//Iterate through factories to see if one is adjacent
				for (int j = 0; j < m_factoryIDs.size(); j++)
				{
					bc_Unit* structure = bc_VecUnit_index(units, m_factoryIDs[j]);

					if (bc_Unit_structure_is_built(structure))
					{
						delete_bc_Unit(structure);
						break;
					}


					bc_Location* loc = bc_Unit_location(structure);
					uint32_t tempDistance = bc_MapLocation_distance_squared_to(bc_Location_map_location(loc), bc_Location_map_location(bc_Unit_location(worker)));

					//Check if this distance is closer than the last closest, if so set the location to it and check to see if it is adjacent.
					if (tempDistance < closestDistance)
					{
						locationOfStructure = loc;
						factory = true;
						structureID = j;
						if (bc_Location_is_adjacent_to(loc, bc_Unit_location(worker)))
						{
							foundAdjacentProject = true;
							break;
						}
					}



					delete_bc_Location(loc);
					delete_bc_Unit(structure);
				}

				//If we did not find an adjacent factory, iterate through rockets to find one.
				if (!foundAdjacentProject)
				{
					for (int j = 0; j < m_factoryIDs.size(); j++)
					{
						bc_Unit* structure = bc_VecUnit_index(units, m_rocketIDs[j]);
						if (bc_Unit_structure_is_built(structure))
						{
							delete_bc_Unit(structure);
							break;
						}
						bc_Location* loc = bc_Unit_location(structure);
						uint32_t tempDistance = bc_MapLocation_distance_squared_to(bc_Location_map_location(loc), bc_Location_map_location(bc_Unit_location(worker)));

						//Check if this distance is closer than the last closest, if so set the location to it and check to see if it is adjacent.
						if (tempDistance < closestDistance)
						{
							locationOfStructure = loc;
							factory = false;
							structureID = j;
							if (bc_Location_is_adjacent_to(loc, bc_Unit_location(worker)))
							{
								foundAdjacentProject = true;
								break;
							}
						}

						delete_bc_Location(loc);
						delete_bc_Unit(structure);
					}
				}

				//If we found an adjacent project, work on it.
				if (foundAdjacentProject)
				{
					bc_Unit* structure = bc_VecUnit_index(units, structureID);

					if (factory)
					{
						if (bc_GameController_can_build(GameController::gc, m_workerIDs[i], bc_UnitType::Factory))
							bc_GameController_build(GameController::gc, m_workerIDs[i], bc_UnitType::Factory);
					}
					else
					{
						if (bc_GameController_can_build(GameController::gc, m_workerIDs[i], bc_UnitType::Rocket))
							bc_GameController_build(GameController::gc, m_workerIDs[i], bc_UnitType::Rocket);
					}
					delete_bc_Unit(structure);
				}
				else //Move towards project.
				{
					bc_Direction directionTo = bc_MapLocation_direction_to(bc_Location_map_location(bc_Unit_location(worker)), bc_Location_map_location(locationOfStructure));

					for (int d : tryRotate)
					{
						bc_Direction dir = static_cast<bc_Direction>(directionTo + d);
						if (bc_GameController_can_move(GameController::gc, m_workerIDs[i], dir))
						{
							bc_GameController_move_robot(GameController::gc, m_workerIDs[i], dir);
							break;
						}
					}

				}

				delete_bc_Location(locationOfStructure);
			}

			if (build)
			{
				acted = true;
				//Find clear spot to build
				bool foundOpenSpot = false;
				int spotX = 0;
				int spotY = 0;

				bc_MapLocation* workerLocation = bc_Location_map_location(bc_Unit_location(worker));

				for (int x = -1; x < 2; x++)
				{
					for (int y = -1; y < 2; y++)
					{
						bool isMars = GameController::Planet();
						foundOpenSpot = bc_GameController_is_occupiable(GameController::gc, workerLocation);

						if (foundOpenSpot)
						{
							spotX = x + bc_MapLocation_x_get(workerLocation);
							spotY = y + bc_MapLocation_y_get(workerLocation);
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
					bc_MapLocation* buildLocation = new_bc_MapLocation(GameMap::Earth().Planet(), spotX, spotY);
					bc_Direction buildDirection = bc_MapLocation_direction_to(workerLocation, buildLocation);
					if (m_rocketCount < m_desiredRockets && GameController::Karbonite() >= 75)
					{
						if (bc_GameController_can_blueprint(GameController::gc, m_workerIDs[i], bc_UnitType::Rocket, buildDirection))
						{
							bc_GameController_blueprint(GameController::gc, m_workerIDs[i], bc_UnitType::Rocket, buildDirection);
							m_unfinishedRockets = true;
						}
					}
					else if (m_factoryCount < m_desiredFactories && GameController::Karbonite() >= 100)
					{
						if (bc_GameController_can_blueprint(GameController::gc, m_workerIDs[i], bc_UnitType::Factory, buildDirection))
						{
							bc_GameController_blueprint(GameController::gc, m_workerIDs[i], bc_UnitType::Factory, buildDirection);
							m_unfinishedFactories = true;
						}
					}
					delete_bc_MapLocation(buildLocation);
				}
				delete_bc_MapLocation(workerLocation);
			}

			if (!acted)
			{
				bool foundKarboniteDeposit = false;
				int spotX = 0;
				int spotY = 0;

				bc_MapLocation* workerLocation = bc_Location_map_location(bc_Unit_location(worker));

				for (int x = -1; x < 2; x++)
				{
					for (int y = -1; y < 2; y++)
					{
						//Checking GameController::Planet() returns mars or earth, 1 and 0 respectively.
						if (GameController::Planet())
						{
							foundKarboniteDeposit = bc_GameController_karbonite_at(GameController::gc, new_bc_MapLocation(GameMap::Mars().Planet(), spotX, spotY));
						}
						else
						{
							foundKarboniteDeposit = bc_GameController_karbonite_at(GameController::gc, new_bc_MapLocation(GameMap::Earth().Planet(), spotX, spotY));
						}



						if (foundKarboniteDeposit)
						{
							spotX = x + bc_MapLocation_x_get(workerLocation);
							spotY = y + bc_MapLocation_y_get(workerLocation);
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
					bc_MapLocation* buildLocation = new_bc_MapLocation(GameMap::Earth().Planet(), spotX, spotY);
					bc_Direction buildDirection = bc_MapLocation_direction_to(workerLocation, buildLocation);

					if (bc_GameController_can_harvest(GameController::gc, m_workerIDs[i], buildDirection))
					{
						bc_GameController_harvest(GameController::gc, m_workerIDs[i], buildDirection);
						
					}
					delete_bc_MapLocation(buildLocation);
				}

				if (workerLocation)
					delete_bc_MapLocation(workerLocation);
			}

		}


		delete_bc_Unit(worker);
	}

	for (int id : m_factoryIDs)
	{
		m_unfinishedFactories = false;
		bc_Unit* factory = bc_VecUnit_index(units, m_factoryIDs[id]);

		if (!bc_Unit_structure_is_built(factory))
		{
			m_unfinishedFactories = false;
		}


		delete_bc_Unit(factory);

	}

	for (int id : m_rocketIDs)
	{
		m_unfinishedFactories = false;
		bc_Unit* rocket = bc_VecUnit_index(units, m_rocketIDs[id]);

		if (!bc_Unit_structure_is_built(rocket))
		{
			m_unfinishedRockets = false;
		}


		delete_bc_Unit(rocket);
	}
	delete_bc_VecUnit(units);
}


void BuilderOverlord::AddToUnitContainers(bc_UnitType unitType, int unitID)
{
	switch (unitType)
	{
		//worker
	case 0:
		m_workerIDs.push_back(unitID);
		m_workerCount++;
		break;

		//factory
	case 5:
		m_factoryIDs.push_back(unitID);
		m_factoryCount++;
		break;

		//rocket
	case 6:
		m_rocketIDs.push_back(unitID);
		m_rocketCount++;
		break;
	}
}
