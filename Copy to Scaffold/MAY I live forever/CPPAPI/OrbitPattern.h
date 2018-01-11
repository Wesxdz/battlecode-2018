#pragma once

#include "bc.h"

/*
The duration of flight is determined by a sinusoidal function y=a*sin(bx)+c where the x-axis is the round number of takeoff and the y-axis is the flight duration.
The time delay is guaranteed to be in the range [50, 200], inclusive.
*/
class OrbitPattern
{
private:
	bc_OrbitPattern* self;
public:
	OrbitPattern();
	~OrbitPattern();
	/*
	@return Amplitude of the orbit.
	*/
	uint32_t Amplitude();
	/*
	@return Period of the orbit.
	*/
	uint32_t Period();
	/*
	@return Center of the orbit.
	*/
	uint32_t Center();
	/*
	@return The duration of flight if the rocket were to take off from either planet on #round
	*/
	uint32_t Duration(uint32_t round);
};

