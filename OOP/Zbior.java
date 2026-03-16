public class Zbior{
	int[] zbior;

	public Zbior(){}

	public Zbior(int[] zbior){
		this.zbior = zbior;
	}

	public Zbior przeciecie(Zbior zbior_drugi){
		int i = 0, j = 0, k = 0;
		int max_dlugosc = 0;

		if(this.zbior.length < zbior_drugi.zbior.length)
			max_dlugosc = this.zbior.length;
		else
			max_dlugosc = zbior_drugi.zbior.length;

		int[] temp = new int[max_dlugosc];

		while(i < this.zbior.length && j < zbior_drugi.zbior.length){
			if(this.zbior[i] < zbior_drugi.zbior[j])
				++i;
			else if(this.zbior[i] > zbior_drugi.zbior[j])
				++j;
			else{
				temp[k] = this.zbior[i];
				++i;
				++j;
				++k;
			}
		}

		int[] wynik = new int[k];
		for(i = 0; i < k; ++i)
			wynik[i] = temp[i];

		return new Zbior(wynik);
	}

	public void wypisz(){
		for(int i = 0; i < this.zbior.length; ++i)
			System.out.print(this.zbior[i] + (i == this.zbior.length - 1 ? "" : ", "));
		System.out.println();
	}

	public static void main(String[] in){
		int[] t1 = {1, 3, 5, 7, 11};
		int[] t2 = {3, 7, 11, 13};

		Zbior z1 = new Zbior(t1);
		Zbior z2 = new Zbior(t2);

		Zbior z3 = z1.przeciecie(z2);
		z3.wypisz();

		t1 = new int[] {-30, -21, 50, 1000000000, 2000000000};
		t2 = new int[] {-2000000000, -30, 2000000000};
		
		z1 = new Zbior(t1);
		z2 = new Zbior(t2);

		z3 = z1.przeciecie(z2);
		z3.wypisz();
	}
}
