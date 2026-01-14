#ifndef _ZBIOR_ARY_H_
#define _ZBIOR_ARY_H_

#include <stdbool.h>

// Struktura reprezentujaca ciag arytmetyczny
typedef struct{
    int pocz;
    int kon;
} przedzial;

// Struktura reprezentujaca zbior
// Trzyma rozlaczne, w obrebie reszty mod q, ciagi arytmetyczne posortowane po (pocz mod q, pocz)
typedef struct{
    int rozmiar;
    przedzial *t;
} zbior_ary;

// Funkcja zwracajaca reszte z dzielenia przez q
int reszta(int x);

// Funkcja zwracajaca maksimum liczb a i b
int max(int a, int b);

// Funkcja zwracajaca minimum liczb a i b
int min(int a, int b);

// Funkcja zwracajaca nowy zbior_ary po dopasowaniu wielkosci tablicy do ret.rozmiar
zbior_ary skroc_tablice(zbior_ary ret);

//Funkcja zwraca nowy zbior_ary po dodaniu p na koniec ret, zakladajac porzadek
zbior_ary dodaj_scal(zbior_ary ret, przedzial p);

/* Najlepiej nie modyfikowac nic ponizej. */

// Funkcja daje w wyniku zbior reprezentujacy ciag arytmetyczny o elemencie poczatkowym a, końcowym b i roznicy q>0, tj. {a,a+q,...,b}.
// Mozesz zalozyc, ze we wszystkich zapytaniach w danym tescie wartosc q bedzie taka sama.
// Mozesz zalozyc, ze ta funkcja zostanie wywolana jako pierwsza.
zbior_ary ciag_arytmetyczny(int a, int q, int b);

// Daje w wyniku zbior zlozony z samego elementu a, tj. {a}.
zbior_ary singleton(int a);

// Daje w wyniku zbior reprezentujacy teoriomnogosciowa sume zbiorow A i B.
zbior_ary suma(zbior_ary A, zbior_ary B);

// Daje w wyniku zbior reprezentujacy czesc wspolna zbiorow A i B.
zbior_ary iloczyn(zbior_ary A, zbior_ary B);

// Daje w wyniku zbior A \ B.
zbior_ary roznica(zbior_ary A, zbior_ary B);

// Daje w wyniku true wtw. gdy liczba b nalezy do zbioru A.
bool nalezy(zbior_ary A, int b);

// Wynikiem funkcji jest liczba elementow w zbiorze A.
unsigned moc(zbior_ary A);

// Wynikiem funkcji jest Ary_q(A), czyli minimalna liczba parami rozlacznych ciagow arytmetycznych o roznicy q, ktorych suma jest zbior A.
unsigned ary(zbior_ary A);

#endif
