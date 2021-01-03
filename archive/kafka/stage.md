


# Replication

<!-- 
- If the leader becomes unavailable, one of the in-sync replicas is elected as the new leader.
- Unclean elections can be enabled to allow out-of-sync replicas to become leaders as a last resort. -->


In the event that a leader replica for a partition crashes, one of the follower replicas will be promoted to become the new leader for the partition.

Another task the leader is responsible for is knowing which of the follower replicas is up-to-date with the leader.

Followers attempt to stay up-to-date by replicating all the messages from the leader as the messages arrive, but they can fail to stay in sync for various reasons, such as when network congestion slows down replication or when a broker crashes and all replicas on that broker start falling behind until we start the broker and they can start replicating again.



<!-- 
In response to those requests, the leader sends the messages to the replicas.

Those
Fetch requests contain the offset of the message that the replica wants to receive next,
and will always be in order.

A replica will request message 1, then message 2, and then message 3, and it will not
request message 4 before it gets all the previous messages. This means that the leader
can know that a replica got all messages up to message 3 when the replica requests
message 4. By looking at the last offset requested by each replica, the leader can tell
how far behind each replica is. If a replica hasn’t requested a message in more than 10
seconds or if it has requested messages but hasn’t caught up to the most recent message
in more than 10 seconds, the replica is considered out of sync. If a replica fails to
keep up with the leader, it can no longer become the new leader in the event of failure
—after all, it does not contain all the messages.
The inverse of this, replicas that are consistently asking for the latest messages, is
called in-sync replicas. Only in-sync replicas are eligible to be elected as partition leaders
in case the existing leader fails.
The amount of time a follower can be inactive or behind before it is considered out of
sync is controlled by the replica.lag.time.max.ms configuration parameter. This
allowed lag has implications on client behavior and data retention during leader election.
We will discuss this in depth in Chapter 6, when we discuss reliability guarantees.
In addition to the current leader, each partition has a preferred leader—the replica
that was the leader when the topic was originally created. It is preferred because when
partitions are first created, the leaders are balanced between brokers (we explain the
algorithm for distributing replicas and leaders among brokers later in the chapter).
As a result, we expect that when the preferred leader is indeed the leader for all partitions
in the cluster, load will be evenly balanced between brokers. By default, Kafka is
configured with auto.leader.rebalance.enable=true, which will check if the preferred
leader replica is not the current leader but is in-sync and trigger leader election
to make the preferred leader the current leader.

The best way to identify the current preferred leader is by looking
at the list of replicas for a partition (You can see details of partitions
and replicas in the output of the kafka-topics.sh tool. We’ll discuss
this and other admin tools in Chapter 10.) The first replica in
the list is always the preferred leader. This is true no matter who is
the current leader and even if the replicas were reassigned to different
brokers using the replica reassignment tool. In fact, if you manually
reassign replicas, it is important to remember that the replica
you specify first will be the preferred replica, so make sure you
spread those around different brokers to avoid overloading some
brokers with leaders while other brokers are not handling their fair
share of the work.
-->






# Partition Allocation



<!-- 
To do this, we start with a random broker and start assigning partitions to each broker in round-robin manner to determine the location for the leaders.

Then, for each partition, we place the replicas at increasing offsets from the leader. If the leader for partition
0 is on broker 4, the first follower will be on broker 5 and the second on broker
0. The leader for partition 1 is on broker 5, so the first replica is on broker 0 and the
second on broker 1.
When rack awareness is taken into account, instead of picking brokers in numerical
order, we prepare a rack-alternating broker list. Suppose that we know that brokers 0,
1, and 2 are on the same rack, and brokers 3, 4, and 5 are on a separate rack. Instead
of picking brokers in the order of 0 to 5, we order them as 0, 3, 1, 4, 2, 5—each broker
is followed by a broker from a different rack (Figure 5-5). In this case, if the leader for
partition 0 is on broker 4, the first replica will be on broker 2, which is on a completely
different rack. This is great, because if the first rack goes offline, we know that
we still have a surviving replica and therefore the partition is still available. This will
be true for all our replicas, so we have guaranteed availability in the case of rack failure.
Once we choose the correct brokers for each partition and replica, it is time to decide
which directory to use for the new partitions. We do this independently for each partition,
and the rule is very simple: we count the number of partitions on each directory and add the new partition to the directory with the fewest partitions. This means that if you add a new disk, all the new partitions will be created on that disk. This is because, until things balance out, the new disk will always have the fewest partitions.
-->


































