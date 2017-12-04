#include"game_display.h"

#define LT_BORDER "\xC9"
#define LB_BORDER "\xC8"
#define RT_BORDER "\xBB"
#define RB_BORDER "\xBC"
#define HOR_BORDER "\xCD"
#define VER_BORDER "\xBA"

void board::show(int color) {
	textbackground(BLACK);
	drawBorder(originPoint.x, originPoint.y, size+2, size+2, color);
	int x = originPoint.x + 1; int y = originPoint.y + 1;
	gotoxy(x, y);
	textcolor(WHITE);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (plane[i][j].editable == false)
				textbackground(DARKGRAY);
			if (plane[i][j].state == S_ONE)
				cputs("1");
			else if (plane[i][j].state == S_ZERO)
				cputs("0");
			else
				cputs(" ");
			textbackground(BLACK);
		}
		gotoxy(x, ++y);
	}
	textcolor(LIGHTGRAY);
	textbackground(BLUE);
}

void drawLegend(coords global, int color, flags& gameFlags) {
	int x = global.x;
	int y = global.y;
	drawBorder(x, y, L_WIDTH, L_HEIGHT, color);
	textbackground(BLACK);
	global.x = ++x; global.y = ++y;
	gotoxy(x, y);
	putTextLine(&global, "Adrian Misiak");
	putTextLine(&global, "nr. indeksu 171600");
	putTextLine(&global, "abcdefghijklmn");
	putTextLine(&global, "esc - wyjscie");
	putTextLine(&global, "n - nowa gra");
	putTextLine(&global, "o - losowe wypelnienie");
	putTextByFlag(&global, gameFlags.simpleTipToggle, "p", "  - prosta podpowiedz");
	putTextLine(&global, "r - zmiana rozmiaru");
	putTextLine(&global, "k - sprawdz sprzecznosc");
	putTextByFlag(&global, gameFlags.rule2CountToggle, "d", "  - licznik reguly 2");
	putTextByFlag(&global, gameFlags.autoMode, "a", "  - detekcja konca gry");
	putTextLine(&global, "j - pola jednoznaczne");
	putTextLine(&global, "s - zapisz gre");
	putTextLine(&global, "l - wczytaj gre");
	putTextLine(&global, "b - rozwiazanie");
	textbackground(DEF_BG_COLOR);

}


void putTextLine(coords *global, char *str) {
	cputs(str);
	global->y += 1;
	gotoxy(global->x, global->y);
}


void putTextByFlag(coords *global, bool flag, char* button, char* body) {
	putTextLine(global, body);
	gotoxy(global->x, global->y - 1);
	if (flag) textbackground(YELLOW);
	cputs(button);
	gotoxy(global->x, global->y);
	textbackground(BLACK);
}

char drawTipFor(int rel_x, int rel_y, int* x, int* y, const board* gameBoard, states state) {
	char buff[2];
	buff[0] = state == S_ZERO ? '0' : '1';
	buff[1] = '\0';
	char tmp[256];
	int rule2, rule3;
	if (gameBoard->plane[rel_y][rel_x].editable == false) {
		gotoxy(*x, (*y)+1);
		cputs("Pole nieedytowalne.");
		return ' ';
	}
	if (checkRule1(gameBoard, rel_x, rel_y, state) || gameBoard->plane[rel_y][rel_x].state == state) {
		if (!(rule2 = checkRule2(gameBoard, rel_x, rel_y, state)) || gameBoard->plane[rel_y][rel_x].state == state) {
			if ( !(rule3 = checkRule3(gameBoard, rel_x, rel_y, state)) || gameBoard->plane[rel_y][rel_x].state == state) {
				return state==S_ZERO ? '0' : '1';
			}
			else {
				tipErrorMsg(x, y, rule3, 3, buff);
			}
		}
		else {
			tipErrorMsg(x, y, rule2>0?rel_y+1:-(rel_x+1), 2, buff);
		}
	}
	else {
		tipErrorMsg(x, y, 0, 1, buff);
	}
	return ' ';
}
void tipErrorMsg(int* x, int* y, int rule_val, int rule, char* buff) {
	char tmp[256];
	gotoxy(*x, ++(*y));
	cputs("dla ");
	cputs(buff);
	if (rule == 1) {
		cputs(": Zasada 1 zlamana");
	}
	else if (rule == 2) {
		rule_val > 0 ? cputs(": wiersz ") : cputs(": kolumna ");
		itoa(rule_val > 0 ? rule_val: -rule_val, tmp, 10);
		cputs(tmp);
		cputs(" przekracza");
		gotoxy(*x, ++(*y));
		cputs("max liczbe elementow.");
	}
	else if (rule == 3) {
		rule_val > 0 ? cputs(": wiersz identyczny") : cputs(": kolumna identyczna");
		gotoxy(*x, ++(*y));
		rule_val > 0 ? cputs("z wierszem nr.") : cputs("z kolumna nr.");
		itoa(rule_val > 0 ? rule_val : -rule_val, tmp, 10);
		cputs(tmp);
	}
}


