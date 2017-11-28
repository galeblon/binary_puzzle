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


void drawLegend(coords global, int color) {
	int x = global.x;
	int y = global.y;
	drawBorder(x, y, L_WIDTH, L_HEIGHT, color);
	textbackground(BLACK);
	x++; y++;
	gotoxy(x, y);
	cputs("Adrian Misiak");
	gotoxy(x, ++y);
	cputs("nr. indeksu 171600");
	gotoxy(x, ++y);
	cputs("abcdefgh");
	gotoxy(x, ++y);
	cputs("esc - wyjscie");
	gotoxy(x, ++y);
	cputs("n - nowa gra");
	gotoxy(x, ++y);
	cputs("o - losowe wypelnienie");
	gotoxy(x, ++y);
	cputs("p - prosta podpowiedz");
	gotoxy(x, ++y);
	cputs("r - zmiana rozmiaru");
	textbackground(DEF_BG_COLOR);

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

void showErrMsg(int x, int y, const char* str) {
	static int inc;
	gotoxy(x, y+inc);
	textbackground(WHITE);
	textcolor(LIGHTRED);
	cputs(str);
	textbackground(BLACK);
	textcolor(LIGHTGRAY);
	inc = (inc + 1) % 2;
}