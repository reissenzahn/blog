// https://www.man7.org/linux/man-pages/man2/mmap.2.html



#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(int argc, char *argv[]) {
  int fd;
  struct stat sb;
  off_t offset, pa_offset;

  if (argc < 3 || argc > 4) {
    fprintf(stderr, "%s file offset [length]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  fd = open(argv[1], O_RDONLY);
  if (fd == -1) handle_error("open");

  if (fstat(fd, &sb) == -1) handle_error("fstat");

  offset = atoi(argv[2]);
  
  // offset for mmap() must be page aligned
  pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);

  if (offset >= sb.st_size) {
    fprintf(stderr, "offset is past end of file\n");
    exit(EXIT_FAILURE);
  }

  if (argc == 4) {
    length = atoi(argv[3]);

    if (offset + length > sb.st_size) {
      // cannot display bytes past end of file
      length = sb.st_size - offset;
    }
  } else {
    // display to end of file by default
    length = sb.st_size - offset;
  }

  addr = mmap(NULL, length + offset - pa_offset, PROT_READ, MAP_PRIVATE, fd, pa_offset);
  
  if (addr == MAP_FAILED) handle_error("mmap");

  s = write(STDOUT_FILENO, addr + offset - pa_offset, length);
  if (s == -1) handle_error("write");

  if (s != length) {
    fprintf(stderr, "partial write");
    exit(EXIT_FAILURE);
  }

  munmap(addr, length + offset - pa_offset);
  close(fd);

  exit(EXIT_SUCCESS);
}
