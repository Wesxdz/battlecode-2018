#include "Pathfind.h"

#include "Constants.h"
#include "PlanetMap.h"
#include "GameController.h"

#include "MapUtil.h"
#include "Utility.h"

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <algorithm>

std::vector<MapLocation> Pathfind::Neighbors(MapLocation & start)
{
	std::vector<MapLocation> neighbors;
	bc_PlanetMap* planetMap = GameController::PlanetMap(start.Planet());
	for (bc_Direction direction : constants::directions_adjacent) {
		MapLocation neighbor = MapLocation::Neighbor(start, direction);
		if (neighbor.IsValid()) neighbors.push_back(neighbor);
	}
	return neighbors;
}

std::vector<MapLocation> Pathfind::Moveable(MapLocation& start)
{
	std::vector<MapLocation> neighbors;
	bc_PlanetMap* planetMap = GameController::PlanetMap(start.Planet());
	for (bc_Direction direction : constants::directions_adjacent) {
		MapLocation neighbor = MapLocation::Neighbor(start, direction);
		if (neighbor.IsValid()) {
			if (neighbor.IsVisible()) {
				if (neighbor.IsOccupiable()) {
					neighbors.push_back(neighbor);
				}
			}
			else if (neighbor.IsPassable()) {
				neighbors.push_back(neighbor);
			}
		}
	}
	return neighbors;
}

MapLocation Pathfind::PickRandom(std::vector<MapLocation>& locations)
{
	if (locations.size() == 0) return nullptr;
	return locations[rand() % locations.size()];
}

bool Pathfind::MoveRandom(units::Robot& robot)
{
	if (!robot.IsMoveReady()) return false;
	std::array<bc_Direction, 8> adjacent = constants::directions_adjacent;
	std::random_shuffle(std::begin(adjacent), std::end(adjacent));
	for (bc_Direction direction : adjacent) {
		if (robot.CanMove(direction)) {
			robot.Move(direction);
			return true;
		}
	}
	return false;
}

bool Pathfind::MoveFuzzy(units::Robot & robot, bc_Direction direction)
{
	if (!robot.IsMoveReady() || direction == Center) return false;
	if (robot.CanMove(direction)) {
		robot.Move(direction);
		return true;
	}
	bc_Direction right1 = bc_Direction_rotate_right(direction);
	if (robot.CanMove(right1)) {
		robot.Move(right1);
		return true;
	}
	bc_Direction left1 = bc_Direction_rotate_left(direction);
	if (robot.CanMove(left1)) {
		robot.Move(left1);
		return true;
	}
	bc_Direction right2 = bc_Direction_rotate_right(right1);
	if (robot.CanMove(right2)) {
		robot.Move(right2);
		return true;
	}
	bc_Direction left2 = bc_Direction_rotate_left(left1);
	if (robot.CanMove(left2)) {
		robot.Move(left2);
		return true;
	}
	return false;
}

// We don't use this in the other mehtods...
std::map<int, FlowChart> Pathfind::flowCharts;
short width;
short height;
short* terrainMap = nullptr;
bool hasInit = false;

void Pathfind::Init() {
	bc_Planet planet = GameController::Planet();
	if (planet == bc_Planet::Earth) {
		width = static_cast<short>(MapUtil::EARTH_MAP_WIDTH);
		height = static_cast<short>(MapUtil::EARTH_MAP_HEIGHT);
		terrainMap = MapUtil::earthTerrainMap;
	} else {
		width = static_cast<short>(MapUtil::MARS_MAP_WIDTH);
		height = static_cast<short>(MapUtil::MARS_MAP_HEIGHT);
		terrainMap = MapUtil::marsTerrainMap;
	}
	hasInit = true;
}

