
2
4
5
6
7
9
10
12
13



3 An In-Depth Overview of TCP Connections

<!-- The hostname could be a domain name like example.com or an IP address such as 192.168.17.23 or ::1. The port can be a number, such as 80, or a protocol, such as http. -->

For this reason, we set up a timeout to the
select() call for 100 milliseconds (100,000 microseconds). If there is no socket activity
after 100 milliseconds, select() returns, and we can check for terminal input manually.


If the socket has closed, send() returns -1. We ignore this case here. This is because a
closed socket causes select() to return immediately, and we notice the closed socket on
the next call to recv(). This is a common paradigm in TCP socket programming to ignore
errors on send() while detecting and handling them on recv(). It allows us to simplify
our program by keeping our connection closing logic all in one place.




Building a chat room

It is also possible, and common, to need to send data between connected clients. We can
modify our tcp_serve_toupper.c program and make it a chat room pretty easily.
First, locate the following code in tcp_serve_toupper.c:
/*tcp_serve_toupper.c excerpt*/
 int j;
 for (j = 0; j < bytes_received; ++j)
 read[j] = toupper(read[j]);
 send(i, read, bytes_received, 0);
Replace the preceding code with the following:
/*tcp_serve_chat.c excerpt*/
 SOCKET j;
 for (j = 1; j <= max_socket; ++j) {
 if (FD_ISSET(j, &master)) {
 if (j == socket_listen || j == i)
 continue;
 else
 send(j, read, bytes_received, 0);
 }
 }


When we call send() with an amount of data, send() first copies this data into an
outgoing buffer provided by the operating system. If we call send() when its outgoing
buffer is already full, it blocks until its buffer has emptied enough to accept more of our
data.

In some cases where send() would block, it instead returns without copying all of the data
as requested. In this case, the return value of send() indicates how many bytes were
actually copied. One example of this is if your program is blocking on send() and then
receives a signal from the operating system. In these cases, it is up to the caller to try again
with any remaining data.

In this chapter's TCP server code section, we ignored the possibility that send() could block
or be interrupted. In a fully robust application, what we need to do is compare the return
value from send() with the number of bytes that we tried to send. If the number of bytes
actually sent is less than requested



This code blocks until we've sent all of buffer or
an error (such as the peer disconnecting) occurs:
int begin = 0;
while (begin < buffer_len) {
 int sent = send(peer_socket, buffer + begin, buffer_len - begin, 0);
 if (sent == -1) {
 //Handle error
 }
 begin += sent;
}

If we are managing multiple sockets and don't want to block, then we should put all
sockets with pending send() into an fd_set and pass it as the third parameter to
select(). When select() signals on these sockets, then we know that they are ready to
send more data.


We will cover techniques for dealing with recv() buffering in Section 2, An Overview of
Application Layer Protocols, of this book.



Questions
Try answering these questions to test your knowledge on this chapter:
1. How can we tell whether the next call to recv() will block?
2. How can you ensure that select() doesn't block for longer than a specified
time?
3. When we used our tcp_client program to connect to a web server, why did we
need to send a blank line before the web server responded?
4. Does send() ever block?
5. How can we tell whether the socket has been disconnected by our peer?
6. Is data received by recv() always the same size as data sent with send()?
7. Consider the following code:
recv(socket_peer, buffer, 4096, 0);
printf(buffer);
What is wrong with it?
Also see what is wrong with this code:
recv(socket_peer, buffer, 4096, 0);
printf("%s", buffer);
The answers can be found in Appendix A, Answers to Questions.






4
Establishing UDP Connections
In this chapter, we will look at how to send and receive User Datagram Protocol (UDP)
packets. UDP socket programming is very similar to Transmission Control Protocol (TCP)
socket programming, so it is recommended that you read and understand Chapter 3, An
In-Depth Overview of TCP Connections, before beginning this one.
The following topics are covered in this chapter:
The differences between TCP socket programming and UDP socket
programming
The sendto() and recvfrom() functions
How connect() works on a UDP socket
Implementing a UDP server using only one socket
Using select() to tell when a UDP socket has data ready
Technical requirements
The example programs in this chapter can be compiled with any modern C compiler. We
recommend MinGW on Windows and GCC on Linux and macOS. See Appendix B, Setting
Up Your C Compiler On Windows, Appendix C, Setting Up Your C Compiler On Linux, and
Appendix D, Setting Up Your C Compiler On macOS, for compiler setup.
Establishing UDP Connections Chapter 4
[ 103 ]
The code for this book can be found in this book's GitHub repository: https://github.com/
codeplea/Hands-On-Network-Programming-with-C.
From the command line, you can download the code for this chapter with the following
command:
git clone https://github.com/codeplea/Hands-On-Network-Programming-with-C
cd Hands-On-Network-Programming-with-C/chap04
Each example program in this chapter runs on Windows, Linux, and macOS. While
compiling on Windows, each example program requires being linked with the Winsock
library. This can be accomplished by passing the -lws2_32 option to gcc.
We provide the exact commands that are needed to compile each example as they are
introduced.
All of the example programs in this chapter require the same header files and C macros that
we developed in Chapter 2, Getting to Grips with Socket APIs. For brevity, we put these
statements in a separate header file, chap04.h, which we can include in each program. For
an explanation of these statements, please refer to Chapter 2, Getting to Grips with Socket
APIs.
The content of chap04.h is shown in the following code:
#if defined(_WIN32)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#endif
#if defined(_WIN32)
#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
Establishing UDP Connections Chapter 4
[ 104 ]
#define CLOSESOCKET(s) closesocket(s)
#define GETSOCKETERRNO() (WSAGetLastError())
#else
#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int
#define GETSOCKETERRNO() (errno)
#endif
#include <stdio.h>
#include <string.h>
How UDP sockets differ
The socket API for UDP sockets is only very slightly different than what we've already
learned for TCP. In fact, they are similar enough that we can take the TCP client from the
last chapter and turn it into a fully functional UDP client by changing only one line of code:
1. Take tcp_client.c from Chapter 3, An In-Depth Overview of TCP Connections,
and find the following line of code:
hints.ai_socktype = SOCK_STREAM;
2. Change the preceding code to the following:
hints.ai_socktype = SOCK_DGRAM;
This modification is included in this chapter's code as udp_client.c.
You can recompile the program using the same commands as before, and you'll get a fully
functional UDP client.
Unfortunately, changing the TCP servers of the previous chapters to UDP won't be as easy.
TCP and UDP server code are different enough that a slightly different approach is needed.
Also, don't assume that because we had to change only one line of the code that the client
behaves exactly the same way – this won't happen. The two programs are using a different
protocol, after all.
Establishing UDP Connections Chapter 4
[ 105 ]
Remember from Chapter 2, Getting to Grips with Socket APIs that UDP does not try to be a
reliable protocol. Lost packets are not automatically re-transmitted, and packets may be
received in a different order than they were sent. It is even possible for one packet to
erroneously arrive twice! TCP attempts to solve all these problems, but UDP leaves you to
your own devices.
Do you know what the best thing about a UDP joke is? I don't care if you get it or not.
Despite UDP's (lack of) reliability, it is still appropriate for many applications. Let's look at
the methods that are used by UDP clients and servers.
UDP client methods
Sending data with TCP requires calling connect() to set the remote address and establish
the TCP connection. Thus, we use send() with TCP sockets, as shown in the following
code:
connect(tcp_socket, peer_address, peer_address_length);
send(tcp_socket, data, data_length, 0);
UDP is a connectionless protocol. Therefore, no connection is established before sending
data. A UDP connection is never established. With UDP, data is simply sent and received.
We can call connect() and then send(), as we mentioned previously, but the socket API
provides an easier way for UDP sockets in the form of the sendto() function. It works like
this:
sendto(udp_socket, data, data_length, 0,
 peer_address, peer_address_length);
