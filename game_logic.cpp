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
	else if (keyAction == 'n' || keyAction == 'N') return NEW_GAME;
	else if (keyAction == 'o' || keyAction == 'O') return RANDOMIZE_BOARD;
	else if (keyAction == 'r' || keyAction == 'R') return RESIZE_BOARD;
	else if (keyAction == 0x1B) return QUIT_GAME;
	else if (keyAction == 'p' || keyAction == 'P') return SIMPLE_TIP;
	else if (keyAction == 'k' || keyAction == 'K') return CHECK_CONTRADICTION;
	else if (keyAction == 'd' || keyAction == 'D') return RULE2_COUNTER;
	else if (keyAction == 'a' || keyAction == 'A') return AUTO_MODE;
	else if (keyAction == 'j' || keyAction == 'J') return CHECK_UNAMBIGUOUS;
	else if (keyAction == 's' || keyAction == 'S') return SAVE_GAME;
	else if (keyAction == 'l' || keyAction == 'L') return LOAD_GAME;
	else if (keyAction == 'b' || keyAction == 'B') return SHOW_SOLUTION;
	return UNDEFINED_ACTION;
}

void parseAction(board &gameBoard, actions action, coords& global_c, flags& gameFlags) {
	_setcursortype(_NOCURSOR);
	coords relative_c = globalToRelative(global_c, &gameBoard);
	switch (action) {
	case NEW_GAME: loadMap(&gameBoard, &gameFlags, "default.map", false); break;
	case RANDOMIZE_BOARD: gameBoard.randomize(); break;
	case RESIZE_BOARD: gameBoard.resize();
		global_c.setCoord(gameBoard.originPoint.x + 1, gameBoard.originPoint.y + 1);
		break;
	case SIMPLE_TIP: gameFlags.simpleTipToggle = !gameFlags.simpleTipToggle; break;
	case CHECK_CONTRADICTION: checkContradiction(&gameBoard, true); break;
	case CHECK_UNAMBIGUOUS: checkUnambiguous(&gameBoard, true); break;
	case RULE2_COUNTER: gameFlags.rule2CountToggle = !gameFlags.rule2CountToggle; break;
	case AUTO_MODE: gameFlags.autoMode = !gameFlags.autoMode; break;
	case SAVE_GAME: saveMap(&gameBoard, &gameFlags); break;
	case LOAD_GAME: loadGame(&gameBoard, &gameFlags, &global_c); break;
	case SHOW_SOLUTION: showSolution(gameBoard.plane, gameBoard.size); break;
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
		if (setField(relative, this, state, false, false)) {
			if (checkContradiction(this, false)) {
				this->plane[relative.y][relative.x].state = S_UNSET;
				this->plane[relative.y][relative.x].editable = true;
			}
			else {
				count++;
			}
		}
		if (count == howMany)
			break;
	}
	textbackground(DEF_BG_COLOR);
	clrscr();

}

