#ifndef BUILDEROVERLORD_H
#define BUILDEROVERLORD_H

#include "bc.h"
#include "PlayerData.h"
#include <vector>
#include "Utility.h"
#include "Worker.h"
#include "Factory.h"
#include "Rocket.h"

class BuilderOverlord
{
public:
	BuilderOverlord();
	void Update();
	void DetermineDesiredUnits();

};

#endif