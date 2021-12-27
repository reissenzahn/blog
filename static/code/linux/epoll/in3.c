#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <string.h>

#define MAX_EVENTS 5
#define READ_SIZE 10

// https://www.suchprogramming.com/epoll-in-3-easy-steps/

int main(int argc, char *argv[]) {
  int running = 1, event_count, i;
  size_t bytes_read;
  char read_buffer[READ_SIZE + 1];
  struct epoll_event event, events[MAX_EVENTS];
  int epoll_fd = epoll_create1(0);
 
  if(epoll_fd == -1)
  {
    fprintf(stderr, "Failed to create epoll file descriptor\n");
    return 1;
  }
 
  event.events = EPOLLIN;
  event.data.fd = 0;
 
  if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, 0, &event))
  {
    fprintf(stderr, "Failed to add file descriptor to epoll\n");
    close(epoll_fd);
    return 1;
  }
 
  while(running)
  {
    printf("\nPolling for input...\n");
    event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, 30000);
    printf("%d ready events\n", event_count);
    for(i = 0; i < event_count; i++)
    {
      printf("Reading file descriptor '%d' -- ", events[i].data.fd);
      bytes_read = read(events[i].data.fd, read_buffer, READ_SIZE);
      printf("%zd bytes read.\n", bytes_read);
      read_buffer[bytes_read] = '\0';
      printf("Read '%s'\n", read_buffer);
 
      if(!strncmp(read_buffer, "stop\n", 5))
        running = 0;
    }
  }
 
  if(close(epoll_fd))
  {
    fprintf(stderr, "Failed to close epoll file descriptor\n");
    return 1;
  }
  return 0;
}


// Here’s the example in action:

// $ ./epoll_example 

// Polling for input..
// hello!

// 1 ready events
// Reading file descriptor '0' -- 7 bytes read.
// Read 'hello!
// '

// Polling for input...
// this is too long for the buffer we made

// 1 ready events
// Reading file descriptor '0' -- 10 bytes read.
// Read 'this is to'

// Polling for input...
// 1 ready events
// Reading file descriptor '0' -- 10 bytes read.
// Read 'o long for'

// Polling for input...
// 1 ready events
// Reading file descriptor '0' -- 10 bytes read.
// Read ' the buffe'

// Polling for input...
// 1 ready events
// Reading file descriptor '0' -- 10 bytes read.
// Read 'r we made
// '

// Polling for input...
// stop

// 1 ready events
// Reading file descriptor '0' -- 5 bytes read.
// Read 'stop
// '
