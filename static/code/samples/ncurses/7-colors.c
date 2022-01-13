#include <ncurses.h>

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string) {
  int length;
	float temp;

	if(win == NULL) {
		win = stdscr;
  }

  int y, x;
	getyx(win, y, x);

	if (startx != 0) {
    x = startx;
  }

	if (starty != 0) {
    y = starty;
  }

	if(width == 0) {
    width = 80;
  }

	x = startx + (int) ((width - strlen(string)) / 2);

	mvwprintw(win, y, x, "%s", string);

	refresh();
}

int main() {
  
  initscr();

  if (has_colors() == FALSE) {
    endwin();
    printf("Your terminal does not support color.\n");
    exit(1);
  }

  start_color();

  init_pair(1, COlOR_RED, COLOR_BLACK);

  attron(COLOR_PAIR(1));

  print_in_middle(stdscr, LINES / 2, 0, 0, "COLOR!");

  attroff(COLOR_PAIR());

  getch();
  endwin();

  return 0;
}

