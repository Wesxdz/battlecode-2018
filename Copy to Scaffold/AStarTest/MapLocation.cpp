#include "MapLocation.h"

#include "GameController.h"
#include "Log.h"
#include "GameMap.h"
#include "PlanetMap.h"

#include "VecUnit.h"

template<class T>
inline std::vector<T> MapLocation::NearbyUnits(uint32_t radius_squared, bc_UnitType type)
{
	return VecUnit::Wrap<T>(bc_GameController_sense_nearby_units_by_type(GameController::gc, self, radius_squared, type));
}

MapLocation::MapLocation() 
{

}

MapLocation::MapLocation(bc_Planet planet, int32_t x, int32_t y)
{
	self = new_bc_MapLocation(planet, x, y);
}

MapLocation::MapLocation(bc_MapLocation* loc)
{
	self = loc;
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
	return bc_MapLocation_eq(this->self, other.self);
}

bc_Planet MapLocation::Planet()
{
	return bc_MapLocation_planet_get(self);
}

int32_t MapLocation::X()
{
	return bc_MapLocation_x_get(self);
}

int32_t MapLocation::Y()
{
	return bc_MapLocation_y_get(self);
}

MapLocation MapLocation::Neighbor(MapLocation& origin, bc_Direction direction)
{
	return MapLocation(bc_MapLocation_add(origin.self, direction));
}

MapLocation MapLocation::Translate(MapLocation& origin, int32_t dx, int32_t dy)
{
	return MapLocation(bc_MapLocation_translate(origin.self, dx, dy));
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

uint8_t MapLocation::IsOccupiable()
{
	auto isOccupiable = bc_GameController_is_occupiable(GameController::gc, self);
	CHECK_ERRORS();
	return isOccupiable;
}

uint8_t MapLocation::IsOccupied()
{
	return bc_GameController_has_unit_at_location(GameController::gc, self);
}

bc_Unit* MapLocation::Occupant()
{
	return bc_GameController_sense_unit_at_location(GameController::gc, self);
}

uint8_t MapLocation::IsValid()
{
	GameMap map;
	PlanetMap planetMap;
	if (Planet() == bc_Planet::Earth) {
		planetMap = map.Earth();;
	}
	else {
		planetMap = map.Mars();
	}
	return planetMap.IsOnMap(*this);
}

uint8_t MapLocation::IsPassable()
{
	GameMap map;
	PlanetMap planetMap;
	if (Planet() == bc_Planet::Earth) {
		planetMap = map.Earth();;
	}
	else {
		planetMap = map.Mars();
	}
	return planetMap.IsPassableTerrain(*this);
}

std::vector<units::Unit> MapLocation::NearbyUnits(uint32_t radius_squared)
{
	std::vector<units::Unit> nearbyUnits;
	bc_VecUnit * nearby = bc_GameController_sense_nearby_units(GameController::gc, self, radius_squared);
	for (uintptr_t i = 0; i < bc_VecUnit_len(nearby); i++) {
		nearbyUnits.push_back(bc_VecUnit_index(nearby, i));
		
	}
	delete_bc_VecUnit(nearby);
	return nearbyUnits;
}

std::vector<units::Unit> MapLocation::NearbyUnits(uint32_t radius_squared, bc_Team team)
{
	std::vector<units::Unit> nearbyUnits;
	bc_VecUnit * nearby = bc_GameController_sense_nearby_units_by_team(GameController::gc, self, radius_squared, team);
	for (uintptr_t i = 0; i < bc_VecUnit_len(nearby); i++) {
		nearbyUnits.push_back(bc_VecUnit_index(nearby, i));
	}
	delete_bc_VecUnit(nearby);
	return nearbyUnits;
}

std::vector<MapLocation> MapLocation::NearbyLocations(MapLocation& location, uint32_t radius_squared)
{
	std::vector<MapLocation> nearbyLocations;
	bc_VecMapLocation * nearby = bc_GameController_all_locations_within(GameController::gc, location.self, radius_squared);
	for (uintptr_t i = 0; i < bc_VecMapLocation_len(nearby); i++) {
		nearbyLocations.push_back(bc_VecMapLocation_index(nearby, i));
		
	}
	delete_bc_VecMapLocation(nearby);
	return nearbyLocations;
}
