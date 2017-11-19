
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdlib.h>
#include <fstream>
#include <time.h>
#include <limits.h>

const int N = 200; // wymiar macierzy (liczba wierzcholkow
const int ZAGESZCZENIE_KROK = 40;
const int ZAGESZCZENIE_MIN = 30;    // <30;30+40>
const int W = N;
const float ZAG_CONST=0.7;
int maksymalnyKolor=0;


//const int LICZBA_KOLOROW = 20;

using namespace std;

int KOLORY[N];

#pragma region LOSOWANIE

void losuj_jedynki(int tab[N][N])
{

    int licz = 0, zageszczenie = ((N*(N - 1)) / 2)*ZAG_CONST;
    while (licz < zageszczenie)
    {
        int wiersz = rand() % (N - 1);
        int kolumna = (rand() % (N - wiersz - 1)) + wiersz + 1;
        if (!tab[wiersz][kolumna])
        {
            tab[wiersz][kolumna] = 1;
            licz++;
        }
    }
    for (int i = 0; i<N; i++)
    {
        //int wiersz = rand() % (N - 1);
        for (int j = 0; j<i; j++)
        {

            tab[i][j] = tab[j][i];

        }
    }

}

void mieszaj(int tab[N][N])
{
    for (int j = 0; j < N; j++)
    {
        int w1 = rand() % (N - 1);
        int w2 = rand() % (N - 1);
        while (w1 == w2)
        {
            w2 = rand() % (N - 1);
        }
        for (int i = 0; i < N; i++)
        {
            int temp = tab[w1][i];
            tab[w1][i] = tab[w2][i];
            tab[w2][i] = temp;
        }
        for (int i = 0; i < N; i++)
        {
            int temp = tab[i][w1];
            tab[i][w1] = tab[i][w2];
            tab[i][w2] = temp;
        }
    }

}
void wyswietl(int tab[N][N])
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cout << tab[i][j] << " ";
        }
        cout << endl;
    }
}


#pragma region LISTA
struct wartosc
{

    int liczba;
    wartosc *nastepna;    // wskaŸnik na nastêpny element
    wartosc();            // konstruktor
};

wartosc::wartosc()
{
    nastepna = NULL;       // konstruktor
}

struct lista
{
    wartosc *pierwsza;  // wskaŸnik na pocz¹tek listy
    void dodaj(int liczba);
    void wyswietl_liste();
    bool przeszukaj_liste(int kolor);
    lista();
};

lista::lista()
{
    pierwsza = NULL;       // konstruktor
}

void lista::wyswietl_liste()
{
    wartosc *temp = pierwsza;

    while (temp)
    {
        cout << temp->liczba<< " ";
        // znajdujemy wskaznik na ostatni element
        temp = temp->nastepna;
    }
    cout << endl;
}


bool lista::przeszukaj_liste(int kolor)
{
    wartosc *temp = pierwsza;

    while (temp)
    {
        int i = temp->liczba;
        if (KOLORY[i] == kolor)return false;
        // znajdujemy wskaznik na ostatni element
        temp = temp->nastepna;
    }
    return true;
}

void lista::dodaj(int liczba)
{
    wartosc *nowa = new wartosc;    // tworzy nowy element listy
    // wypelniamy naszymi danymi

    nowa->liczba = liczba;
    wartosc *temp = pierwsza;
    if (pierwsza == 0) // sprawdzamy czy to pierwszy element listy
    {
        pierwsza = nowa;
        pierwsza->nastepna = NULL;
    }
    else
    {
        while (temp->nastepna != NULL)
        {
            temp = temp->nastepna;

        }
        nowa->nastepna = temp->nastepna;
        temp->nastepna = nowa;

    }
}

#pragma endregion

void make_list(int macierz[N][N], lista *tab_incydencji[N])
{
    for (int i = 0; i < N; i++)
    {
        lista *vertex = new lista;
        tab_incydencji[i] = vertex;
        for (int j = 0; j < N; j++)
        {
            if (macierz[i][j])
            {
                vertex->dodaj(j);
            }
        }
    }
}

void wyswietl_liste_incydencji(lista *tab_incydencji[N])
{
    for (int i = 0; i < N; i++)
    {
        cout << i << "(kolor: "<<KOLORY[i]<< "): ";
        tab_incydencji[i]->wyswietl_liste();
        cout << endl;
    }
}

void pokoloruj(lista *tab_incydencji[N])
{
    for (int i = 0; i < N; i++)
    {

        bool szukaj = true;
        int obecnyKolor = 1;
        while (szukaj)
        {
            if (tab_incydencji[i]->przeszukaj_liste(obecnyKolor))
            {
                szukaj = false;
                break;
            }
            if(szukaj)obecnyKolor++;
        }
        KOLORY[i] = obecnyKolor;
        if(obecnyKolor>maksymalnyKolor)maksymalnyKolor=obecnyKolor;
    }
}

void zapisz_liste(int macierz[N][N])
{
    ofstream wyj;
    wyj.open("instancja.txt");
    wyj<<N<<" "<<endl;
    for(int i=0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            if(macierz[i][j] == 1)
            {
                wyj<<(i+1)<<" "<<(j+1)<<endl;
            }
        }
    }
    wyj.close();
}

int main()
{
    srand(time(NULL));
//    ofstream wyj;
//    wyj.open("wyniki1.txt");
    ifstream wej;
    wej.open("dane.txt");

    int macierz[N][N] { 0 }; //wszystkie elementy na 0
//    lista *tab_incydencji[W];
//    int nic;
//    wej >> nic >> nic;
//    for(int i=0; i<W; i++)
//    {
//
//        lista *vertex = new lista;
//        tab_incydencji[i] = vertex;
//    }
//    int numer_wierzcholka;
//    int sasiad;
//    int p=0;
//    while(!wej.eof())
//    {
//        p++;
//        wej>>numer_wierzcholka;
//        numer_wierzcholka-=1;
//        wej>>sasiad;
//        sasiad-=1;
//       // cout << numer_wierzcholka << " XX " << sasiad << endl;
//        tab_incydencji[sasiad]->dodaj(numer_wierzcholka);
//        //tab_incydencji[numer_wierzcholka]->dodaj(sasiad);
//    }



    losuj_jedynki(macierz);
    zapisz_liste(macierz);
//    mieszaj(macierz);

//    wyswietl(macierz);
    lista *tab_incydencji[N];//TABLICA WSKAZNIKOW NA LISTY SASIADOW KAZDEGO WIERZCHOLKA
    make_list(macierz, tab_incydencji);
    pokoloruj(tab_incydencji);
//    wyswietl_liste_incydencji(tab_incydencji);
    cout << "MAX KOLOR: " << maksymalnyKolor << endl;



    wej.close();
    return 0;
}
