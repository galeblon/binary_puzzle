#ifndef game_display
#define game_display

#include"game_logic.h"
#include"conio2.h"

#define L_WIDTH 20
#define L_HEIGHT 10

void drawBorder(int x, int y, int size_w, int size_h, int color);
void drawLegend(int x, int y, int color);
void showErrMsg(int x, int y, const char* str);

#endif 