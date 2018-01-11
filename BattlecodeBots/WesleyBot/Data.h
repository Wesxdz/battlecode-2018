#ifndef DATA_H
#define DATA_H

#include "bc.h"

/*
Calculates data useful for decision making
*/
class Data
{
public:
	// The number of tiles on a map
	uint32_t mapSize;
	// The amount of Karbonite on the map at the beginning of the game
	uint32_t initialKarbonite;
	// The number of passible tiles
	uint32_t passibleSize;

public:
	Data();
	~Data();
};

#endif

