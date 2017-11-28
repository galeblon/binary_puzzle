#include<stdio.h>
#include"conio2.h"
#include"game_display.h"
#include"game_logic.h"

int main() {
	actions action;
	bool simpleTipToggle = false;
	textbackground(BLUE);
	clrscr();

	board gameBoard;
	gameBoard.initialize(DEFAULT_SIZE);
	coords global_c;
	coords relative_c;
	coords legend_c;

	global_c.x = gameBoard.originPoint.x + 1;
	global_c.y = gameBoard.originPoint.y + 1;

	legend_c.x = L_ORIGIN_X;
	legend_c.y = L_ORIGIN_Y;

	gameBoard.show(DARKGRAY);
	settitle("Adrian Misiak 171600");
	do {
		drawLegend(legend_c, YELLOW);
		gameBoard.show(DARKGRAY);
		relative_c = globalToRelative(global_c, gameBoard);
		if (simpleTipToggle) drawSimpleTip(legend_c, relative_c, &gameBoard, WHITE);
		gotoxy(global_c.x, global_c.y);
		action = getAction();
		textbackground(DEF_BG_COLOR);
		clrscr();
		switch (action) {
			case NEW_GAME: loadMap(&gameBoard, "default.txt"); break;
			case RANDOMIZE_BOARD: gameBoard.randomize(); break;
			case SIMPLE_TIP: simpleTipToggle = simpleTipToggle ? false : true; break;
			case MOVE_UP: move(UP, &global_c, &gameBoard); break;
			case MOVE_DOWN: move(DOWN, &global_c, &gameBoard); break;
			case MOVE_LEFT: move(LEFT, &global_c, &gameBoard); break;
			case MOVE_RIGHT: move(RIGHT, &global_c, &gameBoard); break;
			case SET_FIELD_1: setField(relative_c, &gameBoard, S_ONE, true); break;
			case SET_FIELD_0: setField(relative_c, &gameBoard, S_ZERO, true); break;
			case UNSET_FIELD: setField(relative_c, &gameBoard, S_UNSET, true); break;
			}
	} while (action != QUIT_GAME);
	gameBoard.cleanUp();
	return 0;
	}