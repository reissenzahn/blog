#include <ncurses.h>

int main(int argc, char *argv[]) {
  initscr();

  start_color();

  init_pair(1, COLOR_CYAN, COLOR_BLACK);

	printw("A Big string which i didn't care to type fully ");

  // the third parameter is the number of characters to update; -1 means till end of line
  mvchgat(0, 0, -1, A_BLINK, 1, NULL);

  refresh();
  getch();
  endwin();

  return 0;
}
