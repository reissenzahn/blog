# Draft

# Tracker HTTP Protocol

## Request Parameters

- `left`: The number of bytes this client still has to download in base ten ASCII.
- `compact`: Setting this to 1 indicates that the client accepts a compact response. The peers list is replaced by a peers string with 6 bytes per peer. The first four bytes are the host and the last two bytes are the port.
- `no_peer_id`: Indicates that the tracker can omit peer id field in peers dictionary. This option is ignored if compact is enabled.
- `event`: If specified, must be one of started, completed, stopped, (or empty which is the same as not being specified). If not specified, then this request is one performed at regular intervals.
- `started`: The first request to the tracker must include the event key with this value.
- `stopped`: Must be sent to the tracker if the client is shutting down gracefully.
- `completed`: Must be sent to the tracker when the download completes. However, must not be sent if the download was already 100% complete when the client started. Presumably, this is to allow the tracker to increment the "completed downloads" metric based solely on this event.
- `ip`: Optional. The true IP address of the client machine, in dotted quad format or hexed IPv6 address. This parameter is is only needed in the case where the IP address that the request came in on is not the IP address of the client. It also is necessary when both the client and the tracker are on the same local side of a NAT gateway.
- `numwant`: Optional. Number of peers that the client would like to receive from the tracker. This value is permitted to be zero. If omitted, typically defaults to 50 peers.
- `key`: Optional. An additional identification that is not shared with any other peers. It is intended to allow a client to prove their identity should their IP address change.
- `trackerid`: Optional. If a previous announce contained a tracker id, it should be set here.





# Tracker UDP Protocol

Using HTTP to discover peers introduces significant overhead. This is because HTTP is built on top of another protocol called TCP which guarantees that sent data will be received in its entirety and in the correct order. To achieve these guarantees, a TCP connection must establish a persistent connection before sending data which introduces substantial overhead for trackers serving large numbers of peers.

To address this, a UDP based protocol can be used to reduce traffic. UDP is unreliable protocol in the sense that packets are sent to recipients without waiting to make sure the recipient received the packet. Seeing that 

An additional advantage is that a UDP based binary protocol doesn't require a complex parser and no connection handling, reducing the complexity of tracker code and increasing it's performance.

UDP is an 'unreliable' protocol. This means it doesn't retransmit lost packets itself. The application is responsible for this. If a response is not received after 15 * 2 ^ n seconds, the client should retransmit the request, where n starts at 0 and is increased up to 8 (3840 seconds) after every retransmission. Note that it is necessary to rerequest a connection ID when it has expired.

All values are send in network byte order (big endian). Do not expect packets to be exactly of a certain size. Future extensions could increase the size of packets.


## Connect

As it is possible to spoof the source address of a UDP packet, the tracker must issue the client with a connection ID which will be used to verify future requests.

A connection ID can be used for multiple requests. A client can use a connection ID until one minute after it has received it. Trackers should accept the connection ID until two minutes after it has been send.

Our first step is to obtain a connection ID from the tracker by sending a connect request. This consists of the magic constant `0x41727101980`, a value of `0` to indicate our desire to connect and a random transaction ID.

```
Offset  Size            Name            Value
0       64-bit integer  protocol_id     0x41727101980
8       32-bit integer  action          0
12      32-bit integer  transaction_id  ???
16
```

Our response includes the connection ID that we will extract and use in subsequent announce requests to tell the tracker which files we're interested in.

```
Offset  Size            Name            Value
0       32-bit integer  action          0
4       32-bit integer  transaction_id  ???
8       64-bit integer  connection_id   ???
16
```


## Announce

We provide the connection ID obtained from the connect request, an action of `1` to indicate we are sending an announce request and a random transaction id.

Finally, BitTorrent ports should be between 6881 and 6889.

```
Offset  Size    Name    Value
0       64-bit integer  connection_id
8       32-bit integer  action          1
12      32-bit integer  transaction_id
16      20-byte string  info_hash
36      20-byte string  peer_id
56      64-bit integer  downloaded
64      64-bit integer  left
72      64-bit integer  uploaded
80      32-bit integer  event           0
84      32-bit integer  IP address      0
88      32-bit integer  key
92      32-bit integer  num_want        -1
96      16-bit integer  port
98
```

Choose a random transaction ID.
Fill the announce request structure.
Send the packet.

It’s a bit tricky because the number of addresses that come back isn’t fixed. The addresses come in groups of 6 bytes, the first 4 represent the IP address and the next 2 represent the port. So our code will need to correctly break up the addresses part of the response.



