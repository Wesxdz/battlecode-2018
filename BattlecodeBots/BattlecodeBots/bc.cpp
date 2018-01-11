#include "bc.h"

// Why does God hurt innocent people?

void bc_GameController_launch_rocket(bc_GameController * self, uint16_t rocket_id, bc_MapLocation * location)
{
}

bc_GameController * bc_GameController_new_manager(bc_GameMap * map)
{
	return nullptr;
}

bc_StartGameMessage * bc_GameController_start_game(bc_GameController * self, bc_Player * player)
{
	return nullptr;
}

bc_TurnApplication * bc_GameController_apply_turn(bc_GameController * self, bc_TurnMessage * turn)
{
	return nullptr;
}

bc_InitialTurnApplication * bc_GameController_initial_start_turn_message(bc_GameController * self)
{
	return nullptr;
}

uint8_t bc_GameController_is_over(bc_GameController * self)
{
	return uint8_t();
}

bc_Team bc_GameController_winning_team(bc_GameController * self)
{
	return bc_Team();
}

uint8_t bc_has_err()
{
	return uint8_t();
}

int8_t bc_get_last_err(char ** result)
{
	return int8_t();
}

int8_t bc_free_string(char * err)
{
	return int8_t();
}

bc_Planet bc_Planet_other(bc_Planet self)
{
	return bc_Planet();
}

char * bc_Planet_debug(bc_Planet self)
{
	return nullptr;
}

uint8_t bc_Planet_eq(bc_Planet self, bc_Planet other)
{
	return uint8_t();
}

bc_Planet bc_Planet_from_json(char * s)
{
	return bc_Planet();
}

char * bc_Planet_to_json(bc_Planet self)
{
	return nullptr;
}

int32_t bc_Direction_dx(bc_Direction self)
{
	return int32_t();
}

int32_t bc_Direction_dy(bc_Direction self)
{
	return int32_t();
}

uint8_t bc_Direction_is_diagonal(bc_Direction self)
{
	return uint8_t();
}

bc_Direction bc_Direction_opposite(bc_Direction self)
{
	return bc_Direction();
}

bc_Direction bc_Direction_rotate_left(bc_Direction self)
{
	return bc_Direction();
}

bc_Direction bc_Direction_rotate_right(bc_Direction self)
{
	return bc_Direction();
}

bc_Direction bc_Direction_from_json(char * s)
{
	return bc_Direction();
}

char * bc_Direction_to_json(bc_Direction self)
{
	return nullptr;
}

bc_MapLocation * new_bc_MapLocation(bc_Planet planet, int32_t x, int32_t y)
{
	return nullptr;
}

void delete_bc_MapLocation(bc_MapLocation * self)
{
}

bc_Planet bc_MapLocation_planet_get(bc_MapLocation * self)
{
	return bc_Planet();
}

int32_t bc_MapLocation_x_get(bc_MapLocation * self)
{
	return int32_t();
}

int32_t bc_MapLocation_y_get(bc_MapLocation * self)
{
	return int32_t();
}

void bc_MapLocation_planet_set(bc_MapLocation * self, bc_Planet planet)
{
}

void bc_MapLocation_x_set(bc_MapLocation * self, int32_t x)
{
}

void bc_MapLocation_y_set(bc_MapLocation * self, int32_t y)
{
}

bc_MapLocation * bc_MapLocation_add(bc_MapLocation * self, bc_Direction direction)
{
	return nullptr;
}

bc_MapLocation * bc_MapLocation_subtract(bc_MapLocation * self, bc_Direction direction)
{
	return nullptr;
}

bc_MapLocation * bc_MapLocation_add_multiple(bc_MapLocation * self, bc_Direction direction, int32_t multiple)
{
	return nullptr;
}

bc_MapLocation * bc_MapLocation_translate(bc_MapLocation * self, int32_t dx, int32_t dy)
{
	return nullptr;
}

uint32_t bc_MapLocation_distance_squared_to(bc_MapLocation * self, bc_MapLocation * o)
{
	return uint32_t();
}

bc_Direction bc_MapLocation_direction_to(bc_MapLocation * self, bc_MapLocation * o)
{
	return bc_Direction();
}

uint8_t bc_MapLocation_is_adjacent_to(bc_MapLocation * self, bc_MapLocation * o)
{
	return uint8_t();
}

uint8_t bc_MapLocation_is_within_range(bc_MapLocation * self, uint32_t range, bc_MapLocation * o)
{
	return uint8_t();
}

char * bc_MapLocation_debug(bc_MapLocation * self)
{
	return nullptr;
}

bc_MapLocation * bc_MapLocation_clone(bc_MapLocation * self)
{
	return nullptr;
}

uint8_t bc_MapLocation_eq(bc_MapLocation * self, bc_MapLocation * other)
{
	return uint8_t();
}

bc_MapLocation * bc_MapLocation_from_json(char * s)
{
	return nullptr;
}

char * bc_MapLocation_to_json(bc_MapLocation * self)
{
	return nullptr;
}

bc_VecMapLocation * new_bc_VecMapLocation()
{
	return nullptr;
}

void delete_bc_VecMapLocation(bc_VecMapLocation * self)
{
}

char * bc_VecMapLocation_debug(bc_VecMapLocation * self)
{
	return nullptr;
}

