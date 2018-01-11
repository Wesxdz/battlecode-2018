#include <iostream>
#include <random>
#include "bc.h"

#include "GameController.h"
#include "PlanetMap.h"
#include "Unit.h"
#include "Worker.h"
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
bc_UnitType currFactoryBuild = bc_UnitType::Ranger;

void UpdateWorkerEarth(int index) {
	units::Worker worker;
	worker.Init(units[index]->self);
	std::cout << "Updating Worker " << worker.id;

	auto location = worker.Loc();

	if (location.OnPlanet(bc_Planet::Earth)) {
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
							}
						}
					}
				}
				for (int i = 0; i < 8; i++) {
					bc_Direction currDir = static_cast<bc_Direction>(i);
					if (worker.CanBlueprint(bc_UnitType::Factory, currDir)) {
						worker.Blueprint(bc_UnitType::Factory, currDir);
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
					}
				}
				break;
			case KARB:
				bool hasHarvested = false;
				if (worker.CanHarvest(bc_Direction::Center)) {
					bc_GameController_harvest(gc.gc, worker.id, bc_Direction::Center);
					hasHarvested = true;
				}
				for (int i = 0; i < 8; i++) {
					bc_Direction currDir = static_cast<bc_Direction>(i);
					if (worker.CanHarvest(bc_Direction::Center)) {
						bc_GameController_harvest(gc.gc, worker.id, bc_Direction::Center);
						hasHarvested = true;
					}
				}
				if (!hasHarvested) {
					std::cout << "Worker could not find Karb. Should now Wander.";
					currWorkerPriority = WorkerPriority::EXPLORE;
				}
				break;
			case EXPLORE:
				bool hasMoved = false;
				auto dir = mapLocation.DirectionTo(wayPoint);
				if (worker.CanMove(dir)) {
					worker.Move(dir);
					hasMoved = true;
				}
				for (int i = 1, y = dir+1; i < 8; i++, y++) {
					if(y > 8){ y = 0; }
					dir = static_cast<bc_Direction>(y);
					if (worker.CanMove(dir)) {
						worker.Move(dir);
						hasMoved = true;
						break;
					}
				}
				if (!hasMoved) {
					std::cout << "Worker can not move";
				}
				break;
		}
		
	}
}
void UpdateKnightEarth(int index) {

}
void UpdateRangerEarth(int index) {

}
void UpdateMageEarth(int index) {

}
void UpdateHealerEarth(int index) {
	 
}
void UpdateFactoryEarth(int index){
	units::Factory factory;
	factory.Init(units[index]->self);
	std::cout << "Updating Factory " << factory.id;

	if (currFactoryPriority == FactoryPriority::IDLE || !factory.IsBuilt()) {
		return;
	} else {
		if (factory.CanProduce(currFactoryBuild)) {
			factory.Produce(currFactoryBuild);
		}
		if(factory.Garrison)
	}
}
void UpdateRocketEarth(int index) {

}

void UpdateEarth() {
	std::cout << "This bot is on Earth\n";
	thisTeam = gc.Team();

	while (true) {
		round = gc.Round();
		std::cout << "Round: " << round;

		// Research...
		units::Unit::r

		if (round % 5) {
			currWorkerPriority = static_cast<WorkerPriority>(rand() % workerPriorityAmo);
			std::cout << "Worker Priority set to " << currWorkerPriority;
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
	std::cout << "This bot is on Mars\n";
	while (true) {
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