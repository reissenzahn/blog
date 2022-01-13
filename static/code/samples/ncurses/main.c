
// 1. basics
// #include <stdlib.h>
// #include <curses.h>

// int main(int argc, char *argv[]) {

//   initscr();
  
//   int x = 5;
//   int y = 10;
//   move(y, x);

//   printw("Hello, ncurses!");

//   refresh();


//   int ch = getch();
  
//   clear();

//   mvprintw(5, 10, "%d (%c)", ch, ch);

//   getch();

//   endwin();

//   return EXIT_SUCCESS;
// }


// 2. windows
#include <ncurses.h>

int main(int argc, char *argv[]) {
  
  initscr();

  int height = 10;
  int width = 20;
  int begin_x = 10;
  int begin_y = 10;
  WINDOW *win = newwin(height, width, begin_y, begin_x);

  box(win, 0, 0);

  int ch = getch();

  endwin();

  return 0;
}

