#include "Util.h"
#include <limits>>

Util::Node::Node(int xp, int yp, int d, int p)
{
	xPos = xp; yPos = yp; level = d; priority = p;
}

void Util::Node::UpdatePriority(const int & xDest, const int & yDest)
{
	priority = level + estimate(xDest, yDest) * 10; //A* is not something I think we'll use?
}

void Util::Node::NextLevel(const int & i)
{
	level += (dir == 8 ? (i % 2 == 0 ? 10 : 14) : 10);
}

const int & Util::Node::estimate(const int & xDest, const int & yDest)
{
	static int xd, yd, d;
	xd = xDest - xPos;
	yd = yDest - yPos;

	d = static_cast<int>(sqrt(xd*xd + yd * yd));

	return(d);
}
