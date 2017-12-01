#include"game_logic.h"
#include"game_display.h"
#include"conio2.h"
#include<stdlib.h>
#include<time.h>
#include<math.h>

actions getAction() {
	_setcursortype(_SOLIDCURSOR);
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
	else if (keyAction == 'o') return RANDOMIZE_BOARD;
	else if (keyAction == 'r') return RESIZE_BOARD;
	else if (keyAction == 0x1B) return QUIT_GAME;
	else if (keyAction == 'p') return SIMPLE_TIP;
	else if (keyAction == 'k') return CHECK_CONTRADICTION;
	else if (keyAction == 'd') return RULE2_COUNTER;
	else if (keyAction == 'a') return AUTO_MODE;
	return UNDEFINED_ACTION;
}

void parseAction(board &gameBoard, actions action, coords& global_c, flags& gameFlags) {
	_setcursortype(_NOCURSOR);
	coords relative_c = globalToRelative(global_c, &gameBoard);
	switch (action) {
	case NEW_GAME: loadMap(&gameBoard, "default.map", false); break;
	case RANDOMIZE_BOARD: gameBoard.randomize(); break;
	case RESIZE_BOARD: gameBoard.resize();
		global_c.setCoord(gameBoard.originPoint.x + 1, gameBoard.originPoint.y + 1);
		break;
	case SIMPLE_TIP: gameFlags.simpleTipToggle = !gameFlags.simpleTipToggle; break;
	case CHECK_CONTRADICTION: checkContradiction(&gameBoard, true); break;
	case RULE2_COUNTER: gameFlags.rule2CountToggle = !gameFlags.rule2CountToggle; break;
	case AUTO_MODE: gameFlags.autoMode = !gameFlags.autoMode; break;
	case MOVE_UP: move(UP, &global_c, &gameBoard); break;
	case MOVE_DOWN: move(DOWN, &global_c, &gameBoard); break;
	case MOVE_LEFT: move(LEFT, &global_c, &gameBoard); break;
	case MOVE_RIGHT: move(RIGHT, &global_c, &gameBoard); break;
	case SET_FIELD_1: setField(relative_c, &gameBoard, S_ONE, true, true); break;
	case SET_FIELD_0: setField(relative_c, &gameBoard, S_ZERO, true, true); break;
	case UNSET_FIELD: setField(relative_c, &gameBoard, S_UNSET, true, true); break;
	}
	_setcursortype(_SOLIDCURSOR);
}

coords globalToRelative(coords global, const board *gameBoard) {
	coords relative;
	relative.x = global.x - gameBoard->originPoint.x - 1;
	relative.y = global.y - gameBoard->originPoint.y - 1;
	return relative;
}
coords relativeToGlobal(coords relative, const board *gameBoard) {
	coords global;
	global.x =  relative.x + gameBoard->originPoint.x + 1;
	global.y = relative.y + gameBoard->originPoint.y + 1;
	return global;
}

int board::initialize(int newSize) {
	text_info info;
	gettextinfo(&info);
	int maxSize2 = info.screenwidth - 2 - (ORIGIN_X-1);	// Liczenie dwóch granic, jedna ze wzglêdu na szerokoœæ a druga wysokoœc okna, brana pod uwagê bedziê wartoœæ mniejsza.
	int maxSize = info.screenheight - 2 - (ORIGIN_Y-1) - 2; //Wliczenie obramowania, punktu pocz¹tku oraz dwóch wierszy dla wyœwietlania zliczania jedynek i zer
	maxSize = maxSize < maxSize2 ? maxSize : maxSize2;
	if (newSize < 2 || newSize % 2 || newSize > maxSize)
		return 0;
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
	return 1;
}


void board::cleanUp() {
	for (int i = 0; i < size; i++)
		delete[] plane[i];
	delete[] plane;
	plane = NULL;
}
void board::randomize(){
	srand(time(NULL));
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++) {
			plane[i][j].state = S_UNSET;
			plane[i][j].editable = true;
		}
	int howMany = size*size*(rand() % (MAX_RAND_THRESHOLD - MIN_RAND_THRESHOLD) + MIN_RAND_THRESHOLD) / 100.0;
	if (!howMany)	//Dla ma³ych plansz kiedy zaokraglona wartoœæ jest równa 0.
		howMany++;
	int count = 0; states state; coords relative;
	for (int i = 0; i < pow(size, 4); i++) {
		state = rand() % 2 ? S_ONE : S_ZERO;
		relative.x = rand() % size;
		relative.y = rand() % size;
		if (setField(relative, this, state, false, false))
			count++;
		if (count == howMany)
			break;
	}
	textbackground(DEF_BG_COLOR);
	clrscr();

}

