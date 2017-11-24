#ifndef game_logic
#define game_logic
#include<iostream>
#include"conio2.h"


#define DEFAULT_SIZE 8
#define ORIGIN_X 30
#define ORIGIN_Y 2


enum states {
	unset = 0,
	zeroS = 1,
	oneS = 2
};
enum directions {
	UP,
	DOWN,
	LEFT,
	RIGHT
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


void move(directions direction, coords* global, const board* gameBoard);
int setField(int x, int y, const board* gameBoard, states state, bool editable);
bool checkRule1(const board* gameBoard, int x, int y, states state);
bool checkRule2(const board* gameBoard, int x, int y, states state);
bool checkRule3(const board* gameBoard, int x, int y, states state);
int loadMap(board* gameBoard, const char* fName);


#endif 