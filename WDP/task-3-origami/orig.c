#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const double EPS = 1e-6;

// Uniwersalna struktura dla 3 typow kartek
typedef struct{
	char postac; // P - prostokat, K - kolo, Z - zgiecie
	int rodzic; // Indeks kartki, ktora zginamy
	double x1, y1, x2, y2; // Wspolrzedne prostokata dla postaci P lub srodek kola w (x1, y1) dla typu K
	double r; // Promien kola dla postaci K
} kartka;

kartka *kartki;

// Zwraca kwadrat liczby x
double kwadrat(double x){
	return x * x;
}

// Iloczyn wektorowy w plaszczyznie 2D dla [x2, y2] i [x3, y3] zaczepionych w [x1, y1]
// > 0 - lewa strona, < 0 - prawa strona, = 0 - wspolliniowe
double iloczyn_wektorowy(double x1, double y1, double x2, double y2, double x3, double y3){
	return (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
}

// Odbicie punktu (x3, y3) przez prosta ((x1, y1), (x2, y2))
// Wynik przechowujemy w (*cx, *cy)
void odbij(double x1, double y1, double x2, double y2, double x3, double y3, double *cx, double *cy){
	double dx = x2 - x1;
	double dy = y2 - y1;
	double t = ((x3 - x1) * dx + (y3 - y1) * dy) / (kwadrat(dx) + kwadrat(dy));

	*cx = 2.0 * (x1 + t * dx) - x3;
	*cy = 2.0 * (y1 + t * dy) - y3;
}

// Funkcja zliczajaca przeciecia
// Od konca do poczatku, pozwala to odbijac jedynie pytany punkt przez prosta, zamiast zginac cala kartke od poczatku
int policz_przebicia(int k, double x, double y){
	kartka *obecna = &kartki[k];

	// Prostokat
	if(obecna->postac == 'P')
		return (x >= obecna->x1 - EPS && x <= obecna->x2 + EPS && y >= obecna->y1 - EPS && y <= obecna->y2 + EPS);

	// Kolo
	if(obecna->postac == 'K')
		return (kwadrat(x - obecna->x1) + kwadrat(y - obecna->y1) <= kwadrat(obecna->r) + EPS);

	// Zgiecie
	if(obecna->postac == 'Z'){
		double iw = iloczyn_wektorowy(obecna->x1, obecna->y1, obecna->x2, obecna->y2, x, y);

		// Prawa strona
		if(iw < -EPS)
			return 0;
		// Lewa strona, wiec sprawdzamy punkt i jego odbicie
		if(iw > EPS){
			double cx, cy;
			odbij(obecna->x1, obecna->y1, obecna->x2, obecna->y2, x, y, &cx, &cy);
			return policz_przebicia(obecna->rodzic, x, y) + policz_przebicia(obecna->rodzic, cx, cy);
		}
		// Punkty sa wspolliniowe, wiec liczymy raz
		return policz_przebicia(obecna->rodzic, x, y);
	}

	return 0;
}



int main(){
	int n, q;
	scanf("%d %d", &n, &q);

	// Alokacja tablicy na wszystkie n kartek
	kartki = (kartka *)malloc((unsigned int)(n+1) * sizeof(kartka));

	for(int i = 1; i <= n; i++){
		scanf(" %c", &kartki[i].postac);

		if(kartki[i].postac == 'P')
			scanf("%lf %lf %lf %lf", &kartki[i].x1, &kartki[i].y1, &kartki[i].x2, &kartki[i].y2);
		else if(kartki[i].postac == 'K')
			scanf("%lf %lf %lf", &kartki[i].x1, &kartki[i].y1, &kartki[i].r);
		else if(kartki[i].postac == 'Z')
			scanf("%d %lf %lf %lf %lf", &kartki[i].rodzic, &kartki[i].x1, &kartki[i].y1, &kartki[i].x2, &kartki[i].y2);
	}

	// Odpowiedzi na zapytania
	while(q--){
		int k;
		double x, y;
		scanf("%d %lf %lf", &k, &x, &y);
		printf("%d\n", policz_przebicia(k, x, y));
	}

	free(kartki);
	return 0;
}
