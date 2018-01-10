#include "Location.h"

#include "Structure.h"

Location::Location()
{
}

Location::Location()
{
	delete_bc_Location(self);
}

Location::~Location()
{
}

uint8_t Location::OnMap()
{
	return bc_Location_is_on_map(self);
}

uint8_t Location::OnPlanet(bc_Planet planet)
{
	return bc_Location_is_on_planet(self, planet);
}

MapLocation Location::ToMapLocation()
{
	return MapLocation(bc_Location_map_location(self));
}

uint8_t Location::IsInGarrison()
{
	return bc_Location_is_in_garrison(self);
}

std::shared_ptr<Structure> Location::GarrisonStructure()
{
	uint16_t stuctureID = bc_Location_structure(self);
	// TODO Implement, pass in vector of structures?
	return std::shared_ptr<Structure>();
}

uint8_t Location::IsInSpace()
{
	return bc_Location_is_in_space(self);
}