void drawSimpleTip(coords global, coords relative, const board* gameBoard, int color) {
	int x = global.x;
	int y = global.y;
	y += L_HEIGHT;
	textbackground(BLACK);
	drawBorder(x, y, T_WIDTH, T_HEIGHT, color);
	gotoxy(++x, ++y);
	cputs("Dostepne cyfry dla pola:  ");
	int tmp_x = wherex() - 2;
	int tmp_y = y;
	char toShow[3];
	toShow[2] = '\0';
	toShow[0] = drawTipFor(relative.x, relative.y, &x, &y, gameBoard, S_ZERO);
	toShow[1] = drawTipFor(relative.x, relative.y, &x, &y, gameBoard, S_ONE);
	gotoxy(tmp_x, tmp_y);
	cputs(toShow);
	textbackground(DEF_BG_COLOR);
}

void drawRule2Counter(const board* gameBoard) {
	char buff[256];
	int x = gameBoard->originPoint.x-2; // Wziêcie pod uwagê ramki pola
	int y = gameBoard->originPoint.y;
	gotoxy(x, y);
	textbackground(BROWN);
	cputs("10");
	gotoxy(x, ++y);
	for (int i = 0; i < gameBoard->size; i++) {
		int row_one = countInRow(gameBoard, i, S_ONE);
		int row_zero = countInRow(gameBoard, i, S_ZERO);
		itoa(row_one, buff, 10);
		textbackground(GREEN);
		if (row_one == gameBoard->size / 2) textbackground(RED);
		cputs(buff);
		itoa(row_zero, buff, 10);
		textbackground(LIGHTBLUE);
		if (row_zero == gameBoard->size / 2) textbackground(RED);
		cputs(buff);
		gotoxy(x, ++y);
	}

	x = gameBoard->originPoint.x;
	y = gameBoard->originPoint.y + gameBoard->size + 2;
	gotoxy(x, y);
	textbackground(BROWN);
	cputs("1");
	for (int i = 0; i < gameBoard->size; i++) {
		int col_one = countInCol(gameBoard, i, S_ONE);
		itoa(col_one, buff, 10);
		textbackground(GREEN);
		if (col_one == gameBoard->size / 2) textbackground(RED);
		cputs(buff);
	}
	gotoxy(x, ++y);
	textbackground(BROWN);
	cputs("0");
	for (int i = 0; i < gameBoard->size; i++) {
		int col_zero = countInCol(gameBoard, i, S_ZERO);
		itoa(col_zero, buff, 10);
		textbackground(LIGHTBLUE);
		if(col_zero == gameBoard->size/2) textbackground(RED);
		cputs(buff);
	}
	textbackground(DEF_BG_COLOR);
}



void drawBorder(int x, int y, int size_w, int size_h, int color) {
	textbackground(BLACK);
	int tmp_y = y;
	gotoxy(x, y);
	for (int i = 0; i < size_h; i++) {
		for (int j = 0; j<size_w; j++)
			cputs(" ");
		gotoxy(x, ++tmp_y);
	}
	textcolor(color);
	gotoxy(x, y);
	cputs(LT_BORDER); // lewy górny róg.
	gotoxy(x, y + size_h - 1);
	cputs(LB_BORDER); // lewy dolny róg.
	gotoxy(x + size_w - 1, y);
	cputs(RT_BORDER); // prawy górny róg.
	gotoxy(x + size_w - 1, y + size_h - 1);
	cputs(RB_BORDER); // prawy górny róg.
	//pionowe obramowanie
	int tmp = y + 1;
	for (int i = 0; i < size_h - 2; i++, tmp++) {
		gotoxy(x, tmp);
		cputs(VER_BORDER);
		gotoxy(x + size_w - 1, tmp);
		cputs(VER_BORDER);
	}
	//poziome obramowanie
	tmp = x + 1;
	for (int i = 0; i < size_w - 2; i++, tmp++) {
		gotoxy(tmp, y);
		cputs(HOR_BORDER);
		gotoxy(tmp, y + size_h - 1);
		cputs(HOR_BORDER);
	}
	textcolor(LIGHTGRAY);
}

