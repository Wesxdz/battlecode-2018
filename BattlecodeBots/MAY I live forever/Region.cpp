#include "Region.h"

#include "bc.h"

#include <iostream>
#include <vector>
#include <algorithm>

#include "GameController.h"
#include "MapUtil.h"
#include "MapLocation.h"

std::vector<Region> Region::regions;
short* Region::depthMap;

namespace {
	std::vector<short> openNodes;

	int width;
	int height;
	bc_Planet planetType = bc_Planet::Earth;
	bc_PlanetMap* planetPtr = GameController::PlanetMap(planetType);
}

Region::~Region() {
	// Delete all Locations in all Regions
	for (auto region : regions) {
		for (auto location : locations) {
			delete_bc_MapLocation(location);
		}
	}
}

void Region::GenRegions() {
	// Determine chokepoints, add them to regions

	// Get potential location amo of surrounding. For example 3 in Corner case
	// A choke point is a point that limits access between two other points
	//	For example, A chokpoint will have at least 2 valid positions next, otherwise dead end.
	//	Those two points must be seperated by at LEAST one empty space
	// A

	planetPtr = GameController::PlanetMap(planetType);
	if (planetType == bc_Planet::Earth) {
		width = MapUtil::EARTH_MAP_WIDTH;
		height = MapUtil::EARTH_MAP_HEIGHT;
	} else {
		width = MapUtil::MARS_MAP_WIDTH;
		height = MapUtil::MARS_MAP_HEIGHT;
	}


	depthMap = new short[width * height];
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			short ID = y * width + x;

			// Add locations that are impassable
			if (!MapUtil::earthTerrainMap[ID]) {
				openNodes.push_back(ID);
				depthMap[ID] = 0;
			} else {
				depthMap[ID] = SHRT_MAX;
			}

		}
	}

	/*
	if () { // Check if passable
	short upID = -1;
	short upRightID = -1;
	short rightID = -1;
	short downRightID = -1;
	short downID = -1;
	short downLeftID = -1;
	short leftID = -1;
	short upLeftID = -1;

	// Get the corresponding Value, -1 is Off Map, 0 is unPass, 1 is Pass
	{
	// UP
	{
	// Check if space is acceptable
	if (y + 1 < height) {
	// Get Is Passable
	upID = MapUtil::earthTerrainMap[(y + 1) * width + x];
	}
	}

	// Up Right
	{
	// Check if space is acceptable
	if (y + 1 < height && x + 1 < width) {
	// Get Is Passable
	upRightID = MapUtil::earthTerrainMap[(y + 1) * width + (x + 1)];
	}

	}

	// Right
	{
	// Check if space is acceptable
	if (x + 1 < width) {
	// Get Is Passable
	int rightID = MapUtil::earthTerrainMap[(y)* width + (x + 1)];
	}

	}

	// Down Right
	{
	// Check if space is acceptable
	if (y - 1 > -1 && x + 1 < width) {
	// Get Is Passable
	int downRightID = MapUtil::earthTerrainMap[(y - 1) * width + (x + 1)];

	}

	}

	// Down
	{
	// Check if space is acceptable
	if (y - 1 > -1) {
	// Get Is Passable
	int downID = MapUtil::earthTerrainMap[(y - 1) * width + x];

	}
	}

	// Down Left
	{
	// Check if space is acceptable
	if (y - 1 > -1 && x - 1 > -1) {
	// Get Is Passable
	int downLeftID = MapUtil::earthTerrainMap[(y - 1) * width + (x - 1)];

	}

	}

	// Left
	{
	// Check if space is acceptable
	if (x - 1 > -1) {
	// Get Is Passable
	leftID = MapUtil::earthTerrainMap[(y)* width + (x - 1)];

	}

	}

	// Up Left
	{
	// Check if space is acceptable
	if (y + 1 < height && x - 1 > -1) {
	// Get Is Passable
	upLeftID = MapUtil::earthTerrainMap[(y + 1) * width + (x - 1)];
	}
	}
	}


	*/

	// Create  Depth map. Keep track of Depth and Direction
    // Go through each tile. If it has a depth of 1 (right next to wall) search in opposite direcion
    // Keep searching for you encouter a tile with a different direction or you reach a threshold.

//BWTA regions are obtained from a pruned Voronoi diagram on walkable terrain

// Influence Maps

// Voronoi Diagram

