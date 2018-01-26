#include "Pathfind.h"

#include "Constants.h"
#include "PlanetMap.h"
#include "GameController.h"

#include "MapUtil.h"
#include "Utility.h"
#include "Section.h"

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <algorithm>

void FlowChart::Destory() {
	delete[] pointsMap;
	delete[] directionMap;
}

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
std::vector<short> openNodes;
bc_Planet planet;
short width;
short height;
short* terrainMap = nullptr;
bool hasInit = false;

void Pathfind::Init() {
	planet = GameController::Planet();
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

	// Init
	if (!hasInit) {
		Init();
	}
	MapLocation mapLoc = robot.Loc().ToMapLocation();
	Section* destSection = Section::Get(planet, destX, destY);
	Section* currSection = Section::Get(mapLoc);
	CHECK_ERRORS();

	// Can't path from one section to another.
	if (currSection != destSection) { return false; }

	short destXY = width * destY + destX; // Get Map Pos Total

	auto chartPtr = flowCharts.find(destXY); // Check if We already have a flowchart for this dest
	FlowChart* flowChart = nullptr;

	// Generate Flowpath if it doesnt exist.
	if (chartPtr == flowCharts.end()) {
		GenerateFlowPath(destSection, destX, destY);
		flowChart = &flowCharts[destXY];
	} else {
		flowChart = &chartPtr->second;
	}

	short currX = static_cast<short>(mapLoc.X());
	short currY = static_cast<short>(mapLoc.Y());
	short currXY = width * currY + currX; // Robot Position ID 

	auto dir = flowChart->directionMap[currXY]; // Fuzzy move in that direction
	return Pathfind::MoveFuzzy(robot, dir);
}

int Pathfind::GetFuzzyFlowTurns(int sourceX, int sourceY, int destX, int destY) {
	return 0;

	// Init
	if (!hasInit) {
		Init();
	}
	Section* destSection = Section::Get(planet, destX, destY);
	Section* currSection = Section::Get(planet, sourceX, sourceY);
	CHECK_ERRORS();

	// Can't path from one section to another.
	if (currSection != destSection) { return false; }

	short destXY = width * destY + destX; // Get Map Pos Total

	auto chartPtr = flowCharts.find(destXY); // Check if We already have a flowchart for this dest
	FlowChart* flowChart = nullptr;

	// Generate Flowpath if it doesnt exist.
	if (chartPtr == flowCharts.end()) {
		GenerateFlowPath(destSection, destX, destY);
		flowChart = &flowCharts[destXY];
	} else {
		flowChart = &chartPtr->second;
	}

	short currXY = width * sourceY + sourceX; // Robot Position ID 
	return flowChart->pointsMap[currXY];
}