# ZooKeeper

<!-- 
A Zookeeper cluster is called an ensemble. Due to the algorithm used, it is recommended
that ensembles contain an odd number of servers (e.g., 3, 5, etc.) as a majority
of ensemble members (a quorum) must be working in order for Zookeeper to
respond to requests. This means that in a three-node ensemble, you can run with one
node missing. With a five-node ensemble, you can run with two nodes missing.

Consider running Zookeeper in a five-node ensemble. In order to
make configuration changes to the ensemble, including swapping a
node, you will need to reload nodes one at a time. If your ensemble
cannot tolerate more than one node being down, doing maintenance
work introduces additional risk. It is also not recommended
to run more than seven nodes, as performance can start to degrade
due to the nature of the consensus protocol.

To configure Zookeeper servers in an ensemble, they must have a common configuration
that lists all servers, and each server needs a myid file in the data directory that
specifies the ID number of the server. If the hostnames of the servers in the ensemble
are zoo1.example.com, zoo2.example.com, and zoo3.example.com, the configuration
file might look like this:

tickTime=2000
dataDir=/var/lib/zookeeper
clientPort=2181
initLimit=20
syncLimit=5
server.1=zoo1.example.com:2888:3888
server.2=zoo2.example.com:2888:3888
server.3=zoo3.example.com:2888:3888

In this configuration, the initLimit is the amount of time to allow followers to connect
with a leader. The syncLimit value limits how out-of-sync followers can be with
the leader. Both values are a number of tickTime units, which makes the initLimit
20 * 2000 ms, or 40 seconds. The configuration also lists each server in the ensemble.
The servers are specified in the format server.X=hostname:peerPort:leaderPort, with
the following parameters:
X
The ID number of the server. This must be an integer, but it does not need to be
zero-based or sequential.
hostname
The hostname or IP address of the server.
peerPort
The TCP port over which servers in the ensemble communicate with each other.
leaderPort
The TCP port over which leader election is performed.
Clients only need to be able to connect to the ensemble over the clientPort, but the
members of the ensemble must be able to communicate with each other over all three
ports.
In addition to the shared configuration file, each server must have a file in the data
Dir directory with the name myid. This file must contain the ID number of the server,
which must match the configuration file. Once these steps are complete, the servers
will start up and communicate with each other in an ensemble.
-->





# Other

## Java Stuff

A producer object can be used by multiple threads to send messages. You will probably want to start with one producer and one thread. If you need better throughput, you can add more threads that use the same producer. Once this ceases to increase throughput, you can add more producers to the application to achieve even higher throughput.


## Segments

Logs are rolled into segments which are retained for a certain amount of time or until a storage threshold is met.


## Seeking

The offset is an integer value that continually increases.

Logs are not indexed so you can only seek by offset.


## Retries in Flight

Setting the retries parameter to nonzero and the max.in.flights.requests.per.session to more than one means that it is possible that the broker will fail to write the first batch of messages, succeed to write the second (which was already inflight), and then retry the first batch and succeed, thereby reversing the order.

Usually, setting the number of retries to zero is not an option in a
reliable system, so if guaranteeing order is critical, we recommend
setting in.flight.requests.per.session=1 to make sure that
while a batch of messages is retrying, additional messages will not
be sent (because this has the potential to reverse the correct order).
This will severely limit the throughput of the producer, so only use
this when order is important.


## ZooKeeper Membership Details

Every time a broker process starts, it registers itself with its ID in Zookeeper by creating an ephemeral node.

Different Kafka components subscribe to the /brokers/ids path in Zookeeper where brokers are registered so they get notified when brokers are added or removed.

When a broker loses connectivity to Zookeeper (usually as a result of the broker stopping, but this can also happen as a result of network partition or a long garbage collection pause), the ephemeral node that the broker created when starting will be automatically removed from Zookeeper.

Kafka components that are watching the list of brokers will be notified that the broker is gone.

Even though the node representing the broker is gone when the broker is stopped, the broker ID still exists in other data structures.

For example, the list of replicas of each topic contains the broker IDs for the replica. This way, if you completely lose a broker and start a brand new broker with the ID of the old one, it will immediately join the cluster in place of the missing broker with the same partitions and topics assigned to it.

