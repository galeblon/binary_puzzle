#include"game_logic.h"
#include"game_display.h"

actions getAction() {
	char keyAction = getch();
	if (!keyAction) {
		keyAction = getch();
		if (keyAction == 0x48) return MOVE_UP;
		else if (keyAction == 0x50) return MOVE_DOWN;
		else if (keyAction == 0x4B) return MOVE_LEFT;
		else if (keyAction == 0x4D) return MOVE_RIGHT;
	}
	else if (keyAction == '1') return SET_FIELD_1;
	else if (keyAction == '0') return SET_FIELD_0;
	else if (keyAction == 0x08) return UNSET_FIELD;
	else if (keyAction == 'n') return NEW_GAME;
	else if (keyAction == 0x1B) return QUIT_GAME;
	
}

coords globalToRelative(coords global, const board &gameBoard) {
	coords relative;
	relative.x = global.x - gameBoard.originPoint.x - 1;
	relative.y = global.y - gameBoard.originPoint.y - 1;
	return relative;
}

void board::initialize(int newSize) {
	if (plane != NULL)
		cleanUp();
	size = newSize;
	originPoint.setCoord(ORIGIN_X, ORIGIN_Y);
	plane = new field*[size];
	for (int i = 0; i < size; i++) {
		plane[i] = new field[size];
		for (int j = 0; j < size; j++) {
			plane[i][j].state = S_UNSET;
		}
	}
}
void board::cleanUp() {
	for (int i = 0; i < size; i++)
		delete[] plane[i];
	delete[] plane;
	plane = NULL;
}

void move(directions direction, coords* global, const board* gameBoard) {
	switch (direction) {
	case UP:
		if (global->y > gameBoard->originPoint.y + 1)
			(global->y)--;
		else
			(global->y) += gameBoard->size - 1;
		break;
	case DOWN:
		if (global->y < gameBoard->originPoint.y + gameBoard->size)
			(global->y)++;
		else
			(global->y) = gameBoard->originPoint.y + 1;
		break;
	case LEFT:
		if (global->x > gameBoard->originPoint.x + 1)
			(global->x)--;
		else
			(global->x) = gameBoard->originPoint.x + gameBoard->size;
		break;
	case RIGHT:
		if (global->x < gameBoard->originPoint.x + gameBoard->size)
			(global->x)++;
		else
			(global->x) = gameBoard->originPoint.x + 1;
		break;

	}
}

int setField(coords relative, const board* gameBoard, states state, bool editable) {
	int x = relative.x;
	int y = relative.y;
	if (gameBoard->plane[y][x].editable == true) {
		if (gameBoard->plane[y][x].state == state)
			return 1;
		if (checkRule1(gameBoard, x, y, state)) {
			if (checkRule2(gameBoard, x, y, state)) {
				if (checkRule3(gameBoard, x, y, state)) {
					gameBoard->plane[y][x].state = state;
					gameBoard->plane[y][x].editable = editable;
					return 1;
				}
				else {
					showErrMsg(gameBoard->originPoint.x, gameBoard->originPoint.y + gameBoard->size + 2, "Zasada 3 zlamana.");
					return 0;
				}
			}
			else {
				showErrMsg(gameBoard->originPoint.x, gameBoard->originPoint.y + gameBoard->size + 2, "Zasada 2 zlamana.");
				return 0;
			}
		}		else {
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
	if (state == S_UNSET)
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
bool checkRule2(const board* gameBoard, int x, int y, states state) {
	if (state == S_UNSET)
		return true;
	int count_row = 1;
	int count_col = 1;
	for (int i = 0; i < gameBoard->size; i++) {
		if (gameBoard->plane[y][i].state == state)
			count_row++;
		if (gameBoard->plane[i][x].state == state)
			count_col++;
	}
	if (count_row > ((gameBoard->size) / 2) || count_col > ((gameBoard->size) / 2))
		return false;
	return true;
}
bool checkRule3(const board* gameBoard, int x, int y, states state) {
	if (state == S_UNSET)
		return true;
	for (int i = 0; i < gameBoard->size; i++) {
		int id_row = 0, id_col = 0;
		for (int j = 0; j < gameBoard->size; j++) {
			if (i != y) {
				if (gameBoard->plane[y][j].state == gameBoard->plane[i][j].state && gameBoard->plane[i][j].state != S_UNSET)
					id_row++;
			}
			if (i != x) {
				if (gameBoard->plane[j][x].state == gameBoard->plane[j][i].state && gameBoard->plane[j][i].state != S_UNSET)
					id_col++;
			}		
		}
		if (gameBoard->plane[i][x].state == state)
			id_row++;
		if (gameBoard->plane[y][i].state == state)
			id_col++;
		if (id_row == gameBoard->size || id_col == gameBoard->size)
			return false;
	}
	return true;
}

int loadMap(board* gameBoard, const char* fName) {
	FILE *fpointer;
	char path[256];
	sprintf(path, "map\\%s", fName);
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
			coords relative;
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++) {
					char val;
					relative.x = j;
					relative.y = i;
					if (fscanf(fpointer, "%c", &val) == EOF) {
						gameBoard->cleanUp();
						gameBoard->initialize(DEFAULT_SIZE);
						showErrMsg(gameBoard->originPoint.x, gameBoard->originPoint.y + gameBoard->size + 2, "Plik zbyt krotki.");
						fclose(fpointer);
						return 0;
					}
					if (val == '1') {
						if (!(setField(relative, gameBoard, S_ONE, false))) {
							showErrMsg(gameBoard->originPoint.x, gameBoard->originPoint.y + gameBoard->size + 2, "Niepoprawnie uformowana mapa.");
							return 0;
						}
					}
					else if (val == '0') {
						if (!(setField(relative, gameBoard, S_ZERO, false))) {
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