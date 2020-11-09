
# Abstract

Data location can be easily implemented on top of Chord by associating a key with each data item and storing the key/data pair at the node to which the key maps.

Chord is scalable: communication cost and the state maintained by each node scale logarithmically with the number of Chord nodes.

Chord specifies how keys are assigned to nodes, and how a node can discover the value for a given key by first locating the node responsible for that key.

nted by storing a key-value pairs at the nodes to which their keys map. Chord adapts efficiently as nodes join and leave the network and can answer queries even if the network is continuously changing. Communication cost and state maintained by each node scales logarithmically with the number of nodes.

As such, a node will store the key-value pairs with keys it is responsible for. 


# Introduction

Chord is a scalable protocol for lookup in a dynamic peer-to-peer system with frequent node arrivals and departures.

The Chord protocol supports just one operation: given a key, it maps the key onto a node.

Depending on the application using Chord, that node might be responsible for storing a value associated with the key.

Chord uses consistent hashing to assign keys to Chord nodes.

Consistent hashing tends to balance load since each node receives roughly the same number of keys and requires relatively little movement of keys when nodes join and leave the system.

Previous work on consistent hashing assumes that each node is aware of most of the other nodes in the system, an approach that does not scale well to large numbers of nodes.

In contrast, each Chord node needs routing information about only a few other nodes.

Because the routing table is distributed, a Chord node communicates with other nodes in order to perform a lookup.

In the steady state, in an N-node system, each node maintains information about only O(log N) other nodes and resolves all lookups via O(log N) messages to other nodes.

Chord maintains its routing information as nodes join and leave the system.

A Chord node requires information about O(log N) other nodes for efficient routing, but performance degrades gracefully when that information is out of date.

This is important in practice because nodes will join and leave arbitrarily and consistency of even O(log N) state may be hard to maintain.

Only one piece of information per node need be correct in order for Chord to guarantee correct (though possibly slow) routing of queries; Chord has a simple algorithm for maintaining this information in a dynamic environment.



# Overview

With high probability the hash function balances load so that all nodes receive roughly the same number of keys.

Also with high probability, when an N-th node joins (or leaves) the network, only a O(1/N) fraction of the keys are moved to a different location.

Chord improves the scalability of consistent hashing by avoiding the requirement that every node know about every other node.

A Chord node needs only a small amount of routing information about other nodes.

Because this information is distributed, a node resolves the hash function by communicating with other nodes.

In an N-node network, each node maintains information about only O(log N) other nodes, and a lookup requires O(log N) messages.


# Consistent Hashing

Identifiers are ordered on an identifier circle modulo 2^m.

Key k is assigned to the first node whose identifer is equal to or follows the identifier of k in the identifier space.

This node is called the successor node of key k, denoted by successor(k).

If identifiers are represented as a circle of number from 0 to 2^m - 1, then successor(k) is the first node clockwise from k.

We refer to the identifier circle as the Chord ring.

Consistent hashing is designed to let nodes enter and leave the network with minimal disruption.

To maintain the consistent hashing mapping when a node n joins the network, certain keys previously assigned to n's successor now become assigned to n.

When node n leaves the network, all of its assigned keys are reassigned to n's successor.


# Scalable Key Location

This additional information is not essential for correctness, which is achieved as long as each node knows its correct successor.

This scheme has two important characteristics.

First, each node stores information about only a small number of other nodes, and knows more about nodes closely following it on the identifier circle than about nodes farther away.

Second, a node's finger table generally does not contain enough information to directly determine the successor of an arbitrary key k.

We extend the find_successor operation to use finger tables.

If id falls between n and its successor, find_successor is finished and node n returns its successor.

Otherwise, n searches its finger table for the node n' whose ID most immediately precedes id and then invokes find_successor at n'.

The reason behind this choice of n' is that the closer n' is to id the more it will know about the identifier circle in the region of id.

[figure 4b]

For example, in figure 4(b), suppose node 8 wants to find the successor of key 54.

Since the largest finger of node 8 that precedes 54 is node 42, node 8 will ask node 42 to resolve the query.

In turn node 42 will determine the largest finger in its finger table that precedes 54, i.e., node 51.

Finally, node 51 will discover that its own successor, node 56, succeeds key 54, and thus will return node 56 to node 8.

Since each node has finger entries at power of two intervals around the identifier circle, each node can forward a query at least halfway along the remaining distance between the node and the target identifier.

Indeed, with high probability, the number of nodes that must be contacted to find a successor in an N-node network is O(log N).

Although the finger table contains room for m entries, only O(log N) fingers need be stored.

This is because no node is likely to be within distance 2^m/N^2 of any other node.

