all: 2048v1 2048v2
2048v2: 2048.c
	gcc 2048.c -o 2048v2.exe
2048v1: 2048v1.c
	gcc 2048v1.c -o 2048v1.exe