bc_VecMapLocation * bc_VecMapLocation_clone(bc_VecMapLocation * self)
{
	return nullptr;
}

uintptr_t bc_VecMapLocation_len(bc_VecMapLocation * self)
{
	return uintptr_t();
}

bc_MapLocation * bc_VecMapLocation_index(bc_VecMapLocation * self, uintptr_t index)
{
	return nullptr;
}

bc_Veci32 * new_bc_Veci32()
{
	return nullptr;
}

void delete_bc_Veci32(bc_Veci32 * self)
{
}

char * bc_Veci32_debug(bc_Veci32 * self)
{
	return nullptr;
}

bc_Veci32 * bc_Veci32_clone(bc_Veci32 * self)
{
	return nullptr;
}

uintptr_t bc_Veci32_len(bc_Veci32 * self)
{
	return uintptr_t();
}

int32_t bc_Veci32_index(bc_Veci32 * self, uintptr_t index)
{
	return int32_t();
}

bc_Location * new_bc_Location()
{
	return nullptr;
}

void delete_bc_Location(bc_Location * self)
{
}

bc_Location * bc_Location_new_on_map(bc_MapLocation * map_location)
{
	return nullptr;
}

bc_Location * bc_Location_new_in_garrison(uint16_t id)
{
	return nullptr;
}

bc_Location * bc_Location_new_in_space()
{
	return nullptr;
}

uint8_t bc_Location_is_on_map(bc_Location * self)
{
	return uint8_t();
}

uint8_t bc_Location_is_on_planet(bc_Location * self, bc_Planet planet)
{
	return uint8_t();
}

bc_MapLocation * bc_Location_map_location(bc_Location * self)
{
	return nullptr;
}

uint8_t bc_Location_is_in_garrison(bc_Location * self)
{
	return uint8_t();
}

uint16_t bc_Location_structure(bc_Location * self)
{
	return uint16_t();
}

uint8_t bc_Location_is_in_space(bc_Location * self)
{
	return uint8_t();
}

uint8_t bc_Location_is_adjacent_to(bc_Location * self, bc_Location * o)
{
	return uint8_t();
}

uint8_t bc_Location_is_within_range(bc_Location * self, uint32_t range, bc_Location * o)
{
	return uint8_t();
}

char * bc_Location_debug(bc_Location * self)
{
	return nullptr;
}

bc_Location * bc_Location_clone(bc_Location * self)
{
	return nullptr;
}

uint8_t bc_Location_eq(bc_Location * self, bc_Location * other)
{
	return uint8_t();
}

bc_Location * bc_Location_from_json(char * s)
{
	return nullptr;
}

char * bc_Location_to_json(bc_Location * self)
{
	return nullptr;
}

bc_Team bc_Team_from_json(char * s)
{
	return bc_Team();
}

char * bc_Team_to_json(bc_Team self)
{
	return nullptr;
}

bc_Player * new_bc_Player(bc_Team team, bc_Planet planet)
{
	return nullptr;
}

void delete_bc_Player(bc_Player * self)
{
}

bc_Team bc_Player_team_get(bc_Player * self)
{
	return bc_Team();
}

bc_Planet bc_Player_planet_get(bc_Player * self)
{
	return bc_Planet();
}

void bc_Player_team_set(bc_Player * self, bc_Team team)
{
}

void bc_Player_planet_set(bc_Player * self, bc_Planet planet)
{
}

char * bc_Player_debug(bc_Player * self)
{
	return nullptr;
}

bc_Player * bc_Player_clone(bc_Player * self)
{
	return nullptr;
}

uint8_t bc_Player_eq(bc_Player * self, bc_Player * other)
{
	return uint8_t();
}

bc_Player * bc_Player_from_json(char * s)
{
	return nullptr;
}

char * bc_Player_to_json(bc_Player * self)
{
	return nullptr;
}

bc_VecUnitID * new_bc_VecUnitID()
{
	return nullptr;
}

void delete_bc_VecUnitID(bc_VecUnitID * self)
{
}

char * bc_VecUnitID_debug(bc_VecUnitID * self)
{
	return nullptr;
}

bc_VecUnitID * bc_VecUnitID_clone(bc_VecUnitID * self)
{
	return nullptr;
}

uintptr_t bc_VecUnitID_len(bc_VecUnitID * self)
{
	return uintptr_t();
}

uint16_t bc_VecUnitID_index(bc_VecUnitID * self, uintptr_t index)
{
	return uint16_t();
}

bc_UnitType bc_UnitType_from_json(char * s)
{
	return bc_UnitType();
}

char * bc_UnitType_to_json(bc_UnitType self)
{
	return nullptr;
}

uint32_t bc_UnitType_factory_cost(bc_UnitType self)
{
	return uint32_t();
}

uint32_t bc_UnitType_blueprint_cost(bc_UnitType self)
{
	return uint32_t();
}

uint32_t bc_UnitType_replicate_cost(bc_UnitType self)
{
	return uint32_t();
}

uint32_t bc_UnitType_value(bc_UnitType self)
{
	return uint32_t();
}

bc_VecUnitType * new_bc_VecUnitType()
{
	return nullptr;
}

void delete_bc_VecUnitType(bc_VecUnitType * self)
{
}

