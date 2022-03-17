


// the DatagramPacket class represents a UDP datagram


This class provides methods to get and set the source or destination address from the
IP header, to get and set the source or destination port, to get and set the data, and to
get and set the length of the data.



// different constructors are used depending on whether the packet will be used to send data or to receive data

// all six constructors take as arguments a byte array that holds the datagram’s data and the number of bytes in that array to use for the datagram’s data


When you want to
receive a datagram, these are the only arguments you provide. When the socket receives
a datagram from the network, it stores the datagram’s data in the DatagramPacket
object’s buffer array, up to the length you specified.

The second set of DatagramPacket constructors is used to create datagrams you will
send over the network. Like the first, these constructors require a buffer array and a
length, but they also require an address and port to which the packet will be sent. In this
case, you pass to the constructor a byte array containing the data you want to send and
the destination address and port to which the packet is to be sent. The DatagramSock
et reads the destination address and port from the packet; the address and port aren’t
stored within the socket, as they are in TCP.
Constructors for receiving datagrams



public DatagramPacket(byte[] buffer, int length)
public DatagramPacket(byte[] buffer, int offset, int length)


If the first constructor is used, when a socket receives a datagram, it stores the datagram’s
data part in buffer beginning at buffer[0] and continuing until the packet is com‐
pletely stored or until length bytes have been written into the buffer. For example, this
code fragment creates a new DatagramPacket for receiving a datagram of up to 8,192
bytes:


byte[] buffer = new byte[8192];
DatagramPacket dp = new DatagramPacket(buffer, buffer.length);
If the second constructor is used, storage begins at buffer[offset] instead. Otherwise,
these two constructors are identical. length must be less than or equal to buffer.length
- offset. If you try to construct a DatagramPacket with a length that will overflow the
buffer, the constructor throws an IllegalArgumentException. This is a RuntimeEx
ception, so your code is not required to catch it. It is OK to construct a DatagramPack
et with a length less than buffer.length - offset. In this case, at most the first length
bytes of buffer will be filled when the datagram is received.
The constructor doesn’t care how large the buffer is and would happily let you create a
DatagramPacket with megabytes of data. However, the underlying native network soft‐
The DatagramPacket Class | 401
ware is less forgiving, and most native UDP implementations don’t support more than
8,192 bytes of data per datagram. The theoretical limit for an IPv4 datagram is 65,507
bytes of data, and a DatagramPacket with a 65,507-byte buffer can receive any possible
IPv4 datagram without losing data. IPv6 datagrams raise the theoretical limit to 65,536
bytes. In practice, however, many UDP-based protocols such as DNS and TFTP use
packets with 512 bytes of data per datagram or fewer. The largest data size in common
usage is 8,192 bytes for NFS. Almost all UDP datagrams you’re likely to encounter will
have 8K of data or fewer. In fact, many operating systems don’t support UDP datagrams
with more than 8K of data and either truncate, split, or discard larger datagrams. If a
large datagram is too big and as a result the network truncates or drops it, your Java
program won’t be notified of the problem. Consequently, you shouldn’t create Data
gramPacket objects with more than 8,192 bytes of data.
Constructors for sending datagrams
These four constructors create new DatagramPacket objects used to send data across
the network:
public DatagramPacket(byte[] data, int length,
 InetAddress destination, int port)
public DatagramPacket(byte[] data, int offset, int length,
 InetAddress destination, int port)
public DatagramPacket(byte[] data, int length,
 SocketAddress destination)
public DatagramPacket(byte[] data, int offset, int length,
 SocketAddress destination)