bool Pathfind::MoveFuzzyFlow(units::Robot& robot, int destX, int destY) {
	if (robot.MovementHeat() > 9) { return false; } // Can't move
	
	if (!hasInit) {
		Init();
	}

	short destXY = width * destY + destX; // Get Map Pos Total

	auto chartPtr = flowCharts.find(destXY); // Check if We already have a flowchart for this dest
	FlowChart* flowChart = nullptr;

	// Generate Flowpath if it doesnt exist.
	if (chartPtr == flowCharts.end()) {
		GenerateFlowPath(destX, destY);
		flowChart = &flowCharts[destXY];
	} else {
		flowChart = &chartPtr->second;
	}

	auto mapLoc = robot.Loc().ToMapLocation(); // Get Curr Map Pos Total
	short currX = static_cast<short>(mapLoc.X());
	short currY = static_cast<short>(mapLoc.Y());

	short currXY = width * currY + currX; // Robot Position ID 

	auto dir = flowChart->directionMap[currXY]; // Fuzzy move in that direction
	return Pathfind::MoveFuzzy(robot, dir);
	
	//if (chartPtr != flowCharts.end()) {

	//} else {
	//	

	//	
	//	//Move
	//	auto mapLoc = robot.Loc().ToMapLocation(); // Get Curr Map Pos Total
	//	short currX = static_cast<short>(mapLoc.X());
	//	short currY = static_cast<short>(mapLoc.Y());
	//	short currXY = width * currY + currX; // Robot Position ID 
	//	
	//	auto dir = flowChart->directionMap[currXY]; // Fuzzy move in that direction
	//	return Pathfind::MoveFuzzy(robot, dir);
	//}
	
	return false;
}

bool Pathfind::MoveFuzzyFlow(units::Robot& robot, MapLocation& destination) {
	int destX = destination.X();
	int destY= destination.Y();
	return MoveFuzzyFlow(robot, destX, destY);
}

int Pathfind::GetFuzzyFlowTurns(MapLocation& origin, MapLocation& destination) {
	if (!hasInit) {
		Init();
	}

	short destY = destination.Y();
	short destX = destination.X();

	short destXY = width * destY + destX; // Get Map Pos Total

	auto chartPtr = flowCharts.find(destXY); // Check if We already have a flowchart for this dest
	FlowChart* flowChart = nullptr;

	// Generate Flowpath if it doesnt exist.
	if (chartPtr == flowCharts.end()) {
		GenerateFlowPath(destX, destY);
		flowChart = &flowCharts[destXY];
	} else {
		flowChart = &chartPtr->second;
	}

	short currX = static_cast<short>(destination.X());
	short currY = static_cast<short>(destination.Y());

	short currXY = width * currY + currX; // Robot Position ID 

	//auto dir = flowChart->directionMap[currXY]; // Fuzzy move in that direction
	//return Pathfind::MoveFuzzy(robot, dir);

	return flowChart->pointsMap[currXY];
}

void Pathfind::GenerateFlowPath(short destX, short destY) {
	short destXY = width * destY + destX;
	auto flowChart = &flowCharts[destXY];

	//// Create the Maps that will store the data
	flowChart->pointsMap = new short[width * height];
	flowChart->directionMap = new bc_Direction[width * height];

	// Initialize Point Map
	for (short y = 0; y < height; y++) {
		for (short x = 0; x < width; x++) {
			short ID = y * width + x;
			flowChart->pointsMap[ID] = SHRT_MAX;
		}
	}

	//// Generate a map based off the terrain and destination
	GenerateFlowPathPoints(terrainMap, flowChart->pointsMap, destX, destY);

	//std::cout << "Points Map" << std::endl;
	//for (int y = 0; y < height; y++) {
	//	for (int x = 0; x < width; x++) {
	//		std::cout << std::setw(6) << flowChart->pointsMap[y * width + x] << " ";
	//	}
	//	std::cout << std::endl;
	//}
	//std::cout << std::setw(0) << "\n\n" << std::endl;

	// Generate a map of directions based off surrounding points...
	for (short y = 0; y < height; y++) {
		for (short x = 0; x < width; x++) {
			short ID = y * width + x;
			if (flowChart->pointsMap[ID] != SHRT_MAX) {
				flowChart->directionMap[ID] = GenerateFlowPathDirection(flowChart->pointsMap, x, y, destX, destY);
			} else {
				flowChart->directionMap[ID] = bc_Direction::Center;
			}

		}
	}

	//std::cout << "Direction Map" << std::endl;
	//std::cout << "Destination is " << destX << ", " << destY << std::endl;
	//for (int y = 0; y < height; y++) {
	//	for (int x = 0; x < width; x++) {
	//		std::cout << flowChart->directionMap[y * width + x] << " ";
	//	}
	//	std::cout << std::endl;
	//}
	//std::cout << "\n\n" << std::endl;
}