char * bc_VecUnitType_debug(bc_VecUnitType * self)
{
	return nullptr;
}

bc_VecUnitType * bc_VecUnitType_clone(bc_VecUnitType * self)
{
	return nullptr;
}

uintptr_t bc_VecUnitType_len(bc_VecUnitType * self)
{
	return uintptr_t();
}

bc_UnitType bc_VecUnitType_index(bc_VecUnitType * self, uintptr_t index)
{
	return bc_UnitType();
}

bc_Unit * new_bc_Unit()
{
	return nullptr;
}

void delete_bc_Unit(bc_Unit * self)
{
}

char * bc_Unit_debug(bc_Unit * self)
{
	return nullptr;
}

bc_Unit * bc_Unit_clone(bc_Unit * self)
{
	return nullptr;
}

bc_Unit * bc_Unit_from_json(char * s)
{
	return nullptr;
}

char * bc_Unit_to_json(bc_Unit * self)
{
	return nullptr;
}

uint8_t bc_Unit_eq(bc_Unit * self, bc_Unit * other)
{
	return uint8_t();
}

uint16_t bc_Unit_id(bc_Unit * self)
{
	return uint16_t();
}

bc_Team bc_Unit_team(bc_Unit * self)
{
	return bc_Team();
}

uintptr_t bc_Unit_research_level(bc_Unit * self)
{
	return uintptr_t();
}

bc_UnitType bc_Unit_unit_type(bc_Unit * self)
{
	return bc_UnitType();
}

bc_Location * bc_Unit_location(bc_Unit * self)
{
	return nullptr;
}

uint32_t bc_Unit_health(bc_Unit * self)
{
	return uint32_t();
}

uint32_t bc_Unit_max_health(bc_Unit * self)
{
	return uint32_t();
}

uint32_t bc_Unit_vision_range(bc_Unit * self)
{
	return uint32_t();
}

int32_t bc_Unit_damage(bc_Unit * self)
{
	return int32_t();
}

uint32_t bc_Unit_attack_range(bc_Unit * self)
{
	return uint32_t();
}

uint32_t bc_Unit_movement_heat(bc_Unit * self)
{
	return uint32_t();
}

uint32_t bc_Unit_attack_heat(bc_Unit * self)
{
	return uint32_t();
}

uint32_t bc_Unit_movement_cooldown(bc_Unit * self)
{
	return uint32_t();
}

uint32_t bc_Unit_attack_cooldown(bc_Unit * self)
{
	return uint32_t();
}

uint8_t bc_Unit_is_ability_unlocked(bc_Unit * self)
{
	return uint8_t();
}

uint32_t bc_Unit_ability_heat(bc_Unit * self)
{
	return uint32_t();
}

uint32_t bc_Unit_ability_cooldown(bc_Unit * self)
{
	return uint32_t();
}

uint32_t bc_Unit_ability_range(bc_Unit * self)
{
	return uint32_t();
}

uint8_t bc_Unit_worker_has_acted(bc_Unit * self)
{
	return uint8_t();
}

uint32_t bc_Unit_worker_build_health(bc_Unit * self)
{
	return uint32_t();
}

uint32_t bc_Unit_worker_repair_health(bc_Unit * self)
{
	return uint32_t();
}

uint32_t bc_Unit_worker_harvest_amount(bc_Unit * self)
{
	return uint32_t();
}

uint32_t bc_Unit_knight_defense(bc_Unit * self)
{
	return uint32_t();
}

uint32_t bc_Unit_ranger_cannot_attack_range(bc_Unit * self)
{
	return uint32_t();
}

uint32_t bc_Unit_ranger_max_countdown(bc_Unit * self)
{
	return uint32_t();
}

uint8_t bc_Unit_ranger_is_sniping(bc_Unit * self)
{
	return uint8_t();
}

bc_MapLocation * bc_Unit_ranger_target_location(bc_Unit * self)
{
	return nullptr;
}

uint32_t bc_Unit_ranger_countdown(bc_Unit * self)
{
	return uint32_t();
}

uint32_t bc_Unit_healer_self_heal_amount(bc_Unit * self)
{
	return uint32_t();
}

uint8_t bc_Unit_structure_is_built(bc_Unit * self)
{
	return uint8_t();
}

uintptr_t bc_Unit_structure_max_capacity(bc_Unit * self)
{
	return uintptr_t();
}

bc_VecUnitID * bc_Unit_structure_garrison(bc_Unit * self)
{
	return nullptr;
}

uint8_t bc_Unit_is_factory_producing(bc_Unit * self)
{
	return uint8_t();
}

bc_UnitType bc_Unit_factory_unit_type(bc_Unit * self)
{
	return bc_UnitType();
}

uint32_t bc_Unit_factory_rounds_left(bc_Unit * self)
{
	return uint32_t();
}

uint32_t bc_Unit_factory_max_rounds_left(bc_Unit * self)
{
	return uint32_t();
}

uint8_t bc_Unit_rocket_is_used(bc_Unit * self)
{
	return uint8_t();
}

int32_t bc_Unit_rocket_blast_damage(bc_Unit * self)
{
	return int32_t();
}

uint32_t bc_Unit_rocket_travel_time_decrease(bc_Unit * self)
{
	return uint32_t();
}

