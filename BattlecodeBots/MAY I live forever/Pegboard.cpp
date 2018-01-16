#include "Pegboard.h"

#include <algorithm>
#include <iterator>

std::map<units::Robot, Force> Pegboard::forces;
std::vector<units::Robot> Pegboard::pegs;
std::map<MapLocation, ForceMove> Pegboard::reserved;
std::vector<units::Robot> Pegboard::hasPushed;

void Pegboard::ApplyForce(units::Robot& robot, bc_Direction direction, float power)
{
	forces[robot] += Force(bc_Direction_dx(direction) * power, bc_Direction_dy(direction) * power);
}

void Pegboard::SolveMovement()
{
	// Get all moveable robots on team
	std::copy_if(std::begin(GlobalData::currRobots), std::end(GlobalData::currRobots), std::back_inserter(pegs), [](units::Robot& robot) {
		return robot.IsMoveReady() && robot.Loc().IsOnMap();
	});
	do {
		// Execute reserved movements
		for (auto& pair : reserved) {
			MapLocation moveTo = pair.first;
			bc_Direction moveDir = pair.second.source.Loc().ToMapLocation().DirectionTo(moveTo);
			if (pair.second.source.CanMove(moveDir)) {
				pair.second.source.Move(moveDir);
				std::remove(pegs.begin(), pegs.end(), pair.second.source);
				std::remove(forces.begin(), forces.end(), pair.second.source);
			}
		}
		reserved.clear();
		for (auto& pair : forces) {
			units::Robot robot = pair.first;
			Force force = pair.second;
			ForceMove move = { robot, force };
			CalculateMovement(move);
		}
	} while (reserved.size() > 0);
	forces.clear();
	pegs.clear();
	hasPushed.clear();
}

bool Pegboard::CalculateMovement(ForceMove& move)
{
	// First check the three directions around the ideal movement
	// If the unit can move there, reserve the move (it might be overriden by another move though)
	MapLocation location = move.source.Loc().ToMapLocation();
	bc_Direction ideal = move.force.Direction();
	MapLocation idealLocation = MapLocation::Neighbor(location, ideal);
	if (move.source.CanMove(ideal)) {
		auto reservation = std::find(reserved.begin(), reserved.end(), idealLocation);
		if (reservation == reserved.end() || reservation->second.force.Power() < move.force.Power()) {
			reserved[idealLocation] = move;
			return true;
		}
	}
	bc_Direction right1 = bc_Direction_rotate_right(ideal);
	MapLocation right1Location = MapLocation::Neighbor(location, right1);
	if (move.source.CanMove(right1)) {
		auto reservation = std::find(reserved.begin(), reserved.end(), right1Location);
		if (reservation == reserved.end() || reservation->second.force.Power() < move.force.Power()) {
			reserved[right1Location] = move;
			return true;
		}
	}
	bc_Direction left1 = bc_Direction_rotate_left(ideal);
	MapLocation left1Location = MapLocation::Neighbor(location, left1);
	if (move.source.CanMove(left1)) {
		auto reservation = std::find(reserved.begin(), reserved.end(), left1Location);
		if (reservation == reserved.end() || reservation->second.force.Power() < move.force.Power()) {
			reserved[left1Location] = move;
			return true;
		}
	}
	// None of those three locations were available for moves :(
	// Let's see if there are units in them and we can push them out of the way in our idealDirection

	auto tired = std::find(hasPushed.begin(), hasPushed.end(), move.source);
	// We don't want to try to push a peg out of the way if we've already done so!
	if (tired != hasPushed.end()) {

		bc_Unit* occupant;
		if (idealLocation.IsValid() && idealLocation.IsOccupied()) {
			occupant = idealLocation.Occupant();
		}
		else if (right1Location.IsValid() && right1Location.IsOccupied()) {
			occupant = right1Location.Occupant();
		}
		else if (left1Location.IsValid() && left1Location.IsOccupied()) {
			occupant = left1Location.Occupant();
		}
		auto peg = std::find(pegs.begin(), pegs.end(), units::Robot(bc_Unit_clone(occupant)));
		delete_bc_Unit(occupant);
		if (peg != pegs.end()) {
			hasPushed.push_back(move.source);
			forces[*peg] += move.force; // AHA, let's add our force to this guy
			return true;
		}
	}

	// Oh dear, we have nothing to do!
	// We're probably trying to move into a bunch of impassable terrain or enemy units
	// Maybe we can move to the side?
	bc_Direction right2 = bc_Direction_rotate_right(right1);
	MapLocation right2Location = MapLocation::Neighbor(location, right2);
	if (move.source.CanMove(right2)) {
		auto reservation = std::find(reserved.begin(), reserved.end(), right2Location);
		if (reservation == reserved.end() || reservation->second.force.Power() < move.force.Power()) {
			reserved[right2Location] = move;
			return true;
		}
	}

	bc_Direction left2 = bc_Direction_rotate_left(left1);
	MapLocation left2Location = MapLocation::Neighbor(location, left2);
	if (move.source.CanMove(left2)) {
		auto reservation = std::find(reserved.begin(), reserved.end(), left2Location);
		if (reservation == reserved.end() || reservation->second.force.Power() < move.force.Power()) {
			reserved[left2Location] = move;
			return true;
		}
	}

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

Force Force::operator+=(const Force & other)
{
	return Force(x + other.x, y + other.y);
}

bc_Direction Force::Direction()
{
	MapLocation origin{ new_bc_MapLocation(Earth, 0, 0) };
	MapLocation end{ new_bc_MapLocation(Earth, static_cast<uint32_t>(x), static_cast<uint32_t>(y)) };
	return origin.DirectionTo(end);
}

float Force::Power()
{
	return x + y;
}
