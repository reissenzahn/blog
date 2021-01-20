# BitTorrent Algorithms


## Introduction



<h3 id="hashing">Hashing</h3>

<!-- 
  In order to keep track of which peers have what, BitTorrent cuts files into pieces of fixed size, typically a quarter megabyte.

  Each downloader reports to all of its peers what pieces it has.

  To verify data integrity, the SHA1 hashes of all the pieces are included in the .torrent file and peers don't report that they have a piece until they've checked the hash.

  Peers continuously download pieces from all peers which they can.

  They of course cannot download from peers they aren’t connected to, and sometimes peers don’t have any pieces they want or won’t currently let them download.
-->





<h3 id="pipelining">Pipelining</h3>

<!-- 
  When transferring data over TCP, it is very important to always have several requests pending at once to avoid a delay between pieces being sent.

  BitTorrent facilitates this by breaking pieces further into sub-pieces over the wire, typically 16 KB in size, and always keeping some number, typically five, requests pipelined at once.

  Every time a sub-piece arrives a new request is sent.

  The amount of data to pipeline has been selected as a value which can reliably saturate most connections.



  In general peers are advised to keep a few unfullfilled requests on each connection. This is done because otherwise a full round trip is required from the download of one block to begining the download of a new block
-->





<h3 id="endgame-mode">Endgame Mode</h3>

<!-- 
  Sometimes a piece will be requested from a peer with very slow transfer rates.

  This isn't a problem in the middle of a download but it could potentially delay a download's finish.

  To prevent this, once all sub-pieces which a peer doesn't have are actively requested it sends requests for all sub-pieces to all peers.

  Cancels are sent for sub-pieces which arrive to keep too much bandwidth from being wasted on redundant sends.

  In practice not much bandwidth is wasted as the endgame period is very short.

  While normally each block is requested from a single peer, blocks in the last pending pieces should be downloaded from all peers, on a “whoever sends it first” basis. Once they arrive, requests to the slower peers are simply cancelled. This wastes some bandwidth but saves quite a bit of time.
-->



<h3 id="choking">Choking</h3>

<!--
  BitTorrent does no central resource allocation.

  Each peer is responsible for attempting to maximize its own download rate.

  Peers do this by downloading from whoever they can and deciding which peers to upload to via a variant of tit-for-tat.

  To cooperate, peers upload and to not cooperate they choke peers.

  Choking is a temporary refusal to upload; it stops uploading but downloading can still happen and the connection doesn’t need to be renegotiated when choking stops.

  A good choking algorithm should utilize all available resources, provide reasonably consistent download rates for everyone, and be somewhat resistant to peers only downloading and not uploading.

  Each BitTorrent peer always unchokes a fixed number of other peers (default is four).

  This approach allows TCP's builtin congestion control to reliably saturate upload capacity.

  Decisions as to which peers to unchoke are based strictly on current download rate.
  
  Calculating current download rate meaningfully is a surprisingly difficult problem; The current implementation essentially uses a rolling 20-second average.

  To avoid situations in which resources are wasted by rapidly choking and unchoking peers, BitTorrent peers recalculate who they want to choke once every ten seconds, and then leave the situation as is until the next ten second period is up.
  
  Ten seconds is a long enough period of time for TCP to ramp up new transfers to their full capacity.
-->


<h3 id="optimistic-unchoking">Optimistic Un-choking</h3>

<!-- 
  Simply uploading to the peers which provide the best download rate would suffer from having no method of discovering if currently unused connections are better than the ones being used.

  To fix this, at all times, a BitTorrent peer has a single 'optimistic unchoke' which is unchoked regardless of the current download rate from it.

  Which peer is the optimistic unchoke is rotated every third rechoke period (30 seconds).

  30 seconds is enough time for the upload to get to full capacity, the download to reciprocate, and the download to get to full capacity.
-->


<h3 id="anti-snubbing">Anti-snubbing</h3>


<!-- 
  Occasionally a BitTorrent peer will be choked by all
