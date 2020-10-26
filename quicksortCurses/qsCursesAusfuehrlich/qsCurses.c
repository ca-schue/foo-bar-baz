#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DIM 30


#define GRASS_PAIR     1
#define EMPTY_PAIR     1
#define WATER_PAIR     2
#define MOUNTAIN_PAIR  3
#define PLAYER_PAIR    4

WINDOW * win;

void wprintArrCurses(WINDOW*,int [], int);

int partion(int a[], int l, int r) {
  int i = l;
  int j = r;
  int v = a[r];
  wprintArrCurses(win, a, DIM);
  mvwaddstr(win, DIM+1, 0, "Pivotelement P gesetzt");
  wrefresh(win);
  napms(2000);
  while(i<j) {
    while(i<r && a[i] < v) {
      wprintArrCurses(win, a, DIM);
      mvwaddstr(win, DIM+1, 0, "I sucht Punkt über P");
      mvwaddch(win, DIM-v+1, r+1, 'P');
      mvwaddch(win, DIM-a[i]+1, i+1, 'I');
      wrefresh(win);
      napms(600);
      i++;
    }
    wprintArrCurses(win, a, DIM);
    mvwaddch(win, DIM-v+1, r+1, 'P');
    mvwaddch(win, DIM-a[i]+1, i+1, 'I');
    mvwaddstr(win, DIM+1, 0, "I hat Punkt über P gefunden");
    wrefresh(win);
    napms(2000);
    while(j>l && a[j] >= v) {
      wprintArrCurses(win, a, DIM);
      mvwaddstr(win, DIM+1, 0, "J sucht Punkt unter P");
      mvwaddch(win, DIM-v+1, r+1, 'P');
      mvwaddch(win, DIM-a[i]+1, i+1, 'I');
      mvwaddch(win, DIM-a[j]+1, j+1, 'J');
      wrefresh(win);
      napms(600);
      j--;
    }
    wprintArrCurses(win, a, DIM);
    mvwaddch(win, DIM-v+1, r+1, 'P');
    mvwaddch(win, DIM-a[i]+1, i+1, 'I');
    mvwaddch(win, DIM-a[j]+1, j+1, 'J');
    mvwaddstr(win, DIM+1, 0, "J hat Punkt unter P gefunden");
    wrefresh(win);
    napms(2000);
    if(i<j) {
      wprintArrCurses(win, a, DIM);
      mvwaddch(win, DIM-v+1, r+1, 'P');
      mvwaddch(win, DIM-a[i]+1, i+1, 'I');
      mvwaddch(win, DIM-a[j]+1, j+1, 'J');
      mvwaddstr(win, DIM+1, 0, "I und J Tauschen");
      wrefresh(win);
      napms(1000);
      mvwaddch(win, DIM-a[i]+1, i+1, '>');
      mvwaddch(win, DIM-a[j]+1, j+1, '<');
      wrefresh(win);
      napms(1000);
      int t = a[i]; a[i] = a[j]; a[j] = t;
      wprintArrCurses(win, a, DIM);
      mvwaddch(win, DIM-v+1, r+1, 'P');
      mvwaddch(win, DIM-a[j]+1, j+1, 'I');
      mvwaddch(win, DIM-a[i]+1, i+1, 'J');
      wrefresh(win);
      mvwaddstr(win, DIM+1, 0, "Fertig getauscht!");
      wrefresh(win);
      napms(600);
    }
  }
  if(a[i] > a[r]){
    wprintArrCurses(win, a, DIM);
      mvwaddch(win, DIM-v+1, r+1, 'P');
      mvwaddch(win, DIM-a[j]+1, j+1, 'I');
      mvwaddch(win, DIM-a[i]+1, i+1, 'J');
      wrefresh(win);
    mvwaddstr(win, DIM+1, 0, "Pivotelement tauschen");
    wrefresh(win);
    napms(1000);
    mvwaddch(win, DIM-v+1, r+1, '<');
     mvwaddch(win, DIM-a[j]+1, j+1, '>');
     wrefresh(win);
     napms(1000);

    int t = a[i]; a[i] = a[r]; a[r] = t;
    wprintArrCurses(win, a, DIM);
    mvwaddch(win, DIM-a[j]+1, j+1, 'I');
    mvwaddch(win, DIM-a[r]+1, r+1, 'J');
    mvwaddch(win, DIM-a[i]+1, i+1, 'P');
    mvwaddstr(win, DIM+1, 0, "Fertig getauscht!");
    wrefresh(win);
    napms(600);
  }
  return i;
}

void quicksort(int a[], int l, int r) {
  int i;
  if(l<r) {
    i = partion(a, l, r);
    quicksort(a, l, i-1);
    wprintArrCurses(win, a, DIM);
    quicksort(a, i+1, r);
    wprintArrCurses(win, a, DIM);
  }
}

void printArr(int a[], int n) {
  for(int i = 0; i<n; i++) {
    printf("%d ", a[i]);
  }
  printf("\n");
}

void wprintArrCurses(WINDOW * win, int a[], int n) {
  wclear(win);
  int j, i;
  for(j = n; j > 0; j--) {
    for(i = 0; i < n; i++) {
      if(a[i] == j) {
        mvwaddch(win, n-j+1, i+1,'.');
      } else {
        mvwaddch(win, n-j+1, i+1,' ');
      }
    }
    mvwaddch(win, n-j+1, i+1, '\n');
  }
  box(win,ACS_VLINE,ACS_HLINE);
  mvwaddstr(win,DIM+1, 0,"Sortieren...");
  wrefresh(win);
}

void shuffle(int arr[], size_t n)
{
  if (n > 1) 
  {
    size_t i;
    srand(time(NULL));
    for (i = 0; i < n - 1; i++) 
    {
      size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
      int t = arr[j];
      arr[j] = arr[i];
      arr[i] = t;
    }
  }
}

int main(void) {
  initscr ();
  noecho();
  curs_set(0);
  win = newwin(DIM+2, DIM+2, 0, 0);
  box(win,ACS_VLINE,ACS_HLINE);
  refresh();
  mvwin(win, LINES/2-((DIM+2)/2), COLS/2-(DIM/2));
  wrefresh(win);
  int a[DIM];
  for(int i = 1; i<= DIM; i++) {
    a[i-1] = i; 
  }
  shuffle(a, DIM);  
  wprintArrCurses(win, a, DIM);
  mvwaddstr(win, DIM+1, 0, "Zum Sortieren TASTE drücken");
  refresh();
  wrefresh(win);
  wmove(win, 0,0);
  getch();
  quicksort(a, 0, DIM-1);
  wprintArrCurses(win, a, DIM);
  mvwaddstr(win, DIM+1, 0, "Fertig sortiert!");
  wrefresh(win);
  getch();
  endwin();
}
