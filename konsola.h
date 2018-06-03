#include <windows.h>
#include <string.h>

#ifndef _KONSOLA_H
#define _KONSOLA_H
/* typedef struct _CONSOLE_FONT_INFOEX
{
    ULONG cbSize;
    DWORD nFont;
    COORD dwFontSize;
    UINT FontFamily;
    UINT FontWeight;
    WCHAR FaceName[ LF_FACESIZE ];
} CONSOLE_FONT_INFOEX, * PCONSOLE_FONT_INFOEX; */

/* BOOL WINAPI GetCurrentConsoleFontEx(
  HANDLE hConsoleOutput,
  BOOL bMaximumWindow,
  PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx
); */

/* BOOL WINAPI SetCurrentConsoleFontEx(
  HANDLE hConsoleOutput,
  BOOL bMaximumWindow,
  PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx
); */

void gotoxy(int x, int y)
{
  COORD c;

  c.X = x;
  c.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void rozmiarokna(int x, int y)
{
	SMALL_RECT windowSize = {0, 0, x, y};
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &windowSize);
}

void buforxy(int x, int y)
{
	COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void clrscr()
{
  DWORD written;

  FillConsoleOutputAttribute(GetStdHandle (STD_OUTPUT_HANDLE),
    8 + (0 << 4), 2000, (COORD){0, 0}, &written);
  FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ',
    2000, (COORD){0, 0}, &written);
  gotoxy(1, 1);
}

void CursorOff()
{
  CONSOLE_CURSOR_INFO Info;

  GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info); 
  Info.bVisible = 0;
  SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info); 
}

void textattr(int text, int tlo)
{
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
    text + (tlo << 4));
}

void czcionkaRastrowa(void)
{
	CONSOLE_FONT_INFOEX info;
	info.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &info);
	wcscpy(info.FaceName, L"Terminal");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &info);
}

void czcionkaxy(int x, int y)
{
	CONSOLE_FONT_INFOEX info;
	info.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &info);
	info.dwFontSize.X = x;
	info.dwFontSize.Y = y;
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &info);
}

#endif