connect() on a UDP socket works a bit differently. All connect() does with a UDP
socket is associate a remote address. Thus, while connect() on a TCP socket involves a
handshake for sending packets over the network, connect() on a UDP socket only stores
an address locally.
So, a UDP client can be structured in two different ways, depending on whether you use
connect(), send(), and recv(), or instead use sendto() and recvfrom().
Establishing UDP Connections Chapter 4
[ 106 ]
The following diagram compares the program flow of a TCP Client to a UDP Client using
either method:
Note that, while using connect(), the UDP Client only receives data from the peer having
the IP address and the port that is given to connect(). However, when not using
connect(), the recvfrom() function returns data from any peer that addresses us! Of
course, that peer would need to know our address and port. Unless we call bind(), our
local address and port is assigned automatically by the operating system.
UDP server methods
Programming a UDP server is a bit different than TCP. TCP requires managing a socket for
each peer connection. With UDP, our program only needs one socket. That one socket can
communicate with any number of peers.
Establishing UDP Connections Chapter 4
[ 107 ]
While the TCP program flow required us to use listen() and accept() to wait for and
establish new connections, these functions are not used with UDP. Our UDP server simply
binds to the local address, and then it can immediately start sending and receiving data.
The program flow of a TCP Server compared to a UDP Server is as follows:
Establishing UDP Connections Chapter 4
[ 108 ]
With either a TCP or UDP server, we use select() when we need to check/wait for
incoming data. The difference is that a TCP Server using select() is likely monitoring
many separate sockets, while a UDP Server often only needs to monitor one socket. If your
program uses both TCP and UDP sockets, you can monitor them all with only one call to
select().
A first UDP client/server
To drive these points home, it will be useful to work through a full UDP client and UDP
server program.
To keep things simple, we will create a UDP client program that simply sends the Hello
World string to 127.0.0.1 on port 8080. Our UDP server listens on 8080. It prints any
data it receives, along with the sender's address and port number.
We will begin by implementing the simple UDP server.
A simple UDP server
We will start with the server, since we already have a usable UDP client, that
is, udp_client.c.
Like all of our networked programs, we will begin by including the necessary headers,
starting with the main() function, and initializing Winsock as follows:
/*udp_recvfrom.c*/
#include "chap04.h"
int main() {
#if defined(_WIN32)
 WSADATA d;
 if (WSAStartup(MAKEWORD(2, 2), &d)) {
 fprintf(stderr, "Failed to initialize.\n");
 return 1;
 }
#endif
Establishing UDP Connections Chapter 4
[ 109 ]
If you've been working through this book in order, this code should be very routine for you
by now. If you haven't, then please refer to Chapter 2, Getting to Grips with Socket APIs.
Then, we must configure the local address that our server listens on. We use
getaddrinfo() for this, as follows:
/*udp_recvfrom.c continued*/
 printf("Configuring local address...\n");
 struct addrinfo hints;
 memset(&hints, 0, sizeof(hints));
 hints.ai_family = AF_INET;
 hints.ai_socktype = SOCK_DGRAM;
 hints.ai_flags = AI_PASSIVE;
 struct addrinfo *bind_address;
 getaddrinfo(0, "8080", &hints, &bind_address);
This differs only slightly from how we've done it before. Notably, we set
hints.ai_socktype = SOCK_DGRAM. Recall that SOCK_STREAM was used there for TCP
connections. We are still setting hints.ai_family = AF_INET here. This makes our
server listen for IPv4 connections. We could change that to AF_INET6 to make our server
listen for IPv6 connections instead.
After we have our local address information, we can create the socket, as follows:
/*udp_recvfrom.c continued*/
 printf("Creating socket...\n");
 SOCKET socket_listen;
 socket_listen = socket(bind_address->ai_family,
 bind_address->ai_socktype, bind_address->ai_protocol);
 if (!ISVALIDSOCKET(socket_listen)) {
 fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
 return 1;
 }
This code is exactly the same as in the TCP case. The call to socket() uses our address
information from getaddrinfo() to create the proper type of socket.
We must then bind the new socket to the local address that we got from getaddrinfo().
This is as follows:
/*udp_recvfrom.c continued*/
 printf("Binding socket to local address...\n");
 if (bind(socket_listen,
Establishing UDP Connections Chapter 4
[ 110 ]
 bind_address->ai_addr, bind_address->ai_addrlen)) {
 fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
 return 1;
 }
 freeaddrinfo(bind_address);
Again, that code is exactly the same as in the TCP case.
Here is where the UDP server diverges from the TCP server. Once the local address is
bound, we can simply start to receive data. There is no need to call listen() or accept().
We listen for incoming data using recvfrom(), as shown here:
/*udp_recvfrom.c continued*/
 struct sockaddr_storage client_address;
 socklen_t client_len = sizeof(client_address);
 char read[1024];
 int bytes_received = recvfrom(socket_listen,
 read, 1024,
 0,
 (struct sockaddr*) &client_address, &client_len);
In the previous code, we created a struct sockaddr_storage to store the client's
address. We also defined socklen_t client_len to hold the address size. This keeps our
code robust in the case that we change it from IPv4 to IPv6. Finally, we created a buffer,
char read[1024], to store incoming data.
recvfrom() is used in a similar manner to recv(), except that it returns the sender's
address, as well as the received data. You can think of recvfrom() as a combination of the
TCP server accept() and recv().
Once we've received data, we can print it out. Keep in mind that the data may not be null
terminated. It can be safely printed with the %.*s printf() format specifier, as shown in
the following code:
/*udp_recvfrom.c continued*/
 printf("Received (%d bytes): %.*s\n",
 bytes_received, bytes_received, read);
Establishing UDP Connections Chapter 4
[ 111 ]
It may also be useful to print the sender's address and port number. We can use the
getnameinfo() function to convert this data into a printable string, as shown in the
following code:
/*udp_recvfrom.c continued*/
 printf("Remote address is: ");
 char address_buffer[100];
 char service_buffer[100];
 getnameinfo(((struct sockaddr*)&client_address),
 client_len,
 address_buffer, sizeof(address_buffer),
 service_buffer, sizeof(service_buffer),
 NI_NUMERICHOST | NI_NUMERICSERV);
 printf("%s %s\n", address_buffer, service_buffer);
The last argument to getnameinfo() (NI_NUMERICHOST | NI_NUMERICSERV) tells
getnameinfo() that we want both the client address and port number to be in numeric
form. Without this, it would attempt to return a hostname or protocol name if the port
number matches a known protocol. If you do want a protocol name, pass in the NI_DGRAM
flag to tell getnameinfo() that you're working on a UDP port. This is important for the
few protocols that have different ports for TCP and UDP.
It's also worth noting that the client will rarely set its local port number explicitly. So, the
port number returned here by getnameinfo() is likely to be a high number that's chosen
randomly by the client's operating system. Even if the client did set its local port number,
the port number we can see here might have been changed by network address
translation (NAT).
In any case, if our server were to send data back, it would need to send it to the address and
port stored in client_address. This would be done by passing client_address to
sendto().
Once the data has been received, we'll end our simple UDP server by closing the
connection, cleaning up Winsock, and ending the program:
/*udp_recvfrom.c continued*/
 CLOSESOCKET(socket_listen);
#if defined(_WIN32)
 WSACleanup();
#endif
 printf("Finished.\n");
Establishing UDP Connections Chapter 4
[ 112 ]
 return 0;
}
You can compile and run udp_recvfrom.c on Linux and macOS by using the following
commands:
gcc udp_recvfrom.c -o udp_recvfrom
./udp_recvfrom
Compiling and running on Windows with MinGW is done as follows:
gcc udp_recvfrom.c -o udp_recvfrom.exe -lws2_32
udp_recvfrom.exe
While running, it simply waits for an incoming connection:
You could use udp_client to connect to udp_recvfrom for testing, or you can implement
udp_sendto, which we will do next.
A simple UDP client
Although we've already shown a fairly full-featured UDP client, udp_client.c, it is
worthwhile building a very simple UDP client. This client shows only the minimal required
steps to get a working UDP client, and it uses sendto() instead of send().
Establishing UDP Connections Chapter 4
[ 113 ]
Let's begin the same way we begin each program, by including the necessary headers,
starting main(), and initializing Winsock, as follows:
/*udp_sendto.c*/
#include "chap04.h"
int main() {
#if defined(_WIN32)
 WSADATA d;
 if (WSAStartup(MAKEWORD(2, 2), &d)) {
 fprintf(stderr, "Failed to initialize.\n");
 return 1;
 }
#endif
We then configure the remote address using getaddrinfo(). In this minimal example, we
use 127.0.0.1 as the remote address and 8080 as the remote port. This means that it
connects to the UDP server only if it's running on the same computer.
Here is how the remote address is configured:
/*udp_sendto.c continued*/
 printf("Configuring remote address...\n");
 struct addrinfo hints;
 memset(&hints, 0, sizeof(hints));
 hints.ai_socktype = SOCK_DGRAM;
 struct addrinfo *peer_address;
 if (getaddrinfo("127.0.0.1", "8080", &hints, &peer_address)) {
 fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
 return 1;
 }
Notice that we hardcoded 127.0.0.1 and 8080 into the call to getaddrinfo(). Also,
notice that we've set hints.ai_socktype = SOCK_DGRAM. This tells getaddrinfo() that
we are connecting over UDP. Notice that we did not set AF_INET or AF_INET6. This
allows getaddrinfo() to return the appropriate address for IPv4 or IPv6. In this case, it is
IPv4 because the address, 127.0.0.1, is an IPv4 address. We will cover getaddrinfo() in
more detail in Chapter 5, Hostname Resolution and DNS.
Establishing UDP Connections Chapter 4
[ 114 ]
We can print the configured address using getnameinfo(). The call to getnameinfo() is
the same as in the previous UDP server, udp_recvfrom.c. It works as follows:
/*udp_sendto.c continued*/
 printf("Remote address is: ");
 char address_buffer[100];
 char service_buffer[100];
 getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen,
 address_buffer, sizeof(address_buffer),
 service_buffer, sizeof(service_buffer),
 NI_NUMERICHOST | NI_NUMERICSERV);
 printf("%s %s\n", address_buffer, service_buffer);
