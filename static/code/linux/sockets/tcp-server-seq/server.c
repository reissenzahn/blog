// to get definitions of NI_MAXHOST and NI_MAXSERV from <netdb.h>
#define _BSD_SOURCE

#include <netinet/in.h>
#include <sys/socket.h>
#include <signal.h>
#include <netdb.h>

#define PORT "50000"
#define BUFSIZE 30
#define BACKLOG 50
#define ADDRSTRLEN (NI_MAXHOST + NI_MAXSERV + 10)

int main(int argc, char *argv[]) {

  uint32_t seq_num = 0;

  // ignore the SIGPIPE signal received if the server attempts to write to a socket whose peer has been closed; we instead check if write() fails with error EPIPE
  if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
    fprintf("signal: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  struct addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;      // ipv4 or ipv6
  hints.ai_socktype = SOCK_STREAM;  // tcp
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;
  hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;  // wildcard address

  // obtain a set of socket addresses for a TCP socket that uses the provided port number
  struct addrinfo *result;
  int rc = getaddrinfo(NULL, PORT, &hints, &result);
  if (rc != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rc));
    exit(EXIT_FAILURE);
  }

  // step through the returned socket address structures until a successful bind can be performed
  for (struct addrinfo *rp = result; rp != NULL; rp = rp->ai_next) {
    lfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (lfd == -1) {
      continue;
    }

    int optval = 1;
    if (setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
      fprintf(stderr, "setsockopt: %s\n", strerror(errno));
      exit(EXIT_FAILURE);
    }

    if (bind(lfd, rp->ai_addr, rp->ai_addrlen) == -1) {
      close(lfd);
      continue;
    }

    break;
  }

  freeaddrinfo(result);

  if (rp == NULL) {
    fprintf(stderr, "bind: failed to bind socket to any address\n");
    exit(EXIT_FAILURE);
  }

  if (listen(lfd, BACKLOG) == -1) {
    fprintf(stderr, "listen: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  while (1) {
    struct sockaddr_storage caddr;
    socklet_t addrlen = sizeof(struct sockaddr_storage);
    int cfd = accept(lfd, (struct sockaddr *) &caddr, &addrlen);
    if (cfd == -1) {
      fprintf(stderr, "accept: %s\n", strerror(errno));
      continue;
    }

    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
    int rc = getnameinfo((struct sockaddr *) caddr, addrlen, host, NI_MAXHOST, service, NI_MAXSERV, 0);
    if (rc != 0) {
      printf("connection from unknown\n");
    } else {
      printf("connection from %s@%s\n", host, service);
    }

    char buffer[BUFSIZE];
    if (read_line(cfd, buffer, BUFSIZE) <= 0) {
      close(cfd);
      continue;
    }
  }

  exit(EXIT_SUCCESS);
}

for (;;) { /* Handle clients iteratively */
 
 /* Read client request, send sequence number back */
d if (readLine(cfd, reqLenStr, INT_LEN) <= 0) {
 close(cfd);
 continue; /* Failed read; skip request */
 }
f reqLen = atoi(reqLenStr);
 if (reqLen <= 0) { /* Watch for misbehaving clients */
 close(cfd);
 continue; /* Bad request; skip it */
 }
g snprintf(seqNumStr, INT_LEN, "%d\n", seqNum);
 if (write(cfd, &seqNumStr, strlen(seqNumStr)) != strlen(seqNumStr))
 fprintf(stderr, "Error on write");
Sockets: Internet Domains 1223
h seqNum += reqLen; /* Update sequence number */
 if (close(cfd) == -1) /* Close connection */
 errMsg("close");
 }
}

 
 char reqLenStr[INT_LEN]; /* Length of requested sequence */
 char seqNumStr[INT_LEN]; /* Start of granted sequence */
 struct sockaddr_storage claddr;
 int reqLen;




– Read the client’s message d, which consists of a newline-terminated string
specifying how many sequence numbers the client wants. The server converts this string to an integer and stores it in the variable reqLen f.
– Send the current value of the sequence number (seqNum) back to the client, encoding it as a newline-terminated string g. The client can assume
that it has been allocated all of the sequence numbers in the range seqNum
to (seqNum + reqLen – 1).
– Update the value of the server’s sequence number by adding reqLen to
seqNum h.

----


z Call getaddrinfo() to obtain a set of socket address structures suitable for connecting to a TCP server bound to the specified host q. For the port number,
the client specifies PORT_NUM.
z Enter a loop w that iterates through the socket address structures returned by the
previous step, until the client finds one that can be used to successfully create e
and connect r a socket to the server. Since the client has not bound its socket,
the connect() call causes the kernel to assign an ephemeral port to the socket.
z Send an integer specifying the length of the client’s desired sequence t. This
integer is sent as a newline-terminated string.
z Read the sequence number sent back by the server (which is likewise a newlineterminated string) y and print it on standard output u.



#include <netdb.h>
#include "is_seqnum.h"
int
main(int argc, char *argv[])
{
 char *reqLenStr; /* Requested length of sequence */
 char seqNumStr[INT_LEN]; /* Start of granted sequence */
 int cfd;
 ssize_t numRead;
 struct addrinfo hints;
 struct addrinfo *result, *rp;
 if (argc < 2 || strcmp(argv[1], "--help") == 0)
 usageErr("%s server-host [sequence-len]\n", argv[0]);
 /* Call getaddrinfo() to obtain a list of addresses that
 we can try connecting to */
 memset(&hints, 0, sizeof(struct addrinfo));
 hints.ai_canonname = NULL;
 hints.ai_addr = NULL;
 hints.ai_next = NULL;
 hints.ai_family = AF_UNSPEC; /* Allows IPv4 or IPv6 */
 hints.ai_socktype = SOCK_STREAM;
 hints.ai_flags = AI_NUMERICSERV;
q if (getaddrinfo(argv[1], PORT_NUM, &hints, &result) != 0)
 errExit("getaddrinfo");
 /* Walk through returned list until we find an address structure
 that can be used to successfully connect a socket */
w for (rp = result; rp != NULL; rp = rp->ai_next) {
e cfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
 if (cfd == -1)
 continue; /* On error, try next address */
r if (connect(cfd, rp->ai_addr, rp->ai_addrlen) != -1)
 break; /* Success */
Sockets: Internet Domains 1225
 /* Connect failed: close this socket and try next address */
 close(cfd);
 }
 if (rp == NULL)
 fatal("Could not connect socket to any address");
 freeaddrinfo(result);
 /* Send requested sequence length, with terminating newline */
t reqLenStr = (argc > 2) ? argv[2] : "1";
 if (write(cfd, reqLenStr, strlen(reqLenStr)) != strlen(reqLenStr))
 fatal("Partial/failed write (reqLenStr)");
 if (write(cfd, "\n", 1) != 1)
 fatal("Partial/failed write (newline)");
 /* Read and display sequence number returned by server */
y numRead = readLine(cfd, seqNumStr, INT_LEN);
 if (numRead == -1)
 errExit("readLine");
 if (numRead == 0)
 fatal("Unexpected EOF from server");
u printf("Sequence number: %s", seqNumStr); /* Includes '\n' */
 exit(EXIT_SUCCESS); /* Closes 'cfd' */
}

