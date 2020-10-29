#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>


#define GRASS_PAIR     1
#define EMPTY_PAIR     1
#define WATER_PAIR     2
#define MOUNTAIN_PAIR  3
#define PLAYER_PAIR    4

WINDOW** winArr;
int**aArr; // 'normal' int array
int algNum;
int DIM;

void wprintArrCurses(WINDOW*,int [], int, char*);
void wprintMsgCurses(WINDOW* , char*);
void wresetMsgTitleCurses(WINDOW*);
void wprintTitleCurses(WINDOW*, char*);

void wselectionSort(WINDOW* win, int a[]) {
  int max = DIM-1;
  int pivot = 0;
  int i;
  do {
    mvwaddch(win, DIM-a[pivot]+1, pivot+1, '#');
    wrefresh(win);
    napms(1000);
    int min = pivot;
    int minAlt;
    for(i = pivot+1; i <= DIM-1; i++) {
      if(a[i] < a[min]) {
        minAlt = min;
        min = i;
      }
      mvwaddch(win, DIM-a[i]+1, i+1, '?');
      if(min != pivot) {
        if(minAlt == pivot) {
          mvwaddch(win, DIM-a[minAlt]+1, minAlt+1, '#');
        } else {
          mvwaddch(win, DIM-a[minAlt]+1, minAlt+1, '.');
        }
        mvwaddch(win, DIM-a[min]+1, min+1, 'o');
      }
      wrefresh(win);
      napms(500);
      mvwaddch(win, DIM-a[i]+1, i+1, '.');
    }
    if(min != pivot) {
      mvwaddch(win, DIM-a[pivot]+1, pivot+1, '>');
      mvwaddch(win, DIM-a[min]+1, min+1, '<');
      wrefresh(win);
      napms(1000);
      int t = a[min]; a[min] = a[pivot]; a[pivot] = t;
      mvwaddch(win, DIM-a[pivot]+1, min+1, ' ');
      mvwaddch(win, DIM-a[min]+1, pivot+1, ' ');
      mvwaddch(win, DIM-a[pivot]+1, pivot+1, '<');
      mvwaddch(win, DIM-a[min]+1, min+1, '>');
      wrefresh(win);
      napms(1000);
      mvwaddch(win, DIM-a[min]+1, min+1, '.');
    }
    mvwaddch(win, DIM-a[pivot]+1, pivot+1, '.');
    pivot++;
  } while(pivot < DIM);
}