Now that we've stored the remote address, we are ready to create our socket with a call to
socket(). We pass in fields from peer_address to create the appropriate socket type. The
code for this is as follows:
/*udp_sendto.c continued*/
 printf("Creating socket...\n");
 SOCKET socket_peer;
 socket_peer = socket(peer_address->ai_family,
 peer_address->ai_socktype, peer_address->ai_protocol);
 if (!ISVALIDSOCKET(socket_peer)) {
 fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
 return 1;
 }
Once the socket has been created, we can go straight to sending data with sendto(). There
is no need to call connect(). Here is the code to send Hello World to our UDP server:
/*udp_sendto.c continued*/
 const char *message = "Hello World";
 printf("Sending: %s\n", message);
 int bytes_sent = sendto(socket_peer,
 message, strlen(message),
 0,
 peer_address->ai_addr, peer_address->ai_addrlen);
 printf("Sent %d bytes.\n", bytes_sent);
Notice that sendto() is much like send(), except that we need to pass in an address as the
last parameter.
Establishing UDP Connections Chapter 4
[ 115 ]
It is also worth noting that we do not get an error back if sending fails. send() simply tries
to send a message, but if it gets lost or is misdelivered along the way, there is nothing we
can do about it. If the message is important, it is up to the application protocol to
implement the corrective action.
After we've sent our data, we could reuse the same socket to send data to another address
(as long as it's the same type of address, which is IPv4 in this case). We could also try to
receive a reply from the UDP server by calling recvfrom(). Note that if we did call
recvfrom() here, we could get data from anybody that sends to us – not necessarily the
server we just transmitted to.
When we sent our data, our socket was assigned with a temporary local port number by the
operating system. This local port number is called the ephemeral port number. From then
on, our socket is essentially listening for a reply on this local port. If the local port is
important, you can use bind() to associate a specific port before calling send().
If multiple applications on the same system are connecting to a remote server at the same
port, the operating system uses the local ephemeral port number to keep replies separate.
Without this, it wouldn't be possible to know which application should receive which
reply.
We'll end our example program by freeing the memory for peer_address, closing the
socket, cleaning up Winsock, and finishing main(), as follows:
/*udp_sendto.c continued*/
 freeaddrinfo(peer_address);
 CLOSESOCKET(socket_peer);
#if defined(_WIN32)
 WSACleanup();
#endif
 printf("Finished.\n");
 return 0;
}
You can compile udp_sendto.c on Linux and macOS using the following command:
gcc udp_sendto.c -o udp_sendto
Establishing UDP Connections Chapter 4
[ 116 ]
Compiling on Windows with MinGW is done in the following way:
gcc udp_sendto.c -o udp_sendto.exe -lws2_32
To test it out, first, start udp_recvfrom in a separate terminal. With udp_recvfrom already
running, you can start udp_sendto. It should look as follows:
If no server is running on port 8080, udp_sendto still produces the same output.
udp_sendto doesn't know that the packet was not delivered.
Establishing UDP Connections Chapter 4
[ 117 ]
A UDP server
It will be useful to look at a UDP server that's been designed to service many connections.
Fortunately for us, the UDP socket API makes this very easy.
We will take the motivating example from our last chapter, which was to provide a service
that converts all text into uppercase. This is useful because you can directly compare the
UDP code from here to the TCP server code from Chapter 3, An In-Depth Overview of TCP
Connections.
Our server begins by setting up the socket and binding to our local address. It then waits to
receive data. Once it has received a data string, it converts the string into all uppercase and
sends it back.
The program flow looks as follows:
Establishing UDP Connections Chapter 4
[ 118 ]
If you compare the flow of this program to the TCP server from the last chapter (Chapter 3,
An In-Depth Overview of TCP Connections), you will find that it's much simpler. With TCP,
we had to use listen() and accept(). With UDP, we skip those calls and go straight into
receiving data with recvfrom(). With our TCP server, we had to monitor a listening
socket for new connections while simultaneously monitoring an additional socket for each
connected client. Our UDP server only uses one socket, so there is much less to keep track
of.
Our UDP server program begins by including the necessary headers, starting the main()
function, and initializing Winsock, as follows:
/*udp_serve_toupper.c*/
#include "chap04.h"
#include <ctype.h>
int main() {
#if defined(_WIN32)
 WSADATA d;
 if (WSAStartup(MAKEWORD(2, 2), &d)) {
 fprintf(stderr, "Failed to initialize.\n");
 return 1;
 }
#endif
We then find our local address that we should listen on, create the socket, and bind to it.
This is all exactly the same as in our earlier server, udp_recvfrom.c. The only difference
between this code and the TCP servers in Chapter 3, An In-Depth Overview of TCP
Connections, is that we use SOCK_DGRAM instead of SOCK_STREAM. Recall that SOCK_DGRAM
specifies that we want a UDP socket.
Here is the code for setting the address and creating a new socket:
/*udp_serve_toupper.c continued*/
 printf("Configuring local address...\n");
 struct addrinfo hints;
 memset(&hints, 0, sizeof(hints));
 hints.ai_family = AF_INET;
 hints.ai_socktype = SOCK_DGRAM;
 hints.ai_flags = AI_PASSIVE;
 struct addrinfo *bind_address;
 getaddrinfo(0, "8080", &hints, &bind_address);
Establishing UDP Connections Chapter 4
[ 119 ]
 printf("Creating socket...\n");
 SOCKET socket_listen;
 socket_listen = socket(bind_address->ai_family,
 bind_address->ai_socktype, bind_address->ai_protocol);
 if (!ISVALIDSOCKET(socket_listen)) {
 fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
 return 1;
 }
Binding the new socket to the local address is done as follows:
/*udp_serve_toupper.c continued*/
 printf("Binding socket to local address...\n");
 if (bind(socket_listen,
 bind_address->ai_addr, bind_address->ai_addrlen)) {
 fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
 return 1;
 }
 freeaddrinfo(bind_address);
Because our server uses select(), we need to create a new fd_set to store our listening
socket. We zero the set using FD_ZERO(), and then add our socket to this set using
FD_SET(). We also maintain the maximum socket in the set using max_socket:
/*udp_serve_toupper.c continued*/
 fd_set master;
 FD_ZERO(&master);
 FD_SET(socket_listen, &master);
 SOCKET max_socket = socket_listen;
 printf("Waiting for connections...\n");
Note that we don't really have to use select() for this program, and omitting it would
make the program simpler (see udp_server_toupper_simple.c). However, we are going
to use select() because it makes our code more flexible. We could easily add in an
additional socket (if we needed to listen on multiple ports, for example), and we could add
in a select() timeout if our program needs to perform other functions. Of course, our
program doesn't do those things, so we don't need select(), but I think that most
programs do, so we will show it that way.
Establishing UDP Connections Chapter 4
[ 120 ]
Now, we are ready for the main loop. It copies the socket set into a new variable, reads,
and then uses select() to wait until our socket is ready to read from. Recall that we could
pass in a timeout value as the last parameter to select() if we want to set a maximum
waiting time for the next read. Refer to Chapter 3, An In-Depth Overview of TCP Connections,
the Synchronous multiplexing with select() section, for more information on select().
Once select() returns, we use FD_ISSET() to tell if our particular socket,
socket_listen, is ready to be read from. If we had additional sockets, we would need to
use FD_ISSET() for each socket.
If FD_ISSET() returns true, we read from the socket using recvfrom(). recvfrom()
gives us the sender's address, so we must first allocate a variable to hold the address, that
is, client_address. Once we've read a string from the socket using recvfrom(), we
convert the string into uppercase using the C toupper() function. We then send the
modified text back to the sender using sendto(). Note that the last two parameters to
sendto() are the client's addresses that we got from recvfrom().
The main program loop can be seen in the following code:
/*udp_serve_toupper.c continued*/
 while(1) {
 fd_set reads;
 reads = master;
 if (select(max_socket+1, &reads, 0, 0, 0) < 0) {
 fprintf(stderr, "select() failed. (%d)\n", GETSOCKETERRNO());
 return 1;
 }
 if (FD_ISSET(socket_listen, &reads)) {
 struct sockaddr_storage client_address;
 socklen_t client_len = sizeof(client_address);
 char read[1024];
 int bytes_received = recvfrom(socket_listen, read, 1024, 0,
 (struct sockaddr *)&client_address, &client_len);
 if (bytes_received < 1) {
 fprintf(stderr, "connection closed. (%d)\n",
 GETSOCKETERRNO());
 return 1;
 }
 int j;
 for (j = 0; j < bytes_received; ++j)
 read[j] = toupper(read[j]);
Establishing UDP Connections Chapter 4
[ 121 ]
 sendto(socket_listen, read, bytes_received, 0,
 (struct sockaddr*)&client_address, client_len);
 } //if FD_ISSET
 } //while(1)
We can then close the socket, clean up Winsock, and terminate the program. Note that this
code never runs, because the main loop never terminates. We include this code anyway as
good practice; in case the program is adapted in the future to have an exit function.
The cleanup code is as follows:
/*udp_serve_toupper.c continued*/
 printf("Closing listening socket...\n");
 CLOSESOCKET(socket_listen);
#if defined(_WIN32)
 WSACleanup();
#endif
 printf("Finished.\n");
 return 0;
}
That's our complete UDP server program. You can compile and run it on Linux and macOS
as follows:
gcc udp_serve_toupper.c -o udp_serve_toupper
./udp_serve_toupper
Compiling and running on Windows with MinGW is done in the following manner:
gcc udp_serve_toupper.c -o udp_serve_toupper.exe -lws2_32
udp_serve_toupper.exe
You can abort the program's execution with Ctrl + C.
Once the program is running, you should open another terminal window and run the
udp_client program from earlier to connect to it, as follows:
udp_client 127.0.0.1 8080
Establishing UDP Connections Chapter 4
[ 122 ]
Anything you type in udp_client should be sent back to it in uppercase. Here's what that
might look like:
You may also want to try opening additional terminal windows and connecting with
udp_client.
See udp_serve_toupper_simple.c for an implementation that doesn't use select(),
but manages to work just as well anyway.
Establishing UDP Connections Chapter 4
[ 123 ]
Summary
In this chapter, we saw that programming with UDP sockets is somewhat easier than with
TCP sockets. We learned that UDP sockets don't need the listen(), accept(), or
connect() function calls. This is mostly because sendto() and recvfrom() deal with the
addresses directly. For more complicated programs, we can still use the select() function
to see which sockets are ready for I/O.
We also saw that UDP sockets are connectionless. This is in contrast to connection-oriented
TCP sockets. With TCP, we had to establish a connection before sending data, but with
UDP, we simply send individual packets directly to a destination address. This keeps UDP
socket programming simple, but it can complicate application protocol design, and UDP
does not automatically retry communication failures or ensure that packets arrive in order.
The next chapter, Chapter 5, Hostname Resolution and DNS, is all about hostnames.
Hostnames are resolved using the DNS protocol, which works over UDP. Move on
to Chapter 5, Hostname Resolution and DNS, to learn about implementing a real-world UDP
protocol.
Questions
Try answering these questions to test your knowledge of this chapter:
1. How do sendto() and recvfrom() differ from send() and recv()?
2. Can send() and recv() be used on UDP sockets?
3. What does connect() do in the case of a UDP socket?
4. What makes multiplexing with UDP easier than with TCP?
5. What are the downsides to UDP when compared to TCP?
6. Can the same program use UDP and TCP?
The answers can be found in Appendix A, Answers to Questions.







