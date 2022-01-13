z The host argument is a string containing either a hostname or a numeric
address (in IPv4 dotted-decimal, or IPv6 hex-string notation). Alternatively,
host can be specified as a NULL pointer to indicate that the loopback IP address is
to be used.
z The service argument is either a service name or a port number specified as a
decimal string.
z The type argument is a socket type, specified as either SOCK_STREAM or SOCK_DGRAM.


#include <sys/socket.h>
#include <netdb.h>

int inetConnect(const char *host, const char *service, int type);
int inetListen(const char *service, int backlog, socklen_t *addrlen);
int inetBind(const char *service, int type, socklen_t *addrlen);
char *inetAddressStr(const struct sockaddr *addr, socklen_t addrlen,
 char *addrStr, int addrStrLen);
#define IS_ADDR_STR_LEN 4096
 /* Suggested length for string buffer that caller
 should pass to inetAddressStr(). Must be greater
 than (NI_MAXHOST + NI_MAXSERV + 4) */

The inetListen() function creates a listening stream (SOCK_STREAM) socket bound
to the wildcard IP address on the TCP port specified by service. This function is
designed for use by TCP servers.
The file descriptor for the new socket is returned as the function result.
The backlog argument specifies the permitted backlog of pending connections
(as for listen()).
#include "inet_sockets.h"
int inetConnect(const char *host, const char *service, int type);
Returns a file descriptor on success, or –1 on error
#include "inet_sockets.h"
int inetListen(const char *service, int backlog, socklen_t *addrlen);
Returns a file descriptor on success, or –1 on error
Sockets: Internet Domains 1227
If addrlen is specified as a non-NULL pointer, then the location it points to is used
to return the size of the socket address structure corresponding to the returned file
descriptor. This value allows us to allocate a socket address buffer of the appropriate size to be passed to a later accept() call if we want to obtain the address of a connecting client.
The inetBind() function creates a socket of the given type, bound to the wildcard
IP address on the port specified by service and type. (The socket type indicates
whether this is a TCP or UDP service.) This function is designed (primarily) for
UDP servers and clients to create a socket bound to a specific address.
The file descriptor for the new socket is returned as the function result.
As with inetListen(), inetBind() returns the length of the associated socket
address structure for this socket in the location pointed to by addrlen. This is useful
if we want to allocate a buffer to pass to recvfrom() in order to obtain the address of
the socket sending a datagram. (Many of the steps required for inetListen() and
inetBind() are the same, and these steps are implemented within the library by a
single function, inetPassiveSocket().)
The inetAddressStr() function converts an Internet socket address to printable form.
Given a socket address structure in addr, whose length is specified in addrlen,
inetAddressStr() returns a null-terminated string containing the corresponding hostname and port number in the following form:
(hostname, port-number)
The string is returned in the buffer pointed to by addrStr. The caller must specify the
size of this buffer in addrStrLen. If the returned string would exceed (addrStrLen – 1)
bytes, it is truncated. The constant IS_ADDR_STR_LEN defines a suggested size for the
addrStr buffer that should be large enough to handle all possible return strings. As
its function result, inetAddressStr() returns addrStr.
The implementation of the functions described in this section is shown in
Listing 59-9.
#include "inet_sockets.h"
int inetBind(const char *service, int type, socklen_t *addrlen);
Returns a file descriptor on success, or –1 on error
#include "inet_sockets.h"
char *inetAddressStr(const struct sockaddr *addr, socklen_t addrlen,
 char *addrStr, int addrStrLen);
Returns pointer to addrStr, a string containing host and service name
1228 Chapter 59
Listing 59-9: An Internet domain sockets library
–––––––––––––––––––––––––––––––––––––––––––––––––––– sockets/inet_sockets.c
#define _BSD_SOURCE /* To get NI_MAXHOST and NI_MAXSERV
 definitions from <netdb.h> */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>



// create a socket with the given socket type and connect it to the address specified by host and service
int inet_connect(const char *host, const char *service, int type) {

}

int
inetConnect(const char *host, const char *service, int type)
{
 struct addrinfo hints;
 struct addrinfo *result, *rp;
 int sfd, s;
 memset(&hints, 0, sizeof(struct addrinfo));
 hints.ai_canonname = NULL;
 hints.ai_addr = NULL;
 hints.ai_next = NULL;
 hints.ai_family = AF_UNSPEC; /* Allows IPv4 or IPv6 */
 hints.ai_socktype = type;
 s = getaddrinfo(host, service, &hints, &result);
 if (s != 0) {
 errno = ENOSYS;
 return -1;
 }
 /* Walk through returned list until we find an address structure
 that can be used to successfully connect a socket */
 for (rp = result; rp != NULL; rp = rp->ai_next) {
 sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
 if (sfd == -1)
 continue; /* On error, try next address */
 if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
 break; /* Success */
 /* Connect failed: close this socket and try next address */
 close(sfd);
 }
 freeaddrinfo(result);
 return (rp == NULL) ? -1 : sfd;
}




