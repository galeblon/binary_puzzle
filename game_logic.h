#ifndef game_logic
#define game_logic
#include"conio2.h"
#include<stdio.h>

#define DEFAULT_SIZE 8
#define ORIGIN_X 40
#define ORIGIN_Y 2
#define NULL 0
#define MIN_RAND_THRESHOLD 30
#define MAX_RAND_THRESHOLD 40


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
	RULE2_COUNTER,
	AUTO_MODE,
	MOVE_UP,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT,
	SET_FIELD_1,
	SET_FIELD_0,
	UNSET_FIELD,
	CHECK_CONTRADICTION,
	CHECK_UNAMBIGUOUS,
	UNDEFINED_ACTION
};
struct flags{
	bool simpleTipToggle = false;
	bool rule2CountToggle = false;
	bool autoMode = false;
};
struct field {
	states state;
	bool editable = true;
};
struct coords {
	int x = 0;
	int y = 0;
	void setCoord(int a, int b) { x = a; y = b; };
};


struct board {
	coords originPoint;
	int size = DEFAULT_SIZE;
	field** plane = NULL;

	/*Zwraca 1 je�eli podano odpowiedni rozmiar,
	Zwraca 0 w przeciwnym wypadku*/
	int initialize(int newSize);

	/*Pobiera liczb� od u�ytkownika i pr�buje utworzy� now� plansz� w kolejno�ci plik, losowanie.*/
	void resize();

	/*Zwalnia pami�� zajmowan� przez plansze i ustawia wska�nik na NULL*/
	void cleanUp();

	void show(int color);
	void randomize();
};


/*Odczytuje klawisz i zwraca akcje przypisan� do niego.*/
actions getAction();

/*Wybiera odpowiedni� funkcj� do podanej akcji.*/
void parseAction(board &gameBoard, actions action, coords& global_c, flags& gameFlags);

coords globalToRelative(coords global, const board *gameBoard);
coords relativeToGlobal(coords relative, const board *gameBoard);
void move(directions direction, coords* global, const board* gameBoard);

/*Pr�buje wstawi� do planszy pole o odpowiednim stanie, sprawdzaj�c przy tym zasady gry.
Zwraca 1 w przypadku powodzenia, Zwraca 0 i wy�wietla odpowiedni komunikat w przypadku b��du.*/
int setField(coords relative, const board* gameBoard, states state, bool editable, bool showError);

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
int loadMap(board* gameBoard, const char* fName, bool showError);

/*Sprawdza czy dana warto�� nie �amie �adnej zasady w danym polu, zwraca true je�li tak jest.
zwraca false w przeciwnym wypadku.*/
bool isValidInput(const board* gameBoard, int x, int y, states state);

/*Przechodzi przez plansze i podswietla punkty w ktorych nie mo�na nic wpisa�.
Zwraca 1 je�eli znajdzie si� pole sprzeczne.*/
int checkContradiction(const board* gameBoard, bool visible);

/*W trybie visible Przechodzi przez plansze i podswietla punkty w kt�rych mo�na wpisa� dok�adnie 1 warto��. Wci�ni�cie 'w' wype�ni te pola.
W trybie non-visible wype�nia pola bez interakcji z u�ytkownikiem. 
Zwraca warto�� 0 je�eli nie ma �adnego pola jednoznacznego*/
int checkUnambiguous(board* gameBoard, bool visible);

/*Przepisuje niepuste pola z source do destination, size to rozmiar jednego wymiaru kwadratowej tablicy.*/
void cpField(field** source, field** destination, int size);

/*Zwraca ilo�� p�l wyst�puj�cych w danym wierszy o danej warto�ci.*/
int countInRow(const board* gameBoard, int row, states state);

/*Zwraca ilo�� p�l wyst�puj�cych w danej kolumnie o danej warto�ci.*/
int countInCol(const board* gameBoard, int col, states state);

#endif 