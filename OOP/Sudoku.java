import java.util.Scanner;

public class Sudoku{
	public static void main(String[] in){
		Plansza plansza = new Plansza();

		if(plansza.wczytaj()){
			if(plansza.rozwiaz()){
				plansza.wypisz();
			}else{
				System.out.println("BRAK ROZWIĄZANIA");
			}
		}
	}
}

class Plansza{
	private final int SIZE = 9;
	private int[][] grid = new int[SIZE][SIZE];
	private Scanner scanner = new Scanner(System.in);

	public boolean wczytaj(){
		int r = 0;
		while(scanner.hasNextLine() && r < SIZE){
			String wiersz = scanner.nextLine().trim();
			if(wiersz.isEmpty() || wiersz.startsWith("+")){
				continue;
			}

			wiersz = wiersz.replace("|", "");
			for(int c = 0; c < SIZE; c++){
				if(wiersz.charAt(c) == '.'){
					grid[r][c] = 0;
				}else{
					grid[r][c] = wiersz.charAt(c) - '0';
				}
			}
			
			r++;
		}

		return r == SIZE;
	}

	public void wypisz(){
		for(int i = 0; i < SIZE; i++){
			if(i % 3 == 0){
				System.out.println("+---+---+---+");
			}
			for(int j = 0; j < SIZE; j++){
				if(j % 3 == 0){
					System.out.print("|");
				}
				System.out.print(grid[i][j]);
			}
			System.out.println("|");
		}
		System.out.println("+---+---+---+");
	}

	public boolean rozwiaz(){
		for(int r = 0; r < SIZE; r++){
			for(int c = 0; c < SIZE; c++){
				if(grid[r][c] == 0){
					for(int cyfra = 1; cyfra <= SIZE; cyfra++){
						if(sprawdz(r, c, cyfra)){
							grid[r][c] = cyfra;
							if(rozwiaz()){
								return true;
							}
							grid[r][c] = 0;
						}
					}
					return false;
				}
			}
		}
		return true;
	}

	private boolean sprawdz(int r, int c, int cyfra){
		for(int i = 0; i < SIZE; i++){
			if(grid[r][i] == cyfra || grid[i][c] == cyfra){
				return false;
			}
		}

		int offR = r - r % 3;
		int offC = c - c % 3;
		for(int i = 0; i < 3; i++){
			for(int j = 0; j < 3; j++){
				if(grid[i + offR][j + offC] == cyfra){
					return false;
				}
			}
		}

		return true;
	}
}
