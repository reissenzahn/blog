

# Introduction

Gnutella is a large peer-to-peer network. It was the first decentralized peer-to-peer network of its kind, leading to other, later networks adopting the model. It celebrated two decades of existence on March 14, 2020, and has a user base in the millions for peer-to-peer file sharing.

The Gnutella network is a fully distributed alternative to such semi-centralized systems as FastTrack (KaZaA) and the original Napster. The initial popularity of the network was spurred on by Napster's threatened legal demise in early 2001. This growing surge in popularity revealed the limits of the initial protocol's scalability. In early 2001, variations on the protocol (first implemented in proprietary and closed source clients) allowed an improvement in scalability. Instead of treating every user as client and server, some users were now treated as ultrapeers, routing search requests and responses for users connected to them.

One of the benefits of having Gnutella so decentralized is to make it very difficult to shut the network down and to make it a network in which the users are the only ones who can decide which content will be available. Unlike Napster, where the entire network relied on the central server, Gnutella cannot be shut down by shutting down any one node. A decentralized network prevents bad actors from taking control of the contents of the network and/or manipulating data by controlling the central server.


# Design

To envision how Gnutella originally worked, imagine a large circle of users (called nodes), each of whom has Gnutella client software. On initial startup, the client software must bootstrap and find at least one other node. Various methods have been used for this, including a pre-existing address list of possibly working nodes shipped with the software, using updated web caches of known nodes (called Gnutella Web Caches), UDP host caches and, rarely, even IRC.

Once connected, the client requests a list of working addresses. The client tries to connect to the nodes it was shipped with, as well as nodes it receives from other clients until it reaches a certain quota. It connects to only that many nodes, locally caching the addresses it has not yet tried and discards the addresses it tried that were invalid.

When the user wants to do a search, the client sends the request to each actively connected node. In version 0.4 of the protocol, the number of actively connected nodes for a client was quite small (around 5), so each node then forwarded the request to all its actively connected nodes, and they, in turn, forwarded the request, and so on, until the packet reached a predetermined number of hops from the sender (maximum 7).


Since version 0.6 (2002), Gnutella is a composite network made of leaf nodes and ultra nodes (also called ultrapeers). The leaf nodes are connected to a small number of ultrapeers (typically 3) while each ultrapeer is connected to more than 32 other ultrapeers. With this higher outdegree, the maximum number of hops a query can travel was lowered to 4.

Leaves and ultrapeers use the Query Routing Protocol to exchange a Query Routing Table (QRT), a table of 64 Ki-slots and up to 2 Mi-slots consisting of hashed keywords. A leaf node sends its QRT to each of the ultrapeers it is connected to, and ultrapeers merge the QRT of all their leaves (downsized to 128 Ki-slots) plus their own QRT (if they share files) and exchange that with their own neighbors. Query routing is then done by hashing the words of the query and seeing whether all of them match in the QRT. Ultrapeers do that check before forwarding a query to a leaf node, and also before forwarding the query to a peer ultra node provided this is the last hop the query can travel.

If a search request turns up a result, the node that has the result contacts the searcher. In the classic Gnutella protocol, response messages were sent back along the route the query came through, as the query itself did not contain identifying information of the node. This scheme was later revised, so that search results now are delivered over User Datagram Protocol (UDP) directly to the node that initiated the search, usually an ultrapeer of the node. Thus, in the current protocol, the queries carry the IP address and port number of either node. This lowers the amount of traffic routed through the Gnutella network, making it significantly more scalable.[citation needed]

If the user decides to download the file, they negotiate the file transfer. If the node which has the requested file is not firewalled, the querying node can connect to it directly. However, if the node is firewalled, stopping the source node from receiving incoming connections, the client wanting to download a file sends it a so-called push request to the server for the remote client to initiate the connection instead (to push the file). At first, these push requests were routed along the original chain it used to send the query. This was rather unreliable because routes would often break and routed packets are always subject to flow control. Therefore, so-called push proxies were introduced. These are usually the ultrapeers of a leaf node and they are announced in search results. The client connects to one of these push proxies using an HTTP request and the proxy sends a push request to leaf on behalf of the client. Normally, it is also possible to send a push request over UDP to the push proxy which is more efficient than using TCP. Push proxies have two advantages: First, ultrapeer-leaf connections are more stable than routes which makes push requests much more reliable. Second, it reduces the amount of traffic routed through the Gnutella network.[citation needed]

Finally, when a user disconnects, the client software saves the list of nodes that it was actively connected to and those collected from pong packets for use the next time it attempts to connect so that it becomes independent from any kind of bootstrap services.[citation needed]

In practice, this method of searching on the Gnutella network was often unreliable. Each node is a regular computer user; as such, they are constantly connecting and disconnecting, so the network is never completely stable. Also, the bandwidth cost of searching on Gnutella grew exponentially to the number of connected users,[20] often saturating connections and rendering slower nodes useless. Therefore, search requests would often be dropped, and most queries reached only a very small part of the network. This observation identified the Gnutella network as an unscalable distributed system, and inspired the development of distributed hash tables, which are much more scalable but support only exact-match, rather than keyword, search.[citation needed]

To address the problems of bottlenecks, Gnutella developers implemented a tiered system of ultrapeers and leaves. Instead of all nodes being considered equal, nodes entering into the network were kept at the 'edge' of the network as a leaf, not responsible for any routing, and nodes which were capable of routing messages were promoted to ultrapeers, which would accept leaf connections and route searches and network maintenance messages. This allowed searches to propagate further through the network, and allowed for numerous alterations in the topology which have improved the efficiency and scalability greatly.[citation needed]

Additionally, gnutella adopted a number of other techniques to reduce traffic overhead and make searches more efficient. Most notable are Query Routing Protocol (QRP) and Dynamic Querying (DQ). With QRP a search reaches only those clients which are likely to have the files, so rare files searches grow vastly more efficient, and with DQ the search stops as soon as the program has acquired enough search results, which vastly reduces the amount of traffic caused by popular searches. Gnutella For Users has a vast amount of information about these and other improvements to Gnutella in user-friendly style.[citation needed]


# Protocol features and extensions

Gnutella did once operate on a purely query flooding-based protocol. The outdated Gnutella version 0.4 network protocol employs five different packet types, namely

ping: discover hosts on network
pong: reply to ping
query: search for a file
query hit: reply to query
push: download request for firewalled servants
These are mainly concerned with searching the Gnutella network. File transfers are handled using HTTP.[23]

The development of the Gnutella protocol is currently led by the Gnutella Developers Forum (The GDF). Many protocol extensions have been and are being developed by the software vendors and free Gnutella developers of the GDF. These extensions include intelligent query routing, SHA-1 checksums, query hit transmission via UDP, querying via UDP, dynamic queries via TCP, file transfers via UDP, XML metadata, source exchange (also termed the download mesh) and parallel downloading in slices (swarming).[23]

There are efforts to finalize these protocol extensions in the Gnutella 0.6 specification at the Gnutella protocol development website. The Gnutella 0.4 standard, although still being the latest protocol specification since all extensions only exist as proposals so far, is outdated. In fact, it is hard or impossible to connect today with the 0.4 handshakes and according to developers in the GDF, version 0.6 is what new developers should pursue using the work-in-progress specifications.

The Gnutella protocol remains under development and in spite of attempts to make a clean break with the complexity inherited from the old Gnutella 0.4 and to design a clean new message architecture, it is still one of the most successful file-sharing protocols to date.