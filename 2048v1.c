#include <stdio.h>		//
#include <conio.h>		// getch(), kbhit()
#include <time.h>		// time()
#include <stdbool.h>	// bool, true, false
#include <stdlib.h>		// rand(), srand()
#include <windows.h>	// COORD, SetConsoleCursorPosition(), GetStdHandle(), STD_OUTPUT_HANDLE
#define ROZMIAR 4
int CzytajWejscie(void);
void WyswietlPlansze(char tab[][4]);
void AktualizujPlansze(char tab[][4]);
void Losuj(char tab[][4]);
bool CzyMozliwyRuch(char tab[][4]);
bool Gora(char tab[][4]);
bool Dol(char tab[][4]);
bool Prawo(char tab[][4]);
bool Lewo(char tab[][4]);

const char *kafelki[] = {"  -  ", "  2  ", "  4  ", "  8  ", "  16 ", "  32 ", "  64 ",
						 " 128 ", " 256 ", " 512 ", "1024 ", "2048 ", "4096 ", "8192 ", "16384"};

void gotoxy(int x, int y)
{
  COORD c;

  c.X = x;
  c.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void CursorOff()
{
  CONSOLE_CURSOR_INFO Info;

  GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info); 
  Info.bVisible = 0;
  SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info); 
}

int main(void)
{
	char plansza[ROZMIAR][ROZMIAR] = {};
	int i;
	srand(time(0));
	CursorOff();
	
	WyswietlPlansze(plansza);
	Losuj(plansza);
	Losuj(plansza);
	AktualizujPlansze(plansza);
	
	bool koniec = false, CzyBylRuch = true;
	while (!koniec && CzyMozliwyRuch(plansza))
	{
		switch (CzytajWejscie())
		{
			case 0:
				CzyBylRuch = Gora(plansza);
				break;
			case 1:
				CzyBylRuch = Prawo(plansza);
				break;
			case 2:
				CzyBylRuch = Dol(plansza);
				break;
			case 3:
				CzyBylRuch = Lewo(plansza);
				break;
			case 4:
				koniec = true;
				break;
		}
		AktualizujPlansze(plansza);
		if (CzyBylRuch)
			Losuj(plansza);
	}
	
	return 0;
}

// Zwraca 0 jesli wcisnieto strzalke do gory, 1 - strzalke w prawo, 2 - strzalke w dol,
// 3 - strzalke w lewo, 4 - klawisz esc
int CzytajWejscie(void)
{
	int ch;
	
	while (1)
	{
		if ((ch = getch()) == 224 && kbhit())
			switch (ch = getch())
			{
				case 72: return 0;	//strzalka do gory
				case 77: return 1;	//strzalka w prawo
				case 80: return 2;	//srzalka w dol
				case 75: return 3;	//strzalka w lewo
			}
		if (ch == 27)	// ESC
			return 4;
	}
}

void WyswietlPlansze(char tab[][4])
{
	int row;
	
	for (row = 1; row < 18; row++, putchar('\n'))
	{
		if (row == 1)
			printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
				201, 205, 205, 205, 205, 205, 203, 205, 205, 205, 205, 205,
				203, 205, 205, 205, 205, 205, 203, 205, 205, 205, 205, 205, 187);
		else if (row == 17)
			printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
				200, 205, 205, 205, 205, 205, 202, 205, 205, 205, 205, 205,
				202, 205, 205, 205, 205, 205, 202, 205, 205, 205, 205, 205, 188);
		else if (row == 5 || row == 9 || row == 13)
			printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
				204, 205, 205, 205, 205, 205, 206, 205, 205, 205, 205, 205,
				206, 205, 205, 205, 205, 205, 206, 205, 205, 205, 205, 205, 185);
		else
			printf("%c     %c     %c     %c     %c", 186, 186, 186, 186, 186);
	}
}

void Losuj(char tab[][4])
{
	char temp_tab[16];
	int ile, i;
	
	for (i = ile = 0; i < 16; i++)
		if (tab[0][i] == 0)
			temp_tab[ile++] = i;
	// nie ma wolnego miejsca
	if (ile == 0)
		return;
	
	int los = rand() % ile;
	tab[0][temp_tab[los]] = (rand() % 10 > 0)? 1: 2;
	
	Sleep(65);
	gotoxy((temp_tab[los] % 4)*6 + 1, (temp_tab[los]/4)*4 + 2);
	printf("%s", kafelki[tab[0][temp_tab[los]]]);
}

bool CzyMozliwyRuch(char tab[][4])
{
	int i, j;
	
	for (i = 0; i < ROZMIAR; i++)
	{
		for (j = 0; j < ROZMIAR; j++)
			if (tab[i][j] == 0)
				return true;
		for (j = 1; j < ROZMIAR; j++)
			if (tab[i][j-1] == tab[i][j] || tab[j-1][i] == tab[j][i])
				return true;
	}
	return false;
}

