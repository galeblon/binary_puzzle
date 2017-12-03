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

	/*Zwraca 1 je¿eli podano odpowiedni rozmiar,
	Zwraca 0 w przeciwnym wypadku*/
	int initialize(int newSize);

	/*Pobiera liczbê od u¿ytkownika i próbuje utworzyæ now¹ planszê w kolejnoœci plik, losowanie.*/
	void resize();

	/*Zwalnia pamiêæ zajmowan¹ przez plansze i ustawia wskaŸnik na NULL*/
	void cleanUp();

	void show(int color);
	void randomize();
};


/*Odczytuje klawisz i zwraca akcje przypisan¹ do niego.*/
actions getAction();

/*Wybiera odpowiedni¹ funkcjê do podanej akcji.*/
void parseAction(board &gameBoard, actions action, coords& global_c, flags& gameFlags);

coords globalToRelative(coords global, const board *gameBoard);
coords relativeToGlobal(coords relative, const board *gameBoard);
void move(directions direction, coords* global, const board* gameBoard);

/*Próbuje wstawiæ do planszy pole o odpowiednim stanie, sprawdzaj¹c przy tym zasady gry.
Zwraca 1 w przypadku powodzenia, Zwraca 0 i wyœwietla odpowiedni komunikat w przypadku b³êdu.*/
int setField(coords relative, const board* gameBoard, states state, bool editable, bool showError);

/*Sprawdza czy wprowadzenie danego stanu do komórki o podanych wspó³rzêdnych ³ama³oby zasade mówi¹c¹ o tym ¿e 
maksymalnie 2 liczby o tym samym stanie mog¹ ze sob¹ s¹siadowaæ w danym wierszu lub kolumnie.
Zwraca true jeœli wprowadzenie nie ³amie zasady,
zwraca false w przeciwnym wypadku.*/
bool checkRule1(const board* gameBoard, int x, int y, states state);

/*Sprawdza czy liczba elementów o stanie state nie przekracza po³owy kolumny/wiersza
Zwraca 0 je¿eli wprowadzenie nie ³amie zasady. 
Zwraca liczbe ujemna jeœli przekroczona zosta³a iloœæ liczb w kolumnie,
Zwraca liczbê dodatni¹ jeœli iloœæ liczb w wierszu przekroczy³y dozwolon¹ iloœæ.*/
int checkRule2(const board* gameBoard, int x, int y, states state);

/*Sprawdza czy nie istnieje gdzieœ identyczny wiersz/kolumna.
Zwraca 0 je¿eli wprowadzenie nie ³amie zasady. 
Zwraca liczbê ujemn¹ jeœli istnieje identyczna kolumna, modu³ ze zwróconej liczby to numer tej kolumny numerowane od 1
Zwraca liczbê dodatni¹ jeœli istnieje identyczny wiersz, zwrócona liczba to numer tego wiersza numeruj¹c od 1.*/
int checkRule3(const board* gameBoard, int x, int y, states state);

/*Próbuje wczytaæ mapê z pliku o nazwie przekazanej do parametru do planszy przekazanej do parametru.
Zwraca 1 w przypadku powodzenia, Zwraca 0 i wyœwietla odpowiedni komunikat w przypadku b³êdu.*/
int loadMap(board* gameBoard, const char* fName, bool showError);

/*Sprawdza czy dana wartoœæ nie ³amie ¿adnej zasady w danym polu, zwraca true jeœli tak jest.
zwraca false w przeciwnym wypadku.*/
bool isValidInput(const board* gameBoard, int x, int y, states state);

/*Przechodzi przez plansze i podswietla punkty w ktorych nie mo¿na nic wpisaæ.
Zwraca 1 je¿eli znajdzie siê pole sprzeczne.*/
int checkContradiction(const board* gameBoard, bool visible);

/*W trybie visible Przechodzi przez plansze i podswietla punkty w których mo¿na wpisaæ dok³adnie 1 wartoœæ. Wciœniêcie 'w' wype³ni te pola.
W trybie non-visible wype³nia pola bez interakcji z u¿ytkownikiem. 
Zwraca wartoœæ 0 je¿eli nie ma ¿adnego pola jednoznacznego*/
int checkUnambiguous(board* gameBoard, bool visible);

/*Przepisuje niepuste pola z source do destination, size to rozmiar jednego wymiaru kwadratowej tablicy.*/
void cpField(field** source, field** destination, int size);

/*Zwraca iloœæ pól wystêpuj¹cych w danym wierszy o danej wartoœci.*/
int countInRow(const board* gameBoard, int row, states state);

/*Zwraca iloœæ pól wystêpuj¹cych w danej kolumnie o danej wartoœci.*/
int countInCol(const board* gameBoard, int col, states state);

#endif 