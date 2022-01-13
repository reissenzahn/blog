#include <ncurses.h>
#include <string.h>

int main() {
  char msg[] = "Enter a string: ";

  initscr();

  int row, col;
  getmaxyx(stdscr, row, col);

  mvprintw(row / 2, (col - strlen(msg)) / 2, "%s", msg);

  char str[80];
  getstr(str);

  mvprintw(LINES - 2, 0, "You Entered: %s", str);  
  
  getch();
  endwin();

  return 0;
}