Thus the i-th finger of the node, for any i \le m - 2 log N will be equal to the node's immediate successor with high probability and need not be stored separately.


# Node Joins and Stabilization

In practice, Chord needs to deal with nodes joining the system and with nodes that fail or leave voluntarily.

In order to ensure that lookups execute correctly as the set of participating nodes changes, Chord must ensure that each node's successor pointer is up to date.

It does this using a stabilization protocol that each node runs periodically in the background and which updates Chord's finger tables and successor pointers.

```
// create a node Chord ring
n.create()
  predecessor = nil
  successor = n

// join a Chord ring containing node n'
n.join(n')
  predecessor = nil
  successor = n'.find_successor(n)
```

When node n first starts, it calls n.join(n') where n' is any known Chord node, or n.create() to create a new Chord network.

The join() functions asks n' to find the immediate successor of n.

By itself, join() does not make the rest of the network aware of n.

```
// verifies n's immediate successor and tells the successor about n
n.stabilize()
  x = successor.predecessor
  if (x ∈ (n, successor))
    successor = x
  successor.notify(n)

// n' thinks it might be our predecessor
n.notify(n')
  if (predecessor is nil or n' ∈ (predecessor, n))
    predecessor = n'
```

Every node runs stabilize() periodically to learn about newly joined nodes.

Each time node n runs stabilize(), it asks its successor for the successor's predecessor p and decides whether p should be n's successor instead.

This would be the case if node p recently joined the system.

In addition, stabilize() notifies node n's successor of n's existence, giving the successor the chance to change its predecessor to n.

The successor does this only if it knows of no closer predecessor than n.

```
// refreshes finger table entries
n.fix_fingers()
  next = next + 1
  if (next > m)
    next = 1
  finger[next] = find_successor(n + 2^(next - 1))
```

Each node periodically calls fix fingers to make sure its finger table entries are correct; this is how new nodes initialize their finger tables, and it is how existing nodes incorporate new nodes into their finger tables.

```
// checks whether predecessor has failed
n.check_predecessor()
  if (predecessor has failed)
    predecessor = nil
```

Each node also runs check_predecessor periodically, to clear the node's predecessor pointer if n.predecessor has failed; this allows it to accept a new predecessor in notify.

As a simple example, suppose node n joins the system and its ID lies between nodes n_p and n_s.

In its call to join(), n acquires n_s as its successor.

Node n_s, when notified by n, acquires n as its predecessor.

When n_p next runs stabilize(), it asks n_s for its predecessor (which is now n); n_p then acquires n as its successor.

Finally, n_p notifies n, and n acquires n_p as its predecessor.

At this point all predecessor and successor pointers are correct.

At each step in the process, n_s is reachable from n_p using successor pointers; this means that lookups concurrent with the join are not disrupted.

[figure 7]

Figure 7 illustrates the join procedure, when n's ID is 26 and the IDs of n_s and n_p are 21 and 32 respectively.

As soon as the successor pointers are correct, calls to find_successor() will reflect the new node.

Newly-joined nodes that are not yet reflected in other nodes’ finger tables may cause find_successor() to initially undershoot, but the loop in the lookup algorithm will nevertheless follow successor (finger[1]) pointers through the newly-joined nodes until the correct predecessor is reached.

Eventually fix fingers() will adjust finger table entries, eliminating the need for these linear scans.

In other words, after some time each node is able to reach any other node in the network by following successor pointers.

Our stabilization scheme guarantees to add nodes to a Chord ring in a way that preserves reachability of existing nodes, even in the face of concurrent joins and lost and reordered messages.


# Impact of Node Joins on Lookups

We consider the impact of node joins on lookups.

We first consider correctness. If joining nodes affect some region of the Chord ring, a lookup that occurs before stabilization has finished can exhibit one of three behaviors.

The common case is that all the finger table entries involved in the lookup are reasonably current and the lookup finds the correct successor in O(log N) steps.

The second case is where successor pointers are correct but fingers are inaccurate. This yields correct lookups, but they may be slower.

In the final case, the nodes in the affected region have incorrect successor pointers or keys may not yet have migrated to newly joined nodes and the lookup may fail.

The higher-layer software using Chord will notice that the desired data was not found and has the option of retrying the lookup after a short pause.

Now let us consider performance.

Once stabilization has completed, the new nodes will have no effect beyond increasing the N in the O(log N) lookup time.

If stabilization has not yet completed, existing nodes’ finger table entries may not reflect the new nodes.

The ability of finger entries to carry queries long distances around the identifier ring does not depend on exactly which nodes the entries point to; the distance halving argument depends only on ID-space distance.

Thus the fact that finger table entries may not reflect new nodes does not significantly affect lookup speed.

The main way in which newly joined nodes can influence lookup speed is if the new nodes’ IDs are between the target’s predecessor and the target.

In that case the lookup will have to be forwarded through the intervening nodes, one at a time.

But unless a tremendous number of nodes joins the system, the number of nodes between two old nodes is likely to be very small, so the impact on lookup is negligible.

We call a Chord ring stable if all its successor and finger pointers are correct.

If we take a stable network with N nodes with correct finger pointers, and another set of up toN nodes joins the network, and all successor pointers (but perhaps not all finger pointers) are correct, then lookups will still take O(log N) time with high probability.

More generally, as long as the time it takes to adjust fingers is less than the time it takes the network to double in size, lookups will continue to take O(log N) hops.

We can achieve such adjustment by repeatedly carrying out lookups to update our fingers.


# Failure and Replication

The correctness of the Chord protocol relies on the fact that each node knows its successor.

However, this invariant can be compromised if nodes fail.

For example, in figure 4, if nodes 14, 21, and 32 fail simultaneously, node 8 will not know that node 38 is now its successor, since it has no finger pointing to 38.

An incorrect successor will lead to incorrect lookups. Consider a query for key 30 initiated by node 8. Node 8 will return node 42, the first node it knows about from its finger table, instead of the correct successor, node 38.

To increase robustness, each Chord node maintains a successor list of size r, containing the node’s first r successors.

If a node’s immediate successor does not respond, the node can substitute the second entry in its successor list.

All r successors would have to simultaneously fail in order to disrupt the Chord ring, an event that can be made very improbable with modest values of r.

Assuming each node fails independently with probability p, the probability that all r successors fail simultaneously is only p^r. Increasing r makes the system more robust.

Handling the successor list requires minor changes in the
pseudo-code.

A modified version of the stabilize procedure maintains the successor list.

Successor lists are stabilized as follows: node n reconciles its list with its successor s by copying s’s successor list, removing its last entry, and prepending s to it.

If node n notices that its successor has failed, it replaces it with the first live entry in its successor list and reconciles its successor list with its new successor.

At that point, n can direct ordinary lookups for keys for which the
failed node was the successor to the new successor.

As time passes, fix_fingers and stabilize will correct finger table entries and successor list entries pointing to the failed node.

A modified version of the closest_preceding_node procedure
in searches not only the finger table but also the successor list for the most immediate predecessor of id.

In addition, the pseudo-code needs to be enhanced to handle node failures.

If a node fails during the find_successor procedure, the lookup proceeds, after a timeout, by trying the next best predecessor among the nodes in the finger table and the successor list.

Neither the success nor the performance of Chord lookups is likely to be affected even by massive simultaneous failures.

If we use a successor list of length r = \Omega (log N) in a network that is initially stable, and then every node fails with probability 1/2, then with high probability find successor returns the closest living successor to the query key.

In a network that is initially stable, if every node then fails with probability 1/2, then the expected time to execute find_successor is O(log N).

The successor list mechanism also helps higher-layer software replicate data.

A typical application using Chord might store replicas of the data associated with a key at the k nodes succeeding the key.

The fact that a Chord node keeps track of its r successors means that it can inform the higher layer software when successors come and go, and thus when the software should propagate data to new replicas.


# Voluntary Node Departures

Since Chord is robust in the face of failures, a node voluntarily leaving the system could be treated as a node failure.

However, two enhancements can improve Chord performance when nodes leave voluntarily.

First, a node n that is about to leave may transfer its keys to its successor before it departs.

Second, n may notify its predecessor p and successor s before leaving.

In turn, node p will remove n from its successor list, and add the last node in n’s successor list to its own list.

Similarly, node s will replace its predecessor with n’s predecessor.

Here we assume that n sends its predecessor to s, and the last node in its successor list to p.


# More Realistic Analysis

Our analysis above gives some insight into the behavior of the Chord system, but is inadequate in practice.

The theorems proven above assume that the Chord ring starts in a stable state and then experiences joins or failures.

In practice, a Chord ring will never be in a stable state; instead, joins and departures will occur continuously, interleaved with the stabilization algorithm.

The ring will not have time to stabilize before new changes happen.

The Chord algorithms can be analyzed in this more general setting.

Other work shows that if the stabilization protocol is run at a certain rate (dependent on the rate at which nodes join and fail) then the Chord ring remains continuously in an “almost stable” state in which lookups are fast and correct.


# Conclusion

In the steady state, in an N-node network, each node maintains routing information for only O(log N) other nodes, and resolves all lookups via O(log N) messages to other nodes.

It continues to function correctly, albeit at degraded performance, when a node’s information is only partially correct.

Chord scales well with the number of nodes, recovers from large numbers of simultaneous node failures and joins, and answers most lookups correctly even during recovery.