7. Building a Simple Web Server

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Our server's main loop can then be simplified. It waits for new connections or new data.
When new data is received, it checks whether the data consists of a complete HTTP request.
If a complete HTTP request is received, the server attempts to send the requested resource.
If the HTTP request is malformed or the resource cannot be found, then the server sends an
error message to the connected client instead.

If a file's media type is unknown, then our server should use application/octetstream as a default. This indicates that the browser should treat the content as an unknown binary blob.


Consider a client that sends only the beginning of an HTTP request, followed by a delay,
and then the remainder of the HTTP request. In this case, we cannot respond to that client
until the entire HTTP request is received. However, at the same time, we do not wish to
delay servicing other connected clients while waiting. For this reason, we need to buffer up
received data for each client separately. Only once we've received an entire HTTP request
from a client can we respond to that client.
It is useful to define a C struct to store information on each connected client. Our
program uses the following:


This can be done by
passing around the linked list root as a separate argument to each function call. This
chapter's code repository includes an example of this alternative technique in
the web_server2.c file.


void drop_client(struct client_info *client) {
 CLOSESOCKET(client->socket);
 struct client_info **p = &clients;
 while(*p) {
 if (*p == client) {
 *p = client->next;
 free(client);
 return;
 }
 p = &(*p)->next;
 }

 fprintf(stderr, "drop_client not found.\n");
 exit(1);
}

