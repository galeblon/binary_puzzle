#ifndef game_logic
#define game_logic
#include<iostream>

#define DEFAULT_SIZE 8
#define ORIGIN_X 5
#define ORIGIN_Y 3


enum states {
	unset = 0,
	zeroS = 1,
	oneS = 2
};
struct field {
	states state;
	bool editable = true;
};
struct coords {
	int x = 0;
	int y = 0;
	int setCoord(int a, int b) { if ((x = a) && (y = b)) return 0; return 1; };
};


struct board {
	coords originPoint;
	int size = DEFAULT_SIZE;
	field** plane = NULL;

	void initialize();
	void cleanUp();

	void show();
};


void move(int lr, int ud, int* x, int* y, board* gameBoard);
int setField(int x, int y, board* gameBoard, states state);

#endif 