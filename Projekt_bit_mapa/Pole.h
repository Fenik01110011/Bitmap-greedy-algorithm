#pragma once
#include <iostream>

using namespace std;

class Pole
{
	string nazwa; //nazwa pola
	//czasy potrzebny na wejscie i zejscie z danego pola
	int wejscie_N; //polnoc (North)
	int wejscie_S; //poludnie (South)
	int wejscie_E; //wschod (East)
	int wejscie_W; //zachod (West)
	int zejscie_N;
	int zejscie_S;
	int zejscie_E;
	int zejscie_W;
	int numer; // numer pola
	int sredni_czas_zejsc_wejsc; //sredni czas zejsc i wejsc ze wszystkich stron
	
public:
	bool odwiedzone; //czy dane pole zostalo juz odwiedzone
	bool oN, oS, oE, oW; //z ktorej strony pole zostalo pierwszy raz odwiedzone 
	
	//konstruktory
	Pole(string nazwa_k, int wN, int wS, int wE, int wW, int zN, int zS, int zE, int zW, int numer);
	Pole();

	//funkcje zwracajace wartosci zmiennych klasy
	string s_nazwa() const { return nazwa; }
	int s_wejscie_N() const { return wejscie_N; }
	int s_wejscie_S() const { return wejscie_S; }
	int s_wejscie_E() const { return wejscie_E; }
	int s_wejscie_W() const { return wejscie_W; }
	int s_zejscie_N() const { return zejscie_N; }
	int s_zejscie_S() const { return zejscie_S; }
	int s_zejscie_E() const { return zejscie_E; }
	int s_zejscie_W() const { return zejscie_W; }
	int s_numer() const { return numer; }
	int s_sredni_czas_zejsc_wejsc() const { return sredni_czas_zejsc_wejsc; }

};
