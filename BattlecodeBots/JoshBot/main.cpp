#include <iostream>
#include <random>
#include "bc.h"

#include "GameController.h"
#include "PlanetMap.h"
#include "Research.h"
#include "Unit.h"
#include "Worker.h"
#include "Knight.h"
#include "Factory.h"
#include "Location.h"
#include "MapLocation.h"


bool check_errors()
{
	if (bc_has_err())
	{
		char* err;
		int8_t code = bc_get_last_err(&err);
		std::cout << "Engine error code " << code << ": " << err << std::endl;
		bc_free_string(err);
		return true;
	}
	else
	{
		return false;
	}
}

GameController gc;
Research research;
PlanetMap planetMap;
bc_Team thisTeam;
int round;
int currKarbonite;
std::vector<std::shared_ptr<units::Unit>> units;

MapLocation wayPoint;

enum WorkerPriority {
	EXPLORE,
	KARB,
	REPLICATE,
	FACTORY,
	ROCKET,
};
WorkerPriority currWorkerPriority = WorkerPriority::FACTORY;
int workerPriorityAmo = 4;

enum FactoryPriority {
	IDLE,
	BUILD
};
FactoryPriority currFactoryPriority = FactoryPriority::BUILD;
bc_UnitType currFactoryBuild = bc_UnitType::Knight;