int wpartion(WINDOW* win, int a[], int l, int r) {
  int i = l;
  int j = r;
  int v = a[r];
  mvwaddch(win, DIM-a[r]+1, r+1, '#');
  wrefresh(win);
  napms(1000);
  while(i<j) {
    while(i<r && a[i] < v) {
      mvwaddch(win, DIM-a[i]+1, i+1, '?');
      wrefresh(win);
      napms(500);
      mvwaddch(win, DIM-a[i]+1, i+1, '.');
      wrefresh(win);
      i++;
    }
    if(a[i] >= v && i != r) {
      mvwaddch(win, DIM-a[i]+1, i+1, 'o');
      wrefresh(win);
      napms(500);
    }
    while(j>l && a[j] >= v) {
      if(j != r) {
        mvwaddch(win, DIM-a[j]+1, j+1, '?');
        wrefresh(win);
        napms(500);
        mvwaddch(win, DIM-a[j]+1, j+1, '.');
        wrefresh(win);
      }
      j--;
    }
    if(a[j] < v && j != r) {
      mvwaddch(win, DIM-a[j]+1, j+1, 'o');
      wrefresh(win);
    }
    napms(500);
    if(i<j) {
      mvwaddch(win, DIM-a[i]+1, i+1, '>');
      mvwaddch(win, DIM-a[j]+1, j+1, '<');
      wrefresh(win);
      napms(1000);
      int t = a[i]; a[i] = a[j]; a[j] = t;
      mvwaddch(win, DIM-a[i]+1, j+1, ' ');
      mvwaddch(win, DIM-a[j]+1, i+1, ' ');
      mvwaddch(win, DIM-a[i]+1, i+1, '<');
      mvwaddch(win, DIM-a[j]+1, j+1, '>');
      wrefresh(win);
      napms(1000);
      mvwaddch(win, DIM-a[j]+1, j+1, '.');
      mvwaddch(win, DIM-a[i]+1, i+1, '.');
    } else {
    mvwaddch(win, DIM-a[j]+1, j+1, '.');
    mvwaddch(win, DIM-a[i]+1, i+1, '.');
    }
    wrefresh(win);
    if(a[i] > a[r]){
      mvwaddch(win, DIM-a[i]+1, i+1, '>');
      mvwaddch(win, DIM-a[r]+1, r+1, '<');
      wrefresh(win);
      napms(1000);
      int t = a[i]; a[i] = a[r]; a[r] = t;
      mvwaddch(win, DIM-a[i]+1, r+1, ' ');
      mvwaddch(win, DIM-a[r]+1, i+1, ' ');
      mvwaddch(win, DIM-a[i]+1, i+1, '<');
      mvwaddch(win, DIM-a[r]+1, r+1, '>');
      wrefresh(win);
      napms(1000);
      mvwaddch(win, DIM-a[r]+1, r+1, '.');
      mvwaddch(win, DIM-a[i]+1, i+1, '.');
    }
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

void wresetMsgCurses(WINDOW* win) {
  for(int i = 1; i< DIM-1; i++) {
    mvwaddch(win, DIM+1, i, ACS_HLINE);
  }
}

void wresetMsgTitleCurses(WINDOW* win) {
  box(win,ACS_VLINE,ACS_HLINE);
}

void wprintTitleCurses(WINDOW* win, char* msg) {
  int p = strlen(msg);
  mvwaddstr(win,0, (DIM+1)/2-(p-1)/2, msg);
  wrefresh(win);
}

void wdelPrompt(WINDOW* win, char* msg) {
  int t = strlen(msg);
  for(int i = 0; i < t; i++) {
    mvwdelch(win, (LINES/2 + DIM/2) + DIM/4 - 2, COLS/2 - t/2);
  }
}

void wprintPrompt(WINDOW* win, char* msg) {
  int t = strlen(msg);
  mvwaddstr(win, (LINES/2 + DIM/2) + DIM/4 - 2, COLS/2 - t/2,msg);
}

void wprintMsgCurses(WINDOW* win, char* msg) {
  wresetMsgCurses(win);
  int p = strlen(msg);
  mvwaddstr(win,DIM+1, (DIM+1)/2-(p-1)/2, msg);
  wrefresh(win);
}

void wprintArrCursesNoMsg(WINDOW * win, int a[], int n) {
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
  wrefresh(win);
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
  wprintMsgCurses(winQs, " Sortieren ... ");
  wquicksort(winQs, a, 0, DIM-1);
  wprintMsgCurses(winQs,  " Fertig sortiert! ");
  wrefresh(winQs);
  getch();
}

void startSelSort(WINDOW* winSelSort, int a[]) {
  wprintMsgCurses(winSelSort, " Sortieren ... ");
  wselectionSort(winSelSort, a);
  wprintMsgCurses(winSelSort, " Fertig Sortiert! ");
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
      shuffle(aArr[i], DIM);
      wprintArrCursesNoMsg(winArr[i], aArr[i], DIM);
      wresetMsgTitleCurses(winArr[i]);
      wprintMsgCurses(winArr[i], " Mischen ... ");
      if(i == 0) {
        wprintTitleCurses(winArr[i], " Quicksort ");
      } else {
        wprintTitleCurses(winArr[i], " Selection Sort ");
      }
      wrefresh(winArr[i]);
    }
    wprintPrompt(stdscr, "Zum Sortieren TASTE drücken!");
    refresh();
    napms(700);
  
    wdelPrompt(stdscr, "Zum Sortieren TASTE drücken!");
    refresh();
    napms(400);

  }
}

int main(void) {
  setAlgNum(2);

  initscr ();
  echo();
  nocbreak();
  char* initmsg = "Zahlen sortieren von 0 - ";
  char inp [3];
  mvwaddstr(stdscr, LINES/2, COLS/2 - strlen(initmsg)/2, initmsg);
  mvwgetnstr(stdscr, LINES/2, COLS/2 + strlen(initmsg)/2 + 1, inp, 2);
  DIM = atoi(inp);

  cbreak();
  noecho();
  aArr = malloc(sizeof(int*)*algNum);
  winArr = malloc(sizeof(WINDOW*)*algNum);

  void (*sortArr[algNum]) (WINDOW*, int*);

  sortArr[0] = startQs;
  sortArr[1] = startSelSort;

  pthread_t thread2;
  omp_set_num_threads(algNum);
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
  wdelPrompt(stdscr, "Zum Sortieren TASTE drücken!");
  refresh();

#pragma omp parallel for 
  for(int i = 0; i<algNum; i++)
  {
    wprintPrompt(stdscr, "      ");
    (*sortArr[i])(winArr[i], aArr[i]);
  }
  endwin();
}
