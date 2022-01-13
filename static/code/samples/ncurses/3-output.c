#include <ncurses.h>
#include <string.h>

int main() {
  char msg[] = "Hello, World!";

  initscr();

  int row, col;
  getmaxyx(stdscr, row, col);

  mvprintw(row / 2, (col - strlen(msg)) / 2, "%s", msg);
  mvprintw(row - 1, 0, "This screen has %d rows and %d columns\n", row, col);

  refresh();
  getch();
  endwin();

  return 0;
}