void UpdateWorkerEarth(int index) {
	units::Worker worker;
	worker.Init(units[index]->self);
	std::cout << "Updating Worker " << worker.id << std::endl;

	auto location = worker.Loc();

	MapLocation mapLocation = location.ToMapLocation();
	int x = mapLocation.X();
	int y = mapLocation.Y();
	switch (currWorkerPriority) {
		case FACTORY:
			// Keep track of workign facotries...
			{
				MapLocation ulLoc(bc_Planet::Earth, x - 1, y - 1);
				if (!ulLoc.Occupiable()) {
					auto occupant = ulLoc.Occupant();
					if (occupant->type == bc_UnitType::Factory) {
						units::Factory f;
						f.Init(occupant->self);
						if (!f.IsBuilt() && f.Team() == thisTeam) {
							worker.Build(f.id);
							std::cout << "Worker " << worker.id << " helped build Factory" << f.id << std::endl;
						}
					}
				}
				MapLocation uLoc(bc_Planet::Earth, x, y - 1);
				if (!uLoc.Occupiable()) {
					auto occupant = uLoc.Occupant();
					if (occupant->type == bc_UnitType::Factory) {
						units::Factory f;
						f.Init(occupant->self);
						if (!f.IsBuilt() && f.Team() == thisTeam) {
							worker.Build(f.id);
							std::cout << "Worker " << worker.id << " helped build Factory" << f.id << std::endl;
						}
					}
				}
				MapLocation urLoc(bc_Planet::Earth, x + 1, y - 1);
				if (!urLoc.Occupiable()) {
					auto occupant = urLoc.Occupant();
					if (occupant->type == bc_UnitType::Factory) {
						units::Factory f;
						f.Init(occupant->self);
						if (!f.IsBuilt() && f.Team() == thisTeam) {
							worker.Build(f.id);
							std::cout << "Worker " << worker.id << " helped build Factory" << f.id << std::endl;
						}
					}
				}
				MapLocation lLoc(bc_Planet::Earth, x - 1, y);
				if (!lLoc.Occupiable()) {
					auto occupant = lLoc.Occupant();
					if (occupant->type == bc_UnitType::Factory) {
						units::Factory f;
						f.Init(occupant->self);
						if (!f.IsBuilt() && f.Team() == thisTeam) {
							worker.Build(f.id);
							std::cout << "Worker " << worker.id << " helped build Factory" << f.id << std::endl;
						}
					}
				}
				MapLocation rLoc(bc_Planet::Earth, x + 1, y);
				if (!rLoc.Occupiable()) {
					auto occupant = rLoc.Occupant();
					if (occupant->type == bc_UnitType::Factory) {
						units::Factory f;
						f.Init(occupant->self);
						if (!f.IsBuilt() && f.Team() == thisTeam) {
							worker.Build(f.id);
							std::cout << "Worker " << worker.id << " helped build Factory" << f.id << std::endl;
						}
					}
				}
				MapLocation dlLoc(bc_Planet::Earth, x - 1, y + 1);
				if (!dlLoc.Occupiable()) {
					auto occupant = dlLoc.Occupant();
					if (occupant->type == bc_UnitType::Factory) {
						units::Factory f;
						f.Init(occupant->self);
						if (!f.IsBuilt() && f.Team() == thisTeam) {
							worker.Build(f.id);
							std::cout << "Worker " << worker.id << " helped build Factory" << f.id << std::endl;
						}
					}
				}
				MapLocation dLoc(bc_Planet::Earth, x, y + 1);
				if (!dLoc.Occupiable()) {
					auto occupant = dLoc.Occupant();
					if (occupant->type == bc_UnitType::Factory) {
						units::Factory f;
						f.Init(occupant->self);
						if (!f.IsBuilt() && f.Team() == thisTeam) {
							worker.Build(f.id);
							std::cout << "Worker " << worker.id << " helped build Factory" << f.id << std::endl;
						}
					}
				}
				
				MapLocation drLoc(bc_Planet::Earth, x + 1, y + 1);
				if (!drLoc.Occupiable()) {
					auto occupant = drLoc.Occupant();
					if (occupant->type == bc_UnitType::Factory) {
						units::Factory f;
						f.Init(occupant->self);
						if (!f.IsBuilt() && f.Team() == thisTeam) {
							worker.Build(f.id);
							std::cout << "Worker " << worker.id << " helped build Factory" << f.id << std::endl;
						}
					}
				}
			}
			for (int i = 0; i < 8; i++) {
				bc_Direction currDir = static_cast<bc_Direction>(i);
				if (worker.CanBlueprint(bc_UnitType::Factory, currDir)) {
					worker.Blueprint(bc_UnitType::Factory, currDir);
					std::cout << "Worker " << worker.id << " has blueprinted a Factory" << std::endl;
				}
			}
			break;
		case ROCKET:
			for (int i = 0; i < 8; i++) {
				bc_Direction currDir = static_cast<bc_Direction>(i);
				if (worker.CanBlueprint(bc_UnitType::Rocket, currDir)) {
					worker.Blueprint(bc_UnitType::Rocket, currDir);
				}
			}
			break;
		case REPLICATE:
			for (int i = 0; i < 8; i++) {
				bc_Direction currDir = static_cast<bc_Direction>(i);
				if (worker.CanReplicate(currDir)) {
					worker.Replicate(currDir);
					std::cout << "Worker " << worker.id << " has replaicated" << std::endl;
				}
			}
			break;
		case KARB:
			bool hasHarvested = false;
			if (worker.CanHarvest(bc_Direction::Center)) {
				bc_GameController_harvest(gc.gc, worker.id, bc_Direction::Center);
				std::cout << "Worker " << worker.id << " is harvesting" << std::endl;
				hasHarvested = true;
			}
			// Check if previous action worked
			// OOtherwise contnue
			if (!hasHarvested) {
				for (int i = 0; i < 8; i++) {
					bc_Direction currDir = static_cast<bc_Direction>(i);
					if (worker.CanHarvest(currDir)) {
						bc_GameController_harvest(gc.gc, worker.id, currDir);
						std::cout << "Worker " << worker.id << " is harvesting" << std::endl;
						hasHarvested = true;
						break;
					}
				}
				if (!hasHarvested) {
					std::cout << "Worker could not find Karb. Should now Wander." << std::endl;
					currWorkerPriority = WorkerPriority::EXPLORE;
				}
			}					
			break;
		case EXPLORE:
			bool hasMoved = false;
			auto dir = mapLocation.DirectionTo(wayPoint);
			if (worker.CanMove(dir)) {
				worker.Move(dir);
				std::cout << "Worker " << worker.id << " has moved to " << mapLocation.x << " " << mapLocation.y;
				hasMoved = true;
			}
			for (int i = 1, y = dir+1; i < 8; i++, y++) {
				if(y > 8){ y = 0; }
				dir = static_cast<bc_Direction>(y);
				if (worker.CanMove(dir)) {
					worker.Move(dir);
					std::cout << "Worker " << worker.id << " has moved to " << mapLocation.x << " " << mapLocation.y;
					hasMoved = true;
					break;
				}
			}
			if (!hasMoved) {
				std::cout << "Worker can not move" << std::endl;
			}
			break;
	}
		
	
}
void UpdateKnightEarth(int index) {
	units::Knight knight;
	knight.Init(units[index]->self);
	std::cout << "Updating Knight " << knight.id << std::endl;

	auto location = knight.Loc();
	if (location.IsInSpace()) {
		std::cout << "Knight " << knight.id << " is in space" << std::endl;
		return;
	}
	MapLocation mapLocation = location.ToMapLocation();

	// Check attack;

	bool hasMoved = false;
	auto dir = mapLocation.DirectionTo(wayPoint);
	if (knight.CanMove(dir)) {
		knight.Move(dir);
		std::cout << "Knight " << knight.id << " has moved to " << mapLocation.x << " " << mapLocation.y;
		hasMoved = true;
	}
	if (!hasMoved) {
		for (int i = 1, y = dir + 1; i < 8; i++, y++) {
			if (y > 8) {
				y = 0;
			}
			dir = static_cast<bc_Direction>(y);
			if (knight.CanMove(dir)) {
				knight.Move(dir);
				std::cout << "Knight " << knight.id << " has moved to " << mapLocation.x << " " << mapLocation.y;
				hasMoved = true;
				break;
			}
		}
		if (!hasMoved) {
			std::cout << "Knight can not move" << std::endl;
		}
	}
}
void UpdateRangerEarth(int index) {

}
void UpdateMageEarth(int index) {

}
void UpdateHealerEarth(int index) {
	 
}
void UpdateFactoryEarth(int index){
	if (currFactoryPriority == FactoryPriority::IDLE){ return; }
	units::Factory factory;
	factory.Init(units[index]->self);
	std::cout << "Updating Factory " << factory.id << std::endl;

	if (factory.IsBuilt()) {
		if (factory.CanProduce(currFactoryBuild)) {
			factory.Produce(currFactoryBuild);
			std::cout << "Factory " << factory.id << " is producing UnitType " << factory.id << std::endl;
		}
		auto unitsInGarrison = factory.Garrison();
		auto unitsInGarrisonAmo = unitsInGarrison.size();
		MapLocation mapLocation = factory.Loc().ToMapLocation();
		if (unitsInGarrisonAmo > 0) {
			for (int i = 0; i < 8; i++) {
				bc_Direction currDir = static_cast<bc_Direction>(i);
				MapLocation loc = MapLocation::Neighbor(mapLocation, currDir);
				if (loc.Occupiable()) {
					factory.Unload(currDir);
					std::cout << "Factory " << factory.id << " is Unloading a unit to " << loc.x << ", " << loc.Y() << std::endl;
				}
			}
		}
	}
}
void UpdateRocketEarth(int index) {

}

