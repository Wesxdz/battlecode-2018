#ifndef TEAM_ARRAY_H
#define TEAM_ARRAY_H

#include "bc.h"

// Need DELAY
// Need LENGTH

class TeamArray {
	/// Writes the value at the index of this planet's team array.
	/// 
	/// * ArrayOutOfBounds - the index of the array is out of bounds. It must be within [0, COMMUNICATION_ARRAY_LENGTH).
	void Write(uintptr_t index, const char* message, int length);

	/// Writes the value at the index of this planet's team array.
	/// 
	/// * ArrayOutOfBounds - the index of the array is out of bounds. It must be within [0, COMMUNICATION_ARRAY_LENGTH).
	void Write(uintptr_t index, int32_t value);

	/// Gets a read-only version of this planet's team array. If the given planet is different from the planet of the player, reads the version of the planet's team array from COMMUNICATION_DELAY rounds prior.
	bc_Veci32*  Read(bc_Planet type);

	/// Gets a read-only version of this planet's team array. If the given planet is different from the planet of the player, reads the version of the planet's team array from COMMUNICATION_DELAY rounds prior.
	char* ReadString(bc_Planet type);

	void DeleteMessage(bc_Veci32* ptr);
	void DeleteMessage(const char* ptr);

};

#endif // !TEAM_ARRAY_H

