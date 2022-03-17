
Java Network Programming

6
7
8
9
10
11
12
13



# 4. Internet Addresses

```java
// the InetAddress class represents an IP address (both IPv4 and IPv6)

// InetAddress has static factory methods that connect to a DNS server to resolve a hostname

// if the hostname cannot be resolved then this method throws an UnknownHostException

import java.net.*;

try {
  InetAddress address = InetAddress.getByName("www.oreilly.com");
  System.out.println(address);
} catch (UnknownHostException ex) {
  System.out.println("Could not find www.oreilly.com");
}


// you can also do a reverse lookup by IP address
InetAddress address = InetAddress.getByName("208.201.239.100");
System.out.println(address.getHostName());

// If the address you look up does not have a hostname, getHostName() simply returns the dotted quad address you supplied.

// If, for some reason, you need all the addresses of a host, call getAllByName() instead, which returns an array:

try {
 InetAddress[] addresses = InetAddress.getAllByName("www.oreilly.com");
 for (InetAddress address : addresses) {
 System.out.println(address);
 }
} catch (UnknownHostException ex) {
 System.out.println("Could not find www.oreilly.com");
}

// the getLocalHost() method returns an InetAddress object for the host on which your code is running
try {
 InetAddress address = InetAddress.getLocalHost();
 System.out.println(address);
} catch (UnknownHostException ex) {
  System.out.println("Could not find this computer's address.");
}

If you know a numeric address, you can create an InetAddress object from that address without talking to DNS using InetAddress.getByAddress(). This method can create
addresses for hosts that do not exist or cannot be resolved:

public static InetAddress getByAddress(byte[] addr) throws UnknownHostException
public static InetAddress getByAddress(String hostname, byte[] addr)
 throws UnknownHostException

// The first InetAddress.getByAddress() factory method creates an InetAddress object with an IP address and no hostname. The second InetAddress.getByAddress() method creates an InetAddress object with an IP address and a hostname.

byte[] address = {107, 23, (byte) 216, (byte) 196};
InetAddress lessWrong = InetAddress.getByAddress(address);
InetAddress lessWrongWithname = InetAddress.getByAddress(
 "lesswrong.com", address);

// Unlike the other factory methods, these two methods make no guarantees that such a host exists or that the hostname is correctly mapped to the IP address.


// The InetAddress class contains four getter methods that return the hostname as a string and the IP address as both a string and a byte array:

public String getHostName()
public String getCanonicalHostName()
public byte[] getAddress()
public String getHostAddress()

// InetAddress immutable and thus thread safe.

// The getHostName() method returns a String that contains the name of the host with the IP address represented by this InetAddress object. If the machine in question doesn’t have a hostname, a dotted quad format of the numeric IP address is returned.
InetAddress machine = InetAddress.getLocalHost();
String localhost = machine.getHostName();

// The getCanonicalHostName() method is similar, but it’s a bit more aggressive about contacting DNS. getHostName() will only call DNS if it doesn’t think it already knows the hostname. getCanonicalHostName() calls DNS if it can, and may replace the existing cached hostname
InetAddress machine = InetAddress.getLocalHost();
String localhost = machine.getCanonicalHostName();

// The getHostAddress() method returns a string containing the dotted quad format of the IP address

 try {
 InetAddress me = InetAddress.getLocalHost();
 String dottedQuad = me.getHostAddress();
 System.out.println("My address is " + dottedQuad);
 } catch (UnknownHostException ex) {
 System.out.println("I'm sorry. I don't know my own address.");
 }

If you want to know the IP address of a machine (and you rarely do), then use the
getAddress() method, which returns an IP address as an array of bytes in network byte
order. The most significant byte (i.e., the first byte in the address’s dotted quad form) is
the first byte in the array, or element zero. To be ready for IPv6 addresses, try not to
assume anything about the length of this array. If you need to know the length of the
array, use the array’s length field:
InetAddress me = InetAddress.getLocalHost();
byte[] address = me.getAddress();
The bytes returned are unsigned, which poses a problem. Unlike C, Java doesn’t have
an unsigned byte primitive data type. Bytes with values higher than 127 are treated as
negative numbers. Therefore, if you want to do anything with the bytes returned by
getAddress(), you need to promote the bytes to ints and make appropriate adjust‐
ments. Here’s one way to do it:
int unsignedByte = signedByte < 0 ? signedByte + 256 : signedByte;
The InetAddress Class | 101
Here, signedByte may be either positive or negative. The conditional operator ? tests
whether signedByte is negative. If it is, 256 is added to signedByte to make it positive.
Otherwise, it’s left alone. signedByte is automatically promoted to an int before the
addition is performed, so wraparound is not a problem.
One reason to look at the raw bytes of an IP address is to determine the type of the
address. Test the number of bytes in the array returned by getAddress() to determine
whether you’re dealing with an IPv4 or IPv6 address. Example 4-5 demonstrates.
Example 4-5. Determining whether an IP address is v4 or v6
import java.net.*;
public class AddressTests {
 public static int getVersion(InetAddress ia) {
 byte[] address = ia.getAddress();
 if (address.length == 4) return 4;
 else if (address.length == 16) return 6;
 else return -1;
 }
}


Address Types
public boolean isAnyLocalAddress()
public boolean isLoopbackAddress()
public boolean isLinkLocalAddress()
public boolean isSiteLocalAddress()
public boolean isMulticastAddress()

The isAnyLocalAddress() method returns true if the address is a wildcard address,
false otherwise. A wildcard address matches any address of the local system. This is
important if the system has multiple network interfaces, as might be the case on a system
with multiple Ethernet cards or an Ethernet card and an 802.11 WiFi interface. In IPv4,
the wildcard address is 0.0.0.0. In IPv6, this address is 0:0:0:0:0:0:0:0 (a.k.a. ::).
102 | Chapter 4: Internet Addresses
The isLoopbackAddress() method returns true if the address is the loopback address,
false otherwise. The loopback address connects to the same computer directly in the IP
layer without using any physical hardware. Thus, connecting to the loopback address
enables tests to bypass potentially buggy or nonexistent Ethernet, PPP, and other drivers,
helping to isolate problems. Connecting to the loopback address is not the same as
connecting to the system’s normal IP address from the same system. In IPv4, this address
is 127.0.0.1. In IPv6, this address is 0:0:0:0:0:0:0:1 (a.k.a. ::1).
The isLinkLocalAddress() method returns true if the address is an IPv6 link-local
address, false otherwise. This is an address used to help IPv6 networks self-configure,
much like DHCP on IPv4 networks but without necessarily using a server. Routers do
not forward packets addressed to a link-local address beyond the local subnet. All linklocal addresses begin with the eight bytes FE80:0000:0000:0000. The next eight bytes
are filled with a local address, often copied from the Ethernet MAC address assigned
by the Ethernet card manufacturer.
The isSiteLocalAddress() method returns true if the address is an IPv6 site-local
address, false otherwise. Site-local addresses are similar to link-local addresses except
that they may be forwarded by routers within a site or campus but should not be for‐
warded beyond that site. Site-local addresses begin with the eight bytes
FEC0:0000:0000:0000. The next eight bytes are filled with a local address, often copied
from the Ethernet MAC address assigned by the Ethernet card manufacturer.
The isMulticastAddress() method returns true if the address is a multicast address,
false otherwise. Multicasting broadcasts content to all subscribed computers rather than
to one particular computer. In IPv4, multicast addresses all fall in the range 224.0.0.0
to 239.255.255.255. In IPv6, they all begin with byte FF. Multicasting will be discussed
in Chapter 13.


Testing Reachability
The InetAddress class has two isReachable() methods that test whether a particular node is reachable from the current host (i.e., whether a network connection can be
made).

public boolean isReachable(int timeout) throws IOException

These methods attempt to useICMP echo requests)to find out if the specified address is reachable. If the host responds within timeout milliseconds, the methods return true; otherwise, they return false. An IOException will
be thrown if there’s a network error.


Object Methods
Like all good classes, java.net.InetAddress has a toString() method that returns a
short text representation of the object. Example 4-1 and Example 4-2 implicitly called
this method when passing InetAddress objects to System.out.println(). As you saw,
the string produced by toString() has the form:
hostname/dotted quad address
Not all InetAddress objects have hostnames. If one doesn’t, the dotted quad address is
substituted in Java 1.3 and earlier. In Java 1.4 and later, the hostname is set to the empty
string.
Inet4Address and Inet6Address
Java uses two classes, Inet4Address and Inet6Address, in order to distinguish IPv4
addresses from IPv6 addresses:
public final class Inet4Address extends InetAddress
public final class Inet6Address extends InetAddress
Inet4Address and Inet6Address | 107
Most of the time, you really shouldn’t be concerned with whether an address is an IPv4
or IPv6 address. In the application layer where Java programs reside, you simply don’t
need to know this (and even if you do need to know, it’s quicker to check the size of the
byte array returned by getAddress() than to use instanceof to test which subclass you
have). Inet4Address overrides several of the methods in InetAddress but doesn’t
change their behavior in any public way. Inet6Address is similar, but it does add one
new method not present in the superclass, isIPv4CompatibleAddress():
public boolean isIPv4CompatibleAddress()
This method returns true if and only if the address is essentially an IPv4 address stuffed
into an IPv6 container—which means only the last four bytes are nonzero. That is, the
address has the form 0:0:0:0:0:0:0:xxxx. If this is the case, you can pull off the last four
bytes from the array returned by getBytes() and use this data to create an Inet4Ad
dress instead. However, you rarely need to do this.
The NetworkInterface Class
The NetworkInterface class represents a local IP address. This can either be a physical
interface such as an additional Ethernet card (common on firewalls and routers) or it
can be a virtual interface bound to the same physical hardware as the machine’s other
IP addresses. The NetworkInterface class provides methods to enumerate all the local
addresses, regardless of interface, and to create InetAddress objects from them. These
InetAddress objects can then be used to create sockets, server sockets, and so forth.
Factory Methods
Because NetworkInterface objects represent physical hardware and virtual addresses,
they cannot be constructed arbitrarily. As with the InetAddress class, there are static
factory methods that return the NetworkInterface object associated with a particular
network interface. You can ask for a NetworkInterface by IP address, by name, or by
enumeration.
public static NetworkInterface getByName(String name) throws SocketException
The getByName() method returns a NetworkInterface object representing the network
interface with the particular name. If there’s no interface with that name, it returns null.
If the underlying network stack encounters a problem while locating the relevant net‐
work interface, a SocketException is thrown, but this isn’t too likely to happen.
The format of the names is platform dependent. On a typical Unix system, the Ethernet
interface names have the form eth0, eth1, and so forth. The local loopback address is
probably named something like “lo”. On Windows, the names are strings like “CE31”
and “ELX100” that are derived from the name of the vendor and model of hardware on
108 | Chapter 4: Internet Addresses
that particular network interface. For example, this code fragment attempts to find the
primary Ethernet interface on a Unix system:
try {
 NetworkInterface ni = NetworkInterface.getByName("eth0");
 if (ni == null) {
 System.err.println("No such interface: eth0");
 }
} catch (SocketException ex) {
 System.err.println("Could not list sockets.");
}
public static NetworkInterface getByInetAddress(InetAddress address) throws SocketException
The getByInetAddress() method returns a NetworkInterface object representing the
network interface bound to the specified IP address. If no network interface is bound
to that IP address on the local host, it returns null. If anything goes wrong, it throws a
SocketException. For example, this code fragment finds the network interface for the
local loopback address:
try {
 InetAddress local = InetAddress.getByName("127.0.0.1");
 NetworkInterface ni = NetworkInterface.getByInetAddress(local);
 if (ni == null) {
 System.err.println("That's weird. No local loopback address.");
 }
} catch (SocketException ex) {
 System.err.println("Could not list network interfaces." );
} catch (UnknownHostException ex) {
 System.err.println("That's weird. Could not lookup 127.0.0.1.");
}
public static Enumeration getNetworkInterfaces() throws SocketException
The getNetworkInterfaces() method returns a java.util.Enumeration listing all the
network interfaces on the local host. Example 4-8 is a simple program to list all network
interfaces on the local host:
Example 4-8. A program that lists all the network interfaces
import java.net.*;
import java.util.*;
public class InterfaceLister {
 public static void main(String[] args) throws SocketException {
 Enumeration<NetworkInterface> interfaces = NetworkInterface.getNetwork
 Interfaces();
 while (interfaces.hasMoreElements()) {
 NetworkInterface ni = interfaces.nextElement();
 System.out.println(ni);
 }
The NetworkInterface Class | 109
 }
}
Here’s the result of running this on the IBiblio login server:
% java InterfaceLister
name:eth1 (eth1) index: 3 addresses:
/192.168.210.122;
name:eth0 (eth0) index: 2 addresses:
/152.2.210.122;
name:lo (lo) index: 1 addresses:
/127.0.0.1;
You can see that this host has two separate Ethernet cards plus the local loopback address.
The Ethernet card with index 2 has the IP address 152.2.210.122. The Ethernet card
with index 3 has the IP address 192.168.210.122. The loopback interface has address
127.0.0.1, as always.
Getter Methods
Once you have a NetworkInterface object, you can inquire about its IP address and
name. This is pretty much the only thing you can do with these objects.
public Enumeration getInetAddresses()
A single network interface may be bound to more than one IP address. This situation
isn’t common these days, but it does happen. The getInetAddresses() method returns
a java.util.Enumeration containing an InetAddress object for each IP address the
interface is bound to. For example, this code fragment lists all the IP addresses for the
eth0 interface:
NetworkInterface eth0 = NetworkInterrface.getByName("eth0");
Enumeration addresses = eth0.getInetAddresses();
while (addresses.hasMoreElements()) {
 System.out.println(addresses.nextElement());
}
public String getName()
The getName() method returns the name of a particular NetworkInterface object, such
as eth0 or lo.
public String getDisplayName()
The getDisplayName() method allegedly returns a more human-friendly name for the
particular NetworkInterface—something like “Ethernet Card 0.” However, in my tests
on Unix, it always returned the same string as getName(). On Windows, you may see
slightly friendlier names such as “Local Area Connection” or “Local Area Connection
2.”
110 | Chapter 4: Internet Addresses
Some Useful Programs
You now know everything there is to know about the java.net.InetAddress class. The
tools in this class alone let you write some genuinely useful programs. Here you’ll look
at two examples: one that queries your domain name server interactively and another
that can improve the performance of your web server by processing logfiles offline.
SpamCheck
A number of services monitor spammers, and inform clients whether a host attempting
to connect to them is a known spammer or not. These real-time blackhole lists need to
respond to queries extremely quickly, and process a very high load. Thousands, maybe
millions, of hosts query them repeatedly to find out whether an IP address attempting
a connection is or is not a known spammer.
The nature of the problem requires that the response be fast, and ideally it should be
cacheable. Furthermore, the load should be distributed across many servers, ideally ones
located around the world. Although this could conceivably be done using a web server,
SOAP, UDP, a custom protocol, or some other mechanism, this service is in fact cleverly
implemented using DNS and DNS alone.
To find out if a certain IP address is a known spammer, reverse the bytes of the address,
add the domain of the blackhole service, and look it up. If the address is found, it’s a
spammer. If it isn’t, it’s not. For instance, if you want to ask sbl.spamhaus.org if
207.87.34.17 is a spammer, you would look up the hostname 17.34.87.207.sbl.spam‐
haus.org. (Note that despite the numeric component, this is a hostname ASCII string,
not a dotted quad IP address.)
If the DNS query succeeds (and, more specifically, if it returns the address 127.0.0.2),
then the host is known to be a spammer. If the lookup fails—that is, it throws an
UnknownHostException—it isn’t. Example 4-9 implements this check.
Example 4-9. SpamCheck
import java.net.*;
public class SpamCheck {
 public static final String BLACKHOLE = "sbl.spamhaus.org";
 public static void main(String[] args) throws UnknownHostException {
 for (String arg: args) {
 if (isSpammer(arg)) {
 System.out.println(arg + " is a known spammer.");
 } else {
 System.out.println(arg + " appears legitimate.");
 }
 }
Some Useful Programs | 111
 }
 private static boolean isSpammer(String arg) {
 try {
 InetAddress address = InetAddress.getByName(arg);
 byte[] quad = address.getAddress();
 String query = BLACKHOLE;
 for (byte octet : quad) {
 int unsignedByte = octet < 0 ? octet + 256 : octet;
 query = unsignedByte + "." + query;
 }
 InetAddress.getByName(query);
 return true;
 } catch (UnknownHostException e) {
 return false;
 }
 }
}
Here’s some sample output:
$ java SpamCheck 207.34.56.23 125.12.32.4 130.130.130.130
207.34.56.23 appears legitimate.
125.12.32.4 appears legitimate.
130.130.130.130 appears legitimate.
If you use this technique, be careful to stay on top of changes to blackhole list policies
and addresses. For obvious reasons, blackhole servers are frequent targets of DDOS and
other attacks, so you want to be careful that if the blackhole server changes its address
or simply stops responding to any queries, you don’t begin blocking all traffic.
Further note that different blackhole lists can follow slightly different protocols. For
example, a few lists return 127.0.0.1 for spamming IPs instead of 127.0.0.2.
Processing Web Server Logfiles
Web server logs track the hosts that access a website. By default, the log reports the IP
addresses of the sites that connect to the server. However, you can often get more in‐
formation from the names of those sites than from their IP addresses. Most web servers
have an option to store hostnames instead of IP addresses, but this can hurt performance
because the server needs to make a DNS request for each hit. It is much more efficient
to log the IP addresses and convert them to hostnames at a later time, when the server
isn’t busy or even on another machine completely. Example 4-10 is a program called
Weblog that reads a web server logfile and prints each line with IP addresses converted
to hostnames.
Most web servers have standardized on the common logfile format. A typical line in the
common logfile format looks like this:
112 | Chapter 4: Internet Addresses
205.160.186.76 unknown - [17/Jun/2013:22:53:58 -0500]
 "GET /bgs/greenbg.gif HTTP 1.0" 200 50
This line indicates that a web browser at IP address 205.160.186.76 requested the
file /bgs/greenbg.gif from this web server at 11:53 P.M (and 58 seconds) on June 17, 2013.
The file was found (response code 200) and 50 bytes of data were successfully transferred
to the browser.
The first field is the IP address or, if DNS resolution is turned on, the hostname from
which the connection was made. This is followed by a space. Therefore, for our purposes,
parsing the logfile is easy: everything before the first space is the IP address, and ev‐
erything after it does not need to be changed.
The dotted quad format IP address is converted into a hostname using the usual methods
of java.net.InetAddress. Example 4-10 shows the code.
Example 4-10. Process web server logfiles
import java.io.*;
import java.net.*;
public class Weblog {
 public static void main(String[] args) {
 try (FileInputStream fin = new FileInputStream(args[0]);
 Reader in = new InputStreamReader(fin);
 BufferedReader bin = new BufferedReader(in);) {
 for (String entry = bin.readLine();
 entry != null;
 entry = bin.readLine()) {
 // separate out the IP address
 int index = entry.indexOf(' ');
 String ip = entry.substring(0, index);
 String theRest = entry.substring(index);
 // Ask DNS for the hostname and print it out
 try {
 InetAddress address = InetAddress.getByName(ip);
 System.out.println(address.getHostName() + theRest);
 } catch (UnknownHostException ex) {
 System.err.println(entry);
 }
 }
 } catch (IOException ex) {
 System.out.println("Exception: " + ex);
 }
 }
}
Some Useful Programs | 113
The name of the file to be processed is passed to Weblog as the first argument on the
command line. A FileInputStream fin is opened from this file and an InputStream
Reader is chained to fin. This InputStreamReader is buffered by chaining it to an
instance of the BufferedReader class. The file is processed line by line in a for loop.
Each pass through the loop places one line in the String variable entry. entry is then
split into two substrings: ip, which contains everything before the first space, and
theRest, which is everything from the first space to the end of the string. The position
of the first space is determined by entry.indexOf(" "). The substring ip is converted
to an InetAddress object using getByName(). getHostName() then looks up the host‐
name. Finally, the hostname and everything else on the line (theRest) are printed on
System.out. Output can be sent to a new file through the standard means for redirecting
output.
Weblog is more efficient than you might expect. Most web browsers generate multiple
logfile entries per page served, because there’s an entry in the log not just for the page
itself but for each graphic on the page. And many visitors request multiple pages while
visiting a site. DNS lookups are expensive and it simply doesn’t make sense to look up
each site every time it appears in the logfile. The InetAddress class caches requested
addresses. If the same address is requested again, it can be retrieved from the cache
much more quickly than from DNS.
Nonetheless, this program could certainly be faster. In my initial tests, it took more than
a second per log entry. (Exact numbers depend on the speed of your network connection,
the speed of the local and remote DNS servers, and network congestion when the pro‐
gram is run.) The program spends a huge amount of time sitting and waiting for DNS
requests to return. Of course, this is exactly the problem multithreading is designed to
solve. One main thread can read the logfile and pass off individual entries to other
threads for processing.
A thread pool is absolutely necessary here. Over the space of a few days, even lowvolume web servers can generate a logfile with hundreds of thousands of lines. Trying
to process such a logfile by spawning a new thread for each entry would rapidly bring
even the strongest virtual machine to its knees, especially because the main thread can
read logfile entries much faster than individual threads can resolve domain names and
die. Consequently, reusing threads is essential. The number of threads is stored in a
tunable parameter, numberOfThreads, so that it can be adjusted to fit the VM and net‐
work stack. (Launching too many simultaneous DNS requests can also cause problems.)
This program is now divided into two classes. The first class, LookupTask, shown in
Example 4-11, is a Callable that parses a logfile entry, looks up a single address, and
replaces that address with the corresponding hostname. This doesn’t seem like a lot of
work and CPU-wise, it isn’t. However, because it involves a network connection, and
114 | Chapter 4: Internet Addresses
possibly a hierarchical series of network connections between many different DNS
servers, it has a lot of downtime that can be put to better use by other threads.
Example 4-11. LookupTask
import java.net.*;
import java.util.concurrent.Callable;
public class LookupTask implements Callable<String> {
 private String line;
 public LookupTask(String line) {
 this.line = line;
 }
 @Override
 public String call() {
 try {
 // separate out the IP address
 int index = line.indexOf(' ');
 String address = line.substring(0, index);
 String theRest = line.substring(index);
 String hostname = InetAddress.getByName(address).getHostName();
 return hostname + " " + theRest;
 } catch (Exception ex) {
 return line;
 }
 }
}
The second class, PooledWeblog, shown in Example 4-12, contains the main() method
that reads the file and creates one LookupTask per line. Each task is submitted to an
executor that can run multiple (though not all) tasks in parallel and in sequence.
The Future that is returned from the submit() method is stored in a queue, along with
the original line (in case something goes wrong in the asynchronous thread). A loop
reads values out of the queue and prints them. This maintains the original order of the
logfile.
Example 4-12. PooledWebLog
import java.io.*;
import java.util.*;
import java.util.concurrent.*;
// Requires Java 7 for try-with-resources and multi-catch
public class PooledWeblog {
 private final static int NUM_THREADS = 4;
 public static void main(String[] args) throws IOException {
Some Useful Programs | 115
 ExecutorService executor = Executors.newFixedThreadPool(NUM_THREADS);
 Queue<LogEntry> results = new LinkedList<LogEntry>();
 try (BufferedReader in = new BufferedReader(
 new InputStreamReader(new FileInputStream(args[0]), "UTF-8"));) {
 for (String entry = in.readLine(); entry != null; entry = in.readLine()) {
 LookupTask task = new LookupTask(entry);
 Future<String> future = executor.submit(task);
 LogEntry result = new LogEntry(entry, future);
 results.add(result);
 }
 }
 // Start printing the results. This blocks each time a result isn't ready.
 for (LogEntry result : results) {
 try {
 System.out.println(result.future.get());
 } catch (InterruptedException | ExecutionException ex) {
 System.out.println(result.original);
 }
 }
 executor.shutdown();
 }
 private static class LogEntry {
 String original;
 Future<String> future;
 LogEntry(String original, Future<String> future) {
 this.original = original;
 this.future = future;
 }
 }
}
Using threads like this lets the same logfiles be processed in parallel—a huge time sav‐
ings. In my unscientific tests, the threaded version is 10 to 50 times faster than the
sequential version. The tech editor ran the same test on a different system and only saw
a factor of four improvement, but either way it’s still a significant gain.
There’s still one downside to this design. Although the queue of Callable tasks is much
more efficient than spawning a thread for each logfile entry, logfiles can be huge and
this program can still burn a lot of memory. To avoid this, you could put the output into
a separate thread that shared the queue with the input thread. Because early entries
could be processed and output while the input was still being parsed, the queue would
not grow so large. This does, however, introduce another problem. You’d need a separate
signal to tell you when the output was complete because an empty queue is no longer
sufficient to prove the job is complete. The easiest way is simply to count the number
of input lines and make sure it matches up to the number of output lines.



```

# 5. Urls and URIs