void UpdateEarth() {
	std::cout << "This bot is on Earth" << std::endl;
	thisTeam = gc.Team();

	// Research...
	// Queue all Research for Worker
	research.Queue(bc_UnitType::Worker);
	research.Queue(bc_UnitType::Worker);
	research.Queue(bc_UnitType::Worker);
	research.Queue(bc_UnitType::Worker);
	std::cout << "Queuing research for Worker 4 times" << std::endl;

	while (true) {
		check_errors();

		round = gc.Round();
		std::cout << "Round: " << round << std::endl;
		check_errors();

		if (round % 5) {
			currWorkerPriority = static_cast<WorkerPriority>(rand() % workerPriorityAmo);
			std::cout << "Worker Priority set to " << currWorkerPriority << std::endl;
			if (currWorkerPriority == WorkerPriority::EXPLORE) {
				wayPoint = MapLocation(gc.Planet, rand() % planetMap.width, rand() % planetMap.height);
			}
		}

		currKarbonite = gc.Karbonite();
		units = gc.Units(bc_Selection::MyTeam);
		auto teamCount = units.size();
		for (int i = 0; i < teamCount; i++) {
			switch (units[i]->type) {
				case  Worker:
					UpdateWorkerEarth(i);
					break;
				case Knight:
					UpdateKnightEarth(i);
					break;
				case Ranger:
					UpdateRangerEarth(i);
					break;
				case Mage:
					UpdateMageEarth(i);
					break;
				case Healer:
					UpdateHealerEarth(i);
					break;
			}
		}


		gc.EndTurn();
	}
}

void UpdateMars() {
	std::cout << "This bot is on Mars" << std::endl;
	while (true) {
		check_errors();

		units = gc.Units(bc_Selection::MyTeam);
		if (units.size() > 0) {

		} else {
			gc.EndTurn();
		}
	}
}

int main()
{
	std::cout << "Player C++ bot starting!" << std::endl;
	srand(0);
	check_errors();

	if (gc.Planet() == bc_Planet::Earth) {
		UpdateEarth();
	} else {
		UpdateMars();
	}

	check_errors();
	return 0;



	//while (true)
	//{
	//	uint32_t round = bc_GameController_round(gc);
	//	std::cout << "Round: "<< round << std::endl;

	//	bc_VecUnit *units = bc_GameController_my_units(gc);

	//	int len = bc_VecUnit_len(units);
	//	for (int i = 0; i < len; i++)
	//	{
	//		bc_Unit *unit = bc_VecUnit_index(units, i);

	//		uint16_t id = bc_Unit_id(unit);
	//		if (bc_GameController_can_move(gc, id, North) && bc_GameController_is_move_ready(gc, id)) 
	//		{
	//			bc_GameController_move_robot(gc, id, North);
	//			check_errors();
	//		}
	//		delete_bc_Unit(unit);

	//	}
	//	delete_bc_VecUnit(units);

	//	bc_GameController_next_turn(gc);
	//}
}