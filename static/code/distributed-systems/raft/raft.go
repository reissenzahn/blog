package main

import (
	"fmt"
	"math/rand"
	"sync"
	"time"
)

type State int

const (
	Follower State = iota
	Candidate
	Leader
	Stopped
)

type LogEntry struct {
	Command interface{}
	Term int
}

type Peer struct {
	id int
}

type Raft struct {
	// the local server id
	id int

	// list of other servers in the cluster
	peers []Peer


	// persistent raft state

	// the best guess as to the current term
	currentTerm int

	// id of the last server the local server voted for (or -1 if none)
	votedFor int

	// local copy of the replicated log
	log []LogEntry


	// volatile raft state

	// index of the highest log entry known to be committed (initialized to 0)
	commitIndex int

	// index of highest log entry applied to state machine (initialized to 0)
	lastApplied int

	// current server state
	state State

	// time when the most recent election timeout was started
	electionTimeoutStart time.Time


	// volatile raft state on servers

	// map from server id to index of the next log entry to send to that server
	nextIndex map[int]int

	// map from server id index of highest log entry known to be replicated on server
	matchIndex map[int]int


	// channel to report commands ready for the state machine
	commands chan<- Command
	
	mutex sync.Mutex
}

func NewRaft(id int, peerIds []int, commands chan<- Command) *Raft {
	self := new(Raft)

	self.id = id
	self.state = Follower
	self.votedFor = -1
	self.commitIndex = -1
	self.lastApplied = -1
	self.nextIndex = make(map[int]int)
	self.matchIndex = make(map[int]int)

	// TODO: move to Start() method
	go func() {
		self.mutex.Lock()
		self.electionTimeoutStart = time.Now()
		self.mutex.Unlock()
		self.startElectionTimer()
	}()

	return self
}

// start a blocking one-off election timeout that either exits if it is no longer needed or starts an election if the timeout elapses
func (self *Raft) startElectionTimer() {
	duration := time.Duration(150 + rand.Intn(150)) * time.Millisecond

	self.mutex.Lock()
	savedCurrentTerm := self.currentTerm
	self.mutex.Unlock()
	
	ticker := time.NewTicker(10 * time.Millisecond)
	defer ticker.Stop()

	for {
		<-ticker.C

		// discard the timer if we have become the leader/stopped or if the current term has changed
		self.mutex.Lock()
		if (self.state != Candidate && self.state != Follower) || savedCurrentTerm != self.currentTerm {
			self.mutex.Unlock()
			return
		}

		// start an election if the election timeout elapses
		if time.Since(self.electionTimeoutStart) >= duration {
			self.startElection()
			self.mutex.Unlock()
			return
		}

		self.mutex.Unlock()
	}
}

// start a new election; expects mutex to be locked
func (self *Raft) startElection() {
	// increment current term
	self.currentTerm += 1
	savedCurrentTerm := self.currentTerm

	// transition to candidate state
	self.state = Candidate
	
	// vote for self
	self.votedFor = self.id

	// reset election timeout
	electionTimeoutStart = time.Now()

	// issue RequestVote RPCs in parallel to each of the other servers in the cluster
	votes := 1

	for _, peer := range self.peers {
		go func(peer Peer) {
			self.mutex.Lock()
			
		}(peer)
	}
}

// switch to a follower in acknowledgement of a higher current term; expects mutex to be locked
func (self *Raft) becomeFollower(term int) {
	self.state = Follower
	self.currentTerm = term
	self.votedFor = -1
	self.electionTimeoutStart = time.Now()

	go self.startElectionTimer()
}

// switch to leader and start sending heartbeats; expects mutex to be locked
func (self *Raft) becomeLeader() {
	self.state = Leader

	for _, peer := range peers {
		// initialized to leader last log index + 1
		self.nextIndex[peer.id] = len(self.log)

		// initialized to one before first log index
		self.matchIndex[peer.id] = -1
	}

	go func() {
		ticker := time.NewTicker(50 * time.Millisecond)
		defer ticker.Stop()

		// send period heartbeats as long as still leader
		go func() {
			self.sendHeartbeats()

			<-ticker.C

			self.mutex.Lock()
			if self.state != Leader {
				self.mutex.Unlock()
				return
			}
			self.mutex.Unlock()
		}()
	}
}

