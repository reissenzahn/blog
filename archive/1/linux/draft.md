
# OSI Model

The Open Systems Interconnection Model consists of seven layers:

1. Physical: Physical communication.
2. Data link: Protocols for directly communicating between two nodes.
3. Network layer: Methods to transmit packets between nodes in different networks.
4. Transport layer: Reliable methods for delivering variable length data between hosts.
5. Session layer: Provides methods to establish, checkpoint, suspend, resume and terminate dialogs.
6. Presentation layer: Data encoding, serialization, encryption, etc.
7. Application layer: Applications that the user interfaces with.

Chunks of data are often referred to by different names depending on the OSI layer they're on. A data unit on layer 2 is called a frame, since layer 2 is responsible for framing messages. A data unit on layer 3 is referred to as a packet, while a data unit on layer 4 is a segment if it is part of a TCP connection or a datagram if it is a UDP message.


# TCP/IP Layer Model

The TCP/IP protocol suite is the most common network communication model in use today.

The four layers of the TCP/IP model are as follows:

1. Network access Layer: Physical connections and data framing.
2. Internet layer: Addressing packets and routing them over interconnected networks.
3. Host-to-Host layer: Data order, data segmentation, network congestion and error correction.
4. Application layer: Higher-level protocols like HTTP, FTP and SMTP.

The advantage of these abstractions is that, when programming an application, we only need to consider the highest-level protocol.


# Internet Protocol

The Internet Protocol comes in two versions: IPv4 and IPv6.

IPv4 uses 32-bit addresses, which limits it to addressing no more than 2^32 systems. However, these addresses were not initially assigned efficiently, and now many Internet Service Providers (ISPs) are forced to ration IPv4 addresses.

IPv6 was designed to replace IPv4 and has been standardized by the Internet Engineering Task Force (IETF) since 1998. It uses a 128-bit address, which allows it to address a theoretical 2^128 addresses.

Today, every major desktop and smartphone operating system supports both IPv4 and IPv6 in what is called a dual-stack configuration. However, many applications, servers, and networks are still only configured to use IPv4. From a practical standpoint, this means that you need to support IPv4 in order to access much of the internet.


# Addresses

All Internet Protocol traffic routes to an address.

IPv4 addresses are 32 bits long. They are commonly divided into four 8-bit sections. Each section is displayed as a decimal number between 0 and 255 inclusive and is delineated by a period.

Here are some examples of IPv4 addresses:

0.0.0.0
127.0.0.1
10.0.0.0
172.16.0.5
192.168.0.1
192.168.50.1
255.255.255.255

A special address, called the loopback address, is reserved at 127.0.0.1. This address essentially means establish a connection to myself. Operating systems short-circuit this address so that packets to it never enter the network but instead stay local on the originating system.

IPv4 reserves some address ranges for private use. If you're using IPv4 through a router/NAT, then you are likely using an IP address in one of these ranges. These reserved private ranges are as follows:

10.0.0.0 to 10.255.255.255
172.16.0.0 to 172.31.255.255
192.168.0.0 to 192.168.255.255

Using Classless Inter-Domain Routing (CIDR) notation, we can write the three previous address ranges as follows:

10.0.0.0/8
172.16.0.0/12
192.168.0.0/16

CIDR notation works by specifying the number of bits that are fixed. For example, 10.0.0.0/8 specifies that the first eight bits of the 10.0.0.0 address are fixed, the first eight bits being just the first 10. part; the remaining 0.0.0 part of the address can be anything and still be on the 10.0.0.0/8 block. Therefore, 10.0.0.0/8 encompasses 10.0.0.0 through 10.255.255.255.

IPv6 addresses are 128 bits long. They are written as eight groups of four hexadecimal characters delineated by colons. A hexadecimal character can be from 0-9 or from a-f. Here are some examples of IPv6 addresses:

0000:0000:0000:0000:0000:0000:0000:0001
2001:0db8:0000:0000:0000:ff00:0042:8329
fe80:0000:0000:0000:75f4:ac69:5fa7:67f9
ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff

There are a couple of rules for shortening IPv6 addresses to make them easier. Rule 1 allows for the leading zeros in each section to be omitted (for example, 0db8 = db8). Rule 2 allows for consecutive sections of zeros to be replaced with a double colon (::). Rule 2 may only be used once in each address; otherwise, the address would be ambiguous.

Applying both rules, the preceding addresses can be shortened as follows:

