/*
-------------------------------------------------------------------------------------------
Zadanie:
	Wyszukiwanie drogi na 2-wymiarowym terenie (bitmapa) z polami o ró¿nych ograniczeniach
-------------------------------------------------------------------------------------------
Bitmapa - wyszukiwanie drogi algorytmem zachlannym

Projekt wykonali:
	Adrian Tretyn
	Marcin Bia³ecki

Rok akademicki : 2018 / 2019
Kierunek : Informatyka
	Rok studiow : I
	Semestr : II
-------------------------------------------------------------------------------------------
*/

#define _WIN32_WINNT 0x0500
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <windows.h>
#include <conio.h>
#include <cmath>
#include "Polskie_znaki.h"
#include "Pole.h"

using namespace std;

//pobieranie uchwytow konsoli
HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
HWND console = GetConsoleWindow();
RECT r;

void wczytanie_mapy(Pole mapa[100][100]); // wczytuje mape z pliku
void wyswietl_mape(Pole mapa[100][100], int x, int y, int x1, int y1, int x2, int y2, int x3, int y3); //wyswietla mape, xy - koordynaty odkad wyswietlac mape, x1y1 - start, x2y2 - cel, x3y3 - obecne pole
void wyswietl_mape(Pole mapa[100][100], int x, int y); //wyswietla mape
void gotoxy(int x, int y); //ustawia kursor w wybrane miejsce
void wyszukiwanie_drogi(Pole mapa[100][100], int x1, int y1, int x2, int y2, bool wyszukiwanie_automatyczne); //algorytm zachlanny wyszukiwania drogi
void wyszukiwanie_drogi_1(Pole mapa[100][100]); //wywolanie algorytmu wyszukiwania drogi wraz z interfejsem dla uzytkownika
void zmiana_polozenia(Pole mapa[100][100]); //funkcja pozwalajaca przemieszczas sie po mapie

int main()
{
	Pole mapa[100][100]; //zarezerwowanie pamieci dla pol mapy 100 na 100

	//menu glowne
	while (true)
	{
		wczytanie_mapy(mapa);
		system("cls");
		cout << pl("MENU G£ÓWNE") << endl;
		cout << pl("1. Wyszukiwanie drogi") << endl;
		cout << pl("2. Ogl¹danie mapy") << endl;
		cout << pl("3. Wyjœcie") << endl;
		cout << pl("Wybierz numer:");

		char wybor;
		bool zly_wybor;
		do
		{
			zly_wybor = false;
			wybor = _getch();
			if (wybor >= '1' && wybor <= '3')
				system("cls");
			switch (wybor)
			{
			case '1': wyszukiwanie_drogi_1(mapa); break;
			case '2': zmiana_polozenia(mapa); break;
			case '3': exit(0); break;
			default:
			{
				zly_wybor = true;
				cout << endl << pl("Nie ma takiej opcji. Ponów wybór:");
			}
			}
		} while (zly_wybor);

		system("pause");
	}

	system("pause");
	DeleteObject(hOut);
	DeleteObject(console);
	return 0;
}

void wczytanie_mapy(Pole mapa[100][100])
{
	vector <Pole> rodzaje_pol;

	fstream plik1, plik2;

	plik1.open("Mapa/Rodzaje_przeszkod.txt", ios::in);

	if (plik1.good() == false)
	{
		cout << "Nie udalo sie otworzyc pliku \"Rodzaje_przeszkod.txt\"";
		system("pause");
		exit(0);
	}
	//wczytanie rodzajow pol
	string b[10]; //bufor
	for (int i = 0; getline(plik1, b[i % 10]); ++i)
		if ((i % 10) == 9)
			rodzaje_pol.push_back(Pole{ b[0], atoi(b[1].c_str()), atoi(b[2].c_str()), atoi(b[3].c_str()), atoi(b[4].c_str()), atoi(b[5].c_str()), atoi(b[6].c_str()), atoi(b[7].c_str()), atoi(b[8].c_str()), atoi(b[9].c_str()) });
	plik1.close();

	plik2.open("Mapa/Bit_mapa.txt", ios::in);
	if (plik2.good() == false)
	{
		cout << "Nie udalo sie otworzyc pliku \"Bit_mapa.txt\"";
		system("pause");
		exit(0);
	}
	//wczytanie mapy
	string bufor;
	for (int i = 0; getline(plik2, bufor); ++i)
		for (int j = 0; j < 100; ++j)
			mapa[99 - i][j] = rodzaje_pol[bufor[j*2] - 48];
	plik2.close();
}

