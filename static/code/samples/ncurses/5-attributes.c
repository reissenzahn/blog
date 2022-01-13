#include <ncurses.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

  if (argc != 2) {
    printf("usage: %s filename\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  FILE *fp = fopen(argv[1], "r");
  if (fp == NULL) {
    fprintf(stderr, "failed to open file\n");
    exit(EXIT_FAILURE);
  }

  initscr();
  
  int row, col;
  getmaxyx(stdscr, row, col);

  int ch, prev;

  prev = EOF;

  while ((ch = fgetc(fp)) != EOF) {
    int x, y;
    getyx(stdscr, y, x);

    // check if the end of the screen has been reached
    if (y == (row - 1)) {
      printw("<press any key>");
      getch();
      clear();
      move(0, 0);
    }

    if (prev == '/' && ch == '*') {
      attron(A_BOLD);

      getyx(stdscr, y, x);

      // back up one space
      move(y, x - 1);

      printw("%c%c", '/', ch);
    } else {
      printw("%c", ch);
    }

    refresh();

    if (prev == '*' && ch == '/') {
      attroff(A_BOLD);
    }

    prev = ch;
  }

  endwin();

  fclose(fp);

  return 0;
}