// return the last log index and the term of the last log entry
// NOTE: expects self.mutex to be locked
func (self *Raft) lastLogIndexAndTerm() (int, int) {
	if len(self.log) > 0 {
		lastIndex := len(self.log) - 1
		return lastIndex, self.log[lastIndex].Term
	}

	return -1, -1
}


// RPCs

type RequestVoteArgs struct {
	// current term of the candidate
	Term int

	// id of the candidate
	CandidateId int

	// index of last log entry
	LastLogIndex int

	// term of last log entry
	LastLogTerm int
}

type RequestVoteReply struct {
	// current term for candidate to update itself
	Term int

	// true indicates the candidate received the vote
	VoteGranted bool
}

func (self *Raft) RequestVote(args RequestVoteArgs, reply *RequestVoteReply) error {
	self.mutex.Lock()
	defer self.mutex.Unlock()

	if self.state == Stopped {
		return nil
	}

	// notice that this will set the currentTerm to at least the received term
	if args.Term > self.currentTerm {
		self.becomeFollower(args.Term)
	}

	lastLogIndex, lastLogTerm := self.lastLogIndexAndTerm()

	// requirements to vote for candidate:

	// 1. the current term is not greater than the received term
	isNotObsolete := self.currentTerm == args.Term

	// 2. have not voted for another candidate
	voteNotAlreadyCast := self.votedFor == -1 || self.votedFor == args.CandidateId

	// 3. it is not the case that lastTerm > receivedLastTerm || (lastTerm == receivedLastTerm && lastIndex > receivedLastIndex)
	isUpToDate := (args.LastLogTerm > lastLogTerm || (args.LastLogTerm == lastLogterm && args.LastLogIndex >= lastLogIndex)

	if isNotObsolete && voteNotAlreadyCast && isUpToDate {
		reply.VoteGranted = true
		self.votedFor = args.CandidateId

		// reset election timeout when casting a vote
		self.electionTimeoutStart = time.Now()
	} else {
		reply.VoteGranted = false
	}

	reply.Term = self.currentTerm

	return nil
}


type AppendEntriesArgs struct {
	// current term of the leader
	Term int

	// allows followers to redirect clients
	LeaderId int

	// index of log entry immediately preceding new ones
	PrevLogIndex int

	// term of log entry immediately preceding new ones
	PrevLogTerm int

	// log entries to store (empty for heartbeat)
	Entries []LogEntry

	// commit index of leader
	LeaderCommit int
}

type AppendEntriesReply struct {
	// current term for leader to update itself
	Term int

	// true indicates follower contained entry matching PrevLogIndex and PrevLogterm
	Success bool
}

func (self *Raft) AppendEntries(args AppendEntriesArgs, reply *AppendEntriesReply) error {
	self.mutex.Lock()
	defer self.mutex.Unlock()

	if self.state == Stopped {
		return nil
	}

	if args.Term > cm.currentTerm {
		cm.becomeFollower(args.Term)
	}

	reply.Success = false

	// first check is that the provided term is not obsolete 
	if args.Term == self.currentTerm {
		// recognize authority of leader and reset election timeout
		if self.state != Follower {
			self.becomeFollower(args.Term)
		}

		self.electionTimeoutStart = time.Now()

		// check if the log contains an entry at PrevLogIndex whose term matches PrevLogTerm (note that in the case of PrevLogIndex == -1 this is vacuously true)
		if args.PrevLogIndex == -1 || (args.PrevLogIndex < len(self.log) && args.PrevLogTerm == self.log[args.PrevLogIndex].Term) {
			reply.Success = true

			// TODO
		}
	}

	reply.Term = self.currentTerm

	return nil
}



