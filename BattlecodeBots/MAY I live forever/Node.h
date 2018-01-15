#ifndef NODE_H
#define NODE_H

class Node
{
public:
	int m_xPos;
	int m_yPos;

	int m_level;
	int m_priority;

	Node(int x, int y, int level, int priority);
	void UpdatePriority(const int& xDest, const int& yDest);
	void NextLevel(const int& direction);
	const int Estimate(const int& xDest, const int& yDest) const;
};

bool operator<(const Node& a, const Node& b);





#endif