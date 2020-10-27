#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <unistd.h>
#include <pthread.h>

#define DIM 30


#define GRASS_PAIR     1
#define EMPTY_PAIR     1
#define WATER_PAIR     2
#define MOUNTAIN_PAIR  3
#define PLAYER_PAIR    4

WINDOW** winArr;
int**aArr; // 'normal' int array
int algNum;

void wprintArrCurses(WINDOW*,int [], int, char*);

void wselectionSort(WINDOW* win, int a[]) {
  int max = DIM-1;
  int ins = 0;
  do {
    wprintArrCurses(win, a, DIM, "Sortieren...");
    mvwaddch(win, DIM-a[ins]+1, ins+1, '#');
    wrefresh(win);
    napms(1000);
    int min = ins;
    for(int i = ins+1; i <= max; i++) {
      if(a[i] < a[min]) {
        min = i;
      }
      wprintArrCurses(win, a, DIM, "Sortieren...");
      mvwaddch(win, DIM-a[ins]+1, ins+1, '#');
      mvwaddch(win, DIM-a[i]+1, i+1, '?');
      if(min != ins) mvwaddch(win, DIM-a[min]+1, min+1, 'o');
      wrefresh(win);
      napms(500);
    }
    if(min != ins) {
      wprintArrCurses(win, a, DIM, "Sortieren...");
      mvwaddch(win, DIM-a[ins]+1, ins+1, '>');
      mvwaddch(win, DIM-a[min]+1, min+1, '<');
      wrefresh(win);
      napms(1000);
      int t = a[min]; a[min] = a[ins]; a[ins] = t;
      wprintArrCurses(win, a, DIM, "Sortieren...");
      mvwaddch(win, DIM-a[ins]+1, ins+1, '<');
      mvwaddch(win, DIM-a[min]+1, min+1, '>');
      wrefresh(win);
      napms(1000);
    }
    ins++;
  } while(ins < max);
}

int wpartion(WINDOW* win, int a[], int l, int r) {
  int i = l;
  int j = r;
  int v = a[r];
  wprintArrCurses(win, a, DIM, "Sortieren...");
  mvwaddch(win, DIM-a[r]+1, r+1, '#');
  wrefresh(win);
  napms(1000);
  while(i<j) {
    while(i<r && a[i] < v) {
      wprintArrCurses(win, a, DIM, "Sortieren...");
      mvwaddch(win, DIM-a[r]+1, r+1, '#');
      mvwaddch(win, DIM-a[i]+1, i+1, '?');
      wrefresh(win);
      napms(500);
      i++;
    }
    wprintArrCurses(win, a, DIM, "Sortieren...");
    mvwaddch(win, DIM-a[r]+1, r+1, '#');
    mvwaddch(win, DIM-a[i]+1, i+1, 'o');
    wrefresh(win);
    napms(500);
    while(j>l && a[j] >= v) {
      wprintArrCurses(win, a, DIM, "Sortieren...");
      mvwaddch(win, DIM-a[r]+1, r+1, '#');
      mvwaddch(win, DIM-a[i]+1, i+1, 'o');
      mvwaddch(win, DIM-a[j]+1, j+1, '?');
      wrefresh(win);
      napms(500);
      j--;
    }
    wprintArrCurses(win, a, DIM, "Sortieren...");
    mvwaddch(win, DIM-a[r]+1, r+1, '#');
    mvwaddch(win, DIM-a[i]+1, i+1, 'o');
    mvwaddch(win, DIM-a[j]+1, j+1, 'o');
    wrefresh(win);
    napms(500);
    if(i<j) {
      wprintArrCurses(win, a, DIM, "Sortieren...");
      mvwaddch(win, DIM-a[r]+1, r+1, '#');
      mvwaddch(win, DIM-a[i]+1, i+1, '>');
      mvwaddch(win, DIM-a[j]+1, j+1, '<');
      wrefresh(win);
      napms(1000);
      int t = a[i]; a[i] = a[j]; a[j] = t;
      wprintArrCurses(win, a, DIM, "Sortieren...");
      mvwaddch(win, DIM-a[r]+1, r+1, '#');
      mvwaddch(win, DIM-a[i]+1, i+1, '<');
      mvwaddch(win, DIM-a[j]+1, j+1, '>');
      wrefresh(win);
      napms(1000);
    }
  }
  if(a[i] > a[r]){
    wprintArrCurses(win, a, DIM, "Sortieren...");
    mvwaddch(win, DIM-a[i]+1, i+1, '>');
    mvwaddch(win, DIM-a[r]+1, r+1, '<');
    wrefresh(win);
    napms(1000);
    int t = a[i]; a[i] = a[r]; a[r] = t;
    wprintArrCurses(win, a, DIM, "Sortieren...");
    mvwaddch(win, DIM-a[i]+1, i+1, '<');
    mvwaddch(win, DIM-a[r]+1, r+1, '>');
    wrefresh(win);
    napms(1000);
  }
  return i;
}

