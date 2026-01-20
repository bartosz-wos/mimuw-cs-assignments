#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#define fi first
#define se second

int nwd(int a, int b){
	if(!b) return a;
	return nwd(b, a % b);
}

// Funkcja sprawdza warunki konieczne
int sprawdz_warunki_konieczne(const vector<int>& x, const vector<int>& y, const int& n){
	ll suma_x = 0;
	ll suma_y = 0;
	int nwd_x = 0;
	bool czy_pusta_lub_pelna = 0;

	for(int i = 0; i < n; i++){
		if(y[i] > x[i])
			return -1;
		if(!y[i] || x[i]==y[i])
			czy_pusta_lub_pelna = 1;
		nwd_x = nwd(x[i], nwd_x);
		suma_x += x[i];
		suma_y += y[i];
	}

	if(!suma_y)
		return 0;
	if(suma_y > suma_x)
		return -1;
	if(!czy_pusta_lub_pelna)
		return -1;
	if(nwd_x > 0){
		for(int i = 0; i < n; i++){
			if(y[i] % nwd_x)
				return -1;
		}
	}
	return 1;
}

// Przechowuje grupy indeksow takich samych szklanek
vector< vector<int> > grupy;

// Grupuje takie same szklanki, zeby zredukowac liczbe stanow w bfs'ie
void stworz_grupy(const vector<int>& x, const vector<int>& y, const int& n){
	map< pair<int, int>, vector<int> > zlicz;
	for(int i = 0; i < n; i++)
		zlicz[{x[i], y[i]}].push_back(i);
	for(auto& element : zlicz){
		if(element.se.size() > 1)
			grupy.push_back(element.se);
	}
}

// Normalizuje, czyli sortuje wode wewnatrz grup takich samych szklanek
void znormalizuj(vector<int>& v){
	for(vector<int>& grupa : grupy)
		if(grupa.size() >= 2){
			vector<int> temp;
			for(const int& x : grupa)
				temp.push_back(v[x]);
			sort(temp.begin(), temp.end());
			for(int i = 0; i < grupa.size(); i++)
				v[grupa[i]] = temp[i];
		}
}

// Sprawdza, czy wierzcholek byl odwiedzony, a jesli nie, to wrzuca do kolejki
void dodaj(vector<int> v, map< vector<int>, int >& odleglosc, queue< vector<int> >& kolejka, int odl){
	znormalizuj(v);
	if(!odleglosc.count(v)){
		odleglosc[v] = odl;
		kolejka.push(v);
	}
}

// Szukanie najkrotszej sciezki za pomoca bfs'a po grafie stanow, zaczynajac w (0,0,...,0)
int bfs(vector<int>&x, vector<int>& y, int& n){
	map< vector<int>, int > odleglosc;
	queue< vector<int> > kolejka;

	vector<int> start(n, 0);
	dodaj(start, odleglosc, kolejka, 0);

	// Stan docelowy tez musi zostac znormalizowany
	znormalizuj(y);

	while(!kolejka.empty()){
		vector<int> stan = kolejka.front();
		kolejka.pop();

		int odl = odleglosc[stan];

		if(stan == y)
			return odl;

		// Rozwazanie krawedzi w grafie
		for(int i = 0; i < n; i++){
			int zapisz_i = stan[i];

			// Wypelnienie szklanki
			if(stan[i] < x[i]){
				stan[i] = x[i];
				dodaj(stan, odleglosc, kolejka, odl + 1);
				stan[i] = zapisz_i;
			}
			// Oproznienie szklanki
			if(stan[i] > 0){
				stan[i] = 0;
				dodaj(stan, odleglosc, kolejka, odl + 1);
				stan[i] = zapisz_i;
			}
			// Przelanie ze szklanki do innej
			if(stan[i] > 0){
				for(int j = 0; j < n; j++)
					if(i != j){
						int roznica = x[j] - stan[j];
						if(roznica > 0){
							int ile = min(stan[i], roznica);
							stan[i] -= ile;
							stan[j] += ile;
							dodaj(stan, odleglosc, kolejka, odl + 1);
							stan[i] += ile;
							stan[j] -= ile;
						}
					}
			}
		}
	}
	// Rozwiazanie nie istnieje
	return -1;
}

int main(){
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);

	int n;
	cin >> n;
	vector<int> x(n), y(n);
	for(int i = 0; i < n; i++)
		cin >> x[i] >> y[i];

	int odp = sprawdz_warunki_konieczne(x, y, n);
	if(odp != 1){
		cout << odp << '\n';
		return 0;
	}

	stworz_grupy(x, y, n);
	cout << bfs(x, y, n) << '\n';
	return 0;
}
