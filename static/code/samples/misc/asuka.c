#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <stdarg.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#define VERSION "0.0.1"
#define TAB_STOP 8

// the CTRL key has the effect of clearing bits 5 and 6
#define CTRL_KEY(k) ((k) & 0x1f)

enum keys {
  ARROW_LEFT = 1000,
  ARROW_RIGHT,
  ARROW_UP,
  ARROW_DOWN,
  DEL_KEY,
  HOME_KEY,
  END_KEY,
  PAGE_UP,
  PAGE_DOWN
};

struct row {
  int size;
  char *chars;

  int render_size;
  char *render_chars;
};

struct {
  int cursor_x;  // horizontal cursor coordinate
  int cursor_y;  // vertical cursor coordinate

  int render_x;  // horizontal render cursor coordinate

  int row_offset
}


