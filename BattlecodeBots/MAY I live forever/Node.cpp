#include "Node.h"
#include <math.h>


Node::Node(int x, int y, int level, int priority)
{
	m_xPos = x;
	m_yPos = y;
	m_level = level;
	m_priority = priority;
}

void Node::UpdatePriority(const int & xDest, const int & yDest)
{
	m_priority = m_level + Estimate(xDest, yDest) * 10;
}

void Node::NextLevel(const int & direction)
{
	m_level += (direction % 2 == 0 ? 10 : 14);
}

const int & Node::Estimate(const int & xDest, const int & yDest) const
{
	int xd, yd, d;
	xd = xDest - m_xPos;
	yd = yDest - m_yPos;

	d = static_cast<int>(sqrt(xd*xd + yd * yd));

	return d;
}

bool operator<(const Node & a, const Node & b)
{
	return a.m_priority > b.m_priority;
}
