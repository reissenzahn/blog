#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

int main(void) {
  const char *text = "hello";

  int fd = open("test", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

  lseek(fd, strlen(text), SEEK_SET);

  write(fd, "", 1);

  char *map = mmap(0, strlen(text) + 1, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  memcpy(map, text, strlen(text));

  msync(map, strlen(text) + 1, MS_SYNC);

  munmap(map, strlen(text) + 1);

  close(fd);

  return 0;
}

// https://jameshfisher.com/2017/01/28/mmap-file-write/
