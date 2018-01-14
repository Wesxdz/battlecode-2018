#include "MapLocation.h"

#include "GameController.h"
#include "Unit.h"

MapLocation::MapLocation() { }

MapLocation::MapLocation(bc_Planet planet, int32_t x, int32_t y)
{
	self = new_bc_MapLocation(planet, x, y);
}

MapLocation::MapLocation(bc_MapLocation * loc)
{
	self = bc_MapLocation_clone(loc);
}
MapLocation::MapLocation(const MapLocation& other)
{
	self = bc_MapLocation_clone(other.self);
}

MapLocation::~MapLocation()
{
	delete_bc_MapLocation(self);
}

MapLocation MapLocation::operator=(const MapLocation& other) {
	this->self = bc_MapLocation_clone(other.self);
	return *this;
}

bool MapLocation::operator==(MapLocation& other) {
	return this->X() == other.X() && this->Y() == other.Y() && this->Planet() == other.Planet();
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

uint32_t MapLocation::Karbonite()
{
	return bc_GameController_karbonite_at(GameController::gc, self);
}

uint8_t MapLocation::IsVisible()
{
	return bc_GameController_can_sense_location(GameController::gc, self);
}

uint8_t MapLocation::Occupiable()
{
	return bc_GameController_is_occupiable(GameController::gc, self);
}

int32_t MapLocation::X()
{
	return bc_MapLocation_x_get(self);
}

int32_t MapLocation::Y()
{
	return bc_MapLocation_y_get(self);
}

bc_Planet MapLocation::Planet()
{
	return bc_MapLocation_planet_get(self);
}

std::shared_ptr<units::Unit> MapLocation::Occupant()
{
	auto unit = GameController::Unit(bc_GameController_sense_unit_at_location(GameController::gc, self));
	unit->Init(unit->self);
	return unit;
}