bc_VecUnit * new_bc_VecUnit()
{
	return nullptr;
}

void delete_bc_VecUnit(bc_VecUnit * self)
{
}

char * bc_VecUnit_debug(bc_VecUnit * self)
{
	return nullptr;
}

bc_VecUnit * bc_VecUnit_clone(bc_VecUnit * self)
{
	return nullptr;
}

uintptr_t bc_VecUnit_len(bc_VecUnit * self)
{
	return uintptr_t();
}

bc_Unit * bc_VecUnit_index(bc_VecUnit * self, uintptr_t index)
{
	return nullptr;
}

bc_PlanetMap * new_bc_PlanetMap()
{
	return nullptr;
}

void delete_bc_PlanetMap(bc_PlanetMap * self)
{
}

bc_Planet bc_PlanetMap_planet_get(bc_PlanetMap * self)
{
	return bc_Planet();
}

uintptr_t bc_PlanetMap_height_get(bc_PlanetMap * self)
{
	return uintptr_t();
}

uintptr_t bc_PlanetMap_width_get(bc_PlanetMap * self)
{
	return uintptr_t();
}

bc_VecUnit * bc_PlanetMap_initial_units_get(bc_PlanetMap * self)
{
	return nullptr;
}

void bc_PlanetMap_planet_set(bc_PlanetMap * self, bc_Planet planet)
{
}

void bc_PlanetMap_height_set(bc_PlanetMap * self, uintptr_t height)
{
}

void bc_PlanetMap_width_set(bc_PlanetMap * self, uintptr_t width)
{
}

void bc_PlanetMap_initial_units_set(bc_PlanetMap * self, bc_VecUnit * initial_units)
{
}

void bc_PlanetMap_validate(bc_PlanetMap * self)
{
}

uint8_t bc_PlanetMap_on_map(bc_PlanetMap * self, bc_MapLocation * location)
{
	return uint8_t();
}

uint8_t bc_PlanetMap_is_passable_terrain_at(bc_PlanetMap * self, bc_MapLocation * location)
{
	return uint8_t();
}

uint32_t bc_PlanetMap_initial_karbonite_at(bc_PlanetMap * self, bc_MapLocation * location)
{
	return uint32_t();
}

bc_PlanetMap * bc_PlanetMap_clone(bc_PlanetMap * self)
{
	return nullptr;
}

bc_PlanetMap * bc_PlanetMap_from_json(char * s)
{
	return nullptr;
}

char * bc_PlanetMap_to_json(bc_PlanetMap * self)
{
	return nullptr;
}

bc_Delta * new_bc_Delta()
{
	return nullptr;
}

void delete_bc_Delta(bc_Delta * self)
{
}

bc_Delta * bc_Delta_from_json(char * s)
{
	return nullptr;
}

char * bc_Delta_to_json(bc_Delta * self)
{
	return nullptr;
}

bc_StartGameMessage * new_bc_StartGameMessage()
{
	return nullptr;
}

void delete_bc_StartGameMessage(bc_StartGameMessage * self)
{
}

bc_StartGameMessage * bc_StartGameMessage_from_json(char * s)
{
	return nullptr;
}

char * bc_StartGameMessage_to_json(bc_StartGameMessage * self)
{
	return nullptr;
}

bc_TurnMessage * new_bc_TurnMessage()
{
	return nullptr;
}

void delete_bc_TurnMessage(bc_TurnMessage * self)
{
}

bc_TurnMessage * bc_TurnMessage_from_json(char * s)
{
	return nullptr;
}

char * bc_TurnMessage_to_json(bc_TurnMessage * self)
{
	return nullptr;
}

bc_StartTurnMessage * new_bc_StartTurnMessage()
{
	return nullptr;
}

void delete_bc_StartTurnMessage(bc_StartTurnMessage * self)
{
}

uint32_t bc_StartTurnMessage_round_get(bc_StartTurnMessage * self)
{
	return uint32_t();
}

void bc_StartTurnMessage_round_set(bc_StartTurnMessage * self, uint32_t round)
{
}

bc_StartTurnMessage * bc_StartTurnMessage_from_json(char * s)
{
	return nullptr;
}

char * bc_StartTurnMessage_to_json(bc_StartTurnMessage * self)
{
	return nullptr;
}

bc_ViewerMessage * new_bc_ViewerMessage()
{
	return nullptr;
}

void delete_bc_ViewerMessage(bc_ViewerMessage * self)
{
}

bc_ViewerMessage * bc_ViewerMessage_from_json(char * s)
{
	return nullptr;
}

char * bc_ViewerMessage_to_json(bc_ViewerMessage * self)
{
	return nullptr;
}

bc_ViewerKeyframe * new_bc_ViewerKeyframe()
{
	return nullptr;
}

void delete_bc_ViewerKeyframe(bc_ViewerKeyframe * self)
{
}

bc_ViewerKeyframe * bc_ViewerKeyframe_from_json(char * s)
{
	return nullptr;
}

char * bc_ViewerKeyframe_to_json(bc_ViewerKeyframe * self)
{
	return nullptr;
}

bc_ErrorMessage * new_bc_ErrorMessage()
{
	return nullptr;
}

