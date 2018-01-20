/// GENERATED C, DO NOT EDIT
#ifndef bc_h_
#define bc_h_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
uint8_t bc_has_err();
int8_t bc_get_last_err(char** result);
int8_t bc_free_string(char* err);
/// The planets in the Battlecode world.
typedef enum bc_Planet {
    Earth = 0,
    Mars = 1,
} bc_Planet;
/// The other planet.
bc_Planet bc_Planet_other(bc_Planet self);

/// Create a human-readable representation of a Planet
char* bc_Planet_debug(bc_Planet self);

/// Compare two Planets for deep equality.
uint8_t bc_Planet_eq(bc_Planet self, bc_Planet other);

/// Deserialize a Planet from a JSON string
bc_Planet bc_Planet_from_json(char* s);

/// Serialize a Planet to a JSON string
char* bc_Planet_to_json(bc_Planet self);
/// A direction from one MapLocation to another.
/// 
/// Directions for each of the cardinals (north, south, east, west), and each
/// of the diagonals (northwest, southwest, northeast, southeast). There is
/// also a "center" direction, representing no direction.
/// 
/// Coordinates increase in the north and east directions.
typedef enum bc_Direction {
    North = 0,
    Northeast = 1,
    East = 2,
    Southeast = 3,
    South = 4,
    Southwest = 5,
    West = 6,
    Northwest = 7,
    Center = 8,
} bc_Direction;
/// Returns the x displacement of self direction.
int32_t bc_Direction_dx(bc_Direction self);

/// Returns the y displacement of self direction.
int32_t bc_Direction_dy(bc_Direction self);

/// Whether self direction is a diagonal one.
uint8_t bc_Direction_is_diagonal(bc_Direction self);

/// Returns the direction opposite self one, or Center if it's Center.
bc_Direction bc_Direction_opposite(bc_Direction self);

/// Returns the direction 45 degrees to the left (counter-clockwise) of
/// self one, or Center if it's Center.
bc_Direction bc_Direction_rotate_left(bc_Direction self);

/// Returns the direction 45 degrees to the right (clockwise) of self one,
/// or Center if it's Center.
bc_Direction bc_Direction_rotate_right(bc_Direction self);

/// Deserialize a Direction from a JSON string
bc_Direction bc_Direction_from_json(char* s);

/// Serialize a Direction to a JSON string
char* bc_Direction_to_json(bc_Direction self);
/// Two-dimensional coordinates in the Battlecode world.
typedef struct bc_MapLocation bc_MapLocation;
/// Returns a new MapLocation representing the location with the given
/// coordinates on a planet.
bc_MapLocation* new_bc_MapLocation(bc_Planet planet, int32_t x, int32_t y);
/// 
void delete_bc_MapLocation(bc_MapLocation* self);
/// The planet of the map location.
bc_Planet bc_MapLocation_planet_get(bc_MapLocation* self);
/// The x coordinate of the map location.
int32_t bc_MapLocation_x_get(bc_MapLocation* self);
/// The y coordinate of the map location.
int32_t bc_MapLocation_y_get(bc_MapLocation* self);
/// The planet of the map location.
void bc_MapLocation_planet_set(bc_MapLocation* self, bc_Planet planet);
/// The x coordinate of the map location.
void bc_MapLocation_x_set(bc_MapLocation* self, int32_t x);
/// The y coordinate of the map location.
void bc_MapLocation_y_set(bc_MapLocation* self, int32_t y);
/// Returns the location one square from self one in the given direction.
bc_MapLocation* bc_MapLocation_add(bc_MapLocation* self, bc_Direction direction);
/// Returns the location one square from self one in the opposite direction.
bc_MapLocation* bc_MapLocation_subtract(bc_MapLocation* self, bc_Direction direction);
/// Returns the location `multiple` squares from self one in the given
/// direction.
bc_MapLocation* bc_MapLocation_add_multiple(bc_MapLocation* self, bc_Direction direction, int32_t multiple);
/// Returns the location translated from self location by `dx` in the x
/// direction and `dy` in the y direction.
bc_MapLocation* bc_MapLocation_translate(bc_MapLocation* self, int32_t dx, int32_t dy);
/// Computes the square of the distance from self location to the specified
/// location. If on different planets, returns the maximum integer.
uint32_t bc_MapLocation_distance_squared_to(bc_MapLocation* self, bc_MapLocation* o);
/// Returns the Direction from self location to the specified location.
/// If the locations are equal self method returns Center.
/// 
///  * DifferentPlanet - The locations are on different planets.
bc_Direction bc_MapLocation_direction_to(bc_MapLocation* self, bc_MapLocation* o);
/// 
/// Determines whether self location is adjacent to the specified location,
/// including diagonally. Note that squares are not adjacent to themselves,
/// and squares on different planets are not adjacent to each other.
uint8_t bc_MapLocation_is_adjacent_to(bc_MapLocation* self, bc_MapLocation* o);
/// 
/// Whether self location is within the distance squared range of the
/// specified location, inclusive. False for locations on different planets.
uint8_t bc_MapLocation_is_within_range(bc_MapLocation* self, uint32_t range, bc_MapLocation* o);
/// Create a human-readable representation of a MapLocation
char* bc_MapLocation_debug(bc_MapLocation* self);
/// Deep-copy a MapLocation
bc_MapLocation* bc_MapLocation_clone(bc_MapLocation* self);
/// Compare two MapLocations for deep equality.
uint8_t bc_MapLocation_eq(bc_MapLocation* self, bc_MapLocation* other);
/// Deserialize a MapLocation from a JSON string
bc_MapLocation* bc_MapLocation_from_json(char* s);
/// Serialize a MapLocation to a JSON string
char* bc_MapLocation_to_json(bc_MapLocation* self);
/// An immutable list of bc::location::MapLocation objects
typedef struct bc_VecMapLocation bc_VecMapLocation;
/// An immutable list of bc::location::MapLocation objects
bc_VecMapLocation* new_bc_VecMapLocation();
/// 
void delete_bc_VecMapLocation(bc_VecMapLocation* self);
/// Create a human-readable representation of a VecMapLocation
char* bc_VecMapLocation_debug(bc_VecMapLocation* self);
/// Deep-copy a VecMapLocation
bc_VecMapLocation* bc_VecMapLocation_clone(bc_VecMapLocation* self);
/// The length of the vector.
uintptr_t bc_VecMapLocation_len(bc_VecMapLocation* self);
/// Copy an element out of the vector.
bc_MapLocation* bc_VecMapLocation_index(bc_VecMapLocation* self, uintptr_t index);
/// An immutable list of i32 objects
typedef struct bc_Veci32 bc_Veci32;
/// An immutable list of i32 objects
bc_Veci32* new_bc_Veci32();
/// 
void delete_bc_Veci32(bc_Veci32* self);
/// Create a human-readable representation of a Veci32
char* bc_Veci32_debug(bc_Veci32* self);
/// Deep-copy a Veci32
bc_Veci32* bc_Veci32_clone(bc_Veci32* self);
/// The length of the vector.
uintptr_t bc_Veci32_len(bc_Veci32* self);
/// Copy an element out of the vector.
int32_t bc_Veci32_index(bc_Veci32* self, uintptr_t index);
/// 
typedef struct bc_Location bc_Location;
/// 
bc_Location* new_bc_Location();
/// 
void delete_bc_Location(bc_Location* self);
/// Constructs a new location on the map.
bc_Location* bc_Location_new_on_map(bc_MapLocation* map_location);
/// Constructs a new location in a garrison.
bc_Location* bc_Location_new_in_garrison(uint16_t id);
/// Constructs a new location in space.
bc_Location* bc_Location_new_in_space();
/// Whether the unit is on a map.
uint8_t bc_Location_is_on_map(bc_Location* self);
/// True if and only if the location is on the map and on self planet.
uint8_t bc_Location_is_on_planet(bc_Location* self, bc_Planet planet);
/// The map location of the unit.
/// 
///  * UnitNotOnMap - The unit is in a garrison or in space, and does not
///    have a map location.
bc_MapLocation* bc_Location_map_location(bc_Location* self);
/// Whether the unit is in a garrison.
uint8_t bc_Location_is_in_garrison(bc_Location* self);
/// The structure whose garrison the unit is in.
/// 
///  * UnitNotInGarrison - the unit is not in a garrison.
uint16_t bc_Location_structure(bc_Location* self);
/// Whether the unit is in space.
uint8_t bc_Location_is_in_space(bc_Location* self);
/// Determines whether self location is adjacent to the specified location,
/// including diagonally. Note that squares are not adjacent to themselves,
/// and squares on different planets are not adjacent to each other. Also,
/// nothing is adjacent to something not on a map.
uint8_t bc_Location_is_adjacent_to(bc_Location* self, bc_Location* o);
/// Whether self location is within the distance squared range of the
/// specified location, inclusive. False for locations on different planets.
/// Note that nothing is within the range of something not on the map.
uint8_t bc_Location_is_within_range(bc_Location* self, uint32_t range, bc_Location* o);
/// Create a human-readable representation of a Location
char* bc_Location_debug(bc_Location* self);
/// Deep-copy a Location
bc_Location* bc_Location_clone(bc_Location* self);
/// Compare two Locations for deep equality.
uint8_t bc_Location_eq(bc_Location* self, bc_Location* other);
/// Deserialize a Location from a JSON string
bc_Location* bc_Location_from_json(char* s);
/// Serialize a Location to a JSON string
char* bc_Location_to_json(bc_Location* self);
/// 
typedef enum bc_Team {
    Red = 0,
    Blue = 1,
} bc_Team;
/// Deserialize a Team from a JSON string
bc_Team bc_Team_from_json(char* s);

