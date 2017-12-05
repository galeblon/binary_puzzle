#ifndef game_display
#define game_display

#include"game_logic.h"
#include"conio2.h"
#include<cstdlib>

#define L_WIDTH 25
#define L_HEIGHT 17

/*Punkt zaczepny okna z legend¹.*/
#define L_ORIGIN_X 2
#define L_ORIGIN_Y 2

/*Wymiary okna z podpowiedziami.*/
#define T_WIDTH 32	
#define T_HEIGHT 8

/*Wybra³em jasno-niebieski jako podstawowy kolor t³a.*/
#define DEF_BG_COLOR 1


/*Wymiary i punkt zaczepny okienka wyœwietlanego przy pobieraniu ci¹gu znaków od gracza.*/
#define INPUT_WINDOW_ORIGIN_X 10
#define INPUT_WINDOW_ORIGIN_Y 4
#define INPUT_WINDOW_WIDTH 40
#define INPUT_WINDOW_HEIGHT 3

/*Znaki ascii tzw. pipe'y*/
#define LT_BORDER "\xC9"
#define LB_BORDER "\xC8"
#define RT_BORDER "\xBB"
#define RB_BORDER "\xBC"
#define HOR_BORDER "\xCD"
#define VER_BORDER "\xBA"


/*Funkcja odpowiedzialna za wywo³ywanie funkcji dotycz¹cych rysowania ekranu.*/
void drawGameScreen(flags& gameFlags, board& gamaeBoard, coords legend_c, coords global_c);

/*Wyœwietla obramowanie w podanych wspó³rzêdnych o podanych wymiarach i kolorze*/
void drawBorder(int x, int y, int size_w, int size_h, int color);

/*Podœwietla pole na planszy w podanym kolorze*/
void drawBlankOnPlane(const board* gameBoard, int x, int y, int color);

/*Wyœwietla okno z legend¹ w miejscu o wspó³rzêdnych x,y.
kolor obramowania podany w parametrze color.*/
void drawLegend(coords global, int color, flags& gameFlags);

/*Umieszcza tekst w podanych wspolrzednych i przechodzi do nowej linii.*/
void putTextLine(coords *global, char *str);

/*Przy wyœwietlaniu zakolorowuje opcjê w³¹czone.*/
void putTextByFlag(coords *global, bool flag, char* button, char* body);

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

/*Wyœwietla przy planszy liczbê zer i jedynek w danym wierszu/kolumnie.*/
void drawRule2Counter(const board* gameBoard);

/*Je¿eli dokoñczenie rozgrywki nie jest mo¿liwe, wyœwietla komunikat o tym informuj¹cy.*/
void showIfSolvable(const board* gameBoard);

/*Wyœwietla okienko z zapytaniem i pobiera dane od u¿ytkownika, maksymalnie 8 znakowy ci¹g, res powinno byc tablic¹ min. 9 elementow¹.*/
void getInput(char* query, char* res, bool numerical);

/*Wywo³uje funkcjê showSolution i interpretuje jej wynik*/
void checkSolution(board* gameBoard);

#endif 