void delete_bc_ErrorMessage(bc_ErrorMessage * self)
{
}

char * bc_ErrorMessage_error_get(bc_ErrorMessage * self)
{
	return nullptr;
}

void bc_ErrorMessage_error_set(bc_ErrorMessage * self, char * error)
{
}

bc_ErrorMessage * bc_ErrorMessage_from_json(char * s)
{
	return nullptr;
}

char * bc_ErrorMessage_to_json(bc_ErrorMessage * self)
{
	return nullptr;
}

char * bc_ErrorMessage_debug(bc_ErrorMessage * self)
{
	return nullptr;
}

bc_TurnApplication * new_bc_TurnApplication()
{
	return nullptr;
}

void delete_bc_TurnApplication(bc_TurnApplication * self)
{
}

bc_StartTurnMessage * bc_TurnApplication_start_turn_get(bc_TurnApplication * self)
{
	return nullptr;
}

bc_ViewerMessage * bc_TurnApplication_viewer_get(bc_TurnApplication * self)
{
	return nullptr;
}

void bc_TurnApplication_start_turn_set(bc_TurnApplication * self, bc_StartTurnMessage * start_turn)
{
}

void bc_TurnApplication_viewer_set(bc_TurnApplication * self, bc_ViewerMessage * viewer)
{
}

bc_InitialTurnApplication * new_bc_InitialTurnApplication()
{
	return nullptr;
}

void delete_bc_InitialTurnApplication(bc_InitialTurnApplication * self)
{
}

bc_StartTurnMessage * bc_InitialTurnApplication_start_turn_get(bc_InitialTurnApplication * self)
{
	return nullptr;
}

bc_ViewerKeyframe * bc_InitialTurnApplication_viewer_get(bc_InitialTurnApplication * self)
{
	return nullptr;
}

void bc_InitialTurnApplication_start_turn_set(bc_InitialTurnApplication * self, bc_StartTurnMessage * start_turn)
{
}

void bc_InitialTurnApplication_viewer_set(bc_InitialTurnApplication * self, bc_ViewerKeyframe * viewer)
{
}

bc_AsteroidStrike * new_bc_AsteroidStrike(uint32_t karbonite, bc_MapLocation * location)
{
	return nullptr;
}

void delete_bc_AsteroidStrike(bc_AsteroidStrike * self)
{
}

uint32_t bc_AsteroidStrike_karbonite_get(bc_AsteroidStrike * self)
{
	return uint32_t();
}

bc_MapLocation * bc_AsteroidStrike_location_get(bc_AsteroidStrike * self)
{
	return nullptr;
}

void bc_AsteroidStrike_karbonite_set(bc_AsteroidStrike * self, uint32_t karbonite)
{
}

void bc_AsteroidStrike_location_set(bc_AsteroidStrike * self, bc_MapLocation * location)
{
}

bc_AsteroidStrike * bc_AsteroidStrike_clone(bc_AsteroidStrike * self)
{
	return nullptr;
}

char * bc_AsteroidStrike_debug(bc_AsteroidStrike * self)
{
	return nullptr;
}

bc_AsteroidStrike * bc_AsteroidStrike_from_json(char * s)
{
	return nullptr;
}

char * bc_AsteroidStrike_to_json(bc_AsteroidStrike * self)
{
	return nullptr;
}

uint8_t bc_AsteroidStrike_eq(bc_AsteroidStrike * self, bc_AsteroidStrike * other)
{
	return uint8_t();
}

bc_AsteroidPattern * new_bc_AsteroidPattern(uint16_t seed, bc_PlanetMap * mars_map)
{
	return nullptr;
}

void delete_bc_AsteroidPattern(bc_AsteroidPattern * self)
{
}

void bc_AsteroidPattern_validate(bc_AsteroidPattern * self)
{
}

uint8_t bc_AsteroidPattern_has_asteroid(bc_AsteroidPattern * self, uint32_t round)
{
	return uint8_t();
}

bc_AsteroidStrike * bc_AsteroidPattern_asteroid(bc_AsteroidPattern * self, uint32_t round)
{
	return nullptr;
}

bc_AsteroidPattern * bc_AsteroidPattern_clone(bc_AsteroidPattern * self)
{
	return nullptr;
}

char * bc_AsteroidPattern_debug(bc_AsteroidPattern * self)
{
	return nullptr;
}

bc_AsteroidPattern * bc_AsteroidPattern_from_json(char * s)
{
	return nullptr;
}

char * bc_AsteroidPattern_to_json(bc_AsteroidPattern * self)
{
	return nullptr;
}

bc_OrbitPattern * new_bc_OrbitPattern(uint32_t amplitude, uint32_t period, uint32_t center)
{
	return nullptr;
}

void delete_bc_OrbitPattern(bc_OrbitPattern * self)
{
}

uint32_t bc_OrbitPattern_amplitude_get(bc_OrbitPattern * self)
{
	return uint32_t();
}

uint32_t bc_OrbitPattern_period_get(bc_OrbitPattern * self)
{
	return uint32_t();
}

uint32_t bc_OrbitPattern_center_get(bc_OrbitPattern * self)
{
	return uint32_t();
}

void bc_OrbitPattern_amplitude_set(bc_OrbitPattern * self, uint32_t amplitude)
{
}

