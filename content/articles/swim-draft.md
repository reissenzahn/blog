# Resources




# Membership Protocols

Briefly, a membership protocol provides each process
(“member”) of the group with a locally-maintained list of
other non-faulty processes in the group. The protocol ensures
that the membership list is updated with changes resulting
from new members joining the group, or dropping
out (either voluntarily or through a failure). The membership
list is made available to the application either directly
in its address space, or through a callback interface or an
API. The application is free to use the contents of the list as
required, e.g. gossip-based dissemination protocols would
use the list to periodically pick target members for gossip.

The reliability and scalability of a membership subsystem
can be measured via several performance metrics.
Membership changes have to be propagated within the
group quickly after their occurrence. The asynchrony and
unreliability of the underlying network can cause messages
to be lost, leading to false detection of process failures,
since a process that is losing messages is indistinguishable
from one that has failed [10]. This rate of false positives
has to be low. Finally, the protocol needs to be peer-to-peer
(not rely on a central server), and impose low message and
computation loads on the network and processes.

The design of a distributed membership algorithm
has
traditionally been approached through the technique of
heartbeating. Each process periodically sends out an incremented
heartbeat counter to the outside world. Another
process is detected as failed when a heartbeat is not received
from it for some time. However, actual implementations
of heartbeating suffer from scalability limitations. Sending
all heartbeats to a central server leads to hot-spot creation.
Sending heartbeats to all members (through either network
multicast, or gossiping [16]) leads to a message load on the
network and group that grows quadratically with the group
size. Heartbeating along a logical ring [9] suffers from unpredictability
of failure detection time when there are multiple
failures. Unfortunately, as the group size rises, so does
the likelihood of simultaneous multiple failures.



# Dissemination Component

Upon detecting the failure of another group member, the process simply multicasts this information to the rest of the group as _failed_($M_j$) messages. A member receiving this message deletes $M_j$ from its local membership list.

Information about newly joined members or voluntarily
leaving members are multicast in a similar manner. However,
for a process to join the group, it would need to know
at least one contact member in the group. This can be realized
through one of several means: if the group is associated
with a well known server or IP multicast address, all joins
could be directed to the associated address. In the absence
of such infrastructure, join messages could be broadcast,
and group members hearing it can probabilistically decide
(by tossing a coin) whether to reply to it. Alternatively, to
avoid multiple member replies, a static coordinator could
be maintained within the group for the purpose of handling
group join requests. In fact, existence of multiple coordinators
does not affect the correctness of the protocol, and
only leads to multiple replies to the join request. Discovery
and resolution of multiple coordinators can be done over
time through the Dissemination Component.



# 


# Suspicion Mechanism: Reducing the Frequency of False Positives

The basic SWIM failure detector protocol, in spite of its calculable accuracy, is subject to slow processes (e.g. ones losing a lot of packets from buffer overflow) declaring several other non-faulty processes as faulty. It is also
possible that a process is perturbed for small durations of
time, e.g. on an overloaded host. This might cause the process
to miss the opportunity to send timely replies to pings
received meanwhile, and be mistakenly declared as failed.

In the SWIM failure detector protocol described so far, if a non-faulty group member $M_j$ is mistakenly detected as failed by another group member $M_i$ either due to network packet losses or because $M_j$ was asleep for sometime or because $M_i$ is a slow process, then $M_j$ will be declared as failed in the group. In other words, a healthy process $M_j$ suffers a very heavy penalty, by being forced to drop out of the group at the very first instance that it is mistakenly detected as failed in the group. This leads to a high rate of false positives in detecting failures.

We reduce the effect of this problem by modifying SWIM to run a Suspicion sub-protocol whenever a failure is detected by the basic SWIM failure detector protocol.

