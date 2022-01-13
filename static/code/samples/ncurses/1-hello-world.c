#include <ncurses.h>

int main() {

  // start curses mode
  initscr();

  // print
  printw("Hello world!");

  // 
  refresh();

  // wait for user input
  getch();

  // end curses mode
  endwin();

  return 0;
}

// To use ncurses library functions, you have to include ncurses.h in your programs. To link the program with ncurses the flag -lncurses should be added.


