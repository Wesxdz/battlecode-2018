#ifndef PEGBOARD
#define PEGBOARD

#include "bc.h"
#include <vector>
#include "Robot.h"
#include <map>

// Essentially, a vector
struct Force {
	Force();
	Force(float px, float py);
	~Force();
	Force operator+(const Force& other);
	float x;
	float y;
	bc_Direction Direction();
	float Power();
};

struct ForceMove 
{
	ForceMove();
	ForceMove(uint16_t id, Force& pForce);
	uint16_t source;
	Force force;
};

/*
The Pegboard resolves movement conflicts and allows units to slip past each other or funnel through cooridors
Consider units pegs, units that cannot move are fixed to the board
Pegs fall (move greedy) towards force
Units that cannot move this turn are counted as fixed, but all other units move "together"
A unit does not necessarily have to generate force on itself (you can push or pull other units or groups of units)
*/
class Pegboard
{
public:
	static std::map<uint16_t, Force> forces;
	static void ApplyForce(uint16_t id, bc_Direction direction, float power = 1);
	static void SolveMovement();
private:
	/*
	Reserves a move or pushes another peg
	@return Was action taken
	*/
	static bool CalculateMovement(ForceMove& move);
	// All robots that are ready to move and can be pushed
	static std::vector<uint16_t> pegs;
	// Potential moves, moves to the same location with a greater force will reserve this location
	static std::map<MapLocation, ForceMove> reserved;
	// The pegs which have already exerted their force on another peg
	static std::vector<uint16_t> hasPushed;
};

#endif

