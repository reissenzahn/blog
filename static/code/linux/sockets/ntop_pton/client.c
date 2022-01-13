#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ctype.h>

#define BUF_SIZE 10
#define PORT_NUM 50002


int main(int argc, char *argv[]) {

  if (argc < 3) {
    fprintf(stderr, "%s host-address msg...\n", argv[0]);
    exit(EXIT_FAILURE);
  }
}


 struct sockaddr_in6 svaddr;
 int sfd, j;
 size_t msgLen;
 ssize_t numBytes;
 char resp[BUF_SIZE];

if (argc < 3 || strcmp(argv[1], "--help") == 0)
 usageErr("%s host-address msg...\n", argv[0]);

 sfd = socket(AF_INET6, SOCK_DGRAM, 0); /* Create client socket */
 if (sfd == -1)
 errExit("socket");

 memset(&svaddr, 0, sizeof(struct sockaddr_in6));
 svaddr.sin6_family = AF_INET6;
 svaddr.sin6_port = htons(PORT_NUM);
 if (inet_pton(AF_INET6, argv[1], &svaddr.sin6_addr) <= 0)
 fatal("inet_pton failed for address '%s'", argv[1]);
 /* Send messages to server; echo responses on stdout */
 for (j = 2; j < argc; j++) {
 msgLen = strlen(argv[j]);
 if (sendto(sfd, argv[j], msgLen, 0, (struct sockaddr *) &svaddr,
 sizeof(struct sockaddr_in6)) != msgLen)
 fatal("sendto");
 numBytes = recvfrom(sfd, resp, BUF_SIZE, 0, NULL, NULL);
 if (numBytes == -1)
 errExit("recvfrom");
 printf("Response %d: %.*s\n", j - 1, (int) numBytes, resp);
 }
 exit(EXIT_SUCCESS);
}





i6d_ucase_sv.c
#include "i6d_ucase.h"
int
main(int argc, char *argv[])
{
 struct sockaddr_in6 svaddr, claddr;
 int sfd, j;
 ssize_t numBytes;
 socklen_t len;
 char buf[BUF_SIZE];
 char claddrStr[INET6_ADDRSTRLEN];
 sfd = socket(AF_INET6, SOCK_DGRAM, 0);
 if (sfd == -1)
 errExit("socket");
 memset(&svaddr, 0, sizeof(struct sockaddr_in6));
 svaddr.sin6_family = AF_INET6;
 svaddr.sin6_addr = in6addr_any; /* Wildcard address */
 svaddr.sin6_port = htons(PORT_NUM);
 if (bind(sfd, (struct sockaddr *) &svaddr,
 sizeof(struct sockaddr_in6)) == -1)
 errExit("bind");
 /* Receive messages, convert to uppercase, and return to client */
 for (;;) {
 len = sizeof(struct sockaddr_in6);
 numBytes = recvfrom(sfd, buf, BUF_SIZE, 0,
 (struct sockaddr *) &claddr, &len);
 if (numBytes == -1)
 errExit("recvfrom");
 if (inet_ntop(AF_INET6, &claddr.sin6_addr, claddrStr,
 INET6_ADDRSTRLEN) == NULL)
 printf("Couldn't convert client address to string\n");
 else
 printf("Server received %ld bytes from (%s, %u)\n",
 (long) numBytes, claddrStr, ntohs(claddr.sin6_port));
 for (j = 0; j < numBytes; j++)
 buf[j] = toupper((unsigned char) buf[j]);
 if (sendto(sfd, buf, numBytes, 0, (struct sockaddr *) &claddr, len) !=
 numBytes)
 fatal("sendto");
 }
}
