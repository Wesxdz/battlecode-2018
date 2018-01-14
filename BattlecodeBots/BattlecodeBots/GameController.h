#ifndef GAMECONTROLLER_H 
#define GAMECONTROLLER_H

#include "bc.h"

#include <vector>

namespace units {
	class Unit;
}

enum bc_Selection {
	// All the units within the vision range, in no particular order. Does not include units in space.
	Visible,
	// All the units on your team. Does not include units in space.
	MyTeam,
	// All the units of this team that are in space.You cannot see units on the other team that are in space.
	Space,
};

/*
Should be instantiated before using C++ wrapper
*/
class GameController
{
public:
	static bc_GameController* gc;
public:
	GameController();
	~GameController();
	/*
	Send the moves from the current turn and wait for the next turn.
	*/
	static void EndTurn();
	/*
	A round consists of a turn from each team on each planet.
	@return The current round, starting at round 1 and up to ROUND_LIMIT rounds.
	*/
	static uint32_t Round();
	/*
	@return The current planet
	*/
	static bc_Planet Planet();
	/*
	@return The planet map of this player
	*/
	static bc_PlanetMap* PlanetMap();
	/*
	Research Engineering Division (Red)
	The Branched Logistics Union of Electronicists (Blue)
	Red takes the first turn
	@return The team whose turn it is
	*/
	static bc_Team Team();
	/*
	@return The karbonite in the team's resource pool.
	*/
	static uint32_t Karbonite();
	/*
	@return Units within selection criteria
	*/
	static std::vector<units::Unit> Units(bc_Selection selection);

};

#endif