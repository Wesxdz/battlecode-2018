#ifndef GAMECONTROLLER_H 
#define GAMECONTROLLER_H

#include "bc.h"

#include <vector>
#include <memory>

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
	@error NoSuchUnit - the unit does not exist (inside the vision range).
	@return The single unit with this ID.
	*/
	static std::shared_ptr<units::Unit> Unit(uint16_t id);
	static std::shared_ptr<units::Unit> Unit(bc_Unit* unit);
	/*
	@return Units within selection criteria
	*/
	static std::vector<std::shared_ptr<units::Unit>> Units(bc_Selection selection);
	/*
	Functions you may wish to use with this
	bc_GameController_sense_nearby_units
	bc_GameController_sense_nearby_units_by_team
	bc_GameController_sense_nearby_units_by_type
	@return A vector of your desired unit type (usually units::Unit)
	*/
	template <class T>
	static std::vector<std::shared_ptr<T>> Wrap(bc_VecUnit* bcUnits);

	/*template <class T>
	static std::vector<std::shared_ptr<T>> Subset(std::vector<std::shared_ptr<units::Unit>> units);*/

	template <class T, class V>
	static std::shared_ptr<T> Convert(std::shared_ptr<V> original);

};

template<class T>
inline std::vector<std::shared_ptr<T>> GameController::Wrap(bc_VecUnit * bcUnits)
{
	auto units = std::vector<std::shared_ptr<T>>();
	for (uintptr_t i = 0; i < bc_VecUnit_len(bcUnits); i++) {
		units.push_back(std::dynamic_pointer_cast<T>(GameController::Unit(bc_VecUnit_index(bcUnits, i))));
	}
	delete_bc_VecUnit(bcUnits);
	return units;
}

template<class T, class V>
inline std::shared_ptr<T> GameController::Convert(std::shared_ptr<V> original)
{
	return std::static_pointer_cast<T>(original);
}

#endif