// Should work, pretty Efficient
// Maybe optimize on Memory?
FlowChart Pathfind::CreateFlowChart(std::vector<bc_MapLocation*> destinations) {
	// Assume valid Input
	FlowChart flowChart;

	// Init
	if (!hasInit) {
		Init();
	}

	if (destinations.size() == 0) {
		return FlowChart();
	}

	// Get Position and Section
	int destX =  bc_MapLocation_x_get(destinations[0]);
	int destY =  bc_MapLocation_x_get(destinations[0]);
	Section* destSection = Section::Get(planet, destX, destY);

	// Get Amount in Section and Amount in Destination
	int sectAmo = static_cast<int>(destSection->locations.size());
	int destAmo = static_cast<int>(destinations.size());
	int mapAmo = width * height;

	// Create the Maps that will store the data
	flowChart.pointsMap = new short[mapAmo];
	flowChart.directionMap = new bc_Direction[mapAmo];
	for (short y = 0; y < height; y++) {
		for (short x = 0; x < width; x++) {
			short ID = y * width + x;

			flowChart.pointsMap[ID] = SHRT_MAX;
			flowChart.directionMap[ID] = bc_Direction::Center;
		}
	}

	// Init Destination
	int destID = destY * width + destX;
	flowChart.pointsMap[destID] = 0;
	flowChart.directionMap[destID] = bc_Direction::Center; // Dont Need
	for (int i = 1; i < destAmo; i++) {
		int destX =  bc_MapLocation_x_get(destinations[i]);
		int destY =  bc_MapLocation_x_get(destinations[i]);
		int destID = destY * width + destX;

		flowChart.pointsMap[destID] = 0;
		flowChart.directionMap[destID] = bc_Direction::Center; // Dont Need
	}

	// Create Depth / Influence
	short currDepth = 0;
	bool notChanged = true;
	while (notChanged) {
		notChanged = false;

		for (short y = 0; y < height; y++) {
			for (short x = 0; x < width; x++) {
				short ID = y * width + x;

				if (flowChart.pointsMap[ID] == currDepth) {

					for (int d = 0; d < 8; d++) {
						bc_Direction currDir = static_cast<bc_Direction>(d);

						int dirX = bc_Direction_dx(currDir) + x;
						int dirY = bc_Direction_dy(currDir) + y;

						if(dirX < 0 || dirX >= width || dirY < 0 || dirY >= height){
							//std::cout << "Dir is " << dirX << ", " << dirY << std::endl;
							continue;
						} // Validate
						int dirID = (dirY) * width + (dirX);

						// Is Valid and 
						if (terrainMap[dirID] && flowChart.pointsMap[dirID] > (currDepth + 1)) {

							//std::cout << "Setting " << dirID << " to " << currDepth + 1 << " from " << flowChart.pointsMap[dirID] << std::endl;
							flowChart.pointsMap[dirID] = currDepth + 1;
							flowChart.directionMap[dirID] = bc_Direction_opposite(currDir);
							notChanged = true;
						}
					}
				}
			}
		}

		currDepth++;
		//std::cout << "Current Depth is now " << currDepth << std::endl;
	}

	//std::cout << "Points Map" << std::endl;
	//for (int y = 0; y < height; y++) {
	//	for (int x = 0; x < width; x++) {
	//		std::cout << std::setw(4) << flowChart.pointsMap[y * width + x] << " ";
	//	}
	//	std::cout << std::endl;
	//}
	//std::cout << "\n\n" << std::endl;

	//std::cout << "Direction Map" << std::endl;
	//for (int y = 0; y < height; y++) {
	//	for (int x = 0; x < width; x++) {
	//		std::cout << flowChart.directionMap[y * width + x] << " ";
	//	}
	//	std::cout << std::endl;
	//}
	//std::cout << "\n\n" << std::endl;

	return flowChart;
}
FlowChart Pathfind::CreateFlowChart(std::vector<MapLocation> destinations) {
	// Assume valid Input
	FlowChart flowChart;

	// Init
	if (!hasInit) {
		Init();
	}

	if (destinations.size() == 0) {
		return FlowChart();
	}

	// Get Position and Section
	int destX = static_cast<int>(destinations[0].X());
	int destY = static_cast<int>(destinations[0].Y());
	Section* destSection = Section::Get(planet, destX, destY);

	// Get Amount in Section and Amount in Destination
	int sectAmo = static_cast<int>(destSection->locations.size());
	int destAmo = static_cast<int>(destinations.size());
	int mapAmo = width * height;

	// Create the Maps that will store the data
	flowChart.pointsMap = new short[mapAmo];
	flowChart.directionMap = new bc_Direction[mapAmo];
	for (short y = 0; y < height; y++) {
		for (short x = 0; x < width; x++) {
			short ID = y * width + x;

			flowChart.pointsMap[ID] = SHRT_MAX;
			flowChart.directionMap[ID] = bc_Direction::Center;
		}
	}

	// Init Destination
	int destID = destY * width + destX;
	flowChart.pointsMap[destID] = 0;
	flowChart.directionMap[destID] = bc_Direction::Center; // Dont Need
	for (int i = 1; i < destAmo; i++) {
		int destX = static_cast<int>(destinations[i].X());
		int destY = static_cast<int>(destinations[i].Y());
		int destID = destY * width + destX;

		flowChart.pointsMap[destID] = 0;
		flowChart.directionMap[destID] = bc_Direction::Center; // Dont Need
	}

	// Create Depth / Influence
	short currDepth = 0;
	bool notChanged = true;
	while (notChanged) {
		notChanged = false;

		for (short y = 0; y < height; y++) {
			for (short x = 0; x < width; x++) {
				short ID = y * width + x;

				if (flowChart.pointsMap[ID] == currDepth) {

					for (int d = 0; d < 8; d++) {
						bc_Direction currDir = static_cast<bc_Direction>(d);

						int dirX = bc_Direction_dx(currDir) + x;
						int dirY = bc_Direction_dy(currDir) + y;

						if(dirX < 0 || dirX >= width || dirY < 0 || dirY >= height){
							//std::cout << "Dir is " << dirX << ", " << dirY << std::endl;
							continue;
						} // Validate
						int dirID = (dirY) * width + (dirX);

						// Is Valid and 
						if (terrainMap[dirID] && flowChart.pointsMap[dirID] > (currDepth + 1)) {

							//std::cout << "Setting " << dirID << " to " << currDepth + 1 << " from " << flowChart.pointsMap[dirID] << std::endl;
							flowChart.pointsMap[dirID] = currDepth + 1;
							flowChart.directionMap[dirID] = bc_Direction_opposite(currDir);
							notChanged = true;
						}
					}
				}
			}
		}

		currDepth++;
		//std::cout << "Current Depth is now " << currDepth << std::endl;
	}

	//std::cout << "Points Map" << std::endl;
	//for (int y = 0; y < height; y++) {
	//	for (int x = 0; x < width; x++) {
	//		std::cout << std::setw(4) << flowChart.pointsMap[y * width + x] << " ";
	//	}
	//	std::cout << std::endl;
	//}
	//std::cout << "\n\n" << std::endl;

	//std::cout << "Direction Map" << std::endl;
	//for (int y = 0; y < height; y++) {
	//	for (int x = 0; x < width; x++) {
	//		std::cout << flowChart.directionMap[y * width + x] << " ";
	//	}
	//	std::cout << std::endl;
	//}
	//std::cout << "\n\n" << std::endl;

	return flowChart;
}

