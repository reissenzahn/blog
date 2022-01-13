#include <string>
#include <ncurses.h>

#include "game.h"

WINDOW* wnd;

int init() {
  wnd = initscr();
  cbreak();
  noecho();
  clear();
  refresh();
  keypad(wnd, true);
  nodelay(wnd, true);
  curs_set(0);

  if (!has_colors()) {
    endwin();
    printf("ERROR: Terminal does not support color.\n");
    exit(1);
  }

  start_color();

  attron(A_BOLD);
  box(wnd, 0, 0);
  attroff(A_BOLD);

  return 0;
}

void run() {

  move(5, 5);

  std::string text = "Hello world!";

  for (size_t i = 0; i < text.size(); i++) {
    addch(text[i]);
    addch(' ');
  }

  refresh();

  getch();
}

void close() {
  endwin();
}
