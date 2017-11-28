#ifndef game_logic
#define game_logic
#include"conio2.h"
#include<stdio.h>

#define DEFAULT_SIZE 8
#define ORIGIN_X 50
#define ORIGIN_Y 2
#define NULL 0
#define MIN_RAND_THRESHOLD 20
#define MAX_RAND_THRESHOLD 30


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
	RANDOMIZE_BOARD,
	RESIZE_BOARD,
	SIMPLE_TIP,
	MOVE_UP,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT,
	SET_FIELD_1,
	SET_FIELD_0,
	UNSET_FIELD,
	UNDEFINED_ACTION
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
	void resize();
	void cleanUp();

	void show(int color);
	void randomize();
};

actions getAction();
coords globalToRelative(coords global, const board &gameBoard);
void move(directions direction, coords* global, const board* gameBoard);

/*Pr�buje wstawi� do planszy pole o odpowiednim stanie, sprawdzaj�c przy tym zasady gry.
Zwraca 1 w przypadku powodzenia, Zwraca 0 i wy�wietla odpowiedni komunikat w przypadku b��du.*/
int setField(coords relative, const board* gameBoard, states state, bool editable);

/*Sprawdza czy wprowadzenie danego stanu do kom�rki o podanych wsp�rz�dnych �ama�oby zasade m�wi�c� o tym �e 
maksymalnie 2 liczby o tym samym stanie mog� ze sob� s�siadowa� w danym wierszu lub kolumnie.
Zwraca true je�li wprowadzenie nie �amie zasady,
zwraca false w przeciwnym wypadku.*/
bool checkRule1(const board* gameBoard, int x, int y, states state);

/*Sprawdza czy liczba element�w o stanie state nie przekracza po�owy kolumny/wiersza
Zwraca 0 je�eli wprowadzenie nie �amie zasady. 
Zwraca liczbe ujemna je�li przekroczona zosta�a ilo�� liczb w kolumnie,
Zwraca liczb� dodatni� je�li ilo�� liczb w wierszu przekroczy�y dozwolon� ilo��.*/
int checkRule2(const board* gameBoard, int x, int y, states state);

/*Sprawdza czy nie istnieje gdzie� identyczny wiersz/kolumna.
Zwraca 0 je�eli wprowadzenie nie �amie zasady. 
Zwraca liczb� ujemn� je�li istnieje identyczna kolumna, modu� ze zwr�conej liczby to numer tej kolumny numerowane od 1
Zwraca liczb� dodatni� je�li istnieje identyczny wiersz, zwr�cona liczba to numer tego wiersza numeruj�c od 1.*/
int checkRule3(const board* gameBoard, int x, int y, states state);

/*Pr�buje wczyta� map� z pliku o nazwie przekazanej do parametru do planszy przekazanej do parametru.
Zwraca 1 w przypadku powodzenia, Zwraca 0 i wy�wietla odpowiedni komunikat w przypadku b��du.*/
int loadMap(board* gameBoard, const char* fName);

#endif 