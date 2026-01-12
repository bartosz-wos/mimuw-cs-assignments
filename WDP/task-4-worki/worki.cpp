#include "worki.h"
#include <forward_list>
using namespace std;

// Reprezentuje wnetrze worka lub biurko
// Przechowuje liste elementow oraz liczbe przedmiotow
struct worek::Wnetrze{
    // Glowa listy
    przedmiot::Wezel* przod = nullptr;
    worek* rodzic = nullptr;
    int suma_rozmiarow = 0;
};

// Reprezentuje element listy
// Umozliwia wpiecie przedmiotu lub worka do listy
struct przedmiot::Wezel{
    przedmiot::Wezel* nastepny = nullptr;
    przedmiot::Wezel* poprzedni = nullptr;
    // Wskaznik na obiekt, w ktorym lezy ten element
    worek::Wnetrze* gdzie = nullptr;
    int rozmiar = 0;
};

using Wezel = przedmiot::Wezel;
using Wnetrze = worek::Wnetrze;

int licz_id = 0;
Wnetrze* korzen = nullptr;

// vectory do czyszczenia pamieci
forward_list<przedmiot*> czyscP;
forward_list<worek*> czyscWor;
forward_list<Wezel*> czyscWzl;
forward_list<Wnetrze*> czyscWne;

// Tworzy biurko, jesli nie istnieje
void init(){
    if(!korzen){
        korzen = new Wnetrze();
        czyscWne.push_front(korzen);
    }
}

// Tworzy nowy wezel i go zwraca
// Rozmiar to 1 dla przedmiotu, a 0 dla pustego worka
Wezel* nowy_wezel(int w){
    Wezel* nowy = new Wezel();
    nowy->rozmiar = w;
    czyscWzl.push_front(nowy);
    return nowy;
}

// Tworzy nowe wnetrze i je zwraca
Wnetrze* nowe_wnetrze(worek* wor){
    Wnetrze* nowy = new Wnetrze();
    nowy->rodzic = wor;
    czyscWne.push_front(nowy);
    return nowy;
}

// Usuwa wezel z listy i ja naprawia oraz aktualizuje sume rozmiarow
void odlacz(Wezel* w){
    Wnetrze* cur = w->gdzie;
    if(!cur) return;

    if(w->poprzedni) w->poprzedni->nastepny = w->nastepny;
    if(w->nastepny) w->nastepny->poprzedni = w->poprzedni;
    if(cur->przod == w) cur->przod = w->nastepny;
    cur->suma_rozmiarow -= w->rozmiar;
    w->gdzie = nullptr;
    w->poprzedni = nullptr;
    w->nastepny = nullptr;
}

// Wstawia wezel na poczatek listy w danym wnetrzu oraz aktualizuje sume rozmiarow
void dolacz(Wnetrze* wne, Wezel* w){
    w->gdzie = wne;
    w->poprzedni = nullptr;
    w->nastepny = wne->przod;
    if(wne->przod) wne->przod->poprzedni = w;
    wne->przod = w;
    wne->suma_rozmiarow += w->rozmiar;
}

// Aktualizuje rozmiar worka
// Jesli dodajemy cos do worka, musimy zaktualizowac jego rozmiar oraz sume rozmiarow wnetrza
void aktualizuj_rozmiar(worek* w, int wartosc){
    if(!w) return;
    Wezel* wzl = w->w;
    wzl->rozmiar += wartosc;
    if(wzl->gdzie)
        wzl->gdzie->suma_rozmiarow += wartosc;
}

przedmiot *nowy_przedmiot(){
    init();
    przedmiot* nowy = new przedmiot();
    nowy->w = nowy_wezel(1);
    dolacz(korzen, nowy->w);
    czyscP.push_front(nowy);
    return nowy;
}

worek *nowy_worek(){
    init();
    worek* nowy = new worek();
    nowy->id = licz_id++;
    nowy->srodek = nowe_wnetrze(nowy);
    nowy->w = nowy_wezel(0);
    dolacz(korzen, nowy->w);
    czyscWor.push_front(nowy);
    return nowy;
}

// Zdejmuje przedmiot z biurka i wklada do worka
void wloz(przedmiot* co, worek* gdzie){
    Wezel* w = co->w;
    Wnetrze* s = gdzie->srodek;
    odlacz(w);
    dolacz(s, w);
    aktualizuj_rozmiar(gdzie, w->rozmiar);
}

// To samo, tylko wklada worek do worka
void wloz(worek* co, worek* gdzie){
    Wezel* w = co->w;
    Wnetrze* s = gdzie->srodek;
    odlacz(w);
    dolacz(s, w);
    aktualizuj_rozmiar(gdzie, w->rozmiar);
}

// Funkcja zabiera rozwazany przedmiot i daje na biurko (podpina przedmiot do korzenia)
void wyjmij(przedmiot* p){
    init();
    Wezel* w = p->w;
    Wnetrze* skad = w->gdzie;
    worek* nalezal = skad ? skad->rodzic : nullptr;
    odlacz(w);
    if(nalezal)
        aktualizuj_rozmiar(nalezal, -w->rozmiar);
    dolacz(korzen, w);
}

// To samo, tylko zabiera worek i daje na biurko
void wyjmij(worek* w){
    init();
    Wezel* wzl = w->w;
    Wnetrze* skad = wzl->gdzie;
    worek* nalezal = skad ? skad->rodzic : nullptr;
    odlacz(wzl);
    if(nalezal)
        aktualizuj_rozmiar(nalezal, -wzl->rozmiar);
    dolacz(korzen, wzl);
}

// Zwracamy id rodzica lub -1 jesli lezy na biurku
int w_ktorym_worku(przedmiot* p){
    Wezel* w = p->w;
    if(!w->gdzie || !w->gdzie->rodzic)
        return -1;
    return w->gdzie->rodzic->id;
}

// To samo, tylko dla worka
int w_ktorym_worku(worek* w){
    Wezel* wzl = w->w;
    if(!wzl->gdzie || !wzl->gdzie->rodzic)
        return -1;
    return wzl->gdzie->rodzic->id;
}

// Dzieki suma_rozmiarow, zwracamy juz gotowa wartosc
int ile_przedmiotow(worek* w){
    return w->srodek->suma_rozmiarow;
}

// Wnetrze worka staje sie biurkiem, a poprzednie biurko wchodzi do worka
void na_odwrot(worek* w){
    Wezel* wzl = w->w;
    init();
    Wnetrze* wne = w->srodek;
    Wnetrze* stary_korzen = korzen;
    // Odlaczamy worek w od biurka, zeby nie stal sie czescia swojej wlasnej zawartosci
    odlacz(wzl);
    // Wnetrze worka staje sie nowym biurkiem
    korzen = wne;
    korzen->rodzic = nullptr;
    // Stare biurko wkladamy do srodka worka w
    w->srodek = stary_korzen;
    stary_korzen->rodzic = w;
    // Worek ma cala pozostala zawartosc starego biurka, wiec przejmuje rozmiar
    wzl->rozmiar = stary_korzen->suma_rozmiarow;
    // I koncowo kladziemy worek w na nowym biurku
    dolacz(korzen, wzl);
}

// Sprzata pamiec
void gotowe(){
    for(auto i : czyscP)
        delete i;
    for(auto i : czyscWor)
        delete i;
    for(auto i : czyscWne)
        delete i;
    for(auto i : czyscWzl)
        delete i;
    czyscP.clear();
    czyscWor.clear();
    czyscWne.clear();
    czyscWzl.clear();
    korzen = nullptr;
    licz_id = 0;
}