This pointer-to-pointer variable is useful because we can use it to change the value of clients
directly. Indeed, if the client to be removed is the first element in the linked list, then
clients needs to be updated, so that clients points to the second element in the list.
The code uses a while loop to walk through the linked list. Once it finds that *p ==
client, *p, is set to client->next, which effectively removes the client from the linked
list, the allocated memory is then freed and the function returns.



get_client_address()
It is useful to have a helper function that converts a given client's IP address into text. This
function is given in the following code snippet:
/*web_server.c except*/
const char *get_client_address(struct client_info *ci) {
 static char address_buffer[100];
 getnameinfo((struct sockaddr*)&ci->address,
 ci->address_length,
 address_buffer, sizeof(address_buffer), 0, 0,
 NI_NUMERICHOST);
 return address_buffer;
}
get_client_address() is a simple function. It first allocates a char array to store the IP
address in. This char array is declared static, which ensures that its memory is available
after the function returns. This means that we don't need to worry about having the caller
free() the memory. The downside to this method is that get_client_address() has a
global state and is not re-entrant-safe. See web_server2.c for an alternative version that is
re-entrant-safe.
After a char buffer is available, the code simply uses getnameinfo() to convert the binary
IP address into a text address; getnameinfo() was covered in detail in previous chapters,
but Chapter 5, Hostname Resolution and DNS, has a particularly detailed explanation.
wait_on_clients()
Our server is capable of handling many simultaneous connections. This means that our
server must have a way to wait for data from multiple clients at once. We define a function,
wait_on_clients(), which blocks until an existing client sends data, or a new client
attempts to connect. This function uses select() as described in previous
chapters. Chapter 3, An In-Depth Overview of TCP Connections, has a detailed explanation of
select().
The wait_on_clients() function is defined as follows:
/*web_server.c except*/
fd_set wait_on_clients(SOCKET server) {
 fd_set reads;
 FD_ZERO(&reads);
Building a Simple Web Server Chapter 7
[ 208 ]
 FD_SET(server, &reads);
 SOCKET max_socket = server;
 struct client_info *ci = clients;
 while(ci) {
 FD_SET(ci->socket, &reads);
 if (ci->socket > max_socket)
 max_socket = ci->socket;
 ci = ci->next;
 }
 if (select(max_socket+1, &reads, 0, 0, 0) < 0) {
 fprintf(stderr, "select() failed. (%d)\n", GETSOCKETERRNO());
 exit(1);
 }
 return reads;
}
In the preceding code, first a new fd_set is declared and zeroed-out. The server socket is
then added to the fd_set first. Then the code loops through the linked list of connected
clients and adds the socket for each one in turn. A variable, max_socket, is maintained
throughout this process to store the maximum socket number as required by select().
After all the sockets are added to fd_set reads, the code calls select(),
and select() returns when one or more of the sockets in reads is ready.
The wait_on_clients() function returns reads so that the caller can see which socket is
ready.
send_400()
In the case where a client sends an HTTP request which our server does not understand, it
is helpful to send a code 400 error. Because errors of this type can arise in several
situations, we wrap this functionality in the send_400() function. The entire function
follows:
/*web_server.c except*/
void send_400(struct client_info *client) {
 const char *c400 = "HTTP/1.1 400 Bad Request\r\n"
 "Connection: close\r\n"
 "Content-Length: 11\r\n\r\nBad Request";
 send(client->socket, c400, strlen(c400), 0);
Building a Simple Web Server Chapter 7
[ 209 ]
 drop_client(client);
}
The send_400() function first declares a text array with the entire HTTP response hardcoded. This text is sent using the send() function, and then the client is dropped by calling
the drop_client() function we defined earlier.
send_404()
In addition to the 400 Bad Request error, our server also needs to handle the case where
a requested resource is not found. In this case, a 404 Not Found error should be returned.
We define a helper function to return this error as follows:
/*web_server.c except*/
void send_404(struct client_info *client) {
 const char *c404 = "HTTP/1.1 404 Not Found\r\n"
 "Connection: close\r\n"
 "Content-Length: 9\r\n\r\nNot Found";
 send(client->socket, c404, strlen(c404), 0);
 drop_client(client);
}
The send_404() function works exactly like the send_400() function defined previously.
serve_resource()
The serve_resource() function sends a connected client a requested resource. Our server
expects all hosted files to be in a subdirectory called public. Ideally, our server should not
allow access to any files outside of this public directory. However, as we shall see,
enforcing this restriction may be more difficult than it first appears.
Our serve_resource() function takes as arguments a connected client and a requested
resource path. The function begins as follows:
/*web_server.c except*/
void serve_resource(struct client_info *client, const char *path) {
 printf("serve_resource %s %s\n", get_client_address(client), path);
Building a Simple Web Server Chapter 7
[ 210 ]
The connected client's IP address and the requested path are printed to aid in debugging. In
a production server, you would also want to print additional information. Most production
servers log the date, time, request method, the client's user-agent string, and the response
code as a minimum.
Our function then normalizes the requested path. There are a few things to check for. First,
if the path is /, then we need to serve a default file. There is a tradition of serving a file
called index in that case, and, indeed, this is what our code does.
We also check that the path isn't too long. Once we ensure that the path is below a
maximum length, we can use fixed-size arrays to store it without worrying about buffer
overflows.
Our code also checks that the path doesn't contain two consecutive dots—... In file paths,
two dots indicate a reference to a parent directory. However, for security reasons, we want
to allow access only into our public directory. We do not want to provide access to any
parent directory. If we allowed paths with .., then a malicious client could send GET
/../web_server.c HTTP/1.1 and gain access to our server source code!
The following code is used to redirect root requests and to prevent long or obviously
malicious requests:
/*web_server.c except*/
 if (strcmp(path, "/") == 0) path = "/index.html";
 if (strlen(path) > 100) {
 send_400(client);
 return;
 }
 if (strstr(path, "..")) {
 send_404(client);
 return;
 }
Our code now needs to convert the path to refer to files in the public directory. This is
done with the sprintf() function. First, a text-array is reserved, full_path, and then
sprintf() is used to store the full path into it. We are able to reserve a fixed allocation for
full_path, as our earlier code ensured that path does not exceed 100 characters in length.
Building a Simple Web Server Chapter 7
[ 211 ]
The code to set full_path is as follows:
/*web_server.c except*/
 char full_path[128];
 sprintf(full_path, "public%s", path);
It is important to note that the directory separator differs between Windows and other
operating systems. While Unix-based systems use a slash (/), Windows instead uses a
backslash (\) as its standard. Many Windows functions handle the conversion
automatically, but the difference is sometimes important. For our simple server, the slash
conversion isn't an absolute requirement. However, we include it anyway as a good
practice.
The following code converts slashes to backslashes on Windows:
/*web_server.c except*/
#if defined(_WIN32)
 char *p = full_path;
 while (*p) {
 if (*p == '/') *p = '\\';
 ++p;
 }
#endif
The preceding code works by stepping through the full_path text array and detecting
slash characters. When a slash is found, it is simply overwritten with a backslash. Note that
the C code '\\' is equivalent to only one backslash. This is because the backslash has
special meaning in C, and therefore the first backslash is used to escape the second
backslash.
At this point, our server can check whether the requested resource actually exists. This is
done by using the fopen() function. If fopen() fails, for any reason, then our server
assumes that the file does not exist. The following code sends a 404 error if the requested
resource isn't available:
/*web_server.c except*/
 FILE *fp = fopen(full_path, "rb");
 if (!fp) {
 send_404(client);
 return;
 }
Building a Simple Web Server Chapter 7
[ 212 ]
If fopen() succeeds, then we can use fseek() and ftell() to determine the requested
file's size. This is important, as we need to use the file's size in the Content-Length
header. The following code finds the file size and stores it in the cl variable:
/*web_server.c except*/
 fseek(fp, 0L, SEEK_END);
 size_t cl = ftell(fp);
 rewind(fp);
Once the file size is known, we also want to get the file's type. This is used in the ContentType header. We already defined a function, get_content_type(), which makes this task
easy. The content type is store in the variable ct by the following code:
/*web_server.c except*/
 const char *ct = get_content_type(full_path);
Once the file has been located and we have its length and type, the server can begin
sending the HTTP response. We first reserve a temporary buffer to store header fields in:
/*web_server.c except*/
#define BSIZE 1024
 char buffer[BSIZE];
Once the buffer is reserved, the server prints relevant headers into it and then sends those
headers to the client. This is done using sprintf() and then send() in turn. The
following code sends the HTTP response header:
/*web_server.c except*/
 sprintf(buffer, "HTTP/1.1 200 OK\r\n");
 send(client->socket, buffer, strlen(buffer), 0);
 sprintf(buffer, "Connection: close\r\n");
 send(client->socket, buffer, strlen(buffer), 0);
 sprintf(buffer, "Content-Length: %u\r\n", cl);
 send(client->socket, buffer, strlen(buffer), 0);
 sprintf(buffer, "Content-Type: %s\r\n", ct);
 send(client->socket, buffer, strlen(buffer), 0);
 sprintf(buffer, "\r\n");
 send(client->socket, buffer, strlen(buffer), 0);
Building a Simple Web Server Chapter 7
[ 213 ]
Note that the last send() statement sends \r\n. This has the effect of transmitting a blank
line. This blank line is used by the client to delineate the HTTP header from the beginning
of the HTTP body.
The server can now send the actual file content. This is done by calling fread() repeatedly
until the entire file is sent:
/*web_server.c except*/
 int r = fread(buffer, 1, BSIZE, fp);
 while (r) {
 send(client->socket, buffer, r, 0);
 r = fread(buffer, 1, BSIZE, fp);
 }
In the preceding code, fread() is used to read enough data to fill buffer. This buffer is
then transmitted to the client using send(). These steps are looped until fread() returns
0; this indicates that the entire file has been read.
Note that send() may block on large files. In a truly robust, production-ready server, you
would need to handle this case. It could be done by using select() to determine when
each socket is ready to read. Another common method is to use fork() or similar APIs to
create separate threads/processes for each connected client. For simplicity, our server
accepts the limitation that send() blocks on large files. Please refer to Chapter 13, Socket
Programming Tips and Pitfalls, for more information about the blocking behavior of send().
The function can finish by closing the file handle and using drop_client() to disconnect
the client:
/*web_server.c except*/
 fclose(fp);
 drop_client(client);
}
That concludes the serve_resource() function.
Keep in mind that while serve_resource() attempts to limit access to only the public
directory, it is not adequate in doing so, and serve_resource() should not be used in
production code without carefully considering additional access loopholes. We discuss
more security concerns later in this chapter.
With these helper functions out of the way, implementing our main server loop is a much
easier task. We begin that next.
Building a Simple Web Server Chapter 7
[ 214 ]
The main loop
With our many helper functions out of the way, we can now finish web_server.c.
Remember to first #include chap07.h and also add in all of the types and functions
we've defined so far—struct client_info, get_content_type(), create_socket(),
get_client(), drop_client(), get_client_address(), wait_on_clients(),
send_400(), send_404(), and serve_resource().
We can then begin the main() function. It starts by initializing Winsock on Windows:
/*web_server.c except*/
int main() {
#if defined(_WIN32)
 WSADATA d;
 if (WSAStartup(MAKEWORD(2, 2), &d)) {
 fprintf(stderr, "Failed to initialize.\n");
 return 1;
 }
#endif
We then use our earlier function, create_socket(), to create the listening socket. Our
server listens on port 8080, but feel free to change it. On Unix-based systems, listening on
low port numbers is reserved for privileged accounts. For security reasons, our web server
should be running with unprivileged accounts only. This is why we use 8080 as our port
number instead of the HTTP standard port, 80.
The code to create the server socket is as follows:
/*web_server.c except*/
 SOCKET server = create_socket(0, "8080");
If you want to accept connections from only the local system, and not outside systems, use
the following code instead:
/*web_server.c except*/
 SOCKET server = create_socket("127.0.0.1", "8080");
Building a Simple Web Server Chapter 7
[ 215 ]
We then begin an endless loop that waits on clients. We call wait_on_clients() to wait
until a new client connects or an old client sends new data:
/*web_server.c except*/
 while(1) {
 fd_set reads;
 reads = wait_on_clients(server);
The server then detects whether a new client has connected. This case is indicated by
server being set in fd_set reads. We use the FD_ISSET() macro to detect this
condition:
/*web_server.c except*/
 if (FD_ISSET(server, &reads)) {
 struct client_info *client = get_client(-1);
 client->socket = accept(server,
 (struct sockaddr*) &(client->address),
 &(client->address_length));
 if (!ISVALIDSOCKET(client->socket)) {
 fprintf(stderr, "accept() failed. (%d)\n",
 GETSOCKETERRNO());
 return 1;
 }
 printf("New connection from %s.\n",
 get_client_address(client));
 }
Once a new client connection has been detected, get_client() is called with the
argument -1; -1 is not a valid socket specifier, so get_client() creates a new struct
client_info. This struct client_info is assigned to the client variable.
The accept() socket function is used to accept the new connection and place the
connected clients address information into the respective client fields. The new socket
returned by accept() is stored in client->socket.
The client's address is printed using a call to get_client_address(). This is helpful for
debugging.
Building a Simple Web Server Chapter 7
[ 216 ]
Our server must then handle the case where an already connected client is sending data.
This is a bit more complicated. We first walk through the linked list of clients and use
FD_ISSET() on each client to determine which clients have data available. Recall that the
linked list root is stored in the clients global variable.
We begin our linked list walk with the following:
/*web_server.c except*/
 struct client_info *client = clients;
 while(client) {
 struct client_info *next = client->next;
 if (FD_ISSET(client->socket, &reads)) {
We then check that we have memory available to store more received data for client. If
the client's buffer is already completely full, then we send a 400 error. The following code
checks for this condition:
/*web_server.c except*/
 if (MAX_REQUEST_SIZE == client->received) {
 send_400(client);
 continue;
 }
Knowing that we have at least some memory left to store received data, we can use recv()
to store the client's data. The following code uses recv() to write new data into the client's
buffer while being careful to not overflow that buffer:
/*web_server.c except*/
 int r = recv(client->socket,
 client->request + client->received,
 MAX_REQUEST_SIZE - client->received, 0);
A client that disconnects unexpectedly causes recv() to return a non-positive number. In
this case, we need to use drop_client() to clean up our memory allocated for that client:
/*web_server.c except*/
 if (r < 1) {
 printf("Unexpected disconnect from %s.\n",
 get_client_address(client));
 drop_client(client);
Building a Simple Web Server Chapter 7
[ 217 ]
If the received data was written successfully, our server adds a null terminating character
to the end of that client's data buffer. This allows us to use strstr() to search the buffer,
as the null terminator tells strstr() when to stop.
Recall that the HTTP header and body is delineated by a blank line. Therefore, if strstr()
finds a blank line (\r\n\r\n), we know that the HTTP header has been received and we
can begin to parse it. The following code detects whether the HTTP header has been
received:
/*web_server.c except*/
 } else {
 client->received += r;
 client->request[client->received] = 0;
 char *q = strstr(client->request, "\r\n\r\n");
 if (q) {
Our server only handles GET requests. We also enforce that any valid path should start with
a slash character; strncmp() is used to detect these two conditions in the following code:
/*web_server.c except*/
 if (strncmp("GET /", client->request, 5)) {
 send_400(client);
 } else {
 char *path = client->request + 4;
 char *end_path = strstr(path, " ");
 if (!end_path) {
 send_400(client);
 } else {
 *end_path = 0;
 serve_resource(client, path);
 }
 }
 } //if (q)
In the preceding code, a proper GET request causes the execution of the else branch. Here,
we set the path variable to the beginning of the request path, which is starting at the fifth
character of the HTTP request (because C arrays start at zero, the fifth character is located at
client->request + 4).
The end of the requested path is indicated by finding the next space character. If found, we
just call our serve_resource() function to fulfil the client's request.
Building a Simple Web Server Chapter 7
[ 218 ]
Our server is basically functional at this point. We only need to finish our loops and close
out the main() function. The following code accomplishes this:
/*web_server.c except*/
 }
 }
 client = next;
 }
 } //while(1)
 printf("\nClosing socket...\n");
 CLOSESOCKET(server);
#if defined(_WIN32)
 WSACleanup();
#endif
 printf("Finished.\n");
 return 0;
}
Note that our server doesn't actually have a way to break from its infinite loop. It simply
listens to connections forever. As an exercise, you may want to add in functionality that
allows the server to shut down cleanly. This was omitted only to keep the code simpler. It
may also be useful to drop all connected clients with this line of code—while(clients)
drop_client(clients);
That concludes the code for web_server.c. I recommend you download web_server.c
from this book's code repository and try it out.
You can compile and run web_server.c on Linux and macOS with the following
commands:
gcc web_server.c -o web_server
./web_server
On Windows, the command to compile and run using MinGW is as follows:
gcc web_server.c -o web_server.exe -lws2_32
web_server.exe
Building a Simple Web Server Chapter 7
[ 219 ]
The following screenshot shows the server being compiled and run on macOS:
If you connect to the server using a standard web browser, you should see something such
as the following screenshot:
Building a Simple Web Server Chapter 7
[ 220 ]
You can also drop different files into the public folder and play around with creating
more complicated websites.
An alternative source file, web_server2.c, is also provided in this chapter's code
repository. It behaves exactly like the code we developed, but it avoids having global state
(at the expense of a little added verbosity). This may make web_server2.c more suitable
for integration into more significant projects and continued development.
Although the web server we developed certainly works, it does have a number of
shortcomings. Please don't deploy this server (or any other network code) in the wild
without very carefully considering these shortcomings, some of which we address next.
Security and robustness
One of the most important rules, when developing networked code, is that your program
should never trust the connected peer. Your code should never assume that the connected
peer sends data in a particular format. This is especially vital for server code that may
communicate with multiple clients at once.
If your code doesn't carefully check for errors and unexpected conditions, then it will be
vulnerable to exploits.
Consider the following code which receives data into a buffer until a space character is
found:
char buffer[1028] = {0};
char *p = buffer;
while (!strstr(p, " "))
 p += recv(client, p, 1028, 0);
