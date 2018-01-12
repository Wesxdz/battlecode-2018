#include "Location.h"

#include "GameController.h"
#include "Structure.h"
#include "Log.h"

Location::Location()
{
}

Location::~Location()
{
	delete_bc_Location(self);
}

Location::Location(const Location& other)
{
	self = bc_Location_clone(other.self);
}

uint8_t Location::IsOnMap()
{
	return bc_Location_is_on_map(self);
}

uint8_t Location::OnPlanet(bc_Planet planet)
{
	return bc_Location_is_on_planet(self, planet);
}

MapLocation Location::ToMapLocation()
{
	auto location = MapLocation(bc_Location_map_location(self));
	CHECK_ERRORS();
	return location;
}

uint8_t Location::IsInGarrison()
{
	return bc_Location_is_in_garrison(self);
}

std::shared_ptr<units::Structure> Location::GarrisonStructure()
{
	uint16_t structureId = bc_Location_structure(self);
	auto structure = std::make_shared<units::Structure>();
	structure->Init(bc_GameController_unit(GameController::gc, structureId));
	CHECK_ERRORS();
	return structure;
}

uint8_t Location::IsInSpace()
{
	return bc_Location_is_in_space(self);
}