void bc_OrbitPattern_period_set(bc_OrbitPattern * self, uint32_t period)
{
}

void bc_OrbitPattern_center_set(bc_OrbitPattern * self, uint32_t center)
{
}

void bc_OrbitPattern_validate(bc_OrbitPattern * self)
{
}

uint32_t bc_OrbitPattern_duration(bc_OrbitPattern * self, uint32_t round)
{
	return uint32_t();
}

bc_OrbitPattern * bc_OrbitPattern_from_json(char * s)
{
	return nullptr;
}

char * bc_OrbitPattern_to_json(bc_OrbitPattern * self)
{
	return nullptr;
}

bc_GameMap * new_bc_GameMap()
{
	return nullptr;
}

void delete_bc_GameMap(bc_GameMap * self)
{
}

uint16_t bc_GameMap_seed_get(bc_GameMap * self)
{
	return uint16_t();
}

bc_PlanetMap * bc_GameMap_earth_map_get(bc_GameMap * self)
{
	return nullptr;
}

bc_PlanetMap * bc_GameMap_mars_map_get(bc_GameMap * self)
{
	return nullptr;
}

bc_AsteroidPattern * bc_GameMap_asteroids_get(bc_GameMap * self)
{
	return nullptr;
}

bc_OrbitPattern * bc_GameMap_orbit_get(bc_GameMap * self)
{
	return nullptr;
}

void bc_GameMap_seed_set(bc_GameMap * self, uint16_t seed)
{
}

void bc_GameMap_earth_map_set(bc_GameMap * self, bc_PlanetMap * earth_map)
{
}

void bc_GameMap_mars_map_set(bc_GameMap * self, bc_PlanetMap * mars_map)
{
}

void bc_GameMap_asteroids_set(bc_GameMap * self, bc_AsteroidPattern * asteroids)
{
}

void bc_GameMap_orbit_set(bc_GameMap * self, bc_OrbitPattern * orbit)
{
}

void bc_GameMap_validate(bc_GameMap * self)
{
}

bc_GameMap * bc_GameMap_test_map()
{
	return nullptr;
}

bc_GameMap * bc_GameMap_clone(bc_GameMap * self)
{
	return nullptr;
}

bc_GameMap * bc_GameMap_from_json(char * s)
{
	return nullptr;
}

char * bc_GameMap_to_json(bc_GameMap * self)
{
	return nullptr;
}

uintptr_t max_level(bc_UnitType branch)
{
	return uintptr_t();
}

uint32_t cost_of(bc_UnitType branch, uintptr_t level)
{
	return uint32_t();
}

bc_ResearchInfo * new_bc_ResearchInfo()
{
	return nullptr;
}

void delete_bc_ResearchInfo(bc_ResearchInfo * self)
{
}

uintptr_t bc_ResearchInfo_get_level(bc_ResearchInfo * self, bc_UnitType branch)
{
	return uintptr_t();
}

bc_VecUnitType * bc_ResearchInfo_queue(bc_ResearchInfo * self)
{
	return nullptr;
}

uint8_t bc_ResearchInfo_has_next_in_queue(bc_ResearchInfo * self)
{
	return uint8_t();
}

bc_UnitType bc_ResearchInfo_next_in_queue(bc_ResearchInfo * self)
{
	return bc_UnitType();
}

uint32_t bc_ResearchInfo_rounds_left(bc_ResearchInfo * self)
{
	return uint32_t();
}

bc_ResearchInfo * bc_ResearchInfo_from_json(char * s)
{
	return nullptr;
}

char * bc_ResearchInfo_to_json(bc_ResearchInfo * self)
{
	return nullptr;
}

bc_RocketLanding * new_bc_RocketLanding(uint16_t rocket_id, bc_MapLocation * destination)
{
	return nullptr;
}

void delete_bc_RocketLanding(bc_RocketLanding * self)
{
}

uint16_t bc_RocketLanding_rocket_id_get(bc_RocketLanding * self)
{
	return uint16_t();
}

bc_MapLocation * bc_RocketLanding_destination_get(bc_RocketLanding * self)
{
	return nullptr;
}

void bc_RocketLanding_rocket_id_set(bc_RocketLanding * self, uint16_t rocket_id)
{
}

void bc_RocketLanding_destination_set(bc_RocketLanding * self, bc_MapLocation * destination)
{
}

bc_RocketLanding * bc_RocketLanding_clone(bc_RocketLanding * self)
{
	return nullptr;
}

char * bc_RocketLanding_debug(bc_RocketLanding * self)
{
	return nullptr;
}

bc_RocketLanding * bc_RocketLanding_from_json(char * s)
{
	return nullptr;
}

char * bc_RocketLanding_to_json(bc_RocketLanding * self)
{
	return nullptr;
}

uint8_t bc_RocketLanding_eq(bc_RocketLanding * self, bc_RocketLanding * other)
{
	return uint8_t();
}

bc_VecRocketLanding * new_bc_VecRocketLanding()
{
	return nullptr;
}

void delete_bc_VecRocketLanding(bc_VecRocketLanding * self)
{
}

char * bc_VecRocketLanding_debug(bc_VecRocketLanding * self)
{
	return nullptr;
}

