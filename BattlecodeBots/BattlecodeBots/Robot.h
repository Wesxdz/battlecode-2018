#pragma once
#include "Unit.h"
class Robot :
	public Unit
{
public:
	Robot();
	~Robot();
	virtual uint32_t Cost() override;
	/*
	@return The damage inflicted by the robot during a normal attack.
	*/
	uint32_t Damage();
	/*
	@return The attack range.
	*/
	uint32_t AttackRange();
	/*
	@return The movement heat.
	*/
	uint32_t MovementHeat();
	/*
	@return The attack heat.
	*/
	uint32_t AttackHeat();
};

