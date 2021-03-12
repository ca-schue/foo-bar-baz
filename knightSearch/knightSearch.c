#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INIT_MIN 999999999

int steps[][8] = {
//00  01  02  03  04  05  06  07
		{ 1, 2, 2, 1, -1, -2, -2, -1 },
		// 		10  11  12  13	14	15	16	17
		{ 2, 1, -1, -2, -2, -1, 1, 2 } };

void boardReset(unsigned int board[][8]) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			board[i][j] = -1;
		}
	}
}
void printBoard(unsigned int board[][8]) {
	int tmp;
	        printf("Spalten >   0  1  2  3  4  5  6  7\n"
	        	   "Zeilen  v\n");
	for (int k = 0; k < 8; k++) {
		printf("%9d ", k);
		for (int i = 0; i < 8; i++) {
			tmp = board[k][i];
			if (tmp == -1) {
				printf("  x");
			} else {
				printf("%3d", tmp);
			}
		}
		printf("\n");
	}
}

unsigned int springerSuche(int Zeile, int Spalte, int zZeile, int zSpalte,
		unsigned int boardPrev[][8], unsigned int min, unsigned int currSteps) {
	unsigned int boardCpSpalte[8][8];
// Neues "Besucht"-Feld für diesen Pfad, sichert für Geschwisterknoten das "Besucher"-Feld des Elternknoten
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			boardCpSpalte[i][j] = boardPrev[i][j];
		}
	}
	boardCpSpalte[Zeile][Spalte] = currSteps;

	if (Zeile == zZeile && Spalte == zSpalte) {
		printf("\n           Am Ziel mit %3d Zügen:\n"
				 "           ---------------------\n", currSteps);
		printBoard(boardCpSpalte);
		return currSteps;			// schon am Ziel?
	} else {
		if (currSteps > min) {// bereits länger als aktuelles Minimum unterwegs?
			return min;
		} else {
			for (int i = 0; i < 8; i++) {
				int ZeileNew = Zeile + steps[0][i];
				int SpalteNew = Spalte + steps[1][i];
//	BoundarSpalte-check: Zeile						Spalte						  unvisited?
				if (ZeileNew < 8 && ZeileNew >= 0 && SpalteNew < 8 && SpalteNew >= 0
						&& boardCpSpalte[ZeileNew][SpalteNew] == -1) {
					min = springerSuche(ZeileNew, SpalteNew, zZeile, zSpalte, boardCpSpalte, min,
							currSteps + 1);
				}
			}
			return min;
		}
	}
}

int main(void) {
	int Zeile, Spalte, zZeile, zSpalte;
	unsigned int board[8][8];
	boardReset(board);

	do {
	        printf("Startposition des Springers eingeben.\n(Format: Zeile Spalte, jeweils zwischen 0 und 7): ");
	        scanf("%d\t %d", &Zeile, &Spalte);
	        if(Zeile > 7 || Zeile < 0 || Spalte >7 || Spalte < 0) {
	            printf("Feler bei der Eingabe: Bitte zwei Zahlen zwischen 0 und 7 eingeben.\n");
	            continue;
	        }
	        break;
	} while(true);
	do {
	printf("Zielposition des Springers eingeben\n(Format: zZeile zSpalte): ");
            scanf("%d\t %d", &zZeile, &zSpalte);
	        if(zZeile > 7 || zZeile < 0 || zSpalte >7 || zSpalte < 0) {
	            printf("Feler bei der Eingabe: Bitte zwei Zahlen zwischen 0 und 7 eingeben.\n");
	            continue;
	        }
	        break;
	} while(true);
	printf("\n\n           ##### Zielpfade: #####\n");
	springerSuche(Zeile, Spalte, zZeile, zSpalte, board, INIT_MIN, 0);
	printf("\n          ^^^^^ Bester Pfad ^^^^^\n");
}