static int /* Public interfaces: inetBind() and inetListen() */
inetPassiveSocket(const char *service, int type, socklen_t *addrlen,
 Boolean doListen, int backlog)
{
 struct addrinfo hints;
 struct addrinfo *result, *rp;
 int sfd, optval, s;
 memset(&hints, 0, sizeof(struct addrinfo));
 hints.ai_canonname = NULL;
 hints.ai_addr = NULL;
 hints.ai_next = NULL;
 hints.ai_socktype = type;
 hints.ai_family = AF_UNSPEC; /* Allows IPv4 or IPv6 */
 hints.ai_flags = AI_PASSIVE; /* Use wildcard IP address */
 s = getaddrinfo(NULL, service, &hints, &result);
 if (s != 0)
 return -1;
 /* Walk through returned list until we find an address structure
 that can be used to successfully create and bind a socket */
 optval = 1;
 for (rp = result; rp != NULL; rp = rp->ai_next) {
 sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
 if (sfd == -1)
 continue; /* On error, try next address */
 if (doListen) {
 if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &optval,
 sizeof(optval)) == -1) {
 close(sfd);
 freeaddrinfo(result);
 return -1;
 }
 }
 if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
 break; /* Success */
 /* bind() failed: close this socket and try next address */
 close(sfd);
 }
 if (rp != NULL && doListen) {
 if (listen(sfd, backlog) == -1) {
 freeaddrinfo(result);
 return -1;
 }
 }
 if (rp != NULL && addrlen != NULL)
 *addrlen = rp->ai_addrlen; /* Return address structure size */
1230 Chapter 59
 freeaddrinfo(result);
 return (rp == NULL) ? -1 : sfd;
}
int
inetListen(const char *service, int backlog, socklen_t *addrlen)
{
 return inetPassiveSocket(service, SOCK_STREAM, addrlen, TRUE, backlog);
}
int
inetBind(const char *service, int type, socklen_t *addrlen)
{
 return inetPassiveSocket(service, type, addrlen, FALSE, 0);
}
char *
inetAddressStr(const struct sockaddr *addr, socklen_t addrlen,
 char *addrStr, int addrStrLen)
{
 char host[NI_MAXHOST], service[NI_MAXSERV];
 if (getnameinfo(addr, addrlen, host, NI_MAXHOST,
 service, NI_MAXSERV, NI_NUMERICSERV) == 0)
 snprintf(addrStr, addrStrLen, "(%s, %s)", host, service);
 else
 snprintf(addrStr, addrStrLen, "(?UNKNOWN?)");
 addrStr[addrStrLen - 1] = '\0'; /* Ensure result is null-terminated */
 return addrStr;
}
–––––––––––––––––––––––––––––––––––––––––––––––––––– sockets/inet_sockets.c



The readn() and writen() functions take the same arguments as read() and write(). However, they use a loop to restart these system calls, thus ensuring that the requested number of bytes is always transferred (unless an error occurs or end-offile is detected on a read()).

ssize_t readn(int fd, void *buffer, size_t count);

ssize_t writen(int fd, void *buffer, size_t count);


#include <unistd.h>
#include <errno.h>
#include "rdwrn.h" /* Declares readn() and writen() */
ssize_t
readn(int fd, void *buffer, size_t n)
{
 ssize_t numRead; /* # of bytes fetched by last read() */
 size_t totRead; /* Total # of bytes read so far */
 char *buf;
 buf = buffer; /* No pointer arithmetic on "void *" */
 for (totRead = 0; totRead < n; ) {
 numRead = read(fd, buf, n - totRead);
 if (numRead == 0) /* EOF */
 return totRead; /* May be 0 if this is first read() */
 if (numRead == -1) {
 if (errno == EINTR)
 continue; /* Interrupted --> restart read() */
 else
 return -1; /* Some other error */
 }
 totRead += numRead;
 buf += numRead;
 }
 return totRead; /* Must be 'n' bytes if we get here */
}
ssize_t
writen(int fd, const void *buffer, size_t n)
{
 ssize_t numWritten; /* # of bytes written by last write() */
 size_t totWritten; /* Total # of bytes written so far */
 const char *buf;
 buf = buffer; /* No pointer arithmetic on "void *" */
 for (totWritten = 0; totWritten < n; ) {
 numWritten = write(fd, buf, n - totWritten);
 if (numWritten <= 0) {
 if (numWritten == -1 && errno == EINTR)
 continue; /* Interrupted --> restart write() */
 else
 return -1; /* Some other error */
 }
 totWritten += numWritten;
 buf += numWritten;
 }
 return totWritten; /* Must be 'n' bytes if we get here */
}