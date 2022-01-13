#include <ncurses.h>

int main() {
  initscr();

  addstr("Hello!");

  char text[] = "Greetings from NCurses!";
  char *t = text;

  while (*t != '\0') {
    addch(*t);
    t++;
    refresh();
    napms(100);
  }

  getch();

  endwin();

  return 0;
}


// the standard screen (stdscr) is the default output window which is exactly the same size as the terminal screen

// the current screen (curscr) is the internal representation of what is believed to be on the terminal screen
