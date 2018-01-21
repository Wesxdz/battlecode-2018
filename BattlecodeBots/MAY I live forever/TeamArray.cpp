#include "TeamArray.h"

#include "GameController.h"

void TeamArray::Write(uintptr_t index, const char* message, int length) {
	// This may not work?
	for (uintptr_t i = index; i < length; i++) {
		bc_GameController_write_team_array(GameController::gc, index, message[i]);
	}
	
}
void TeamArray::Write(uintptr_t index, int32_t value) {
	bc_GameController_write_team_array(GameController::gc, index, value);
}


bc_Veci32*  TeamArray::Read(bc_Planet type) {
	return bc_GameController_get_team_array(GameController::gc, type);
}
char* TeamArray::ReadString(bc_Planet type) {
	auto ptr = bc_GameController_get_team_array(GameController::gc, type);
	auto message = bc_Veci32_debug(ptr);
	delete_bc_Veci32(ptr);
	return message;
}

void TeamArray::DeleteMessage(bc_Veci32* ptr) {
	delete_bc_Veci32(ptr);
}
void TeamArray::DeleteMessage(const char* ptr) {
	delete[] ptr;
}