bool Pathfind::MoveFuzzyFlow(units::Robot& robot, MapLocation& destination) {
	int destX = destination.X();
	int destY= destination.Y();
	return MoveFuzzyFlow(robot, destX, destY);
}

int Pathfind::GetFuzzyFlowTurns(MapLocation& origin, MapLocation& destination) {
	short destY = destination.Y();
	short destX = destination.X();

	short sourceX = origin.X();
	short sourceY = origin.Y();

	return GetFuzzyFlowTurns(sourceX, sourceY, destX, destY);
}


void Pathfind::GenerateFlowPath(Section* section, short destX, short destY) {
	short destXY = width * destY + destX;
	auto flowChart = &flowCharts[destXY];

	//// Create the Maps that will store the data
	flowChart->pointsMap = new short[width * height];
	flowChart->directionMap = new bc_Direction[width * height];
	/// NOTE* can probably optimize Memory usage of Flow Map

	// Initialize Point Map
	openNodes.clear();
	for (short y = 0; y < height; y++) {
		for (short x = 0; x < width; x++) {
			short ID = y * width + x;
			flowChart->pointsMap[ID] = SHRT_MAX;
		}
	}

	//// Generate a map based off the terrain and destination
	GenerateFlowPathPoints(terrainMap, flowChart->pointsMap, section, destX, destY);

	 //std::cout << "Points Map" << std::endl;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			 //std::cout << std::setw(6) << flowChart->pointsMap[y * width + x] << " ";
		}
		 std::cout << std::endl;
	}
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
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			 //std::cout << flowChart->directionMap[y * width + x] << " ";
		}
		 //std::cout << std::endl;
	}
	 //std::cout << "\n\n" << std::endl;
}


