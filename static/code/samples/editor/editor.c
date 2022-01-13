#include <ncurses.h>

void finish() {
  endwin();
  exit(EXIT_SUCCESS);
}

void interrupt_handler(int sig) {
  finish();
}

void setup() {
  signal(SIGINT, interrupt_handler);
  
  initscr();
  raw();
  noecho();
  keypad(stdscr, TRUE);
  
}


int main(int argc, char *argv[]) {

}

