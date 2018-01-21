#include "Region.h"

#include "bc.h"

#include "GameController.h"
#include "MapUtil.h"
#include "MapLocation.h"

std::vector<Region> Region::regions;


Region::~Region() {
	// Delete all Locations in all Regions
	for (auto region : regions) {
		for (auto location : locations) {
			delete_bc_MapLocation(location);
		}
	}
}

void Region::GenRegions(std::vector<bc_MapLocation*>& passables) {
	// Determine chokepoints, add them to regions

	// Get potential location amo of surrounding. For example 3 in Corner case
	// A choke point is a point that limits access between two other points
	//	For example, A chokpoint will have at least 2 valid positions next, otherwise dead end.
	//	Those two points must be seperated by at LEAST one empty space
	// A

	int width;
	int height;
	bc_Planet planetType = bc_Planet::Earth;
	bc_PlanetMap* planetPtr = GameController::PlanetMap(planetType);
	if (planetType == bc_Planet::Earth) {
		width = MapUtil::EARTH_MAP_WIDTH;
		height = MapUtil::EARTH_MAP_HEIGHT;
	} else {
		width = MapUtil::MARS_MAP_WIDTH;
		height = MapUtil::MARS_MAP_HEIGHT;
	}


	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			short ID = y * width + x;

			if (MapUtil::earthTerrainMap[ID]) { // Check if passable
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



			}
		}
	}

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


	// Determine regions

	//std::list<std::shared_ptr<Section>> sections;
	//for (auto passsableLocation : passables) {
	//	auto sectionToJoin = std::find_if(std::begin(sections), std::end(sections), [&passsableLocation](std::shared_ptr<Section> section) {
	//		for (auto location : section->locations) {
	//			if (bc_MapLocation_is_adjacent_to(passsableLocation, location)) {
	//				return true; // MapLocation is in section
	//			}
	//		}
	//		return false; // MapLocation is the first tile of another section
	//	});
	//	if (sectionToJoin != std::end(sections)) {
	//		(*sectionToJoin)->locations.push_back(passsableLocation);
	//	}
	//	else {
	//		auto newSection = std::make_shared<Section>();
	//		newSection->locations.push_back(passsableLocation);
	//		sections.push_back(newSection);
	//	}
	//}
	//return sections;
}