void wquicksort(WINDOW* win, int a[], int l, int r) {
  int i;
  if(l<r) {
    i = wpartion(win, a, l, r);
    wquicksort(win, a, l, i-1);
    wquicksort(win, a, i+1, r);
  }
}

void printArr(int a[], int n) {
  for(int i = 0; i<n; i++) {
    printf("%d ", a[i]);
  }
  printf("\n");
}

void wprintArrCurses(WINDOW * win, int a[], int n, char* msg) {
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
  mvwaddstr(win,DIM+1, 0, msg);
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

WINDOW* initWin() {
  WINDOW* win = newwin(DIM+2, DIM+2, 0, 0);
  box(win,ACS_VLINE,ACS_HLINE);
  refresh();
  wrefresh(win);
  return win;
}

void startQs(WINDOW* winQs, int a[]) {
  wprintArrCurses(winQs, a, DIM, "Zum Sortieren TASTE drücken");
  refresh();
  wrefresh(winQs);
  wmove(winQs, 0,0);
  //getch();
  wquicksort(winQs, a, 0, DIM-1);
  wprintArrCurses(winQs, a, DIM, "Fertig sortiert!");
  wrefresh(winQs);
  getch();
}

void startSelSort(WINDOW* winSelSort, int a[]) {
  wprintArrCurses(winSelSort, a, DIM, "Zum Sortieren TASTE drücken");
  refresh();
  wrefresh(winSelSort);
  wmove(winSelSort, 0,0);
  //getch();
  wselectionSort(winSelSort, a);
  wprintArrCurses(winSelSort, a, DIM, "Fertig sortiert!");
  wrefresh(winSelSort);
  getch();
}

void initArr(int a[]) {
  for(int i = 1; i<= DIM; i++) {
    a[i-1] = i; 
  }
  shuffle(a, DIM);
}

void formatWindows(WINDOW** win, int n) {
  for(int i = 1; i <= n; i++) {
    clear();
    mvwin(win[i-1], LINES/2-((DIM+2)/2), i*(COLS/(n+1))-DIM/2);
    refresh();
  }
}

void setAlgNum(int i) {
  algNum = i;
}

int getAlgNum() {
  return algNum;
}

void* buffering(void* arg) {
  while(1) {
    for(int i = 0; i<algNum; i++) {
      wprintArrCurses(winArr[i], aArr[i], DIM, "Zum Sortieren TASTE drücken");
      shuffle(aArr[i], DIM);
    }
    refresh();
    napms(600);
  }
}

int main(void) {
  setAlgNum(2);
  aArr = malloc(sizeof(int*)*algNum);
  winArr = malloc(sizeof(WINDOW*)*algNum);

  pthread_t thread2;
  omp_set_num_threads(algNum);
  initscr ();
  noecho();
  curs_set(0);

  for(int i = 0; i<algNum; i++) {
  aArr[i] = malloc(sizeof(int)*DIM);
  initArr(aArr[i]);
  }

  for(int i = 0; i < algNum; i++) {
    winArr[i] = initWin();
  }


  formatWindows(winArr, algNum);


  pthread_create(&thread2, NULL, &buffering, (void*) NULL);

  getch();
  pthread_cancel(thread2);
#pragma omp parallel for 
  for(int i = 0; i<algNum; i++)
  {
    startQs(winArr[i], aArr[i]);
  }
  endwin();
}
