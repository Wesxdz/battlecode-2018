#include "Region.h"

#include "bc.h"

#include <iostream>
#include <vector>
#include <algorithm>

#include "GameController.h"
#include "MapUtil.h"
#include "MapLocation.h"
#include "InfluenceMap.h"

std::vector<Region> Region::regions;
//short* Region::depthMap;

//namespace {
//	int width;
//	int height;
//	bc_Planet planetType = bc_Planet::Earth;
//	bc_PlanetMap* planetPtr = GameController::PlanetMap(planetType);
//
//	std::vector<short> openNodes;
//	std::vector<short> gates;
//	short maxDepth = 0;
//	short uniqueID = 0;
//}

Region::~Region() {
	// Delete all Locations in all Regions
	for (auto region : regions) {
		for (auto location : locations) {
			delete_bc_MapLocation(location);
		}
	}
}

void Region::GenRegions() {
	//planetPtr = GameController::PlanetMap(planetType);
	//if (planetType == bc_Planet::Earth) {
	//	width = MapUtil::EARTH_MAP_WIDTH;
	//	height = MapUtil::EARTH_MAP_HEIGHT;
	//} else {
	//	width = MapUtil::MARS_MAP_WIDTH;
	//	height = MapUtil::MARS_MAP_HEIGHT;
	//}
	//gates.clear();
	//openNodes.clear();
	//maxDepth = 0;
	//uniqueID = 0;

	short height = MapUtil::EARTH_MAP_HEIGHT;
	short width = MapUtil::EARTH_MAP_WIDTH;

	InfluenceMap wallMap;
	wallMap.Init(GameController::PlanetMap(bc_Planet::Earth));
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			short ID = y * width + x;

			if (!MapUtil::earthTerrainMap[ID]) {
				wallMap.SetInfluence(x, y, 5.0f, 3);
			}
		}
	}

	wallMap.Print();
	

	// We could use a quad tree... / Hierachical A* 

	/////////////////////////////////////////////////////////////////
	// Construct DepthMap
	//depthMap = new short[width * height];
	//for (int y = 0; y < height; y++) {
	//	for (int x = 0; x < width; x++) {
	//		short ID = y * width + x;

	//		// Add locations that are impassable
	//		if (!MapUtil::earthTerrainMap[ID]) {
	//			openNodes.push_back(ID);
	//			depthMap[ID] = 0;
	//		} else {
	//			depthMap[ID] = SHRT_MAX;
	//		}

	//	}
	//}

	//// Generate Depth Map
	//GenDepthMap(depthMap, MapUtil::earthTerrainMap);

	//std::cout << "Depth Map\n";
	//for (int y = 0; y < height; y++) {
	//	for (int x = 0; x < width; x++) {
	//		short ID = y * width + x;
	//		std::cout << depthMap[ID] << " ";
	//	}
	//	std::cout << "\n";
	//}
	//std::cout << std::endl;

	///////////////////////////////////////////////////////////////////
	// Construct Zone Map or 'Regions'
	//short waterDepth = maxDepth;
	//for (int y = 0; y < height; y++) {
	//	for (int x = 0; x < width; x++) {
	//		short ID = y * width + x;

	//		if (depthMap[ID] == waterDepth) {
	//			// Check for Adjacent tiles
	//			// If adjacent tiles have label, then our ID is that tile's ID
	//			//		Otherwise Create a new Label (uniqueID++)
	//			//		Take on BOTH ID's if we have multiple labels AND mark as gate tile (aka new region)
	//		}
	//	}
	//}

	//for (int i = 0; i < gates.size(); i++) {
		//	FloodFill cluster of connected gate tiles
		//	Remove all tiles not adjacent to wall
		//	Select one tile from each remaining connected cluster
		//	Build gate from two selected tiles
		//	Add gate to list of gates
		//	Remove remaining tiles from cluster
	//}

}
/*

void Region::GenDepthMap(short* depthMap, short* terrainMap) {

	// Add all the points that are unpassable
	while (openNodes.size() > 0) {
		short currentID = openNodes[openNodes.size() - 1];
		openNodes.pop_back();

		short currX = currentID % width;
		short currY = currentID / width;

		short newValue = depthMap[currentID] + 1; // New value for surrounding points
		if(newValue > maxDepth) { maxDepth = newValue; } // Keep track of max depth

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

void Region::GenZoneMap(short ID, short* labelMap) {
	short currX = ID % width;
	short currY = ID / width;

	short currID = 0;


	// UP
	{
		// Check if space is acceptable
		if (currY + 1 < height) {
			// Get new ID
			int upID = (currY + 1) * width + currX;
			if (labelMap[upID]) {

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
		}
	}
}
*/