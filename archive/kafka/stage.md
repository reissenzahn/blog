



# Rebalancing

A rebalance will happen in a number of scenarios:

A new member joins the consumer group
A member leaves the consumer group (for example when shutting down)
A member is considered dead by the group coordinator. This happens when there have been no heartbeats sent within the configured session timeout. This would indicate that the consumer has crashed or is busy with some long-running processing, such as for example if the execution of eachMessage takes longer than the session timeout.
Partitions have been added or removed from the topic



# Terminology


Partition	Data in a topic is spread across a number of partitions. Each partition can be thought of as a log file, ordered by time. To guarantee that you read messages in the correct order, only one instance can read from a particular partition at a time.
Producer	A client that writes data to one or more Kafka topics
Consumer	A client that reads data from one or more Kafka topics
Replica	Partitions are typically replicated to one or more brokers to avoid data loss.
Leader	Although a partition may be replicated to one or more brokers, a single broker is elected the leader for that partition, and is the only one who is allowed to write or read to/from that partition
Consumer group	A collective group of consumer instances, identified by a groupId. In a horizontally scaled application, each instance would be a consumer and together they would act as a consumer group.
Group Coordinator	An instance in the consumer group that is responsible for assigning partitions to consume from to the consumers in the group
Offset	A certain point in the partition log. When a consumer has consumed a message, it "commits" that offset, meaning that it tells the broker that the consumer group has consumed that message. If the consumer group is restarted, it will restart from the highest committed offset.
Rebalance	When a consumer has joined or left a consumer group (such as during booting or shutdown), the group has to "rebalance", meaning that a group coordinator has to be chosen and partitions need to be assigned to the members of the consumer group.
Heartbeat	The mechanism by which the cluster knows which consumers are alive. Every now and then (heartbeatInterval), each consumer has to send a heartbeat request to the cluster leader. If one fails to do so for a certain period (sessionTimeout), it is considered dead and will be removed from the consumer group, triggering a rebalance.









# Partition Allocation

When a topic is created, Kafka decides how to allocate the partitions between brokers.

When doing the allocations, the objective is to spread replicas evenly among broker and to make sure that each replica for a given partition is on a different broker.








# Publishing Messages

There are three primary methods of sending messages:

- Fire-and-forget: Messages are sent and we do not wait to receive a reply. This allows for very high message throughput at the cost of the possibility of silently dropping messages.

- Synchronous send: Messages are sent and the producer waits for a reply from Kafka. If the message failed to be sent the and the number of retries has been exhausted or a non-retriable error occurs then an exception will be thrown.

- Asynchronous send: Messages are sent asynchronously and a callback function is triggered when the producer receives a response from the broker. 





When a broker receives messages, it sends back a response.

If the messages were successfully written to Kafka, it will return the topic, partition, and the offset of the record within the partition.

Otherwise, if the broker failed to write the messages, it will return an error.


Retriable errors are those that can be resolved by sending the message again. For example, a connection error can be resolved
because the connection may get reestablished. A “no leader” error can be resolved
when a new leader is elected for the partition. KafkaProducer can be configured to
retry those errors automatically, so the application code will get retriable exceptions
only when the number of retries was exhausted and the error was not resolved. Some
errors will not be resolved by retrying. For example, “message size too large.” In those
cases, KafkaProducer will not attempt a retry and will return the exception immediately.



# Request Processing

Kafka brokers process requests sent to the partition leaders from clients, partition replicas and the controller.

The most common types of requests are:

- Producer requests: Sent by producers and contain messages the client write to Kafka brokers.

- Fetch requests: Sent by consumers and follower replicas when they read messages from Kafka brokers.


Both produce requests and fetch requests have to be sent to the leader replica of a partition.

If a broker receives a produce request for a specific partition and the leader for this partition is on a different broker, the client that sent the produce request will get an error response of “Not a Leader for Partition.”

The same error will occur if a fetch request for a specific partition arrives at a broker that does not have the leader
for that partition.

Kafka’s clients are responsible for sending produce and fetch requests to the broker that contains the leader for the relevant partition for the request.

How do the clients know where to send the requests? Kafka clients use another request type called a metadata request, which includes a list of topics the client is interested in.

The server response specifies which partitions exist in the topics, the replicas for each partition, and which replica is the leader.

