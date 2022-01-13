#include "game.h"

int main(int argc, char *argv[])
{
  int status = init();

  if (status == 0) {
    run();
  }

  close();

  return 0;
}