Each constructor creates a new DatagramPacket to be sent to another host. The packet
is filled with length bytes of the data array starting at offset or 0 if offset is not used.
If you try to construct a DatagramPacket with a length that is greater than data.length
(or greater than data.length - offset), the constructor throws an IllegalArgumen
tException. It’s OK to construct a DatagramPacket object with an offset and a length
that will leave extra, unused space at the end of the data array. In this case, only length
bytes of data will be sent over the network. The InetAddress or SocketAddress object
destination points to the host you want the packet delivered to; the int argument port
is the port on that host.
Choosing a Datagram Size
The correct amount of data to stuff into one packet depends on the situation. Some
protocols dictate the size of the packet. For example, rlogin transmits each character to
the remote system almost as soon as the user types it. Therefore, packets tend to be short:
a single byte of data, plus a few bytes of headers. Other applications aren’t so picky. For
example, file transfer is more efficient with large buffers; the only requirement is that
you split files into packets no larger than the maximum allowable packet size.
402 | Chapter 12: UDP
Several factors are involved in choosing the optimal packet size. If the network is highly
unreliable, such as a packet radio network, smaller packets are preferable because they’re
less likely to be corrupted in transit. On the other hand, very fast and reliable LANs
should use the largest packet size possible. Eight kilobytes—that is, 8,192 bytes—is a
good compromise for many types of networks.
It’s customary to convert the data to a byte array and place it in data before creating the
DatagramPacket, but it’s not absolutely necessary. Changing data after the datagram
has been constructed and before it has been sent changes the data in the datagram; the
data isn’t copied into a private buffer. In some applications, you can take advantage of
this. For example, you could store data that changes over time in data and send out the
current datagram (with the most recent data) every minute. However, it’s more impor‐
tant to make sure that the data doesn’t change when you don’t want it to. This is especially
true if your program is multithreaded, and different threads may write into the data
buffer. If this is the case, copy the data into a temporary buffer before you construct the
DatagramPacket.
For instance, this code fragment creates a new DatagramPacket filled with the data “This
is a test” in UTF-8. The packet is directed at port 7 (the echo port) of www.ibiblio.org:
String s = "This is a test";
byte[] data = s.getBytes("UTF-8");
try {
 InetAddress ia = InetAddress.getByName("www.ibiblio.org");
 int port = 7;
 DatagramPacket dp = new DatagramPacket(data, data.length, ia, port);
 // send the packet...
} catch (IOException ex)
}
Most of the time, the hardest part of creating a new DatagramPacket is translating the
data into a byte array. Because this code fragment wants to send a string, it uses the
getBytes() method of java.lang.String. The java.io.ByteArrayOutputStream
class can also be very useful for preparing data for inclusion in datagrams.
The get Methods
DatagramPacket has six methods that retrieve different parts of a datagram: the actual
data plus several fields from its header. These methods are mostly used for datagrams
received from the network.
public InetAddress getAddress()
The getAddress() method returns an InetAddress object containing the address of
the remote host. If the datagram was received from the Internet, the address returned
The DatagramPacket Class | 403
is the address of the machine that sent it (the source address). On the other hand, if the
datagram was created locally to be sent to a remote machine, this method returns the
address of the host to which the datagram is addressed (the destination address). This
method is most commonly used to determine the address of the host that sent a UDP
datagram, so that the recipient can reply.
public int getPort()
The getPort() method returns an integer specifying the remote port. If this datagram
was received from the Internet, this is the port on the host that sent the packet. If the
datagram was created locally to be sent to a remote host, this is the port to which the
packet is addressed on the remote machine.
public SocketAddress getSocketAddress()
The getSocketAddress() method returns a SocketAddress object containing the IP
address and port of the remote host. As is the case for getInetAddress(), if the datagram
was received from the Internet, the address returned is the address of the machine that
sent it (the source address). On the other hand, if the datagram was created locally to
be sent to a remote machine, this method returns the address of the host to which the
datagram is addressed (the destination address). You typically invoke this method to
determine the address and port of the host that sent a UDP datagram before you reply.
The net effect is not noticeably different than calling getAddress() and getPort().
Also, if you’re using nonblocking I/O, the DatagramChannel class accepts a SocketAd
dress but not an InetAddress and port.
public byte[] getData()
The getData() method returns a byte array containing the data from the datagram. It’s
often necessary to convert the bytes into some other form of data before they’ll be useful
to your program. One way to do this is to change the byte array into a String. For
example, given a DatagramPacket dp received from the network, you can convert it to
a UTF-8 String like this:
String s = new String(dp.getData(), "UTF-8");
If the datagram does not contain text, converting it to Java data is more difficult. One
approach is to convert the byte array returned by getData() into a ByteArrayInput
Stream. For example:
InputStream in = new ByteArrayInputStream(packet.getData(),
 packet.getOffset(), packet.getLength());
