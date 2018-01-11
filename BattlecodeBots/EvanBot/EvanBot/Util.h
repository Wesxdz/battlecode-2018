#pragma once

namespace Util
{

	//This node class was for pathfinding (when i started i was thinking A* but it now occurs to me that A* is too slow for this)
	class Node
	{
		//Current position
		int xPos, yPos;

		//Total distance already travelled
		int level;

		//level + remaining distance estimates
		int priority;

		//Number of potential directions
		static const int dir = 8; 
	public:
		Node(int xp, int yp, int d, int p);
		

		int GetxPos() const { return xPos; }
		int GetyPos() const { return yPos; }
		int GetLevel() const { return level; }
		int GetPriority() const { return priority; }

		void UpdatePriority(const int & xDest, const int & yDest);

		// i: direction
		void NextLevel(const int & i); 

		// Estimation remaining distance to goal
		const int & estimate(const int & xDest, const int & yDest);
	};


}