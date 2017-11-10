#include"game_logic.h"


void board::initialize() {
	originPoint.setCoord(ORIGIN_X, ORIGIN_Y);
	plane = new field*[size];
	for (int i = 0; i < size; i++) {
		plane[i] = new field[size];
		for (int j = 0; j < size; j++) {
			plane[i][j].state = unset;
		}
	}
}
void board::cleanUp() {
	for (int i = 0; i < size; i++)
		delete[] plane[i];
	delete[] plane;
}

void move(int lr, int ud, int* x, int* y, board* gameBoard) {
	if (lr) {
		if (lr > 0) {
			if (*x > gameBoard->originPoint.x + 1)
				(*x)--;
			else
				(*x) = gameBoard->originPoint.x + gameBoard->size;
		}
		else {
			if (*x < gameBoard->originPoint.x + gameBoard->size)
				(*x)++;
			else
				(*x) = gameBoard->originPoint.x + 1;
		}

	}
	if (ud) {
		if (ud > 0) {
			if (*y > gameBoard->originPoint.y + 1)
				(*y)--;
			else
				(*y) += gameBoard->size - 1;
		} else{
			if (*y < gameBoard->originPoint.y + gameBoard->size)
				(*y)++;
			else
				(*y) = gameBoard->originPoint.y + 1;
		}
	}
}

int setField(int x, int y, board* gameBoard, states state) {
	x -= gameBoard->originPoint.x+1;
	y -= gameBoard->originPoint.y+1;
	if (gameBoard->plane[y][x].editable == true) {
		gameBoard->plane[y][x].state = state;
		return 0;
	}
	else {
		//tu bedzie error msg
		return 1;
	}
}