```java
import java.net.*;

// the immutable URL class represents a url

public URL(String protocol, String hostname, String file) throws MalformedURLException
public URL(String protocol, String host, int port, String file) throws MalformedURLException
public URL(URL base, String relative) throws MalformedURLException

try {
 URL u = new URL("http://www.audubon.org/");
} catch (MalformedURLException ex) {
 System.err.println(ex);
}


// You can also build a URL by specifying the protocol, the hostname, and the file (the default port for the protocol will be used)
try {
 URL u = new URL("http", "www.eff.org", "/blueribbon.html#intro");
} catch (MalformedURLException ex) {
 throw new RuntimeException("shouldn't happen; all VMs recognize http");
}


// the next constructor lets you specify the port explicitly as an int. The other arguments are the same
try {
 URL u = new URL("http", "fourier.dur.ac.uk", 8000, "/~dma3mjh/jsci/");
} catch (MalformedURLException ex) {
 throw new RuntimeException("shouldn't happen; all VMs recognize http");
}



// this constructor builds an absolute URL from a relative URL and a base URL
try {
 URL u1 = new URL("http://www.ibiblio.org/javafaq/index.html");
 URL u2 = new URL (u1, "mailinglists.html");
} catch (MalformedURLException ex) {
 System.err.println(ex);
}

// The URL class has several methods that retrieve data from a URL

public URLConnection openConnection() throws IOException
public URLConnection openConnection(Proxy proxy) throws IOException
public Object getContent() throws IOException
public Object getContent(Class[] classes) throws IOException

The most basic and most commonly used of these methods is openStream() which returns an InputStream from which you can read the data


If you need more control over the download process, call openConnection() instead, which gives you a URLConnection which you can configure, and then get an InputStream from it

Finally, you can ask the URL for its content with getContent() which
may give you a more complete object such as String or an Image.



public final InputStream openStream() throws IOException
The openStream() method connects to the resource referenced by the URL, performs
any necessary handshaking between the client and the server, and returns an Input
Stream from which data can be read. The data you get from this InputStream is the raw
(i.e., uninterpreted) content the URL references: ASCII if you’re reading an ASCII text
file, raw HTML if you’re reading an HTML file, binary image data if you’re reading an
image file, and so forth. It does not include any of the HTTP headers or any other
protocol-related information. You can read from this InputStream as you would read
from any other InputStream. For example:
try {
 URL u = new URL("http://www.lolcats.com");
 InputStream in = u.openStream();
 int c;
 while ((c = in.read()) != -1) System.out.write(c);
 in.close();
} catch (IOException ex) {
 System.err.println(ex);
}
The preceding code fragment catches an IOException, which also catches the Malfor
medURLException that the URL constructor can throw, since MalformedURLException
subclasses IOException.
As with most network streams, reliably closing the stream takes a bit of effort. In Java
6 and earlier, we use the dispose pattern: declare the stream variable outside the try
block, set it to null, and then close it in the finally block if it’s not null. For example:
InputStream in = null
try {
 URL u = new URL("http://www.lolcats.com");
 in = u.openStream();
 int c;
 while ((c = in.read()) != -1) System.out.write(c);
} catch (IOException ex) {
 System.err.println(ex);
} finally {
 try {
 if (in != null) {
 in.close();
 }
 } catch (IOException ex) {
 // ignore
 }
}
Java 7 makes this somewhat cleaner by using a nested try-with-resources statement:
try {
 URL u = new URL("http://www.lolcats.com");
The URL Class | 129
 try (InputStream in = u.openStream()) {
 int c;
 while ((c = in.read()) != -1) System.out.write(c);
 }
} catch (IOException ex) {
 System.err.println(ex);
}
Example 5-2 reads a URL from the command line, opens an InputStream from that
URL, chains the resulting InputStream to an InputStreamReader using the default
encoding, and then uses InputStreamReader’s read() method to read successive char‐
acters from the file, each of which is printed on System.out. That is, it prints the raw
data located at the URL if the URL references an HTML file; the program’s output is raw
HTML.
Example 5-2. Download a web page
import java.io.*;
import java.net.*;
public class SourceViewer {
 public static void main (String[] args) {
 if (args.length > 0) {
 InputStream in = null;
 try {
 // Open the URL for reading
 URL u = new URL(args[0]);
 in = u.openStream();
 // buffer the input to increase performance
 in = new BufferedInputStream(in);
 // chain the InputStream to a Reader
 Reader r = new InputStreamReader(in);
 int c;
 while ((c = r.read()) != -1) {
 System.out.print((char) c);
 }
 } catch (MalformedURLException ex) {
 System.err.println(args[0] + " is not a parseable URL");
 } catch (IOException ex) {
 System.err.println(ex);
 } finally {
 if (in != null) {
 try {
 in.close();
 } catch (IOException e) {
 // ignore
 }
 }
 }
 }
130 | Chapter 5: URLs and URIs
 }
}
And here are the first few lines of output when SourceViewer downloads http://
www.oreilly.com:
&lt;!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"&gt;
&lt;html xmlns="http://www.w3.org/1999/xhtml" lang="en-US" xml:lang="en-US"&gt;
&lt;head&gt;
&lt;title&gt;oreilly.com -- Welcome to O'Reilly Media, Inc. -- computer books,
software conferences, online publishing&lt;/title&gt;
&lt;meta name="keywords" content="O'Reilly, oreilly, computer books, technical
books, UNIX, unix, Perl, Java, Linux, Internet, Web, C, C++, Windows, Windows
NT, Security, Sys Admin, System Administration, Oracle, PL/SQL, online books,
books online, computer book online, e-books, ebooks, Perl Conference, Open Source
Conference, Java Conference, open source, free software, XML, Mac OS X, .Net, dot
net, C#, PHP, CGI, VB, VB Script, Java Script, javascript, Windows 2000, XP,
There are quite a few more lines in that web page; if you want to see them, you can fire
up your web browser.
The shakiest part of this program is that it blithely assumes that the URL points to text,
which is not necessarily true. It could well be pointing to a GIF or JPEG image, an MP3
sound file, or something else entirely. Even if does resolve to text, the document en‐
coding may not be the same as the default encoding of the client system. The remote
host and local client may not have the same default character set. As a general rule, for
pages that use a character set radically different from ASCII, the HTML will include a
META tag in the header specifying the character set in use. For instance, this META tag
specifies the Big-5 encoding for Chinese:
<meta http-equiv="Content-Type" content="text/html; charset=big5">
An XML document will likely have an XML declaration instead:
<?xml version="1.0" encoding="Big5"?>
In practice, there’s no easy way to get at this information other than by parsing the file
and looking for a header like this one, and even that approach is limited. Many HTML
files handcoded in Latin alphabets don’t have such a META tag. Since Windows, Mac, and
most Unixes have somewhat different interpretations of the characters from 128 to 255,
the extended characters in these documents do not translate correctly on platforms
other than the one on which they were created.
And as if this isn’t confusing enough, the HTTP header that precedes the actual docu‐
ment is likely to have its own encoding information, which may completely contradict
what the document itself says. You can’t read this header using the URL class, but you
can with the URLConnection object returned by the openConnection() method. En‐
coding detection and declaration is one of the thornier parts of the architecture of the
Web.
The URL Class | 131
public URLConnection openConnection() throws IOException
The openConnection() method opens a socket to the specified URL and returns a
URLConnection object. A URLConnection represents an open connection to a network
resource. If the call fails, openConnection() throws an IOException. For example:
try {
 URL u = new URL("https://news.ycombinator.com/");
 try {
 URLConnection uc = u.openConnection();
 InputStream in = uc.getInputStream();
 // read from the connection...
 } catch (IOException ex) {
 System.err.println(ex);
 }
} catch (MalformedURLException ex) {
 System.err.println(ex);
}
You should use this method when you want to communicate directly with the server.
The URLConnection gives you access to everything sent by the server: in addition to the
document itself in its raw form (e.g., HTML, plain text, binary image data), you can
access all the metadata specified by the protocol. For example, if the scheme is HTTP
or HTTPS, the URLConnection lets you access the HTTP headers as well as the raw
HTML. The URLConnection class also lets you write data to as well as read from a URL
—for instance, in order to send email to a mailto URL or post form data. The URLCon
nection class will be the primary subject of Chapter 7.
An overloaded variant of this method specifies the proxy server to pass the connection
through:
public URLConnection openConnection(Proxy proxy) throws IOException
This overrides any proxy server set with the usual socksProxyHost, socksProxyPort,
http.proxyHost, http.proxyPort, http.nonProxyHosts, and similar system proper‐
ties. If the protocol handler does not support proxies, the argument is ignored and the
connection is made directly if possible.
public final Object getContent() throws IOException
The getContent() method is the third way to download data referenced by a URL. The
getContent() method retrieves the data referenced by the URL and tries to make it into
some type of object. If the URL refers to some kind of text such as an ASCII or HTML
file, the object returned is usually some sort of InputStream. If the URL refers to an
image such as a GIF or a JPEG file, getContent() usually returns a java.awt.Image
Producer. What unifies these two disparate classes is that they are not the thing itself
but a means by which a program can construct the thing:
132 | Chapter 5: URLs and URIs
URL u = new URL("http://mesola.obspm.fr/");
Object o = u.getContent();
// cast the Object to the appropriate type
// work with the Object...
getContent() operates by looking at the Content-type field in the header of the data
it gets from the server. If the server does not use MIME headers or sends an unfamiliar
Content-type, getContent() returns some sort of InputStream with which the data
can be read. An IOException is thrown if the object can’t be retrieved. Example 5-3
demonstrates this.
Example 5-3. Download an object
import java.io.*;
import java.net.*;
public class ContentGetter {
 public static void main (String[] args) {
 if (args.length > 0) {
 // Open the URL for reading
 try {
 URL u = new URL(args[0]);
 Object o = u.getContent();
 System.out.println("I got a " + o.getClass().getName());
 } catch (MalformedURLException ex) {
 System.err.println(args[0] + " is not a parseable URL");
 } catch (IOException ex) {
 System.err.println(ex);
 }
 }
 }
}
Here’s the result of trying to get the content of http://www.oreilly.com:
% java ContentGetter http://www.oreilly.com/ I got a
sun.net.www.protocol.http.HttpURLConnection$HttpInputStream</programlisting>
The exact class may vary from one version of Java to the next (in earlier versions, it’s
been java.io.PushbackInputStream or sun.net.www.http.KeepAliveStream) but it
should be some form of InputStream.
Here’s what you get when you try to load a header image from that page:
% java ContentGetter http://www.oreilly.com/graphics_new/animation.gif
I got a sun.awt.image.URLImageSource</programlisting>
Here’s what happens when you try to load a Java applet using getContent():
The URL Class | 133
% java ContentGetter http://www.cafeaulait.org/RelativeURLTest.class</userinput>
I got a sun.net.www.protocol.http.HttpURLConnection$HttpInputStream
</programlisting>
Here’s what happens when you try to load an audio file using getContent():
% java ContentGetter http://www.cafeaulait.org/course/week9/spacemusic.au
</userinput>
I got a sun.applet.AppletAudioClip</programlisting>
The last result is the most unusual because it is as close as the Java core API gets to a
class that represents a sound file. It’s not just an interface through which you can load
the sound data.
This example demonstrates the biggest problems with using getContent(): it’s hard to
predict what kind of object you’ll get. You could get some kind of InputStream or an
ImageProducer or perhaps an AudioClip; it’s easy to check using the instanceof op‐
erator. This information should be enough to let you read a text file or display an image.
public final Object getContent(Class[] classes) throws IOException
A URL’s content handler may provide different views of a resource. This overloaded
variant of the getContent() method lets you choose which class you’d like the content
to be returned as. The method attempts to return the URL’s content in the first available
format. For instance, if you prefer an HTML file to be returned as a String, but your
second choice is a Reader and your third choice is an InputStream, write:
URL u = new URL("http://www.nwu.org");
Class<?>[] types = new Class[3];
types[0] = String.class;
types[1] = Reader.class;
types[2] = InputStream.class;
Object o = u.getContent(types);
If the content handler knows how to return a string representation of the resource, then
it returns a String. If it doesn’t know how to return a string representation of the re‐
source, then it returns a Reader. And if it doesn’t know how to present the resource as
a reader, then it returns an InputStream. You have to test for the type of the returned
object using instanceof. For example:
if (o instanceof String) {
 System.out.println(o);
} else if (o instanceof Reader) {
 int c;
 Reader r = (Reader) o;
 while ((c = r.read()) != -1) System.out.print((char) c);
 r.close();
} else if (o instanceof InputStream) {
 int c;
 InputStream in = (InputStream) o;
 while ((c = in.read()) != -1) System.out.write(c);
134 | Chapter 5: URLs and URIs
 in.close();
} else {
 System.out.println("Error: unexpected type " + o.getClass());
}
Splitting a URL into Pieces
URLs are composed of five pieces:
• The scheme, also known as the protocol
• The authority
• The path
• The fragment identifier, also known as the section or ref
• The query string
For example, in the URL http://www.ibiblio.org/javafaq/books/jnp/index.html?
isbn=1565922069#toc, the scheme is http, the authority is www.ibiblio.org, the path is /
javafaq/books/jnp/index.html, the fragment identifier is toc, and the query string is
isbn=1565922069. However, not all URLs have all these pieces. For instance, the URL
http://www.faqs.org/rfcs/rfc3986.html has a scheme, an authority, and a path, but no
fragment identifier or query string.
The authority may further be divided into the user info, the host, and the port. For
example, in the URL http://admin@www.blackstar.com:8080/, the authority is ad
min@www.blackstar.com:8080. This has the user info admin, the host www.black‐
star.com, and the port 8080.
Read-only access to these parts of a URL is provided by nine public methods: get
File(), getHost(), getPort(), getProtocol(), getRef(), getQuery(), getPath(),
getUserInfo(), and getAuthority().
public String getProtocol()
The getProtocol() method returns a String containing the scheme of the URL (e.g.,
“http”, “https”, or “file”). For example, this code fragment prints https:
URL u = new URL("https://xkcd.com/727/");
System.out.println(u.getProtocol());
public String getHost()
The getHost() method returns a String containing the hostname of the URL. For
example, this code fragment prints xkcd.com:
URL u = new URL("https://xkcd.com/727/");
System.out.println(u.getHost());
The URL Class | 135
public int getPort()
The getPort() method returns the port number specified in the URL as an int. If no
port was specified in the URL, getPort() returns -1 to signify that the URL does not
specify the port explicitly, and will use the default port for the protocol. For example, if
the URL is http://www.userfriendly.org/, getPort() returns -1; if the URL is http://
www.userfriendly.org:80/, getPort() returns 80. The following code prints -1 for the
port number because it isn’t specified in the URL:
URL u = new URL("http://www.ncsa.illinois.edu/AboutUs/");
System.out.println("The port part of " + u + " is " + u.getPort());
public int getDefaultPort()
The getDefaultPort() method returns the default port used for this URL’s protocol
when none is specified in the URL. If no default port is defined for the protocol, then
getDefaultPort() returns -1. For example, if the URL is http://www.userfriendly.org/,
getDefaultPort() returns 80; if the URL is ftp://ftp.userfriendly.org:8000/, getDefault
Port() returns 21.
public String getFile()
The getFile() method returns a String that contains the path portion of a URL; re‐
member that Java does not break a URL into separate path and file parts. Everything
from the first slash (/) after the hostname until the character preceding the # sign that
begins a fragment identifier is considered to be part of the file. For example:
URL page = this.getDocumentBase();
System.out.println("This page's path is " + page.getFile());
If the URL does not have a file part, Java sets the file to the empty string.
public String getPath()
The getPath() method is a near synonym for getFile(); that is, it returns a String
containing the path and file portion of a URL. However, unlike getFile(), it does not
include the query string in the String it returns, just the path.
Note that the getPath() method does not return only the directory
path and getFile() does not return only the filename, as you might
expect. Both getPath() and getFile() return the full path and file‐
name. The only difference is that getFile() also returns the query
string and getPath() does not.
136 | Chapter 5: URLs and URIs
public String getRef()
The getRef() method returns the fragment identifier part of the URL. If the URL doesn’t
have a fragment identifier, the method returns null. In the following code, getRef()
returns the string xtocid1902914:
URL u = new URL(
 "http://www.ibiblio.org/javafaq/javafaq.html#xtocid1902914");
System.out.println("The fragment ID of " + u + " is " + u.getRef());
public String getQuery()
The getQuery() method returns the query string of the URL. If the URL doesn’t have
a query string, the method returns null. In the following code, getQuery() returns the
string category=Piano:
URL u = new URL(
 "http://www.ibiblio.org/nywc/compositions.phtml?category=Piano");
System.out.println("The query string of " + u + " is " + u.getQuery());
public String getUserInfo()
Some URLs include usernames and occasionally even password information. This in‐
formation comes after the scheme and before the host; an @ symbol delimits it. For
instance, in the URL http://elharo@java.oreilly.com/, the user info is elharo. Some URLs
also include passwords in the user info. For instance, in the URL ftp://
mp3:secret@ftp.example.com/c%3a/stuff/mp3/, the user info is mp3:secret. However,
most of the time, including a password in a URL is a security risk. If the URL doesn’t
have any user info, getUserInfo() returns null.
Mailto URLs may not behave like you expect. In a URL like mailto:elharo@ibiblio.org,
“elharo@ibiblio.org” is the path, not the user info and the host. That’s because the URL
specifies the remote recipient of the message rather than the username and host that’s
sending the message.
public String getAuthority()
Between the scheme and the path of a URL, you’ll find the authority. This part of the
URI indicates the authority that resolves the resource. In the most general case, the
authority includes the user info, the host, and the port. For example, in the URL
ftp://mp3:mp3@138.247.121.61:21000/c%3a/, the authority is
mp3:mp3@138.247.121.61:21000, the user info is mp3:mp3, the host is 138.247.121.61,
and the port is 21000. However, not all URLs have all parts. For instance, in the URL
http://conferences.oreilly.com/java/speakers/, the authority is simply the hostname con‐
ferences.oreilly.com. The getAuthority() method returns the authority as it exists in
the URL, with or without the user info and port.
Example 5-4 uses these methods to split URLs entered on the command line into their
component parts.
The URL Class | 137
Example 5-4. The parts of a URL
import java.net.*;
public class URLSplitter {
 public static void main(String args[]) {
 for (int i = 0; i < args.length; i++) {
 try {
 URL u = new URL(args[i]);
 System.out.println("The URL is " + u);
 System.out.println("The scheme is " + u.getProtocol());
 System.out.println("The user info is " + u.getUserInfo());
 String host = u.getHost();
 if (host != null) {
 int atSign = host.indexOf('@');
 if (atSign != -1) host = host.substring(atSign+1);
 System.out.println("The host is " + host);
 } else {
 System.out.println("The host is null.");
 }
 System.out.println("The port is " + u.getPort());
 System.out.println("The path is " + u.getPath());
 System.out.println("The ref is " + u.getRef());
 System.out.println("The query string is " + u.getQuery());
 } catch (MalformedURLException ex) {
 System.err.println(args[i] + " is not a URL I understand.");
 }
 System.out.println();
 }
 }
}
Here’s the result of running this against several of the URL examples in this chapter:
% java URLSplitter \
ftp://mp3:mp3@138.247.121.61:21000/c%3a/ \
http://www.oreilly.com \
http://www.ibiblio.org/nywc/compositions.phtml?category=Piano \
http://admin@www.blackstar.com:8080/ \
The URL is ftp://mp3:mp3@138.247.121.61:21000/c%3a/
The scheme is ftp
The user info is mp3:mp3
The host is 138.247.121.61
The port is 21000
The path is /c%3a/
The ref is null
The query string is null
138 | Chapter 5: URLs and URIs
The URL is http://www.oreilly.com
The scheme is http
The user info is null
The host is www.oreilly.com
The port is -1
The path is
The ref is null
The query string is null
The URL is http://www.ibiblio.org/nywc/compositions.phtml?category=Piano
The scheme is http
The user info is null
The host is www.ibiblio.org
The port is -1
The path is /nywc/compositions.phtml
The ref is null
The query string is category=Piano
The URL is http://admin@www.blackstar.com:8080/
The scheme is http
The user info is admin
The host is www.blackstar.com
The port is 8080
The path is /
The ref is null
The query string is null</programlisting>
Equality and Comparison
The URL class contains the usual equals() and hashCode() methods. These behave
almost as you’d expect. Two URLs are considered equal if and only if both URLs point
to the same resource on the same host, port, and path, with the same fragment identifier
and query string. However there is one surprise here. The equals() method actually
tries to resolve the host with DNS so that, for example, it can tell that http://
www.ibiblio.org/ and http://ibiblio.org/ are the same.
This means that equals() on a URL is potentially a blocking I/O oper‐
ation! For this reason, you should avoid storing URLs in data struc‐
ture that depend on equals() such as java.util.HashMap. Prefer
java.net.URI for this, and convert back and forth from URIs to URLs
when necessary.
On the other hand, equals() does not go so far as to actually compare the resources
identified by two URLs. For example, http://www.oreilly.com/ is not equal to http://
www.oreilly.com/index.html; and http://www.oreilly.com:80 is not equal to http://
www.oreilly.com/.
The URL Class | 139
Example 5-5 creates URL objects for http://www.ibiblio.org/ and http://ibiblio.org/ and
tells you if they’re the same using the equals() method.
Example 5-5. Are http://www.ibiblio.org and http://ibiblio.org the same?
import java.net.*;
public class URLEquality {
 public static void main (String[] args) {
 try {
 URL www = new URL ("http://www.ibiblio.org/");
 URL ibiblio = new URL("http://ibiblio.org/");
 if (ibiblio.equals(www)) {
 System.out.println(ibiblio + " is the same as " + www);
 } else {
 System.out.println(ibiblio + " is not the same as " + www);
 }
 } catch (MalformedURLException ex) {
 System.err.println(ex);
 }
 }
}
When you run this program, you discover:
<programlisting format="linespecific" id="I_7_tt233">% <userinput moreinfo=
"none">
 java URLEquality</userinput>
http://www.ibiblio.org/ is the same as http://ibiblio.org/</programlisting>
URL does not implement Comparable.
The URL class also has a sameFile() method that checks whether two URLs point to the
same resource:
public boolean sameFile(URL other)
The comparison is essentially the same as with equals(), DNS queries included, except
that sameFile() does not consider the fragment identifier. This sameFile() returns
true when comparing http://www.oreilly.com/index.html#p1 and http://www.oreil‐
ly.com/index.html#q2 while equals() would return false.
Here’s a fragment of code that uses sameFile() to compare two URLs:
URL u1 = new URL("http://www.ncsa.uiuc.edu/HTMLPrimer.html#GS");
URL u2 = new URL("http://www.ncsa.uiuc.edu/HTMLPrimer.html#HD");
if (u1.sameFile(u2)) {
 System.out.println(u1 + " is the same file as \n" + u2);
} else {
 System.out.println(u1 + " is not the same file as \n" + u2);
}
140 | Chapter 5: URLs and URIs
The output is:
http://www.ncsa.uiuc.edu/HTMLPrimer.html#GS is the same file as
http://www.ncsa.uiuc.edu/HTMLPrimer.html#HD
Conversion
URL has three methods that convert an instance to another form: toString(),
toExternalForm(), and toURI().
Like all good classes, java.net.URL has a toString() method. The String produced
by toString() is always an absolute URL, such as http://www.cafeaulait.org/javatuto‐
rial.html. It’s uncommon to call toString() explicitly. Print statements call to
String() implicitly. Outside of print statements, it’s more proper to use toExternal
Form() instead:
public String toExternalForm()
The toExternalForm() method converts a URL object to a string that can be used in an
HTML link or a web browser’s Open URL dialog.
The toExternalForm() method returns a human-readable String representing the
URL. It is identical to the toString() method. In fact, all the toString() method does
is return toExternalForm().
Finally, the toURI() method converts a URL object to an equivalent URI object:
public URI toURI() throws URISyntaxException
We’ll take up the URI class shortly. In the meantime, the main thing you need to know
is that the URI class provides much more accurate, specification-conformant behavior
than the URL class. For operations like absolutization and encoding, you should prefer
the URI class where you have the option. You should also prefer the URI class if you need
to store URLs in a hashtable or other data structure, since its equals() method is not
blocking. The URL class should be used primarily when you want to download content
from a server.
The URI Class
A URI is a generalization of a URL that includes not only Uniform Resource Locators
but also Uniform Resource Names (URNs). Most URIs used in practice are URLs, but
most specifications and standards such as XML are defined in terms of URIs. In Java,
URIs are represented by the java.net.URI class. This class differs from the
java.net.URL class in three important ways:
The URI Class | 141
• The URI class is purely about identification of resources and parsing of URIs. It
provides no methods to retrieve a representation of the resource identified by its
URI.
• The URI class is more conformant to the relevant specifications than the URL class.
• A URI object can represent a relative URI. The URL class absolutizes all URIs before
storing them.
In brief, a URL object is a representation of an application layer protocol for network
retrieval, whereas a URI object is purely for string parsing and manipulation. The URI
class has no network retrieval capabilities. The URL class has some string parsing meth‐
ods, such as getFile() and getRef(), but many of these are broken and don’t always
behave exactly as the relevant specifications say they should. Normally, you should use
the URL class when you want to download the content at a URL and the URI class when
you want to use the URL for identification rather than retrieval, for instance, to represent
an XML namespace. When you need to do both, you may convert from a URI to a URL
with the toURL() method, and from a URL to a URI using the toURI() method.


Constructing a URI
URIs are built from strings. You can either pass the entire URI to the constructor in a
single string, or the individual pieces:
public URI(String uri) throws URISyntaxException
public URI(String scheme, String schemeSpecificPart, String fragment)
 throws URISyntaxException
public URI(String scheme, String host, String path, String fragment)
 throws URISyntaxException
public URI(String scheme, String authority, String path, String query,
 String fragment) throws URISyntaxException
public URI(String scheme, String userInfo, String host, int port,
 String path, String query, String fragment) throws URISyntaxException
Unlike the URL class, the URI class does not depend on an underlying protocol handler.
As long as the URI is syntactically correct, Java does not need to understand its protocol
in order to create a representative URI object. Thus, unlike the URL class, the URI class
can be used for new and experimental URI schemes.
The first constructor creates a new URI object from any convenient string. For example:
URI voice = new URI("tel:+1-800-9988-9938");
URI web = new URI("http://www.xml.com/pub/a/2003/09/17/stax.html#id=_hbc");
URI book = new URI("urn:isbn:1-565-92870-9");
If the string argument does not follow URI syntax rules—for example, if the URI begins
with a colon—this constructor throws a URISyntaxException. This is a checked ex‐
ception, so either catch it or declare that the method where the constructor is invoked
142 | Chapter 5: URLs and URIs
can throw it. However, one syntax rule is not checked. In contradiction to the URI
specification, the characters used in the URI are not limited to ASCII. They can include
other Unicode characters, such as ø and é. Syntactically, there are very few restrictions
on URIs, especially once the need to encode non-ASCII characters is removed and
relative URIs are allowed. Almost any string can be interpreted as a URI.
The second constructor that takes a scheme specific part is mostly used for nonhier‐
archical URIs. The scheme is the URI’s protocol, such as http, urn, tel, and so forth. It
must be composed exclusively of ASCII letters and digits and the three punctuation
characters +, -, and .. It must begin with a letter. Passing null for this argument omits
the scheme, thus creating a relative URI. For example:
URI absolute = new URI("http", "//www.ibiblio.org" , null);
URI relative = new URI(null, "/javafaq/index.shtml", "today");
The scheme-specific part depends on the syntax of the URI scheme; it’s one thing for
an http URL, another for a mailto URL, and something else again for a tel URI. Because
the URI class encodes illegal characters with percent escapes, there’s effectively no syntax
error you can make in this part.
Finally, the third argument contains the fragment identifier, if any. Again, characters
that are forbidden in a fragment identifier are escaped automatically. Passing null for
this argument simply omits the fragment identifier.
The third constructor is used for hierarchical URIs such as http and ftp URLs. The host
and path together (separated by a /) form the scheme-specific part for this URI. For
example:
URI today= new URI("http", "www.ibiblio.org", "/javafaq/index.html", "today");
This produces the URI http://www.ibiblio.org/javafaq/index.html#today.
If the constructor cannot form a legal hierarchical URI from the supplied pieces—for
instance, if there is a scheme so the URI has to be absolute but the path doesn’t start
with /—then it throws a URISyntaxException.
The fourth constructor is basically the same as the third, with the addition of a query
string. For example:
URI today = new URI("http", "www.ibiblio.org", "/javafaq/index.html",
 "referrer=cnet&date=2014-02-23", "today");
As usual, any unescapable syntax errors cause a URISyntaxException to be thrown and
null can be passed to omit any of the arguments.
The fifth constructor is the master hierarchical URI constructor that the previous two
invoke. It divides the authority into separate user info, host, and port parts, each of which
has its own syntax rules. For example:
The URI Class | 143
URI styles = new URI("ftp", "anonymous:elharo@ibiblio.org",
 "ftp.oreilly.com", 21, "/pub/stylesheet", null, null);
However, the resulting URI still has to follow all the usual rules for URIs; and again null
can be passed for any argument to omit it from the result.
If you’re sure your URIs are legal and do not violate any of the rules, you can use the
static factory URI.create() method instead. Unlike the constructors, it does not throw
a URISyntaxException. For example, this invocation creates a URI for anonymous FTP
access using an email address as password:
URI styles = URI.create(
 "ftp://anonymous:elharo%40ibiblio.org@ftp.oreilly.com:21/pub/stylesheet");
If the URI does prove to be malformed, then an IllegalArgumentException is thrown
by this method. This is a runtime exception, so you don’t have to explicitly declare it or
catch it.
The Parts of the URI
A URI reference has up to three parts: a scheme, a scheme-specific part, and a fragment
identifier. The general format is:
scheme:scheme-specific-part:fragment
If the scheme is omitted, the URI reference is relative. If the fragment identifier is omit‐
ted, the URI reference is a pure URI. The URI class has getter methods that return these
three parts of each URI object. The getRawFoo() methods return the encoded forms of
the parts of the URI, while the equivalent getFoo() methods first decode any percentescaped characters and then return the decoded part:
public String getScheme()
public String getSchemeSpecificPart()
public String getRawSchemeSpecificPart()
public String getFragment()
public String getRawFragment()
There’s no getRawScheme() method because the URI specification re‐
quires that all scheme names be composed exclusively of URI-legal
ASCII characters and does not allow percent escapes in scheme names.
These methods all return null if the particular URI object does not have the relevant
component: for example, a relative URI without a scheme or an http URI without a
fragment identifier.
A URI that has a scheme is an absolute URI. A URI without a scheme is relative. The
isAbsolute() method returns true if the URI is absolute, false if it’s relative:
144 | Chapter 5: URLs and URIs
public boolean isAbsolute()
The details of the scheme-specific part vary depending on the type of the scheme. For
example, in a tel URL, the scheme-specific part has the syntax of a telephone number.
However, in many useful URIs, including the very common file and http URLs, the
scheme-specific part has a particular hierarchical format divided into an authority, a
path, and a query string. The authority is further divided into user info, host, and port.
The isOpaque() method returns false if the URI is hierarchical, true if it’s not hier‐
archical—that is, if it’s opaque:
public boolean isOpaque()
If the URI is opaque, all you can get is the scheme, scheme-specific part, and fragment
identifier. However, if the URI is hierarchical, there are getter methods for all the dif‐
ferent parts of a hierarchical URI:
public String getAuthority()
public String getFragment()
public String getHost()
public String getPath()
public String getPort()
public String getQuery()
public String getUserInfo()
These methods all return the decoded parts; in other words, percent escapes, such as
%3C, are changed into the characters they represent, such as <. If you want the raw,
encoded parts of the URI, there are five parallel getRaw_Foo_() methods:
public String getRawAuthority()
public String getRawFragment()
public String getRawPath()
public String getRawQuery()
public String getRawUserInfo()
Remember the URI class differs from the URI specification in that non-ASCII characters
such as é and ü are never percent escaped in the first place, and thus will still be present
in the strings returned by the getRawFoo() methods unless the strings originally used
to construct the URI object were encoded.
There are no getRawPort() and getRawHost() methods because these
components are always guaranteed to be made up of ASCII characters.
In the event that the specific URI does not contain this information—for instance, the
URI http://www.example.com has no user info, path, port, or query string—the relevant
methods return null. getPort() is the single exception. Since it’s declared to return an
int, it can’t return null. Instead, it returns -1 to indicate an omitted port.
The URI Class | 145
For various technical reasons that don’t have a lot of practical impact, Java can’t always
initially detect syntax errors in the authority component. The immediate symptom of
this failing is normally an inability to return the individual parts of the authority, port,
host, and user info. In this event, you can call parseServerAuthority() to force the
authority to be reparsed:
public URI parseServerAuthority() throws URISyntaxException
The original URI does not change (URI objects are immutable), but the URI returned will
have separate authority parts for user info, host, and port. If the authority cannot be
parsed, a URISyntaxException is thrown.
Example 5-6 uses these methods to split URIs entered on the command line into their
component parts. It’s similar to Example 5-4 but works with any syntactically correct
URI, not just the ones Java has a protocol handler for.
Example 5-6. The parts of a URI
import java.net.*;
public class URISplitter {
 public static void main(String args[]) {
 for (int i = 0; i < args.length; i++) {
 try {
 URI u = new URI(args[i]);
 System.out.println("The URI is " + u);
 if (u.isOpaque()) {
 System.out.println("This is an opaque URI.");
 System.out.println("The scheme is " + u.getScheme());
 System.out.println("The scheme specific part is "
 + u.getSchemeSpecificPart());
 System.out.println("The fragment ID is " + u.getFragment());
 } else {
 System.out.println("This is a hierarchical URI.");
 System.out.println("The scheme is " + u.getScheme());
 try {
 u = u.parseServerAuthority();
 System.out.println("The host is " + u.getHost());
 System.out.println("The user info is " + u.getUserInfo());
 System.out.println("The port is " + u.getPort());
 } catch (URISyntaxException ex) {
 // Must be a registry based authority
 System.out.println("The authority is " + u.getAuthority());
 }
 System.out.println("The path is " + u.getPath());
 System.out.println("The query string is " + u.getQuery());
 System.out.println("The fragment ID is " + u.getFragment());
 }
 } catch (URISyntaxException ex) {
146 | Chapter 5: URLs and URIs
 System.err.println(args[i] + " does not seem to be a URI.");
 }
 System.out.println();
 }
 }
}
Here’s the result of running this against three of the URI examples in this section:
% java URISplitter tel:+1-800-9988-9938 \
 http://www.xml.com/pub/a/2003/09/17/stax.html#id=_hbc \
 urn:isbn:1-565-92870-9
The URI is tel:+1-800-9988-9938
This is an opaque URI.
The scheme is tel
The scheme specific part is +1-800-9988-9938
The fragment ID is null
The URI is http://www.xml.com/pub/a/2003/09/17/stax.html#id=_hbc
This is a hierarchical URI.
The scheme is http
The host is www.xml.com
The user info is null
The port is -1
The path is /pub/a/2003/09/17/stax.html
The query string is null
The fragment ID is id=_hbc
The URI is urn:isbn:1-565-92870-9
This is an opaque URI.
The scheme is urn
The scheme specific part is isbn:1-565-92870-9
The fragment ID is null</programlisting>
Resolving Relative URIs
The URI class has three methods for converting back and forth between relative and
absolute URIs:
public URI resolve(URI uri)
public URI resolve(String uri)
public URI relativize(URI uri)
The resolve() methods compare the uri argument to this URI and use it to construct
a new URI object that wraps an absolute URI. For example, consider these three lines of
code:
URI absolute = new URI("http://www.example.com/");
URI relative = new URI("images/logo.png");
URI resolved = absolute.resolve(relative);
The URI Class | 147
After they’ve executed, resolved contains the absolute URI http://www.example.com/
images/logo.png.
If the invoking URI does not contain an absolute URI itself, the resolve() method
resolves as much of the URI as it can and returns a new relative URI object as a result.
For example, take these three statements:
URI top = new URI("javafaq/books/");
URI resolved = top.resolve("jnp3/examples/07/index.html");
After they’ve executed, resolved now contains the relative URI javafaq/books/jnp3/
examples/07/index.html with no scheme or authority.
It’s also possible to reverse this procedure; that is, to go from an absolute URI to a relative
one. The relativize() method creates a new URI object from the uri argument that
is relative to the invoking URI. The argument is not changed. For example:
URI absolute = new URI("http://www.example.com/images/logo.png");
URI top = new URI("http://www.example.com/");
URI relative = top.relativize(absolute);
The URI object relative now contains the relative URI images/logo.png.
Equality and Comparison
URIs are tested for equality pretty much as you’d expect. It’s not quite direct string
comparison. Equal URIs must both either be hierarchical or opaque. The scheme and
authority parts are compared without considering case. That is, http and HTTP are the
same scheme, and www.example.com is the same authority as www.EXAMPLE.com.
The rest of the URI is case sensitive, except for hexadecimal digits used to escape illegal
characters. Escapes are not decoded before comparing. http://www.example.com/A and
http://www.example.com/%41 are unequal URIs.
The hashCode() method is consistent with equals. Equal URIs do have the same hash
code and unequal URIs are fairly unlikely to share the same hash code.
URI implements Comparable, and thus URIs can be ordered. The ordering is based on
string comparison of the individual parts, in this sequence:
1. If the schemes are different, the schemes are compared, without considering case.
2. Otherwise, if the schemes are the same, a hierarchical URI is considered to be less
than an opaque URI with the same scheme.
3. If both URIs are opaque URIs, they’re ordered according to their scheme-specific
parts.
4. If both the scheme and the opaque scheme-specific parts are equal, the URIs are
compared by their fragments.
148 | Chapter 5: URLs and URIs
5. If both URIs are hierarchical, they’re ordered according to their authority compo‐
nents, which are themselves ordered according to user info, host, and port, in that
order. Hosts are case insensitive.
6. If the schemes and the authorities are equal, the path is used to distinguish them.
7. If the paths are also equal, the query strings are compared.
8. If the query strings are equal, the fragments are compared.
URIs are not comparable to any type except themselves. Comparing a URI to anything
except another URI causes a ClassCastException.
String Representations
Two methods convert URI objects to strings, toString() and toASCIIString():
public String toString()
public String toASCIIString()
The toString() method returns an unencoded string form of the URI (i.e., characters
like é and \ are not percent escaped). Therefore, the result of calling this method is not
guaranteed to be a syntactically correct URI, though it is in fact a syntactically correct
IRI. This form is sometimes useful for display to human beings, but usually not for
retrieval.
The toASCIIString() method returns an encoded string form of the URI. Characters
like é and \ are always percent escaped whether or not they were originally escaped. This
is the string form of the URI you should use most of the time. Even if the form returned
by toString() is more legible for humans, they may still copy and paste it into areas
that are not expecting an illegal URI. toASCIIString() always returns a syntactically
correct URI.
x-www-form-urlencoded
One of the challenges faced by the designers of the Web was dealing with the differences
between operating systems. These differences can cause problems with URLs: for ex‐
ample, some operating systems allow spaces in filenames; some don’t. Most operating
systems won’t complain about a # sign in a filename; but in a URL, a # sign indicates
that the filename has ended, and a fragment identifier follows. Other special characters,
nonalphanumeric characters, and so on, all of which may have a special meaning inside
a URL or on another operating system, present similar problems. Furthermore, Unicode
was not yet ubiquitous when the Web was invented, so not all systems could handle
characters such as é and 本. To solve these problems, characters used in URLs must
come from a fixed subset of ASCII, specifically:
• The capital letters A–Z
x-www-form-urlencoded | 149
• The lowercase letters a–z
• The digits 0–9
• The punctuation characters - _ . ! ~ * ' (and ,)
The characters : / & ? @ # ; $ + = and % may also be used, but only for their specified
purposes. If these characters occur as part of a path or query string, they and all other
characters should be encoded.
The encoding is very simple. Any characters that are not ASCII numerals, letters, or the
punctuation marks specified earlier are converted into bytes and each byte is written as
a percent sign followed by two hexadecimal digits. Spaces are a special case because
they’re so common. Besides being encoded as %20, they can be encoded as a plus sign
(+). The plus sign itself is encoded as %2B. The / # = & and ? characters should be
encoded when they are used as part of a name, and not as a separator between parts of
the URL.
The URL class does not encode or decode automatically. You can construct URL objects
that use illegal ASCII and non-ASCII characters and/or percent escapes. Such characters
and escapes are not automatically encoded or decoded when output by methods such
as getPath() and toExternalForm(). You are responsible for making sure all such
characters are properly encoded in the strings used to construct a URL object.
Luckily, Java provides URLEncoder and URLDecoder classes to cipher strings in this for‐
mat.
URLEncoder
To URL encode a string, pass the string and the character set name to the URLEncod
er.encode() method. For example:
String encoded = URLEncoder.encode("This*string*has*asterisks", "UTF-8");
URLEncoder.encode() returns a copy of the input string with a few changes. Any non‐
alphanumeric characters are converted into % sequences (except the space, underscore,
hyphen, period, and asterisk characters). It also encodes all non-ASCII characters. The
space is converted into a plus sign. This method is a little overaggressive; it also converts
tildes, single quotes, exclamation points, and parentheses to percent escapes, even
though they don’t absolutely have to be. However, this change isn’t forbidden by the
URL specification, so web browsers deal reasonably with these excessively encoded
URLs.
Although this method allows you to specify the character set, the only such character
set you should ever pick is UTF-8. UTF-8 is compatible with the IRI specification, the
URI class, modern web browsers, and more additional software than any other encoding
you could choose.
150 | Chapter 5: URLs and URIs
Example 5-7 is a program that uses URLEncoder.encode() to print various encoded
strings.
Example 5-7. x-www-form-urlencoded strings
import java.io.*;
import java.net.*;
public class EncoderTest {
 public static void main(String[] args) {
 try {
 System.out.println(URLEncoder.encode("This string has spaces",
 "UTF-8"));
 System.out.println(URLEncoder.encode("This*string*has*asterisks",
 "UTF-8"));
 System.out.println(URLEncoder.encode("This%string%has%percent%signs",
 "UTF-8"));
 System.out.println(URLEncoder.encode("This+string+has+pluses",
 "UTF-8"));
 System.out.println(URLEncoder.encode("This/string/has/slashes",
 "UTF-8"));
 System.out.println(URLEncoder.encode("This\"string\"has\"quote\"marks",
 "UTF-8"));
 System.out.println(URLEncoder.encode("This:string:has:colons",
 "UTF-8"));
 System.out.println(URLEncoder.encode("This~string~has~tildes",
 "UTF-8"));
 System.out.println(URLEncoder.encode("This(string)has(parentheses)",
 "UTF-8"));
 System.out.println(URLEncoder.encode("This.string.has.periods",
 "UTF-8"));
 System.out.println(URLEncoder.encode("This=string=has=equals=signs",
 "UTF-8"));
 System.out.println(URLEncoder.encode("This&string&has&ampersands",
 "UTF-8"));
 System.out.println(URLEncoder.encode("Thiséstringéhasé
 non-ASCII characters", "UTF-8"));
 } catch (UnsupportedEncodingException ex) {
 throw new RuntimeException("Broken VM does not support UTF-8");
 }
 }
}
Here is the output (note that the code needs to be saved in something other than ASCII,
and the encoding chosen should be passed as an argument to the compiler to account
for the non-ASCII characters in the source code):
% javac -encoding UTF8 EncoderTest
% java EncoderTest
This+string+has+spaces
x-www-form-urlencoded | 151
This*string*has*asterisks
This%25string%25has%25percent%25signs
This%2Bstring%2Bhas%2Bpluses
This%2Fstring%2Fhas%2Fslashes
This%22string%22has%22quote%22marks
This%3Astring%3Ahas%3Acolons
This%7Estring%7Ehas%7Etildes
This%28string%29has%28parentheses%29
This.string.has.periods
This%3Dstring%3Dhas%3Dequals%3Dsigns
This%26string%26has%26ampersands
This%C3%A9string%C3%A9has%C3%A9non-ASCII+characters</programlisting>
Notice in particular that this method encodes the forward slash, the ampersand, the
equals sign, and the colon. It does not attempt to determine how these characters are
being used in a URL. Consequently, you have to encode URLs piece by piece rather than
encoding an entire URL in one method call. This is an important point, because the
most common use of URLEncoder is preparing query strings for communicating with
server-side programs that use GET. For example, suppose you want to encode this URL
for a Google search:
https://www.google.com/search?hl=en&as_q=Java&as_epq=I/O
This code fragment encodes it:
String query = URLEncoder.encode(
 "https://www.google.com/search?hl=en&as_q=Java&as_epq=I/O", "UTF-8");
System.out.println(query);
Unfortunately, the output is:
https%3A%2F%2Fwww.google.com%2Fsearch%3Fhl%3Den%26as_q%3DJava%26as_epq%3DI%2FO
The problem is that URLEncoder.encode() encodes blindly. It can’t distinguish between
special characters used as part of the URL or query string, like / and =, and characters
that need to be encoded. Consequently, URLs need to be encoded a piece at a time like
this:
String url = "https://www.google.com/search?";
url += URLEncoder.encode("hl", "UTF-8");
url += "=";
url += URLEncoder.encode("en", "UTF-8");
url += "&";
url += URLEncoder.encode("as_q", "UTF-8");
url += "=";
url += URLEncoder.encode("Java", "UTF-8");
url += "&";
url += URLEncoder.encode("as_epq", "UTF-8");
url += "=";
url += URLEncoder.encode("I/O", "UTF-8");
System.out.println(url);
152 | Chapter 5: URLs and URIs
The output of this is what you actually want:
https://www.google.com/search?hl=en&as_q=Java&as_epq=I/O
In this case, you could have skipped encoding several of the constant strings such as
“Java” because you know from inspection that they don’t contain any characters that
need to be encoded. However, in general, these values will be variables, not constants;
and you’ll need to encode each piece to be safe.
Example 5-8 is a QueryString class that uses URLEncoder to encode successive name
and value pairs in a Java object, which will be used for sending data to server-side
programs. To add name-value pairs, call the add() method, which takes two strings as
arguments and encodes them. The getQuery() method returns the accumulated list of
encoded name-value pairs.
Example 5-8. The QueryString class
import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
public class QueryString {
 private StringBuilder query = new StringBuilder();
 public QueryString() {
 }
 public synchronized void add(String name, String value) {
 query.append('&');
 encode(name, value);
 }
 private synchronized void encode(String name, String value) {
 try {
 query.append(URLEncoder.encode(name, "UTF-8"));
 query.append('=');
 query.append(URLEncoder.encode(value, "UTF-8"));
 } catch (UnsupportedEncodingException ex) {
 throw new RuntimeException("Broken VM does not support UTF-8");
 }
 }
 public synchronized String getQuery() {
 return query.toString();
 }
 @Override
 public String toString() {
 return getQuery();
 }
}
x-www-form-urlencoded | 153
Using this class, we can now encode the previous example:
QueryString qs = new QueryString();
qs.add("hl", "en");
qs.add("as_q", "Java");
qs.add("as_epq", "I/O");
String url = "http://www.google.com/search?" + qs;
System.out.println(url);
URLDecoder
The corresponding URLDecoder class has a static decode() method that decodes strings
encoded in x-www-form-url-encoded format. That is, it converts all plus signs to spaces
and all percent escapes to their corresponding character:
public static String decode(String s, String encoding)
 throws UnsupportedEncodingException
If you have any doubt about which encoding to use, pick UTF-8. It’s more likely to be
correct than anything else.
An IllegalArgumentException should be thrown if the string contains a percent sign
that isn’t followed by two hexadecimal digits or decodes into an illegal sequence.
Since URLDecoder does not touch non-escaped characters, you can pass an entire URL
to it rather than splitting it into pieces first. For example:
String input = "https://www.google.com/" +
 "search?hl=en&as_q=Java&as_epq=I%2FO";
String output = URLDecoder.decode(input, "UTF-8");
System.out.println(output);
Proxies
Many systems access the Web and sometimes other non-HTTP parts of the Internet
through proxy servers. A proxy server receives a request for a remote server from a local
client. The proxy server makes the request to the remote server and forwards the result
back to the local client. Sometimes this is done for security reasons, such as to prevent
remote hosts from learning private details about the local network configuration. Other
times it’s done to prevent users from accessing forbidden sites by filtering outgoing
requests and limiting which sites can be viewed. For instance, an elementary school
might want to block access to http://www.playboy.com. And still other times it’s done
purely for performance, to allow multiple users to retrieve the same popular documents
from a local cache rather than making repeated downloads from the remote server.
Java programs based on the URL class can work through most common proxy servers
and protocols. Indeed, this is one reason you might want to choose to use the URL class
rather than rolling your own HTTP or other client on top of raw sockets.
154 | Chapter 5: URLs and URIs
System Properties
For basic operations, all you have to do is set a few system properties to point to the
addresses of your local proxy servers. If you are using a pure HTTP proxy, set
http.proxyHost to the domain name or the IP address of your proxy server and
http.proxyPort to the port of the proxy server (the default is 80). There are several
ways to do this, including calling System.setProperty() from within your Java code
or using the -D options when launching the program. This example sets the proxy server
to 192.168.254.254 and the port to 9000:
<programlisting format="linespecific" id="I_7_tt264">% <userinput moreinfo=
"none">
 java -Dhttp.proxyHost=192.168.254.254 -Dhttp.proxyPort=9000 </userinput>
<emphasis role="bolditalic">com.domain.Program</emphasis></programlisting>
If the proxy requires a username and password, you’ll need to install an Authentica
tor, as we’ll discuss shortly in “Accessing Password-Protected Sites” on page 161.
If you want to exclude a host from being proxied and connect directly instead, set the
http.nonProxyHosts system property to its hostname or IP address. To exclude mul‐
tiple hosts, separate their names by vertical bars. For example, this code fragment proxies
everything except java.oreilly.com and xml.oreilly.com:
System.setProperty("http.proxyHost", "192.168.254.254");
System.setProperty("http.proxyPort", "9000");
System.setProperty("http.nonProxyHosts", "java.oreilly.com|xml.oreilly.com");
You can also use an asterisk as a wildcard to indicate that all the hosts within a particular
domain or subdomain should not be proxied. For example, to proxy everything except
hosts in the oreilly.com domain:
% java -Dhttp.proxyHost=192.168.254.254 -Dhttp.nonProxyHosts=*.oreilly.com
<emphasis role="bolditalic">com.domain.Program</emphasis></programlisting>
If you are using an FTP proxy server, set the ftp.proxyHost, ftp.proxyPort, and
ftp.nonProxyHosts properties in the same way.
Java does not support any other application layer proxies, but if you’re using a transport
layer SOCKS proxy for all TCP connections, you can identify it with the socksProxy
Host and socksProxyPort system properties. Java does not provide an option for non‐
proxying with SOCKS. It’s an all-or-nothing decision.
The Proxy Class
The Proxy class allows more fine-grained control of proxy servers from within a Java
program. Specifically, it allows you to choose different proxy servers for different remote
hosts. The proxies themselves are represented by instances of the java.net.Proxy class.
Proxies | 155
There are still only three kinds of proxies, HTTP, SOCKS, and direct connections (no
proxy at all), represented by three constants in the Proxy.Type enum:
• Proxy.Type.DIRECT
• Proxy.Type.HTTP
• Proxy.Type.SOCKS
Besides its type, the other important piece of information about a proxy is its address
and port, given as a SocketAddress object. For example, this code fragment creates a
Proxy object representing an HTTP proxy server on port 80 of proxy.example.com:
SocketAddress address = new InetSocketAddress("proxy.example.com", 80);
Proxy proxy = new Proxy(Proxy.Type.HTTP, address);
Although there are only three kinds of proxy objects, there can be many proxies of the
same type for different proxy servers on different hosts.
The ProxySelector Class
Each running virtual machine has a single java.net.ProxySelector object it uses to
locate the proxy server for different connections. The default ProxySelector merely
inspects the various system properties and the URL’s protocol to decide how to connect
to different hosts. However, you can install your own subclass of ProxySelector in place
of the default selector and use it to choose different proxies based on protocol, host,
path, time of day, or other criteria.
The key to this class is the abstract select() method:
public abstract List<Proxy> select(URI uri)
Java passes this method a URI object (not a URL object) representing the host to which
a connection is needed. For a connection made with the URL class, this object typically
has the form http://www.example.com/ or ftp://ftp.example.com/pub/files/, for example.
For a pure TCP connection made with the Socket class, this URI will have the form
socket://host:port:, for instance, socket://www.example.com:80. The ProxySelector ob‐
ject then chooses the right proxies for this type of object and returns them in a
List<Proxy>.
The second abstract method in this class you must implement is connectFailed():
public void connectFailed(URI uri, SocketAddress address, IOException ex)
This is a callback method used to warn a program that the proxy server isn’t actually
making the connection. Example 5-9 demonstrates with a ProxySelector that attempts
to use the proxy server at proxy.example.com for all HTTP connections unless the proxy
server has previously failed to resolve a connection to a particular URL. In that case, it
suggests a direct connection instead.
156 | Chapter 5: URLs and URIs
Example 5-9. A ProxySelector that remembers what it can connect to
import java.io.*;
import java.net.*;
import java.util.*;
public class LocalProxySelector extends ProxySelector {
 private List<URI> failed = new ArrayList<URI>();
 public List<Proxy> select(URI uri) {
 List<Proxy> result = new ArrayList<Proxy>();
 if (failed.contains(uri)
 || !"http".equalsIgnoreCase(uri.getScheme())) {
 result.add(Proxy.NO_PROXY);
 } else {
 SocketAddress proxyAddress
 = new InetSocketAddress( "proxy.example.com", 8000);
 Proxy proxy = new Proxy(Proxy.Type.HTTP, proxyAddress);
 result.add(proxy);
 }
 return result;
 }
 public void connectFailed(URI uri, SocketAddress address, IOException ex) {
 failed.add(uri);
 }
}
As I said, each virtual machine has exactly one ProxySelector. To change the Proxy
Selector, pass the new selector to the static ProxySelector.setDefault() method,
like so:
ProxySelector selector = new LocalProxySelector():
ProxySelector.setDefault(selector);
From this point forward, all connections opened by that virtual machine will ask the
ProxySelector for the right proxy to use. You normally shouldn’t use this in code run‐
ning in a shared environment. For instance, you wouldn’t change the ProxySelector
in a servlet because that would change the ProxySelector for all servlets running in the
same container.
Communicating with Server-Side Programs Through GET
The URL class makes it easy for Java applets and applications to communicate with serverside programs such as CGIs, servlets, PHP pages, and others that use the GET method.
(Server-side programs that use the POST method require the URLConnection class and
Communicating with Server-Side Programs Through GET | 157
are discussed in Chapter 7.) All you need to know is what combination of names and
values the program expects to receive. Then you can construct a URL with a query string
that provides the requisite names and values. All names and values must be x-wwwform-url-encoded—as by the URLEncoder.encode() method, discussed earlier in this
chapter.
There are a number of ways to determine the exact syntax for a query string that talks
to a particular program. If you’ve written the server-side program yourself, you already
know the name-value pairs it expects. If you’ve installed a third-party program on your
own server, the documentation for that program should tell you what it expects. If you’re
talking to a documented external network API such as the eBay Shopping API, then the
service usually provides fairly detailed documentation to tell you exactly what data to
send for which purposes.
Many programs are designed to process form input. If this is the case, it’s straightforward
to figure out what input the program expects. The method the form uses should be the
value of the METHOD attribute of the FORM element. This value should be either GET, in
which case you use the process described here, or POST, in which case you use the process
described in Chapter 7. The part of the URL that precedes the query string is given by
the value of the ACTION attribute of the FORM element. Note that this may be a relative
URL, in which case you’ll need to determine the corresponding absolute URL. Finally,
the names in the name-value pairs are simply the values of the NAME attributes of the
INPUT elements. The values of the pairs are whatever the user types into the form.
For example, consider this HTML form for the local search engine on my Cafe con
Leche site. You can see that it uses the GET method. The program that processes the form
is accessed via the URL http://www.google.com/search. It has four separate name-value
pairs, three of which have default values:
<form name="search" action="http://www.google.com/search" method="get">
 <input name="q" />
 <input type="hidden" value="cafeconleche.org" name="domains" />
 <input type="hidden" name="sitesearch" value="cafeconleche.org" />
 <input type="hidden" name="sitesearch2" value="cafeconleche.org" />
 <br />
 <input type="image" height="22" width="55"
 src="images/search_blue.gif" alt="search" border="0"
 name="search-image" />
</form>
The type of the INPUT field doesn’t matter. For instance, it doesn’t matter if it’s a set of
checkboxes, a pop-up list, or a text field. Only the name of each INPUT field and the
value you give it is significant. The submit input tells the web browser when to send the
data but does not give the server any extra information. Sometimes you find hidden
INPUT fields that must have particular required default values. This form has three hid‐
den INPUT fields. There are many different form tags in HTML that produce pop-up
158 | Chapter 5: URLs and URIs
menus, radio buttons, and more. However, although these input widgets appear differ‐
ent to the user, the format of data they send to the server is the same. Each form element
provides a name and an encoded string value.
In some cases, the program you’re talking to may not be able to handle arbitrary text
strings for values of particular inputs. However, since the form is meant to be read and
filled in by human beings, it should provide sufficient clues to figure out what input is
expected; for instance, that a particular field is supposed to be a two-letter state abbre‐
viation or a phone number. Sometimes the inputs may not have such obvious names.
There may not even be a form, just links to follow. In this case, you have to do some
experimenting, first copying some existing values and then tweaking them to see what
values are and aren’t accepted. You don’t need to do this in a Java program. You can
simply edit the URL in the address or location bar of your web browser window.
The likelihood that other hackers may experiment with your own
server-side programs in such a fashion is a good reason to make them
extremely robust against unexpected input.
Regardless of how you determine the set of name-value pairs the server expects, com‐
municating with it once you know them is simple. All you have to do is create a query
string that includes the necessary name-value pairs, then form a URL that includes that
query string. Send the query string to the server and read its response using the same
methods you use to connect to a server and retrieve a static HTML page. There’s no
special protocol to follow once the URL is constructed. (There is a special protocol to
follow for the POST method, however, which is why discussion of that method will have
to wait until Chapter 7.)
To demonstrate this procedure, let’s write a very simple command-line program to look
up topics in the Open Directory. This site is shown in Figure 5-1 and it has the advantage
of being really simple.
Communicating with Server-Side Programs Through GET | 159
Figure 5-1. The user interface for the Open Directory
The Open Directory interface is a simple form with one input field named search; input
typed in this field is sent to a program at http://search.dmoz.org/cgi-bin/search, which
does the actual search. The HTML for the form looks like this:
<form class="center mb1em" action="search" method="GET">
 <input style="*vertical-align:middle;" size="45" name="q" value="" class="qN">
 <input style="*vertical-align:middle; *padding-top:1px;" value="Search"
 class="btn" type="submit">
 <a href="search?type=advanced"><span class="advN">advanced</span></a>
</form>
There are only two input fields in this form: the Submit button and a text field named
q. Thus, to submit a search request to the Open Directory, you just need to append
q=searchTerm to http://www.dmoz.org/search. For example, to search for “java”, you
160 | Chapter 5: URLs and URIs
would open a connection to the URL http://www.dmoz.org/search/?q=java and read the
resulting input stream. Example 5-10 does exactly this.
Example 5-10. Do an Open Directory search
import java.io.*;
import java.net.*;
public class DMoz {
 public static void main(String[] args) {
 String target = "";
 for (int i = 0; i < args.length; i++) {
 target += args[i] + " ";
 }
 target = target.trim();
 QueryString query = new QueryString();
 query.add("q", target);
 try {
 URL u = new URL("http://www.dmoz.org/search/q?" + query);
 try (InputStream in = new BufferedInputStream(u.openStream())) {
 InputStreamReader theHTML = new InputStreamReader(in);
 int c;
 while ((c = theHTML.read()) != -1) {
 System.out.print((char) c);
 }
 }
 } catch (MalformedURLException ex) {
 System.err.println(ex);
 } catch (IOException ex) {
 System.err.println(ex);
 }
 }
}
```