void drawBlankOnPlane(const board* gameBoard,int x, int y, int color) {
	coords relative, global;
	relative.setCoord(x, y);
	global = relativeToGlobal(relative, gameBoard);
	gotoxy(global.x, global.y);
	textbackground(color);
	cputs(" ");
	textbackground(DEF_BG_COLOR);
}

void showErrMsg(int x, int y, const char* str) {
	_setcursortype(_NOCURSOR);
	static int inc;
	gotoxy(x, y+inc);
	textbackground(WHITE);
	textcolor(LIGHTRED);
	cputs(str);
	textbackground(DEF_BG_COLOR);
	textcolor(LIGHTGRAY);
	inc = (inc + 1) % 2;
	getch();
	clrscr();
	_setcursortype(_SOLIDCURSOR);
}

void drawGameScreen(flags& gameFlags, board& gameBoard, coords legend_c, coords global_c) {
	_setcursortype(_NOCURSOR);
	drawLegend(legend_c, YELLOW, gameFlags);
	gameBoard.show(DARKGRAY);
	if (gameFlags.simpleTipToggle) drawSimpleTip(legend_c, globalToRelative(global_c, &gameBoard), &gameBoard, WHITE);
	if (gameFlags.rule2CountToggle) drawRule2Counter(&gameBoard);
	if (gameFlags.autoMode) showIfSolvable(&gameBoard);
	_setcursortype(_SOLIDCURSOR);
}

void showIfSolvable(const board* gameBoard) {
	_setcursortype(_NOCURSOR);
	coords global;
	global.setCoord(gameBoard->originPoint.x, gameBoard->originPoint.y);
	gotoxy(global.x, global.y);
	bool filled = true;
	for (int i = 0; i < gameBoard->size; i++) {
		for (int j = 0; j < gameBoard->size; j++) {
			if (gameBoard->plane[i][j].state == S_UNSET)
				filled = false;
		}
	}
	textbackground(WHITE);
	if (filled) {
		textcolor(GREEN);
		putTextLine(&global, "Gra ukonczona.");
		textbackground(DEF_BG_COLOR);
		return;
	}
	if (checkContradiction(gameBoard, false)) {
		textcolor(RED);
		putTextLine(&global, "Gry nie mozna ukonczyc.");
		textcolor(BLACK);
	}
	textbackground(DEF_BG_COLOR);
	_setcursortype(_SOLIDCURSOR);
}

void getInput(char* query, char* res, bool numerical) {
	clrscr();
	drawBorder(INPUT_WINDOW_ORIGIN_X, INPUT_WINDOW_ORIGIN_Y, INPUT_WINDOW_WIDTH, INPUT_WINDOW_HEIGHT, WHITE);
	gotoxy(INPUT_WINDOW_ORIGIN_X + 1, INPUT_WINDOW_ORIGIN_Y + 1);
	cputs(query);
	char digit[2];
	digit[1] = '\0';
	int counter = 0;
	while (true) {
		digit[0] = getch();
		if (numerical) {
			if ('0' <= digit[0] && digit[0] <= '9' && counter < 8) {
				res[counter] = digit[0];
				counter++;
				cputs(digit);
			}
		}
		else if('0'<= digit[0] &&  digit[0]<='z' && counter < 8){
			res[counter] = digit[0];
			counter++;
			cputs(digit);
		}
		if(digit[0] == 13) // Je¿eli wciœniêto enter.
			break;
		if (digit[0] == 8 && counter > 0) { // Umo¿liwia cofanie wprowadzenia. kod ascii BackSpace.
			counter--;
			res[counter] = 0;
			gotoxy(wherex() - 1, wherey());
			cputs(" ");
			gotoxy(wherex() - 1, wherey());
		}
	}
	textbackground(DEF_BG_COLOR);
	clrscr();
	res[counter] = '\0';
}