#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <conio.h>
#include "konsola.h"

/* CZARNY 		0 */
/* NIEBIESKI 		1 */
/* ZIELONY 		2 */
/* BLEKITNY 		3 */
/* CZERWONY		4 */
/* PURPUROWY		5 */
/* ZOLTY			6 */
/* BIALY			7 */
/* SZARY			8 */
/* JASNONIEBIESKI	9 */
/* JASNOZIELONY	10 */
/* JASNOBLEKITNY	11 */
/* JASNOCZERWONY	12 */
/* JASNOPURPUROWY	13 */
/* JASNOZOLTY		14 */
/* JASKRAWOBIALY	15 */

struct wlasciwosci {
	char *wsk;
	char Dana;
	char Kolejna;
};
const char *kafelki[] = {"     ", "  2  ", "  4  ", "  8  ", "  16 ", "  32 ", "  64 ",
						 " 128 ", " 256 ", " 512 ", "1024 ", "2048 ", "4096 ", "8192 ", "16384"};
const char kolor_tlo[] = {8, 15, 7, 14, 6, 12, 4, 13, 5, 11, 3, 10, 2, 9, 1};
const char kolor_text[] = {15, 0, 0, 0, 0, 15, 15, 15, 15, 0, 15, 0, 15, 15, 15};
unsigned int Score = 0;

int CzytajWejscie(void);
void LikwidujZera(char *wsk, char odstep);
void Scalaj(char *wsk, char odstep);
bool CzyMozliwyRuch(char tab[][4]);
bool CzyBylRuch(char przed[][4], char po[][4]);
void Losuj(char tab[][4]);
void WykonajRuch(const struct wlasciwosci *linia);
void WyswietlPlansze(char tab[][4]);
bool KoniecGry(void);
unsigned int Potega(char p) { unsigned long long x = 1; while (p--) x *= 2; return x;}

int main(void)
{
	char plansza[4][4] = {{0, 0, 0, 0},
						  {0, 0, 0, 0},
						  {0, 0, 0, 0},
						  {0, 0, 0, 0}};
	char cofnij_ruch[3][4][4];
	char kopia_planszy[4][4];
	char kierunek, licznik;
	const struct wlasciwosci kierunki[4] = {{&(plansza[0][0]), 4, 1},	// klawisz w gore
											{&(plansza[0][3]), -1, 4},	// klawisz w prawo
											{&(plansza[3][0]), -4, 1},	// klawisz w dol
											{&(plansza[0][0]), 1, 4}};	// klawisz w lewo
	
	/*gotoxy(16, 8);
	printf("GRA STWORZONA PRZEZ: Micha%ca S%cowika, kl. 3 a", 136, 136);
	gotoxy(16, 9);
	printf("Oparta na grze stworzonej przez: Gabriele Cirulli");
	gotoxy(16, 11);
	printf("Nacisnij enter by zagrac...");
	gotoxy(42, 24);
	printf("Feedback: chusta.additional@gmail.com");
	while (getchar() != '\n')
		continue;
	
	clrscr();*/
	czcionkaRastrowa();
	rozmiarokna(34, 52);
	buforxy(35, 53);
	czcionkaxy(8, 8);
	CursorOff();
	srand((unsigned int) time(NULL));
	WyswietlPlansze(plansza);
	gotoxy(2, 18);
	textattr(10, 0);
	printf("SCORE: %u", Score);
	Losuj(plansza);
	Losuj(plansza);
	
	bool graj = true;
	while (graj)
	{
		textattr(8, 0);
		if (!CzyMozliwyRuch(plansza))
		{	
			graj = KoniecGry();
			continue;
		}
		if ((kierunek = CzytajWejscie()) == 4)
			break;
		for (licznik = 0; licznik < 16; licznik++)
			kopia_planszy[0][licznik] = plansza[0][licznik];
		WykonajRuch(&kierunki[kierunek]);
		textattr(10, 0);
		gotoxy(2, 18);
		printf("SCORE: %u", Score);
		WyswietlPlansze(plansza);
		//Sleep(10);
		if (CzyBylRuch(plansza, kopia_planszy))
			Losuj(plansza);
	}
	
	getchar();
	return 0;
}

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
		if (ch == '1')
			return 5;
	}
}

void LikwidujZera(char *wsk, char odstep)
{
	char i, *korzen;
	
	for (i = 0; i < 3; i++)
		if (*wsk != 0)
			wsk += odstep;
		else
			break;
	
	if (i == 3)	//jesli i == 3 to 3 element tablicy nie jest rowny zero czyli tablica jest 'przesunieta'
		return;
	
	for (korzen = wsk + odstep; i++ < 3; korzen += odstep)
		if (*korzen != 0)
		{	
			*wsk = *korzen;
			*korzen = 0;
			wsk += odstep;
		}
}

