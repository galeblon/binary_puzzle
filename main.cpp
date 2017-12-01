#include<stdio.h>
#include"conio2.h"
#include"game_display.h"
#include"game_logic.h"

int main() {
	_setcursortype(_SOLIDCURSOR);
	textbackground(DEF_BG_COLOR);

	actions action;
	flags gameFlags;
	clrscr();

	board gameBoard;
	gameBoard.initialize(DEFAULT_SIZE);
	coords global_c;
	coords legend_c;

	global_c.setCoord(gameBoard.originPoint.x + 1, gameBoard.originPoint.y + 1);
	legend_c.setCoord(L_ORIGIN_X, L_ORIGIN_Y);

	settitle("Adrian Misiak 171600");
	do {
		drawGameScreen(gameFlags, gameBoard, legend_c, global_c);
		gotoxy(global_c.x, global_c.y);
		action = getAction();
		parseAction(gameBoard, action, global_c, gameFlags);
		clrscr();
	} while (action != QUIT_GAME);
	gameBoard.cleanUp();
	return 0;
	}