# 9. Sockets for Servers

The previous chapter discussed sockets from the standpoint of clients: programs that
open a socket to a server that’s listening for connections. However, client sockets them‐
selves aren’t enough; clients aren’t much use unless they can talk to a server, and the
Socket class discussed in the previous chapter is not sufficient for writing servers. To
create a Socket, you need to know the Internet host to which you want to connect.
When you’re writing a server, you don’t know in advance who will contact you; and even
if you did, you wouldn’t know when that host wanted to contact you. In other words,
servers are like receptionists who sit by the phone and wait for incoming calls. They
don’t know who will call or when, only that when the phone rings, they have to pick it
up and talk to whoever is there. You can’t program that behavior with the Socket class
alone.
For servers that accept connections, Java provides a ServerSocket class that represents
server sockets. In essence, a server socket’s job is to sit by the phone and wait for in‐
coming calls. More technically, a server socket runs on the server and listens for in‐
coming TCP connections. Each server socket listens on a particular port on the server
machine. When a client on a remote host attempts to connect to that port, the server
wakes up, negotiates the connection between the client and the server, and returns a
regular Socket object representing the socket between the two hosts. In other words,
server sockets wait for connections while client sockets initiate connections. Once a
ServerSocket has set up the connection, the server uses a regular Socket object to send
data to the client. Data always travels over the regular socket.
Using ServerSockets
The ServerSocket class contains everything needed to write servers in Java. It has con‐
structors that create new ServerSocket objects, methods that listen for connections on
283
a specified port, methods that configure the various server socket options, and the usual
miscellaneous methods such as toString().
In Java, the basic life cycle of a server program is this:
1. A new ServerSocket is created on a particular port using a ServerSocket() con‐
structor.
2. The ServerSocket listens for incoming connection attempts on that port using its
accept() method. accept() blocks until a client attempts to make a connection,
at which point accept() returns a Socket object connecting the client and the
server.
3. Depending on the type of server, either the Socket’s getInputStream() method,
getOutputStream() method, or both are called to get input and output streams
that communicate with the client.
4. The server and the client interact according to an agreed-upon protocol until it is
time to close the connection.
5. The server, the client, or both close the connection.
6. The server returns to step 2 and waits for the next connection.
Let’s demonstrate with one of the simpler protocols, daytime. Recall from the Chap‐
ter 8 that a daytime server listens on port 13. When a client connects, the server sends
the time in a human-readable format and closes the connection. For example, here’s a
connection to the daytime server at time-a.nist.gov:
$ telnet time-a.nist.gov 13
Trying 129.6.15.28...
Connected to time-a.nist.gov.
Escape character is '^]'.
56375 13-03-24 13:37:50 50 0 0 888.8 UTC(NIST) *
Connection closed by foreign host.
Implementing your own daytime server is easy. First, create a server socket that listens
on port 13:
ServerSocket server = new ServerSocket(13);
Next, accept a connection:
Socket connection = server.accept();
The accept() call blocks. That is, the program stops here and waits, possibly for hours
or days, until a client connects on port 13. When a client does connect, the accept()
method returns a Socket object.
284 | Chapter 9: Sockets for Servers
Note that the connection is returned a java.net.Socket object, the same as you used
for clients in the previous chapter. The daytime protocol requires the server (and only
the server) to talk, so get an OutputStream from the socket. Because the daytime protocol
requires text, chain this to an OutputStreamWriter:
OutputStream out = connection.getOutputStream();
Writer writer = new OutputStreamWriter(writer, "ASCII");
Now get the current time and write it onto the stream. The daytime protocol doesn’t
require any particular format other than that it be human readable, so let Java pick for
you:
Date now = new Date();
out.write(now.toString() +"\r\n");
Do note, however, the use of a carriage return/linefeed pair to terminate the line. This
is almost always what you want in a network server. You should explicitly choose this
rather than using the system line separator, whether explicitly with System.getProper
ty("line.separator") or implicitly via a method such as println().
Finally, flush the connection and close it:
out.flush();
connection.close();
You won’t always have to close the connection after just one write. Many protocols, dict
and HTTP 1.1 for instance, allow clients to send multiple requests over a single socket
and expect the server to send multiple responses. Some protocols such as FTP can even
hold a socket open indefinitely. However, the daytime protocol only allows a single
response.
If the client closes the connection while the server is still operating, the input and/or
output streams that connect the server to the client throw an InterruptedIOExcep
tion on the next read or write. In either case, the server should then get ready to process
the next incoming connection.
Of course, you’ll want to do all this repeatedly, so you’ll put this all inside a loop. Each
pass through the loop invokes the accept() method once. This returns a Socket object
representing the connection between the remote client and the local server. Interaction
with the client takes place through this Socket object. For example:
ServerSocket server = new ServerSocket(port);
while (true) {
 try (Socket connection = server.accept()) {
 Writer out = new OutputStreamWriter(connection.getOutputStream());
 Date now = new Date();
 out.write(now.toString() +"\r\n");
 out.flush();
 } catch (IOException ex) {
 // problem with one client; don't shut down the server
Using ServerSockets | 285
 System.err.println(ex.getMessage());
 }
}
This is called an iterative server. There’s one big loop, and in each pass through the loop
a single connection is completely processed. This works well for a very simple protocol
with very small requests and responses like daytime, though even with this simple a
protocol it’s possible for one slow client to delay other faster clients. Upcoming examples
will address this with multiple threads or asynchronous I/O.
When exception handling is added, the code becomes somewhat more convoluted. It’s
important to distinguish between exceptions that should probably shut down the server
and log an error message, and exceptions that should just close that active connection.
Exceptions within the scope of a particular connection should close that connection,
but not affect other connections or shut down the server. Exceptions outside the scope
of an individual request probably should shut down the server. To organize this, nest
the try blocks:
ServerSocket server = null;
try {
 server = new ServerSocket(port);
 while (true) {
 Socket connection = null;
 try {
 connection = server.accept();
 Writer out = new OutputStreamWriter(connection.getOutputStream());
 Date now = new Date();
 out.write(now.toString() +"\r\n");
 out.flush();
 connection.close();
 } catch (IOException ex) {
 // this request only; ignore
 } finally {
 try {
 if (connection != null) connection.close();
 } catch (IOException ex) {}
 }
 }
} catch (IOException ex) {
 ex.printStackTrace();
} finally {
 try {
 if (server != null) server.close();
 } catch (IOException ex) {}
}
Always close a socket when you’re finished with it. In Chapter 8, I said that a client
shouldn’t rely on the other side of a connection to close the socket; that goes triple for
servers. Clients time out or crash; users cancel transactions; networks go down in hightraffic periods; hackers launch denial-of-service attacks. For any of these or a hundred
286 | Chapter 9: Sockets for Servers
more reasons, you cannot rely on clients to close sockets, even when the protocol re‐
quires them to, which this one doesn’t.
Example 9-1 puts this all together. It uses Java 7’s try-with-resources to autoclose the
sockets.
Example 9-1. A daytime server
import java.net.*;
import java.io.*;
import java.util.Date;
public class DaytimeServer {
 public final static int PORT = 13;
 public static void main(String[] args) {
 try (ServerSocket server = new ServerSocket(PORT)) {
 while (true) {
 try (Socket connection = server.accept()) {
 Writer out = new OutputStreamWriter(connection.getOutputStream());
 Date now = new Date();
 out.write(now.toString() +"\r\n");
 out.flush();
 connection.close();
 } catch (IOException ex) {}
 }
 } catch (IOException ex) {
 System.err.println(ex);
 }
 }
}
The class has a single method, main(), which does all the work. The outer try block
traps any IOExceptions that may arise while the ServerSocket object server is con‐
structed on the daytime port. The inner try block watches for exceptions thrown while
the connections are accepted and processed. The accept() method is called within an
infinite loop to watch for new connections; like many servers, this program never ter‐
minates but continues listening until an exception is thrown or you stop it manually.
The command for stopping a program manually depends on your
system; under Unix, Windows, and many other systems, Ctrl-C will
do the job. If you are running the server in the background on a Unix
system, stop it by finding the server’s process ID and killing it with the
kill command (kill pid ).
Using ServerSockets | 287
When a client connects, accept() returns a Socket, which is stored in the local variable
connection, and the program continues. It calls getOutputStream() to get the output
stream associated with that Socket and then chains that output stream to a new Out
putStreamWriter, out. A new Date object provides the current time. The content is
sent to the client by writing its string representation on out with write().
Connecting from Telnet, you should see something like this:
$ telnet localhost 13
Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.
Sat Mar 30 16:15:10 EDT 2013
Connection closed by foreign host
If you run this program on Unix (including Linux and Mac OS X), you
need to run it as root in order to connect to port 13. If you don’t want
to or can’t run it as root, change the port number to something above
1024—say, 1313.
Serving Binary Data
Sending binary, nontext data is not significantly harder. You just use an Output
Stream that writes a byte array rather than a Writer that writes a String. Example 9-2
demonstrates with an iterative time server that follows the time protocol outlined in
RFC 868. When a client connects, the server sends a 4-byte, big-endian, unsigned integer
specifying the number of seconds that have passed since 12:00 A.M., January 1, 1900,
GMT (the epoch). Once again, the current time is found by creating a new Date object.
However, because Java’s Date class counts milliseconds since 12:00 A.M., January 1,
1970, GMT rather than seconds since 12:00 A.M., January 1, 1900, GMT, some con‐
version is necessary.
Example 9-2. A time server
import java.io.*;
import java.net.*;
import java.util.Date;
public class TimeServer {
 public final static int PORT = 37;
 public static void main(String[] args) {
 // The time protocol sets the epoch at 1900,
 // the Date class at 1970. This number
 // converts between them.
288 | Chapter 9: Sockets for Servers
 long differenceBetweenEpochs = 2208988800L;
 try (ServerSocket server = new ServerSocket(PORT)) {
 while (true) {
 try (Socket connection = server.accept()) {
 OutputStream out = connection.getOutputStream();
 Date now = new Date();
 long msSince1970 = now.getTime();
 long secondsSince1970 = msSince1970/1000;
 long secondsSince1900 = secondsSince1970
 + differenceBetweenEpochs;
 byte[] time = new byte[4];
 time[0]
 = (byte) ((secondsSince1900 & 0x00000000FF000000L) >> 24);
 time[1]
 = (byte) ((secondsSince1900 & 0x0000000000FF0000L) >> 16);
 time[2]
 = (byte) ((secondsSince1900 & 0x000000000000FF00L) >> 8);
 time[3] = (byte) (secondsSince1900 & 0x00000000000000FFL);
 out.write(time);
 out.flush();
 } catch (IOException ex) {
 System.err.println(ex.getMessage());
 }
 }
 } catch (IOException ex) {
 System.err.println(ex);
 }
 }
}
As with the TimeClient of the previous chapter, most of the effort here goes into working
with a data format (32-bit unsigned integers) that Java doesn’t natively support.
Multithreaded Servers
Daytime and time are both very quick protocols. The server sends a few dozen bytes at
most and then closes the connection. It’s plausible here to process each connection fully
before moving on to the next one. Even in that case, though, it is possible that a slow or
crashed client might hang the server for a few seconds until it notices the socket is
broken. If the sending of data can take a significant amount of time even when client
and server are behaving, you really don’t want each connection to wait for the next.
Old-fashioned Unix servers such as wu-ftpd create a new process to handle each con‐
nection so that multiple clients can be serviced at the same time. Java programs should
spawn a thread to interact with the client so that the server can be ready to process the
next connection sooner. A thread places a far smaller load on the server than a complete
child process. In fact, the overhead of forking too many processes is why the typical
Unix FTP server can’t handle more than roughly 400 connections without slowing to a
Using ServerSockets | 289
crawl. On the other hand, if the protocol is simple and quick and allows the server to
close the connection when it’s through, it will be more efficient for the server to process
the client request immediately without spawning a thread.
The operating system stores incoming connection requests addressed to a particular
port in a first-in, first-out queue. By default, Java sets the length of this queue to 50,
although it can vary from operating system to operating system. Some operating systems
(not Solaris) have a maximum queue length. For instance, on FreeBSD, the default
maximum queue length is 128. On these systems, the queue length for a Java server
socket will be the largest operating-system allowed value less than or equal to 50. After
the queue fills to capacity with unprocessed connections, the host refuses additional
connections on that port until slots in the queue open up. Many (though not all) clients
will try to make a connection multiple times if their initial attempt is refused. Several
ServerSocket constructors allow you to change the length of the queue if its default
length isn’t large enough. However, you won’t be able to increase the queue beyond the
maximum size that the operating system supports. Whatever the queue size, though,
you want to be able to empty it faster than new connections are coming in, even if it
takes a while to process each connection.
The solution here is to give each connection its own thread, separate from the thread
that accepts incoming connections into the queue. For instance, Example 9-3 is a day‐
time server that spawns a new thread to handle each incoming connection. This prevents
one slow client from blocking all the other clients. This is a thread per connection design.
Example 9-3. A multithreaded daytime server
import java.net.*;
import java.io.*;
import java.util.Date;
public class MultithreadedDaytimeServer {
 public final static int PORT = 13;
 public static void main(String[] args) {
 try (ServerSocket server = new ServerSocket(PORT)) {
 while (true) {
 try {
 Socket connection = server.accept();
 Thread task = new DaytimeThread(connection);
 task.start();
 } catch (IOException ex) {}
 }
 } catch (IOException ex) {
 System.err.println("Couldn't start server");
 }
 }
 private static class DaytimeThread extends Thread {
290 | Chapter 9: Sockets for Servers
 private Socket connection;
 DaytimeThread(Socket connection) {
 this.connection = connection;
 }
 @Override
 public void run() {
 try {
 Writer out = new OutputStreamWriter(connection.getOutputStream());
 Date now = new Date();
 out.write(now.toString() +"\r\n");
 out.flush();
 } catch (IOException ex) {
 System.err.println(ex);
 } finally {
 try {
 connection.close();
 } catch (IOException e) {
 // ignore;
 }
 }
 }
 }
}
Example 9-3 uses try-with-resources to autoclose the server socket. However, it delib‐
erately does not use try-with-resources for the client sockets accepted by the server
socket. This is because the client socket escapes from the try block into a separate thread.
If you used try-with-resources, the main thread would close the socket as soon as it got
to the end of the while loop, likely before the spawned thread had finished using it.
There’s actually a denial-of-service attack on this server though. Because Example 9-3
spawns a new thread for each connection, numerous roughly simultaneous incoming
connections can cause it to spawn an indefinite number of threads. Eventually, the Java
virtual machine will run out of memory and crash. A better approach is to use a fixed
thread pool as described in Chapter 3 to limit the potential resource usage. Fifty threads
should be plenty. Example 9-4 shouldn’t crash no matter what load it’s under. It may
start refusing connections, but it won’t crash.
Example 9-4. A daytime server using a thread pool
import java.io.*;
import java.net.*;
import java.util.*;
import java.util.concurrent.*;
public class PooledDaytimeServer {
Using ServerSockets | 291
 public final static int PORT = 13;
 public static void main(String[] args) {
 ExecutorService pool = Executors.newFixedThreadPool(50);
 try (ServerSocket server = new ServerSocket(PORT)) {
 while (true) {
 try {
 Socket connection = server.accept();
 Callable<Void> task = new DaytimeTask(connection);
 pool.submit(task);
 } catch (IOException ex) {}
 }
 } catch (IOException ex) {
 System.err.println("Couldn't start server");
 }
 }
 private static class DaytimeTask implements Callable<Void> {
 private Socket connection;
 DaytimeTask(Socket connection) {
 this.connection = connection;
 }
 @Override
 public Void call() {
 try {
 Writer out = new OutputStreamWriter(connection.getOutputStream());
 Date now = new Date();
 out.write(now.toString() +"\r\n");
 out.flush();
 } catch (IOException ex) {
 System.err.println(ex);
 } finally {
 try {
 connection.close();
 } catch (IOException e) {
 // ignore;
 }
 }
 return null;
 }
 }
}
Example 9-4 is structured much like Example 9-3. The single difference is that it uses a
Callable rather than a Thread subclass, and rather than starting threads it submits these
callables to an executor service preconfigured with 50 threads.
292 | Chapter 9: Sockets for Servers
Writing to Servers with Sockets
In the examples so far, the server has only written to client sockets. It hasn’t read from
them. Most protocols, however, require the server to do both. This isn’t hard. You’ll
accept a connection as before, but this time ask for both an InputStream and an Out
putStream. Read from the client using the InputStream and write to it using the Out
putStream. The main trick is understanding the protocol: when to write and when to
read.
The echo protocol, defined in RFC 862, is one of the simplest interactive TCP services.
The client opens a socket to port 7 on the echo server and sends data. The server sends
the data back. This continues until the client closes the connection. The echo protocol
is useful for testing the network to make sure that data is not mangled by a misbehaving
router or firewall. You can test echo with Telnet like this:
$ telnet rama.poly.edu 7
Trying 128.238.10.212...
Connected to rama.poly.edu.
Escape character is '^]'.
This is a test
This is a test
This is another test
This is another test
9876543210
9876543210
^]
telnet> close
Connection closed.
This sample is line oriented because that’s how Telnet works. It reads a line of input from
the console, sends it to the server, then waits to read a line of output it gets back. However,
the echo protocol doesn’t require this. It echoes each byte as it receives it. It doesn’t really
care whether those bytes represent characters in some encoding or are divided into lines.
Unlike many protocols, echo does not specify lockstep behavior where the client sends
a request but then waits for the full server response before sending any more data.
Unlike daytime and time, in the echo protocol the client is responsible for closing the
connection. This makes it even more important to support asynchronous operation
with many threads because a single client can remain connected indefinitely. In
Example 9-5, the server spawns up to 500 threads.
Example 9-5. An echo server
import java.nio.*;
import java.nio.channels.*;
import java.net.*;
import java.util.*;
import java.io.IOException;
Using ServerSockets | 293
public class EchoServer {
 public static int DEFAULT_PORT = 7;
 public static void main(String[] args) {
 int port;
 try {
 port = Integer.parseInt(args[0]);
 } catch (RuntimeException ex) {
 port = DEFAULT_PORT;
 }
 System.out.println("Listening for connections on port " + port);
 ServerSocketChannel serverChannel;
 Selector selector;
 try {
 serverChannel = ServerSocketChannel.open();
 ServerSocket ss = serverChannel.socket();
 InetSocketAddress address = new InetSocketAddress(port);
 ss.bind(address);
 serverChannel.configureBlocking(false);
 selector = Selector.open();
 serverChannel.register(selector, SelectionKey.OP_ACCEPT);
 } catch (IOException ex) {
 ex.printStackTrace();
 return;
 }
 while (true) {
 try {
 selector.select();
 } catch (IOException ex) {
 ex.printStackTrace();
 break;
 }
 Set<SelectionKey> readyKeys = selector.selectedKeys();
 Iterator<SelectionKey> iterator = readyKeys.iterator();
 while (iterator.hasNext()) {
 SelectionKey key = iterator.next();
 iterator.remove();
 try {
 if (key.isAcceptable()) {
 ServerSocketChannel server = (ServerSocketChannel) key.channel();
 SocketChannel client = server.accept();
 System.out.println("Accepted connection from " + client);
 client.configureBlocking(false);
 SelectionKey clientKey = client.register(
 selector, SelectionKey.OP_WRITE | SelectionKey.OP_READ);
 ByteBuffer buffer = ByteBuffer.allocate(100);
 clientKey.attach(buffer);
294 | Chapter 9: Sockets for Servers
 }
 if (key.isReadable()) {
 SocketChannel client = (SocketChannel) key.channel();
 ByteBuffer output = (ByteBuffer) key.attachment();
 client.read(output);
 }
 if (key.isWritable()) {
 SocketChannel client = (SocketChannel) key.channel();
 ByteBuffer output = (ByteBuffer) key.attachment();
 output.flip();
 client.write(output);
 output.compact();
 }
 } catch (IOException ex) {
 key.cancel();
 try {
 key.channel().close();
 } catch (IOException cex) {}
 }
 }
 }
 }
}
Closing Server Sockets
If you’re finished with a server socket, you should close it, especially if the program is
going to continue to run for some time. This frees up the port for other programs that
may wish to use it. Closing a ServerSocket should not be confused with closing a
Socket. Closing a ServerSocket frees a port on the local host, allowing another server
to bind to the port; it also breaks all currently open sockets that the ServerSocket has
accepted.
Server sockets are closed automatically when a program dies, so it’s not absolutely nec‐
essary to close them in programs that terminate shortly after the ServerSocket is no
longer needed. Nonetheless, it doesn’t hurt. Programmers often follow the same closeif-not-null pattern in a try-finally block that you’re already familiar with from streams
and client-side sockets:
ServerSocket server = null;
try {
 server = new ServerSocket(port);
 // ... work with the server socket
} finally {
 if (server != null) {
 try {
 server.close();
 } catch (IOException ex) {
 // ignore
 }
Using ServerSockets | 295
 }
}
You can improve this slightly by using the noargs ServerSocket() constructor, which
does not throw any exceptions and does not bind to a port. Instead, you call the bind()
method to bind to a socket address after the ServerSocket() object has been construc‐
ted:
ServerSocket server = new ServerSocket();
try {
 SocketAddress address = new InetSocketAddress(port);
 server.bind(address);
 // ... work with the server socket
} finally {
 try {
 server.close();
 } catch (IOException ex) {
 // ignore
 }
}
In Java 7, ServerSocket implements AutoCloseable so you can take advantage of trywith-resources instead:
try (ServerSocket server = new ServerSocket(port)) {
 // ... work with the server socket
}
After a server socket has been closed, it cannot be reconnected, even to the same port.
The isClosed() method returns true if the ServerSocket has been closed, false if it
hasn’t:
public boolean isClosed()
ServerSocket objects that were created with the noargs ServerSocket() constructor
and not yet bound to a port are not considered to be closed. Invoking isClosed() on
these objects returns false. The isBound() method tells you whether the ServerSock
et has been bound to a port:
public boolean isBound()
As with the isBound() method of the Socket class discussed in the Chapter 8, the name
is a little misleading. isBound() returns true if the ServerSocket has ever been bound
to a port, even if it’s currently closed. If you need to test whether a ServerSocket is open,
you must check both that isBound() returns true and that isClosed() returns false.
For example:
public static boolean isOpen(ServerSocket ss) {
 return ss.isBound() && !ss.isClosed();
}
296 | Chapter 9: Sockets for Servers
Logging
Servers run unattended for long periods of time. It’s often important to debug what
happened when in a server long after the fact. For this reason, it’s advisable to store
server logs for at least some period of time.
What to Log
There are two primary things you want to store in your logs:
• Requests
• Server errors
Indeed, servers often keep two different logfiles for these two different items. The audit
log usually contains one entry for each connection made to the server. Servers that
perform multiple operations per connection may have one entry per operation instead.
For instance, a dict server might log one entry for each word a client looks up.
The error log contains mostly unexpected exceptions that occurred while the server was
running. For instance, any NullPointerException that happens should be logged here
because it indicates a bug in the server you’ll need to fix. The error log does not contain
client errors, such as a client that unexpectedly disconnects or sends a malformed re‐
quest. These go into the request log. The error log is exclusively for unexpected excep‐
tions.
The general rule of thumb for error logs is that every line in the error log should be
looked at and resolved. The ideal number of entries in an error log is zero. Every entry
in this log represents a bug to be investigated and resolved. If investigation of an error
log entry ends with the decision that that exception is not really a problem, and the code
is working as intended, remove the log statement. Error logs that fill up with too many
false alarms rapidly become ignored and useless.
For the same reason, do not keep debug logs in production. Do not log every time you
enter a method, every time a condition is met, and so on. No one ever looks at these
logs. They just waste space and hide real problems. If you need method-level logging
for debugging, put it in a separate file, and turn it off in the global properties file when
running in production.
More advanced logging systems provide log analysis tools that enable you to do things
like show only messages with priority INFO or higher, or only show messages that
originated from a certain part of the code. These tools make it more feasible to keep a
single logfile or database, perhaps even share one log among many different binaries or
programs. Nonetheless, the principle still applies that a log record no one will ever look
at is worthless at best and more often than not distracting or confusing.
Logging | 297
Do not follow the common antipattern of logging everything you can think of just in
case someone might need it someday. In practice, programmers are terrible at guessing
in advance which log messages they might need for debugging production problems.
Once a problem occurs, it is sometimes obvious what messages you need; but it is rare
to be able to anticipate this in advance. Adding “just in case” messages to logfiles usually
means that when a problem does occur, you’re frantically hunting for the relevant mes‐
sages in an even bigger sea of irrelevant data.
How to Log
Many legacy programs dating back to Java 1.3 and earlier still use third-party logging
libraries such as log4j or Apache Commons Logging, but the java.util.logging
package available since Java 1.4 suffices for most needs. Choosing it avoids a lot of
complex third-party dependencies.
Although you can load a logger on demand, it’s usually easiest to just create one per
class like so:
private final static Logger auditLogger = Logger.getLogger("requests");
Loggers are thread safe, so there’s no problem storing them in a shared static field.
Indeed, they almost have to be because even if the Logger object were not shared between
threads, the logfile or database would be. This is important in highly multithreaded
servers.
This example outputs to a log named “requests.” Multiple Logger objects can output to
the same log, but each logger always logs to exactly one log. What and where the log is
depends on external configuration. Most commonly it’s a file, which may or may not be
named “requests”; but it can be a database, a SOAP service running on a different server,
another Java program on the same host, or something else.
Once you have a logger, you can write to it using any of several methods. The most basic
is log(). For example, this catch block logs an unexpected runtime exception at the
highest level:
catch (RuntimeException ex) {
 logger.log(Level.SEVERE, "unexpected error " + ex.getMessage(), ex);
}
Including the exception instead of just a message is optional but customary when log‐
ging from a catch block.
There are seven levels defined as named constants in java.util.logging.Level in
descending order of seriousness:
• Level.SEVERE (highest value)
• Level.WARNING
298 | Chapter 9: Sockets for Servers
• Level.INFO
• Level.CONFIG
• Level.FINE
• Level.FINER
• Level.FINEST (lowest value)
I use info for audit logs and warning or severe for error logs. Lower levels are for de‐
bugging only and should not be used in production systems. Info, severe, and warning
all have convenience helper methods that log at that level. For example, this statement
logs a hit including the date and the remote address:
logger.info(new Date() + " " + connection.getRemoteSocketAddress());
You can use any format that’s convenient for the individual log records. Generally, each
record should contain a timestamp, the client address, and any information specific to
the request that was being processed. If the log message represents an error, include the
specific exception that was thrown. Java fills in the location in the code where the mes‐
sage was logged automatically, so you don’t need to worry about that.
Example 9-6 demonstrates by adding logging to the daytime server.
Example 9-6. A daytime server that logs requests and errors
import java.io.*;
import java.net.*;
import java.util.Date;
import java.util.concurrent.*;
import java.util.logging.*;
public class LoggingDaytimeServer {
 public final static int PORT = 13;
 private final static Logger auditLogger = Logger.getLogger("requests");
 private final static Logger errorLogger = Logger.getLogger("errors");
 public static void main(String[] args) {
 ExecutorService pool = Executors.newFixedThreadPool(50);
 try (ServerSocket server = new ServerSocket(PORT)) {
 while (true) {
 try {
 Socket connection = server.accept();
 Callable<Void> task = new DaytimeTask(connection);
 pool.submit(task);
 } catch (IOException ex) {
 errorLogger.log(Level.SEVERE, "accept error", ex);
 } catch (RuntimeException ex) {
 errorLogger.log(Level.SEVERE, "unexpected error " + ex.getMessage(), ex);
Logging | 299
 }
 }
 } catch (IOException ex) {
 errorLogger.log(Level.SEVERE, "Couldn't start server", ex);
 } catch (RuntimeException ex) {
 errorLogger.log(Level.SEVERE, "Couldn't start server: " + ex.getMessage(), ex);
 }
 }
 private static class DaytimeTask implements Callable<Void> {
 private Socket connection;
 DaytimeTask(Socket connection) {
 this.connection = connection;
 }
 @Override
 public Void call() {
 try {
 Date now = new Date();
 // write the log entry first in case the client disconnects
 auditLogger.info(now + " " + connection.getRemoteSocketAddress());
 Writer out = new OutputStreamWriter(connection.getOutputStream());
 out.write(now.toString() +"\r\n");
 out.flush();
 } catch (IOException ex) {
 // client disconnected; ignore;
 } finally {
 try {
 connection.close();
 } catch (IOException ex) {
 // ignore;
 }
 }
 return null;
 }
 }
}
As well as logging, Example 9-6 has also added catch blocks for RuntimeException that
cover most of the code and all of the network connections. This is strongly advisable in
network servers. The last thing you want is for your entire server to fall down just because
one request went down an unplanned code path and threw an IllegalArgumentExcep
tion. Usually when this happens that request is going to fail, but you can continue
processing other requests. If you’re even more careful, you can send the client the ap‐
propriate error response. In HTTP, this would be a 500 internal server error.
Not every exception automatically turns into an error log entry. For example, if a client
disconnects while you’re writing the time, that’s an IOException. However, it’s not a bug
300 | Chapter 9: Sockets for Servers
or a server error, so it isn’t written to the error log. In some situations, you might want
to log it in the audit log, or a third location. However, remember the golden rule of
logging: if no one’s going to look at it, don’t log it. Unless you really plan to investigate
and do something about client disconnects, don’t bother to record them.
By default, the logs are just output to the console. For example, here’s the output from
the preceding server when I connected to it a few times in quick succession:
Apr 13, 2013 8:54:50 AM LoggingDaytimeServer$DaytimeTask call
INFO: Sat Apr 13 08:54:50 EDT 2013 /0:0:0:0:0:0:0:1:56665
Apr 13, 2013 8:55:08 AM LoggingDaytimeServer$DaytimeTask call
INFO: Sat Apr 13 08:55:08 EDT 2013 /0:0:0:0:0:0:0:1:56666
Apr 13, 2013 8:55:16 AM LoggingDaytimeServer$DaytimeTask call
INFO: Sat Apr 13 08:55:16 EDT 2013 /0:0:0:0:0:0:0:1:56667
You’ll want to configure the runtime environment such that logs go to a more permanent
destination. Although you can specify this in code, it’s usually advisable to set this up
in a configuration file so log locations can be changed without recompiling.
The java.util.logging.config.file system property points to a file in the normal
properties format that controls the logging. You set this property by passing the
-Djava.util.logging.config.file=_filename_ argument when launching the vir‐
tual machine. For instance, in Mac OS X, it might be set in the VMOptions in the In‐
fo.plist file:
<key>Java</key>
<dict>
 <key>VMOptions</key>
 <array>
 <string>-Djava.util.logging.config.file=/opt/daytime/logging.properties
 </string>
 </array>
