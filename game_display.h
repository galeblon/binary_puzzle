#ifndef game_display
#define game_display

#include"game_logic.h"
#include"conio2.h"

#define L_WIDTH 25
#define L_HEIGHT 10

/*Wyœwietla obramowanie w podanych wspó³rzêdnych o podanych wymiarach i kolorze*/
void drawBorder(int x, int y, int size_w, int size_h, int color);

/*Wyœwietla okno z legend¹ w miejscu o wspó³rzêdnych x,y.
kolor obramowania podany w parametrze color.*/
void drawLegend(int x, int y, int color);

/*Wyœwietla wiadomoœæ b³êdu w miejscu o wspó³rzêdnych x,y i treœci str*/
void showErrMsg(int x, int y, const char* str);

#endif 