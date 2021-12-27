




























// struct cbuf {
//   unsigned long int size;
//   unsigned int head, tail;
//   void *data;
// };

// struct cbuf *cbuf_init(const unsigned int order) {
//   struct cbuf *;
//   char *path;
//   void *data, *address;

//   this = malloc(sizeof(struct cbuf));
  
//   if (this == NULL) {
//     fprintf(stderr, "out of memory!");
//     return -1;
//   }

//   this->size = 1UL << order;
//   this->head = 0;
//   this->tail = 0;

//   path = "/tmp/cbuf-XXXXXX";

//   fd = mkstemp(path);
//   if (fd == -1) {
//     perror("mkstemp");
//     return -1;
//   }

//   if (unlink(path) == -1) {
//     perror("unlink");
//     return -1;
//   }

//   if (ftruncate(fd, this->size) == -1) {
//     perror("ftruncate");
//     return -1;
//   }

//   data = mmap(NULL, this->size << 1, PROT_NONE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
//   if (data == MAP_FAILED) {
//     perror("mmap");
//     return -1;
//   }

//   address = mmap(this->data, this->size, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_SHARED, fd, 0);
//   if (address != this->data) {
//     fprintf("mmap hint ignored!");
//     return -1;
//   }

//   address = mmap(cb->data + cb->size, cb->size, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_SHARED, fd, 0);
//   if (address != cb->data + cb->size)
//     fprintf("mmap hint ignored!");
//     return -1;
//   }

//   this->data = data; 

//   if (close(fd) == -1) {
//     perror("close");
//     return -1;
//   }

//   return this;
// }

// void cbuf_destroy(struct cbuf *this) {
//   munmap(this->data, this->size << 1);
//   free(this);
// }



// int cbuf_size(struct cbuf *this) {
//   return this->size;
// }

// int cbuf_is_empty(struct cbuf *this) {
//   return this->head == this->tail;
// }


// // A circular-buffer implementation may be optimized by mapping the underlying buffer to two contiguous regions of virtual memory.[5] (Naturally, the underlying buffer‘s length must then equal some multiple of the system’s page size.) Reading from and writing to the circular buffer may then be carried out with greater efficiency by means of direct memory access; those accesses which fall beyond the end of the first virtual-memory region will automatically wrap around to the beginning of the underlying buffer. When the read offset is advanced into the second virtual-memory region, both offsets—read and write—are decremented by the length of the underlying buffer.