</dict>
Example 9-7 is a sample logging properties file that specifies:
• Logs should be written to a file.
• The requests log should be in /var/logs/daytime/requests.log at level Info.
• The errors log should be in /var/logs/daytime/requests.log at level Severe.
• Limit the log size to about 10 megabytes, then rotate.
• Keep two logs: the current one and the previous one.
• Use the basic text formatter (not XML).
• Each line of the logfile should be in the form level message timestamp.
Logging | 301
Example 9-7. A logging properties file
handlers=java.util.logging.FileHandler
java.util.logging.FileHandler.pattern = /var/logs/daytime/requests.log
java.util.logging.FileHandler.limit = 10000000
java.util.logging.FileHandler.count = 2
java.util.logging.FileHandler.formatter = java.util.logging.SimpleFormatter
java.util.logging.FileHandler.append = true
java.util.logging.SimpleFormatter.format=%4$s: %5$s [%1$tc]%n
requests.level = INFO
audit.level = SEVERE
Here’s some typical log output (note that it looks like the timestamp is doubled in request
messages because the log message also includes the current time; this would not typically
be the case for a server whose purpose was anything other than serving the current
time):
SEVERE: Couldn't start server [Sat Apr 13 10:07:01 EDT 2013]
INFO: Sat Apr 13 10:08:05 EDT 2013 /0:0:0:0:0:0:0:1:57275
 [Sat Apr 13 10:08:05 EDT 2013]
