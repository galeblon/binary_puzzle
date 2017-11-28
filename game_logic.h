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
	void cleanUp();

	void show(int color);
	void randomize();
};

actions getAction();
coords globalToRelative(coords global, const board &gameBoard);
void move(directions direction, coords* global, const board* gameBoard);

/*Próbuje wstawiæ do planszy pole o odpowiednim stanie, sprawdzaj¹c przy tym zasady gry.
Zwraca 1 w przypadku powodzenia, Zwraca 0 i wyœwietla odpowiedni komunikat w przypadku b³êdu.*/
int setField(coords relative, const board* gameBoard, states state, bool editable);

/*Sprawdza czy wprowadzenie danego stanu do komórki o podanych wspó³rzêdnych ³ama³oby zasade mówi¹c¹ o tym ¿e 
maksymalnie 2 liczby o tym samym stanie mog¹ ze sob¹ s¹siadowaæ w danym wierszu lub kolumnie.
Zwraca true jeœli wprowadzenie nie ³amie zasady, zwraca false w przeciwnym wypadku.*/
bool checkRule1(const board* gameBoard, int x, int y, states state);

/*Zwraca 0 w przypadku powodzenia liczbe ujemna to liczby w kolumnie,
liczba dodatnia to liczby w wierszu przekroczyly dozwolona ilosc*/
int checkRule2(const board* gameBoard, int x, int y, states state);
bool checkRule3(const board* gameBoard, int x, int y, states state);

/*Próbuje wczytaæ mapê z pliku o nazwie przekazanej do parametru do planszy przekazanej do parametru.
Zwraca 1 w przypadku powodzenia, Zwraca 0 i wyœwietla odpowiedni komunikat w przypadku b³êdu.*/
int loadMap(board* gameBoard, const char* fName);

#endif 