void board::resize() {
	char number[9];
	flags localFlags;
	getInput("Podaj nowy rozmiar: ", number, true);
	gotoxy(1, 1);
	int newSize = atoi(number);
	if (initialize(newSize)) {
		char buff[256];
		sprintf(buff, "%i.map", newSize);
		if (loadMap(this, &localFlags, buff, false)) {
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

int loadGame(board* gameBoard, flags* gameFlags, coords* global) {
	char buff[9];
	char path[256];
	getInput("Podaj nazwe zapisanej gry: ", buff, false);
	if (buff[0] == 0) {
		showErrMsg(1, 1, "Niepoprawna nazwa.");
		return 0;
	}
	sprintf(path, "saves\\%s.sav", buff);
	if (!loadMap(gameBoard, gameFlags, path, true))
		showErrMsg(1, 2, "Niepowodzenie we wczytywaniu stanu gry.");
	global->setCoord(gameBoard->originPoint.x + 1, gameBoard->originPoint.y + 1);
}


int saveMap(const board* gameBoard, const flags* gameFlags) {
	FILE *fpointer;
	char buff[9];
	char path[256];
	getInput("Podaj nazwe zapisu: ", buff, false);
	if (buff[0] == 0) {
		showErrMsg(1, 1, "Niepoprawna nazwa.");
		return 0;
	}
	sprintf(path, "map\\saves\\%s.sav", buff);
	fpointer = fopen(path, "w");
	coords relative;
	if (fpointer != NULL) {
		fprintf(fpointer, "%d\n", gameBoard->size);
		for (int i = 0; i < gameBoard->size; i++) {
			for (int j = 0; j < gameBoard->size; j++) {
				relative.setCoord(j, i);
				if (gameBoard->plane[i][j].state == S_ONE) {
					if (gameBoard->plane[i][j].editable)
						fprintf(fpointer, "i");
					else
						fprintf(fpointer, "1");
				}
				else if (gameBoard->plane[i][j].state == S_ZERO) {
					if (gameBoard->plane[i][j].editable)
						fprintf(fpointer, "o");
					else
						fprintf(fpointer, "0");
				}
				else {
					fprintf(fpointer, ".");
				}

			}
			fprintf(fpointer, "\n");
		}
		fprintf(fpointer, "\n%d%d%d", gameFlags->autoMode, gameFlags->rule2CountToggle, gameFlags->simpleTipToggle);
	}
	else {
		showErrMsg(1, 1, "Zapis sie nie powiodl.");
		return 0;
	}
	fclose(fpointer);
	return	1;
}
void loadFlags(flags* gameFlags, FILE* fpointer) {
	int val = 0;
	fscanf(fpointer, "%1d", &val);
	gameFlags->autoMode =  val ? true : false;
	fscanf(fpointer, "%1d", &val);
	gameFlags->rule2CountToggle = val ? true : false;
	fscanf(fpointer, "%1d", &val);
	gameFlags->simpleTipToggle = val ? true : false;
}


int isValidFromFile(board* gameBoard, coords relative,char val, bool showError) {
	if (val == '1') {
		if (!(setField(relative, gameBoard, S_ONE, false, false))) {
			if (showError)showErrMsg(gameBoard->originPoint.x, gameBoard->originPoint.y + gameBoard->size + 2, "Niepoprawnie uformowana mapa.");
			return 0;
		}
	}
	else if (val == 'i') {
		if (!(setField(relative, gameBoard, S_ONE, true, false))) {
			if (showError)showErrMsg(gameBoard->originPoint.x, gameBoard->originPoint.y + gameBoard->size + 2, "Niepoprawnie uformowana mapa.");
			return 0;
		}
	}
	else if (val == 'o') {
		if (!(setField(relative, gameBoard, S_ZERO, true, false))) {
			if (showError)showErrMsg(gameBoard->originPoint.x, gameBoard->originPoint.y + gameBoard->size + 2, "Niepoprawnie uformowana mapa.");
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


int loadMap(board* gameBoard, flags* gameFlags, const char* fName, bool showError) {
	FILE *fpointer;
	char path[256];
	sprintf(path, "map\\%s", fName);
	gotoxy(1, 1);
	cputs(path);
	fpointer = fopen(path, "r+");
	if(fpointer != NULL) {
		int size=0;
		fscanf(fpointer, "%d\n", &size);
		if (!gameBoard->initialize(size)) {
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
					relative.setCoord(j, i);
					if (fscanf(fpointer, "%c", &val) == EOF) {
						gameBoard->initialize(DEFAULT_SIZE);
						showErrMsg(gameBoard->originPoint.x, gameBoard->originPoint.y + gameBoard->size + 2, "Plik zbyt krotki.");
						fclose(fpointer);
						return 0;
					}
					isValidFromFile(gameBoard, relative, val, showError);
				}
				fscanf(fpointer, "\n");
			}

		}
		loadFlags(gameFlags, fpointer);
	fclose(fpointer);
	return 1;
	}
	else {
		if (showError)showErrMsg(gameBoard->originPoint.x, gameBoard->originPoint.y + gameBoard->size+2, "Blad z otwarciem pliku.");
		return 0;
	}
}


bool isValidInput(const board* gameBoard, int x, int y, states state) {
	if (checkRule1(gameBoard, x, y, state)
		&& !checkRule2(gameBoard, x, y, state)
		&& !checkRule3(gameBoard, x, y, state))
		return true;
	return false;
}

int checkContradiction(const board* gameBoard, bool visible) {
	_setcursortype(_NOCURSOR);
	for (int i = 0; i < gameBoard->size; i++) {
		for (int j = 0; j < gameBoard->size; j++) {
			if (gameBoard->plane[i][j].state == S_UNSET) {
				if (!isValidInput(gameBoard, j, i, S_ZERO))
					if (!isValidInput(gameBoard, j, i, S_ONE)) {
						if (!visible)
							return 1;
						drawBlankOnPlane(gameBoard, j, i, RED);
					}
			}
		}
	}
	if(visible)getch();
	_setcursortype(_SOLIDCURSOR);
	return 0;
}
int checkUnambiguous(board* gameBoard, bool visible) {
	field** tmp_plane = new field*[gameBoard->size];
	for (int i = 0; i < gameBoard->size; i++) {
		tmp_plane[i] = new field[gameBoard->size];
		for (int j = 0; j < gameBoard->size; j++) {
			tmp_plane[i][j].state = S_UNSET;
		}
	}
	cpField(gameBoard->plane, tmp_plane, gameBoard->size);
	_setcursortype(_NOCURSOR);
	bool didSomething = false;
	for (int i = 0; i < gameBoard->size; i++) {
		for (int j = 0; j < gameBoard->size; j++) {
			if (gameBoard->plane[i][j].state == S_UNSET) {
				if (isValidInput(gameBoard, j, i, S_ZERO)) {
					if (!isValidInput(gameBoard, j, i, S_ONE)) {
						if(visible)drawBlankOnPlane(gameBoard, j, i, GREEN);
						gameBoard->plane[i][j].state = S_ZERO;
						didSomething = true;
					}
				}
				else {
					if (isValidInput(gameBoard, j, i, S_ONE)) {
						if(visible)drawBlankOnPlane(gameBoard, j, i, GREEN);
						gameBoard->plane[i][j].state = S_ONE;
						didSomething = true;
					}
				}
			}
		}
	}
	if (visible) {

	}
	gotoxy(gameBoard->originPoint.x, gameBoard->originPoint.y-1);
	textcolor(WHITE);
	if(visible)cputs("w - wypelnij te pola");
	char action; 
	action = visible ? getch() : 'w';
	if (action != 'w' && action != 'W')
		cpField(tmp_plane, gameBoard->plane, gameBoard->size);

	for (int i = 0; i < gameBoard->size; i++) {
		delete[] tmp_plane[i];
	}
	delete[] tmp_plane;
	clrscr();
	_setcursortype(_SOLIDCURSOR);
	return didSomething;
}

void cpField(field** source, field** destination, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
				destination[i][j].state = source[i][j].state;
				destination[i][j].editable = source[i][j].editable;
		}
	}
}

int showSolution(field** gameField, int size) {
	_setcursortype(_NOCURSOR);
	board gameBoard;
	gameBoard.initialize(size);
	cpField(gameField, gameBoard.plane, size);

	while (checkUnambiguous(&gameBoard, false));

	gameBoard.show(GREEN);
	getch();
	gameBoard.cleanUp();
	_setcursortype(_SOLIDCURSOR);
	return 1;
}