void Pathfind::GenerateFlowPathPoints(short* terrainMap, short* pointsMap, Section* section, short destX, short destY) {
	
	short targetID = destY * width + destX;
	pointsMap[targetID] = 0;

	openNodes.push_back(targetID);

	while (openNodes.size() > 0) {
		short currentID = openNodes[openNodes.size() - 1];
		openNodes.pop_back();

		short currX = currentID % width;
		short currY = currentID / width;

		short newValue = pointsMap[currentID] + 1; // New value for surrounding points

		// UP
		{
			// Check if in same Section
			Section* newSection = Section::Get(planet, currX, currY + 1);
			if (newSection == section) {
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
				}
			}
		}

		// Up Right
		{
			// Check if in same Section
			Section* newSection = Section::Get(planet, currX + 1, currY + 1);
			if (newSection == section) {
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
				}
			}
		}

		// Right
		{
			// Check if in same Section
			Section* newSection = Section::Get(planet, currX + 1, currY);
			if (newSection == section) {
				// Get new ID
				short upID = (currY) * width + (currX + 1);

				// Check if new value is better or if it is passable
				if (pointsMap[upID] > newValue && terrainMap[upID] > 0) {
					// Add to list if it hasnt
					auto iterPtr = std::find(openNodes.begin(), openNodes.end(), upID);
					if (iterPtr == openNodes.end()) {
						openNodes.push_back(upID);
					}
					// Set value
					pointsMap[upID] = newValue;
				}
			}
		}

		// Down Right
		{
			// Check if in same Section
			Section* newSection = Section::Get(planet, currX + 1, currY - 1);
			if (newSection == section) {
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
				}
			}
		}

		// Down
		{
			// Check if in same Section
			Section* newSection = Section::Get(planet, currX, currY - 1);
			if (newSection == section) {
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
				}
			}
		}

		// Down Left
		{
			// Check if in same Section
			Section* newSection = Section::Get(planet, currX - 1, currY - 1);
			if (newSection == section) {
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
				}
			}
		}

		// Left
		{
			// Check if in same Section
			Section* newSection = Section::Get(planet, currX - 1, currY);
			if (newSection == section) {
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
				}
			}
		}

		// Up Left
		{
			// Check if in same Section
			Section* newSection = Section::Get(planet, currX - 1, currY + 1);
			if (newSection == section) {
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
				}
			}
		}

	}

}

bc_Direction Pathfind::GenerateFlowPathDirection(short* pointsMap,
	short sourceX, short sourceY, short destX, short destY) {

	short ID = sourceY * width + sourceX;
	if (pointsMap[ID] == 0) {
		return bc_Direction::Center;
	}

	short lowestValue = SHRT_MAX;
	bc_Direction mapDir = Utility::DirectionTo(sourceX, sourceY, destX, destY);

	// Loop through all directions to get the most optimal one
	for (short i = 0, x = mapDir; i < 8; i++, x++) {
		if (x > 7) {
			x = 0;
		}
		// Get new Direction
		bc_Direction currDir = static_cast<bc_Direction>(x);
		short currX = bc_Direction_dx(currDir);
		short currY = bc_Direction_dy(currDir); /// Optimize these out...

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

// To Generate a flow chart
// Create a grid / array of all the points in the map with cost of traversing
//		Impassable, Difficult, etc.
// Create a grid / array of all the points revolving around goal
//		Start from Dest, evaluate surrounding tiles, keep lowest value
// Create a grid / array of all the direction towards the goal
//		Iterate around the given Location and find the smallest value. Move there.

int FlowChart::GetIndex(MapLocation & location)
{
	return location.X() + location.Y() * width;
}
