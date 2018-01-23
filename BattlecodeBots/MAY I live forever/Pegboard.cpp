#include "Pegboard.h"

#include <algorithm>
#include <iterator>
#include "Utility.h"
#include <iostream>
#include <math.h>

#include "GameController.h"

void Pegboard::ApplyForce(uint16_t id, bc_Direction direction, float power)
{
	Force f{ (float)bc_Direction_dx(direction) * power, (float)bc_Direction_dy(direction) * power};
	auto alreadyForce = forces.find(id);
	if (alreadyForce != forces.end()) {
		Force f2 = f + forces[id];
		forces[id] = f2;
	}
	else {
		forces[id] = f;
	}
}

void Pegboard::SolveMovement()
{
	// Get all moveable robots on team
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
		for (auto& pair : forces) {
			units::Robot robot{ bc_GameController_unit(GameController::gc, pair.first) };
			Force force = pair.second;
			ForceMove move = { robot.id, force };
			if (CalculateMovement(move)) {
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
	if (!moving.IsMoveReady()) return false;
	// First check the three directions around the ideal movement
	// If the unit can move there, reserve the move (it might be overriden by another move though)
	MapLocation location = moving.Loc().ToMapLocation();
	bc_Direction ideal = move.force.Direction();
	MapLocation idealLocation = MapLocation::Neighbor(location, ideal);
	//std::cout << (bool)idealLocation.IsValid() << "is ideal location valid\n";
	bc_Direction right1 = bc_Direction_rotate_right(ideal);
	MapLocation right1Location = MapLocation::Neighbor(location, right1);
	bc_Direction left1 = bc_Direction_rotate_left(ideal);
	MapLocation left1Location = MapLocation::Neighbor(location, left1);
	 // None of those three locations were available for moves :(
	 // Let's see if there are units in them and we can push them out of the way in our idealDirection

	// We don't want to try to push a peg out of the way if we've already done so!
	auto test = std::find(hasPushed.begin(), hasPushed.end(), moving.id);
	if (test == hasPushed.end()) {
		if (idealLocation.IsValid() && idealLocation.IsOccupied()) {
			units::Robot occupant{ idealLocation.Occupant() };
			auto peg = std::find(pegs.begin(), pegs.end(), occupant.id);
			if (peg != pegs.end()) {
				hasPushed.push_back(moving.id);
				forces[*peg] = forces[*peg] + move.force; // AHA, let's add our force to this guy
				return true;
			}
		}
		else if (right1Location.IsValid() && right1Location.IsOccupied()) {
			bc_Unit* occupant = right1Location.Occupant();
			auto peg = std::find(pegs.begin(), pegs.end(), bc_Unit_id(occupant));
			delete_bc_Unit(occupant);
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
	if (moving.CanMove(ideal)) {
		moving.Move(ideal);
		forces.erase(moving.id);
		return true;
	}
	if (moving.CanMove(left1)) {
		moving.Move(left1);
		forces.erase(moving.id);
		return true;
	}
	if (moving.CanMove(right1)) {
		moving.Move(right1);
		forces.erase(moving.id);
		return true;
	}

	//// Oh dear, we have nothing to do!
	//// We're probably trying to move into a bunch of impassable terrain or enemy units
	//// Maybe we can move to the side?
	bc_Direction right2 = bc_Direction_rotate_right(right1);
	MapLocation right2Location = MapLocation::Neighbor(location, right2);
	if (moving.CanMove(right2)) {
		moving.Move(right2);
		forces.erase(moving.id);
		return true;
	}

	bc_Direction left2 = bc_Direction_rotate_left(left1);
	MapLocation left2Location = MapLocation::Neighbor(location, left2);
	if (moving.CanMove(left2)) {
		moving.Move(left2);
		forces.erase(moving.id);
		return true;
	}
	return false;
}

int Pegboard::Key(MapLocation location)
{
	return location.Planet() * 3000 + location.Y() * 51 + location.X();
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
	CHECK_ERRORS();
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
