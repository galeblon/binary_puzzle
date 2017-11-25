#ifndef game_logic
#define game_logic
#include"conio2.h"
#include<stdio.h>

#define DEFAULT_SIZE 8
#define ORIGIN_X 30
#define ORIGIN_Y 2
#define NULL 0


enum states {
	S_UNSET = 0,
	S_ZERO = 1,
	S_ONE = 2
};
enum directions {
	UP,
	DOWN,
	LEFT,
	RIGHT
};
enum actions {
	NEW_GAME,
	QUIT_GAME,
	MOVE_UP,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT,
	SET_FIELD_1,
	SET_FIELD_0,
	UNSET_FIELD
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

	void initialize(int newSize);
	void cleanUp();

	void show(int color);
};

actions getAction();
coords globalToRelative(coords global, const board &gameBoard);
void move(directions direction, coords* global, const board* gameBoard);
int setField(coords relative, const board* gameBoard, states state, bool editable);
bool checkRule1(const board* gameBoard, int x, int y, states state);
bool checkRule2(const board* gameBoard, int x, int y, states state);
bool checkRule3(const board* gameBoard, int x, int y, states state);
int loadMap(board* gameBoard, const char* fName);

#endif 