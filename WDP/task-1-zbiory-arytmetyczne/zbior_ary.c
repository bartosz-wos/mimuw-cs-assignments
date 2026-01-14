#include "zbior_ary.h"
#include <stdlib.h>
#include <stdbool.h>

#define ll long long

// Roznica ciagow arytmetycznych q
int Q = -1;

int reszta(int x){
    // Rozwaza rowniez liczby ujemne
    return (x % Q + Q) % Q;
}

int max(int a, int b){
    return a > b ? a : b;
}

int min(int a, int b){
    return a < b ? a : b;
}

zbior_ary skroc_tablice(zbior_ary ret){
    if(ret.rozmiar == 0){
        free(ret.t);
        ret.t = NULL;
        return ret;
    }
    // Skraca ret.t do rozmiaru ret.rozmiar
    przedzial *nowy = realloc(ret.t, (unsigned int)ret.rozmiar * sizeof(przedzial));
    ret.t = nowy;
    return ret;
}

zbior_ary ciag_arytmetyczny(int a, int q, int b){
    // Zapisanie q do zmiennej globalnej Q, jesli widzimy ja po raz pierwszy
    if(Q == -1)
        Q = q;

    zbior_ary ret;
    ret.rozmiar = 1;
    ret.t = malloc((unsigned int)sizeof(przedzial));

    ret.t[0].pocz = a;
    ret.t[0].kon = b;
    
    return ret;
}

zbior_ary singleton(int a){
    // Tworzy singleton, wykorzystujac funkcje ciag_arytmetyczny
    return ciag_arytmetyczny(a, Q, a);
}

zbior_ary dodaj_scal(zbior_ary ret, przedzial p){
    // Jesli p jest puste, to zwracamy ret
    if(p.pocz > p.kon)
        return ret;
    
    // Probujemy zlaczyc przedzialy, jesli sie nakladaja
    if(ret.rozmiar > 0){
        przedzial *ostatni = &ret.t[ret.rozmiar-1];
        if(reszta(ostatni->pocz) == reszta(p.pocz) && (ll)p.pocz <= (ll)ostatni->kon + (ll)Q){
            if(p.kon > ostatni->kon)
                ostatni->kon = p.kon;
            return ret;
        }
    }

    // Ostatni przedzial w ret i p sa rozlaczne, wiec dodajemy p na koniec
    ret.t[ret.rozmiar++] = p;

    return ret;
}

zbior_ary suma(zbior_ary A, zbior_ary B){
    zbior_ary ret;
    ret.rozmiar = 0;

    //Maksymalny rozmiar tablicy przedzialow sumy A i B to A.rozmiar + B.rozmiar
    ret.t = malloc((unsigned int)(A.rozmiar + B.rozmiar) * sizeof(przedzial));
    
    // Wskazniki odpowiednio na przedzial w A i przedzial w B
    int i = 0, j = 0;

    while(i < A.rozmiar || j < B.rozmiar){
        przedzial *kolejny;

        // Dodalismy wszystkie przedzialy z A, dodajemy do ret elementy z sufiksu B
        if(i >= A.rozmiar)
            kolejny = &B.t[j++];
        // Dodalismy wszystkie przedzialy z B, dodajemy do ret elementy z sufiksu A
        else if(j >= B.rozmiar)
            kolejny = &A.t[i++];
        else{
            int rA = reszta(A.t[i].pocz);
            int rB = reszta(B.t[j].pocz);

            // Porownujemy oba przedzialy leksykograficznie i wybieramy mniejszy
            if(rA < rB || (rA == rB && A.t[i].pocz < B.t[j].pocz))
                kolejny = &A.t[i++];
            else
                kolejny = &B.t[j++];
        }
        // Dodajemy kolejny element do zbioru koncowego ret, w porzadku leksykograficznym
        ret = dodaj_scal(ret, *kolejny);
    }

    return skroc_tablice(ret);
}

zbior_ary iloczyn(zbior_ary A, zbior_ary B){
    zbior_ary ret;
    ret.rozmiar = 0;

    // Maksymalny rozmiar tablicy przedzialow iloczynu zbiorow A i B, to A.rozmiar + B.rozmiar
    ret.t = malloc((unsigned int)(A.rozmiar + B.rozmiar) * sizeof(przedzial));

    // Wskazniki na przedzialy odpowiednio w A i w B
    int i = 0, j = 0;

    while(i < A.rozmiar && j < B.rozmiar){
        int rA = reszta(A.t[i].pocz);
        int rB = reszta(B.t[j].pocz);

    	// Gdy reszty nie sa rowne, nie mozemy przeciac przedzialow, wiec przesuwamy wskaznik wskazujacy na mniejszy leksykograficznie przedzial
        if(rA != rB){
            rA > rB ? j++ : i++;
            continue;
        }

    	// Przeciecie przedzialow A.t[i] oraz B.t[j]
        int nowy_pocz = max(A.t[i].pocz, B.t[j].pocz);
        int nowy_kon = min(A.t[i].kon, B.t[j].kon);

    	// Jesli przedzial nie jest pusty, to dodajemy go do zbioru wynikowego
        if(nowy_pocz <= nowy_kon)
            ret = dodaj_scal(ret, (przedzial) {nowy_pocz, nowy_kon});

    	// Odpowiednio przesuwamy wskaznik, wskazujacy na przedzial konczacy sie wczesniej
        A.t[i].kon < B.t[j].kon ? i++ : j++;
    }

    return skroc_tablice(ret);
}