Metadata requests can be sent to any broker because all brokers have a metadata cache that contains this information.

Clients typically cache this information and use it to direct produce and fetch requests to the correct broker for each partition.

They also need to occasionally refresh this information by sending another metadata request so they know if the topic metadata changed—for example, if a new broker was added or some replicas were moved to a new broker.

In addition, if a client receives the “Not a Leader” error to one of its requests, it will refresh its metadata before trying to send the request again, since the error indicates that the client is using outdated information and is sending requests to the wrong broker.



























# Done

## Consumers

<!-- 
A consumer is an application that reads and processes messages from Kafka.

A consumer subscribes to one or more topics and reads the messages in the order in which they were produced.

Kafka acts as a buffer allowing producers to write records faster than consumers can read them.
-->

## Producers

<!-- 
A producer is an application that creates and publishes messages.

Each message will be produced to a specific topic.

A producer has a partitioner which determines which partition of a topic a given message will be written to.

By default, messages with the same key will be sent to the same partition provided the number of partitions does not change.

This is done by calculating the partition number as the consistent hash of the key modulus the total number of partitions in the topic.

Messages with null keys will be evenly balanced writes over all partitions of a topic.

The producer could also use a custom partitioner that follows other business rules for mapping messages to partitions.

Alternatively, the producer can choose precisely which partition to send a given message to.
-->


## Resizing Topics

<!-- 
It is sometimes necessary to increase the number of partitions for a topic.

The most common reason to increase the partition count is to spread out a topic further, or decrease the throughput for a single partition.

Topics that are produced with keyed messages can be very difficult
to add partitions to.

This is because the mapping of keys to partitions will change when the number of partitions is changed.

For this reason, it is advisable to avoid resizing topics that contain keyed messages.

It is not possible to reduce the number of partitions for a topic.

The reason this is not supported is because deleting a partition from a topic would cause part of the data in that topic to be deleted as well, which would be inconsistent from a client point of view.

In addition, trying to redistribute the data to remaining partitions would be difficult and result in out-of-order messages. 

Should you need to reduce the number of partitions, you will need to delete the topic and recreate it.
-->



## Introduction

<!-- 
Kafka is an event streaming platform used to collect, store and process real time data streams at scale.

It carries messages between the various members of the infrastructure, providing a consistent interface for all clients. 

Fundamentally, applications called producers publish messages to categories called topics and applications called consumers read messages from these topics.

Messages are stored durably in ordered logs.

Producers and consumers are decoupled so slow or failing consumers do not effect producers.

Components can be added and removed as business cases are created and dissolved, and producers do not need to be concerned about who is using the data or the number of consuming applications.

Kafka replicates and distributes data among a cluster of Kafka brokers to provide redundancy and scalability.
-->




## Messages and Batches

<!-- 
The unit of data in Kafka is called a message.

Each message consists of a value, a timestamp and an optional key.

As far as Kafka is concerned, both values and keys are opaque byte arrays.

Messages are immutable and are persisted to disk in append-only ordered sequences called logs.


Message produced to the same partition of a topic are written in collections called batches for efficiency.

This is a tradeoff between latency and throughput: the larger the batches, the more messages that can be handled per unit of time, but the longer it takes an individual message to propagate.

Batches are also typically compressed, providing more efficient data transfer and storage at the cost of some processing power. -->


## Topics

<!-- 
Messages in Kafka are categorized into named collections called topics.

Producers publish messages to particular topics and consumers subscribe to topics to read messages from them.
-->


## Partitions

<!-- 
Topics are divided into a number of partitions.

Each partition consists of an immutable log.

Messages are written to a partition in an append-only fashion and are read in order from beginning to end.

Each message in a given partition is assigned a unique sequential integer identifier called an offset which identifies its position in the log.

Message ordering is guaranteed within a single partition but there is no such guarantee across the partitions of a topic.

This means that if messages were sent from the producer in a specific order, the broker will write them to a partition in that order and all consumers will read them in that order.
-->


## Replication

<!--
Kafka provides availability and durability despite broker failure by replicating partitions across the brokers of the cluster.

Every partition is replicated n times where n is the replication factor of the topic.

This allows Kafka to automatically failover to these replicas when a server in the cluster fails so that messages remain available in the presence of failures.

