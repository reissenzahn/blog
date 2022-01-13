#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/inotify.h>
#include <limits.h>

#define BUFSIZE (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "usage: %s pathname...\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int fd = inotify_init();
  if (fd == -1) {
    fprintf(stderr, "inotify_init: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  for (int j = 1; j < argc; j++) {
    int wd = inotify_add_watch(fd, argv[j], IN_ALL_EVENTS);
    if (wd == -1) {
      fprintf(stderr, "inotify_add_watch: %s\n", strerror(errno));
      exit(EXIT_FAILURE);
    }

    printf("watching %s (wd=%d)\n", argv[j], wd);
  }

  char buf[BUFSIZE];

  while (true) {
    ssize_t nread = read(fd, buf, BUFSIZE);

    if (nread == -1) {
      fprintf(stderr, "read: %s\n", strerror(errno));
      exit(EXIT_FAILURE);
    }

    if (nread == 0) {
      fprintf(stderr, "read: zero bytes read\n");
      exit(EXIT_FAILURE);
    }

    printf("read %ld bytes from inotify instance (fd=%d)\n", (long) nread, fd);

    for (char *ptr = buf; ptr < buf + nread; ) {
      struct inotify_event *event = (struct inotify_event *) ptr;

      printf("[wd = %2d", event->wd);
      
      if (event->cookie > 0) {
        printf("; cookie = %4d", event->cookie);
      }

      printf("; mask =");

      if (event->mask & IN_ACCESS) printf(" IN_ACCESS");
      if (event->mask & IN_ATTRIB) printf(" IN_ATTRIB");
      if (event->mask & IN_CLOSE_NOWRITE) printf(" IN_CLOSE_NOWRITE");
      if (event->mask & IN_CLOSE_WRITE) printf(" IN_CLOSE_WRITE");
      if (event->mask & IN_CREATE) printf(" IN_CREATE");
      if (event->mask & IN_DELETE) printf(" IN_DELETE");
      if (event->mask & IN_DELETE_SELF) printf(" IN_DELETE_SELF");
      if (event->mask & IN_IGNORED) printf(" IN_IGNORED");
      if (event->mask & IN_ISDIR) printf(" IN_ISDIR");
      if (event->mask & IN_MODIFY) printf(" IN_MODIFY");
      if (event->mask & IN_MOVE_SELF) printf(" IN_MOVE_SELF");
      if (event->mask & IN_MOVED_FROM) printf(" IN_MOVED_FROM");
      if (event->mask & IN_MOVED_TO) printf(" IN_MOVED_TO");
      if (event->mask & IN_OPEN) printf(" IN_OPEN");
      if (event->mask & IN_Q_OVERFLOW) printf(" IN_Q_OVERFLOW");
      if (event->mask & IN_UNMOUNT) printf(" IN_UNMOUNT");

      if (event->len > 0) {
        printf("; name = %s", event->name);
      }

      printf("]\n");

      ptr += sizeof(struct inotify_event) + event->len;
    }
  }

  exit(EXIT_SUCCESS);
}

// $ ./a.out dir1 dir2 &
// $ cat > dir1/aaa
// $ mv dir1/aaa dir2/bbb
// $ mkdir dir2/ddd
// $ rmdir dir1
