#pragma once
#include "Unit.h"
class Structure : public Unit 
{
public:
	Structure();
	~Structure();
	virtual uint32_t Cost() override;
};