You must specify the offset and the length when constructing the ByteArrayInput
Stream. Do not use the ByteArrayInputStream() constructor that takes only an array
as an argument. The array returned by packet.getData() probably has extra space in
it that was not filled with data from the network. This space will contain whatever
404 | Chapter 12: UDP
random values those components of the array had when the DatagramPacket was con‐
structed.
The ByteArrayInputStream can then be chained to a DataInputStream:
DataInputStream din = new DataInputStream(in);
The data can then be read using the DataInputStream’s readInt(), readLong(), read
Char(), and other methods. Of course, this assumes that the datagram’s sender uses the
same data formats as Java; it’s probably the case when the sender is written in Java, and
is often (though not necessarily) the case otherwise. (Most modern computers use the
same floating-point format as Java, and most network protocols specify two’s comple‐
ment integers in network byte order, which also matches Java’s formats.)
public int getLength()
The getLength() method returns the number of bytes of data in the datagram. This is
not necessarily the same as the length of the array returned by getData() (i.e., getDa
ta().length). The int returned by getLength() may be less than the length of the
array returned by getData().
public int getOffset()
This method simply returns the point in the array returned by getData() where the
data from the datagram begins.
Example 12-3 uses all the methods covered in this section to print the information in
the DatagramPacket. This example is a little artificial; because the program creates a
DatagramPacket, it already knows what’s in it. More often, you’ll use these methods on
a DatagramPacket received from the network, but that will have to wait for the intro‐
duction of the DatagramSocket class in the next section.
Example 12-3. Construct a DatagramPacket to receive data
import java.io.*;
import java.net.*;
public class DatagramExample {
 public static void main(String[] args) {
 String s = "This is a test.";
 try {
 byte[] data = s.getBytes("UTF-8");
 InetAddress ia = InetAddress.getByName("www.ibiblio.org");
 int port = 7;
 DatagramPacket dp
 = new DatagramPacket(data, data.length, ia, port);
 System.out.println("This packet is addressed to "
 + dp.getAddress() + " on port " + dp.getPort());
The DatagramPacket Class | 405
 System.out.println("There are " + dp.getLength()
 + " bytes of data in the packet");
 System.out.println(
 new String(dp.getData(), dp.getOffset(), dp.getLength(), "UTF-8"));
 } catch (UnknownHostException | UnsupportedEncodingException ex) {
 System.err.println(ex);
 }
 }
}
Here’s the output:
% java DatagramExample
This packet is addressed to www.ibiblio.org/152.2.254.81 on port 7
There are 15 bytes of data in the packet
This is a test.
The setter Methods
Most of the time, the six constructors are sufficient for creating datagrams. However,
Java also provides several methods for changing the data, remote address, and remote
port after the datagram has been created. These methods might be important in a sit‐
uation where the time to create and garbage collect new DatagramPacket objects is a
significant performance hit. In some situations, reusing objects can be significantly
faster than constructing new ones: for example, in a networked twitch game that sends
a datagram for every bullet fired or every centimeter of movement. However, you would
have to use a very speedy connection for the improvement to be noticeable relative to
the slowness of the network itself.
public void setData(byte[] data)
The setData() method changes the payload of the UDP datagram. You might use this
method if you are sending a large file (where large is defined as “bigger than can com‐
fortably fit in one datagram”) to a remote host. You could repeatedly send the same
DatagramPacket object, just changing the data each time.
public void setData(byte[] data, int offset, int length)
This overloaded variant of the setData() method provides an alternative approach to
sending a large quantity of data. Instead of sending lots of new arrays, you can put all
the data in one array and send it a piece at a time. For instance, this loop sends a large
array in 512-byte chunks:
int offset = 0;
DatagramPacket dp = new DatagramPacket(bigarray, offset, 512);
int bytesSent = 0;
while (bytesSent < bigarray.length) {
 socket.send(dp);
 bytesSent += dp.getLength();
 int bytesToSend = bigarray.length - bytesSent;
406 | Chapter 12: UDP
 int size = (bytesToSend > 512) ? 512 : bytesToSend;
 dp.setData(bigarray, bytesSent, size);
}
On the other hand, this strategy requires either a lot of confidence that the data will in
fact arrive or, alternatively, a disregard for the consequences of its not arriving. It’s rel‐
atively difficult to attach sequence numbers or other reliability tags to individual packets
when you take this approach.
public void setAddress(InetAddress remote)
The setAddress() method changes the address a datagram packet is sent to. This might
allow you to send the same datagram to many different recipients. For example:
String s = "Really Important Message";
byte[] data = s.getBytes("UTF-8");
DatagramPacket dp = new DatagramPacket(data, data.length);
dp.setPort(2000);
int network = "128.238.5.";
for (int host = 1; host < 255; host++) {
 try {
 InetAddress remote = InetAddress.getByName(network + host);
 dp.setAddress(remote);
 socket.send(dp);
 } catch (IOException ex) {
 // skip it; continue with the next host
 }
}
Whether this is a sensible choice depends on the application. If you’re trying to send to
all the stations on a network segment, as in this fragment, you’d probably be better off
using the local broadcast address and letting the network do the work. The local broad‐
cast address is determined by setting all bits of the IP address after the network and
subnet IDs to 1. For example, Polytechnic University’s network address is 128.238.0.0.
Consequently, its broadcast address is 128.238.255.255. Sending a datagram to
128.238.255.255 copies it to every host on that network (although some routers and
firewalls may block it, depending on its origin).
For more widely separated hosts, you’re probably better off using multicasting. Multi‐
casting actually uses the same DatagramPacket class described here. However, it uses
different IP addresses and a MulticastSocket instead of a DatagramSocket. We’ll dis‐
cuss this further in Chapter 13.
public void setPort(int port)
The setPort() method changes the port a datagram is addressed to. I honestly can’t
think of many uses for this method. It could be used in a port scanner application that
tried to find open ports running particular UDP-based services such as FSP. Another
possibility might be some sort of networked game or conferencing server where the
clients that need to receive the same information are all running on different ports as
The DatagramPacket Class | 407
well as different hosts. In this case, setPort() could be used in conjunction with se
tAddress() to change destinations before sending the same datagram out again.
public void setAddress(SocketAddress remote)
The setSocketAddress() method changes the address and port a datagram packet is
sent to. You can use this when replying. For example, this code fragment receives a
datagram packet and responds to the same address with a packet containing the string
“Hello there”:
DatagramPacket input = new DatagramPacket(new byte[8192], 8192);
socket.receive(input);
DatagramPacket output = new DatagramPacket(
 "Hello there".getBytes("UTF-8"), 11);
SocketAddress address = input.getSocketAddress();
output.setAddress(address);
socket.send(output);
You could certainly write the same code using InetAddress objects and ports instead
of a SocketAddress. The code would be just a few lines longer.
public void setLength(int length)
The setLength() method changes the number of bytes of data in the internal buffer
that are considered to be part of the datagram’s data as opposed to merely unfilled space.
This method is useful when receiving datagrams, as we’ll explore later in this chapter.
When a datagram is received, its length is set to the length of the incoming data. This
means that if you try to receive another datagram into the same DatagramPacket, it’s
limited to no more than the number of bytes in the first. That is, once you’ve received
a 10-byte datagram, all subsequent datagrams will be truncated to 10 bytes; once you’ve
received a 9-byte datagram, all subsequent datagrams will be truncated to 9 bytes; and
so on. This method lets you reset the length of the buffer so that subsequent datagrams
aren’t truncated.