// Voronoi Point - X, Y, Region*
// Voronoi Region - Point*, Edge[]
// Voronoi Edge - PointA*, PointB*, Dist, (x1, y1, x2, y2 - Visualization)

	GenDepthMap(depthMap, MapUtil::earthTerrainMap);

	std::cout << "Depth Map\n";
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			short ID = y * width + x;
			std::cout << depthMap[ID] << " ";
		}
		std::cout << "\n";
	}
	std::cout << std::endl;

		//	1	1	0	0	0	0
		//	1	1	0	0	0	0
		//	0	0	1	0	0	0 <- Choke point
		//	0	0	0	1	1	1
		//	0	0	0	1	1	1

		//	0	0	0	0	0	0
		//	0	0	0	0	0	0
		//	0	0	1	0	0	0 <- Choke point
		//	1	1	0	1	1	1
		//	1	1	0	1	1	1

		//	0	0	0	0	0	0
		//	0	0	0	0	0	0
		//	0	0	1	0	0	0 
		//	1	1	1	1	1	1 <- Choke point
		//	1	1	0	1	1	1
		// In this case, the second one is a chokepoint
		// Additionally, all surrounding blocks are chokepoints until they are evaluated to be not chokepoints

		//	0	0	0	0	0	0
		//	0	0	0	0	0	0
		//	0	0	1	0	0	0 <- Not Choke point
		//	1	1	1	1	1	1
		//	1	1	1	1	1	1

		//	1	1	0	1	1	0
		//	1	1	0	1	1	0
		//	0	0	1	0	0	0 <- Choke point
		//	1	1	0	1	1	1
		//	1	1	0	1	1	1

		//	1	1	0	1	1	0
		//	1	1	0	1	1	0
		//	0	1	1	0	0	0 <- Choke point
		//	1	1	0	1	1	1
		//	1	1	0	1	1	1

		//	1	1	0	1	1	0
		//	1	1	1	1	1	0
		//	0	1	1	1	0	0 <- ??? Choke point ???
		//	1	1	1	1	1	1
		//	1	1	0	1	1	1

		//	1	1	1	1	1	1
		//	0	0	1	1	0	0
		//	0	0	1	1	0	0 
		//	1	1	1	1	1	1
		//	1	1	1	1	1	1
		//	North = 0,
		//	Northeast = 1,
		//	East = 2,
		//	Southeast = 3,
		//	South = 4,
		//	Southwest = 5,
		//	West = 6,
		//	Northwest = 7,
		//	Center = 8,
		//	3	3	4	5	5	6
		//	0	0	4	5	0	0 <- ChokePoint?
		//	0	0	4	5	0	0 
		//	3	3	4	5	5	5
		//	2	2	8	6	6	6
		// In this case, the '4' is pointed at by 3 directions. It also has 2 holes next to it.
		// If it were to be the destination, this would not work.

		// From there, we should determine that the 2 '5's and '4' next to it are also ChokePoints
		// How?
		// I point down
		// Top points to Me
		// TopRight points to Me
		// TopLeft points to Me
		// Right points to Similar Direction
		// BotRight points to Similar Direction
		// BotLeft and Left do not point

		//Here's an idea. Let's say I construct a flow diagram to a random position. I then iterate through the flow diagram and check if any position has multiple directions towards it and empty squares around it.
		// Degree 2 nodes

}

void Region::GenDepthMap(short* depthMap, short* terrainMap) {

	// Add all the points that are unpassable
	while (openNodes.size() > 0) {
		short currentID = openNodes[openNodes.size() - 1];
		openNodes.pop_back();

		short currX = currentID % width;
		short currY = currentID / width;

		short newValue = depthMap[currentID] + 1; // New value for surrounding points

		// UP
		{
			// Check if space is acceptable
			if (currY + 1 < height) {
				// Get new ID
				int upID = (currY + 1) * width + currX;

				// Check if new value is better or if it is passable
				if (depthMap[upID] > newValue && terrainMap[upID] > 0) {
					// Add to list if it hasnt
					auto iterPtr = std::find(openNodes.begin(), openNodes.end(), upID);
					if (iterPtr == openNodes.end()) {
						openNodes.push_back(upID);
					}
					// Set value
					depthMap[upID] = newValue;
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
				if (depthMap[upID] > newValue && terrainMap[upID] > 0) {
					// Add to list if it hasnt
					auto iterPtr = std::find(openNodes.begin(), openNodes.end(), upID);
					if (iterPtr == openNodes.end()) {
						openNodes.push_back(upID);
					}
					// Set value
					depthMap[upID] = newValue;
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
				if (depthMap[upID] > newValue && terrainMap[upID] > 0) {
					// Add to list if it hasnt
					auto iterPtr = std::find(openNodes.begin(), openNodes.end(), upID);
					if (iterPtr == openNodes.end()) {
						openNodes.push_back(upID);
					}
					// Set value
					depthMap[upID] = newValue;
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
				if (depthMap[upID] > newValue && terrainMap[upID] > 0) {
					// Add to list if it hasnt
					auto iterPtr = std::find(openNodes.begin(), openNodes.end(), upID);
					if (iterPtr == openNodes.end()) {
						openNodes.push_back(upID);
					}
					// Set value
					depthMap[upID] = newValue;
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
				if (depthMap[upID] > newValue && terrainMap[upID] > 0) {
					// Add to list if it hasnt
					auto iterPtr = std::find(openNodes.begin(), openNodes.end(), upID);
					if (iterPtr == openNodes.end()) {
						openNodes.push_back(upID);
					}
					// Set value
					depthMap[upID] = newValue;
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
				if (depthMap[upID] > newValue && terrainMap[upID] > 0) {
					// Add to list if it hasnt
					auto iterPtr = std::find(openNodes.begin(), openNodes.end(), upID);
					if (iterPtr == openNodes.end()) {
						openNodes.push_back(upID);
					}
					// Set value
					depthMap[upID] = newValue;
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
				if (depthMap[upID] > newValue && terrainMap[upID] > 0) {
					// Add to list if it hasnt
					auto iterPtr = std::find(openNodes.begin(), openNodes.end(), upID);
					if (iterPtr == openNodes.end()) {
						openNodes.push_back(upID);
					}
					// Set value
					depthMap[upID] = newValue;
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
				if (depthMap[upID] > newValue && terrainMap[upID] > 0) {
					// Add to list if it hasnt
					auto iterPtr = std::find(openNodes.begin(), openNodes.end(), upID);
					if (iterPtr == openNodes.end()) {
						openNodes.push_back(upID);
					}
					// Set value
					depthMap[upID] = newValue;
					//std::cout << "ID " << upID << " is now " << newValue << std::endl;
				}
			}
		}

	}

}