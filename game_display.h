#ifndef game_display
#define game_display

#include"game_logic.h"
#include"conio2.h"
#include<cstdlib>

#define L_WIDTH 25
#define L_HEIGHT 15
#define L_ORIGIN_X 2
#define L_ORIGIN_Y 2
#define T_WIDTH 32
#define T_HEIGHT 8
#define DEF_BG_COLOR 1
#define INPUT_WINDOW_ORIGIN_X 10
#define INPUT_WINDOW_ORIGIN_Y 4
#define INPUT_WINDOW_WIDTH 40
#define INPUT_WINDOW_HEIGHT 3

/*Wy�wietla obramowanie w podanych wsp�rz�dnych o podanych wymiarach i kolorze*/
void drawBorder(int x, int y, int size_w, int size_h, int color);

/*Pod�wietla pole na planszy w podanym kolorze*/
void drawBlankOnPlane(const board* gameBoard, int x, int y, int color);

/*Wy�wietla okno z legend� w miejscu o wsp�rz�dnych x,y.
kolor obramowania podany w parametrze color.*/
void drawLegend(coords global, int color, flags& gameFlags);

/*Umieszcza tekst w podanych wspolrzednych i przechodzi do nowej linii.*/
void putTextLine(coords *global, char *str);

/*Przy wy�wietlaniu zakolorowuje opcj� w��czone.*/
void putTextByFlag(coords *global, bool flag, char* button, char* body);

/*Wy�wietla okno z prostymi podpowiedziami*/
void drawSimpleTip(coords global, coords relative, const board* gameBoard, int color);

/*Funkcja uzupe�niaj�ca do drawSimpleTip, sprawdza zasady 1,2,3 w polu o wsp�rz�dnych rel_ dla warto��i state*/
char drawTipFor(int rel_x, int rel_y, int* x, int* y, const board* gameBoard, states state);

/*Wy�wietla przy planszy liczb� zer i jedynek w danym wierszu/kolumnie.*/
void drawRule2Counter(const board* gameBoard);

/*Funkcja odpowiedzialna za wywo�ywanie funkcji dotycz�cych rysowania ekranu.*/
void drawGameScreen(flags& gameFlags, board& gamaeBoard, coords legend_c, coords global_c);

/*Kolejna podfunkcja, zajmuje si� obs�ug� poszczeg�lnych zasad i zamiane ich na komunikaty
rule_val to warto�c zwr�cona przed funkcje checkRule 1, 2 lub 3
int rule przyjmuje warto�ci 1, 2, 3 aby program wiedzia� kt�ry tekst wy�wietli�.*/
void tipErrorMsg(int* x, int* y, int rule_val, int rule, char* buff);

/*Wy�wietla wiadomo�� b��du w miejscu o wsp�rz�dnych x,y i tre�ci str*/
void showErrMsg(int x, int y, const char* str);

/*Je�eli doko�czenie rozgrywki nie jest mo�liwe, wy�wietla komunikat o tym informuj�cy.*/
void showIfSolvable(const board* gameBoard);

/*Wy�wietla okienko z zapytaniem i pobiera dane od u�ytkownika, maksymalnie 8 znakowy ci�g.*/
void getInput(char* query, char* res, bool numerical);

#endif 