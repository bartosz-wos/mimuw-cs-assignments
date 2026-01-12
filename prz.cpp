#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using pll = pair<ll, ll>;

__extension__ using int128 = __int128;

#define fi first
#define se second

ll n, U;
vector<pll> punkty, przedzialy;
deque<ll> kolejka_min, kolejka_max;

// Porownuje jakosc przedzialow
// Zwraca true, jesli przedzial x ma mniejsza jakosc niz y
bool jakosc(pll x, pll y){
	ll dx = punkty[x.se].fi - punkty[x.fi].fi;
	ll lx = x.se - x.fi + 1;
	ll dy = punkty[y.se].fi - punkty[y.fi].fi;
	ll ly = y.se - y.fi + 1;
	int128 vx = (int128)dx * dx * ly;
	int128 vy = (int128)dy * dy * lx;
	return vx < vy;
}

int main(){
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);

	cin >> n >> U;
	punkty.resize(n);
	for(pll& x : punkty)
		cin >> x.fi >> x.se;

	ll r = 0;
	kolejka_min.push_front(0);
	kolejka_max.push_front(0);
	ll poprzedni=-1;

	// Wyznaczanie maksymalnych przedzialow
	// Metoda gasienicy i kolejki monotoniczne
	for(ll l = 0; l < n; l++){
		if(r < l){
			r = l;
			kolejka_min.clear();
			kolejka_max.clear();
			kolejka_max.push_front(r);
			kolejka_min.push_front(r);
		}
		// Inkrementacja r dopoki roznica max - min <= U
		while(r + 1 < n){
			ll kolejny = punkty[r + 1].se;
			ll obecny_min = punkty[kolejka_min.front()].se;
			ll obecny_max = punkty[kolejka_max.front()].se;
			if(max(obecny_max, kolejny) - min(obecny_min, kolejny) > U)
				break;
			r++;
			// Aktualizacje kolejek
			while(!kolejka_min.empty() and punkty[kolejka_min.back()].se >= punkty[r].se)
				kolejka_min.pop_back();
			while(!kolejka_max.empty() and punkty[kolejka_max.back()].se <= punkty[r].se)
				kolejka_max.pop_back();
			kolejka_min.push_back(r);
			kolejka_max.push_back(r);
		}
		// Zapisujemy jedynie, jesli przedzial nie zawiera sie w poprzednim znalezionym
		if(r > poprzedni){
			przedzialy.push_back({l, r});
			poprzedni = r;
		}
		// Sprzatanie elementow spoza gasienicy
		if(!kolejka_min.empty() and kolejka_min.front() == l)
			kolejka_min.pop_front();
		if(!kolejka_max.empty() and kolejka_max.front() == l)
			kolejka_max.pop_front();
	}

	// Kolejka zawiera indeksy przedzialow posortowane wedlug jakosci
	deque<ll> kolejka;
	ll idx = 0;
	// Szukanie najlepszego przedzialu dla kazdego indeksu
	for(ll i = 0; i < n; i++){
		// Usuwa przedzialy, ktore koncza sie przed i
		while(!kolejka.empty() and przedzialy[kolejka.front()].se < i)
			kolejka.pop_front();
		// Dodaj nowe przedzialy, ktore zaczynaja sie co najwyzej w i
		while(idx < (ll)przedzialy.size() and przedzialy[idx].fi <= i){
			// Utrzymanie monotonicznosci
			while(!kolejka.empty() and jakosc(przedzialy[kolejka.back()], przedzialy[idx]))
				kolejka.pop_back();
			kolejka.push_back(idx);
			idx++;
		}
		if(!kolejka.empty()){
			pll przedzial = przedzialy[kolejka.front()];
			cout << przedzial.fi + 1LL << ' ' << przedzial.se + 1LL << '\n';
		}
	}
}
