
Kademlia is a structured peer-to-peer system.


# XOR

Kademlia has several advantages to protocols like Chord as a result of its novel usage of the XOR metric for distance between points in identifier space.

Because XOR is a symmetric operation, Kademlia nodes receive lookup queries from the same nodes which are also in their local routing tables. This is important in order that nodes can learn useful routing information from lookup queries and update their routing tables.

The XOR metric is also unidirectional: for any constant x there exists exactly one y which has a distance of d(x,y). This ensures that lookups for the same key converge along the same path independently of the origination which is an important property for caching mechanisms.


# IDs

In Kademlia every node chooses a random 160-bit nodeId.


# k-Buckets

In Kademlia every node maintains a routing table consisting of up to 160 k-buckets.

Every k-bucket contains at most k entries with (IP Address, UDP port, NodeID) triples of other nodes.

The parameter k is a redundancy factor to make the routing more robust by spanning several disjoint paths between overlay nodes.

Buckets are arranged as a binary tree and nodes get assigned to buckets according to the shortest unique prefix of their nodeIds.

An example routing table of a node with a nodeId prefix 0011 is shown in figure X.

Initially, a node's routing table consists of a single bucket covering the entire ID space.

When a node U learns of a new contact C, the node U inserts C in the appropriate bucket according to the prefix of C's nodeId. 

If this bucket already contains k nodes and the bucket's range includes U's o n nodeId, the bucket is split into two new buckets. Otherwise the new contact is simply dropped.


# Unequal Distribution Problem

A problem arises if nodeIds are unequally distributed. In this case the standard bucket splitting algorithm can lead to nodes not knowing their complete k neighborhood.

An example for this is given in figure 2. This figure shows the routing table of a node U with nodeId prefix of 00.

According to the splitting rule given above, there would be only one bucket for prefix 01 with k entries, which doesn’t get split any further. In this case the node with prefix 010, which is the closest node to node U could get dropped, leading to an incomplete neighborhood.

To avoid this, the authors of Kademlia propose in these cases to also split buckets in which node U’s nodeId isn’t contained, resulting in an irregular subtree next to the bucket of U’s nodeId.