INFO: Sat Apr 13 10:08:06 EDT 2013 /0:0:0:0:0:0:0:1:57276
 [Sat Apr 13 10:08:06 EDT 2013]
The one thing I don’t like about the Java Logging API is that it doesn’t
give you an easy way to specify by configuration alone that different
messages belong in different logs. For instance, you can’t easily sepa‐
rate your error and your audit log. It can be done, but it requires you
to define a new subclass of FileHandler for each separate log so you
can assign it a new file.
Finally, once you’ve configured your servers with logging, don’t forget to look in them,
especially the error logs. There’s no point to a logfile no one ever looks at. You’ll also
want to plan for and implement log rotation and retention policies. Hard drives get
bigger every year, but it’s still possible for a high-volume server to fill up a filesystem
with log data if you aren’t paying attention. Murphy’s law says this is most likely to
happen at 4:00 A.M. on New Year’s Day when you’re on vacation halfway around the
world.
Constructing Server Sockets
There are four public ServerSocket constructors:
public ServerSocket(int port) throws BindException, IOException
public ServerSocket(int port, int queueLength)
 throws BindException, IOException
public ServerSocket(int port, int queueLength, InetAddress bindAddress)
302 | Chapter 9: Sockets for Servers
 throws IOException
public ServerSocket() throws IOException
These constructors specify the port, the length of the queue used to hold incoming
connection requests, and the local network interface to bind to. They pretty much all
do the same thing, though some use default values for the queue length and the address
to bind to.
For example, to create a server socket that would be used by an HTTP server on port
80, you would write:
ServerSocket httpd = new ServerSocket(80);
To create a server socket that would be used by an HTTP server on port 80 and queues
up to 50 unaccepted connections at a time:
ServerSocket httpd = new ServerSocket(80, 50);
If you try to expand the queue past the operating system’s maximum queue length, the
maximum queue length is used instead.
By default, if a host has multiple network interfaces or IP addresses, the server socket
listens on the specified port on all the interfaces and IP addresses. However, you can
add a third argument to bind only to one particular local IP address. That is, the server
socket only listens for incoming connections on the specified address; it won’t listen for
connections that come in through the host’s other addresses.
For example, login.ibiblio.org is a particular Linux box in North Carolina. It’s connected
to the Internet with the IP address 152.2.210.122. The same box has a second Ethernet
card with the local IP address 192.168.210.122 that is not visible from the public Internet,
only from the local network. If, for some reason, you wanted to run a server on this host
that only responded to local connections from within the same network, you could
create a server socket that listens on port 5776 of 192.168.210.122 but not on port 5776
of 152.2.210.122, like so:
InetAddress local = InetAddress.getByName("192.168.210.122");
ServerSocket httpd = new ServerSocket(5776, 10, local);
In all three constructors, you can pass 0 for the port number so the system will select
an available port for you. A port chosen by the system like this is sometimes called an
anonymous port because you don’t know its number in advance (though you can find
out after the port has been chosen). This is often useful in multisocket protocols such
as FTP. In passive FTP the client first connects to a server on the well-known port 21,
so the server has to specify that port. However, when a file needs to be transferred, the
server starts listening on any available port. The server then tells the client what other
port it should connect to for data using the command connection already open on port
21. Thus, the data port can change from one session to the next and does not need to
be known in advance. (Active FTP is similar except the client listens on an ephemeral
port for the server to connect to it, rather than the other way around.)
Constructing Server Sockets | 303
All these constructors throw an IOException, specifically, a BindException, if the
socket cannot be created and bound to the requested port. An IOException when cre‐
ating a ServerSocket almost always means one of two things. Either another server
socket, possibly from a completely different program, is already using the requested
port, or you’re trying to connect to a port from 1 to 1023 on Unix (including Linux and
Mac OS X) without root (superuser) privileges.
You can take advantage of this to write a variation on the LowPortScanner program of
the previous chapter. Rather than attempting to connect to a server running on a given
port, you instead attempt to open a server on that port. If it’s occupied, the attempt will
fail. Example 9-8 checks for ports on the local machine by attempting to create Server
Socket objects on them and seeing on which ports that fails. If you’re using Unix and
are not running as root, this program works only for ports 1024 and above.
Example 9-8. Look for local ports
import java.io.*;
import java.net.*;
public class LocalPortScanner {
 public static void main(String[] args) {
 for (int port = 1; port <= 65535; port++) {
 try {
 // the next line will fail and drop into the catch block if
 // there is already a server running on the port
 ServerSocket server = new ServerSocket(port);
 } catch (IOException ex) {
 System.out.println("There is a server on port " + port + ".");
 }
 }
 }
}
Here’s the output I got when running LocalPortScanner on my Windows workstation:
D:\JAVA\JNP4\examples\9>java LocalPortScanner
There is a server on port 135.
There is a server on port 1025.
There is a server on port 1026.
There is a server on port 1027.
There is a server on port 1028.
Constructing Without Binding
The noargs constructor creates a ServerSocket object but does not actually bind it to
a port, so it cannot initially accept any connections. It can be bound later using the
bind() methods:
304 | Chapter 9: Sockets for Servers
public void bind(SocketAddress endpoint) throws IOException
public void bind(SocketAddress endpoint, int queueLength) throws IOException
The primary use for this feature is to allow programs to set server socket options before
binding to a port. Some options are fixed after the server socket has been bound. The
general pattern looks like this:
ServerSocket ss = new ServerSocket();
// set socket options...
SocketAddress http = new InetSocketAddress(80);
ss.bind(http);
You can also pass null for the SocketAddress to select an arbitrary port. This is like
passing 0 for the port number in the other constructors.
Getting Information About a Server Socket
The ServerSocket class provides two getter methods that tell you the local address and
port occupied by the server socket. These are useful if you’ve opened a server socket on
an anonymous port and/or an unspecified network interface. This would be the case,
for one example, in the data connection of an FTP session:
public InetAddress getInetAddress()
This method returns the address being used by the server (the local host). If the local
host has a single IP address (as most do), this is the address returned by InetAd
dress.getLocalHost(). If the local host has more than one IP address, the specific
address returned is one of the host’s IP addresses. You can’t predict which address you
will get. For example:
ServerSocket httpd = new ServerSocket(80);
InetAddress ia = httpd.getInetAddress();
If the ServerSocket has not yet bound to a network interface, this method returns null:
public int getLocalPort()
The ServerSocket constructors allow you to listen on an unspecified port by passing
0 for the port number. This method lets you find out what port you’re listening on. You
might use this in a peer-to-peer multisocket program where you already have a means
to inform other peers of your location. Or a server might spawn several smaller servers
to perform particular operations. The well-known server could inform clients on what
ports they can find the smaller servers. Of course, you can also use getLocalPort() to
find a nonanonymous port, but why would you need to? Example 9-9 demonstrates.
Example 9-9. A random port
import java.io.*;
import java.net.*;
Getting Information About a Server Socket | 305
public class RandomPort {
 public static void main(String[] args) {
 try {
 ServerSocket server = new ServerSocket(0);
 System.out.println("This server runs on port "
 + server.getLocalPort());
 } catch (IOException ex) {
 System.err.println(ex);
 }
 }
}
Here’s the output of several runs:
$ java RandomPort
This server runs on port 1154
D:\JAVA\JNP4\examples\9>java RandomPort
This server runs on port 1155
D:\JAVA\JNP4\examples\9>java RandomPort
This server runs on port 1156
At least on this system, the ports aren’t truly random, but they are indeterminate until
runtime.
If the ServerSocket has not yet bound to a port, getLocalPort() returns –1.
As with most Java objects, you can also just print out a ServerSocket using its to
String() method. A String returned by a ServerSocket’s toString() method looks
like this:
ServerSocket[addr=0.0.0.0,port=0,localport=5776]
addr is the address of the local network interface to which the server socket is bound.
This will be 0.0.0.0 if it’s bound to all interfaces, as is commonly the case. port is always
0. The localport is the local port on which the server is listening for connections. This
method is sometimes useful for debugging, but not much more. Don’t rely on it.
Socket Options
Socket options specify how the native sockets on which the ServerSocket class relies
send and receive data. For server sockets, Java supports three options:
• SO_TIMEOUT
• SO_REUSEADDR
• SO_RCVBUF
It also allows you to set performance preferences for the socket’s packets.
306 | Chapter 9: Sockets for Servers
SO_TIMEOUT
SO_TIMEOUT is the amount of time, in milliseconds, that accept() waits for an in‐
coming connection before throwing a java.io.InterruptedIOException. If
SO_TIMEOUT is 0, accept() will never time out. The default is to never time out.
Setting SO_TIMEOUT is uncommon. You might need it if you were implementing a
complicated and secure protocol that required multiple connections between the client
and the server where responses needed to occur within a fixed amount of time. However,
most servers are designed to run for indefinite periods of time and therefore just use
the default timeout value, 0 (never time out). If you want to change this, the setSoTi
meout() method sets the SO_TIMEOUT field for this server socket object:
public void setSoTimeout(int timeout) throws SocketException
public int getSoTimeout() throws IOException
The countdown starts when accept() is invoked. When the timeout expires, ac
cept() throws a SocketTimeoutException, a subclass of IOException. You need to set
this option before calling accept(); you cannot change the timeout value while ac
cept() is waiting for a connection. The timeout argument must be greater than or equal
to zero; if it isn’t, the method throws an IllegalArgumentException. For example:
try (ServerSocket server = new ServerSocket(port)) {
 server.setSoTimeout(30000); // block for no more than 30 seconds
 try {
 Socket s = server.accept();
 // handle the connection
 // ...
 } catch (SocketTimeoutException ex) {
 System.err.println("No connection within 30 seconds");
 }
} catch (IOException ex) {
 System.err.println("Unexpected IOException: " + e);
}
The getSoTimeout() method returns this server socket’s current SO_TIMEOUT value.
For example:
public void printSoTimeout(ServerSocket server) {
 int timeout = server.getSoTimeOut();
 if (timeout > 0) {
 System.out.println(server + " will time out after "
 + timeout + "milliseconds.");
 } else if (timeout == 0) {
 System.out.println(server + " will never time out.");
 } else {
 System.out.println("Impossible condition occurred in " + server);
 System.out.println("Timeout cannot be less than zero." );
 }
}
Socket Options | 307
SO_REUSEADDR
The SO_REUSEADDR option for server sockets is very similar to the same option for
client sockets, discussed in the previous chapter. It determines whether a new socket
will be allowed to bind to a previously used port while there might still be data traversing
the network addressed to the old socket. As you probably expect, there are two methods
to get and set this option:
public boolean getReuseAddress() throws SocketException
public void setReuseAddress(boolean on) throws SocketException
The default value is platform dependent. This code fragment determines the default
value by creating a new ServerSocket and then calling getReuseAddress():
ServerSocket ss = new ServerSocket(10240);
System.out.println("Reusable: " + ss.getReuseAddress());
On the Linux and Mac OS X boxes where I tested this code, server sockets were reusable
by default.
SO_RCVBUF
The SO_RCVBUF option sets the default receive buffer size for client sockets accepted
by the server socket. It’s read and written by these two methods:
public int getReceiveBufferSize() throws SocketException
public void setReceiveBufferSize(int size) throws SocketException
Setting SO_RCVBUF on a server socket is like calling setReceiveBufferSize() on
each individual socket returned by accept() (except that you can’t change the receive
buffer size after the socket has been accepted). Recall from the previous chapter that
this option suggests a value for the size of the individual IP packets in the stream. Faster
connections will want to use larger buffers, although most of the time the default value
is fine.
You can set this option before or after the server socket is bound, unless you want to set
a receive buffer size larger than 64K. In that case, you must set the option on an unbound
ServerSocket before binding it. For example:
ServerSocket ss = new ServerSocket();
int receiveBufferSize = ss.getReceiveBufferSize();
if (receiveBufferSize < 131072) {
 ss.setReceiveBufferSize(131072);
}
ss.bind(new InetSocketAddress(8000));
//...
308 | Chapter 9: Sockets for Servers
Class of Service
As you learned in the previous chapter, different types of Internet services have different
performance needs. For instance, live streaming video of sports needs relatively high
bandwidth. On the other hand, a movie might still need high bandwidth but be able to
tolerate more delay and latency. Email can be passed over low-bandwidth connections
and even held up for several hours without major harm.
Four general traffic classes are defined for TCP:
• Low cost
• High reliability
• Maximum throughput
• Minimum delay
These traffic classes can be requested for a given Socket. For instance, you can request
the minimum delay available at low cost. These measures are all fuzzy and relative, not
guarantees of service. Not all routers and native TCP stacks support these classes.
The setPerformancePreferences() method expresses the relative preferences given
to connection time, latency, and bandwidth for sockets accepted on this server:
public void setPerformancePreferences(int connectionTime, int latency,
 int bandwidth)
