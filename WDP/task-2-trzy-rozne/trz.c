#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

#define ll long long

// Pierwsza wartosc wieksza badz rowna
int lower_bound(int *t, int len, int wartosc){
    int lo = 0, hi = len;
    while(lo < hi){
        int mid = lo + (hi - lo) / 2;
        if(t[mid] < wartosc)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

// Funkcja szuka najblizszej trojki
// Gasienica, ktora dla kazdego r znajduje najwieksze takie l, ze na przedziale [l, r] istnieja dokladnie 3 kolory (jesli takie l istnieje)
// Majac dwa rozne kolory pod indeksami l i r, szukamy na przedziale stacji 3 koloru, ktora jest jak najblizej srodka przedzialu
int liczMinMax(int n, int *sieci, int *pos, int *ileS, int *offS, int *colPos, int maxS){

    // Tablica zliczajaca wystapenia sieci na rozpatrywanym przedziale
    int *licz = malloc((unsigned int)(maxS + 5) * sizeof(int));
    memset(licz, 0, (unsigned int)(maxS + 5) * sizeof(int));

    int obecne[4];
    int ileRoz = 0;
    int l = 0;
    int wynik = INT_MAX;

    for(int r = 0; r < n; r++){
        int siecR = sieci[r];

	    // SiecR wchodzi do przedzialu
        if(licz[siecR] == 0){
            obecne[ileRoz] = siecR;
            ileRoz++;
        }
        licz[siecR]++;

	    // Jesli mamy wiecej niz 3 kolory, przesuwamy l, tym samym zmniejszajac liczbe kolorow w przedziale
        while(ileRoz > 3 && l <= r){
            int siecL = sieci[l];
            licz[siecL]--;
            if(licz[siecL] == 0){
                int znajdz = -1;
                for(int t = 0; t < ileRoz; t++)
                    if(obecne[t] == siecL){
                        znajdz = t;
                        break;
                    }
                if(znajdz != -1){
                    obecne[znajdz] = obecne[ileRoz - 1];
                    ileRoz--;
                }
            }
            l++;
        }

        if(ileRoz == 3){

	        // Szukamy najwiekszego l, nastepnie szukamy stacji 3 koloru najblizszej srodkowi przedzialu
            // To gwarantuje, ze sieci[l] != sieci[r]
            while(l <= r && licz[sieci[l]] > 1){
                licz[sieci[l]]--;
                l++;
            }

            int siecA = sieci[l];
            int siecC = sieci[r];
            int siecB = -1;

            for(int k = 0; k < ileRoz; k++){
                if(obecne[k] != siecA && obecne[k] != siecC)
                    siecB = obecne[k];
            }

            int posA = pos[l];
            int posC = pos[r];
            int *posSieciB = (int *)colPos + offS[siecB];

	        // Szukamy stacji o 3 kolorze, ktora jest nablizej srodka przedzialu
            ll srodekX2 = (ll)posA + (ll)posC;
            ll srodek = ((ll)posA + (ll)posC + 1) / 2LL;

            int idx = lower_bound(posSieciB, ileS[siecB], (int)srodek);


	        // Sprawdzanie kandydatow idx i idx - 1
            int bestPosB = -1;
            ll best = LLONG_MAX;

            for(int k = idx - 1; k <= idx; k++){
                if(k >= 0 && k < ileS[siecB]){
                    int posB = posSieciB[k];
                    if(posB >= posA && posB <= posC){
                        ll d = llabs((ll)posB * 2LL - srodekX2);
                        if(d < best){
                            best = d;
                            bestPosB = posB;
                        } else if(d == best){
                            int d1 = bestPosB - posA;
                            int d2 = posC - bestPosB;
                            int cst = d1 > d2 ? d1 : d2;
                            int d1nw = posB - posA;
                            int d2nw = posC - posB;
                            int cstnw = d1nw > d2nw ? d1nw : d2nw;
    
                            if(cstnw < cst)
                                bestPosB = posB;
                        }
                    }
                }
            }

            // Aktualizujemy wynik
           if(bestPosB != -1){
                int d1 = bestPosB - posA;
                int d2 = posC - bestPosB;
                int cst = d1 > d2 ? d1 : d2;
                if(cst < wynik)
                    wynik = cst;
            }
        }
    }

    free(licz);
    if(wynik == INT_MAX)
        wynik = 0;
    return wynik;
}

// Zwraca prawde lub falsz, w zaleznosci czy istnieje trojka (A, B, C) o roznych sieciach taka, ze min(B - A, C - B) >= D
// Subrutyna wyszukiwania binarnego
bool sprawdz(int D, int n, int *sieci, int *pos, int maxS){
    if(n < 3 || D < 0)
        return 0;

    // Tablice, ktore trzymaja pierwsze napotkane sieci stacji, ktore sa oddalone o co najmniej D
    // 3 tablice na strone gwarantuje znalezienie odpowiedzi
    // L1[i], L2[i], L3[i] (te rozne od -1) sa parami rozne
    int *L1 = (int *)malloc((unsigned int)n * 3 * sizeof(int));
    int *L2 = L1 + n;
    int *L3 = L2 + n;
    int *R1 = (int *)malloc((unsigned int)n * 3 * sizeof(int));
    int *R2 = R1 + n;
    int *R3 = R2 + n;

    // Tablica widzianych kolorow w przedziale
    bool *vis = (bool *)malloc((unsigned int)(maxS + 1) * sizeof(bool));
    memset(vis, 0, (unsigned int)(maxS + 1) * sizeof(bool));

    // Obliczanie tablic L1, L2, L3
    int a = -1, b = -1, c = -1;
    int l = 0;
    for(int i = 0; i < n; ++i){
        while(l < i && (ll)pos[l] <= (ll)pos[i] - D){
            int siecL = sieci[l];
            if(!vis[siecL]){
                vis[siecL] = true;
                if(a == -1) 
			        a = siecL;
                else if(b == -1 && siecL != a)
            		b = siecL;
                else if(c == -1 && siecL != a && siecL != b)
            		c = siecL;
            }
            ++l;
        }
        L1[i] = a;
        L2[i] = b;
        L3[i] = c;
    }

    memset(vis, 0, (unsigned int)(maxS + 1) * sizeof(bool));

    // Obliczanie tablic R1, R2, R3
    a = b = c = -1;
    int r = n - 1;
    for(int i = n - 1; i >= 0; --i){
        while(r > i && (ll)pos[r] >= (ll)pos[i] + D){
            int siecR = sieci[r];
            if(!vis[siecR]){
                vis[siecR] = true;
                if(a == -1)
            		a = siecR;
                else if(b == -1 && siecR != a)
            		b = siecR;
                else if(c == -1 && siecR != a && siecR != b)
            		c = siecR;
            }
            --r;
        }
        R1[i] = a;
        R2[i] = b;
        R3[i] = c;
    }
    free(vis);

    // Szukamy trojki sieci
    // Siec pod indeksem i traktujemy jako srodkowa, szukamy A i C
    for(int i = 0; i < n; ++i){
        int siecB = sieci[i];

	    int L[3] = {L1[i], L2[i], L3[i]};
	    int R[3] = {R1[i], R2[i], R3[i]};

        for(int idxA = 0; idxA < 3; ++idxA){
            int siecA = L[idxA];

            if(siecA != -1 && siecA != siecB){
                for(int idxC = 0; idxC < 3; ++idxC){
                    int siecC = R[idxC];
                    if(siecC != -1 && siecC != siecB && siecA != siecC){
                        free(L1); 
                        free(R1);
                        return true;
                    }
                }
            }
        }
    }

    free(L1);
    free(R1);

    return false;
}

// Wyszukiwanie binarne po wyniku
// Maksymalizuje minimum odleglosci
int liczMaxMin(int n, int *sieci, int *pos, int maxS){
    if(n < 3)
        return 0;

    int lo = 0;
    int hi = pos[n - 1] - pos[0];
    int ans = 0;

    while(lo <= hi){
        int mid = lo + (hi - lo) / 2;
        if(sprawdz(mid, n, sieci, pos, maxS)){
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

int main(){
    int n;
    scanf("%d", &n);

    int *sieci = malloc((unsigned int)n * sizeof(int));
    int *pos = malloc((unsigned int)n * sizeof(int));

    for(int i = 0; i < n; ++i)
        scanf("%d %d", &sieci[i], &pos[i]);

    if(n < 3){
        printf("0 0\n");

        free(sieci);
        free(pos);
        return 0;
    }

    int maxS = n;
    int *ileS = malloc((unsigned int)(maxS + 2) * sizeof(int));
    memset(ileS, 0, (unsigned int)(maxS + 2) * sizeof(int));

    for(int i = 0; i < n; i++)
        ileS[sieci[i]]++;

    int sieci_cnt = 0;
    for(int c = 1; c <= maxS; c++){
        if(ileS[c] > 0)
            sieci_cnt++;
    }

    if(sieci_cnt < 3){
        printf("0 0\n");

        free(sieci);
        free(pos);
        free(ileS);
        return 0;
    }

    // offS to tablica sum prefiksowych, ktora mowi na jakim indeksie zaczynaja sie pozycje koloru c
    int *offS = malloc((unsigned int)(maxS + 2) * sizeof(int));
    offS[1] = 0;
    for(int c = 1; c <= maxS; c++)
        offS[c + 1] = offS[c] + ileS[c];

    int *ptrS = malloc((unsigned int)(maxS + 2) * sizeof(int));
    for(int c = 1; c <= maxS + 1; c++)
        ptrS[c] = offS[c];

    int *colPos = malloc((unsigned int)n * sizeof(int));

    // Tworzenie tablicy, w ktorej pozycje sa pogrupowane wobec sieci (sortowanie po (kolor, pozycja))
    for(int i = 0; i < n; i++){
        colPos[ptrS[sieci[i]]] = pos[i];
	    ptrS[sieci[i]]++;
    }

    // Wyniki dla obu funkcji
    int res_minmax = liczMinMax(n, sieci, pos, ileS, offS, colPos, maxS);
    int res_maxmin = liczMaxMin(n, sieci, pos, maxS);

    printf("%d %d\n", res_minmax, res_maxmin);

    free(sieci);
    free(pos);
    free(ileS);
    free(offS);
    free(ptrS);
    free(colPos);

    return 0;
}