The preceding code works simply. It reserves 1,028 bytes of buffer space and then uses
recv() to write received data into that space. The p pointer is updated on each read to
indicate where the next data should be written. The code then loops until the strstr()
function detects a space character.
That code could be useful to read data from a client until an HTTP verb is detected. For
example, it could receive data until GET is received, at which point the server can begin to
process a GET request.
Building a Simple Web Server Chapter 7
[ 221 ]
One problem with the preceding code is that recv() could write past the end of the
allocated space for buffer. This is because 1028 is passed to recv(), even if some data has
already been written. If a network client can cause your code to write past the end of a
buffer, then that client may be able to completely compromise your server. This is because
both data and executable code are stored in your server's memory. A malicious client may
be able to write executable code past the buffer array and cause your program to execute
it. Even if the malicious code isn't executed, the client could still overwrite other important
data in your server's memory.
The preceding code can be fixed by passing to recv() only the amount of buffer space
remaining:
char buffer[1028] = {0};
char *p = buffer;
while (!strstr(p, " "))
 p += recv(client, p, 1028 - (p - buffer), 0);
In this case, recv() is not be able to write more than 1,028 bytes total into buffer. You
may think that the memory errors are resolved, but you would still be wrong. Consider a
client that sends 1,028 bytes, but no space characters. Your code then calls strstr()
looking for a space character. Considering that buffer is completely full now,
strstr() cannot find a space character or a null terminating character! In that
case, strstr() continues to read past the end of buffer into unallocated memory.
So, you fix this issue by only allowing recv() to write 1,027 bytes total. This reserves one
byte to remain as the null terminating character:
char buffer[1028] = {0};
char *p = buffer;
while (!strstr(p, " "))
 p += recv(client, p, 1027 - (p - buffer), 0);