Among the replicas of a partition, one is designated as the leader and the others are the followers.

In order to guarantee consistency all reads and writes go through the leader, while followers only try to stay up-to-date with the leader.
-->



## In-Sync Replicas

<!-- 
The fundamental guarantee a log replication algorithm must provide is that if it tells the client a message is committed, and the leader fails, the newly elected leader must also have that message.

Kafka gives this guarantee by requiring the leader to be elected from a subset of replicas that are in-sync with the previous leader which is to say they are caught up to the leader’s log.

The leader for every partition tracks this in-sync replica list by computing the lag of every replica from itself.

When a producer sends a message to the broker, it is written by the leader and replicated to all the partition’s replicas.

A message is committed only after it has been successfully copied to all the in-sync replicas.

Since the message replication latency is capped by the slowest in-sync replica, it is important to quickly detect slow replicas and remove them from the in-sync replica list.

A replica is considered to be out-of-sync or lagging when it falls “sufficiently” behind the leader of the partition.

Produced messages are considered committed when they were written to the partition on all its in-sync replicas.

Only committed messages are available to be read by consumers.
-->


## Consumer Groups

<!-- 
A consumer typically operates as part of a consumer group consisting of one or more consumes that work together to consume a topic.

Each member of a consumer group reads from zero or more exclusive partitions.

The mapping of partitions to consumers is called partition ownership.

This ensures that each partition is only consumed by one consumer.

Consumer groups allows topic consumption to be horizontally scaled.

Indeed, the main way to scale message consumption from a topic is to add more consumers to a consumer group.

However, If there are more consumers in a group than partitions then some consumers will be inactive.

This is a good reason to create topics with a large number of partitions.

It is very common to have multiple applications that need to read data from the same topic.

In those cases, we want each application to get all of the messages, rather than just a subset. To make sure an application gets all the messages in a topic, ensure the application has its own consumer group.

Kafka scales to a large number of consumers and consumer groups without reducing performance.
-->


## Rebalancing

<!-- If a consumer becomes unavailable, its partitions will be reassigned to the remaining consumers in its consumer group. -->

<!-- Similarly, if a consumer joins a consumer group then it will be assigned partitions previously consumed by other consumers in the consumer group. -->

<!-- This movement of partition ownership from one consumer to another is called a rebalance. -->

<!-- Reassignment of partitions to consumers also happen when the topics the consumer group is consuming are modified such as when adding partitions. -->

<!-- During a rebalance, consumers can't consume messages, so a rebalance is constitutes a short window of unavailability of the entire consumer group. -->

The way consumers maintain membership in a consumer group and ownership of the partitions assigned to them is by sending heartbeats to a Kafka broker designated as the group coordinator (this broker can be different for different consumer groups).

As long as the consumer is sending heartbeats at regular intervals, it is assumed to be alive and processing messages from its partitions.

Heartbeats are sent when the consumer polls (i.e., retrieves records) and when it commits records it has consumed.

If the consumer stops sending heartbeats for long enough, its session will time out and the group coordinator will consider it dead and trigger a rebalance.

If a consumer crashed and stopped processing messages, it will take the group coordinator a few seconds without heartbeats to decide it is dead and trigger the rebalance.

During those seconds, no messages will be processed from the partitions owned by the dead consumer.

<!-- When closing a consumer cleanly, the consumer will notify the group coordinator that it is leaving, and the group coordinator will trigger a rebalance immediately, reducing the gap in processing. -->







## Consumer Offsets

<!-- 
Each consumer keeps track of which messages it has already processed by committing the offsets of those messages to Kafka.

This is done by producing a message to a specific __consumer_offsets topic with the committed offset for each partition.

By storing the offset of the last consumed message for each partition, a consumer can stop and restart without losing its place.

Furthermore, if a rebalance occurs and a consumer is assigned a new partition, then the consumer will read the latest committed offset for that partition and continue from there.

If the committed offset is smaller than the offset of the last message the client processed, the messages between the last processed offset and the committed offset will be processed twice.

If the committed offset is larger than the offset of the last message the client actually processed, all messages between the last processed offset and the committed offset will be missed by the consumer group.

Offsets also allow consumers to read from different places in the log simultaneously and replay offsets as required.
-->


