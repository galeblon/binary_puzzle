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


void drawLegend(int x, int y, int color) {
	textbackground(BLACK);
	drawBorder(x, y, L_WIDTH, L_HEIGHT, color);
	x++; y++;
	gotoxy(x, y);
	cputs("Adrian Misiak     ");
	gotoxy(x, ++y);
	cputs("nr. indeksu 171600");
	gotoxy(x, ++y);
	cputs("abcdefg");
	gotoxy(x, ++y);
	cputs("esc - wyjscie     ");
	gotoxy(x, ++y);
	cputs("n - nowa gra      ");
	gotoxy(x, ++y);
	cputs("o - losowe wypelnienie");
	textbackground(BLUE);

}

void drawBorder(int x, int y, int size_w, int size_h, int color) {
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