zbior_ary roznica(zbior_ary A, zbior_ary B){
    if(A.rozmiar == 0)
        return A;

    zbior_ary ret;
    ret.rozmiar = 0;

    // Maksymalny rozmiar zbioru przedzialow roznicy A i B to 2 * B.rozmiar + A.rozmiar, poniewaz pewne przedzialy w B moga przecinac pewne przedzialy w A na czesci
    ret.t = malloc((unsigned int)(2 * B.rozmiar + A.rozmiar) * sizeof(przedzial));

    // Wskaznik na przedzial w B
    int j = 0;

    // Tworzymy pomocniczy przedzial pusty
    przedzial PRZEDZIAL_PUSTY;
    PRZEDZIAL_PUSTY.pocz = 1;
    PRZEDZIAL_PUSTY.kon = 0;

    // Iteracja po tablicy przedzialow zbioru A
    for(int i = 0; i < A.rozmiar; i++){
        przedzial obecny_A = A.t[i];

        // Pomijamy przedzialy w B o mniejszej reszcie, bo juz na pewno nie przetna A.t[i], ani kolejnych w porzadku przedzialow
        while(j < B.rozmiar && reszta(B.t[j].pocz) < reszta(obecny_A.pocz))
            j++;

        int kopia_j = j;

        // Szukamy potencjalnych przeciec, zeby je usunac
        while(kopia_j < B.rozmiar && reszta(B.t[kopia_j].pocz) == reszta(obecny_A.pocz) && B.t[kopia_j].pocz <= obecny_A.kon){

            // Brak przeciecia
            if(B.t[kopia_j].kon < obecny_A.pocz){
                ++kopia_j;
                continue;
            }

            // Przedzial B.t[kopia_j] ostro wiekszy od obecny_A, wiec nie rozwazamy dalej
            if(B.t[kopia_j].pocz > obecny_A.kon)
                break;

            // B.t[kopia_j] zaczyna sie po poczatku obecny_A, wiec potencjalnie mozna dodac prefiks przedzialu obecny_A
            if(B.t[kopia_j].pocz > obecny_A.pocz){
                przedzial lewy;
                lewy.pocz = obecny_A.pocz;
                lewy.kon = B.t[kopia_j].pocz - Q;
                
                // Dodajemy przedzial, ktory zostal po lewej stronie po roznicy z B.t[kopia_j]
                if(lewy.pocz <= lewy.kon)
                    ret = dodaj_scal(ret, lewy);
            }

            // Potencjalny prawy przedzial po roznicy z B.t[kopia_j]
            obecny_A.pocz = B.t[kopia_j].kon + Q;

            if(obecny_A.pocz > obecny_A.kon){
                obecny_A = PRZEDZIAL_PUSTY;
                break;
            }

            kopia_j++;
        }

        // Jesli nie jest pusty, to dodajemy go do zbioru wynikowego
        if(obecny_A.pocz <= obecny_A.kon)
            ret = dodaj_scal(ret, obecny_A);

        // Przesuwamy j do kopia_j, co gwarantuje zlozonosc O(AryQ(A) + AryQ(B))
        j = kopia_j;
    }

    return skroc_tablice(ret);
}

// Wyszukiwanie binarne po tablicy uporzadkowanej po (a mod q, a)
bool nalezy(zbior_ary A, int x){
    int r = reszta(x);
    int lewy = 0, prawy = A.rozmiar - 1;

    while(lewy <= prawy){
        // (lewy + prawy) / 2 = lewy + (prawy - lewy) / 2, natomiast prawa strona zapobiega overflow
        int srodek = lewy + (prawy - lewy) / 2;
        int r_srodek = reszta(A.t[srodek].pocz);
        
        // Reszta mniejsza od pozadanej
        if(r_srodek < r)
            lewy = srodek + 1;
        // Reszta wieksza od pozadanej
        else if(r_srodek > r)
            prawy = srodek - 1;
        else{
            // Reszta rowna x mod q, szukamy przedzialu, w ktorym zawiera sie x
            if(A.t[srodek].kon < x)
                lewy = srodek + 1;
            else if(A.t[srodek].pocz > x)
                prawy = srodek - 1;
            else
                return true;
        }
    }
    
    // Nie znaleziono elementu
    return false;
}

unsigned int moc(zbior_ary A){
    unsigned int licznik = 0;
    
    for(int i = 0; i < A.rozmiar; i++)
        // Liczba elementow w ciagu arytmetycznym od A.t[i].pocz do A.t[i].kon o roznicy q
        licznik += (unsigned int) (((ll)A.t[i].kon - (ll)A.t[i].pocz) / Q + 1);

    return licznik;
}

unsigned int ary(zbior_ary A){
    // A.rozmiar = Aryq(A)
    return (unsigned int)A.rozmiar;
}