void Scalaj(char *wsk, char odstep)
{
	char ch, *korzen = wsk + odstep;
	
	for (ch = 0; ch < 3; ch++, wsk = korzen, korzen += odstep)
		if (*wsk != 0 && *korzen == *wsk)
		{
			Score += Potega(*(wsk) += 1);
			*korzen = 0;
		}
}

bool CzyMozliwyRuch(char tab[][4])
{
	int i, j;
	
	for (i = 0; i < 4; i++)
	{	
		for (j = 0; j < 4; j++)	// sprawdza czy na planszy sa jakies zera
			if (tab[i][j] == 0)	//
				return true;
		for (j = 0; j < 3; j++)		// jesli nie ma zer to sprawdza czy mozna polaczyc liczby ktore
			if (tab[i][j] == tab[i][j+1] || tab[j][i] == tab[j+1][i])	// sa na planszy
				return true;
	}
	return false;
}

void Losuj(char tab[][4])
{
	char temp_tab[14], i, j;
	char los;
	
	for (j = i = 0; i < 16; i++)
		if (tab[0][i] == 0)
			temp_tab[j++] = i;
	if (i == 0)
		return;
	los = rand() % j;
	i = tab[0][temp_tab[los]] = (rand() % 10 > 0)? 1: 2;
	
	int x, y;
	x = (temp_tab[los] % 4)*8 + 5;
	y = (temp_tab[los] / 4)*8 + 23;
	gotoxy(x, y);
	if (i == 1)
	{
		textattr(0, 15);
		putchar('2');
	}
	else
	{
		textattr(0, 7);
		putchar('4');
	}
	
	
	Sleep(20);
	for (i = 1; i <= 3; i++)
	{
		gotoxy(x - i, y - i);
		for (j = 0; j < 2*i + 1; j++)
			putchar(' ');
		gotoxy(x - i, y + i);
		for (j = 0; j < 2*i + 1; j++)
			putchar(' ');
		for (j = 0; j < 2*i - 1; j++)
		{
			gotoxy(x - i, y - i + j + 1);
			putchar(' ');
		}
		for (j = 0; j < 2*i - 1; j++)
		{
			gotoxy(x + i, y - i + j + 1);
			putchar(' ');
		}
		Sleep(20);
	}
}

void WykonajRuch(const struct wlasciwosci *linia)
{
	char licznik;
	
	for (licznik = 0; licznik < 4; licznik++)
	{
		LikwidujZera(linia->wsk + (licznik * linia->Kolejna), linia->Dana);
		Scalaj(linia->wsk + (licznik * linia->Kolejna), linia->Dana);
		LikwidujZera(linia->wsk + (licznik * linia->Kolejna), linia->Dana);
	}
}

void WyswietlPlansze(char tab[][4])
{
	int i, j, licz;
	
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
		{
			textattr(kolor_text[tab[i][j]], kolor_tlo[tab[i][j]]);
			for (licz = 0; licz < 7; licz++)
			{
				gotoxy(8*j + 2, 8*i + licz + 1 + 19);
				if (licz == 3)
					printf(" %s ", kafelki[tab[i][j]]);
				else
					printf("       ");
			}
		}
	textattr(8, 0);	
}

bool CzyBylRuch(char przed[][4], char po[][4])
{
	char i, j;
	
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			if (przed[i][j] != po[i][j])
				return true;
	return false;
}

bool KoniecGry(void)
{
	int x, y, j;
	
	textattr(12, 0);
	/*for (x = 0; x < 5; x++)
	{	
		gotoxy(1, 8*x + 19);
		for (y = 0; y < 4*7 + 5; y++)
			putchar('#');
		for (y = 0; y < 4*7 + 5; y++)
		{
			gotoxy(8*x + 1, y + 19);
			putchar('#');
		}
	}*/
	gotoxy(0, 0);
	puts("    GGGGGG    A   MM  MM EEEEE");
	puts("    G        A A  M MM M E    ");
	puts("    G  GGG  A   A M MM M EEEE ");
	puts("    G    G  AAAAA M    M E    ");
	puts("    GGGGGG  A   A M    M EEEEE\n");
	puts("    OOOOOO  V   V EEEEE  RRRRR");
	puts("    O    O  V   V E      R   R");
	puts("    O    O  V   V EEEE   RRRRR");
	puts("    O    O   V V  E      R  R ");
	puts("    OOOOOO    V   EEEEE  R   R");
	
	textattr(9, 0);
	gotoxy(5, 12);
	puts("Chcesz grac jeszcze raz?");
	gotoxy(15, 15);
	puts("NIE");
	gotoxy(14, 14);
	textattr(10, 0);
	putchar('[');
	textattr(9, 0);
	printf("TAK");
	textattr(10, 0);
	putchar(']');
	
	//return true;
	return false;
}