::1
2001:db8::ff00:42:8329
fe80::75f4:ac69:5fa7:67f9
ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff

Like IPv4, IPv6 also has a loopback address. It is ::1.

Dual-stack implementations also recognize a special class of IPv6 address that map directly to an IPv4 address. These reserved addresses start with 80 zero bits, and then by 16 one bits, followed by the 32-bit IPv4 address. Using CIDR notation, this block of address is ::ffff:0:0/96.

These mapped addresses are commonly written with the first 96 bits in IPv6 format followed by the remaining 32 bits in IPv4 format. Here are some examples:

10.0.0.0     -> ::ffff:10.0.0.0
172.16.0.5   -> ::ffff:172.16.0.5
192.168.0.1  -> ::ffff:192.168.0.1
192.168.50.1 -> ::ffff:192.168.50.1


# Domain Names

The Internet Protocol can only route packets to an IP address, not a name. So, if you try to connect to a website, such as example.com, your system must first resolve that domain name, example.com, into an IP address for the server that hosts that website.

This is done by connecting to a Domain Name System (DNS) server. You connect to a domain name server by knowing in advance its IP address. The IP address for a domain name server is usually assigned by your ISP.

To resolve a hostname, your computer sends a UDP message to your domain name server and asks it for an AAAA-type record for the domain you're trying to resolve. If this record exists, an IPv6 address is returned. You can then connect to a server at that address to load the website. If no AAAA record exists, then your computer queries the server again, but asks for an A record. If this record exists, you will receive an IPv4 address for the server. In many cases, a site will publish an A record and an AAAA record that route to the same server.

It is also possible, and common, for multiple records of the same type to exist, each pointing to a different address. This is useful for redundancy in the case where multiple servers can provide the same service.


# Internet Routing

When you make a connection over the internet, your data first transmits to your local router. From there, it is transmitted to another router, which is connected to another router, and so on. Eventually, your data reaches a router that is connected to the receiving device, at which point, the data has reached its destination.


# Local Networks and Address Translation

It's common for households and organizations to have small Local Area Networks (LANs). There are IPv4 addresses ranges reserved for use in these small local networks.

These reserved private ranges are as follows:

10.0.0.0 to 10.255.255.255
172.16.0.0 to 172.31.255.255
192.168.0.0 to 192.168.255.255

When a packet originates from a device on an IPv4 local network, it must undergo Network Address Translation (NAT) before being routed on the internet. A router that implements NAT remembers which local address a connection is established from.

The devices on the same LAN can directly address one another by their local address. However, any traffic communicated to the internet must undergo address translation by the router. The router does this by modifying the source IP address from the original private LAN IP address to its public internet IP address:

Likewise, when the router receives the return communication, it must modify the destination address from its public IP to the private IP of the original sender. It knows the private IP address because it was stored in memory after the first outgoing packet.

Network address translation can be more complicated than it first appears. In addition to modifying the source IP address in the packet, it must also update the checksums in the packet. Otherwise, the packet would be detected as containing errors and discarded by the next router. The NAT router must also remember which private IP address sent the packet in order to route the reply. Without remembering the translation address, the NAT router wouldn't know where to send the reply to on the private network.


# Subnetting

IP addresses can be split into parts. The most significant bits are used to identify the network or subnetwork, and the least significant bits are used to identify the specific device on the network.

IPv4 traditionally uses a mask notation to identify the IP address parts. For example, consider a router on the 10.0.0.0 network with a subnet mask of 255.255.255.0. This router can take any incoming packet and perform a bitwise AND operation with the subnet mask to determine whether the packet belongs on the local subnet or needs to be forwarded on.

For example, this router receives a packet to be delivered to 10.0.0.105. It does a bitwise AND operation on this address with the subnet mask of 255.255.255.0, which produces 10.0.0.0. That matches the subnet of the router, so the traffic is local. If, instead, we consider a packet destined for 10.0.15.22, the result of the bitwise AND with the subnet mask is 10.0.15.0. This address doesn't match the subnet the router is on, and so it must be forwarded.

IPv6 uses CIDR. Networks and subnetworks are specified using CIDR notation. For example, if the IPv6 subnet is /112, then the router knows that any address that matches on the first 112 bits is on the local subnet.


# Multicast, Broadcast, and Anycast

When a packet is routed from one sender to one receiver, it uses unicast addressing. This is the simplest and most common type of addressing.

