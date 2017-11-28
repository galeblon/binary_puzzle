#ifndef game_display
#define game_display

#include"game_logic.h"
#include"conio2.h"
#include<cstdlib>

#define L_WIDTH 25
#define L_HEIGHT 10
#define L_ORIGIN_X 2
#define L_ORIGIN_Y 2
#define T_WIDTH 28
#define T_HEIGHT 5
#define DEF_BG_COLOR 1

/*Wy�wietla obramowanie w podanych wsp�rz�dnych o podanych wymiarach i kolorze*/
void drawBorder(int x, int y, int size_w, int size_h, int color);

/*Wy�wietla okno z legend� w miejscu o wsp�rz�dnych x,y.
kolor obramowania podany w parametrze color.*/
void drawLegend(coords global, int color);

void drawSimpleTip(coords global, coords relative, const board* gameBoard, int color);
char drawTipFor(int rel_x, int rel_y, int* x, int* y, const board* gameBoard, states state);

/*Wy�wietla wiadomo�� b��du w miejscu o wsp�rz�dnych x,y i tre�ci str*/
void showErrMsg(int x, int y, const char* str);

#endif 