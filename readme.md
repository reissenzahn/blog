

# Plate

- BitTorrent
  - Protocol
- Consensus
  - Raft
- Gossip
  - Newscast
  - SWIM
- DHT
  - Chord


Manually set URL???

https://gohugo.io/content-management/organization/


# Horizon

- [Tapestry](https://sites.cs.ucsb.edu/~ravenben/publications/CSD-01-1141.pdf)
- [Pastry](https://www.microsoft.com/en-us/research/wp-content/uploads/2001/11/pastry.pdf)
- [Cubit](http://www.cs.cornell.edu/~bwong/Cubit/)




## Chord

- [Chord Wikipedia](https://en.wikipedia.org/wiki/Chord_(peer-to-peer))
- [Chord: Building a Distributed Hash Table in Golang](https://medium.com/techlog/chord-building-a-dht-distributed-hash-table-in-golang-67c3ce17417b)
- [Chord Implementation](https://github.com/arriqaaq/chord)
- [Chord: A Scalable Peer-to-peer Lookup Protocol for Internet Applications](https://pdos.csail.mit.edu/papers/ton:chord/paper-ton.pdf)
- [Distributed Hash Table using Node.js, gRPC, and the Chord Algorithm](https://www.youtube.com/watch?v=rhch2dZFcdM)
- [Distributed Hash Tables and Chord](http://web.mit.edu/6.829/www/currentsemester/materials/chord.pdf)



```html
<ul class="contents">
	<li>
		<ul>
				<li><a href="#introduction">Introduction</a></li>
				<li><a href="#resources">Resources</a></li>
		</ul>
	</li>
</ul>

<h3 id="introduction">Introduction</h3>




<h3 id="resources">Resources</h3>

<ul>
  <li>
    <a href=""></a>
  </li>
</ul>
```


# Raft


<h3 id="introduction">Replicated State machines</h3>

<p>
  Consensus algorithms arise in the context of replicated state machines. In this approach, state machines on a cluster of servers compute commands in lockstep for the purpose of fault tolerance. This is typically achieved through a consensus algorithm that maintains a replicated log of commands. The consensus algorithm manages adding commands to the logs and keeping them consistent. Once commands are properly replicated, each state machine processes them in log order. Since the state machines are deterministic, each computes the same state and the same sequence of outputs.
</p>



# BitTorrent