void wyswietl_mape(Pole mapa[100][100], int x, int y, int x1, int y1, int x2, int y2, int x3, int y3)
{
	gotoxy(0, 0);

	GetWindowRect(console, &r); //stores the console's current dimensions
	//MoveWindow(window_handle, x, y, width, height, redraw_window);
	MoveWindow(console, r.left, r.top, 1274, 720, true);

	if (x > 27) x = 27;
	else if (x < 0) x = 0;
	if (y > 85) y = 85;
	else if (y < 0) y = 0;

	char bufor;
	for (int i = y; i < y + 15; ++i)
	{
		for (int j = x; j < x + 73; ++j)
		{
			if (x3 == j && y3 == i)
			{
				SetConsoleTextAttribute(hOut, BACKGROUND_RED | BACKGROUND_BLUE);
				cout << " ";
			}
			else if (x2 == j && y2 == i)
			{
				SetConsoleTextAttribute(hOut, BACKGROUND_RED | BACKGROUND_GREEN);
				cout << "M";
			}
			else if (x1 == j && y1 == i)
			{
				SetConsoleTextAttribute(hOut, BACKGROUND_RED | BACKGROUND_GREEN);
				cout << "S";
			}
			else if (mapa[j][i].odwiedzone)
			{
				SetConsoleTextAttribute(hOut, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
				cout << " ";
			}
			else
			{
				switch (mapa[j][i].s_numer())
				{
				case 0:
				{
					SetConsoleTextAttribute(hOut, BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
					cout << " ";
				}
				break;
				case 1:
				{
					SetConsoleTextAttribute(hOut, BACKGROUND_GREEN | BACKGROUND_INTENSITY);
					cout << " ";
				}
				break;
				case 2:
				{
					SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
					bufor = (char)177;
					cout << bufor;
					bufor = ' ';
				}
				break;
				case 3:
				{
					SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
					bufor = (char)176;
					cout << bufor;
				}
				break;
				case 4:
				{
					SetConsoleTextAttribute(hOut, BACKGROUND_GREEN);
					cout << " ";
				}
				break;
				case 5:
				{
					SetConsoleTextAttribute(hOut, BACKGROUND_BLUE | BACKGROUND_INTENSITY);
					cout << " ";
				}
				break;
				case 6:
				{
					SetConsoleTextAttribute(hOut, BACKGROUND_BLUE);
					cout << " ";
				}
				break;
				case 7:
				{
					SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
					bufor = (char)178;
					cout << bufor;
				}
				break;
				case 8:
				{
					SetConsoleTextAttribute(hOut, NULL);
					cout << " ";
				}
				break;
				case 9:
				{
					SetConsoleTextAttribute(hOut, BACKGROUND_RED);
					cout << " ";
				}
				break;
				default:
				{
					SetConsoleTextAttribute(hOut, BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
					cout << "?";
				}
				}
			}
		}
		SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
		cout << endl;
	}
}

void wyswietl_mape(Pole mapa[100][100], int x, int y)
{
	wyswietl_mape(mapa, x, y, -1, -1, -1, -1, -1, -1);
}

void gotoxy(int x, int y)
{
	COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(hOut, c);
}

void wyszukiwanie_drogi(Pole mapa[100][100], int x1, int y1, int x2, int y2, bool wyszukiwanie_automatyczne)
{
	system("cls");
	int x{ x1 }, y{ y1 }; //koordynaty obecnej pozycji
	int sredni_czas_pola = 0; //najkrotszy znaleziony sredni czas potrzebny na przebycie pola
	int pole = 0; //pole na ktore najlepiej sie udac,
	// 1 - polnoc, 2 - poludnie, 3 - wschod, 4 - zachod, 0 - brak mozliwosci wyjscia z obecnego pola
	int etap = 1; //który etap sprawdzania pol
	int czas_zmiany_pola = 0; //czas potrzebny na zejscie z obecnego pola i wejscie na nowe pole
	int calkowity_czas_podrozy = 0;

	while (true)
	{
		if (!mapa[x][y].odwiedzone)
		{
			mapa[x][y].odwiedzone = true;
			//zaznaczenie z ktorej strony zostalo pierwszy raz odwiedzone pole
			if (pole == 1) mapa[x][y].oS = true;
			else if (pole == 2) mapa[x][y].oN = true;
			else if (pole == 3) mapa[x][y].oW = true;
			else if (pole == 4) mapa[x][y].oE = true; 
		}
		wyswietl_mape(mapa, x - 37, y - 8, x1, y1, x2, y2, x, y);
		cout << pl("Punkt startowy: (") << x1 + 1 << ", " << 99 - y1 + 1 << ")\t";
		cout << pl("Punkt docelowy: (") << x2 + 1 << ", " << 99 - y2 + 1 << ")" << endl;
		cout << "                                                                      "; //wyczyszczenie lini tekstu
		gotoxy(0, 16);
		cout << pl("Obecne po³o¿enie: (") << x + 1 << ", " << 99 - y + 1 << ")\t";
		cout << pl("Ca³kowity czas podró¿y: ") << calkowity_czas_podrozy / 100 << "h" << endl;

		if (x == x2 && y == y2)
		{
			cout << pl("Dotarto do celu!") << endl;
			return;
		}

		sredni_czas_pola = 100000;
		pole = 0;

		for (etap = 1; etap <= 3; ++etap) // 2 etapy, w pierwszym sprawdza pola nie odwiedzone w kirunku w jaki powinien iœæ, a 2 etap sprawdza we wszystkich nieodwiedzonych polach do oko³a
		{
			if (etap != 2 || (mapa[x][y - 1].s_wejscie_S() == 0 && y > 0) || (mapa[x][y + 1].s_wejscie_N() == 0 && y < 99)) //jesli napotkano przeszkode od polnocy lub poludnia
			{
				if (((x2 - x) > 0 || etap >= 2) && x < 99) //czy trzeba isc na wschod
				{
					if ((!mapa[x + 1][y].odwiedzone) && (mapa[x + 1][y].s_wejscie_W() > 0)) //jesli pole nie zostalo jeszcze odwiedzone i jest mozliwe na nie wejscie od zachodu
					{
						if (sredni_czas_pola > mapa[x + 1][y].s_sredni_czas_zejsc_wejsc() && mapa[x + 1][y].s_sredni_czas_zejsc_wejsc() > 0) //jesli sredni czas wejscia na pole wchodnie jest mniejszy od juz wczesniej znalezionego
						{
							sredni_czas_pola = mapa[x + 1][y].s_sredni_czas_zejsc_wejsc();
							pole = 3;
						}
					}
				}
				if (((x2 - x) < 0 || etap >= 2) && x > 0)  //czy trzeba isc na zachod
				{
					if ((!mapa[x - 1][y].odwiedzone) && (mapa[x - 1][y].s_wejscie_E() > 0)) //jesli pole nie zostalo jeszcze odwiedzone i jest mozliwe na nie wejscie od wschodu
					{
						if (sredni_czas_pola > mapa[x - 1][y].s_sredni_czas_zejsc_wejsc() && mapa[x - 1][y].s_sredni_czas_zejsc_wejsc() > 0) //jesli sredni czas wejscia na pole zachodnie jest mniejszy od juz wczesniej znalezionego
						{
							sredni_czas_pola = mapa[x - 1][y].s_sredni_czas_zejsc_wejsc();
							pole = 4;
						}
					}
				}
			}

			if (etap != 2 || (mapa[x + 1][y].s_wejscie_W() == 0 && x < 99) || (mapa[x - 1][y].s_wejscie_E() == 0 && x > 0)) //jesli napotkano przeszkode od wschodu lub zachodu
			{
				if (((y2 - y) < 0 || etap >= 2) && y > 0)  //czy trzeba isc na polnoc
				{
					if ((!mapa[x][y - 1].odwiedzone) && (mapa[x][y - 1].s_wejscie_S() > 0)) //jesli pole nie zostalo jeszcze odwiedzone i jest mozliwe na nie wejscie od poludnia
					{
						if (((abs(y2 - y) > abs(x2 - x) && sredni_czas_pola >= mapa[x][y - 1].s_sredni_czas_zejsc_wejsc()) || sredni_czas_pola > mapa[x][y - 1].s_sredni_czas_zejsc_wejsc()) && mapa[x][y - 1].s_sredni_czas_zejsc_wejsc() > 0) //jesli sredni czas wejscia na pole polnocne jest mniejszy od juz wczesniej znalezionego
						{
							sredni_czas_pola = mapa[x][y - 1].s_sredni_czas_zejsc_wejsc();
							pole = 1;
						}
					}
				}
				if (((y2 - y) > 0 || etap >= 2) && y < 99)  //czy trzeba isc na poludnie
				{
					if ((!mapa[x][y + 1].odwiedzone) && (mapa[x][y + 1].s_wejscie_N() > 0)) //jesli pole nie zostalo jeszcze odwiedzone i jest mozliwe na nie wejscie od polnocy
					{
						if (((abs(y2 - y) > abs(x2 - x) && sredni_czas_pola >= mapa[x][y + 1].s_sredni_czas_zejsc_wejsc()) || sredni_czas_pola > mapa[x][y + 1].s_sredni_czas_zejsc_wejsc()) && mapa[x][y + 1].s_sredni_czas_zejsc_wejsc() > 0) //jesli sredni czas wejscia na pole poludniowe jest mniejszy od juz wczesniej znalezionego
						{
							sredni_czas_pola = mapa[x][y + 1].s_sredni_czas_zejsc_wejsc();
							pole = 2;
						}
					}
				}
			}

			if (etap >= 2 && pole <= 0) //po etapie w ktorym napotkal przeszkode jeœli nie znaleziono jeszcze odpowiedniego pola, to wraca na pole z ktorego przybyl
			{
				if (mapa[x][y].oE)
				{
					sredni_czas_pola = mapa[x + 1][y].s_sredni_czas_zejsc_wejsc();
					pole = 3;
				}
				else if (mapa[x][y].oW)
				{
					sredni_czas_pola = mapa[x - 1][y].s_sredni_czas_zejsc_wejsc();
					pole = 4;
				}
				else if (mapa[x][y].oN)
				{
					sredni_czas_pola = mapa[x][y - 1].s_sredni_czas_zejsc_wejsc();
					pole = 1;
				}
				else if (mapa[x][y].oS)
				{
					sredni_czas_pola = mapa[x][y + 1].s_sredni_czas_zejsc_wejsc();
					pole = 2;
				}
			}
			if (pole > 0) break; //przerwie petle jesli zostanie znalezione odpowiednie pole
		}
		
		//zmiana pola i naliczenie czasu potrzebnego na dotarcie do niego
		czas_zmiany_pola = 0;
		switch (pole)
		{
			case 0:
			{
				cout << pl("Nie znaleziono drogi prowadz¹cej do celu.") << endl;
				system("pause");
				return;
			}
			break;
			case 1:
			{
				czas_zmiany_pola += mapa[x][y].s_zejscie_N();
				--y;
				czas_zmiany_pola += mapa[x][y].s_wejscie_S();
			}
			break;
			case 2:
			{
				czas_zmiany_pola += mapa[x][y].s_zejscie_S();
				++y;
				czas_zmiany_pola += mapa[x][y].s_wejscie_N();
			}
			break;
			case 3:
			{
				czas_zmiany_pola += mapa[x][y].s_zejscie_E();
				++x;
				czas_zmiany_pola += mapa[x][y].s_wejscie_W();
			}
			break;
			case 4:
			{
				czas_zmiany_pola += mapa[x][y].s_zejscie_W();
				--x;
				czas_zmiany_pola += mapa[x][y].s_wejscie_E();
			}
			break;
			default:
			{
				cout << pl("Coœ posz³o nie tak.") << endl;
				system("pause");
				return;
			}
		}
		calkowity_czas_podrozy += czas_zmiany_pola;

		if (wyszukiwanie_automatyczne)
			Sleep(100);
		else if (_getch() == 27)
		{
			Sleep(500);
			return;
		}
			
	}
}

void wyszukiwanie_drogi_1(Pole mapa[100][100])
{
	int x1, y1, x2, y2;
	system("cls");
	cout << pl("Podaj koordynaty punktu startowego:") << endl;
	cout << "x = ";
	cin >> x1;
	cout << "y = ";
	cin >> y1;
	cout << pl("Podaj koordynaty punktu docelowego:") << endl;
	cout << "x = ";
	cin >> x2;
	cout << "y = ";
	cin >> y2;

	//zabezpieczenie przed wartosciami z poza przedzialu 1-100
	if (x1 < 1) x1 = 1;
	else if (x1 > 100) x1 = 100;
	if (y1 < 1) y1 = 1;
	else if (y1 > 100) y1 = 100;
	if (x2 < 1) x2 = 1;
	else if (x2 > 100) x2 = 100;
	if (y2 < 1) y2 = 1;
	else if (y2 > 100) y2 = 100;

	system("cls");
	cout << pl("Miejsce startowe: (") + to_string(x1) + "," + to_string(y1) + ")" << endl;
	cout << pl("Miejsce docelowe: (") + to_string(x2) + "," + to_string(y2) + ")" << endl;
	cout << pl("Wyszykaæ drogê automatycznie czy krokowo? (a/k)");
	char wybor;
	bool zly_wybor;
	do
	{
		zly_wybor = false;
		wybor = _getch();
		switch (wybor)
		{
		case 'a': wyszukiwanie_drogi(mapa, x1 - 1, 100 - y1, x2 - 1, 100 - y2, true); break;
		case 'k': wyszukiwanie_drogi(mapa, x1 - 1, 100 - y1, x2 - 1, 100 - y2, false); break;
		case 27: return; break;
		default:
		{
			zly_wybor = true;
			cout << endl << pl("Nie ma takiej opcji. Ponów wybór:");
		}
		}
	} while (zly_wybor);
}

void zmiana_polozenia(Pole mapa[100][100])
{
	char bufor;
	int x = 0, y = 0;
	wyswietl_mape(mapa, x, y);

	bool petla = true;
	while (petla)
	{
		gotoxy(0, 16);
		cout << pl("Ogl¹daj mapê za pomoc¹ strza³ek lub wróciæ do menu klawiszem \"Esc\"");
		bufor = _getch();
		switch (bufor)
		{
		case 77: if (x < 27) ++x; //strzalka w prawo
			break;
		case 75: if (x > 0) --x; //strzalka w lewo
			break;
		case 80: if (y < 85) ++y; //strzalka w w dol
			break;
		case 72: if (y > 0) --y; //strzalka w gore
			break;
		case 27: return; //klawisz "Esc"
		default:
		{
			;
		}
		}
		if(bufor == 72 || bufor == 75 || bufor == 77 || bufor == 80)
			wyswietl_mape(mapa, x, y);
	}
}