void AktualizujPlansze(char tab[][4])
{
	int i, j;
	for (i = 0; i < ROZMIAR; i++)
	{
		for (j = 0; j < ROZMIAR; j++)
		{
			gotoxy(j*6 + 1, i*4 + 2);
			printf("%s", kafelki[tab[i][j]]);
		}
	}
}

bool Gora(char tab[][4])
{
	int i, next, col;
	bool zmiana = false;
	
	for (col = 0; col < ROZMIAR; col++)
	{
		i = 0;
		next = i+1;
		while (i < ROZMIAR - 1)
		{
			while (tab[next][col] == 0 && next < ROZMIAR)
				next++;
			if (next == ROZMIAR)
				break;
			if (tab[i][col] == 0)
			{
				tab[i][col] = tab[next][col];
				tab[next][col] = 0;
				zmiana = true;
			}
			else if (tab[i][col] == tab[next][col])
			{
				tab[i++][col]++;
				tab[next][col] = 0;
				zmiana = true;
			}
			else
				i++;
		}
	}
	return zmiana;
	/*int col, row, i;
	
	for (col = 0; col < ROZMIAR; col++)
	{
		for (row = 1, i = 0; row < ROZMIAR; row++)
			if (tab[i][col] == 0)
			{	
				if (tab[row][col] == 0)
					continue;
				tab[i++][col] = tab[row][col];
				tab[row][col] = 0;
			}
			else
				i++;
		
		for (row = 0; row < ROZMIAR - 1 && tab[row + 1][col] != 0; row++)
			if (tab[row][col] == tab[row + 1][col])
			{	
				tab[row++][col]++;
				tab[row][col] = 0;
			}
		
		for (row = 1, i = 0; row < ROZMIAR; row++)
			if (tab[i][col] == 0)
			{	
				if (tab[row][col] == 0)
					continue;
				tab[i++][col] = tab[row][col];
				tab[row][col] = 0;
			}
			else
				i++;
	}*/
}

bool Dol(char tab[][4])
{
	int col, row, i;
	bool zmiana = false;
	
	for (col = 0; col < ROZMIAR; col++)
	{
		for (row = ROZMIAR - 2, i = ROZMIAR - 1; row >= 0; row--)
			if (tab[i][col] == 0)
			{	
				if (tab[row][col] == 0)
					continue;
				tab[i--][col] = tab[row][col];
				tab[row][col] = 0;
				zmiana = true;
			}
			else
				i--;
		
		for (row = ROZMIAR - 1; row > 0 && tab[row - 1][col] != 0; row--)
			if (tab[row][col] == tab[row - 1][col])
			{	
				tab[row--][col]++;
				tab[row][col] = 0;
				zmiana = true;
			}
		
		for (row = ROZMIAR - 2, i = ROZMIAR - 1; row >= 0; row--)
			if (tab[i][col] == 0)
			{	
				if (tab[row][col] == 0)
					continue;
				tab[i--][col] = tab[row][col];
				tab[row][col] = 0;
			}
			else
				i--;
	}
	return zmiana;
}

bool Prawo(char tab[][4])
{
	int col, row, i;
	bool zmiana = false;
	
	for (row = 0; row < ROZMIAR; row++)
	{
		for (col = ROZMIAR - 2, i = ROZMIAR - 1; col >= 0; col--)
			if (tab[row][i] == 0)
			{	
				if (tab[row][col] == 0)
					continue;
				tab[row][i--] = tab[row][col];
				tab[row][col] = 0;
				zmiana = true;
			}
			else
				i--;
		
		for (col = ROZMIAR - 1; col > 0 && tab[row][col - 1] != 0; col--)
			if (tab[row][col] == tab[row][col - 1])
			{	
				tab[row][col--]++;
				tab[row][col] = 0;
				zmiana = true;
			}
		
		for (col = ROZMIAR - 2, i = ROZMIAR - 1; col >= 0; col--)
			if (tab[row][i] == 0)
			{	
				if (tab[row][col] == 0)
					continue;
				tab[row][i--] = tab[row][col];
				tab[row][col] = 0;
			}
			else
				i--;
	}
	return zmiana;
}

bool Lewo(char tab[][4])
{
	int col, row, i;
	bool zmiana = false;
	
	for (row = 0; row < ROZMIAR; row++)
	{
		for (col = 1, i = 0; col < ROZMIAR; col++)
			if (tab[row][i] == 0)
			{	
				if (tab[row][col] == 0)
					continue;
				tab[row][i++] = tab[row][col];
				tab[row][col] = 0;
				zmiana = true;
			}
			else
				i++;
		
		for (col = 0; col < ROZMIAR - 1 && tab[row][col + 1] != 0; col++)
			if (tab[row][col] == tab[row][col + 1])
			{	
				tab[row][col++]++;
				tab[row][col] = 0;
				zmiana = true;
			}
		
		for (col = 1, i = 0; col < ROZMIAR; col++)
			if (tab[row][i] == 0)
			{	
				if (tab[row][col] == 0)
					continue;
				tab[row][i++] = tab[row][col];
				tab[row][col] = 0;
			}
			else
				i++;
	}
	return zmiana;
}