The Suspicion sub-protocol works as follows. Consider a member $M_i$ that chooses a member $M_j$ as a _ping_ target in the current protocol period and runs the basic SWIM failure detector protocol period. If $M_i$ receives no acknowledgements, either directly or through the indirect probing sub-group, it does not declare $M_j$ as failed. Instead, $M_i$ marks $M_j$ as a Suspected member in its local membership list.  In addition, a $\left\{ Suspect \> M_j : M_i \> suspects \> M_j\right\}$ message is disseminated through the group through the Dissemination Component. Any group member $M_l$ receiving such a message also marks $M_j$ as suspected. Suspected members stay on in the membership list and are treated similar to non-faulty members with regards to _ping_ target selection operation of the SWIM failure detector protocol.

If a member $M_l$ successfully _pings_ a suspected member member $M_j$ during the due course of the basic SWIM protocol, it un-marks the previous suspicion of $M_j$ in its membership list, and spreads an $\left\{ Alive \> M_j : M_l \> knows \> M_j \> is alive \right\}$ message in the group through the Dissemination
Component (in infection-style in our system). Such an Alive
message un-marks the suspected member $M_j$ in membership
lists of recipient members. Notice that if member $M_j$ receives such a message suspecting it, it can start propagating an Alive message clarifying its non-failure.

Suspected entries in membership lists expire after a prespecified
time-out. If $M_j$ is suspected at some member $M_h$, and thistimesout before receipt of an Alive message, $M_h$ declares $M_j$ as faulty, drops it from the local membership list and begins spreading the message $\left\{ Confirm \> M_j : M_h \> declares \> M_j \> as faulty \right\}$ through the Dissemination
Component. This message overrides any previous Suspect 
or Alive messages, and cascades in deletion of $M_j$ from the
membership lists of all recipients.

This mechanism reduces (but does not eliminate) the rate
of failure detection false positives. Notice also that the
Strong Completeness property of the original protocol continues
to hold. Failures of processes suspecting a failed process $M_j$
may prolong detection time, but eventual detection
is guaranteed.

From the above discussion, Alive messages override Suspect
messages, and Confirm messages override both Suspect
and Alive messages, in their effect on the local membership
list element corresponding to the suspected member $M_j$.
However, a member might be suspected and unsuspected
multiple times during its lifetime. These multiple
versions of Suspect and Alive messages (all pertaining to the
same member $M_j$) need to be distinguished through unique
identifiers. These identifiers are provided by using a virtual
incarnation number field with each element in the membership
lists. Incarnation numbers are global. A member
$M_i$'s incarnation number is initialized to 0 when it joins the
group, and it can be incremented only by $M_i$ when it receives
information (through the Dissemination Component)
about itself being suspected in the current incarnation - $M_i$
then generates an Alive message with its identifier and
 an
incremented incarnation number, and spreads this through
the Dissemination Component to the group.
Thus,
Suspect, Alive and Confirm
messages contain
the incarnation number of the member, besides its identifier.
The order of preference among these messages and their
effect on the membership list is specified below.

- $\left\{ Alive \> M_l, inc = i \right\}$ overrides
  - $\left\{ Suspect \> M_l, inc = j \right\}$, i > j
  - $\left\{ Alive \> M_l, inc = j \right\}$, i > j

- $\left\{ Suspect \> M_l, inc = i \right\}$ overrides
  - $\left\{ Suspect \> M_l, inc = j \right\}$, i > j
  - $\left\{ Alive \> M_l, inc = j \right\}$, i >= j

- $\left\{ Confirm \> M_l, inc = i \right\}$ overrides
  - $\left\{ Alive \> M_l, inc = j \right\}$, any j
  - $\left\{ Suspect \> M_l, inc = j \right\}$, any j

It is easy to see that these orders of preference and overriding
maintain the desired correctness properties of the
Failure Detector Component.
The preference rules and infection-style Dissemination
Component also accommodate suspicions of a process by
multiple other processes. Preference rules do not depend on
the source of suspicion, and the infection-style dissemination
spreads a message (Suspect, Alive or Confirm) quicker
if there are multiple sources, with exactly the same overhead
per process as with one source of infection [8].







# RoundRobin Probe Target Selection




# Conclusions




