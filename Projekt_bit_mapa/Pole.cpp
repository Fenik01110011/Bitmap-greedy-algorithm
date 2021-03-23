#include <iostream>
#include "Pole.h"

using namespace std;

//konstruktor
Pole::Pole(string nazwa_k, int wN, int wS,int wE,int wW, int zN, int zS, int zE, int zW, int numer_k)
	:nazwa{ nazwa_k },
	wejscie_N{ wN },
	wejscie_S{ wS },
	wejscie_E{ wE },
	wejscie_W{ wW },
	zejscie_N{ zN },
	zejscie_S{ zS },
	zejscie_E{ zE },
	zejscie_W{ zW },
	numer{ numer_k },
	odwiedzone{ false },
	oN{ false },
	oS{ false },
	oE{ false },
	oW{ false }
{
	sredni_czas_zejsc_wejsc = (wN + wS + wE + wW + zN + zS + zE + zW)/8;
}

const Pole& domyslne_pole()
{
	static Pole p("Rownina", 100, 100, 100, 100, 100, 100, 100, 100, 0);
	return p;
}

//konstruktor domyslny
Pole::Pole()
	:nazwa{ domyslne_pole().s_nazwa() },
	wejscie_N{ domyslne_pole().s_wejscie_N() },
	wejscie_S{ domyslne_pole().s_wejscie_S() },
	wejscie_E{ domyslne_pole().s_wejscie_E() },
	wejscie_W{ domyslne_pole().s_wejscie_W() },
	zejscie_N{ domyslne_pole().s_zejscie_N() },
	zejscie_S{ domyslne_pole().s_zejscie_S() },
	zejscie_E{ domyslne_pole().s_zejscie_E() },
	zejscie_W{ domyslne_pole().s_zejscie_W() },
	numer{ domyslne_pole().s_numer() },
	sredni_czas_zejsc_wejsc{ domyslne_pole().s_sredni_czas_zejsc_wejsc() },
	odwiedzone{ domyslne_pole().odwiedzone },
	oN{ domyslne_pole().oN },
	oS{ domyslne_pole().oS },
	oE{ domyslne_pole().oE },
	oW{ domyslne_pole().oW }
{
	;
}