For instance, by setting connectionTime to 2, latency to 1, and bandwidth to 3, you
indicate that maximum bandwidth is the most important characteristic, minimum la‐
tency is the least important, and connection time is in the middle:
ss.setPerformancePreferences(2, 1, 3);
Exactly how any given VM implements this is implementation dependent. The under‐
lying socket implementation is not required to respect any of these requests. They only
provide a hint to the TCP stack about the desired policy. Many implementations in‐
cluding Android ignore these values completely.
HTTP Servers
This section shows several different HTTP servers you can build with server sockets,
each with a different special purpose and each slightly more complex than the previous
one.
HTTP is a large protocol. As you saw in Chapter 5, a full-featured HTTP server must
respond to requests for files, convert URLs into filenames on the local system, respond
to POST and GET requests, handle requests for files that don’t exist, interpret MIME types,
and much, much more. However, many HTTP servers don’t need all of these features.
HTTP Servers | 309
For example, many sites simply display an “under construction” message. Clearly,
Apache is overkill for a site like this. Such a site is a candidate for a custom server that
does only one thing. Java’s network class library makes writing simple servers like this
almost trivial.
Custom servers aren’t useful only for small sites. High-traffic sites like Yahoo! are also
candidates for custom servers because a server that does only one thing can often be
much faster than a general-purpose server such as Apache or Microsoft IIS. It is easy to
optimize a special-purpose server for a particular task; the result is often much more
efficient than a general-purpose server that needs to respond to many different kinds
of requests. For instance, icons and images that are used repeatedly across many pages
or on high-traffic pages might be better handled by a server that read all the image files
into memory on startup and then served them straight out of RAM, rather than having
to read them off disk for each request. Furthermore, this server could avoid wasting
time on logging if you didn’t want to track the image requests separately from the re‐
quests for the pages in which they were included.
Finally, Java isn’t a bad language for full-featured web servers meant to compete with
the likes of Apache or IIS. Even if you believe CPU-intensive Java programs are slower
than CPU-intensive C and C++ programs (something I very much doubt is true in
modern VMs), most HTTP servers are limited by network bandwidth and latency, not
by CPU speed. Consequently, Java’s other advantages, such as its half-compiled/halfinterpreted nature, dynamic class loading, garbage collection, and memory protection
really get a chance to shine. In particular, sites that make heavy use of dynamic content
through servlets, PHP pages, or other mechanisms can often run much faster when
reimplemented on top of a pure or mostly pure Java web server. Indeed, there are several
production web servers written in Java, such as the Eclipse Foundation’s Jetty. Many
other web servers written in C now include substantial Java components to support the
Java Servlet API and Java Server Pages. These largely replaced traditional CGIs, ASPs,
and server-side includes, mostly because the Java equivalents are faster and less resource
intensive. I’m not going to explore these technologies here because they easily deserve
a book of their own. I refer interested readers to Jason Hunter’s Java Servlet Program‐
ming (O’Reilly). However, it is important to note that servers in general and web servers
in particular are one area where Java really is competitive with C for real-world perfor‐
mance.
A Single-File Server
Our investigation of HTTP servers begins with a server that always sends out the same
file, no matter what the request. It’s called SingleFileHTTPServer and is shown in
Example 9-10. The filename, local port, and content encoding are read from the com‐
mand line. If the port is omitted, port 80 is assumed. If the encoding is omitted, ASCII
is assumed.
310 | Chapter 9: Sockets for Servers
Example 9-10. An HTTP server that serves a single file
import java.io.*;
import java.net.*;
import java.nio.charset.Charset;
import java.nio.file.*;
import java.util.concurrent.*;
import java.util.logging.*;
public class SingleFileHTTPServer {
 private static final Logger logger = Logger.getLogger("SingleFileHTTPServer");
 private final byte[] content;
 private final byte[] header;
 private final int port;
 private final String encoding;
 public SingleFileHTTPServer(String data, String encoding,
 String mimeType, int port) throws UnsupportedEncodingException {
 this(data.getBytes(encoding), encoding, mimeType, port);
 }
 public SingleFileHTTPServer(
 byte[] data, String encoding, String mimeType, int port) {
 this.content = data;
 this.port = port;
 this.encoding = encoding;
 String header = "HTTP/1.0 200 OK\r\n"
 + "Server: OneFile 2.0\r\n"
 + "Content-length: " + this.content.length + "\r\n"
 + "Content-type: " + mimeType + "; charset=" + encoding + "\r\n\r\n";
 this.header = header.getBytes(Charset.forName("US-ASCII"));
 }
 public void start() {
 ExecutorService pool = Executors.newFixedThreadPool(100);
 try (ServerSocket server = new ServerSocket(this.port)) {
 logger.info("Accepting connections on port " + server.getLocalPort());
 logger.info("Data to be sent:");
 logger.info(new String(this.content, encoding));
 while (true) {
 try {
 Socket connection = server.accept();
 pool.submit(new HTTPHandler(connection));
 } catch (IOException ex) {
 logger.log(Level.WARNING, "Exception accepting connection", ex);
 } catch (RuntimeException ex) {
 logger.log(Level.SEVERE, "Unexpected error", ex);
 }
 }
 } catch (IOException ex) {
HTTP Servers | 311
 logger.log(Level.SEVERE, "Could not start server", ex);
 }
 }
 private class HTTPHandler implements Callable<Void> {
 private final Socket connection;
 HTTPHandler(Socket connection) {
 this.connection = connection;
 }
 @Override
 public Void call() throws IOException {
 try {
 OutputStream out = new BufferedOutputStream(
 connection.getOutputStream()
 );
 InputStream in = new BufferedInputStream(
 connection.getInputStream()
 );
 // read the first line only; that's all we need
 StringBuilder request = new StringBuilder(80);
 while (true) {
 int c = in.read();
 if (c == '\r' || c == '\n' || c == -1) break;
 request.append((char) c);
 }
 // If this is HTTP/1.0 or later send a MIME header
 if (request.toString().indexOf("HTTP/") != -1) {
 out.write(header);
 }
 out.write(content);
 out.flush();
 } catch (IOException ex) {
 logger.log(Level.WARNING, "Error writing to client", ex);
 } finally {
 connection.close();
 }
 return null;
 }
 }
 public static void main(String[] args) {
 // set the port to listen on
 int port;
 try {
 port = Integer.parseInt(args[1]);
 if (port < 1 || port > 65535) port = 80;
 } catch (RuntimeException ex) {
 port = 80;
 }
312 | Chapter 9: Sockets for Servers
 String encoding = "UTF-8";
 if (args.length > 2) encoding = args[2];
 try {
 Path path = Paths.get(args[0]);;
 byte[] data = Files.readAllBytes(path);
 String contentType = URLConnection.getFileNameMap().getContentTypeFor(args[0]);
 SingleFileHTTPServer server = new SingleFileHTTPServer(data, encoding,
 contentType, port);
 server.start();
 } catch (ArrayIndexOutOfBoundsException ex) {
 System.out.println(
 "Usage: java SingleFileHTTPServer filename port encoding");
 } catch (IOException ex) {
 logger.severe(ex.getMessage());
 }
 }
}
The constructors set up the data to be sent along with an HTTP header that includes
information about content length and content encoding. The header and the body of
the response are stored in byte arrays in the desired encoding so that they can be blasted
to clients very quickly.
The SingleFileHTTPServer class holds the content to send, the header to send, and the
port to bind to. The start() method creates a ServerSocket on the specified port, then
enters an infinite loop that continually accepts connections and processes them.
Each incoming socket is processed by a runnable Handler object that is submitted to a
thread pool. Thus, one slow client can’t starve other clients. Each Handler gets an
InputStream from it which it reads the client request. It looks at the first line to see
whether it contains the string HTTP. If it sees this string, the server assumes that the client
understands HTTP/1.0 or later and therefore sends a MIME header for the file; then it
sends the data. If the client request doesn’t contain the string HTTP, the server omits the
header, sending the data by itself. Finally, the handler closes the connection.
The main() method just reads parameters from the command line. The name of the file
to be served is read from the first command-line argument. If no file is specified or the
file cannot be opened, an error message is printed and the program exits. Assuming the
file can be read, its contents are read into the byte array data using the Path and Files
classes introduced in Java 7. The URLConnection class makes a reasonable guess about
the content type of the file, and that guess is stored in the contentType variable. Next,
the port number is read from the second command-line argument. If no port is specified
or if the second argument is not an integer from 1 to 65,535, port 80 is used. The encoding
HTTP Servers | 313
is read from the third command-line argument, if present. Otherwise, UTF-8 is as‐
sumed. Then these values are used to construct a SingleFileHTTPServer object and
start it.
The main() method is only one possible interface. You could easily use this class as part
of some other program. If you added a setter method to change the content, you could
easily use it to provide simple status information about a running server or system.
However, that would raise some additional issues of thread safety that Example 9-10
doesn’t have to address because the data is immutable.
Here’s what you see when you connect to this server via Telnet (the specifics depend on
the exact server and file):
% telnet macfaq.dialup.cloud9.net 80
Trying 168.100.203.234...
Connected to macfaq.dialup.cloud9.net.
Escape character is '^]'.
GET / HTTP/1.0
HTTP/1.0 200 OK
Server: OneFile 2.0
Content-length: 959
Content-type: text/html; charset=UTF-8
&lt;!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2//EN"&gt;
&lt;HTML&gt;
&lt;HEAD&gt;
&lt;TITLE&gt;Under Construction&lt;/TITLE&gt;
&lt;/HEAD&gt;
&lt;BODY&gt;
...
A Redirector
Redirection is another simple but useful application for a special-purpose HTTP server.
In this section, you develop a server that redirects users from one website to another—
for example, from cnet.com to www.cnet.com. Example 9-11 reads a URL and a port
number from the command line, opens a server socket on the port, and redirects all
requests that it receives to the site indicated by the new URL using a 302 FOUND code.
In this example, I chose to use a new thread rather than a thread pool for each connec‐
tion. This is perhaps a little simpler to code and understand but somewhat less efficient.
Example 9-11. An HTTP redirector
import java.io.*;
import java.net.*;
import java.util.*;
import java.util.logging.*;
public class Redirector {
314 | Chapter 9: Sockets for Servers
 private static final Logger logger = Logger.getLogger("Redirector");
 private final int port;
 private final String newSite;
 public Redirector(String newSite, int port) {
 this.port = port;
 this.newSite = newSite;
 }
 public void start() {
 try (ServerSocket server = new ServerSocket(port)) {
 logger.info("Redirecting connections on port "
 + server.getLocalPort() + " to " + newSite);
 while (true) {
 try {
 Socket s = server.accept();
 Thread t = new RedirectThread(s);
 t.start();
 } catch (IOException ex) {
 logger.warning("Exception accepting connection");
 } catch (RuntimeException ex) {
 logger.log(Level.SEVERE, "Unexpected error", ex);
 }
 }
 } catch (BindException ex) {
 logger.log(Level.SEVERE, "Could not start server.", ex);
 } catch (IOException ex) {
 logger.log(Level.SEVERE, "Error opening server socket", ex);
 }
 }
 private class RedirectThread extends Thread {
 private final Socket connection;
 RedirectThread(Socket s) {
 this.connection = s;
 }
 public void run() {
 try {
 Writer out = new BufferedWriter(
 new OutputStreamWriter(
 connection.getOutputStream(), "US-ASCII"
 )
 );
 Reader in = new InputStreamReader(
 new BufferedInputStream(
 connection.getInputStream()
HTTP Servers | 315
 )
 );
 // read the first line only; that's all we need
 StringBuilder request = new StringBuilder(80);
 while (true) {
 int c = in.read();
 if (c == '\r' || c == '\n' || c == -1) break;
 request.append((char) c);
 }
 String get = request.toString();
 String[] pieces = get.split("\\w*");
 String theFile = pieces[1];
 // If this is HTTP/1.0 or later send a MIME header
 if (get.indexOf("HTTP") != -1) {
 out.write("HTTP/1.0 302 FOUND\r\n");
 Date now = new Date();
 out.write("Date: " + now + "\r\n");
 out.write("Server: Redirector 1.1\r\n");
 out.write("Location: " + newSite + theFile + "\r\n");
 out.write("Content-type: text/html\r\n\r\n");
 out.flush();
 }
 // Not all browsers support redirection so we need to
 // produce HTML that says where the document has moved to.
 out.write("<HTML><HEAD><TITLE>Document moved</TITLE></HEAD>\r\n");
 out.write("<BODY><H1>Document moved</H1>\r\n");
 out.write("The document " + theFile
 + " has moved to\r\n<A HREF=\"" + newSite + theFile + "\">"
 + newSite + theFile
 + "</A>.\r\n Please update your bookmarks<P>");
 out.write("</BODY></HTML>\r\n");
 out.flush();
 logger.log(Level.INFO,
 "Redirected " + connection.getRemoteSocketAddress());
 } catch(IOException ex) {
 logger.log(Level.WARNING,
 "Error talking to " + connection.getRemoteSocketAddress(), ex);
 } finally {
 try {
 connection.close();
 } catch (IOException ex) {}
 }
 }
 }
 public static void main(String[] args) {
 int thePort;
 String theSite;
316 | Chapter 9: Sockets for Servers
 try {
 theSite = args[0];
 // trim trailing slash
 if (theSite.endsWith("/")) {
 theSite = theSite.substring(0, theSite.length() - 1);
 }
 } catch (RuntimeException ex) {
 System.out.println(
 "Usage: java Redirector http://www.newsite.com/ port");
 return;
 }
 try {
 thePort = Integer.parseInt(args[1]);
 } catch (RuntimeException ex) {
 thePort = 80;
 }
 Redirector redirector = new Redirector(theSite, thePort);
 redirector.start();
 }
}
In order to start the redirector on port 80 and redirect incoming requests to http://
www.cafeconleche.org/, type:
D:\JAVA\JNP4\examples\09&gt; java Redirector http://www.cafeconleche.org/
Redirecting connections on port 80 to http://www.cafeconleche.org/
If you connect to this server via Telnet, this is what you’ll see:
% <userinput moreinfo="none">telnet macfaq.dialup.cloud9.net 80
Trying 168.100.203.234...
Connected to macfaq.dialup.cloud9.net.
Escape character is '^]'.
GET / HTTP/1.0
HTTP/1.0 302 FOUND
Date: Sun Mar 31 12:38:42 EDT 2013
Server: Redirector 1.1
Location: http://www.cafeconleche.org/
Content-type: text/html
&lt;HTML&gt;&lt;HEAD&gt;&lt;TITLE&gt;Document moved&lt;/TITLE&gt;&lt;/HEAD&gt;
&lt;BODY&gt;&lt;H1&gt;Document moved&lt;/H1&gt;
The document / has moved to
&lt;A HREF="http://www.cafeconleche.org/"&gt;http://www.cafeconleche.
org/&lt;/A&gt;.
Please update your bookmarks&lt;P&gt;&lt;/BODY&gt;&lt;/HTML&gt;
Connection closed by foreign host.
If, however, you connect with a web browser, you should be sent to http://www.cafe‐
conleche.org/ with only a slight delay. You should never see the HTML added after the
HTTP Servers | 317
response code; this is only provided to support very old browsers that don’t do redi‐
rection automatically, as well as a few security paranoids who have configured their
browsers not to redirect automatically.
The main() method provides a very simple interface that reads the URL of the new site
to redirect connections to and the local port to listen on. It uses this information to
construct a Redirector object. Then it invokes start(). If the port is not specified,
Redirector listens on port 80. If the site is omitted, Redirector prints an error message
and exits.
The start() method of Redirector binds the server socket to the port, prints a brief
status message, and then enters an infinite loop in which it listens for connections. Every
time a connection is accepted, the resulting Socket object is used to construct a Redi
rectThread. This RedirectThread is then started. All further interaction with the client
takes place in this new thread. The start() method then simply waits for the next
incoming connection.
The run() method of RedirectThread does most of the work. It begins by chaining a
Writer to the Socket’s output stream and a Reader to the Socket’s input stream. Both
input and output are buffered. Then the run() method reads the first line the client
sends. Although the client will probably send a whole MIME header, you can ignore
that. The first line contains all the information you need. The line looks something like
this:
GET /directory/filename.html HTTP/1.0
It is possible that the first word will be POST or PUT instead or that there will be no HTTP
version. The second “word” is the file the client wants to retrieve. This must begin with
a slash (/). Browsers are responsible for converting relative URLs to absolute URLs that
begin with a slash; the server does not do this. The third word is the version of the HTTP
protocol the browser understands. Possible values are nothing at all (pre-HTTP/1.0
browsers), HTTP/1.0, or HTTP/1.1.
To handle a request like this, Redirector ignores the first word. The second word is
attached to the URL of the target server (stored in the field newSite) to give a full
redirected URL. The third word is used to determine whether to send a MIME header;
MIME headers are not used for old browsers that do not understand HTTP/1.0. If there
is a version, a MIME header is sent; otherwise, it is omitted.
Sending the data is almost trivial. The Writer out is used. Because all the data you send
is pure ASCII, the exact encoding isn’t too important. The only trick here is that the
end-of-line character for HTTP requests is \r\n–a carriage return followed by a line‐
feed.
The next lines each send one line of text to the client. The first line printed is:
HTTP/1.0 302 FOUND
318 | Chapter 9: Sockets for Servers
This is an HTTP/1.0 response code that tells the client to expect to be redirected. The
second line is a Date: header that gives the current time at the server. This line is optional.
The third line is the name and version of the server; this line is also optional but is used
by spiders that try to keep statistics about the most popular web servers. The next line
is the Location: header, which is required for this response type. It tells the client where
it is being redirected to. Last is the standard Content-type: header. You send the content
type text/html to indicate that the client should expect to see HTML. Finally, a blank
line is sent to signify the end of the header data.
Everything after this will be HTML, which is processed by the browser and displayed
to the user. The next several lines print a message for browsers that do not support
redirection, so those users can manually jump to the new site. That message looks like:
<HTML><HEAD><TITLE>Document moved</TITLE></HEAD>
<BODY><H1>Document moved</H1>
The document / has moved to
<A HREF="http://www.cafeconleche.org/">http://www.cafeconleche.org/</A>.
Please update your bookmarks<P></BODY></HTML>
Finally, the connection is closed and the thread dies.
A Full-Fledged HTTP Server
Enough special-purpose HTTP servers. This next section develops a full-blown HTTP
server, called JHTTP, that can serve an entire document tree, including images, applets,
HTML files, text files, and more. It will be very similar to the SingleFileHTTPServer,
except that it pays attention to the GET requests. This server is still fairly lightweight;
after looking at the code, we’ll discuss other features you might want to add.
Because this server may have to read and serve large files from the filesystem over
potentially slow network connections, you’ll change its approach. Rather than process‐
ing each request as it arrives in the main thread of execution, you’ll place incoming
connections in a pool. Separate instances of a RequestProcessor class will remove the
connections from the pool and process them. Example 9-12 shows the main JHTTP class.
As in the previous two examples, the main() method of JHTTP handles initialization,
but other programs can use this class to run basic web servers.
Example 9-12. The JHTTP web server
import java.io.*;
import java.net.*;
import java.util.concurrent.*;
import java.util.logging.*;
public class JHTTP {
 private static final Logger logger = Logger.getLogger(
 JHTTP.class.getCanonicalName());
HTTP Servers | 319
 private static final int NUM_THREADS = 50;
 private static final String INDEX_FILE = "index.html";
 private final File rootDirectory;
 private final int port;
 public JHTTP(File rootDirectory, int port) throws IOException {
 if (!rootDirectory.isDirectory()) {
 throw new IOException(rootDirectory
 + " does not exist as a directory");
 }
 this.rootDirectory = rootDirectory;
 this.port = port;
 }
 public void start() throws IOException {
 ExecutorService pool = Executors.newFixedThreadPool(NUM_THREADS);
 try (ServerSocket server = new ServerSocket(port)) {
 logger.info("Accepting connections on port " + server.getLocalPort());
 logger.info("Document Root: " + rootDirectory);
 while (true) {
 try {
 Socket request = server.accept();
 Runnable r = new RequestProcessor(
 rootDirectory, INDEX_FILE, request);
 pool.submit(r);
 } catch (IOException ex) {
 logger.log(Level.WARNING, "Error accepting connection", ex);
 }
 }
 }
 }
 public static void main(String[] args) {
 // get the Document root
 File docroot;
 try {
 docroot = new File(args[0]);
 } catch (ArrayIndexOutOfBoundsException ex) {
 System.out.println("Usage: java JHTTP docroot port");
 return;
 }
 // set the port to listen on
 int port;
 try {
 port = Integer.parseInt(args[1]);
 if (port < 0 || port > 65535) port = 80;
 } catch (RuntimeException ex) {
320 | Chapter 9: Sockets for Servers
 port = 80;
 }
 try {
 JHTTP webserver = new JHTTP(docroot, port);
 webserver.start();
 } catch (IOException ex) {
 logger.log(Level.SEVERE, "Server could not start", ex);
 }
 }
}
The main() method of the JHTTP class sets the document root directory from
args[0]. The port is read from args[1] or 80 is used for a default. Then a new JHTTP
object is constructed and started. JHTTP creates a thread pool to handle requests and
repeatedly accepts incoming connections. You submit one RequestProcessor thread
per incoming connection into the pool.
Each connection is handled by the run() method of the RequestProcessor class shown
in Example 9-13. It gets input and output streams from the socket and chains them to
a reader and a writer. The reader reads the first line of the client request to determine
the version of HTTP that the client supports—you want to send a MIME header only
if this is HTTP/1.0 or later—and the requested file. Assuming the method is GET, the
file that is requested is converted to a filename on the local filesystem. If the file requested
is a directory (i.e., its name ends with a slash), you add the name of an index file. You
use the canonical path to make sure that the requested file doesn’t come from outside
the document root directory. Otherwise, a sneaky client could walk all over the local
filesystem by including .. in URLs to walk up the directory hierarchy. This is all you’ll
need from the client, although a more advanced web server, especially one that logged
hits, would read the rest of the MIME header the client sends.
Next, the requested file is opened and its contents are read into a byte array. If the HTTP
version is 1.0 or later, you write the appropriate MIME headers on the output stream.
To figure out the content type, you call the URLConnection.getFileNameMap().get
ContentTypeFor(fileName) method to map file extensions such as .html onto MIME
types such as text/html. The byte array containing the file’s contents is written onto the
output stream and the connection is closed. If the file cannot be found or opened, you
send the client a 404 response instead. If the client sends a method you don’t support,
such as POST, you send back a 501 error. If an exception occurs, you log it, close the
connection, and continue.
Example 9-13. The runnable class that handles HTTP requests
import java.io.*;
import java.net.*;
import java.nio.file.Files;
import java.util.*;
HTTP Servers | 321
import java.util.logging.*;
public class RequestProcessor implements Runnable {
 private final static Logger logger = Logger.getLogger(
 RequestProcessor.class.getCanonicalName());
 private File rootDirectory;
 private String indexFileName = "index.html";
 private Socket connection;
 public RequestProcessor(File rootDirectory,
 String indexFileName, Socket connection) {
 if (rootDirectory.isFile()) {
 throw new IllegalArgumentException(
 "rootDirectory must be a directory, not a file");
 }
 try {
 rootDirectory = rootDirectory.getCanonicalFile();
 } catch (IOException ex) {
 }
 this.rootDirectory = rootDirectory;
 if (indexFileName != null) this.indexFileName = indexFileName;
 this.connection = connection;
 }
 @Override
 public void run() {
 // for security checks
 String root = rootDirectory.getPath();
 try {
 OutputStream raw = new BufferedOutputStream(
 connection.getOutputStream()
 );
 Writer out = new OutputStreamWriter(raw);
 Reader in = new InputStreamReader(
 new BufferedInputStream(
 connection.getInputStream()
 ),"US-ASCII"
 );
 StringBuilder requestLine = new StringBuilder();
 while (true) {
 int c = in.read();
 if (c == '\r' || c == '\n') break;
 requestLine.append((char) c);
 }
 String get = requestLine.toString();
 logger.info(connection.getRemoteSocketAddress() + " " + get);
322 | Chapter 9: Sockets for Servers
 String[] tokens = get.split("\\s+");
 String method = tokens[0];
 String version = "";
 if (method.equals("GET")) {
 String fileName = tokens[1];
 if (fileName.endsWith("/")) fileName += indexFileName;
 String contentType =
 URLConnection.getFileNameMap().getContentTypeFor(fileName);
 if (tokens.length > 2) {
 version = tokens[2];
 }
 File theFile = new File(rootDirectory,
 fileName.substring(1, fileName.length()));
 if (theFile.canRead()
 // Don't let clients outside the document root
 && theFile.getCanonicalPath().startsWith(root)) {
 byte[] theData = Files.readAllBytes(theFile.toPath());
 if (version.startsWith("HTTP/")) { // send a MIME header
 sendHeader(out, "HTTP/1.0 200 OK", contentType, theData.length);
 }
 // send the file; it may be an image or other binary data
 // so use the underlying output stream
 // instead of the writer
 raw.write(theData);
 raw.flush();
 } else { // can't find the file
 String body = new StringBuilder("<HTML>\r\n")
 .append("<HEAD><TITLE>File Not Found</TITLE>\r\n")
 .append("</HEAD>\r\n")
 .append("<BODY>")
 .append("<H1>HTTP Error 404: File Not Found</H1>\r\n")
 .append("</BODY></HTML>\r\n").toString();
 if (version.startsWith("HTTP/")) { // send a MIME header
 sendHeader(out, "HTTP/1.0 404 File Not Found",
 "text/html; charset=utf-8", body.length());
 }
 out.write(body);
 out.flush();
 }
 } else { // method does not equal "GET"
 String body = new StringBuilder("<HTML>\r\n")
 .append("<HEAD><TITLE>Not Implemented</TITLE>\r\n")
 .append("</HEAD>\r\n")
 .append("<BODY>")
 .append("<H1>HTTP Error 501: Not Implemented</H1>\r\n")
 .append("</BODY></HTML>\r\n").toString();
 if (version.startsWith("HTTP/")) { // send a MIME header
 sendHeader(out, "HTTP/1.0 501 Not Implemented",
HTTP Servers | 323
 "text/html; charset=utf-8", body.length());
 }
 out.write(body);
 out.flush();
 }
 } catch (IOException ex) {
 logger.log(Level.WARNING,
 "Error talking to " + connection.getRemoteSocketAddress(), ex);
 } finally {
 try {
 connection.close();
 }
 catch (IOException ex) {}
 }
 }
 private void sendHeader(Writer out, String responseCode,
 String contentType, int length)
 throws IOException {
 out.write(responseCode + "\r\n");
 Date now = new Date();
 out.write("Date: " + now + "\r\n");
 out.write("Server: JHTTP 2.0\r\n");
 out.write("Content-length: " + length + "\r\n");
 out.write("Content-type: " + contentType + "\r\n\r\n");
 out.flush();
 }
}
This server is functional but still rather austere. Here are a few features that could be
added:
• A server administration interface
• Support for the Java Servlet API
• Support for other request methods, such as POST, HEAD, and PUT
• Support for multiple document roots so individual users can have their own sites
Finally, spend a little time thinking about ways to optimize this server. If you really want
to use JHTTP to run a high-traffic site, there are a couple of things that can speed this
server up. The first thing to do is implement smart caching. Keep track of the requests
you’ve received and store the data from the most frequently requested files in a Map so
that they’re kept in memory. Use a low-priority thread to update this cache. You can
also try using nonblocking I/O and channels instead of threads and streams. We’ll ex‐
plore this possibility in Chapter 11.


# 12. UDP

Java’s implementation of UDP is split into two classes: DatagramPacket and DatagramSocket

The DatagramPacket class stuffs bytes of data into datagrams and lets you unstuff datagrams that you receive.

A DatagramSocket sends as well as receives UDP datagrams. To send data, you put the data in a DatagramPacket and send the packet using a DatagramSocket.

To receive data, you take a DatagramPacket object from a DatagramSocket and then inspect the contents of the packet.

Everything about a datagram, including
the address to which it is directed, is included in the packet itself; the socket only needs
to know the local port on which to listen or send.

A single DatagramSocket can send data to and receive data from
many independent hosts. The socket isn’t dedicated to a single connection, as it is in TCP.



UDP Clients


First, open a
socket on port 0:
This is very different than a TCP socket. You only specify a local port to connect to. The
socket does not know the remote host or address. By specifying port 0 you ask Java to
pick a random available port for you, much as with server sockets.

```java
DatagramSocket socket = new DatagramSocket(0);


// Set a timeout on the connection using the setSoTimeout() method. Timeouts are measured in milliseconds, so this statement sets the socket to time out after 10 seconds of nonresponsiveness
socket.setSoTimeout(10000);

// Next you need to set up the packets. You’ll need two, one to send and one to receive. For the daytime protocol it doesn’t matter what data you put in the packet, but you do need to tell it the remote host and remote port to connect to:

InetAddress host = InetAddress.getByName("time.nist.gov");

DatagramPacket request = new DatagramPacket(new byte[1], 1, host, 13);

// The packet that receives the server’s response just contains an empty byte array. This needs to be large enough to hold the entire response. If it’s too small, it will be silently truncated—1k should be enough space:

byte[] data = new byte[1024];
DatagramPacket response = new DatagramPacket(data, data.length);

// First send the packet over the socket and then receive the response:
socket.send(request);
socket.receive(response);

// Finally, extract the bytes from the response and convert them to a string you can show to the end user:
String daytime = new String(response.getData(), 0, response.getLength(), "US-ASCII");
System.out.println(daytime);


// The constructor and send() and receive() methods can each throw an IOException,

// DatagramSocket implements Autocloseable so you can use try-with-resources:

try (DatagramSocket socket = new DatagramSocket(0)) {
 // connect to the server...
} catch (IOException ex) {
 System.err.println("Could not connect to time.nist.gov");
}

// In Java 6 and earlier, you’ll want to explicitly close the socket in a finally block to release resources the socket holds:
DatagramSocket socket = null;
try {
 socket = new DatagramSocket(0);
 // connect to the server...
} catch (IOException ex) {
 System.err.println(ex);
} finally {
 if (socket != null) {
 try {
 socket.close();
 } catch (IOException ex) {
 // ignore
 }
 }
}


import java.io.*;
import java.net.*;

public class DaytimeUDPClient {
 private final static int PORT = 13;
 private static final String HOSTNAME = "time.nist.gov";
 public static void main(String[] args) {
 try (DatagramSocket socket = new DatagramSocket(0)) {
 socket.setSoTimeout(10000);
 InetAddress host = InetAddress.getByName(HOSTNAME);
 DatagramPacket request = new DatagramPacket(new byte[1], 1, host , PORT);
 DatagramPacket response = new DatagramPacket(new byte[1024], 1024);
 socket.send(request);
 socket.receive(response);
 String result = new String(response.getData(), 0, response.getLength(),
 "US-ASCII");
 System.out.println(result);
 } catch (IOException ex) {
 ex.printStackTrace();
 }
 }
}
```




UDP Servers

A UDP server follows almost the same pattern as a UDP client, except that you usually receive before sending and don’t choose an anonymous port to bind to.


Begin by opening a datagram
socket on a well-known port. For daytime, this port is 13:

```java
DatagramSocket socket = new DatagramSocket(13);

// Next, create a packet into which to receive a request. You need to supply both a byte array in which to store incoming data, the offset into the array, and the number of bytes to store.
DatagramPacket request = new DatagramPacket(new byte[1024], 0, 1024);


// This call blocks indefinitely until a UDP packet arrives on port 13. When it does, Java fills the byte array with data and the receive() method returns.
socket.receive(request);

// Next, create a response packet. This has four parts: the raw data to send, the number of bytes of the raw data to send, the host to send to, and the port on that host to address.
String daytime = new Date().toString() + "\r\n";
byte[] data = daytime.getBytes("US-ASCII");

InetAddress host = request.getAddress();
int port = request.getPort();

DatagramPacket response = new DatagramPacket(data, data.length, host, port);

// Finally, send the response back over the same socket that received it:
socket.send(response);


import java.net.*;
import java.util.Date;
import java.util.logging.*;
import java.io.*;
public class DaytimeUDPServer {
 private final static int PORT = 13;
 private final static Logger audit = Logger.getLogger("requests");
 private final static Logger errors = Logger.getLogger("errors");
 public static void main(String[] args) {
 try (DatagramSocket socket = new DatagramSocket(PORT)) {
 while (true) {
 try {
 DatagramPacket request = new DatagramPacket(new byte[1024], 1024);
 socket.receive(request);
 String daytime = new Date().toString();
 byte[] data = daytime.getBytes("US-ASCII");
 DatagramPacket response = new DatagramPacket(data, data.length,
 request.getAddress(), request.getPort());
 socket.send(response);
 audit.info(daytime + " " + request.getAddress());
 } catch (IOException | RuntimeException ex) {
 errors.log(Level.SEVERE, ex.getMessage(), ex);
 }
 }
 } catch (IOException ex) {
 errors.log(Level.SEVERE, ex.getMessage(), ex);
 }
 }
}
```




