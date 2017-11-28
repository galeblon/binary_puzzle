#ifndef game_display
#define game_display

#include"game_logic.h"
#include"conio2.h"
#include<cstdlib>

#define L_WIDTH 25
#define L_HEIGHT 10
#define L_ORIGIN_X 2
#define L_ORIGIN_Y 2
#define T_WIDTH 32
#define T_HEIGHT 8
#define DEF_BG_COLOR 1

/*Wyœwietla obramowanie w podanych wspó³rzêdnych o podanych wymiarach i kolorze*/
void drawBorder(int x, int y, int size_w, int size_h, int color);

/*Wyœwietla okno z legend¹ w miejscu o wspó³rzêdnych x,y.
kolor obramowania podany w parametrze color.*/
void drawLegend(coords global, int color);

/*Wyœwietla okno z prostymi podpowiedziami*/
void drawSimpleTip(coords global, coords relative, const board* gameBoard, int color);

/*Funkcja uzupe³niaj¹ca do drawSimpleTip, sprawdza zasady 1,2,3 w polu o wspó³rzêdnych rel_ dla wartoœæi state*/
char drawTipFor(int rel_x, int rel_y, int* x, int* y, const board* gameBoard, states state);

/*Kolejna podfunkcja, zajmuje siê obs³ug¹ poszczególnych zasad i zamiane ich na komunikaty
rule_val to wartoœc zwrócona przed funkcje checkRule 1, 2 lub 3
int rule przyjmuje wartoœci 1, 2, 3 aby program wiedzia³ który tekst wyœwietliæ.*/
void tipErrorMsg(int* x, int* y, int rule_val, int rule, char* buff);

/*Wyœwietla wiadomoœæ b³êdu w miejscu o wspó³rzêdnych x,y i treœci str*/
void showErrMsg(int x, int y, const char* str);

#endif 