/// Serialize a Team to a JSON string
char* bc_Team_to_json(bc_Team self);
/// 
typedef struct bc_Player bc_Player;
/// 
bc_Player* new_bc_Player(bc_Team team, bc_Planet planet);
/// 
void delete_bc_Player(bc_Player* self);
/// 
bc_Team bc_Player_team_get(bc_Player* self);
/// 
bc_Planet bc_Player_planet_get(bc_Player* self);
/// 
void bc_Player_team_set(bc_Player* self, bc_Team team);
/// 
void bc_Player_planet_set(bc_Player* self, bc_Planet planet);
/// Create a human-readable representation of a Player
char* bc_Player_debug(bc_Player* self);
/// Deep-copy a Player
bc_Player* bc_Player_clone(bc_Player* self);
/// Compare two Players for deep equality.
uint8_t bc_Player_eq(bc_Player* self, bc_Player* other);
/// Deserialize a Player from a JSON string
bc_Player* bc_Player_from_json(char* s);
/// Serialize a Player to a JSON string
char* bc_Player_to_json(bc_Player* self);
/// An immutable list of bc::unit::UnitID objects
typedef struct bc_VecUnitID bc_VecUnitID;
/// An immutable list of bc::unit::UnitID objects
bc_VecUnitID* new_bc_VecUnitID();
/// 
void delete_bc_VecUnitID(bc_VecUnitID* self);
/// Create a human-readable representation of a VecUnitID
char* bc_VecUnitID_debug(bc_VecUnitID* self);
/// Deep-copy a VecUnitID
bc_VecUnitID* bc_VecUnitID_clone(bc_VecUnitID* self);
/// The length of the vector.
uintptr_t bc_VecUnitID_len(bc_VecUnitID* self);
/// Copy an element out of the vector.
uint16_t bc_VecUnitID_index(bc_VecUnitID* self, uintptr_t index);
/// The different unit types, which include factories, rockets, and the robots.
typedef enum bc_UnitType {
    Worker = 0,
    Knight = 1,
    Ranger = 2,
    Mage = 3,
    Healer = 4,
    Factory = 5,
    Rocket = 6,
} bc_UnitType;
/// Deserialize a UnitType from a JSON string
bc_UnitType bc_UnitType_from_json(char* s);

/// Serialize a UnitType to a JSON string
char* bc_UnitType_to_json(bc_UnitType self);

/// The cost of the unit in a factory.
/// 
///  * InappropriateUnitType - the unit type cannot be produced in a factory.
uint32_t bc_UnitType_factory_cost(bc_UnitType self);

/// The cost to blueprint the unit.
/// 
///  * InappropriateUnitType - the unit type cannot be blueprinted.
uint32_t bc_UnitType_blueprint_cost(bc_UnitType self);

/// The cost to replicate the unit.
/// 
///  * InappropriateUnitType - the unit type is not a worker.
uint32_t bc_UnitType_replicate_cost(bc_UnitType self);

