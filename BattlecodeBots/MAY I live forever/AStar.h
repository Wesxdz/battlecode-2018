#ifndef ASTAR_H
#define ASTAR_H
#include <vector>





class AStar
{
public:
	//Initializes a new AStar object, leaving this blank will select the entire map, passing in boundaries will limit pathing to those coordinates.
	AStar(const int& xMinBounds = -1, const int& yMinBounds = -1, const int& xMaxBounds = -1, const int& yMaxBounds = -1);
	
	//Returns a vector of bc_Direction from the starting position that will lead to the end, it also populates the path variable.
	std::vector<bc_Direction> PathFind(const int& xStart, const int& yStart, const int& xFinish, const int& yFinish, bool earth);
	
	int m_mapHSize;
	int m_mapVSize;

	bool m_pathPopulated = false;
	//MapLocations of the path (this may be removed if it takes up too much space.)
	std::vector<MapLocation> m_path;
	std::vector<bc_Direction> m_pathDirections;



private:
	static const int dir = 8;
	const int dx[dir] = { 1, 1, 0, -1, -1, -1, 0, 1 };
	const int dy[dir] = { 0, 1, 1, 1, 0, -1, -1, -1 };

	bool m_hasBounds = false;
	//min and max for bounds
	int m_minBX, m_minBY, m_maxBX, m_maxBY;

	//This is called during pathing, you don't need to call it.
	void CalculatePathFromDirections(int xStart, int yStart);
};

#endif