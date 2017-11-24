#include<stdio.h>
#include"conio2.h"
#include"game_display.h"
#include"game_logic.h"

int main() {
	int zn = 0, zero = 0;
	char txt[32];
#ifndef __cplusplus
	Conio2_Init();
#endif

	textbackground(BLUE);
	clrscr();

	board gameBoard;
	gameBoard.initialize(DEFAULT_SIZE);
	coords global;
	global.x = gameBoard.originPoint.x + 1;
	global.y = gameBoard.originPoint.y + 1;
	
	gameBoard.show(DARKGRAY);
	settitle("Adrian Misiak 171600");
	do {
		// ustawiamy czarny kolor t³a, z którego bêdziemy
		// korzystali rysuj¹c po ekranie
		// proszê zerkn¹æ do conio2.h na listê dostêpnych kolorów
		textbackground(BLACK);
		textcolor(7);
		// przesuwamy kursor do kolumny 55 i wiersza 1
		// wiersze i kolumny liczymy od 1
		gotoxy(55, 1);
		// wyœwietlamy na ekranie, w miejscu kursora napis
		// kursor przesuwa siê w prawo o d³ugoœæ napisu
		cputs("q = wyjscie");
		gotoxy(55, 2);
		cputs("strzalki = poruszanie");
		gotoxy(55, 3);
		cputs("spacja = zmiana koloru");
		gotoxy(55, 4);
		cputs("enter = zmiana koloru tla");

		drawLegend(2, 2, YELLOW);
		gameBoard.show(DARKGRAY);

		// wypisujemy na ekranie kod ostatnio naciœniêtego klawisza
		if(zero) sprintf(txt, "kod klawisza: 0x00 0x%02x", zn);
		else sprintf(txt, "kod klawisza: 0x%02x", zn);
		gotoxy(55, 5);
		cputs(txt);
		gotoxy(global.x, global.y);
		

		zero = 0;
		zn = getch();
		textbackground(BLUE);
		clrscr();
		if(zn == 0) {
			zero = 1;		// je¿eli tak, to czytamy
			zn = getch();		// kolejny znak wiedz¹c ju¿,  ¿e bêdzie to znak specjalny
			if (zn == 0x48) move(UP, &global, &gameBoard);//y--;
			else if (zn == 0x50) move(DOWN, &global, &gameBoard); //y++
			else if (zn == 0x4b) move(LEFT, &global, &gameBoard); //x--;
			else if (zn == 0x4d) move(RIGHT, &global, &gameBoard); //x++;
		}
		else if (zn == 0x31) setField(global.x-gameBoard.originPoint.x-1, global.y-gameBoard.originPoint.y - 1, &gameBoard, oneS, true);
		else if (zn == 0x30) setField(global.x - gameBoard.originPoint.x-1, global.y - gameBoard.originPoint.y - 1, &gameBoard, zeroS, true);
		else if (zn == 0x08) setField(global.x - gameBoard.originPoint.x-1, global.y - gameBoard.originPoint.y - 1, &gameBoard, unset, true);
		else if (zn == 0x6e) loadMap(&gameBoard, "default.txt");
	} while (zn != 'q' && zn != 0x1b);
	gameBoard.cleanUp();
	return 0;
	}