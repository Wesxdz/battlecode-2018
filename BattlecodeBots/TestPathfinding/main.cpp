
#include <iostream>

enum bc_Direction {
	North = 0,
	Northeast = 1,
	East = 2,
	Southeast = 3,
	South = 4,
	Southwest = 5,
	West = 6,
	Northwest = 7,
	Center = 8,
};

bc_Direction DirectionTo(int sourceX, int sourceY, int destX, int destY) {
	int resultX = destX - sourceX;
	int resultY = destY - sourceY;

	// Right
	if (resultX > 0) {
		if (resultY > 0) {
			return bc_Direction::Northeast;
		} else if (resultY < 0) {
			return bc_Direction::Southeast;
		} else {
			return bc_Direction::East;
		}
	} 
	// Left
	else if (resultX < 0) {
		if (resultY > 0) {
			return bc_Direction::Northwest;
		} else if (resultY < 0) {
			return bc_Direction::Southwest;
		} else {
			return bc_Direction::West;
		}
	} 
	// Center
	else {
		if (resultY > 0) {
			return bc_Direction::North;
		} else if (resultY < 0) {
			return bc_Direction::South;
		} else {
			return bc_Direction::Center;
		}
	}
}

int main() {
	for (int y = 0; y < 10; y++) {
		for (int x = 0; x < 10; x++) {
			auto dir = DirectionTo(5, 5, x, y);

			std::cout << "Direction to" << x << ", " << y << " is " << dir << std::endl;
		}
	}
	int x;
	std::cin >> x;
}