void board::resize() {
	char number[9]; 
	getInput("Podaj nowy rozmiar: ", number, true);
	gotoxy(1, 1);
	int newSize = atoi(number);
	if (initialize(newSize)) {
		char buff[256];
		sprintf(buff, "%i.map", newSize);
		if (loadMap(this, buff, false)) {
			cputs(" Wczytano mape z pliku.");
		}
		else {
			randomize();
			gotoxy(1, 1);
			cputs("Stworzono losowa mape.");
		}
	}
	else {
		cputs("Podano niewlasciwy rozmiar.");
	}
	getch();
	gotoxy(originPoint.x + 1, originPoint.y + 1);
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


int setField(coords relative, const board* gameBoard, states state, bool editable, bool showError) {
	int x = relative.x;
	int y = relative.y;
	if (gameBoard->plane[y][x].editable == true) {
		if (gameBoard->plane[y][x].state == state)
			return 1;
		if (checkRule1(gameBoard, x, y, state)) {
			if (!checkRule2(gameBoard, x, y, state)) {
				if (!checkRule3(gameBoard, x, y, state)) {
					gameBoard->plane[y][x].state = state;
					gameBoard->plane[y][x].editable = editable;
					return 1;
				}
				else if(showError){
					showErrMsg(gameBoard->originPoint.x, gameBoard->originPoint.y + gameBoard->size + 2, "Zasada 3 zlamana.");
					return 0;
				}
			}
			else if (showError){
				showErrMsg(gameBoard->originPoint.x, gameBoard->originPoint.y + gameBoard->size + 2, "Zasada 2 zlamana.");
				return 0;
			}
		}		else if (showError){
			showErrMsg(gameBoard->originPoint.x, gameBoard->originPoint.y + gameBoard->size + 2, "Zasada 1 zlamana.");
			return 0;
		}
	}
	else if (showError){
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


int checkRule2(const board* gameBoard, int x, int y, states state) {
	if (state == S_UNSET)
		return 0;
	int count_row = 1 + countInRow(gameBoard, y, state);
	int count_col = 1 + countInCol(gameBoard, x, state);
	if (count_row > (gameBoard->size / 2))
		return 1;
	if (count_col > (gameBoard->size / 2))
		return -1;
	return 0;
}

int countInRow(const board* gameBoard, int row, states state) {
	int count = 0;
	for (int i = 0; i < gameBoard->size; i++)
		if (gameBoard->plane[row][i].state == state)
			count++;
	return count;
}

int countInCol(const board* gameBoard, int col, states state) {
	int count = 0;
	for (int i = 0; i < gameBoard->size; i++)
		if (gameBoard->plane[i][col].state == state)
			count++;
	return count;
}

int checkRule3(const board* gameBoard, int x, int y, states state) {
	if (state == S_UNSET)
		return 0;
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
		if (id_row == gameBoard->size)
			return (i+1);
		if (id_col == gameBoard->size)
			return -(i+1);
	}
	return 0;
}


int loadMap(board* gameBoard, const char* fName, bool showError) {
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
			fclose(fpointer);
			return 0;
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
						if (!(setField(relative, gameBoard, S_ONE, false, false))) {
							if(showError)showErrMsg(gameBoard->originPoint.x, gameBoard->originPoint.y + gameBoard->size + 2, "Niepoprawnie uformowana mapa.");
							return 0;
						}
					}
					else if (val == '0') {
						if (!(setField(relative, gameBoard, S_ZERO, false, false))) {
							if (showError)showErrMsg(gameBoard->originPoint.x, gameBoard->originPoint.y + gameBoard->size + 2, "Niepoprawnie uformowana mapa.");
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
		if (showError)showErrMsg(gameBoard->originPoint.x, gameBoard->originPoint.y + gameBoard->size+2, "Blad z otwarciem pliku.");
		return 0;
	}
}


int checkContradiction(const board* gameBoard, bool visible) {
	_setcursortype(_NOCURSOR);
	coords relative;
	coords global;
	for (int i = 0; i < gameBoard->size; i++) {
		for (int j = 0; j < gameBoard->size; j++) {
			if(!checkRule1(gameBoard, j, i, S_ZERO) 
				|| checkRule2(gameBoard, j, i, S_ZERO)
				|| checkRule3(gameBoard, j, i, S_ZERO))
				if (!checkRule1(gameBoard, j, i, S_ONE)
					|| checkRule2(gameBoard, j, i, S_ONE)
					|| checkRule3(gameBoard, j, i, S_ONE)) {
					relative.setCoord(j, i);
					if (!visible)
						return 1;
					global = relativeToGlobal(relative, gameBoard);
					gotoxy(global.x, global.y);
					textbackground(RED);
					cputs(" ");
					textbackground(DEF_BG_COLOR);
				}
		}
	}
	if(visible)getch();
	_setcursortype(_SOLIDCURSOR);
	return 0;
}