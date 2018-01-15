#include "AStar.h"
#include <queue>
#include "Node.h"
#include "GameController.h"

AStar::AStar(const int& xMinBounds, const int& yMinBounds, const int& xMaxBounds, const int& yMaxBounds)
{
	if (xMinBounds < 0 || yMinBounds < 0 || xMaxBounds < 0 || yMaxBounds < 0)
	{
		m_hasBounds = false;
	}
	else
	{
		m_minBX = xMinBounds;
		m_minBY = yMinBounds;
		m_maxBX = xMaxBounds;
		m_maxBY = yMaxBounds;
	}
}

std::vector<bc_Direction> AStar::PathFind(const int & xStart, const int & yStart, const int & xFinish, const int & yFinish, bool earth)
{
	PlanetMap* gameMap = GameMap::OurPlanet();
	if (m_hasBounds)
	{
		m_mapHSize = m_maxBX - m_minBX;
		m_mapVSize = m_maxBY - m_minBY;
	}
	else
	{
		m_mapHSize = gameMap->Width();
		m_mapVSize = gameMap->Height();
		m_minBX = 0;
		m_minBY = 0;
		m_maxBX = m_mapHSize;
		m_maxBY = m_mapVSize;
	}



	int* map = new int[m_mapHSize * m_mapVSize];
	int* closedNodesMap = new int[m_mapHSize * m_mapVSize];
	int* openNodesMap = new int[m_mapHSize * m_mapVSize];
	int* directionMap = new int[m_mapHSize * m_mapVSize];

	std::priority_queue<Node> pq[2];
	int pqIndex = 0;
	Node* hNode;
	Node* vNode;
	char c;
	int x, y, i, j, xdx, ydy;

	for (y = 0; y < m_mapVSize; y++)
	{
		for (x = 0; x < m_mapHSize; x++)
		{
			closedNodesMap[x * m_mapVSize + y];
			openNodesMap[x * m_mapVSize + y];
		}
	}

	hNode = new Node(xStart, yStart, 0, 0);
	hNode->UpdatePriority(xFinish, yFinish);
	pq[pqIndex].push(*hNode);
	openNodesMap[x * m_mapVSize + y] = hNode->m_priority;

	while (!pq[pqIndex].empty())
	{
		hNode = new Node(pq[pqIndex].top().m_xPos, pq[pqIndex].top().m_yPos, pq[pqIndex].top().m_level, pq[pqIndex].top().m_priority);

		x = hNode->m_xPos;
		y = hNode->m_yPos;
		pq[pqIndex].pop();
		openNodesMap[x * m_mapVSize + y] = 0;
		closedNodesMap[x * m_mapVSize + y] = 1;

		if (x == xFinish && y == yFinish)
		{
			std::string path = "";

			while (!(x == xStart && y == yStart))
			{
				j = directionMap[x * m_mapVSize + y];
				c = '0' + (j + dir / 2) % dir;
				path = c + path;
				x += dx[j];
				y += dy[j];
			}

			delete hNode;
			while (!pq[pqIndex].empty())
			{
				pq[pqIndex].pop();
			}
			
			std::vector<bc_Direction> returnPath;
			for (int plen = 0; plen < path.length(); plen++)
			{
				char pChar = path.at(i);
				int pInt = atoi(&pChar);
				switch (pInt)
				{
				case 0: returnPath.push_back(East); break;
				case 1: returnPath.push_back(Southeast); break;
				case 2: returnPath.push_back(South); break;
				case 3: returnPath.push_back(Southwest); break;
				case 4: returnPath.push_back(West); break;
				case 5: returnPath.push_back(Northwest); break;
				case 6: returnPath.push_back(North); break;
				case 7: returnPath.push_back(Northeast); break;
				}
			}
			m_pathDirections = returnPath;

			CalculatePathFromDirections(xStart, yStart);
			return returnPath;
		}

		for (i = 0; i < dir; i++)
		{
			xdx = x + dx[i];
			ydy = y + dy[i];

			if (!(xdx<0 || xdx > m_mapHSize - 1 || ydy<0 || ydy > m_mapVSize - 1 || map[xdx * m_mapVSize + ydy] == 1 || closedNodesMap[xdx * m_mapVSize + ydy] == 1))
			{
				vNode = new Node(xdx, ydy, hNode->m_level, hNode->m_priority);

				vNode->NextLevel(i);
				vNode->UpdatePriority(xFinish, yFinish);

				if (openNodesMap[xdx * m_mapVSize + ydy] == 0)
				{
					openNodesMap[xdx * m_mapVSize + ydy] = vNode->m_priority;

					pq[pqIndex].push(*vNode);

					directionMap[xdx * m_mapVSize + ydy] = (i + dir / 2) % dir;

				}
				else if (openNodesMap[xdx * m_mapVSize + ydy] > vNode->m_priority)
				{
					openNodesMap[xdx * m_mapVSize + ydy] = vNode->m_priority;
					directionMap[xdx * m_mapVSize + ydy] = (i + dir / 2) % dir;

					while (!(pq[pqIndex].top().m_xPos == xdx &&
						pq[pqIndex].top().m_yPos == ydy))
					{
						pq[1 - pqIndex].push(pq[pqIndex].top());
						pq[pqIndex].pop();
					}
					pq[pqIndex].pop();

					if (pq[pqIndex].size()>pq[1 - pqIndex].size()) pqIndex = 1 - pqIndex;
					while (!pq[pqIndex].empty())
					{
						pq[1 - pqIndex].push(pq[pqIndex].top());
						pq[pqIndex].pop();
					}
					pqIndex = 1 - pqIndex;
					pq[pqIndex].push(*vNode);
				}
				else
				{
					delete vNode;
				}
			}
		}
		delete hNode;



	}
	delete [] directionMap;
	delete [] openNodesMap;
	delete [] closedNodesMap;
	delete [] map;
	delete [] gameMap;
	//No route found
	return std::vector<bc_Direction>();
}

void AStar::CalculatePathFromDirections(int xStart, int yStart)
{
	int x, y;
	x = xStart;
	y = yStart;

	for (bc_Direction dir : m_pathDirections)
	{

		switch (dir)
		{
		case North: y++; break;
		case Northeast: y++; x++; break;
		case East: x++; break;
		case Southeast: y--; x++; break;
		case South: y--; break;
		case Southwest: y--; x--; break;
		case West: x--; break;
		case Northwest: y++; x--; break;
		}

		MapLocation ml = MapLocation(GameController::Planet(), x, y);
		m_path.push_back(ml);
	}
}
