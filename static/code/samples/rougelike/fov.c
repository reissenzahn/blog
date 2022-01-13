#include <ncurses.h>
#include <assert.h>

struct vec {
  int x;
  int y;
};

struct vec transform_octant(int row, int col, int octant) {
  struct vec v;

  switch (octant) {
  case 0:
    v.x = col;
    v.y = -row;
    break;
  case 1:
    v.x = row;
    v.y = -col;
    break;
  case 2:
    v.x = row;
    v.y = col;
    break;
  case 3:
    v.x = col;
    v.y = row;
    break;
  case 4:
    v.x = -col;
    v.y = row;
    break;
  case 5:
    v.x = -row;
    v.y = col;
    break;
  case 6:
    v.x = -row;
    v.y = -col;
    break;
  case 7:
    v.x = -col;
    v.y = -row;
    break;
  default:
    assert(false);
    break;
  }

  return v;
}

void paint_octants(int player_x, int player_y, int max_distance) {
  mvaddch(player_y, player_x, '@');

  for (int octant = 0; octant < 8; octant++) {
    for (int row = 1; row < max_distance; row++) {
      for (int col = 0; col <= row; col++) {
        

        struct vec v = transform_octant(col, row, octant);

        // map octant space to tile space
        int x = player_x + v.x;
        int y = player_y - v.y;

        attron(COLOR_PAIR(octant + 1));
        mvaddch(y, x, '.');
        attroff(COLOR_PAIR(octant + 1));

        refresh();
        
        napms(100);
      }
    }
  }
}

int main(int argc, char *argv[]) {

  initscr();
  cbreak();
  noecho();
  keypad(stdscr, true);
  curs_set(0);

  start_color();
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);
  init_pair(4, COLOR_BLUE, COLOR_BLACK);
  init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(6, COLOR_CYAN, COLOR_BLACK);
  init_pair(7, COLOR_WHITE, COLOR_BLACK);
  init_pair(8, COLOR_BLACK, COLOR_BLACK);

  clear();
  refresh();

  paint_octants(20, 20, 10);

  refresh();

  getch();

  endwin();

  return 0;
}

