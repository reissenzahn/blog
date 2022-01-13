#include <ncurses.h>

int main() {
  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();

  printw("Type any character to see it in bold...\n");

  int ch = getch();

  if (ch == KEY_F(1)) {
    printw("F1 Key pressed");
  } else {
    attron(A_BOLD);
    printw("%c", ch);
    attroff(A_BOLD);
  }

  refresh();

  getch();

  endwin();

  return 0;
}