bc_VecRocketLanding * bc_VecRocketLanding_clone(bc_VecRocketLanding * self)
{
	return nullptr;
}

uintptr_t bc_VecRocketLanding_len(bc_VecRocketLanding * self)
{
	return uintptr_t();
}

bc_RocketLanding * bc_VecRocketLanding_index(bc_VecRocketLanding * self, uintptr_t index)
{
	return nullptr;
}

bc_RocketLandingInfo * new_bc_RocketLandingInfo()
{
	return nullptr;
}

void delete_bc_RocketLandingInfo(bc_RocketLandingInfo * self)
{
}

bc_VecRocketLanding * bc_RocketLandingInfo_landings_on(bc_RocketLandingInfo * self, uint32_t round)
{
	return nullptr;
}

bc_RocketLandingInfo * bc_RocketLandingInfo_clone(bc_RocketLandingInfo * self)
{
	return nullptr;
}

char * bc_RocketLandingInfo_debug(bc_RocketLandingInfo * self)
{
	return nullptr;
}

bc_RocketLandingInfo * bc_RocketLandingInfo_from_json(char * s)
{
	return nullptr;
}

char * bc_RocketLandingInfo_to_json(bc_RocketLandingInfo * self)
{
	return nullptr;
}

uint8_t bc_RocketLandingInfo_eq(bc_RocketLandingInfo * self, bc_RocketLandingInfo * other)
{
	return uint8_t();
}

bc_GameController * new_bc_GameController()
{
	return nullptr;
}

void delete_bc_GameController(bc_GameController * self)
{
}

void bc_GameController_next_turn(bc_GameController * self)
{
}

uint32_t bc_GameController_round(bc_GameController * self)
{
	return uint32_t();
}

bc_Planet bc_GameController_planet(bc_GameController * self)
{
	return bc_Planet();
}

bc_Team bc_GameController_team(bc_GameController * self)
{
	return bc_Team();
}

bc_PlanetMap * bc_GameController_starting_map(bc_GameController * self, bc_Planet planet)
{
	return nullptr;
}

uint32_t bc_GameController_karbonite(bc_GameController * self)
{
	return uint32_t();
}

bc_Unit * bc_GameController_unit(bc_GameController * self, uint16_t id)
{
	return nullptr;
}

bc_VecUnit * bc_GameController_units(bc_GameController * self)
{
	return nullptr;
}

bc_VecUnit * bc_GameController_my_units(bc_GameController * self)
{
	return nullptr;
}

bc_VecUnit * bc_GameController_units_in_space(bc_GameController * self)
{
	return nullptr;
}

uint32_t bc_GameController_karbonite_at(bc_GameController * self, bc_MapLocation * location)
{
	return uint32_t();
}

bc_VecMapLocation * bc_GameController_all_locations_within(bc_GameController * self, bc_MapLocation * location, uint32_t radius_squared)
{
	return nullptr;
}

uint8_t bc_GameController_can_sense_location(bc_GameController * self, bc_MapLocation * location)
{
	return uint8_t();
}

uint8_t bc_GameController_can_sense_unit(bc_GameController * self, uint16_t id)
{
	return uint8_t();
}

bc_VecUnit * bc_GameController_sense_nearby_units(bc_GameController * self, bc_MapLocation * location, uint32_t radius)
{
	return nullptr;
}

bc_VecUnit * bc_GameController_sense_nearby_units_by_team(bc_GameController * self, bc_MapLocation * location, uint32_t radius, bc_Team team)
{
	return nullptr;
}

bc_VecUnit * bc_GameController_sense_nearby_units_by_type(bc_GameController * self, bc_MapLocation * location, uint32_t radius, bc_UnitType unit_type)
{
	return nullptr;
}

uint8_t bc_GameController_has_unit_at_location(bc_GameController * self, bc_MapLocation * location)
{
	return uint8_t();
}

bc_Unit * bc_GameController_sense_unit_at_location(bc_GameController * self, bc_MapLocation * location)
{
	return nullptr;
}

bc_AsteroidPattern * bc_GameController_asteroid_pattern(bc_GameController * self)
{
	return nullptr;
}

bc_OrbitPattern * bc_GameController_orbit_pattern(bc_GameController * self)
{
	return nullptr;
}

uint32_t bc_GameController_current_duration_of_flight(bc_GameController * self)
{
	return uint32_t();
}

bc_Veci32 * bc_GameController_get_team_array(bc_GameController * self, bc_Planet planet)
{
	return nullptr;
}

void bc_GameController_write_team_array(bc_GameController * self, uintptr_t index, int32_t value)
{
}

void bc_GameController_disintegrate_unit(bc_GameController * self, uint16_t unit_id)
{
}

uint8_t bc_GameController_is_occupiable(bc_GameController * self, bc_MapLocation * location)
{
	return uint8_t();
}

uint8_t bc_GameController_can_move(bc_GameController * self, uint16_t robot_id, bc_Direction direction)
{
	return uint8_t();
}

uint8_t bc_GameController_is_move_ready(bc_GameController * self, uint16_t robot_id)
{
	return uint8_t();
}

void bc_GameController_move_robot(bc_GameController * self, uint16_t robot_id, bc_Direction direction)
{
}

