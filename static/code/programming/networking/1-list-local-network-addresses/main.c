#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// list network adapters and addresses
// $ gcc list.c && ./a.out

int main() {
  struct ifaddrs *addresses;
  if (getifaddrs(&addresses) == -1) {
    perror("getifaddrs");
    exit(EXIT_FAILURE);
  }

  for (struct ifaddrs *address = addresses; address != NULL; address = address->ifa_next) {
    int family = address->ifa_addr->sa_family;

    if (family == AF_INET || family == AF_INET6) {
      char ap[100];
      int family_size = family == AF_INET ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);

      if (getnameinfo(address->ifa_addr, family_size, ap, sizeof(ap), 0, 0, NI_NUMERICHOST) != 0) {
        fprintf(stderr, "getnameinfo: %s", gai_strerror(errno));
      }

      printf("%s %s %s\n", address->ifa_name, family == AF_INET ? "IPv4" : "IPv6", ap);
    }
  }

  freeifaddrs(addresses);
  
  exit(EXIT_SUCCESS);
}