Broadcast addressing allows a single sender to address a packet to all recipients simultaneously. It is typically used to deliver a packet to every receiver on an entire subnet.

If a broadcast is a one-to-all communication, then multicast is a one-to-many
communication. Multicast involves some group management, and a message is addressed
and delivered to members of a group.
Anycast addressed packets are used to deliver a message to one recipient when you don't
care who that recipient is. This is useful if you have several servers that provide the same
functionality, and you simply want one of them (you don't care which) to handle your
request.
IPv4 and lower network levels support local broadcast addressing. IPv4 provides some
optional (but commonly implemented) support for multicasting. IPv6 mandates
multicasting support while providing additional features over IPv4's multicasting. Though
IPv6 is not considered to broadcast, its multicasting functionality can essentially emulate it.
It's worth noting that these alternative addressing methods don't generally work over the
broader internet. Imagine if one peer was able to broadcast a packet to every connected
internet device. It would be a mess!
If you can use IP multicasting on your local network, though, it is worthwhile to implement
it. Sending one IP level multicast conserves bandwidth compared to sending the same
unicast message multiple times.
However, multicasting is often done at the application level. That is, when the application
wants to deliver the same message to several recipients, it sends the message multiple times
– once to each recipient.


# Port Numbers

An IP address alone isn't quite enough. We need port numbers.

Generally, an IP address gets a packet routed to a specific system, but a port number is used to route the packet to a specific application on that system.

Port numbers are stored as unsigned 16-bit integers. This means that they are between 0 and 65,535 inclusive.



# Sockets

A socket is one end-point of a communication link between systems. It's an abstraction in which your application can send and receive data over the network.

An open socket is uniquely defined by a 5-tuple consisting of the following:

- Local IP address
- Local port
- Remote IP address
- Remote port
- Protocol (UDP or TCP)

This 5-tuple is important, as it is how your operating system knows which application is responsible for any packets received.

A socket is one endpoint of a communication link between systems. Your application sends and receives all of its network data through a socket.

There are a few different socket application programming interfaces (APIs). The first were Berkeley sockets, which were released in 1983 with 4.3BSD Unix. The Berkeley socket API was widely successful and quickly evolved into a de facto standard. From there, it was adopted as a POSIX standard with little modification. The terms Berkeley sockets, BSD sockets, Unix sockets, and Portable Operating System Interface (POSIX) sockets are often used interchangeably.

Sockets come in two basic types: connection-oriented and connectionless. The two protocols that are used today are Transmission Control Protocol (TCP) and User Datagram Protocol (UDP). TCP is a connection-oriented protocol, and UDP is a connectionless protocol.

In a connectionless protocol, such as UDP, each data packet is addressed individually. From the protocol's perspective, each data packet is completely independent and unrelated to any packets coming before or after it.

UDP makes no guarantee that a packet will arrive. UDP doesn't generally provide a method to know if a packet did not arrive, and UDP does not guarantee that the packets will arrive in the same order they were sent. A single packet may even arrive twice.

TCP guarantees that data arrives in the same order it is sent. It prevents duplicate data from arriving twice, and it retries sending missing data. It also provides additional features such as notifications when a connection is terminated and algorithms to mitigate network congestion.

TCP is used by many protocols. HTTP (for severing web pages), FTP (for transferring files), SSH (for remote administration), and SMTP (for delivering email) all use TCP.

UDP is used by DNS (for resolving domain names). It is suitable for this purpose because an entire request and response can fit in a single packet. UDP is also commonly used in real-time applications, such as audio streaming, video streaming, and multiplayer video games.

If the guarantees that TCP provides are not needed, then UDP can achieve greater efficiency. This is because TCP adds some additional overhead by numbering packets. TCP must also delay packets that arrive out of order, which can cause unnecessary delays in real-time applications.

In UNIX, a socket descriptor is represented by a standard file descriptor. Additionally, in UNIX, all file descriptors (and therefore socket descriptors) are small, nonnegative integers.

---

Sockets are a way to speak to other programs using standard Unix file descriptors.

There are two main types of Internet Sockets. One is Stream Sockets; the other is Datagram Sockets, which may hereafter be referred to as SOCK_STREAM and SOCK_DGRAM, respectively. Datagram sockets are sometimes called connectionless sockets.

Stream sockets are reliable two-way connected communication streams. If you output two items into the socket in the order 1, 2, they will arrive in the order 1, 2 at the opposite end. They will also be error free.

Stream sockets use a protocol called The Transmission Control Protocol (TCP). TCP makes sure your data arrives sequentially and error-free. You may have heard TCP before as the better half of TCP/IP where IP stands for Internet Protocol. IP deals primarily with Internet routing and is not generally responsible for data integrity.

Datagram sockets also use IP for routing, but they don’t use TCP; they use the User Datagram Protocol (UDP).

Why are they connectionless? Well, basically, it’s because you don’t have to maintain an open connection
as you do with stream sockets. You just build a packet, slap an IP header on it with destination information,
and send it out. No connection needed. They are generally used either when a TCP stack is unavailable
or when a few dropped packets here and there don’t mean the end of the Universe.

Why would you use an unreliable underlying protocol? Two reasons: speed and speed. It’s way faster
to fire-and-forget than it is to keep track of what has arrived safely and make sure it’s in order and all
that.


# Socket Functions

- `socket()` creates and initializes a new socket.
- `bind()` associates a socket with a particular local IP address and port number.
- `listen()` is used on the server to cause a TCP socket to listen for new connections.
- `connect()` is used on the client to set the remote address and port. In the case of TCP, it also establishes a connection.
- `accept()` is used on the server to create a new socket for an incoming TCP connection.
- `send()` and `recv()` are used to send and receive data with a socket.
- `sendto()` and `recvfrom()` are used to send and receive data from sockets without a bound remote address.
- `close()` are used to close a socket. In the case of TCP, this also terminates the connection.
- `shutdown()` is used to close one side of a TCP connection. It is useful to ensure an orderly connection teardown.
select() is used to wait for an event on one or more sockets.
- `getnameinfo()` and `getaddrinfo()` provide a protocol-independent manner of working with hostnames and addresses.
- `setsockopt()` is used to change some socket options.
- `fcntl()` are also used to get and set some socket options.

connect() on a UDP socket works a bit differently. All connect() does with a UDP
socket is associate a remote address. Thus, while connect() on a TCP socket involves a
handshake for sending packets over the network, connect() on a UDP socket only stores
an address locally.


# Anatomy of a Socket Program

Network programming is usually done using a client-server paradigm. In this paradigm, a server listens for new connections at a published address. The client, knowing the server's address, is the one to establish the connection initially. Once the connection is established, the client and the server can both send and receive data. This can continue until either the client or the server terminates the connection.

An alternative paradigm is the peer-to-peer model. In the peer-to-peer model, each peer has essentially the same responsibilities. A peer-to-peer protocol is balanced in that data is exchanged somewhat evenly between peers. However, even in the peer-to-peer model, the underlying sockets that are using TCP or UDP aren't created equal. That is, for each peer-to-peer connection, one peer was listening and the other connecting.


# TCP Program Flow

A TCP client program must first know the TCP server's address. This is often input by a user.

The TCP client takes this address (for example, http://example.com) and uses the getaddrinfo() function to resolve it into a struct addrinfo structure.

The client then creates a socket using a call to socket().

The client then establishes the new TCP connection by calling connect().

At this point, the client can freely exchange data using send() and recv().

A TCP server listens for connections at a particular port number on a particular interface.

The program must first initialize a struct addrinfo structure with the proper listening IP address and port number.

The getaddrinfo() function is helpful so that you can do this in an IPv4/IPv6 independent way.

The server then creates the socket with a call to socket().

The socket must be bound to the listening IP address and port.This is accomplished with a call to bind().

The server program then calls listen(), which puts the socket in a state where it listens for new connections.

The server can then call accept(), which will wait until a client establishes a connection to the server. When the new connection has been established, accept() returns a new socket.

This new socket can be used to exchange data with the client using send() and recv().

Meanwhile, the first socket remains listening for new connections, and repeated calls to accept() allow the server to handle multiple clients.




# UDP Program Flow

A UDP client must know the address of the remote UDP peer in order to send the first
packet. The UDP client uses the getaddrinfo() function to resolve the address into a
struct addrinfo structure. Once this is done, the client creates a socket of the proper
type. The client can then call sendto() on the socket to send the first packet. The client can
continue to call sendto() and recvfrom() on the socket to send and receive additional
packets. Note that the client must send the first packet with sendto(). The UDP client
cannot receive data first, as the remote peer would have no way of knowing where to send
data without it first receiving data from the client. This is different from TCP, where a
connection is first established with a handshake. In TCP, either the client or server can send
the first application data.
A UDP server listens for connections from a UDP client. This server should
initialize struct addrinfo structure with the proper listening IP address and port
number. The getaddrinfo() function can be used to do this in a protocol-independent
way. The server then creates a new socket with socket() and binds it to the listening IP
address and port number using bind(). At this point, the server can call recvfrom(),
which causes it to block until it receives data from a UDP client. After the first data is
received, the server can reply with sendto() or listen for more data (from the first client or
any new client) with recvfrom().

With UDP, our program only needs one socket. That one socket can
communicate with any number of peers.

While the TCP program flow required us to use listen() and accept() to wait for and
establish new connections, these functions are not used with UDP. Our UDP server simply
binds to the local address, and then it can immediately start sending and receiving data.

When we sent our data with sendto(), our socket was assigned with a temporary local port number by the
operating system. This local port number is called the ephemeral port number. From then
on, our socket is essentially listening for a reply on this local port. If the local port is
important, you can use bind() to associate a specific port before calling send().


# Multiplexing TCP Connections

The socket APIs are blocking by default. When you use accept() to wait for an incoming
connection, your program's execution is blocked until a new incoming connection is
actually available. When you use recv() to read incoming data, your program's execution
blocks until new data is actually available.

Blocking on recv() like this isn't really acceptable. A real application usually needs to be
able to manage several connections simultaneously. This is obviously true on the server
side, as most servers are built to manage many connected clients. Imagine running a
website where hundreds of clients are connected at once. Serving these clients one at a time
would be a non-starter.

Blocking also isn't usually acceptable on the client side either. If you imagine building a fast
web browser, it needs to be able to download many images, scripts, and other resources in
parallel.


## Polling Non-Blocking Sockets

It is possible to configure sockets to use a non-blocking operation. One way to do this is by
calling fcntl() with the O_NONBLOCK flag (ioctlsocket() with the FIONBIO flag on
Windows), although other ways also exist. Once in non-blocking mode, a call to recv()
with no data will return immediately.

A program structured with this in mind could simply check each of its active sockets in
turn, continuously. It would handle any socket that returned data and ignore any socket
that didn't. This is called polling. Polling can be a waste of computer resources since most
of the time, there will be no data to read. It also complicates the program somewhat, as the
programmer is required to manually track which sockets are active and which state, they
are in. Return values from recv() must also be handled differently than with blocking
sockets.


## Forking and Multi-Threading

Another possible solution to multiplexing socket connections is to start a new thread or
process for each connection. In this case, blocking sockets are fine, as they block only their
servicing thread/process, and they do not block other threads/processes. This can be a
useful technique, but it also has some downsides. First of all, threading is tricky to get right.
This is especially true if the connections must share any state between them. It is also less
portable as each operating system provides a different API for these features.

Using multiple processes/threads is much more complicated on Windows. Windows
provides CreateProcess(), CreateThread(), and many other functions for these
features. However—and I can say this objectively—they are all much harder to use than
Unix's fork().

Debugging these multi-process/thread programs can be much more difficult compared to
the single process case. Communicating between sockets and managing shared state is also
much more burdensome.

## The select() Function

Our preferred technique for multiplexing is to use the select() function. We can give
select() a set of sockets, and it tells us which ones are ready to be read. It can also tell us
which sockets are ready to write to and which sockets have exceptions.













# HTTP

Hypertext Transfer Protocol (HTTP) is the application protocol that powers the World Wide Web (WWW).

HTTP is a text-based client-server protocol that runs over TCP. Plain HTTP runs over TCP port 80.

It should be noted that plain HTTP is mostly deprecated for security reasons. Today, sites should use HTTPS, the secure version of HTTP. HTTPS secures HTTP by merely running the HTTP protocol through a Transport Layer Security (TLS) layer.

HTTP works by first having the web client send an HTTP request to the web server. Then, the web server responds with an HTTP response. Generally, the HTTP request indicates which resource the client is interested in, and the HTTP response delivers the requested resource.


## Request Types

- GET is used when the client wants to download a resource.
- HEAD is just like GET, except that the client only wants information about the resource instead of the resource itself. For example, if the client only wants to know the size of a hosted file, it could send a HEAD request.
- POST is used when the client needs to send information to the server. Your web browser typically uses a POST request when you submit an online form, for example. A POST request will typically cause a web server to change its state somehow. A web server could send an email, update a database, or change a file in response to a POST request.
- PUT is used to send a document to the web server.
- DELETE is used to request that the web server should delete a document or resource.
- TRACE is used to request diagnostic information from web proxies.
- CONNECT is sometimes used to initiate an HTTP connection through a proxy server.
- OPTIONS is used to ask which HTTP request types are supported by the server for a given resource. A typical web server that implements OPTIONS may respond with something similar to Allow: OPTIONS, GET, HEAD, POST.

If you send a request that the web server doesn't support, then the server should respond with a 400 Bad Request code.


## Request Format

If you open your web browser and navigate to http://www.example.com/page1.htm, your browser will need to send an HTTP request to the web server at www.example.com. That HTTP request may look like this:

```
GET /page1.htm HTTP/1.1
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36
Accept-Language: en-US
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
Accept-Encoding: gzip, deflate
Host: example.com
Connection: Keep-Alive
```

This GET request is asking the server for the document /page1.htm. A GET request consists of HTTP headers only. There is no HTTP body because the client isn't sending data to the server. The client is only requesting data from the server. In contrast, a POST request would contain an HTTP body.

The first line of an HTTP request is called the request line. The request line consists of three parts – the request type, the document path, and the protocol version. Each part is separated by a space. In the preceding example, the request line is GET /page1.htm HTTP/1.1. We can see that the request type is GET, the document path is /page1.htm, and the protocol version is HTTP/1.1.

When dealing with text-based network protocols, it is always important to be explicit about line endings. This is because different operating systems have standardized on different line-ending conventions. Each line of an HTTP message ends with a carriage return, followed by a newline character. In C, this looks like \r\n.

After the request line, there are various HTTP header fields. Each header field consists of its
name followed by a colon, and then its value. Consider the User-Agent: Mozilla/5.0
(Windows NT 10.0; Win64; x64) AppleWebKit/537.36 line. This User-Agent line is
telling the web server what software is contacting it. Some web servers will serve different
documents to different user agents. For example, it is common for some websites to serve
full documents to search engine spiders while serving paywalls to actual visitors. The
server generally uses the user-agent HTTP header field to determine which is which. At the
same time, there is a long history of web clients lying in the user-agent field. I suggest you
take the high road in your applications and clearly identify your application with a unique
user-agent value.
The only header field that is actually required is Host. The Host field tells the web server
which web host the client is requesting the resource from. This is important because one
web server may be hosting many different websites. The request line tells the web server
that the /page1.htm document is wanted, but it doesn't specify which server that page is
on. The Host field fills this role.
The Connection: Keep-Alive line tells the web server that the HTTP client would like to
issue additional requests after the current request finishes. If the client had sent
Connection: Close instead, that would indicate that the client intended to close the TCP
connection after the HTTP response was received.
The web client must send a blank line after the HTTP request header. This blank line is how
the web server knows that the HTTP request is finished. Without this blank line, the web
server wouldn't know whether any additional header fields were still going to being sent.
In C, the blank line looks like this: \r\n\r\n.


## Response Format












## Uniform Resource Locators

Uniform Resource Locators (URL), also known as web addresses, provide a convenient way to specify particular web resources.

Consider the http://www.example.com:80/res/page1.php?user=bob#account URL.

The URL can indicate the protocol, the host, the port number, the document path, and hash. However, the host is the only required part. The other parts can be implied.

- http://: The part before the first :// indicates the protocol. In this example, the protocol is http, but it could be a different protocol such as ftp:// or https://. If the protocol is omitted, the application will generally make an assumption. For example, your web browser would assume the protocol to be http.
- www.example.com: This specifies the hostname. It is used to resolve an IP address that the HTTP client can connect to. This hostname must also appear in the HTTP request Host header field. This is required since multiple hostnames can resolve to the same IP address. This part can also be an IP address instead of a name. IPv4 addresses are used directly (http://192.168.50.1/), but IPv6 addresses should be put inside square brackets (http://[::1]/).
- :80: The port number can be specified explicitly by using a colon after the hostname. If the port number is not specified, then the client uses the default port number for the given protocol. The default port number for http is 80, and the default port number for https is 443. Non-standard port numbers are common for testing and development.
- /res/page1.php?user/bob: This specifies the document path. The HTTP server usually makes a distinction between the part before and after the question mark, but the HTTP client should not assign significance to this. The part after the question mark is often called the query string.
- #account: This is called the hash. The hash specifies a position within a document, and the hash is not sent to the HTTP server. It instead allows a browser to scroll to a particular part of a document after the entire document is received from the HTTP server.