uint8_t bc_GameController_can_attack(bc_GameController * self, uint16_t robot_id, uint16_t target_unit_id)
{
	return uint8_t();
}

uint8_t bc_GameController_is_attack_ready(bc_GameController * self, uint16_t robot_id)
{
	return uint8_t();
}

void bc_GameController_attack(bc_GameController * self, uint16_t robot_id, uint16_t target_unit_id)
{
}

bc_ResearchInfo * bc_GameController_research_info(bc_GameController * self)
{
	return nullptr;
}

uint8_t bc_GameController_reset_research(bc_GameController * self)
{
	return uint8_t();
}

uint8_t bc_GameController_queue_research(bc_GameController * self, bc_UnitType branch)
{
	return uint8_t();
}

uint8_t bc_GameController_can_harvest(bc_GameController * self, uint16_t worker_id, bc_Direction direction)
{
	return uint8_t();
}

void bc_GameController_harvest(bc_GameController * self, uint16_t worker_id, bc_Direction direction)
{
}

uint8_t bc_GameController_can_blueprint(bc_GameController * self, uint16_t worker_id, bc_UnitType unit_type, bc_Direction direction)
{
	return uint8_t();
}

void bc_GameController_blueprint(bc_GameController * self, uint16_t worker_id, bc_UnitType structure_type, bc_Direction direction)
{
}

uint8_t bc_GameController_can_build(bc_GameController * self, uint16_t worker_id, uint16_t blueprint_id)
{
	return uint8_t();
}

void bc_GameController_build(bc_GameController * self, uint16_t worker_id, uint16_t blueprint_id)
{
}

uint8_t bc_GameController_can_repair(bc_GameController * self, uint16_t worker_id, uint16_t structure_id)
{
	return uint8_t();
}

void bc_GameController_repair(bc_GameController * self, uint16_t worker_id, uint16_t structure_id)
{
}

uint8_t bc_GameController_can_replicate(bc_GameController * self, uint16_t worker_id, bc_Direction direction)
{
	return uint8_t();
}

void bc_GameController_replicate(bc_GameController * self, uint16_t worker_id, bc_Direction direction)
{
}

uint8_t bc_GameController_can_javelin(bc_GameController * self, uint16_t knight_id, uint16_t target_unit_id)
{
	return uint8_t();
}

uint8_t bc_GameController_is_javelin_ready(bc_GameController * self, uint16_t knight_id)
{
	return uint8_t();
}

void bc_GameController_javelin(bc_GameController * self, uint16_t knight_id, uint16_t target_unit_id)
{
}

uint8_t bc_GameController_can_begin_snipe(bc_GameController * self, uint16_t ranger_id, bc_MapLocation * location)
{
	return uint8_t();
}

uint8_t bc_GameController_is_begin_snipe_ready(bc_GameController * self, uint16_t ranger_id)
{
	return uint8_t();
}

void bc_GameController_begin_snipe(bc_GameController * self, uint16_t ranger_id, bc_MapLocation * location)
{
}

uint8_t bc_GameController_can_blink(bc_GameController * self, uint16_t mage_id, bc_MapLocation * location)
{
	return uint8_t();
}

uint8_t bc_GameController_is_blink_ready(bc_GameController * self, uint16_t mage_id)
{
	return uint8_t();
}

void bc_GameController_blink(bc_GameController * self, uint16_t mage_id, bc_MapLocation * location)
{
}

uint8_t bc_GameController_can_heal(bc_GameController * self, uint16_t healer_id, uint16_t target_robot_id)
{
	return uint8_t();
}

uint8_t bc_GameController_is_heal_ready(bc_GameController * self, uint16_t healer_id)
{
	return uint8_t();
}

void bc_GameController_heal(bc_GameController * self, uint16_t healer_id, uint16_t target_robot_id)
{
}

uint8_t bc_GameController_can_overcharge(bc_GameController * self, uint16_t healer_id, uint16_t target_robot_id)
{
	return uint8_t();
}

uint8_t bc_GameController_is_overcharge_ready(bc_GameController * self, uint16_t healer_id)
{
	return uint8_t();
}

void bc_GameController_overcharge(bc_GameController * self, uint16_t healer_id, uint16_t target_robot_id)
{
}

uint8_t bc_GameController_can_load(bc_GameController * self, uint16_t structure_id, uint16_t robot_id)
{
	return uint8_t();
}

void bc_GameController_load(bc_GameController * self, uint16_t structure_id, uint16_t robot_id)
{
}

uint8_t bc_GameController_can_unload(bc_GameController * self, uint16_t structure_id, bc_Direction direction)
{
	return uint8_t();
}

void bc_GameController_unload(bc_GameController * self, uint16_t structure_id, bc_Direction direction)
{
}

uint8_t bc_GameController_can_produce_robot(bc_GameController * self, uint16_t factory_id, bc_UnitType robot_type)
{
	return uint8_t();
}

void bc_GameController_produce_robot(bc_GameController * self, uint16_t factory_id, bc_UnitType robot_type)
{
}

bc_RocketLandingInfo * bc_GameController_rocket_landings(bc_GameController * self)
{
	return nullptr;
}

uint8_t bc_GameController_can_launch_rocket(bc_GameController * self, uint16_t rocket_id, bc_MapLocation * destination)
{
	return uint8_t();
}
