


























# Resolving Inconsistencies

When the entry has been
safely replicated (as described below), the leader applies
the entry to its state machine and returns the result of that
execution to the client.


Raft maintains the following properties,
which together constitute the Log Matching Property
in Figure 3:

• If two entries in different logs have the same index
and term, then they store the same command.
• If two entries in different logs have the same index
and term, then the logs are identical in all preceding
entries.


During normal operation, the logs of the leader and
followers stay consistent, so the AppendEntries consistency
check never fails. However, leader crashes can leave
the logs inconsistent (the old leader may not have fully
replicated all of the entries in its log). These inconsistencies
can compound over a series of leader and follower
crashes. Figure 7 illustrates the ways in which followers’
logs may differ from that of a new leader. A follower may
be missing entries that are present on the leader, it may
have extra entries that are not present on the leader, or
both. Missing and extraneous entries in a log may span
multiple terms.

In Raft, the leader handles inconsistencies by forcing
the followers’ logs to duplicate its own. This means that
conflicting entries in follower logs will be overwritten
with entries from the leader’s log. Section 5.4 will show
that this is safe when coupled with one more restriction.
To bring a follower’s log into consistency with its own,
the leader must find the latest log entry where the two
logs agree, delete any entries in the follower’s log after
that point, and send the follower all of the leader’s entries
after that point. All of these actions happen in response
to the consistency check performed by AppendEntries
RPCs. The leader maintains a nextIndex for each follower,
which is the index of the next log entry the leader will
send to that follower.When a leader first comes to power,
it initializes all nextIndex values to the index just after the
last one in its log (11 in Figure 7). If a follower’s log is
inconsistent with the leader’s, the AppendEntries consistency
check will fail in the next AppendEntries RPC. After
a rejection, the leader decrements nextIndex and retries
the AppendEntries RPC. Eventually nextIndex will reach
a point where the leader and follower logs match. When
this happens,AppendEntrieswill succeed, which removes
any conflicting entries in the follower’s log and appends
entries fromthe leader’s log (if any). Once AppendEntries
succeeds, the follower’s log is consistentwith the leader’s,
and it will remain that way for the rest of the term.