peers which it was formerly downloading from. In
such cases it will usually continue to get poor download
rates until the optimistic unchoke finds better
peers. To mitigate this problem, when over a minute
goes by without getting a single piece from a particular
peer, BitTorrent assumes it is ’snubbed’ by
that peer and doesn’t upload to it except as an optimistic
unchoke. This frequently results in more than
one concurrent optimistic unchoke, (an exception to
the exactly one optimistic unchoke rule mentioned
above), which causes download rates to recover much
more quickly when they falter.
-->











# BitTorrent Protocol

## Bencoding

Bencoding encodes data in a platform independent way. In BTP/1.0 the metainfo file and all responses from the tracker are encoded in the bencoding format. The format specifies two scalar types (integers and strings) and two compound types (lists and dictionaries).

Integers are encoded by prefixing a string containing the base ten representation of the integer with the letter "i" and postfixing it with the letter "e". E.g. the integer 123 is encoded as i123e.

Strings are encoded by prefixing the string content with the length of the string followed by a colon. E.g. the string "announce" is encoded as "8:announce".

The compound types provides a mean to structure elements of any bencoding type.

Lists are an arbitrary number of bencoded elements prefixed with the letter "l" and postfixed with the letter "e". It follows that lists can contain nested lists and dictionaries. For instance "li2e3:fooe" defines a list containing the integer "2" and the string "foo".

Dictionaries are an arbitrary number of key/value pairs delimited by the letter "d" at the beginning and the letter "e" at the end. All keys are bencoded strings while the associated value can be any bencoded element. E.g. "d5:monthi4e4:name5:aprile" defines a dictionary holding the associations: "month" => "4" and "name" => "april". All dictionary keys MUST be sorted.





## Terminology

Peer: A peer is a node in a network participating in file sharing. It can simultaneously act both as a server and a client to other nodes on the network.

Neighboring peers: Peers to which a client has an active point to point TCP connection.

Client: A client is a user agent (UA) that acts as a peer on behalf of a user.

Torrent: A torrent is the term for the file (single-file torrent) or group of files (multi-file torrent) the client is downloading.

Swarm: A network of peers that actively operate on a given torrent.

Seeder: A peer that has a complete copy of a torrent.

Tracker: A tracker is a centralized server that holds information about one or more torrents and associated swarms. It functions as a gateway for peers into a swarm.

Metainfo file: A text file that holds information about the torrent, e.g. the URL of the tracker. It usually has the extension .torrent.

Peer ID: A 20-byte string that identifies the peer. How the peer ID is obtained is outside the scope of this document, but a peer must make sure that the peer ID it uses has a very high probability of being unique in the swarm.

Info hash: A SHA1 hash that uniquely identifies the torrent. It is calculated from data in the metainfo file.


## Pieces and Blocks


              ---------------------------------------
              | Piece #0 | Piece #1 | .. | Piece #N |
              ---------------------------------------
                      _-´            `-_
                   _-´                  `-_
                 ----------------------------
                 | Block #0 | .. | Block #M |
                 ----------------------------






# The Peer Wire Protocol

The PWP facilitates communication between neighboring peers for the purpose of sharing file content.

PWP describes the steps taken by a peer after it has read in a metainfo file and contacted a tracker to gather information about other peers it may communicate with.

PWP is layered on top of TCP and handles all its communication using asynchronous messages.


## Handshaking

The local peer opens a port on which to listen for incoming connections from remote peers.

This port is then reported to the tracker.

As BTP/1.0 does not specify any standard port for listening it is the sole responsibility of the implementation to select a port.

Any remote peer wishing to communicate with the local peer must open a TCP connection to this port and perform a handshake operation.

The handshake operation MUST be carried out before any other data is sent from the remote peer.

The local peer MUST NOT send any data back to the remote peer before a well constructed handshake has been recognized according to the rules below.

If the handshake in any way violates these rules the local peer MUST close the connection with the remote peer.

A handshake is a string of bytes with the following structure:

The local peer knows its own protocol name and hence also the length of it. If this length is different than the value of this first byte, then the connection MUST be dropped.

Protocol Name: This is a character string which MUST contain the exact name of the protocol in ASCII and have the same length as given in the Name Length field. The protocol name is used to identify to the local peer which version of BTP the remote peer uses. In BTP/1.0 the name is 'BitTorrent protocol'.

If this string is different from the local peers own protocol name, then the connection is to be dropped.

Presumably, since both the local and the remote peer contacted the tracker as a result of reading in the same .torrent file, the local peer will recognize the info hash value and will be able to serve the remote peer. If this is not the case, then the connection MUST be dropped. This situation can arise if the local peer decides to no longer serve the file in question for some reason.

At this stage, if the connection has not been dropped, then the local peer MUST send its own handshake back, which includes the last step:

The local peer must use this name to identify the connection hereafter. Thus, if this name matches the local peers own ID name, the connection MUST be dropped. Also, if any other peer has already identified itself to the local peer using that same peer ID, the connection MUST be dropped.

In BTP/1.0 the handshake has a total of 68 bytes.


# Peer States

For each end of a connection, a peer must maintain the following two state flags:

Choked: When true, this flag means that the choked peer is not allowed to request data.

Interested: When true, this flag means a peer is interested in requesting data from another peer. This indicates that the peer will start requesting blocks if it is unchoked.

A choked peer MUST not send any data-oriented messages, but is free to send any other message to the peer that has choked it. If a peer chokes a remote peer, it MUST also discard any unanswered requests for blocks previously received from the remote peer.

An unchoked peer is allowed to send data-oriented messages to the remote peer. It is left to the implementation how many peers any given peer may choose to choke or unchoke, and in what fashion. This is done deliberately to allow peers to use different heuristics for peer selection.

An interested peer indicates to the remote peer that it must expect to receive data-oriented messages as soon as it unchokes the interested peer. It must be noted, that a peer must not assume a remote peer is interested solely because it has pieces that the remote peer is lacking. There may be valid reasons why a peer is not interested in another peer other than data-based ones.




## Peer Wire Messages

A PWP message has the following structure:

-----------------------------------------
| Message Length | Message ID | Payload |
-----------------------------------------

Message Length: This is an integer which denotes the length of the message, excluding the length part itself. If a message has no payload, its size is 1. Messages of size 0 MAY be sent periodically as keep-alive messages. Apart from the limit that the four bytes impose on the message length, BTP does not specify a maximum limit on this value. Thus an implementation MAY choose to specify a different limit, and for instance disconnect a remote peer that wishes to communicate using a message length that would put too much strain on the local peer's resources.

Message ID: This is a one byte value, indicating the type of the message. BTP/1.0 specifies 9 different messages, as can be seen further below.

Payload: The payload is a variable length stream of bytes.

If an incoming message in any way violates this structure then the connection SHOULD be dropped. In particular the receiver SHOULD make sure the message ID constitutes a valid message, and the payload matches the the expected payload, as given below.

For the purpose of compatibility with future protocol extensions the client SHOULD ignore unknown messages. There may arise situations in which a client may choose to drop a connection after receiving an unknown message, either for security reasons, or because discarding large unknown messages may be viewed as excessive waste.




# Piece Selection

PWP does not specify a standard algorithm for selecting elements from a clients neighboring peers with whom to share pieces, although the following guidelines are expected to be observed by any such algorithm:

The algorithm should not be constructed with the goal in mind to reduce the amount of data uploaded compared to downloaded. At the very least a peer should upload the same amount that it has downloaded.

The algorithm should not use a strict tit-for-tat schema when dealing with remote peers that have just joined the swarm and thus have no pieces to offer.

The algorithm should make good use of both download and upload bandwidth by putting a cap on the number of simultaneous connection that actively send or receive data. By reducing the number of active connections, TCP congestion can be avoided.

The algorithm should pipeline data requests in order so saturate active connections.

The algorithm should be able to cooperate with peers that implement a different algorithm.





