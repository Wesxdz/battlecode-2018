#include "Pegboard.h"

#include <algorithm>
#include <iterator>
#include "Utility.h"
#include <iostream>
#include <math.h>

#include "GameController.h"

std::map<uint16_t, Force> Pegboard::forces;
std::vector<uint16_t> Pegboard::pegs;
std::map<MapLocation, ForceMove> Pegboard::reserved;
std::vector<uint16_t> Pegboard::hasPushed;

void Pegboard::ApplyForce(uint16_t id, bc_Direction direction, float power)
{
	Force f{ (float)bc_Direction_dx(direction) * power, (float)bc_Direction_dy(direction) * power};
	//std::cout << "Create force " << f.x << ", " << f.y << "\n";
	auto alreadyForce = forces.find(id);
	if (alreadyForce != forces.end()) {
		//std::cout << "Already has force!\n";
		Force f2 = f + forces[id];
		forces[id] = f2;
	}
	else {
		forces[id] = f;
	}
	//std::cout << "Updated force " << forces[id].x << ", " << forces[id].y << "\n";
	//units::Robot robot(bc_GameController_unit(GameController::gc, id));
}

void Pegboard::SolveMovement()
{
	// Get all moveable robots on team
	// TODO Use GlobalData::currRobots
	auto currRobots = GameController::Wrap<units::Robot>(bc_GameController_my_units(GameController::gc));
	for (auto& robot : currRobots) {
		if (Utility::IsRobot(robot.type) && robot.IsMoveReady() && robot.Loc().IsOnMap()) {
			pegs.push_back(robot.id);
		}
	}
	bool actionTaken = false;
	do {
		actionTaken = false;
		// Execute reserved movements
		for (auto& move : reserved) {
			uint16_t id = move.second.source;
			MapLocation moveTo = move.first;
			units::Robot moving{ bc_GameController_unit(GameController::gc, id) };
			CHECK_ERRORS()
			bc_Direction moveDir = moving.Loc().ToMapLocation().DirectionTo(moveTo);
			if (moving.CanMove(moveDir)) {
				//std::cout << "Moving with force " << move.second.force.x << ", " << move.second.force.y << "\n";
				moving.Move(moveDir);
				pegs.erase(std::remove(pegs.begin(), pegs.end(), id)); // The robot being moved MUST be a peg
				forces.erase(forces.find(id));
			}
		}
		reserved.clear();
		for (auto& pair : forces) {
			units::Robot robot{ bc_GameController_unit(GameController::gc, pair.first) };
			CHECK_ERRORS()
			//std::cout << "Considering Force " << pair.second.x << ", " << pair.second.y << "\n";
			Force force = pair.second;
			ForceMove move = { robot.id, force };
			CHECK_ERRORS()
			//std::cout << "Considering ForceMove " << move.force.x << ", " << move.force.y << "\n";
			if (CalculateMovement(move)) {
				CHECK_ERRORS()
				actionTaken = true;
			}
		}
	} while (actionTaken);
	forces.clear();
	pegs.clear();
	hasPushed.clear();
}

