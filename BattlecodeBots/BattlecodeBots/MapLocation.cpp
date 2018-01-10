#include "MapLocation.h"

MapLocation::MapLocation(bc_Planet planet, int32_t x, int32_t y)
{
	self = new_bc_MapLocation(planet, x, y);
}

MapLocation::MapLocation(bc_MapLocation * loc)
{
	self = loc;
}

MapLocation::~MapLocation()
{
	delete_bc_MapLocation(self);
}

bc_Planet MapLocation::Planet()
{
	return bc_Planet();
}

int32_t MapLocation::X()
{
	return int32_t();
}

int32_t MapLocation::Y()
{
	return int32_t();
}

MapLocation MapLocation::Neighbor(MapLocation & origin, bc_Direction direction)
{
	return MapLocation(bc_MapLocation_add(origin.self, direction));
}

MapLocation MapLocation::Translate(MapLocation & origin, int32_t dx, int32_t dy)
{
	return bc_MapLocation_translate(origin.self, dx, dy);
}

uint32_t MapLocation::DistanceTo(MapLocation& location)
{
	return bc_MapLocation_distance_squared_to(self, location.self);
}

bc_Direction MapLocation::DirectionTo(MapLocation& location)
{
	return bc_MapLocation_direction_to(self, location.self);
}

uint8_t MapLocation::IsAdjacentTo(MapLocation& location)
{
	return bc_MapLocation_is_adjacent_to(self, location.self);
}

uint8_t MapLocation::IsWithinRange(MapLocation& location, uint32_t range)
{
	return bc_MapLocation_is_within_range(self, range, location.self);
}