void Pathfind::GenerateFlowPathPoints(short* terrainMap, short* pointsMap, short destX, short destY) {
	
	short targetID = destY * width + destX;
	pointsMap[targetID] = 0;

	std::vector<short> openNodes;
	openNodes.push_back(targetID);

	while (openNodes.size() > 0) {
		short currentID = openNodes[openNodes.size() - 1];
		openNodes.pop_back();

		short currX = currentID % width;
		short currY = currentID / width;

		short newValue = pointsMap[currentID] + 1; // New value for surrounding points

		// UP
		{
			// Check if space is acceptable
			if (currY + 1 < height) {
				// Get new ID
				int upID = (currY + 1) * width + currX;

				// Check if new value is better or if it is passable
				if (pointsMap[upID] > newValue && terrainMap[upID] > 0) {
					// Add to list if it hasnt
					auto iterPtr = std::find(openNodes.begin(), openNodes.end(), upID);
					if (iterPtr == openNodes.end()) {
						openNodes.push_back(upID);
					}
					// Set value
					pointsMap[upID] = newValue;
					//std::cout << "ID " << upID << " is now " << newValue << std::endl;
				}
			}

		}

		// Up Right
		{
			// Check if space is acceptable
			if (currY + 1 < height && currX + 1 < width) {
				// Get new ID
				short upID = (currY + 1) * width + (currX + 1);

				// Check if new value is better or if it is passable
				if (pointsMap[upID] > newValue && terrainMap[upID] > 0) {
					// Add to list if it hasnt
					auto iterPtr = std::find(openNodes.begin(), openNodes.end(), upID);
					if (iterPtr == openNodes.end()) {
						openNodes.push_back(upID);
					}
					// Set value
					pointsMap[upID] = newValue;
					//std::cout << "ID " << upID << " is now " << newValue << std::endl;
				}
			}

		}

		// Right
		{
			// Check if space is acceptable
			if (currX + 1 < width) {
				// Get new ID
				short upID = (currY)* width + (currX + 1);

				// Check if new value is better or if it is passable
				if (pointsMap[upID] > newValue && terrainMap[upID] > 0) {
					// Add to list if it hasnt
					auto iterPtr = std::find(openNodes.begin(), openNodes.end(), upID);
					if (iterPtr == openNodes.end()) {
						openNodes.push_back(upID);
					}
					// Set value
					pointsMap[upID] = newValue;
					//std::cout << "ID " << upID << " is now " << newValue << std::endl;
				}
			}

		}

		// Down Right
		{
			// Check if space is acceptable
			if (currY - 1 > -1 && currX + 1 < width) {
				// Get new ID
				short upID = (currY - 1) * width + (currX + 1);

				// Check if new value is better or if it is passable
				if (pointsMap[upID] > newValue && terrainMap[upID] > 0) {
					// Add to list if it hasnt
					auto iterPtr = std::find(openNodes.begin(), openNodes.end(), upID);
					if (iterPtr == openNodes.end()) {
						openNodes.push_back(upID);
					}
					// Set value
					pointsMap[upID] = newValue;
					//std::cout << "ID " << upID << " is now " << newValue << std::endl;
				}
			}

		}

		// Down
		{
			// Check if space is acceptable
			if (currY - 1 > -1) {
				// Get new ID
				short upID = (currY - 1) * width + currX;

				 //Check if new value is better or if it is passable
				if (pointsMap[upID] > newValue && terrainMap[upID] > 0) {
					// Add to list if it hasnt
					auto iterPtr = std::find(openNodes.begin(), openNodes.end(), upID);
					if (iterPtr == openNodes.end()) {
						openNodes.push_back(upID);
					}
					// Set value
					pointsMap[upID] = newValue;
					//std::cout << "ID " << upID << " is now " << newValue << std::endl;
				}
			}
		}

		// Down Left
		{
			// Check if space is acceptable
			if (currY - 1 > -1 && currX - 1 > -1) {
				// Get new ID
				short upID = (currY - 1) * width + (currX - 1);

				// Check if new value is better or if it is passable
				if (pointsMap[upID] > newValue && terrainMap[upID] > 0) {
					// Add to list if it hasnt
					auto iterPtr = std::find(openNodes.begin(), openNodes.end(), upID);
					if (iterPtr == openNodes.end()) {
						openNodes.push_back(upID);
					}
					// Set value
					pointsMap[upID] = newValue;
					//std::cout << "ID " << upID << " is now " << newValue << std::endl;
				}
			}

		}

		// Left
		{
			// Check if space is acceptable
			if (currX - 1 > -1) {
				// Get new ID
				short upID = (currY)* width + (currX - 1);

				// Check if new value is better or if it is passable
				if (pointsMap[upID] > newValue && terrainMap[upID] > 0) {
					// Add to list if it hasnt
					auto iterPtr = std::find(openNodes.begin(), openNodes.end(), upID);
					if (iterPtr == openNodes.end()) {
						openNodes.push_back(upID);
					}
					// Set value
					pointsMap[upID] = newValue;
					//std::cout << "ID " << upID << " is now " << newValue << std::endl;
				}
			}

		}

		// Up Left
		{
			// Check if space is acceptable
			if (currY + 1 < height && currX - 1 > -1) {
				// Get new ID
				short upID = (currY + 1) * width + (currX - 1);

				// Check if new value is better or if it is passable
				if (pointsMap[upID] > newValue && terrainMap[upID] > 0) {
					// Add to list if it hasnt
					auto iterPtr = std::find(openNodes.begin(), openNodes.end(), upID);
					if (iterPtr == openNodes.end()) {
						openNodes.push_back(upID);
					}
					// Set value
					pointsMap[upID] = newValue;
					//std::cout << "ID " << upID << " is now " << newValue << std::endl;
				}
			}
		}

	}

}