/// The value of a unit, as relevant to tiebreakers.
uint32_t bc_UnitType_value(bc_UnitType self);
/// An immutable list of bc::unit::UnitType objects
typedef struct bc_VecUnitType bc_VecUnitType;
/// An immutable list of bc::unit::UnitType objects
bc_VecUnitType* new_bc_VecUnitType();
/// 
void delete_bc_VecUnitType(bc_VecUnitType* self);
/// Create a human-readable representation of a VecUnitType
char* bc_VecUnitType_debug(bc_VecUnitType* self);
/// Deep-copy a VecUnitType
bc_VecUnitType* bc_VecUnitType_clone(bc_VecUnitType* self);
/// The length of the vector.
uintptr_t bc_VecUnitType_len(bc_VecUnitType* self);
/// Copy an element out of the vector.
bc_UnitType bc_VecUnitType_index(bc_VecUnitType* self, uintptr_t index);
/// A single unit in the game and all its associated properties.
typedef struct bc_Unit bc_Unit;
/// A single unit in the game and all its associated properties.
bc_Unit* new_bc_Unit();
/// 
void delete_bc_Unit(bc_Unit* self);
/// Create a human-readable representation of a Unit
char* bc_Unit_debug(bc_Unit* self);
/// Deep-copy a Unit
bc_Unit* bc_Unit_clone(bc_Unit* self);
/// Deserialize a Unit from a JSON string
bc_Unit* bc_Unit_from_json(char* s);
/// Serialize a Unit to a JSON string
char* bc_Unit_to_json(bc_Unit* self);
/// Compare two Units for deep equality.
uint8_t bc_Unit_eq(bc_Unit* self, bc_Unit* other);
/// The unique ID of a unit.
uint16_t bc_Unit_id(bc_Unit* self);
/// The team the unit belongs to.
bc_Team bc_Unit_team(bc_Unit* self);
/// The current research level.
uintptr_t bc_Unit_research_level(bc_Unit* self);
/// The unit type.
bc_UnitType bc_Unit_unit_type(bc_Unit* self);
/// The location of the unit.
bc_Location* bc_Unit_location(bc_Unit* self);
/// The current health.
uint32_t bc_Unit_health(bc_Unit* self);
/// The maximum health.
uint32_t bc_Unit_max_health(bc_Unit* self);
/// The unit vision range.
uint32_t bc_Unit_vision_range(bc_Unit* self);
/// The damage inflicted by the robot during a normal attack.
/// 
///  * InappropriateUnitType - the unit is not a robot.
int32_t bc_Unit_damage(bc_Unit* self);
/// The attack range.
/// 
///  * InappropriateUnitType - the unit is not a robot.
uint32_t bc_Unit_attack_range(bc_Unit* self);
/// The movement heat.
/// 
///  * InappropriateUnitType - the unit is not a robot.
uint32_t bc_Unit_movement_heat(bc_Unit* self);
/// The attack heat.
/// 
///  * InappropriateUnitType - the unit is not a robot.
uint32_t bc_Unit_attack_heat(bc_Unit* self);
/// The movement cooldown.
/// 
///  * InappropriateUnitType - the unit is not a robot.
uint32_t bc_Unit_movement_cooldown(bc_Unit* self);
/// The attack cooldown.
/// 
///  * InappropriateUnitType - the unit is not a robot.
uint32_t bc_Unit_attack_cooldown(bc_Unit* self);
/// Whether the active ability is unlocked.
/// 
///  * InappropriateUnitType - the unit is not a robot.
uint8_t bc_Unit_is_ability_unlocked(bc_Unit* self);
/// The active ability heat.
/// 
///  * InappropriateUnitType - the unit is not a robot.
uint32_t bc_Unit_ability_heat(bc_Unit* self);
/// The active ability cooldown.
/// 
///  * InappropriateUnitType - the unit is not a robot.
uint32_t bc_Unit_ability_cooldown(bc_Unit* self);
/// The active ability range. self is the range in which: workers can replicate, knights can javelin, rangers can snipe, mages can blink, and healers can overcharge.
/// 
///  * InappropriateUnitType - the unit is not a robot.
uint32_t bc_Unit_ability_range(bc_Unit* self);
/// Whether the worker has already acted (harveted, blueprinted, built, or repaired) self round.
/// 
///  * InappropriateUnitType - the unit is not a worker.
uint8_t bc_Unit_worker_has_acted(bc_Unit* self);
/// The health restored when building a structure.
/// 
///  * InappropriateUnitType - the unit is not a worker.
uint32_t bc_Unit_worker_build_health(bc_Unit* self);
/// The health restored when repairing a structure.
/// 
///  * InappropriateUnitType - the unit is not a worker.
uint32_t bc_Unit_worker_repair_health(bc_Unit* self);
/// The maximum amount of karbonite harvested from a deposit in one turn.
/// 
///  * InappropriateUnitType - the unit is not a worker.
uint32_t bc_Unit_worker_harvest_amount(bc_Unit* self);
/// The amount of damage resisted by a knight when attacked.
/// 
///  * InappropriateUnitType - the unit is not a knight.
uint32_t bc_Unit_knight_defense(bc_Unit* self);
/// The range within a ranger cannot attack.
/// 
///  * InappropriateUnitType - the unit is not a ranger.
uint32_t bc_Unit_ranger_cannot_attack_range(bc_Unit* self);
/// The maximum countdown for ranger's snipe, which is the number of turns that must pass before the snipe is executed.
/// 
///  * InappropriateUnitType - the unit is not a ranger.
uint32_t bc_Unit_ranger_max_countdown(bc_Unit* self);
/// Whether the ranger is sniping.
/// 
///  * InappropriateUnitType - the unit is not a ranger.
uint8_t bc_Unit_ranger_is_sniping(bc_Unit* self);
/// The target location for ranger's snipe.
/// 
///  * InappropriateUnitType - the unit is not a ranger.
///  * NullValue - the ranger is not sniping.
bc_MapLocation* bc_Unit_ranger_target_location(bc_Unit* self);
/// The countdown for ranger's snipe. Errors if the ranger is not sniping.
/// 
///  * InappropriateUnitType - the unit is not a ranger.
///  * NullValue - the ranger is not sniping.
uint32_t bc_Unit_ranger_countdown(bc_Unit* self);
/// The amount of health passively restored to itself each round.
/// 
///  * InappropriateUnitType - the unit is not a healer.
uint32_t bc_Unit_healer_self_heal_amount(bc_Unit* self);
/// Whether self structure has been built.
/// 
///  * InappropriateUnitType - the unit is not a structure.
uint8_t bc_Unit_structure_is_built(bc_Unit* self);
/// The max capacity of a structure.
/// 
///  * InappropriateUnitType - the unit is not a structure.
uintptr_t bc_Unit_structure_max_capacity(bc_Unit* self);
/// Returns the units in the structure's garrison.
/// 
///  * InappropriateUnitType - the unit is not a structure.
bc_VecUnitID* bc_Unit_structure_garrison(bc_Unit* self);
/// Whether the factory is currently producing a unit.
/// 
/// * InappropriateUnitType - the unit is not a factory.
uint8_t bc_Unit_is_factory_producing(bc_Unit* self);
/// The unit type currently being produced by the factory.
/// 
///  * InappropriateUnitType - the unit is not a factory.
/// * NullValue - the factory is not producing.
bc_UnitType bc_Unit_factory_unit_type(bc_Unit* self);
/// The number of rounds left to produce a robot in self factory.
/// 
///  * InappropriateUnitType - the unit is not a factory.
///  * NullValue - the factory is not producing.
uint32_t bc_Unit_factory_rounds_left(bc_Unit* self);
/// The maximum number of rounds left to produce a robot in self factory.
/// 
///  * InappropriateUnitType - the unit is not a factory.
uint32_t bc_Unit_factory_max_rounds_left(bc_Unit* self);
/// Whether the rocket has already been used.
/// 
///  * InappropriateUnitType - the unit is not a rocket.
uint8_t bc_Unit_rocket_is_used(bc_Unit* self);
/// The damage a rocket deals to adjacent units upon landing.
/// 
///  * InappropriateUnitType - the unit is not a rocket.
int32_t bc_Unit_rocket_blast_damage(bc_Unit* self);
/// The number of rounds the rocket travel time is reduced by compared to the travel time determined by the orbit of the planets.
/// 
///  * InappropriateUnitType - the unit is not a rocket.
uint32_t bc_Unit_rocket_travel_time_decrease(bc_Unit* self);
/// An immutable list of bc::unit::Unit objects
typedef struct bc_VecUnit bc_VecUnit;
/// An immutable list of bc::unit::Unit objects
bc_VecUnit* new_bc_VecUnit();
/// 
void delete_bc_VecUnit(bc_VecUnit* self);
/// Create a human-readable representation of a VecUnit
char* bc_VecUnit_debug(bc_VecUnit* self);
/// Deep-copy a VecUnit
bc_VecUnit* bc_VecUnit_clone(bc_VecUnit* self);
/// The length of the vector.
uintptr_t bc_VecUnit_len(bc_VecUnit* self);
/// Copy an element out of the vector.
bc_Unit* bc_VecUnit_index(bc_VecUnit* self, uintptr_t index);
/// The map for one of the planets in the Battlecode world. self information defines the terrain, dimensions, and initial units of the planet.
typedef struct bc_PlanetMap bc_PlanetMap;
/// The map for one of the planets in the Battlecode world. self information defines the terrain, dimensions, and initial units of the planet.
bc_PlanetMap* new_bc_PlanetMap();
/// 
void delete_bc_PlanetMap(bc_PlanetMap* self);
/// The planet of the map.
bc_Planet bc_PlanetMap_planet_get(bc_PlanetMap* self);
/// The height of self map, in squares. Must be in the range [MAP_HEIGHT_MIN, MAP_HEIGHT_MAX], inclusive.
uintptr_t bc_PlanetMap_height_get(bc_PlanetMap* self);
/// The height of self map, in squares. Must be in the range [MAP_WIDTH_MIN, MAP_WIDTH_MAX], inclusive.
uintptr_t bc_PlanetMap_width_get(bc_PlanetMap* self);
/// The initial units on the map. Each team starts with 1 to 3 Workers on Earth.
bc_VecUnit* bc_PlanetMap_initial_units_get(bc_PlanetMap* self);
/// The planet of the map.
void bc_PlanetMap_planet_set(bc_PlanetMap* self, bc_Planet planet);
/// The height of self map, in squares. Must be in the range [MAP_HEIGHT_MIN, MAP_HEIGHT_MAX], inclusive.
void bc_PlanetMap_height_set(bc_PlanetMap* self, uintptr_t height);
/// The height of self map, in squares. Must be in the range [MAP_WIDTH_MIN, MAP_WIDTH_MAX], inclusive.
void bc_PlanetMap_width_set(bc_PlanetMap* self, uintptr_t width);
/// The initial units on the map. Each team starts with 1 to 3 Workers on Earth.
void bc_PlanetMap_initial_units_set(bc_PlanetMap* self, bc_VecUnit* initial_units);
/// Validates the map and checks some invariants are followed.
/// 
///  * InvalidMapObject - the planet map is invalid.
void bc_PlanetMap_validate(bc_PlanetMap* self);
/// Whether a location is on the map.
uint8_t bc_PlanetMap_on_map(bc_PlanetMap* self, bc_MapLocation* location);
/// 
/// Whether the location on the map contains passable terrain. Is only false when the square contains impassable terrain (distinct from containing a building, for instance).
/// 
/// LocationOffMap - the location is off the map.
uint8_t bc_PlanetMap_is_passable_terrain_at(bc_PlanetMap* self, bc_MapLocation* location);
/// The amount of Karbonite initially deposited at the given location.
/// 
/// LocationOffMap - the location is off the map.
uint32_t bc_PlanetMap_initial_karbonite_at(bc_PlanetMap* self, bc_MapLocation* location);
/// Deep-copy a PlanetMap
bc_PlanetMap* bc_PlanetMap_clone(bc_PlanetMap* self);
/// Deserialize a PlanetMap from a JSON string
bc_PlanetMap* bc_PlanetMap_from_json(char* s);
/// Serialize a PlanetMap to a JSON string
char* bc_PlanetMap_to_json(bc_PlanetMap* self);
/// 
typedef struct bc_Delta bc_Delta;
/// 
bc_Delta* new_bc_Delta();
/// 
void delete_bc_Delta(bc_Delta* self);
/// Deserialize a Delta from a JSON string
bc_Delta* bc_Delta_from_json(char* s);
/// Serialize a Delta to a JSON string
char* bc_Delta_to_json(bc_Delta* self);
/// 
typedef struct bc_StartGameMessage bc_StartGameMessage;
/// 
bc_StartGameMessage* new_bc_StartGameMessage();
/// 
void delete_bc_StartGameMessage(bc_StartGameMessage* self);
/// Deserialize a StartGameMessage from a JSON string
bc_StartGameMessage* bc_StartGameMessage_from_json(char* s);
/// Serialize a StartGameMessage to a JSON string
char* bc_StartGameMessage_to_json(bc_StartGameMessage* self);
/// 
typedef struct bc_TurnMessage bc_TurnMessage;
/// 
bc_TurnMessage* new_bc_TurnMessage();
/// 
void delete_bc_TurnMessage(bc_TurnMessage* self);
/// Deserialize a TurnMessage from a JSON string
bc_TurnMessage* bc_TurnMessage_from_json(char* s);
/// Serialize a TurnMessage to a JSON string
char* bc_TurnMessage_to_json(bc_TurnMessage* self);
/// 
typedef struct bc_StartTurnMessage bc_StartTurnMessage;
/// 
bc_StartTurnMessage* new_bc_StartTurnMessage();
/// 
void delete_bc_StartTurnMessage(bc_StartTurnMessage* self);
/// 
int32_t bc_StartTurnMessage_time_left_ms_get(bc_StartTurnMessage* self);
/// 
uint32_t bc_StartTurnMessage_round_get(bc_StartTurnMessage* self);
/// 
void bc_StartTurnMessage_time_left_ms_set(bc_StartTurnMessage* self, int32_t time_left_ms);
/// 
void bc_StartTurnMessage_round_set(bc_StartTurnMessage* self, uint32_t round);
/// Deserialize a StartTurnMessage from a JSON string
bc_StartTurnMessage* bc_StartTurnMessage_from_json(char* s);
/// Serialize a StartTurnMessage to a JSON string
char* bc_StartTurnMessage_to_json(bc_StartTurnMessage* self);
/// 
typedef struct bc_ViewerMessage bc_ViewerMessage;
/// 
bc_ViewerMessage* new_bc_ViewerMessage();
/// 
void delete_bc_ViewerMessage(bc_ViewerMessage* self);
/// Deserialize a ViewerMessage from a JSON string
bc_ViewerMessage* bc_ViewerMessage_from_json(char* s);
/// Serialize a ViewerMessage to a JSON string
char* bc_ViewerMessage_to_json(bc_ViewerMessage* self);
/// 
typedef struct bc_ViewerKeyframe bc_ViewerKeyframe;
/// 
bc_ViewerKeyframe* new_bc_ViewerKeyframe();
/// 
void delete_bc_ViewerKeyframe(bc_ViewerKeyframe* self);
/// Deserialize a ViewerKeyframe from a JSON string
bc_ViewerKeyframe* bc_ViewerKeyframe_from_json(char* s);
/// Serialize a ViewerKeyframe to a JSON string
char* bc_ViewerKeyframe_to_json(bc_ViewerKeyframe* self);
/// 
typedef struct bc_ErrorMessage bc_ErrorMessage;
/// 
bc_ErrorMessage* new_bc_ErrorMessage();
/// 
void delete_bc_ErrorMessage(bc_ErrorMessage* self);
/// 
char* bc_ErrorMessage_error_get(bc_ErrorMessage* self);
/// 
void bc_ErrorMessage_error_set(bc_ErrorMessage* self, char* error);
/// Deserialize a ErrorMessage from a JSON string
bc_ErrorMessage* bc_ErrorMessage_from_json(char* s);
/// Serialize a ErrorMessage to a JSON string
char* bc_ErrorMessage_to_json(bc_ErrorMessage* self);
/// Create a human-readable representation of a ErrorMessage
char* bc_ErrorMessage_debug(bc_ErrorMessage* self);
/// 
typedef struct bc_ReceivedMessaTurnMessage bc_ReceivedMessaTurnMessage;
/// 
bc_ReceivedMessaTurnMessage* new_bc_ReceivedMessaTurnMessage();
/// 
void delete_bc_ReceivedMessaTurnMessage(bc_ReceivedMessaTurnMessage* self);
/// Deserialize a ReceivedMessaTurnMessage from a JSON string
bc_ReceivedMessaTurnMessage* bc_ReceivedMessaTurnMessage_from_json(char* s);
/// Serialize a ReceivedMessaTurnMessage to a JSON string
char* bc_ReceivedMessaTurnMessage_to_json(bc_ReceivedMessaTurnMessage* self);
/// Create a human-readable representation of a ReceivedMessaTurnMessage
char* bc_ReceivedMessaTurnMessage_debug(bc_ReceivedMessaTurnMessage* self);
/// 
typedef struct bc_SentMessage bc_SentMessage;
/// 
bc_SentMessage* new_bc_SentMessage();
/// 
void delete_bc_SentMessage(bc_SentMessage* self);
/// 
char* bc_SentMessage_client_id_get(bc_SentMessage* self);
/// 
bc_TurnMessage* bc_SentMessage_turn_message_get(bc_SentMessage* self);
/// 
void bc_SentMessage_client_id_set(bc_SentMessage* self, char* client_id);
/// 
void bc_SentMessage_turn_message_set(bc_SentMessage* self, bc_TurnMessage* turn_message);
/// Deserialize a SentMessage from a JSON string
bc_SentMessage* bc_SentMessage_from_json(char* s);
/// Serialize a SentMessage to a JSON string
char* bc_SentMessage_to_json(bc_SentMessage* self);
/// Create a human-readable representation of a SentMessage
char* bc_SentMessage_debug(bc_SentMessage* self);
/// 
typedef struct bc_TurnApplication bc_TurnApplication;
/// 
bc_TurnApplication* new_bc_TurnApplication();
/// 
void delete_bc_TurnApplication(bc_TurnApplication* self);
/// 
bc_StartTurnMessage* bc_TurnApplication_start_turn_get(bc_TurnApplication* self);
/// 
bc_ViewerMessage* bc_TurnApplication_viewer_get(bc_TurnApplication* self);
/// 
void bc_TurnApplication_start_turn_set(bc_TurnApplication* self, bc_StartTurnMessage* start_turn);
/// 
void bc_TurnApplication_viewer_set(bc_TurnApplication* self, bc_ViewerMessage* viewer);
/// 
typedef struct bc_InitialTurnApplication bc_InitialTurnApplication;
/// 
bc_InitialTurnApplication* new_bc_InitialTurnApplication();
/// 
void delete_bc_InitialTurnApplication(bc_InitialTurnApplication* self);
/// 
bc_StartTurnMessage* bc_InitialTurnApplication_start_turn_get(bc_InitialTurnApplication* self);
/// 
bc_ViewerKeyframe* bc_InitialTurnApplication_viewer_get(bc_InitialTurnApplication* self);
/// 
void bc_InitialTurnApplication_start_turn_set(bc_InitialTurnApplication* self, bc_StartTurnMessage* start_turn);
/// 
void bc_InitialTurnApplication_viewer_set(bc_InitialTurnApplication* self, bc_ViewerKeyframe* viewer);
/// A single asteroid strike on Mars.
typedef struct bc_AsteroidStrike bc_AsteroidStrike;
/// 
bc_AsteroidStrike* new_bc_AsteroidStrike(uint32_t karbonite, bc_MapLocation* location);
/// 
void delete_bc_AsteroidStrike(bc_AsteroidStrike* self);
/// 
uint32_t bc_AsteroidStrike_karbonite_get(bc_AsteroidStrike* self);
/// 
bc_MapLocation* bc_AsteroidStrike_location_get(bc_AsteroidStrike* self);
/// 
void bc_AsteroidStrike_karbonite_set(bc_AsteroidStrike* self, uint32_t karbonite);
/// 
void bc_AsteroidStrike_location_set(bc_AsteroidStrike* self, bc_MapLocation* location);
/// Deep-copy a AsteroidStrike
bc_AsteroidStrike* bc_AsteroidStrike_clone(bc_AsteroidStrike* self);
/// Create a human-readable representation of a AsteroidStrike
char* bc_AsteroidStrike_debug(bc_AsteroidStrike* self);
/// Deserialize a AsteroidStrike from a JSON string
bc_AsteroidStrike* bc_AsteroidStrike_from_json(char* s);
/// Serialize a AsteroidStrike to a JSON string
char* bc_AsteroidStrike_to_json(bc_AsteroidStrike* self);
/// Compare two AsteroidStrikes for deep equality.
uint8_t bc_AsteroidStrike_eq(bc_AsteroidStrike* self, bc_AsteroidStrike* other);
/// The asteroid pattern, defined by the timing and contents of each asteroid strike.
typedef struct bc_AsteroidPattern bc_AsteroidPattern;
/// Constructs a pseudorandom asteroid pattern given a map of Mars.
bc_AsteroidPattern* new_bc_AsteroidPattern(uint16_t seed, bc_PlanetMap* mars_map);
/// 
void delete_bc_AsteroidPattern(bc_AsteroidPattern* self);
/// Validates the asteroid pattern.
/// 
///  * InvalidMapObject - the asteroid pattern is invalid.
void bc_AsteroidPattern_validate(bc_AsteroidPattern* self);
/// Whether there is an asteroid strike at the given round.
uint8_t bc_AsteroidPattern_has_asteroid(bc_AsteroidPattern* self, uint32_t round);
/// Get the asteroid strike at the given round.
/// 
///  * NullValue - There is no asteroid strike at self round.
bc_AsteroidStrike* bc_AsteroidPattern_asteroid(bc_AsteroidPattern* self, uint32_t round);
/// Deep-copy a AsteroidPattern
bc_AsteroidPattern* bc_AsteroidPattern_clone(bc_AsteroidPattern* self);
/// Create a human-readable representation of a AsteroidPattern
char* bc_AsteroidPattern_debug(bc_AsteroidPattern* self);
/// Deserialize a AsteroidPattern from a JSON string
bc_AsteroidPattern* bc_AsteroidPattern_from_json(char* s);
/// Serialize a AsteroidPattern to a JSON string
char* bc_AsteroidPattern_to_json(bc_AsteroidPattern* self);
/// The orbit pattern that determines a rocket's flight duration. self pattern is a sinusoidal function y=a*sin(bx)+c.
typedef struct bc_OrbitPattern bc_OrbitPattern;
/// Construct a new orbit pattern. self pattern is a sinusoidal function y=a*sin(bx)+c, where the x-axis is the round number of takeoff and the the y-axis is the duration of flight to the nearest integer.
/// 
/// The amplitude, period, and center are measured in rounds.
bc_OrbitPattern* new_bc_OrbitPattern(uint32_t amplitude, uint32_t period, uint32_t center);
/// 
void delete_bc_OrbitPattern(bc_OrbitPattern* self);
/// Amplitude of the orbit.
uint32_t bc_OrbitPattern_amplitude_get(bc_OrbitPattern* self);
/// The period of the orbit.
uint32_t bc_OrbitPattern_period_get(bc_OrbitPattern* self);
/// The center of the orbit.
uint32_t bc_OrbitPattern_center_get(bc_OrbitPattern* self);
/// Amplitude of the orbit.
void bc_OrbitPattern_amplitude_set(bc_OrbitPattern* self, uint32_t amplitude);
/// The period of the orbit.
void bc_OrbitPattern_period_set(bc_OrbitPattern* self, uint32_t period);
/// The center of the orbit.
void bc_OrbitPattern_center_set(bc_OrbitPattern* self, uint32_t center);
/// Validates the orbit pattern.
/// 
///  * InvalidMapObject - the orbit pattern is invalid.
void bc_OrbitPattern_validate(bc_OrbitPattern* self);
/// Get the duration of flight if the rocket were to take off from either planet on the given round.
uint32_t bc_OrbitPattern_duration(bc_OrbitPattern* self, uint32_t round);
/// Deserialize a OrbitPattern from a JSON string
bc_OrbitPattern* bc_OrbitPattern_from_json(char* s);
/// Serialize a OrbitPattern to a JSON string
char* bc_OrbitPattern_to_json(bc_OrbitPattern* self);
/// The map defining the starting state for an entire game.
typedef struct bc_GameMap bc_GameMap;
/// The map defining the starting state for an entire game.
bc_GameMap* new_bc_GameMap();
/// 
void delete_bc_GameMap(bc_GameMap* self);
/// Seed for random number generation.
uint16_t bc_GameMap_seed_get(bc_GameMap* self);
/// Earth map.
bc_PlanetMap* bc_GameMap_earth_map_get(bc_GameMap* self);
/// Mars map.
bc_PlanetMap* bc_GameMap_mars_map_get(bc_GameMap* self);
/// The asteroid strike pattern on Mars.
bc_AsteroidPattern* bc_GameMap_asteroids_get(bc_GameMap* self);
/// The orbit pattern that determines a rocket's flight duration.
bc_OrbitPattern* bc_GameMap_orbit_get(bc_GameMap* self);
/// Seed for random number generation.
void bc_GameMap_seed_set(bc_GameMap* self, uint16_t seed);
/// Earth map.
void bc_GameMap_earth_map_set(bc_GameMap* self, bc_PlanetMap* earth_map);
/// Mars map.
void bc_GameMap_mars_map_set(bc_GameMap* self, bc_PlanetMap* mars_map);
/// The asteroid strike pattern on Mars.
void bc_GameMap_asteroids_set(bc_GameMap* self, bc_AsteroidPattern* asteroids);
/// The orbit pattern that determines a rocket's flight duration.
void bc_GameMap_orbit_set(bc_GameMap* self, bc_OrbitPattern* orbit);
/// Validate the game map.
/// 
///  * InvalidMapObject - the game map is invalid.
void bc_GameMap_validate(bc_GameMap* self);
/// 
bc_GameMap* bc_GameMap_test_map();
/// parse a .bc18t map file
bc_GameMap* bc_GameMap_parse_text_map(char* map);
/// Deep-copy a GameMap
bc_GameMap* bc_GameMap_clone(bc_GameMap* self);
/// Deserialize a GameMap from a JSON string
bc_GameMap* bc_GameMap_from_json(char* s);
/// Serialize a GameMap to a JSON string
char* bc_GameMap_to_json(bc_GameMap* self);
/// 
uintptr_t max_level(bc_UnitType branch);
/// 
uint32_t cost_of(bc_UnitType branch, uintptr_t level);
/// The status of research for a single team.
typedef struct bc_ResearchInfo bc_ResearchInfo;
/// Construct an initial research state.
bc_ResearchInfo* new_bc_ResearchInfo();
/// 
void delete_bc_ResearchInfo(bc_ResearchInfo* self);
/// Returns the current level of the research branch.
uintptr_t bc_ResearchInfo_get_level(bc_ResearchInfo* self, bc_UnitType branch);
/// Returns the research queue, where the front of the queue is at the beginning of the list.
bc_VecUnitType* bc_ResearchInfo_queue(bc_ResearchInfo* self);
/// Whether there is a branch in the research queue.
uint8_t bc_ResearchInfo_has_next_in_queue(bc_ResearchInfo* self);
/// Returns the next branch to be researched, which is the branch at the front of the research queue.
/// 
///  * NullValue - There is no branch to be researched.
bc_UnitType bc_ResearchInfo_next_in_queue(bc_ResearchInfo* self);
/// Returns the number of rounds left until the upgrade at the front of the research queue is applied.
/// 
///  * NullValue - There is no branch to be researched.
uint32_t bc_ResearchInfo_rounds_left(bc_ResearchInfo* self);
/// Deserialize a ResearchInfo from a JSON string
bc_ResearchInfo* bc_ResearchInfo_from_json(char* s);
/// Serialize a ResearchInfo to a JSON string
char* bc_ResearchInfo_to_json(bc_ResearchInfo* self);
/// 
typedef struct bc_RocketLanding bc_RocketLanding;
/// 
bc_RocketLanding* new_bc_RocketLanding(uint16_t rocket_id, bc_MapLocation* destination);
/// 
void delete_bc_RocketLanding(bc_RocketLanding* self);
/// The ID of the rocket.
uint16_t bc_RocketLanding_rocket_id_get(bc_RocketLanding* self);
/// The landing destination of the rocket.
bc_MapLocation* bc_RocketLanding_destination_get(bc_RocketLanding* self);
/// The ID of the rocket.
void bc_RocketLanding_rocket_id_set(bc_RocketLanding* self, uint16_t rocket_id);
/// The landing destination of the rocket.
void bc_RocketLanding_destination_set(bc_RocketLanding* self, bc_MapLocation* destination);
/// Deep-copy a RocketLanding
bc_RocketLanding* bc_RocketLanding_clone(bc_RocketLanding* self);
/// Create a human-readable representation of a RocketLanding
char* bc_RocketLanding_debug(bc_RocketLanding* self);
/// Deserialize a RocketLanding from a JSON string
bc_RocketLanding* bc_RocketLanding_from_json(char* s);
/// Serialize a RocketLanding to a JSON string
char* bc_RocketLanding_to_json(bc_RocketLanding* self);
/// Compare two RocketLandings for deep equality.
uint8_t bc_RocketLanding_eq(bc_RocketLanding* self, bc_RocketLanding* other);
/// An immutable list of bc::rockets::RocketLanding objects
typedef struct bc_VecRocketLanding bc_VecRocketLanding;
/// An immutable list of bc::rockets::RocketLanding objects
bc_VecRocketLanding* new_bc_VecRocketLanding();
/// 
void delete_bc_VecRocketLanding(bc_VecRocketLanding* self);
/// Create a human-readable representation of a VecRocketLanding
char* bc_VecRocketLanding_debug(bc_VecRocketLanding* self);
/// Deep-copy a VecRocketLanding
bc_VecRocketLanding* bc_VecRocketLanding_clone(bc_VecRocketLanding* self);
/// The length of the vector.
uintptr_t bc_VecRocketLanding_len(bc_VecRocketLanding* self);
/// Copy an element out of the vector.
bc_RocketLanding* bc_VecRocketLanding_index(bc_VecRocketLanding* self, uintptr_t index);
/// 
typedef struct bc_RocketLandingInfo bc_RocketLandingInfo;
/// Construct an empty rocket landing info.
bc_RocketLandingInfo* new_bc_RocketLandingInfo();
/// 
void delete_bc_RocketLandingInfo(bc_RocketLandingInfo* self);
/// Get the rocket landings on self round.
bc_VecRocketLanding* bc_RocketLandingInfo_landings_on(bc_RocketLandingInfo* self, uint32_t round);
/// Deep-copy a RocketLandingInfo
bc_RocketLandingInfo* bc_RocketLandingInfo_clone(bc_RocketLandingInfo* self);
/// Create a human-readable representation of a RocketLandingInfo
char* bc_RocketLandingInfo_debug(bc_RocketLandingInfo* self);
/// Deserialize a RocketLandingInfo from a JSON string
bc_RocketLandingInfo* bc_RocketLandingInfo_from_json(char* s);
/// Serialize a RocketLandingInfo to a JSON string
char* bc_RocketLandingInfo_to_json(bc_RocketLandingInfo* self);
/// Compare two RocketLandingInfos for deep equality.
uint8_t bc_RocketLandingInfo_eq(bc_RocketLandingInfo* self, bc_RocketLandingInfo* other);
/// 
typedef struct bc_GameController bc_GameController;
/// Use environment variables to connect to the manager.
bc_GameController* new_bc_GameController();
/// 
void delete_bc_GameController(bc_GameController* self);
/// Send the moves from the current turn and wait for the next turn.
void bc_GameController_next_turn(bc_GameController* self);
/// Get the time left at the start of self player's turn, in milliseconds.
int32_t bc_GameController_get_time_left_ms(bc_GameController* self);
/// The current round, starting at round 1 and up to ROUND_LIMIT rounds. A round consists of a turn from each team on each planet.
uint32_t bc_GameController_round(bc_GameController* self);
/// The current planet.
bc_Planet bc_GameController_planet(bc_GameController* self);
/// The team whose turn it is.
bc_Team bc_GameController_team(bc_GameController* self);
/// The starting map of the given planet. Includes the map's planet, dimensions, impassable terrain, and initial units and karbonite.
bc_PlanetMap* bc_GameController_starting_map(bc_GameController* self, bc_Planet planet);
/// The karbonite in the team's resource pool.
uint32_t bc_GameController_karbonite(bc_GameController* self);
/// The single unit with self ID. Use self method to get detailed statistics on a unit - heat, cooldowns, and properties of special abilities like units garrisoned in a rocket.
/// 
/// * NoSuchUnit - the unit does not exist (inside the vision range).
bc_Unit* bc_GameController_unit(bc_GameController* self, uint16_t id);
/// All the units within the vision range, in no particular order. Does not include units in space.
bc_VecUnit* bc_GameController_units(bc_GameController* self);
/// All the units on your team. Does not include units in space.
bc_VecUnit* bc_GameController_my_units(bc_GameController* self);
/// All the units of self team that are in space. You cannot see units on the other team that are in space.
bc_VecUnit* bc_GameController_units_in_space(bc_GameController* self);
/// The karbonite at the given location.
/// 
/// * LocationOffMap - the location is off the map.
/// * LocationNotVisible - the location is outside the vision range.
uint32_t bc_GameController_karbonite_at(bc_GameController* self, bc_MapLocation* location);
/// Returns an array of all locations within a certain radius squared of self location that are on the map.
/// 
/// The locations are ordered first by the x-coordinate, then the y-coordinate. The radius squared is inclusive.
bc_VecMapLocation* bc_GameController_all_locations_within(bc_GameController* self, bc_MapLocation* location, uint32_t radius_squared);
/// Whether the location is on the map and within the vision range.
uint8_t bc_GameController_can_sense_location(bc_GameController* self, bc_MapLocation* location);
/// Whether there is a unit with self ID within the vision range.
uint8_t bc_GameController_can_sense_unit(bc_GameController* self, uint16_t id);
/// Sense units near the location within the given radius, inclusive, in distance squared. The units are within the vision range.
bc_VecUnit* bc_GameController_sense_nearby_units(bc_GameController* self, bc_MapLocation* location, uint32_t radius);
/// Sense units near the location within the given radius, inclusive, in distance squared. The units are within the vision range. Additionally filters the units by team.
bc_VecUnit* bc_GameController_sense_nearby_units_by_team(bc_GameController* self, bc_MapLocation* location, uint32_t radius, bc_Team team);
/// Sense units near the location within the given radius, inclusive, in distance squared. The units are within the vision range. Additionally filters the units by unit type.
bc_VecUnit* bc_GameController_sense_nearby_units_by_type(bc_GameController* self, bc_MapLocation* location, uint32_t radius, bc_UnitType unit_type);
/// Whether there is a visible unit at a location.
uint8_t bc_GameController_has_unit_at_location(bc_GameController* self, bc_MapLocation* location);
/// The unit at the location, if it exists.
/// 
/// * LocationOffMap - the location is off the map.
/// * LocationNotVisible - the location is outside the vision range.
bc_Unit* bc_GameController_sense_unit_at_location(bc_GameController* self, bc_MapLocation* location);
/// The asteroid strike pattern on Mars.
bc_AsteroidPattern* bc_GameController_asteroid_pattern(bc_GameController* self);
/// The orbit pattern that determines a rocket's flight duration.
bc_OrbitPattern* bc_GameController_orbit_pattern(bc_GameController* self);
/// The current duration of flight if a rocket were to be launched self round. Does not take into account any research done on rockets.
uint32_t bc_GameController_current_duration_of_flight(bc_GameController* self);
/// Gets a read-only version of self planet's team array. If the given planet is different from the planet of the player, reads the version of the planet's team array from COMMUNICATION_DELAY rounds prior.
bc_Veci32* bc_GameController_get_team_array(bc_GameController* self, bc_Planet planet);
/// Writes the value at the index of self planet's team array.
/// 
/// * ArrayOutOfBounds - the index of the array is out of bounds. It must be within [0, COMMUNICATION_ARRAY_LENGTH).
void bc_GameController_write_team_array(bc_GameController* self, uintptr_t index, int32_t value);
/// Disintegrates the unit and removes it from the map. If the unit is a factory or a rocket, also disintegrates any units garrisoned inside it.
/// 
/// * NoSuchUnit - the unit does not exist (inside the vision range).
/// * TeamNotAllowed - the unit is not on the current player's team.
void bc_GameController_disintegrate_unit(bc_GameController* self, uint16_t unit_id);
/// Whether the location is clear for a unit to occupy, either by movement or by construction.
/// 
/// * LocationOffMap - the location is off the map.
/// * LocationNotVisible - the location is outside the vision range.
uint8_t bc_GameController_is_occupiable(bc_GameController* self, bc_MapLocation* location);
/// Whether the robot can move in the given direction, without taking into account the unit's movement heat. Takes into account only the map terrain, positions of other robots, and the edge of the game map.
uint8_t bc_GameController_can_move(bc_GameController* self, uint16_t robot_id, bc_Direction direction);
/// Whether the robot is ready to move. Tests whether the robot's attack heat is sufficiently low.
uint8_t bc_GameController_is_move_ready(bc_GameController* self, uint16_t robot_id);
/// Moves the robot in the given direction.
/// 
/// * NoSuchUnit - the robot does not exist (within the vision range).
/// * TeamNotAllowed - the robot is not on the current player's team.
/// * UnitNotOnMap - the robot is not on the map.
/// * LocationNotVisible - the location is outside the vision range.
/// * LocationOffMap - the location is off the map.
/// * LocationNotEmpty - the location is occupied by a unit or terrain.
/// * Overheated - the robot is not ready to move again.
void bc_GameController_move_robot(bc_GameController* self, uint16_t robot_id, bc_Direction direction);
/// Whether the robot can attack the given unit, without taking into account the robot's attack heat. Takes into account only the robot's attack range, and the location of the robot and target.
/// 
/// Healers cannot attack, and should use can_heal() instead.
uint8_t bc_GameController_can_attack(bc_GameController* self, uint16_t robot_id, uint16_t target_unit_id);
/// Whether the robot is ready to attack. Tests whether the robot's attack heat is sufficiently low.
/// 
/// Healers cannot attack, and should use is_heal_ready() instead.
uint8_t bc_GameController_is_attack_ready(bc_GameController* self, uint16_t robot_id);
/// Commands a robot to attack a unit, dealing the robot's standard amount of damage.
/// 
/// Healers cannot attack, and should use heal() instead.
/// 
/// * NoSuchUnit - the unit does not exist (inside the vision range).
/// * TeamNotAllowed - the unit is not on the current player's team.
/// * InappropriateUnitType - the unit is not a robot, or is a healer.
/// * UnitNotOnMap - the unit or target is not on the map.
/// * OutOfRange - the target location is not in range.
/// * Overheated - the unit is not ready to attack.
void bc_GameController_attack(bc_GameController* self, uint16_t robot_id, uint16_t target_unit_id);
/// The research info of the current team, including what branch is currently being researched, the number of rounds left.
bc_ResearchInfo* bc_GameController_research_info(bc_GameController* self);
/// Resets the research queue to be empty. Returns true if the queue was not empty before, and false otherwise.
uint8_t bc_GameController_reset_research(bc_GameController* self);
/// Adds a branch to the back of the queue, if it is a valid upgrade, and starts research if it is the first in the queue.
/// 
/// Returns whether the branch was successfully added.
uint8_t bc_GameController_queue_research(bc_GameController* self, bc_UnitType branch);
/// Whether the worker is ready to harvest, and the given direction contains karbonite to harvest. The worker cannot already have performed an action self round.
uint8_t bc_GameController_can_harvest(bc_GameController* self, uint16_t worker_id, bc_Direction direction);
/// Harvests up to the worker's harvest amount of karbonite from the given location, adding it to the team's resource pool.
/// 
/// * NoSuchUnit - the worker does not exist (within the vision range).
/// * TeamNotAllowed - the worker is not on the current player's team.
/// * InappropriateUnitType - the unit is not a worker.
/// * Overheated - the worker has already performed an action self turn.
/// * UnitNotOnMap - the worker is not on the map.
/// * LocationOffMap - the location in the target direction is off the map.
/// * LocationNotVisible - the location is not in the vision range.
/// * KarboniteDepositEmpty - the location described contains no Karbonite.
void bc_GameController_harvest(bc_GameController* self, uint16_t worker_id, bc_Direction direction);
/// Whether the worker can blueprint a unit of the given type. The worker can only blueprint factories, and rockets if Rocketry has been researched. The team must have sufficient karbonite in its resource pool. The worker cannot already have performed an action self round.
uint8_t bc_GameController_can_blueprint(bc_GameController* self, uint16_t worker_id, bc_UnitType unit_type, bc_Direction direction);
/// Blueprints a unit of the given type in the given direction. Subtract cost of that unit from the team's resource pool.
/// 
/// * NoSuchUnit - the worker does not exist (within the vision range).
/// * TeamNotAllowed - the worker is not on the current player's team.
/// * InappropriateUnitType - the unit is not a worker, or the unit type is not a structure.
/// * Overheated - the worker has already performed an action self turn.
/// * UnitNotOnMap - the unit is not on the map.
/// * LocationOffMap - the location in the target direction is off the map.
/// * LocationNotVisible - the location is outside the vision range.
/// * LocationNotEmpty - the location in the target direction is already occupied.
/// * CannotBuildOnMars - you cannot blueprint a structure on Mars.
/// * ResearchNotUnlocked - you do not have the needed research to blueprint rockets.
/// * InsufficientKarbonite - your team does not have enough Karbonite to build the requested structure.
void bc_GameController_blueprint(bc_GameController* self, uint16_t worker_id, bc_UnitType structure_type, bc_Direction direction);
/// Whether the worker can build a blueprint with the given ID. The worker and the blueprint must be adjacent to each other. The worker cannot already have performed an action self round.
uint8_t bc_GameController_can_build(bc_GameController* self, uint16_t worker_id, uint16_t blueprint_id);
/// Builds a given blueprint, increasing its health by the worker's build amount. If raised to maximum health, the blueprint becomes a completed structure.
/// 
/// * NoSuchUnit - either unit does not exist (within the vision range).
/// * TeamNotAllowed - either unit is not on the current player's team.
/// * UnitNotOnMap - the worker is not on the map.
/// * InappropriateUnitType - the unit is not a worker, or the blueprint is not a structure.
/// * Overheated - the worker has already performed an action self turn.
/// * OutOfRange - the worker is not adjacent to the blueprint.
/// * StructureAlreadyBuilt - the blueprint has already been completed.
void bc_GameController_build(bc_GameController* self, uint16_t worker_id, uint16_t blueprint_id);
/// Whether the given worker can repair the given strucutre. Tests that the worker is able to execute a worker action, that the structure is built, and that the structure is within range.
uint8_t bc_GameController_can_repair(bc_GameController* self, uint16_t worker_id, uint16_t structure_id);
/// Commands the worker to repair a structure, repleneshing health to it. self can only be done to structures which have been fully built.
/// 
/// * NoSuchUnit - either unit does not exist (within the vision range).
/// * TeamNotAllowed - either unit is not on the current player's team.
/// * UnitNotOnMap - the worker is not on the map.
/// * InappropriateUnitType - the unit is not a worker, or the target is not a structure.
/// * Overheated - the worker has already performed an action self turn.
/// * OutOfRange - the worker is not adjacent to the structure.
/// * StructureNotYetBuilt - the structure has not been completed.
void bc_GameController_repair(bc_GameController* self, uint16_t worker_id, uint16_t structure_id);
/// Whether the worker is ready to replicate. Tests that the worker's ability heat is sufficiently low, that the team has sufficient karbonite in its resource pool, and that the square in the given direction is empty.
uint8_t bc_GameController_can_replicate(bc_GameController* self, uint16_t worker_id, bc_Direction direction);
/// Replicates a worker in the given direction. Subtracts the cost of the worker from the team's resource pool.
/// 
/// * NoSuchUnit - the worker does not exist (within the vision range).
/// * TeamNotAllowed - the worker is not on the current player's team.
/// * InappropriateUnitType - the unit is not a worker.
/// * Overheated - the worker is not ready to replicate again.
/// * InsufficientKarbonite - your team does not have enough Karbonite for the worker to replicate.
/// * UnitNotOnMap - the worker is not on the map.
/// * LocationOffMap - the location in the target direction is off the map.
/// * LocationNotVisible - the location is outside the vision range.
/// * LocationNotEmpty - the location in the target direction is already occupied.
void bc_GameController_replicate(bc_GameController* self, uint16_t worker_id, bc_Direction direction);
/// Whether the knight can javelin the given robot, without taking into account the knight's ability heat. Takes into account only the knight's ability range, and the location of the robot.
uint8_t bc_GameController_can_javelin(bc_GameController* self, uint16_t knight_id, uint16_t target_unit_id);
/// Whether the knight is ready to javelin. Tests whether the knight's ability heat is sufficiently low.
uint8_t bc_GameController_is_javelin_ready(bc_GameController* self, uint16_t knight_id);
/// Javelins the robot, dealing the knight's standard damage.
/// 
/// * NoSuchUnit - either unit does not exist (inside the vision range).
/// * TeamNotAllowed - the knight is not on the current player's team.
/// * UnitNotOnMap - the knight is not on the map.
/// * InappropriateUnitType - the unit is not a knight.
/// * ResearchNotUnlocked - you do not have the needed research to use javelin.
/// * OutOfRange - the target does not lie within ability range of the knight.
/// * Overheated - the knight is not ready to use javelin again.
void bc_GameController_javelin(bc_GameController* self, uint16_t knight_id, uint16_t target_unit_id);
/// Whether the ranger can begin to snipe the given location, without taking into account the ranger's ability heat. Takes into account only the target location and the unit's type and unlocked abilities.
uint8_t bc_GameController_can_begin_snipe(bc_GameController* self, uint16_t ranger_id, bc_MapLocation* location);
/// Whether the ranger is ready to begin snipe. Tests whether the ranger's ability heat is sufficiently low.
uint8_t bc_GameController_is_begin_snipe_ready(bc_GameController* self, uint16_t ranger_id);
/// Begins the countdown to snipe a given location. Maximizes the units attack and movement heats until the ranger has sniped. The ranger may begin the countdown at any time, including resetting the countdown to snipe a different location.
/// 
/// * NoSuchUnit - either unit does not exist (inside the vision range).
/// * TeamNotAllowed - the ranger is not on the current player's team.
/// * UnitNotOnMap - the ranger is not on the map.
/// * InappropriateUnitType - the unit is not a ranger.
/// * ResearchNotUnlocked - you do not have the needed research to use snipe.
/// * Overheated - the ranger is not ready to use snipe again.
void bc_GameController_begin_snipe(bc_GameController* self, uint16_t ranger_id, bc_MapLocation* location);
/// Whether the mage can blink to the given location, without taking into account the mage's ability heat. Takes into account only the mage's ability range, the map terrain, positions of other units, and the edge of the game map.
uint8_t bc_GameController_can_blink(bc_GameController* self, uint16_t mage_id, bc_MapLocation* location);
/// Whether the mage is ready to blink. Tests whether the mage's ability heat is sufficiently low.
uint8_t bc_GameController_is_blink_ready(bc_GameController* self, uint16_t mage_id);
/// Blinks the mage to the given location.
/// 
/// * NoSuchUnit - the mage does not exist (inside the vision range).
/// * TeamNotAllowed - the mage is not on the current player's team.
/// * UnitNotOnMap - the mage is not on the map.
/// * InappropriateUnitType - the unit is not a mage.
/// * ResearchNotUnlocked - you do not have the needed research to use blink.
/// * OutOfRange - the target does not lie within ability range of the mage.
/// * LocationOffMap - the target location is not on self planet's map.
/// * LocationNotVisible - the target location is outside the vision range.
/// * LocationNotEmpty - the target location is already occupied.
/// * Overheated - the mage is not ready to use blink again.
void bc_GameController_blink(bc_GameController* self, uint16_t mage_id, bc_MapLocation* location);
/// Whether the healer can heal the given robot, without taking into account the healer's attack heat. Takes into account only the healer's attack range, and the location of the robot.
uint8_t bc_GameController_can_heal(bc_GameController* self, uint16_t healer_id, uint16_t target_robot_id);
/// Whether the healer is ready to heal. Tests whether the healer's attack heat is sufficiently low.
uint8_t bc_GameController_is_heal_ready(bc_GameController* self, uint16_t healer_id);
/// Commands the healer to heal the target robot.
/// 
/// * NoSuchUnit - either unit does not exist (inside the vision range).
/// * InappropriateUnitType - the unit is not a healer, or the target is not a robot.
/// * TeamNotAllowed - either robot is not on the current player's team.
/// * UnitNotOnMap - the healer is not on the map.
/// * OutOfRange - the target does not lie within "attack" range of the healer.
/// * Overheated - the healer is not ready to heal again.
void bc_GameController_heal(bc_GameController* self, uint16_t healer_id, uint16_t target_robot_id);
/// Whether the healer can overcharge the given robot, without taking into account the healer's ability heat. Takes into account only the healer's ability range, and the location of the robot.
uint8_t bc_GameController_can_overcharge(bc_GameController* self, uint16_t healer_id, uint16_t target_robot_id);
/// Whether the healer is ready to overcharge. Tests whether the healer's ability heat is sufficiently low.
uint8_t bc_GameController_is_overcharge_ready(bc_GameController* self, uint16_t healer_id);
/// Overcharges the robot, resetting the robot's cooldowns. The robot must be on the same team as you.
/// 
/// * NoSuchUnit - either unit does not exist (inside the vision range).
/// * TeamNotAllowed - either robot is not on the current player's team.
/// * UnitNotOnMap - the healer is not on the map.
/// * InappropriateUnitType - the unit is not a healer, or the target is not a robot.
/// * ResearchNotUnlocked - you do not have the needed research to use overcharge.
/// * OutOfRange - the target does not lie within ability range of the healer.
/// * Overheated - the healer is not ready to use overcharge again.
void bc_GameController_overcharge(bc_GameController* self, uint16_t healer_id, uint16_t target_robot_id);
/// Whether the robot can be loaded into the given structure's garrison. The robot must be ready to move and must be adjacent to the structure. The structure and the robot must be on the same team, and the structure must have space.
uint8_t bc_GameController_can_load(bc_GameController* self, uint16_t structure_id, uint16_t robot_id);
/// Loads the robot into the garrison of the structure.
/// 
/// * NoSuchUnit - either unit does not exist (inside the vision range).
/// * TeamNotAllowed - either unit is not on the current player's team.
/// * UnitNotOnMap - either unit is not on the map.
/// * Overheated - the robot is not ready to move again.
/// * InappropriateUnitType - the first unit is not a structure, or the second unit is not a robot.
/// * StructureNotYetBuilt - the structure has not yet been completed.
/// * GarrisonFull - the structure's garrison is already full.
/// * OutOfRange - the robot is not adjacent to the structure.
void bc_GameController_load(bc_GameController* self, uint16_t structure_id, uint16_t robot_id);
/// Tests whether the given structure is able to unload a unit in the given direction. There must be space in that direction, and the unit must be ready to move.
uint8_t bc_GameController_can_unload(bc_GameController* self, uint16_t structure_id, bc_Direction direction);
/// Unloads a robot from the garrison of the specified structure into an adjacent space. Robots are unloaded in the order they were loaded.
/// 
/// * NoSuchUnit - the unit does not exist (inside the vision range).
/// * TeamNotAllowed - either unit is not on the current player's team.
/// * UnitNotOnMap - the structure is not on the map.
/// * InappropriateUnitType - the unit is not a structure.
/// * StructureNotYetBuilt - the structure has not yet been completed.
/// * GarrisonEmpty - the structure's garrison is already empty.
/// * LocationOffMap - the location in the target direction is off the map.
/// * LocationNotEmpty - the location in the target direction is already occupied.
/// * Overheated - the robot inside the structure is not ready to move again.
void bc_GameController_unload(bc_GameController* self, uint16_t structure_id, bc_Direction direction);
/// Whether the factory can produce a robot of the given type. The factory must not currently be producing a robot, and the team must have sufficient resources in its resource pool.
uint8_t bc_GameController_can_produce_robot(bc_GameController* self, uint16_t factory_id, bc_UnitType robot_type);
/// Starts producing the robot of the given type.
/// 
/// * NoSuchUnit - the factory does not exist (inside the vision range).
/// * TeamNotAllowed - the factory is not on the current player's team.
/// * InappropriateUnitType - the unit is not a factory, or the unit type is not a robot.
/// * StructureNotYetBuilt - the factory has not yet been completed.
/// * FactoryBusy - the factory is already producing a unit.
/// * InsufficientKarbonite - your team does not have enough Karbonite to produce the given robot.
void bc_GameController_produce_robot(bc_GameController* self, uint16_t factory_id, bc_UnitType robot_type);
/// The landing rounds and locations of rockets in space that belong to the current team.
bc_RocketLandingInfo* bc_GameController_rocket_landings(bc_GameController* self);
/// Whether the rocket can launch into space to the given destination. The rocket can launch if the it has never been used before. The destination is valid if it contains passable terrain on the other planet.
uint8_t bc_GameController_can_launch_rocket(bc_GameController* self, uint16_t rocket_id, bc_MapLocation* destination);
/// Launches the rocket into space, damaging the units adjacent to the takeoff location.
/// 
/// * NoSuchUnit - the rocket does not exist (inside the vision range).
/// * TeamNotAllowed - the rocket is not on the current player's team.
/// * SamePlanet - the rocket cannot fly to a location on the same planet.
/// * InappropriateUnitType - the unit is not a rocket.
/// * StructureNotYetBuilt - the rocket has not yet been completed.
/// * RocketUsed - the rocket has already been used.
/// * LocationOffMap - the given location is off the map.
/// * LocationNotEmpty - the given location contains impassable terrain.
void bc_GameController_launch_rocket(bc_GameController* self, uint16_t rocket_id, bc_MapLocation* location);
/// 
bc_GameController* bc_GameController_new_manager(bc_GameMap* map);
/// 
bc_StartGameMessage* bc_GameController_start_game(bc_GameController* self, bc_Player* player);
/// 
bc_TurnApplication* bc_GameController_apply_turn(bc_GameController* self, bc_TurnMessage* turn, int32_t time_left_ms);
/// 
bc_InitialTurnApplication* bc_GameController_initial_start_turn_message(bc_GameController* self, int32_t time_left_ms);
/// 
uint8_t bc_GameController_is_over(bc_GameController* self);
/// 
bc_Team bc_GameController_winning_team(bc_GameController* self);
/// 
char* bc_GameController_manager_viewer_message(bc_GameController* self);
/// 
void bc_GameController_print_game_ansi(bc_GameController* self);
/// 
uint32_t bc_GameController_manager_karbonite(bc_GameController* self, bc_Team team);
#ifdef __cplusplus
}
#endif
#endif // bc_h_