```
Offset      Size            Name            Value
0           32-bit integer  action          1
4           32-bit integer  transaction_id
8           32-bit integer  interval
12          32-bit integer  leechers
16          32-bit integer  seeders
20 + 6 * n  32-bit integer  IP address
24 + 6 * n  16-bit integer  TCP port
20 + 6 * N
```

Receive the packet.
Check whether the packet is at least 20 bytes.
Check whether the transaction ID is equal to the one you chose.
Check whether the action is announce.
Do not announce again until interval seconds have passed or an event has occurred.






# DHT

The BitTorrent protocol provides no way to index torrent files. As a result, a comparatively small number of websites have hosted a large majority of torrents. 

port: <len=0003><id=9><listen-port>

The port message is sent by newer versions of the Mainline that implements a DHT tracker. The listen port is the port this peer's DHT node is listening on. This peer should be inserted in the local routing table (if DHT tracker is supported).


# Glossary



- _Availability_: The number of full copies of a file (or files) directly available to the client. Each seed adds 1 to this number. A connected peer with a fraction of the file available adds that fraction to the availability if no other peer has this part of the file. For instance, a peer with 65.3% of the file downloaded increases the availability by 0.653. However, if two peers both have the same portion of the file downloaded - say 50% - and there is only one seeder, the availability is 1.5.

- _Downloader_: A downloader is any peer that is currently downloading a particular file (or files) but does not yet have a complete copy of the file.

- _Leecher_: A leecher is any peer that is currently downloading a particular file (or files) but does not yet have a complete copy of the file. Frequently used to refer to a peer that has a negative influence on the swarm by downloading much more than they upload.

- _Peer_: A peer is one instance of a BitTorrent client running on a computer on the Internet to which other clients connect and transfer data. 

- _Piece_: Files shared using the BitTorrent protocol are divided into equal sized pieces.

- _Seed_: A seed refers to a machine possessing all of the data. A peer or downloader becomes a seed when it completely downloads all the data nad continues/starts uploading data for other peers to download from. This includes any peer possessing 10




- `block`: When a file is distributed via BitTorrent it is broken into smaller pieces called blocks. Breaking the file into pieces allows it to be distributed as efficiently as possible.

- `client`: A BitTorrent client is a program used to upload and download files using the BitTorrent protocol.
- `leecher`: A peer that is downloading while uploading very little or nothing at all. Also used to refer to a peer that is not seeding yet.
- `peer`: One of a group of clients that are downloading the same file.
- `re-seed`: 
- `seed`: A complete copy of the file being made available for download.
- `seeder`: A peer that is done downloading a file and is now just making it available to others.
- `swarm`: A group of seeds and peers sharing the same torrent.
- `torrent file`: A file which describes what file or files are being distributed, where to find parts and other info needed for the distribution of the file.
- `scrape`: This is when a client sends a request to the tracker for information about the statistics of the torrent, like who to share the file with and how well those other users are sharing.




# Appendix

Interval tells us how often we’re supposed to connect to the tracker again to refresh our list of peers. A value of 900 means we should reconnect every 15 minutes (900 seconds).






block
A block is a piece of a file. When a file is distributed via BitTorrent, it is broken into smaller
pieces, or blocks. Typically the block is 250kb in size, but it can vary with the size of the
file being distributed. Breaking the file into pieces allows it to be distributed as efficiently
as possible. Users get their files faster using less bandwidth.
client
the BitTorrent software used to download and upload files. The BitTorrent client can be
downloaded here.
leech or leecher
usually refers to a peer that is downloading while uploading very little, or nothing at all.
Sometimes this is unintentional and due to firewall issues. The term leech is also
sometimes used to simply refer to a peer that is not seeding yet.
peer
one of a group of clients downloading the same file.
re-seed
Re-seeding is the act of putting up a new complete copy of a file after no more seeds are
available to download from. This is done to allow clients with only partial downloads to
complete the download process and increases availability


scrape
This is when a client sends a request to the tracker for information about the statistics of
the torrent, like who to share the file with and how well those other users are sharing.
seed
a complete copy of the file being made available for download.
seeder/seeding
a peer that is done downloading a file and is now just making it available to others.
swarm
a group of seeds and peers sharing the same torrent.
torrent
generally, the instance of a file or group of files being distributed via BitTorrent.
torrent file
a file which describes what file or files are being distributed, where to find parts, and other
info needed for the distribution of the file.
tracker
a server that keeps track of the peers and seeds in a swarm. A tracker does not have a
copy of the file itself, but it helps manage the file transfer process.