## Acknowledgements

<!-- 
The acks parameter controls how many partition replicas must receive the record before the producer can consider a write successful.

There are three allowed values for the acks parameter:

- If acks=0, the producer will not wait for any verification from the broker. This allows for very high throughput at the cost of possible silent message losses.

- If acks=1, the producer will wait for verification that the leader has received the message. The message can still get lost if the leader crashes and a replica without this message gets elected via unclean leader election.

- If acks=all, the producer will wait for verification that all in-sync replicas have received the message. This will ensure a high degree of safety but will result in higher latency.
-->


## Brokers

<!--
A single Kafka server is called a broker.

Broker receive messages from producers, assigns offsets to them and commits them to disk.

Brokers also service consumers by responding to fetch requests for partitions and by responding with the messages that have been committed to disk.
-->


## Clusters

<!-- 
Kafka is designed to operate as a cluster of multiple brokers running on separate servers.

Within a cluster of brokers, one live broker will be automatically elected as the cluster controller.

The controller is responsible for administrative operations including assigning partitions to brokers and monitoring for broker failures.

The controller is responsible for electing partition leaders.
-->


## Cluster Membership

<!--
Kafka uses ZooKeeper for storing certain broker metadata including the list of brokers that are currently members of the cluster.

Every broker has a unique identifier that is either set in the broker configuration file or automatically generated.
-->


## Committing Offsets

<!-- There are a number of approaches to committing offsets. Configuring enable.auto.commit=true will enable automatic commit. Whenever the consumer polls for messages, it will check if the time interval set by auto.commit.interval.ms has elapsed since its last commit and commit the offsets returned in the last poll if it has. Unfortunately, this approach is always vulnerable to message duplication during rebalancing.

Alternatively, configuring auto.commit.offset=false will require the application to explicitly commit offsets. A synchronous commit can be used to commit the latest offset returned by polling and return once the offset is committed. However, when rebalance is triggered, all the messages from the beginning of the most recent batch until the time of the rebalance will be processed twice.

A drawback of synchronous commits is that the application is blocked until the broker responds to the commit request. Another option is to asynchronously commit offsets. The drawback is that while a synchronous commit will retry the commit until it either succeeds or encounters a non-retriable error, an asynchronous commit will not retry. This is to avoid having retried commits overwrite later successful commits. -->

A simple pattern to get commit order right for asynchronous
retries is to use a monotonically increasing sequence number.
Increase the sequence number every time you commit and add the
sequence number at the time of the commit to the commitAsync
callback. When you’re getting ready to send a retry, check if the
commit sequence number the callback got is equal to the instance
variable; if it is, there was no newer commit and it is safe to retry. If
the instance sequence number is higher, don’t retry because a
newer commit was already sent.


## Storage

<!-- 
A partition replica is the unit of storage in Kafka.

Partitions cannot be split between multiple brokers and not even between multiple disks on the same broker which limits the size of a partition.
-->


## Retention

<!-- 
A defining feature of Kafka is that it can durably store messages for some period of time.

Kafka brokers are configured with a default retention setting for topics, either retaining messages for some period of time or until the topic reaches a certain size in bytes.

Once these limits are reached, messages are expired and deleted so that the retention configuration is a minimum amount of data available at any time.

These options can be selected on a per-topic basis, allowing for different streams of messages to have different amounts of retention depending on the consumer needs.

Durable retention means that if a consumer falls behind, either due to slow processing or a burst in traffic, there is no danger of losing data

It also means that maintenance can be performed on consumers, taking applications offline for a short period of time, with no concern about messages backing up on the producer or getting lost.

Consumers can be stopped, and the messages will be retained in Kafka. This allows them to restart and pick up processing messages where they left off with no data loss.
-->

## Asynchronous Commit

<!-- 
  Normally, occasional failures to commit without retrying are not a huge problem because if the problem is temporary, the following commit will be successful. But if we know that this is the last commit before we close the consumer, or before a rebalance, we want to make extra sure that the commit succeeds. Therefore, a common pattern is to used asynchronous commits and then use a synchronous commit just before shutdown because it will retry until it succeeds or suffers unrecoverable failure.

But if we are closing, there is no “next commit.” We call commitSync(),
-->




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