bc_Direction Pathfind::GenerateFlowPathDirection(short* pointsMap,
	short sourceX, short sourceY, short destX, short destY) {

	short lowestValue = SHRT_MAX;
	bc_Direction mapDir = Utility::DirectionTo(sourceX, sourceY, destX, destY);

	// If center, then source == dest
	if(mapDir == bc_Direction::Center){ 
		return bc_Direction::Center; 
	} 

	// Loop through all directions to get the most optimal one
	for (short i = 0, x = mapDir; i < 8; i++, x++) {
		if (x > 7) {
			x = 0;
		}
		// Get new Direction
		bc_Direction currDir = static_cast<bc_Direction>(x);
		short currX = bc_Direction_dx(currDir);
		short currY = bc_Direction_dy(currDir);

		// Check for invalid direction
		short newY = sourceY + currY;
		short newX = sourceX + currX;
		if (newY < height && newY > -1 && newX < width && newY > -1) {
			// Get new ID
			short dirID = newY * width + newX;

			// If lower, new Direction
			if (pointsMap[dirID] < lowestValue) {

				mapDir = currDir;
				lowestValue = pointsMap[dirID];
			}
		}
	}
	return mapDir;
}
// NOTE: Optimize by making our own Rotate / X and Y functions for directions?

// To Generate a flow chart
// Create a grid / array of all the points in the map with cost of traversing
//		Impassable, Difficult, etc.
// Create a grid / array of all the points revolving around goal
//		Start from Dest, evaluate surrounding tiles, keep lowest value
// Create a grid / array of all the direction towards the goal
//		Iterate around the given Location and find the smallest value. Move there.