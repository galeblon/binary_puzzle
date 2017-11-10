#include"game_display.h"

void drawBorder(int x, int y, int size);

void board::show() {
	drawBorder(originPoint.x, originPoint.y, size+2);
	int x = originPoint.x + 1; int y = originPoint.y + 1;
	gotoxy(x, y);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (plane[i][j].state == oneS)
				cputs("1");
			else if (plane[i][j].state == zeroS)
				cputs("0");
			else
				cputs(" ");
		}
		gotoxy(x, ++y);
	}
}

void drawBorder(int x, int y, int size) {
	gotoxy(x, y);
	cputs("\xC9"); // lewy górny róg.
	gotoxy(x, y + size - 1);
	cputs("\xC8"); // lewy dolny róg.
	gotoxy(x  +size -1, y);
	cputs("\xBB"); // prawy górny róg.
	gotoxy(x + size - 1, y + size -1);
	cputs("\xBC"); // prawy górny róg.
	//pionowe obramowanie
	int tmp = y + 1;
	for (int i = 0; i < size-2; i++, tmp++) {
		gotoxy(x, tmp);
		cputs("\xBA");
		gotoxy(x + size -1, tmp);
		cputs("\xBA");
	}
	//poziome obramowanie
	tmp = x + 1;
	for (int i = 0; i < size - 2; i++, tmp++) {
		gotoxy(tmp, y);
		cputs("\xCD");
		gotoxy(tmp, y + size -1);
		cputs("\xCD");
	}
}