bool Pegboard::CalculateMovement(ForceMove& move)
{
	units::Robot moving{ bc_GameController_unit(GameController::gc, move.source) };
	CHECK_ERRORS()
	// First check the three directions around the ideal movement
	// If the unit can move there, reserve the move (it might be overriden by another move though)
	MapLocation location = moving.Loc().ToMapLocation();
	CHECK_ERRORS()
	bc_Direction ideal = move.force.Direction();
	MapLocation idealLocation = MapLocation::Neighbor(location, ideal);
	//std::cout << (bool)idealLocation.IsValid() << "is ideal location valid\n";
	bc_Direction right1 = bc_Direction_rotate_right(ideal);
	MapLocation right1Location = MapLocation::Neighbor(location, right1);
	bc_Direction left1 = bc_Direction_rotate_left(ideal);
	MapLocation left1Location = MapLocation::Neighbor(location, left1);
	 // None of those three locations were available for moves :(
	 // Let's see if there are units in them and we can push them out of the way in our idealDirection
	CHECK_ERRORS()
	// We don't want to try to push a peg out of the way if we've already done so!
	auto test = std::find(hasPushed.begin(), hasPushed.end(), moving.id);
	if (test == hasPushed.end()) {
		if (idealLocation.IsValid() && idealLocation.IsOccupied()) {
			std::cout << "OCCUPIED\n";
			units::Unit occupant{ idealLocation.Occupant() };
			auto peg = std::find(pegs.begin(), pegs.end(), occupant.id);
			if (peg != pegs.end()) {
				std::cout << "TRYING TO PUSH\n";
				hasPushed.push_back(moving.id);
				forces[*peg] = forces[*peg] + move.force; // AHA, let's add our force to this guy
				return true;
			}
		}
		else if (right1Location.IsValid() && right1Location.IsOccupied()) {
			bc_Unit* occupant = right1Location.Occupant();
			auto peg = std::find(pegs.begin(), pegs.end(), bc_Unit_id(occupant));
			if (peg != pegs.end()) {
				hasPushed.push_back(moving.id);
				Force pushForce = forces[*peg] + move.force;
				return true;

			}
		}
		else if (left1Location.IsValid() && left1Location.IsOccupied()) {
			bc_Unit* occupant = left1Location.Occupant();
			auto peg = std::find(pegs.begin(), pegs.end(), bc_Unit_id(occupant));
			delete_bc_Unit(occupant);
			if (peg != pegs.end()) {
				hasPushed.push_back(moving.id);
				forces[*peg] = forces[*peg] + move.force; 
				return true;

			}
		}
	}
	CHECK_ERRORS()
	if (moving.CanMove(ideal)) {
		auto reservation = reserved.find(bc_MapLocation_clone(idealLocation.self));
		if (reservation == reserved.end()) {
			reserved[idealLocation] = move;
			return true;
		}
	}
	CHECK_ERRORS()
	//if (moving.CanMove(left1)) {
	//	auto reservation = reserved.find(bc_MapLocation_clone(left1Location.self));
	//	if (reservation == reserved.end()) {
	//		reserved[left1Location] = move;
	//		return true;
	//	}
	//}
	//if (moving.CanMove(right1)) {
	//	auto reservation = reserved.find(bc_MapLocation_clone(right1Location.self));
	//	if (reservation == reserved.end()) {
	//		reserved[right1Location] = move;
	//		return true;
	//	}
	//}
	//CHECK_ERRORS()

	//// Oh dear, we have nothing to do!
	//// We're probably trying to move into a bunch of impassable terrain or enemy units
	//// Maybe we can move to the side?
	//bc_Direction right2 = bc_Direction_rotate_right(right1);
	//MapLocation right2Location = MapLocation::Neighbor(location, right2);
	//if (moving.CanMove(right2)) {
	//	auto reservation = reserved.find(bc_MapLocation_clone(right2Location.self));
	//	if (reservation == reserved.end()) {
	//		reserved[right2Location] = move;
	//		return true;
	//	}
	//}

	//bc_Direction left2 = bc_Direction_rotate_left(left1);
	//MapLocation left2Location = MapLocation::Neighbor(location, left2);
	//if (moving.CanMove(left2)) {
	//	auto reservation = reserved.find(bc_MapLocation_clone(left2Location.self));
	//	if (reservation == reserved.end()) {
	//		reserved[left2Location] = move;
	//		return true;
	//	}
	//}
	return false;
}

Force::Force()
{
}

Force::Force(float px, float py)
{
	x = px;
	y = py;
}

Force::~Force()
{
}

Force Force::operator+(const Force& other)
{
	return Force(x + other.x, y + other.y);
}

bc_Direction Force::Direction()
{
	MapLocation origin{ new_bc_MapLocation(Earth, 0, 0) };
	MapLocation end{ new_bc_MapLocation(Earth, static_cast<int32_t>(std::round(x)), static_cast<int32_t>(std::round(y))) };
	CHECK_ERRORS()
	return origin.DirectionTo(end);
}

float Force::Power()
{
	return x + y;
}

ForceMove::ForceMove()
{
}

ForceMove::ForceMove(uint16_t id, Force & pForce)
{
	source = id;
	force = pForce;
}
