#include <iostream>
#include <random>
#include <assert.h>
#include <bc.h>

int TestFunction() {
	int x = 0;
	for (int i = 0; i < 99999999; i++) {
		x *= 2;
	}
	return 0;
}

bool check_errors()
{
	if (bc_has_err())
	{
		char* err;
		int8_t code = bc_get_last_err(&err);
		std::cout << "Engine error code " << code << ": " << err << std::endl;
		bc_free_string(err);
		return true;
	}
	else
	{
		return false;
	}
}

int main()
{
	std::cout << "Player C++ bot starting!" << std::endl;

	srand(0);

	bc_Direction dir = North;
	bc_Direction opposite = bc_Direction_opposite(dir);

	check_errors();

	printf("Opposite direction of %d: %d\n", dir, opposite);

	assert(opposite == South);


	std::cout << "Connecting to manager..." << std::endl;

	bc_GameController *gc = new_bc_GameController();

	if (check_errors())
	{
		std::cout << "Failed, dying." << std::endl;
		return 1;
	}
	std::cout << "Connected!" << std::endl;

	while (true)
	{
		uint32_t round = bc_GameController_round(gc);
		std::cout << "Round: "<< round << std::endl;

		bc_VecUnit *units = bc_GameController_my_units(gc);

		int len = bc_VecUnit_len(units);
		for (int i = 0; i < len; i++)
		{
			bc_Unit *unit = bc_VecUnit_index(units, i);

			uint16_t id = bc_Unit_id(unit);
			if (bc_GameController_can_move(gc, id, North) && bc_GameController_is_move_ready(gc, id)) 
			{
				bc_GameController_move_robot(gc, id, North);
				check_errors();
			}
			delete_bc_Unit(unit);

		}
		delete_bc_VecUnit(units);

		bc_GameController_next_turn(gc);
	}
}