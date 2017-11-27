#ifndef game_display
#define game_display

#include"game_logic.h"
#include"conio2.h"

#define L_WIDTH 25
#define L_HEIGHT 10

/*Wy�wietla obramowanie w podanych wsp�rz�dnych o podanych wymiarach i kolorze*/
void drawBorder(int x, int y, int size_w, int size_h, int color);

/*Wy�wietla okno z legend� w miejscu o wsp�rz�dnych x,y.
kolor obramowania podany w parametrze color.*/
void drawLegend(int x, int y, int color);

/*Wy�wietla wiadomo�� b��du w miejscu o wsp�rz�dnych x,y i tre�ci str*/
void showErrMsg(int x, int y, const char* str);

#endif 