```java

The DatagramSocket Class
To send or receive a DatagramPacket, you must open a datagram socket. In Java, a
datagram socket is created and accessed through the DatagramSocket class:
public class DatagramSocket extends Object
All datagram sockets bind to a local port, on which they listen for incoming data and
which they place in the header of outgoing datagrams. If you’re writing a client, you
don’t care what the local port is, so you call a constructor that lets the system assign an
unused port (an anonymous port). This port number is placed in any outgoing data‐
grams and will be used by the server to address any response datagrams. If you’re writing
a server, clients need to know on which port the server is listening for incoming data‐
grams; therefore, when a server constructs a DatagramSocket, it specifies the local port
408 | Chapter 12: UDP
on which it will listen. However, the sockets used by clients and servers are otherwise
identical: they differ only in whether they use an anonymous (system-assigned) or a
well-known port. There’s no distinction between client sockets and server sockets, as
there is with TCP. There’s no such thing as a DatagramServerSocket.
The Constructors
The DatagramSocket constructors are used in different situations, much like the Data
gramPacket constructors. The first constructor opens a datagram socket on an anony‐
mous local port. The second constructor opens a datagram socket on a well-known local
port that listens to all local network interfaces. The last two constructors open a data‐
gram socket on a well-known local port on a specific network interface. All constructors
deal only with the local address and port. The remote address and port are stored in the
DatagramPacket, not the DatagramSocket. Indeed, one DatagramSocket can send and
receive datagrams from multiple remote hosts and ports.
public DatagramSocket() throws SocketException
This constructor creates a socket that is bound to an anonymous port. For example:
try {
 DatagramSocket client = new DatagramSocket();
 // send packets...
} catch (SocketException ex) {
 System.err.println(ex);
}
Pick this constructor for a client that initiates a conversation with a server. In this sce‐
nario, you don’t care what port the socket is bound to because the server will send its
response to the port from which the datagram originated. Letting the system assign a
port means that you don’t have to worry about finding an unused port. If, for some
reason, you need to know the local port, you can find out with the getLocalPort()
method described later in this chapter.
The same socket can receive the datagrams that a server sends back to it. The constructor
throws a SocketException if the socket can’t bind to a port. It’s unusual for this con‐
structor to throw an exception; it’s hard to imagine situations in which the socket could
not be opened, because the system gets to choose an available port.
public DatagramSocket(int port) throws SocketException
This constructor creates a socket that listens for incoming datagrams on a particular
port, specified by the port argument. Use this constructor to write a server that listens
on a well-known port. A SocketException is thrown if the socket can’t be created. There
are two common reasons for the constructor to fail: the specified port is already occu‐
pied, or you are trying to connect to a port below 1024 and you don’t have sufficient
The DatagramSocket Class | 409
privileges (i.e., you are not root on a Unix system; for better or worse, other platforms
allow anyone to connect to low-numbered ports).
TCP ports and UDP ports are not related. Two different programs can use the same
port number if one uses UDP and the other uses TCP. Example 12-4 is a port scanner
that looks for UDP ports in use on the local host. It decides that the port is in use if the
DatagramSocket constructor throws an exception. As written, it looks at ports from
1024 and up to avoid Unix’s requirement that it run as root to bind to ports below 1024.
You can easily extend it to check ports below 1024, however, if you have root access or
are running it on Windows.
Example 12-4. Look for local UDP ports
import java.net.*;
public class UDPPortScanner {
 public static void main(String[] args) {
 for (int port = 1024; port <= 65535; port++) {
 try {
 // the next line will fail and drop into the catch block if
 // there is already a server running on port i
 DatagramSocket server = new DatagramSocket(port);
 server.close();
 } catch (SocketException ex) {
 System.out.println("There is a server on port " + port + ".");
 }
 }
 }
}
Here are the results from the Linux workstation on which much of the code in this book
was written:
% java UDPPortScanner
There is a server on port 2049.
There is a server on port 32768.
There is a server on port 32770.
There is a server on port 32771.
The first port, 2049, is an NFS server. The high-numbered ports in the 30,000 range are
Remote Procedure Call (RPC) services. Along with RPC, common protocols that use
UDP include NFS, TFTP, and FSP.
It’s much harder to scan UDP ports on a remote system than to scan for remote TCP
ports. Whereas there’s always some indication that a listening port, regardless of appli‐
cation layer protocol, has received your TCP packet, UDP provides no such guarantees.
To determine that a UDP server is listening, you have to send it a packet it will recognize
and respond to.
410 | Chapter 12: UDP
public DatagramSocket(int port, InetAddress interface) throws SocketException
This constructor is primarily used on multihomed hosts; it creates a socket that listens
for incoming datagrams on a specific port and network interface. The port argument
is the port on which this socket listens for datagrams. As with TCP sockets, you need
to be root on a Unix system to create a DatagramSocket on a port below 1024. The
address argument is an InetAddress object matching one of the host’s network ad‐
dresses. A SocketException is thrown if the socket can’t be created. There are three
common reasons for this constructor to fail: the specified port is already occupied, you
are trying to connect to a port below 1024 and you’re not root on a Unix system, or
address is not the address of one of the system’s network interfaces.
public DatagramSocket(SocketAddress interface) throws SocketException
This constructor is similar to the previous one except that the network interface address
and port are read from a SocketAddress. For example, this code fragment creates a
socket that only listens on the local loopback address:
SocketAddress address = new InetSocketAddress("127.0.0.1", 9999);
DatagramSocket socket = new DatagramSocket(address);
protected DatagramSocket(DatagramSocketImpl impl) throws SocketException
This constructor enables subclasses to provide their own implementation of the UDP
protocol, rather than blindly accepting the default. Unlike sockets created by the other
four constructors, this socket is not initially bound to a port. Before using it, you have
to bind it to a SocketAddress using the bind() method:
public void bind(SocketAddress addr) throws SocketException
You can pass null to this method, binding the socket to any available address and port.
Sending and Receiving Datagrams
The primary task of the DatagramSocket class is to send and receive UDP datagrams.
One socket can both send and receive. Indeed, it can send and receive to and from
multiple hosts at the same time.
public void send(DatagramPacket dp) throws IOException
Once a DatagramPacket is created and a DatagramSocket is constructed, send the packet
by passing it to the socket’s send() method. For example, if theSocket is a Datagram
Socket object and theOutput is a DatagramPacket object, send theOutput using the
Socket like this:
theSocket.send(theOutput);
If there’s a problem sending the data, this method may throw an IOException. However,
this is less common with DatagramSocket than Socket or ServerSocket, because the
unreliable nature of UDP means you won’t get an exception just because the packet
The DatagramSocket Class | 411
doesn’t arrive at its destination. You may get an IOException if you’re trying to send a
larger datagram than the host’s native networking software supports, but then again you
may not. This depends heavily on the native UDP software in the OS and the native
code that interfaces between this and Java’s DatagramSocketImpl class. This method
may also throw a SecurityException if the SecurityManager won’t let you commu‐
nicate with the host to which the packet is addressed. This is primarily a problem for
applets and other remotely loaded code.
Example 12-5 is a UDP-based discard client. It reads lines of user input from Sys
tem.in and sends them to a discard server, which simply discards all the data. Each line
is stuffed in a DatagramPacket. Many of the simpler Internet protocols, such as discard
and echo, have both TCP and UDP implementations.
Example 12-5. A UDP discard client
import java.net.*;
import java.io.*;
public class UDPDiscardClient {
 public final static int PORT = 9;
 public static void main(String[] args) {
 String hostname = args.length > 0 ? args[0] : "localhost";
 try (DatagramSocket theSocket = new DatagramSocket()) {
 InetAddress server = InetAddress.getByName(hostname);
 BufferedReader userInput
 = new BufferedReader(new InputStreamReader(System.in));
 while (true) {
 String theLine = userInput.readLine();
 if (theLine.equals(".")) break;
 byte[] data = theLine.getBytes();
 DatagramPacket theOutput
 = new DatagramPacket(data, data.length, server, PORT);
 theSocket.send(theOutput);
 } // end while
 } catch (IOException ex) {
 System.err.println(ex);
 }
 }
}
The UDPDiscardClient class should look familiar. It has a single static field, PORT, which
is set to the standard port for the discard protocol (port 9), and a single method,
main(). The main() method reads a hostname from the command line and converts
that hostname to the InetAddress object called server. A BufferedReader is chained
to System.in to read user input from the keyboard. Next, a DatagramSocket object
412 | Chapter 12: UDP
called theSocket is constructed. After creating the socket, the program enters an infinite
while loop that reads user input line by line using readLine(). Example 12-5 is careful,
however, to use only readLine() to read data from the console, the one place where it
is guaranteed to work as advertised. Because the discard protocol deals only with raw
bytes, it can ignore character encoding issues.
In the while loop, each line is converted to a byte array using the getBytes() method,
and the bytes are stuffed in a new DatagramPacket, theOutput. Finally, theOutput is
sent over theSocket, and the loop continues. If at any point the user types a period on
a line by itself, the program exits. The DatagramSocket constructor may throw a Sock
etException, so that needs to be caught. Because this is a discard client, you don’t need
to worry about data coming back from the server.
public void receive(DatagramPacket dp) throws IOException
This method receives a single UDP datagram from the network and stores it in the
preexisting DatagramPacket object dp. Like the accept() method in the ServerSock
et class, this method blocks the calling thread until a datagram arrives. If your program
does anything besides wait for datagrams, you should call receive() in a separate
thread.
The datagram’s buffer should be large enough to hold the data received. If it’s not,
receive() places as much data in the buffer as it can hold; the rest is lost. Remember
that the maximum size of the data portion of a UDP datagram is 65,507 bytes. (That’s
the 65,536-byte maximum size of an IP datagram minus the 20-byte size of the IP header
and the 8-byte size of the UDP header.) Some application protocols that use UDP further
restrict the maximum number of bytes in a packet; for instance, NFS uses a maximum
packet size of 8,192 bytes.
If there’s a problem receiving the data, receive() may throw an IOException. In prac‐
tice, this is rare because problems like dropped packets that would shut down a TCP
stream are silently discarded by the network or network stack before Java ever sees them.
Example 12-6 shows a UDP discard server that receives incoming datagrams. Just for
fun, it logs the data in each datagram to System.out so that you can see who’s sending
what to your discard server.
Example 12-6. The UDPDiscardServer
import java.net.*;
import java.io.*;
public class UDPDiscardServer {
 public final static int PORT = 9;
 public final static int MAX_PACKET_SIZE = 65507;
 public static void main(String[] args) {
The DatagramSocket Class | 413
 byte[] buffer = new byte[MAX_PACKET_SIZE];
 try (DatagramSocket server = new DatagramSocket(PORT)) {
 DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
 while (true) {
 try {
 server.receive(packet);
 String s = new String(packet.getData(), 0, packet.getLength(), "8859_1");
 System.out.println(packet.getAddress() + " at port "
 + packet.getPort() + " says " + s);
 // reset the length for the next packet
 packet.setLength(buffer.length);
 } catch (IOException ex) {
 System.err.println(ex);
 }
 } // end while
 } catch (SocketException ex) {
 System.err.println(ex);
 }
 }
}
This is a simple class with a single method, main(). It reads the port the server listens
to from the command line. If the port is not specified on the command line, it listens
on port 9. It then opens a DatagramSocket on that port and creates a DatagramPacket
with a 65,507-byte buffer—large enough to receive any possible packet. Then the server
enters an infinite loop that receives packets and prints the contents and the originating
host on the console. There’s no particular encoding expected for discard packets. Indeed,
there’s no particular reason these packets have to be text at all. I somewhat arbitrarily
picked the Latin-1 ISO 8859-1 encoding because it’s ASCII compatible and defines a
character for every byte.
As each datagram is received, the length of packet is set to the length of the data in that
datagram. Consequently, as the last step of the loop, the length of the packet is reset to
the maximum possible value. Otherwise, the incoming packets would be limited to the
minimum size of all previous packets. You can run the discard client on one machine
and connect to the discard server on a second machine to verify that the network is
working.
public void close()
Calling a DatagramSocket object’s close() method frees the port occupied by that
socket. As with streams and TCP sockets, you’ll want to take care to close the datagram
socket in a finally block:
DatagramSocket server = null
try {
 server = new DatagramSocket();
414 | Chapter 12: UDP
 // use the socket...
} catch (IOException ex) {
 System.err.println(ex);
} finally {
 try {
 if (server != null) server.close();
 } catch (IOException ex) {
 }
}
In Java 7, DatagramSocket implements AutoCloseable so you can use try-withresources:
try (DatagramSocket server = new DatagramSocket()) {
 // use the socket...
}
It’s never a bad idea to close a DatagramSocket when you’re through with it; it’s partic‐
ularly important to close an unneeded socket if the program will continue to run for a
significant amount of time. For example, the close() method was essential in
Example 12-4, UDPPortScanner: if this program did not close the sockets it opened, it
would tie up every UDP port on the system for a significant amount of time. On the
other hand, if the program ends as soon as you’re through with the DatagramSocket,
you don’t need to close the socket explicitly; the socket is automatically closed upon
garbage collection. However, Java won’t run the garbage collector just because you’ve
run out of ports or sockets, unless by lucky happenstance you run out of memory at the
same time. Closing unneeded sockets never hurts and is good programming practice.
public int getLocalPort()
A DatagramSocket’s getLocalPort() method returns an int that represents the local
port on which the socket is listening. Use this method if you created a DatagramSock
et with an anonymous port and want to find out what port the socket has been as‐
signed. For example:
DatagramSocket ds = new DatagramSocket();
System.out.println("The socket is using port " + ds.getLocalPort());
public InetAddress getLocalAddress()
A DatagramSocket’s getLocalAddress() method returns an InetAddress object that
represents the local address to which the socket is bound. It’s rarely needed in practice.
Normally, you either already know or simply don’t care which address a socket is lis‐
tening to.
public SocketAddress getLocalSocketAddress()
The getLocalSocketAddress() method returns a SocketAddress object that wraps the
local interface and port to which the socket is bound. Like getLocalAddress(), it’s a
The DatagramSocket Class | 415
little hard to imagine a realistic use case here. This method probably exists mostly for
parallelism with setLocalSocketAddress().
Managing Connections
Unlike TCP sockets, datagram sockets aren’t very picky about whom they’ll talk to. In
fact, by default they’ll talk to anyone; but this is often not what you want. For instance,
applets are only allowed to send datagrams to and receive datagrams from the applet
host. An NFS or FSP client should accept packets only from the server it’s talking to. A
networked game should listen to datagrams only from the people playing the game. The
next five methods let you choose which host you can send datagrams to and receive
datagrams from, while rejecting all others’ packets.
public void connect(InetAddress host, int port)
The connect() method doesn’t really establish a connection in the TCP sense. However,
it does specify that the DatagramSocket will only send packets to and receive packets
from the specified remote host on the specified remote port. Attempts to send packets
to a different host or port will throw an IllegalArgumentException. Packets received
from a different host or a different port will be discarded without an exception or other
notification.
A security check is made when the connect() method is invoked. If the VM is allowed
to send data to that host and port, the check passes silently. If not, a SecurityExcep
tion is thrown. However, once the connection has been made, send() and receive()
on that DatagramSocket no longer make the security checks they’d normally make.
public void disconnect()
The disconnect() method breaks the “connection” of a connected DatagramSocket so
that it can once again send packets to and receive packets from any host and port.
public int getPort()
If and only if a DatagramSocket is connected, the getPort() method returns the remote
port to which it is connected. Otherwise, it returns –1.
public InetAddress getInetAddress()
If and only if a DatagramSocket is connected, the getInetAddress() method returns
the address of the remote host to which it is connected. Otherwise, it returns null.
public InetAddress getRemoteSocketAddress()
If a DatagramSocket is connected, the getRemoteSocketAddress() method returns the
address of the remote host to which it is connected. Otherwise, it returns null.
416 | Chapter 12: UDP
Socket Options
Java supports six socket options for UDP:
• SO_TIMEOUT
• SO_RCVBUF
• SO_SNDBUF
• SO_REUSEADDR
• SO_BROADCAST
• IP_TOS
SO_TIMEOUT
SO_TIMEOUT is the amount of time, in milliseconds, that receive() waits for an
incoming datagram before throwing an InterruptedIOException, which is a subclass
of IOException. Its value must be nonnegative. If SO_TIMEOUT is 0, receive() never
times out. This value can be changed with the setSoTimeout() method and inspected
with the getSoTimeout() method:
public void setSoTimeout(int timeout) throws SocketException
public int getSoTimeout() throws IOException
The default is to never time out, and indeed there are few situations in which you need
to set SO_TIMEOUT. You might need it if you were implementing a secure protocol
that required responses to occur within a fixed amount of time. You might also decide
that the host you’re communicating with is dead (unreachable or not responding) if you
don’t receive a response within a certain amount of time.
The setSoTimeout() method sets the SO_TIMEOUT field for a datagram socket. When
the timeout expires, a blocked receive() method throws a SocketTimeoutException.
Set this option before you call receive(). You cannot change it while receive() is
waiting for a datagram. The timeout argument must be greater than or equal to zero.
For example:
try {
 byte[] buffer = new byte[2056];
 DatagramPacket dp = new DatagramPacket(buffer, buffer.length);
 DatagramSocket ds = new DatagramSocket(2048);
 ds.setSoTimeout(30000); // block for no more than 30 seconds
 try {
 ds.receive(dp);
 // process the packet...
 } catch (SocketTimeoutException ex) {
 ss.close();
 System.err.println("No connection within 30 seconds");
Socket Options | 417
 }
} catch (SocketException ex) {
 System.err.println(ex);
} catch (IOException ex) {
 System.err.println("Unexpected IOException: " + ex);
}
The getSoTimeout() method returns the current value of this DatagramSocket object’s
SO_TIMEOUT field. For example:
public void printSoTimeout(DatagramSocket ds) {
 int timeout = ds.getSoTimeOut();
 if (timeout > 0) {
 System.out.println(ds + " will time out after "
 + timeout + "milliseconds.");
 } else if (timeout == 0) {
 System.out.println(ds + " will never time out.");
 } else {
 System.out.println("Something is seriously wrong with " + ds);
 }
}
SO_RCVBUF
The SO_RCVBUF option of DatagramSocket is closely related to the SO_RCVBUF
option of Socket. It determines the size of the buffer used for network I/O. Larger buffers
tend to improve performance for reasonably fast (say, Ethernet-speed) connections be‐
cause they can store more incoming datagrams before overflowing. Sufficiently large
receive buffers are even more important for UDP than for TCP, because a UDP datagram
that arrives when the buffer is full will be lost, whereas a TCP datagram that arrives at
a full buffer will eventually be retransmitted. Furthermore, SO_RCVBUF sets the max‐
imum size of datagram packets that can be received by the application. Packets that
won’t fit in the receive buffer are silently discarded.
DatagramSocket has methods to set and get the suggested receive buffer size used for
network input:
public void setReceiveBufferSize(int size) throws SocketException
public int getReceiveBufferSize() throws SocketException
The setReceiveBufferSize() method suggests a number of bytes to use for buffering
input from this socket. However, the underlying implementation is free to ignore this
suggestion. For instance, many 4.3 BSD-derived systems have a maximum receive buffer
size of about 52K and won’t let you set a limit higher than this. My Linux box was limited
to 64K. Other systems raise this to about 240K. The details are highly platformdependent. Consequently, you may wish to check the actual size of the receive buffer
with getReceiveBufferSize() after setting it. The getReceiveBufferSize() method
returns the number of bytes in the buffer used for input from this socket.
418 | Chapter 12: UDP
Both methods throw a SocketException if the underlying socket implementation does
not recognize the SO_RCVBUF option. This might happen on a non-POSIX operating
system. The setReceiveBufferSize() method will throw an IllegalArgumentExcep
tion if its argument is less than or equal to zero.
SO_SNDBUF
DatagramSocket has methods to get and set the suggested send buffer size used for
network output:
public void setSendBufferSize(int size) throws SocketException
public int getSendBufferSize() throws SocketException
The setSendBufferSize() method suggests a number of bytes to use for buffering
output on this socket. Once again, however, the operating system is free to ignore this
suggestion. Consequently, you’ll want to check the result of setSendBufferSize() by
immediately following it with a call to getSendBufferSize() to find out the real buffer
size.
Both methods throw a SocketException if the underlying native network software
doesn’t understand the SO_SNDBUF option. The setSendBufferSize() method also
throws an IllegalArgumentException if its argument is less than or equal to zero.
SO_REUSEADDR
The SO_REUSEADDR option does not mean the same thing for UDP sockets as it does
for TCP sockets. For UDP, SO_REUSEADDR controls whether multiple datagram
sockets can bind to the same port and address at the same time. If multiple sockets are
bound to the same port, received packets will be copied to all bound sockets. This option
is controlled by these two methods:
public void setReuseAddress(boolean on) throws SocketException
public boolean getReuseAddress() throws SocketException
For this to work reliably, setReuseAddress() must be called before the new socket binds
to the port. This means the socket must be created in an unconnected state using the
protected constructor that takes a DatagramImpl as an argument. In other words, it
won’t work with a plain vanilla DatagramSocket. Reusable ports are most commonly
used for multicast sockets, which will be discussed in the next chapter. Datagram chan‐
nels also create unconnected datagram sockets that can be configured to reuse ports, as
you’ll see later in this chapter.
SO_BROADCAST
The SO_BROADCAST option controls whether a socket is allowed to send packets to
and receive packets from broadcast addresses such as 192.168.254.255, the local network
Socket Options | 419
broadcast address for the network with the local address 192.168.254.*. UDP broad‐
casting is often used for protocols such as DHCP that need to communicate with servers
on the local net whose addresses are not known in advance. This option is controlled
with these two methods:
public void setBroadcast(boolean on) throws SocketException
public boolean getBroadcast() throws SocketException
Routers and gateways do not normally forward broadcast messages, but they can still
kick up a lot of traffic on the local network. This option is turned on by default, but if
you like you can disable it thusly:
socket.setBroadcast(false);
This option can be changed after the socket has been bound.
On some implementations, sockets bound to a specific address do not
receive broadcast packets. In other words, you should use the Data
gramPacket(int port) constructor, not the DatagramPacket(InetAd
dress address, int port) constructor to listen to broadcasts. This
is necessary in addition to setting the SO_BROADCAST option to true.
IP_TOS
Because the traffic class is determined by the value of the IP_TOS field in each IP packet
header, it is essentially the same for UDP as it is for TCP. After all, packets are actually
routed and prioritized according to IP, which both TCP and UDP sit on top of. There’s
really no difference between the setTrafficClass() and getTrafficClass() methods
in DatagramSocket and those in Socket. They just have to be repeated here because
DatagramSocket and Socket don’t have a common superclass. These two methods let
you inspect and set the class of service for a socket using these two methods:
public int getTrafficClass() throws SocketException
public void setTrafficClass(int trafficClass) throws SocketException
The traffic class is given as an int between 0 and 255. Because this value is copied to an
eight-bit field in the TCP header, only the low-order byte of this int is used; and values
outside this range cause IllegalArgumentExceptions.
The JavaDoc for these options is severely out of date, and describes a
quality of service scheme based on bit fields for four traffic classes: low
cost, high reliability, maximum throughput, and minimum delay. This
scheme was never widely implemented and probably hasn’t been used
in this century.
420 | Chapter 12: UDP
This code fragment sets a socket to use Expedited Forwarding by setting the traffic class
to 10111000:
DatagramSocket s = new DatagramSocket();
s.setTrafficClass(0xB8); // 10111000 in binary
For details of the individual traffic classes, refer to “IP_TOS Class of Service” on page
265.
The underlying socket implementation is not required to respect any of these requests.
Some implementations ignore these values completely. Android in particular treats the
setTrafficClass() method as a noop. If the native network stack is unable to provide
the requested class of service, Java may but is not required to throw a SocketException.
Some Useful Applications
In this section, you’ll see several Internet servers and clients that use DatagramPacket
and DatagramSocket. Some of these will be familiar from previous chapters because
many Internet protocols have both TCP and UDP implementations. When an IP packet
is received by a host, the host determines whether the packet is a TCP packet or a UDP
datagram by inspecting the IP header. As I said earlier, there’s no connection between
UDP and TCP ports; TCP and UDP servers can share the same port number without
problems. By convention, if a service has both TCP and UDP implementations, it uses
the same port for both, although there’s no technical reason this has to be the case.
Simple UDP Clients
Several Internet services need to know only the client’s address and port; they ignore
any data the client sends in its datagrams. Daytime, quote of the day, time, and chargen
are four such protocols. Each of these responds the same way, regardless of the data
contained in the datagram, or indeed regardless of whether there actually is any data in
the datagram. Clients for these protocols simply send a UDP datagram to the server and
read the response that comes back. Therefore, let’s begin with a simple client called
UDPPoke , shown in Example 12-7, which sends an empty UDP packet to a specified host
and port and reads a response packet from the same host.
The UDPPoke class has four private fields. The bufferSize field specifies how large a
return packet is expected. An 8,192-byte buffer is large enough for most of the protocols
that UDPPoke is useful for, but it can be increased by passing a different value to the
constructor. The timeout field specifies how long to wait for a response. The host and
the port fields specify the remote host to connect to.
If the buffer length is not specified, 8,192 bytes is used. If the timeout is not given, 30
seconds (30,000 milliseconds) is used. The host, port, and buffer size are also used to
construct the outgoing DatagramPacket. Although in theory you should be able to send
Some Useful Applications | 421
a datagram with no data at all, bugs in some Java implementations require that you add
at least one byte of data to the datagram. The simple servers we’re currently considering
ignore this data.
Once a UDPPoke object has been constructed, clients call its poke() method to send an
empty outgoing datagram to the target and read its response. The response is initially
set to null. When the expected datagram appears, its data is copied into the response
field. This method returns null if the response doesn’t come quickly enough or never
comes at all.
The main() method merely reads the host and port to connect to from the command
line, constructs a UDPPoke object, and pokes it. Most of the simple protocols that this
client suits will return ASCII text, so this example attempts to convert the response to
an ASCII string and print it.
Example 12-7. The UDPPoke class
import java.io.*;
import java.net.*;
public class UDPPoke {
 private int bufferSize; // in bytes
 private int timeout; // in milliseconds
 private InetAddress host;
 private int port;
 public UDPPoke(InetAddress host, int port, int bufferSize, int timeout) {
 this.bufferSize = bufferSize;
 this.host = host;
 if (port < 1 || port > 65535) {
 throw new IllegalArgumentException("Port out of range");
 }
 this.port = port;
 this.timeout = timeout;
 }
 public UDPPoke(InetAddress host, int port, int bufferSize) {
 this(host, port, bufferSize, 30000);
 }
 public UDPPoke(InetAddress host, int port) {
 this(host, port, 8192, 30000);
 }
 public byte[] poke() {
 try (DatagramSocket socket = new DatagramSocket(0)) {
 DatagramPacket outgoing = new DatagramPacket(new byte[1], 1, host, port);
 socket.connect(host, port);
 socket.setSoTimeout(timeout);
422 | Chapter 12: UDP
 socket.send(outgoing);
 DatagramPacket incoming
 = new DatagramPacket(new byte[bufferSize], bufferSize);
 // next line blocks until the response is received
 socket.receive(incoming);
 int numBytes = incoming.getLength();
 byte[] response = new byte[numBytes];
 System.arraycopy(incoming.getData(), 0, response, 0, numBytes);
 return response;
 } catch (IOException ex) {
 return null;
 }
 }
 public static void main(String[] args) {
 InetAddress host;
 int port = 0;
 try {
 host = InetAddress.getByName(args[0]);
 port = Integer.parseInt(args[1]);
 } catch (RuntimeException | UnknownHostException ex) {
 System.out.println("Usage: java UDPPoke host port");
 return;
 }
 try {
 UDPPoke poker = new UDPPoke(host, port);
 byte[] response = poker.poke();
 if (response == null) {
 System.out.println("No response within allotted time");
 return;
 }
 String result = new String(response, "US-ASCII");
 System.out.println(result);
 } catch (UnsupportedEncodingException ex) {
 // Really shouldn't happen
 ex.printStackTrace();
 }
 }
}
For example, this connects to a daytime server over UDP:
$ java UDPPoke rama.poly.edu 13
Sun Oct 3 13:04:22 2009
This connects to a chargen server:
$ java UDPPoke rama.poly.edu 19
123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuv
Some Useful Applications | 423
Given this class, UDP daytime, time, chargen, and quote of the day clients are almost
trivial. A time client is only slightly harder, and only because you need to convert the
four raw bytes returned by the server to a java.util.Date object. The same algorithm
as in Example 8-3 will accomplish that as demonstrated in Example 12-8.
Example 12-8. A UDP time client
import java.net.*;
import java.util.*;
public class UDPTimeClient {
 public final static int PORT = 37;
 public final static String DEFAULT_HOST = "time.nist.gov";
 public static void main(String[] args) {
 InetAddress host;
 try {
 if (args.length > 0) {
 host = InetAddress.getByName(args[0]);
 } else {
 host = InetAddress.getByName(DEFAULT_HOST);
 }
 } catch (RuntimeException | UnknownHostException ex) {
 System.out.println("Usage: java UDPTimeClient [host]");
 return;
 }
 UDPPoke poker = new UDPPoke(host, PORT);
 byte[] response = poker.poke();
 if (response == null) {
 System.out.println("No response within allotted time");
 return;
 } else if (response.length != 4) {
 System.out.println("Unrecognized response format");
 return;
 }
 // The time protocol sets the epoch at 1900,
 // the Java Date class at 1970. This number
 // converts between them.
 long differenceBetweenEpochs = 2208988800L;
 long secondsSince1900 = 0;
 for (int i = 0; i < 4; i++) {
 secondsSince1900
 = (secondsSince1900 << 8) | (response[i] & 0x000000FF);
 }
 long secondsSince1970
424 | Chapter 12: UDP
 = secondsSince1900 - differenceBetweenEpochs;
 long msSince1970 = secondsSince1970 * 1000;
 Date time = new Date(msSince1970);
 System.out.println(time);
 }
}
UDPServer
Clients aren’t the only programs that benefit from a reusable implementation. The
servers for these protocols are also very similar. They all wait for UDP datagrams on a
specified port and reply to each datagram with another datagram. The servers differ
only in the content of the datagram that they return. Example 12-9 is a simple iterative
UDPServer class that can be subclassed to provide specific servers for different protocols.
The UDPServer class has two fields, the int bufferSize and the DatagramSocket sock
et, the latter of which is protected so it can be used by subclasses. The constructor opens
a datagram socket on a specified local port to receive datagrams of no more than
bufferSize bytes.
UDPServer implements Runnable so that multiple instances can run in parallel. Its run()
method contains a loop that repeatedly receives an incoming datagram and responds
by passing it to the abstract respond() method. This method will be overridden by
particular subclasses in order to implement different kinds of servers.
Assuming this class may be used as part of other programs that do more than just run
one server, you need a way to shut it down. This is provided by the shutDown() method,
which sets a flag. The main loop checks this flag each pass to see if it should exit. Because
the receive() call can block indefinitely if there’s no traffic, you also set a timeout on
the socket. This will wake it up once every 10 seconds to check for shutdown whether
there’s traffic or not.
UDPServer is a very flexible class. Subclasses can send zero, one, or many datagrams in
response to each incoming datagram. If a lot of processing is required to respond to a
packet, the respond() method can spawn a thread to do it. However, UDP servers tend
not to have extended interactions with a client. Each incoming packet is treated inde‐
pendently of other packets, so the response can usually be handled directly in the
respond() method without spawning a thread.
Example 12-9. The UDPServer class
import java.io.*;
import java.net.*;
import java.util.logging.*;
public abstract class UDPServer implements Runnable {
Some Useful Applications | 425
 private final int bufferSize; // in bytes
 private final int port;
 private final Logger logger = Logger.getLogger(UDPServer.class.getCanonicalName());
 private volatile boolean isShutDown = false;
 public UDPServer(int port, int bufferSize) {
 this.bufferSize = bufferSize;
 this.port = port;
 }
 public UDPServer(int port) {
 this(port, 8192);
 }
 @Override
 public void run() {
 byte[] buffer = new byte[bufferSize];
 try (DatagramSocket socket = new DatagramSocket(port)) {
 socket.setSoTimeout(10000); // check every 10 seconds for shutdown
 while (true) {
 if (isShutDown) return;
 DatagramPacket incoming = new DatagramPacket(buffer, buffer.length);
 try {
 socket.receive(incoming);
 this.respond(socket, incoming);
 } catch (SocketTimeoutException ex) {
 if (isShutDown) return;
 } catch (IOException ex) {
 logger.log(Level.WARNING, ex.getMessage(), ex);
 }
 } // end while
 } catch (SocketException ex) {
 logger.log(Level.SEVERE, "Could not bind to port: " + port, ex);
 }
 }
 public abstract void respond(DatagramSocket socket, DatagramPacket request)
 throws IOException;
 public void shutDown() {
 this.isShutDown = true;
 }
}
The easiest protocol to handle is discard. All that’s needed is a main() method that sets
the port and starts the thread. respond() is a do-nothing method. Example 12-10 is a
high-performance UDP discard server that does nothing with incoming packets.
426 | Chapter 12: UDP
Example 12-10. A UDP discard server
import java.net.*;
public class FastUDPDiscardServer extends UDPServer {
 public final static int DEFAULT_PORT = 9;
 public FastUDPDiscardServer() {
 super(DEFAULT_PORT);
 }
 public static void main(String[] args) {
 UDPServer server = new FastUDPDiscardServer();
 Thread t = new Thread(server);
 t.start();
 }
 @Override
 public void respond(DatagramSocket socket, DatagramPacket request) {
 }
}
It isn’t much harder to implement an echo server, as Example 12-11 shows. Unlike a
stream-based TCP echo server, multiple threads are not required to handle multiple
clients.
Example 12-11. A UDP echo server
import java.io.*;
import java.net.*;
public class UDPEchoServer extends UDPServer {
 public final static int DEFAULT_PORT = 7;
 public UDPEchoServer() {
 super(DEFAULT_PORT);
 }
 @Override
 public void respond(DatagramSocket socket, DatagramPacket packet)
 throws IOException {
 DatagramPacket outgoing = new DatagramPacket(packet.getData(),
 packet.getLength(), packet.getAddress(), packet.getPort());
 socket.send(outgoing);
 }
 public static void main(String[] args) {
 UDPServer server = new UDPEchoServer();
 Thread t = new Thread(server);
 t.start();
Some Useful Applications | 427
 }
}
A UDP Echo Client
The UDPPoke class implemented earlier isn’t suitable for all protocols. In particular, pro‐
tocols that require multiple datagrams require a different implementation. The echo
protocol has both TCP and UDP implementations. Implementing the echo protocol
with TCP is simple; it’s more complex with UDP because you don’t have I/O streams or
the concept of a connection to work with. A TCP-based echo client can send a message
and wait for a response on the same connection. However, a UDP-based echo client has
no guarantee that the message it sent was received. Therefore, it cannot simply wait for
the response; it needs to be prepared to send and receive data asynchronously.
This behavior is fairly simple to implement using threads, however. One thread can
process user input and send it to the echo server, while a second thread accepts input
from the server and displays it to the user. The client is divided into three classes: the
main UDPEchoClient class, the SenderThread class, and the ReceiverThread class.
The UDPEchoClient class should look familiar. It reads a hostname from the command
line and converts it to an InetAddress object. UDPEchoClient uses this object and the
default echo port to construct a SenderThread object. This constructor can throw a
SocketException, so the exception must be caught. Then the SenderThread starts. The
same DatagramSocket that the SenderThread uses is used to construct a ReceiverTh
read, which is then started. It’s important to use the same DatagramSocket for both
sending and receiving data because the echo server will send the response back to the
port the data was sent from. Example 12-12 shows the code for the UDPEchoClient.
Example 12-12. The UDPEchoClient class
import java.net.*;
public class UDPEchoClient {
 public final static int PORT = 7;
 public static void main(String[] args) {
 String hostname = "localhost";
 if (args.length > 0) {
 hostname = args[0];
 }
 try {
 InetAddress ia = InetAddress.getByName(hostname);
 DatagramSocket socket = new DatagramSocket();
 SenderThread sender = new SenderThread(socket, ia, PORT);
 sender.start();
428 | Chapter 12: UDP
 Thread receiver = new ReceiverThread(socket);
 receiver.start();
 } catch (UnknownHostException ex) {
 System.err.println(ex);
 } catch (SocketException ex) {
 System.err.println(ex);
 }
 }
}
The SenderThread class reads input from the console a line at a time and sends it to the
echo server. It’s shown in Example 12-13. The input is provided by System.in, but a
different client could include an option to read input from a different stream—perhaps
opening a FileInputStream to read from a file. The fields of this class define the server
to which it sends data, the port on that server, and the DatagramSocket that does the
sending, all set in the single constructor. The DatagramSocket is connected to the remote
server to make sure all datagrams received were in fact sent by the right server. It’s rather
unlikely that some other server on the Internet is going to bombard this particular port
with extraneous data, so this is not a big flaw. However, it’s a good habit to make sure
that the packets you receive come from the right place, especially if security is a concern.
The run() method processes user input a line at a time. To do this, the BufferedRead
er userInput is chained to System.in. An infinite loop reads lines of user input. Each
line is stored in theLine. A period on a line by itself signals the end of user input and
breaks out of the loop. Otherwise, the bytes of data are stored in the data array using
the getBytes() method from java.lang.String. Next, the data array is placed in the
payload part of the DatagramPacket output, along with information about the server,
the port, and the data length. This packet is then sent to its destination by socket. This
thread then yields to give other threads an opportunity to run.
Example 12-13. The SenderThread class
import java.io.*;
import java.net.*;
class SenderThread extends Thread {
 private InetAddress server;
 private DatagramSocket socket;
 private int port;
 private volatile boolean stopped = false;
 SenderThread(DatagramSocket socket, InetAddress address, int port) {
 this.server = address;
 this.port = port;
 this.socket = socket;
 this.socket.connect(server, port);
 }
Some Useful Applications | 429
 public void halt() {
 this.stopped = true;
 }
 @Override
 public void run() {
 try {
 BufferedReader userInput
 = new BufferedReader(new InputStreamReader(System.in));
 while (true) {
 if (stopped) return;
 String theLine = userInput.readLine();
 if (theLine.equals(".")) break;
 byte[] data = theLine.getBytes("UTF-8");
 DatagramPacket output
 = new DatagramPacket(data, data.length, server, port);
 socket.send(output);
 Thread.yield();
 }
 } catch (IOException ex) {
 System.err.println(ex);
 }
 }
}
The ReceiverThread class shown in Example 12-14 waits for datagrams to arrive from
the network. When a datagram is received, it is converted to a String and printed on
System.out for display to the user. A more advanced echo client could include an option
to send the output elsewhere.
This class has two fields. The more important is the DatagramSocket, theSocket, which
must be the same DatagramSocket used by the SenderThread. Data arrives on the port
used by that DatagramSocket; any other DatagramSocket would not be allowed to con‐
nect to the same port. The second field, stopped, is a boolean used to halt this thread
without invoking the deprecated stop() method.
The run() method is an infinite loop that uses socket’s receive() method to wait for
incoming datagrams. When an incoming datagram appears, it is converted into a String
with the same length as the incoming data and printed on System.out. As in the input
thread, this thread then yields to give other threads an opportunity to execute.
Example 12-14. The ReceiverThread class
import java.io.*;
import java.net.*;
class ReceiverThread extends Thread {
 private DatagramSocket socket;
430 | Chapter 12: UDP
 private volatile boolean stopped = false;
 ReceiverThread(DatagramSocket socket) {
 this.socket = socket;
 }
 public void halt() {
 this.stopped = true;
 }
 @Override
 public void run() {
 byte[] buffer = new byte[65507];
 while (true) {
 if (stopped) return;
 DatagramPacket dp = new DatagramPacket(buffer, buffer.length);
 try {
 socket.receive(dp);
 String s = new String(dp.getData(), 0, dp.getLength(), "UTF-8");
 System.out.println(s);
 Thread.yield();
 } catch (IOException ex) {
 System.err.println(ex);
 }
 }
 }
}
You can run the echo client on one machine and connect to the echo server on a second
machine to verify that the network is functioning properly between them.
DatagramChannel
The DatagramChannel class is used for nonblocking UDP applications, in the same way
as SocketChannel and ServerSocketChannel are used for nonblocking TCP applica‐
tions. Like SocketChannel and ServerSocketChannel, DatagramChannel is a subclass
of SelectableChannel that can be registered with a Selector. This is useful in servers
where one thread can manage communications with multiple clients. However, UDP is
by its nature much more asynchronous than TCP so the net effect is smaller. In UDP, a
single datagram socket can process requests from multiple clients for both input and
output. What the DatagramChannel class adds is the ability to do this in a nonblocking
fashion, so methods return quickly if the network isn’t immediately ready to receive or
send data.
Using DatagramChannel
DatagramChannel is a near-complete alternate API for UDP. In Java 6 and earlier, you
still need to use the DatagramSocket class to bind a channel to a port. However, you do
DatagramChannel | 431
not have to use it thereafter, and you don’t have to use it all in Java 7 and later. Nor do
you ever use DatagramPacket. Instead, you read and write byte buffers, just as you do
with a SocketChannel.
Opening a socket
The java.nio.channels.DatagramChannel class does not have any public construc‐
tors. Instead, you create a new DatagramChannel object using the static open() method
For example:
DatagramChannel channel = DatagramChannel.open();
This channel is not initially bound to any port. To bind it, you access the channel’s peer
DatagramSocket object using the socket() method. For example, this binds a channel
to port 3141:
SocketAddress address = new InetSocketAddress(3141);
DatagramSocket socket = channel.socket();
socket.bind(address);
Java 7 adds a convenient bind() method directly to DatagramChannel, so you don’t have
to use a DatagramSocket at all. For example:
SocketAddress address = new InetSocketAddress(3141);
channel.bind(address);
Receiving
The receive() method reads one datagram packet from the channel into a ByteBuffer.
It returns the address of the host that sent the packet:
public SocketAddress receive(ByteBuffer dst) throws IOException
If the channel is blocking (the default), this method will not return until a packet has
been read. If the channel is nonblocking, this method will immediately return null if no
packet is available to read.
If the datagram packet has more data than the buffer can hold, the extra data is thrown
away with no notification of the problem. You do not receive a BufferOverflowExcep
tion or anything similar. Again you see that UDP is unreliable. This behavior introduces
an additional layer of unreliability into the system. The data can arrive safely from the
network and still be lost inside your own program.
Using this method, you can reimplement the discard server to log the host sending the
data as well as the data sent. Example 12-15 demonstrates. It avoids the potential loss of
data by using a buffer that’s big enough to hold any UDP packet and clearing it before
it’s used again.
Example 12-15. A UDPDiscardServer based on channels
import java.io.*;
import java.net.*;

import java.nio.*;
import java.nio.channels.*;
public class UDPDiscardServerWithChannels {
 public final static int PORT = 9;
 public final static int MAX_PACKET_SIZE = 65507;
 public static void main(String[] args) {
 try {
 DatagramChannel channel = DatagramChannel.open();
 DatagramSocket socket = channel.socket();
 SocketAddress address = new InetSocketAddress(PORT);
 socket.bind(address);
 ByteBuffer buffer = ByteBuffer.allocateDirect(MAX_PACKET_SIZE);
 while (true) {
 SocketAddress client = channel.receive(buffer);
 buffer.flip();
 System.out.print(client + " says ");
 while (buffer.hasRemaining()) System.out.write(buffer.get());
 System.out.println();
 buffer.clear();
 }
 } catch (IOException ex) {
 System.err.println(ex);
 }
 }
}
Sending
The send() method writes one datagram packet into the channel from a ByteBuffer
to the address specified as the second argument:
public int send(ByteBuffer src, SocketAddress target) throws IOException
The source ByteBuffer can be reused if you want to send the same data to multiple
clients. Just don’t forget to rewind it first.
The send() method returns the number of bytes written. This will either be the number
of bytes that were available in the buffer to be written or zero, nothing in between. It is
zero if the channel is in nonblocking mode and the data can’t be sent immediately.
Otherwise, if the channel is not in nonblocking mode, send() simply waits to return
until it can send all the data in the buffer.
Example 12-16 demonstrates with a simple echo server based on channels. Just as it did
in Example 12-15, the receive() method reads a packet. However, this time, rather
than logging the packet on System.out, it returns the same data to the client that sent
it.
DatagramChannel | 433
Example 12-16. A UDPEchoServer based on channels
import java.io.*;
import java.net.*;
import java.nio.*;
import java.nio.channels.*;
public class UDPEchoServerWithChannels {
 public final static int PORT = 7;
 public final static int MAX_PACKET_SIZE = 65507;
 public static void main(String[] args) {
 try {
 DatagramChannel channel = DatagramChannel.open();
 DatagramSocket socket = channel.socket();
 SocketAddress address = new InetSocketAddress(PORT);
 socket.bind(address);
 ByteBuffer buffer = ByteBuffer.allocateDirect(MAX_PACKET_SIZE);
 while (true) {
 SocketAddress client = channel.receive(buffer);
 buffer.flip();
 channel.send(buffer, client);
 buffer.clear();
 }
 } catch (IOException ex) {
 System.err.println(ex);
 }
 }
}
This program is iterative, blocking, and synchronous. This is much less of a problem
for UDP-based protocols than for TCP protocols. The unreliable, packet-based, con‐
nectionless nature of UDP means that the server at most has to wait for the local buffer
to clear. It does not wait for the client to be ready to receive data. There’s much less
opportunity for one client to get held up behind a slower client.
Connecting
Once you’ve opened a datagram channel, you connect it to a particular remote address
using the connect() method:
SocketAddress remote = new InetSocketAddress("time.nist.gov", 37);
channel.connect(remote);
The channel will only send data to or receive data from this host. Unlike the con
nect() method of SocketChannel, this method alone does not send or receive any
packets across the network because UDP is a connectionless protocol. It merely estab‐
lishes the host it will send packets to when there’s data ready to be sent. Thus, con
nect() returns fairly quickly, and doesn’t block in any meaningful sense. There’s no
434 | Chapter 12: UDP
need here for a finishConnect() or isConnectionPending() method. There is an
isConnected() method that returns true if and only if the DatagramSocket is connec‐
ted:
public boolean isConnected()
This tells you whether the DatagramChannel is limited to one host. Unlike SocketChan
nel, a DatagramChannel doesn’t have to be connected to transmit or receive data.
Finally, the disconnect() method breaks the connection:
public DatagramChannel disconnect() throws IOException
This doesn’t really close anything because nothing was really open in the first place. It
just allows the channel to be connected to a different host in the future.
Reading
Besides the special-purpose receive() method, DatagramChannel has the usual three
read() methods:
public int read(ByteBuffer dst) throws IOException
public long read(ByteBuffer[] dsts) throws IOException
public long read(ByteBuffer[] dsts, int offset, int length)
 throws IOException
However, these methods can only be used on connected channels. That is, before in‐
voking one of these methods, you must invoke connect() to glue the channel to a
particular remote host. This makes them more suitable for use with clients that know
who they’ll be talking to than for servers that must accept input from multiple hosts at
the same time that are normally not known prior to the arrival of the first packet.
Each of these three methods only reads a single datagram packet from the network. As
much data from that datagram as possible is stored in the argument ByteBuffer(s).
Each method returns the number of bytes read or –1 if the channel has been closed.
This method may return 0 for any of several reasons, including:
• The channel is nonblocking and no packet was ready.
• A datagram packet contained no data.
• The buffer is full.
As with the receive() method, if the datagram packet has more data than the Byte
Buffer(s) can hold, the extra data is thrown away with no notification of the problem.
You do not receive a BufferOverflowException or anything similar.
Writing
Naturally, DatagramChannel has the three write methods common to all writable, scat‐
tering channels, which can be used instead of the send() method:
DatagramChannel | 435
public int write(ByteBuffer src) throws IOException
public long write(ByteBuffer[] dsts) throws IOException
public long write(ByteBuffer[] dsts, int offset, int length)
 throws IOException
These methods can only be used on connected channels; otherwise, they don’t know
where to send the packet. Each of these methods sends a single datagram packet over
the connection. None of these methods are guaranteed to write the complete contents
of the buffer(s). Fortunately, the cursor-based nature of buffers enables you to easily call
this method again and again until the buffer is fully drained and the data has been
completely sent, possibly using multiple datagram packets. For example:
while (buffer.hasRemaining() && channel.write(buffer) != -1) ;
You can use the read and write methods to implement a simple UDP echo client. On
the client side, it’s easy to connect before sending. Because packets may be lost in transit
(always remember UDP is unreliable), you don’t want to tie up the sending while waiting
to receive a packet. Thus, you can take advantage of selectors and nonblocking I/O.
These work for UDP pretty much exactly like they worked for TCP in Chapter 11. This
time, though, rather than sending text data, let’s send one hundred ints from 0 to 99.
You’ll print out the values returned so it will be easy to figure out if any packets are being
lost. Example 12-17 demonstrates.
Example 12-17. A UDP echo client based on channels
import java.io.*;
import java.net.*;
import java.nio.*;
import java.nio.channels.*;
import java.util.*;
public class UDPEchoClientWithChannels {
 public final static int PORT = 7;
 private final static int LIMIT = 100;
 public static void main(String[] args) {
 SocketAddress remote;
 try {
 remote = new InetSocketAddress(args[0], PORT);
 } catch (RuntimeException ex) {
 System.err.println("Usage: java UDPEchoClientWithChannels host");
 return;
 }
 try (DatagramChannel channel = DatagramChannel.open()) {
 channel.configureBlocking(false);
 channel.connect(remote);
 Selector selector = Selector.open();
436 | Chapter 12: UDP
 channel.register(selector, SelectionKey.OP_READ | SelectionKey.OP_WRITE);
 ByteBuffer buffer = ByteBuffer.allocate(4);
 int n = 0;
 int numbersRead = 0;
 while (true) {
 if (numbersRead == LIMIT) break;
 // wait one minute for a connection
 selector.select(60000);
 Set<SelectionKey> readyKeys = selector.selectedKeys();
 if (readyKeys.isEmpty() && n == LIMIT) {
 // All packets have been written and it doesn't look like any
 // more are will arrive from the network
 break;
 }
 else {
 Iterator<SelectionKey> iterator = readyKeys.iterator();
 while (iterator.hasNext()) {
 SelectionKey key = (SelectionKey) iterator.next();
 iterator.remove();
 if (key.isReadable()) {
 buffer.clear();
 channel.read(buffer);
 buffer.flip();
 int echo = buffer.getInt();
 System.out.println("Read: " + echo);
 numbersRead++;
 }
 if (key.isWritable()) {
 buffer.clear();
 buffer.putInt(n);
 buffer.flip();
 channel.write(buffer);
 System.out.println("Wrote: " + n);
 n++;
 if (n == LIMIT) {
 // All packets have been written; switch to read-only mode
 key.interestOps(SelectionKey.OP_READ);
 }
 }
 }
 }
 }
 System.out.println("Echoed " + numbersRead + " out of " + LIMIT +
 " sent");
 System.out.println("Success rate: " + 100.0 * numbersRead / LIMIT +
 "%");
 } catch (IOException ex) {
 System.err.println(ex);
 }
 }
}
DatagramChannel | 437
There is one major difference between selecting TCP channels and selecting datagram
channels. Because datagram channels are truly connectionless (despite the connect()
method), you need to notice when the data transfer is complete and shut down. In this
example, you assume the data is finished when all packets have been sent and one minute
has passed since the last packet was received. Any expected packets that have not been
received by this point are assumed to be lost in the ether.
A typical run produced output like this:
Wrote: 0
Read: 0
Wrote: 1
Wrote: 2
Read: 1
Wrote: 3
Read: 2
Wrote: 4
Wrote: 5
Wrote: 6
Wrote: 7
Wrote: 8
Wrote: 9
Wrote: 10
Wrote: 11
Wrote: 12
Wrote: 13
Wrote: 14
Wrote: 15
Wrote: 16
Wrote: 17
Wrote: 18
Wrote: 19
Wrote: 20
Wrote: 21
Wrote: 22
Read: 3
Wrote: 23
...
Wrote: 97
Read: 72
Wrote: 98
Read: 73
Wrote: 99
Read: 75
Read: 76
...
Read: 97
Read: 98
Read: 99
Echoed 92 out of 100 sent
Success rate: 92.0%
438 | Chapter 12: UDP
Connecting to a remote server a couple of miles and seven hops away (according to
traceroute), I saw between 90% and 98% of the packets make the round trip.
Closing
Just as with regular datagram sockets, a channel should be closed when you’re done with
it to free up the port and any other resources it may be using:
public void close() throws IOException
Closing an already closed channel has no effect. Attempting to write data to or read data
from a closed channel throws an exception. If you’re uncertain whether a channel has
been closed, check with isOpen():
public boolean isOpen()
This returns false if the channel is closed, true if it’s open.
Like all channels, in Java 7 DatagramChannel implements AutoCloseable so you can
use it in try-with-resources statements. Prior to Java 7, close it in a finally block if you
can. By now the pattern should be quite familiar. In Java 6 and earlier:
DatagramChannel channel = null;
try {
 channel = DatagramChannel.open();
 // Use the channel...
} catch (IOException ex) {
 // handle exceptions...
} finally {
 if (channel != null) {
 try {
 channel.close();
 } catch (IOException ex) {
 // ignore
 }
 }
}
and in Java 7 and later:
try (DatagramChannel channel = DatagramChannel.open()) {
 // Use the channel...
} catch (IOException ex) {
 // handle exceptions...
}
Socket Options // Java 7
In Java 7 and later, DatagramChannel supports eight socket options listed in Table 12-1.
DatagramChannel | 439
Table 12-1. Socket options supported by datagram sockets
Option Type Constant Purpose
SO_SNDBUF StandardSocketOptions. Integer Size of the buffer used for
sending datagram packets
SO_RCVBUF StandardSocketOp
tions.SO_RCVBUF
Integer Size of the buffer used for
receiving datagram packets
SO_REUSEADDR StandardSocketOptions.SO_RE
USEADDR
Boolean Enable/disable address reuse
SO_BROADCAST StandardSocketOp
tions.SO_BROADCAST
Boolean Enable/disable broadcast
messages
IP_TOS StandardSocketOptions.IP_TOS Integer Traffic class
IP_MULTICAST_IF StandardSocketOptions.IP_MUL
TICAST_IF
NetworkInter
face
Local networkinterfacetousefor
multicast
IP_MULTICAST_TTL StandardSocketOptions.IP_MUL
TICAST_TTL
Integer Time-to-live value for multicast
datagrams
IP_MULTICAST_LOOP StandardSocketOptions.IP_MUL
TICAST_LOOP
Boolean Enable/disable loopback of
multicast datagrams
The first five options have the same meanings as they do for datagram sockets as de‐
scribed in “Socket Options” on page 417. The last three are used by multicast sockets,
which we’ll take up in Chapter 13.
These are inspected and configured by just three methods:
public <T> DatagramChannel setOption(SocketOption<T> name, T value)
 throws IOException
public <T> T getOption(SocketOption<T> name) throws IOException
public Set<SocketOption<?>> supportedOptions()
The supportedOptions() method lists the available socket options. The getOption()
method tells you the current value of any of these. And setOption() lets you change
the value. For example, suppose you want to send a broadcast message. SO_BROAD‐
CAST is usually turned off by default, but you can switch it on like so:
try (DatagramChannel channel = DatagramChannel.open()) {
 channel.setOption(StandardSocketOptions.SO_BROADCAST, true);
 // Send the broadcast message...
} catch (IOException ex) {
 // handle exceptions...
}
Example 12-18 opens a channel just to check the default values of these options.
Example 12-18. Default socket option values
import java.io.IOException;
import java.net.SocketOption;
import java.nio.channels.DatagramChannel;
440 | Chapter 12: UDP
public class DefaultSocketOptionValues {
 public static void main(String[] args) {
 try (DatagramChannel channel = DatagramChannel.open()) {
 for (SocketOption<?> option : channel.supportedOptions()) {
 System.out.println(option.name() + ": " + channel.getOption(option));
 }
 } catch (IOException ex) {
 ex.printStackTrace();
 }
 }
}
Here’s the output I got on my Mac:
IP_MULTICAST_TTL: 1
SO_BROADCAST: false
SO_REUSEADDR: false
SO_RCVBUF: 196724
IP_MULTICAST_LOOP: true
SO_SNDBUF: 9216
IP_MULTICAST_IF: null
IP_TOS: 0
It’s a bit surprising that my send buffer is so much larger than my receive buffer.