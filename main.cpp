#include<stdio.h>
#include"conio2.h"
#include"game_display.h"
#include"game_logic.h"

int main() {
	actions action;

	textbackground(BLUE);
	clrscr();

	board gameBoard;
	gameBoard.initialize(DEFAULT_SIZE);
	coords global;
	coords relative;
	global.x = gameBoard.originPoint.x + 1;
	global.y = gameBoard.originPoint.y + 1;
	
	gameBoard.show(DARKGRAY);
	settitle("Adrian Misiak 171600");
	do {
		drawLegend(2, 2, YELLOW);
		gameBoard.show(DARKGRAY);
		gotoxy(global.x, global.y);
		relative = globalToRelative(global, gameBoard);
		action = getAction();
		textbackground(BLUE);
		clrscr();
		switch (action) {
			case NEW_GAME: loadMap(&gameBoard, "default.txt"); break;
			case RANDOMIZE_BOARD: gameBoard.randomize(); break;
			case MOVE_UP: move(UP, &global, &gameBoard); break;
			case MOVE_DOWN: move(DOWN, &global, &gameBoard); break;
			case MOVE_LEFT: move(LEFT, &global, &gameBoard); break;
			case MOVE_RIGHT: move(RIGHT, &global, &gameBoard); break;
			case SET_FIELD_1: setField(relative, &gameBoard, S_ONE, true); break;
			case SET_FIELD_0: setField(relative, &gameBoard, S_ZERO, true); break;
			case UNSET_FIELD: setField(relative, &gameBoard, S_UNSET, true); break;
			}
	} while (action != QUIT_GAME);
	gameBoard.cleanUp();
	return 0;
	}