Now your code won't write or read past the array bounds for buffer, but the code is still
very broken. Consider a client that sends 1,027 characters. Or consider a client that sends a
single null character. In either case, the preceding code continues to loop forever, thus
locking up your server and preventing other clients from being served.
Hopefully, the previous examples illustrate the care needed to implement a server in C.
Indeed, it's easy to create bugs in any programming language, but in C special care needs to
be taken to avoid memory errors.
Building a Simple Web Server Chapter 7
[ 222 ]
Another issue with server software is that the server wants to allow access to some files on
the system, but not others. A malicious client could send an HTTP request that tries to
download arbitrary files from your server system. For example, if an HTTP request such as
GET /../secret_file.c HTTP/1.1 was sent to a naive HTTP server, that server may
send the secret_file.c to the connected client, even though it exists outside of the
public directory!
Our code in web_server.c detects the most obvious attempts at this by searching for
requests containing .. and denying those requests.
A robust server should use operating systems features to detect that requested files exist as
actual files in the permitted directory. Unfortunately, there is no cross-platform way to do
this, and the platform-dependent options are somewhat complicated.
Please understand that these are not purely theoretical concerns, but actual exploitable
bugs. For example, if you run our web_server.c program on Windows and a client sends
the request GET /this_will_be_funny/PRN HTTP/1.1, what do you suppose happens?
The this_will_be_funny directory doesn't exist, and the PRN file certainly doesn't exist
in that non-existent directory. These facts may lead you to think that the server simply
returns a 404 Not Found error, as expected. However, that's not what happens. Under
Windows, PRN is a special filename. When your server calls fopen() on this special name,
Windows doesn't look for a file, but rather it connects to a printer interface! Other special
names include COM1 (connects to serial port 1) and LPT1 (connects to parallel port 1),
although there are others. Even if these filenames have an extension, such as PRN.txt,
Windows still redirects instead of looking for a file.
One generally applicable piece of security advice is this—run your networked programs
under non-privileged accounts that have access to only the minimum resources needed to
function. In other words, if you are going to run a networked server, create a new account
to run it under. Give that account read access to only the files that server needs to serve.
This is not a substitute for writing secure code, but rather running as a non-privilege user
creates one final barrier. It is advice you should apply even when running hardened,
industry-tested server software.
Hopefully, the previous examples illustrate that programming is complicated, and safe
network programming in C can be difficult. It is best approached with care. Oftentimes, it is
not possible to know that you have all the loopholes covered. Operating systems don't
always have adequate documentation. Operating system APIs often behave in non-obvious
and non-intuitive ways. Be careful.
Building a Simple Web Server Chapter 7
[ 223 ]
Open source servers
The code developed in this chapter is suitable for use in trusted applications on trusted
networks. For example, if you are developing a video game, it can be very useful to make it
serve a web page that displays debugging information. This doesn't have to be a security
concern, as it can limit connections to the local machine.
If you must deploy a web server on the internet, I suggest you consider using a free and
open source implementation that's already available. The web servers Nginx and Apache,
for example, are highly performant, cross-platform, secure, written in C, and completely
free. They are also well-documented and easy to find support for.
If you want to expose your program to the internet, you can communicate to a web server
using either CGI or FastCGI. With CGI, the web server handles the HTTP request. When a
request comes in, it runs your program and returns your program's output in the HTTP
response body.
Alternatively, many web servers (such as Nginx or Apache) work as a reverse proxy. This
essentially puts the web server between your code and the internet. The web server accepts
and forwards HTTP messages to your HTTP server. This can have the effect of slightly
shielding your code from attackers.
Summary
In this chapter, we worked through implementing an HTTP server in C from scratch. That's
no small feat! Although the text-based nature of HTTP makes parsing HTTP requests
simple, we needed to spend a lot of effort to ensure that multiple clients could be served
simultaneously. We accomplished this by buffering received data for each client separately.
Each client's state information was organized into a linked list.
Another difficulty was ensuring the safe handling of received data and detecting errors. We
learned that a programmer must be very careful when handling network data to avoid
creating security risks. We also saw that even very subtle issues, such as Windows's special
filenames, can potentially create dangerous security holes for networked server
applications.
In the next chapter, Chapter 8, Making Your Program Send Email, we move on from HTTP
and consider the primary protocol associated with email—Simple Mail Transfer Protocol
(SMTP).









