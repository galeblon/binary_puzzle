#include"game_logic.h"
#include"game_display.h"


void board::initialize(int newSize) {
	size = newSize;
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
	plane = NULL;
}

void move(int lr, int ud, int* x, int* y, const board* gameBoard) {
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

int setField(int x, int y, const board* gameBoard, states state, bool editable) {
	if (gameBoard->plane[y][x].editable == true) {
		if (checkRule1(gameBoard, x, y, state)) {
			gameBoard->plane[y][x].state = state;
			gameBoard->plane[y][x].editable = editable;
			return 1;
		} else{
			showErrMsg(gameBoard->originPoint.x, gameBoard->originPoint.y + gameBoard->size + 2, "Zasada 1 zlamana.");
			return 0;
		}
	}
	else {
		showErrMsg(gameBoard->originPoint.x, gameBoard->originPoint.y + gameBoard->size + 2, "Nie mozna zmieniac wartosci pola stalego.");
		return 0;
	}
}


bool checkRule1(const board* gameBoard, int x, int y, states state) {
	if (state == unset)
		return true;
	int count_row = 1;
	int count_col = 1;
	for (int i = y-1; i >= 0; i--)
		if (gameBoard->plane[i][x].state == state)
			count_col++;
		else
			break;
	for (int i = y+1; i < gameBoard->size; i++)
		if (gameBoard->plane[i][x].state == state)
			count_col++;
		else
			break;
	if (count_col >= 3)
		return false;

	for (int i = x-1; i >= 0; i--)
		if (gameBoard->plane[y][i].state == state)
			count_row++;
		else
			break;
	for (int i = x+1; i < gameBoard->size; i++)
		if (gameBoard->plane[y][i].state == state)
			count_row++;
		else
			break;
	if (count_row >= 3)
		return false;
	return true;
}

int loadMap(board* gameBoard, const char* fName) {
	FILE *fpointer;
	char path[256] = { "map\\" };
	strcat(path, fName);
	gotoxy(1, 1);
	cputs(path);
	fpointer = fopen(path, "r+");
	if(fpointer != NULL) {
		gameBoard->cleanUp();
		int size=0;
		fscanf(fpointer, "%d\n", &size);
		if (size < 2 || size%2) {
			gameBoard->initialize(DEFAULT_SIZE);
			showErrMsg(gameBoard->originPoint.x, gameBoard->originPoint.y + gameBoard->size+2, "Niepoprawna wielkosc planszy.");

		}
		else {
			gameBoard->initialize(size);
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++) {
					char val;
					if (fscanf(fpointer, "%c", &val) == EOF) {
						gameBoard->cleanUp();
						gameBoard->initialize(DEFAULT_SIZE);
						showErrMsg(gameBoard->originPoint.x, gameBoard->originPoint.y + gameBoard->size + 2, "Plik zbyt krotki.");
						fclose(fpointer);
						return 0;
					}
					if (val == '1') {
						if (!(setField(j, i, gameBoard, oneS, false))) {
							showErrMsg(gameBoard->originPoint.x, gameBoard->originPoint.y + gameBoard->size + 2, "Niepoprawnie uformowana mapa.");
							return 0;
						}
					}
					else if (val == '0') {
						if (!(setField(j, i, gameBoard, zeroS, false))) {
							showErrMsg(gameBoard->originPoint.x, gameBoard->originPoint.y + gameBoard->size + 2, "Niepoprawnie uformowana mapa.");
							return 0;
						}
					}
				}
				fscanf(fpointer, "\n");
			}

		}
	
	fclose(fpointer);
	return 1;
	}
	else {
		showErrMsg(gameBoard->originPoint.x, gameBoard->originPoint.y + gameBoard->size+2, "Blad z otwarciem pliku.");
		return 0;
	}
}