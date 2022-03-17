
// // CommitEntry is the data reported by Raft to the commit channel. Each commit
// // entry notifies the client that consensus was reached on a command and it can
// // be applied to the client's state machine.
// type CommitEntry struct {
// 	// Command is the client command being committed.
// 	Command interface{}

// 	// Index is the log index at which the client command is committed.
// 	Index int

// 	// Term is the Raft term at which the client command is committed.
// 	Term int
// }

// ConsensusModule (CM) implements a single node of Raft consensus.
type ConsensusModule struct {
	// newCommitReadyChan is an internal notification channel used by goroutines
	// that commit new entries to the log to notify that these entries may be sent
	// on commitChan.
	newCommitReadyChan chan struct{}
}

func NewConsensusModule(id int, peerIds []int, server *Server, ready <-chan interface{}, commitChan chan<- CommitEntry) *ConsensusModule {

	cm.peerIds = peerIds
	cm.commitChan = commitChan
	cm.newCommitReadyChan = make(chan struct{}, 16)

	// NB
	go cm.commitChanSender()

	return cm
}


// Submit submits a new command to the CM. This function doesn't block; clients
// read the commit channel passed in the constructor to be notified of new
// committed entries. It returns true iff this CM is the leader - in which case
// the command is accepted. If false is returned, the client will have to find
// a different CM to submit this command to.
func (cm *ConsensusModule) Submit(command interface{}) bool {
	cm.mu.Lock()
	defer cm.mu.Unlock()

	cm.dlog("Submit received by %v: %v", cm.state, command)
	if cm.state == Leader {
		cm.log = append(cm.log, LogEntry{Command: command, Term: cm.currentTerm})
		cm.dlog("... log=%v", cm.log)
		return true
	}
	return false
}

// Stop stops this CM, cleaning up its state. This method returns quickly, but
// it may take a bit of time (up to ~election timeout) for all goroutines to
// exit.
func (cm *ConsensusModule) Stop() {
	cm.mu.Lock()
	defer cm.mu.Unlock()
	cm.state = Dead
	cm.dlog("becomes Dead")
	close(cm.newCommitReadyChan)
}


func (cm *ConsensusModule) AppendEntries(args AppendEntriesArgs, reply *AppendEntriesReply) error {

			// Find an insertion point - where there's a term mismatch between
			// the existing log starting at PrevLogIndex+1 and the new entries sent
			// in the RPC.
			logInsertIndex := args.PrevLogIndex + 1
			newEntriesIndex := 0

			for {
				if logInsertIndex >= len(cm.log) || newEntriesIndex >= len(args.Entries) {
					break
				}
				if cm.log[logInsertIndex].Term != args.Entries[newEntriesIndex].Term {
					break
				}
				logInsertIndex++
				newEntriesIndex++
			}

			// At the end of this loop:
			// - logInsertIndex points at the end of the log, or an index where the
			//   term mismatches with an entry from the leader
			// - newEntriesIndex points at the end of Entries, or an index where the
			//   term mismatches with the corresponding log entry
			if newEntriesIndex < len(args.Entries) {
				cm.log = append(cm.log[:logInsertIndex], args.Entries[newEntriesIndex:]...)
			}

			// Set commit index.
			if args.LeaderCommit > cm.commitIndex {
				cm.commitIndex = intMin(args.LeaderCommit, len(cm.log)-1)
				cm.newCommitReadyChan <- struct{}{}
			}
		}
	}

	reply.Term = cm.currentTerm
	cm.dlog("AppendEntries reply: %+v", *reply)
	return nil
}


// startElection starts a new election with this CM as a candidate.
// Expects cm.mu to be locked.
func (cm *ConsensusModule) startElection() {

	// Send RequestVote RPCs to all other servers concurrently.
	for _, peerId := range cm.peerIds {
		go func(peerId int) {
			cm.mu.Lock()
			savedLastLogIndex, savedLastLogTerm := cm.lastLogIndexAndTerm()
			cm.mu.Unlock()

			args := RequestVoteArgs{
				Term:         savedCurrentTerm,
				CandidateId:  cm.id,
				LastLogIndex: savedLastLogIndex,
				LastLogTerm:  savedLastLogTerm,
			}

			var reply RequestVoteReply
			if err := cm.server.Call(peerId, "ConsensusModule.RequestVote", args, &reply); err == nil {
				cm.mu.Lock()
				defer cm.mu.Unlock()

				if cm.state != Candidate {
					return
				}

				if reply.Term > savedCurrentTerm {
					cm.becomeFollower(reply.Term)
					return
				} else if reply.Term == savedCurrentTerm {
					if reply.VoteGranted {
						votesReceived += 1
						if votesReceived*2 > len(cm.peerIds)+1 {
							cm.startLeader()
							return
						}
					}
				}
			}
		}(peerId)
	}

	// Run another election timer, in case this election is not successful.
	go cm.runElectionTimer()
}


// leaderSendHeartbeats sends a round of heartbeats to all peers, collects their
// replies and adjusts cm's state.
func (cm *ConsensusModule) leaderSendHeartbeats() {
	cm.mu.Lock()
	savedCurrentTerm := cm.currentTerm
	cm.mu.Unlock()

	for _, peerId := range cm.peerIds {
		go func(peerId int) {
			cm.mu.Lock()
			ni := cm.nextIndex[peerId]
			prevLogIndex := ni - 1
			prevLogTerm := -1
			if prevLogIndex >= 0 {
				prevLogTerm = cm.log[prevLogIndex].Term
			}
			entries := cm.log[ni:]

			args := AppendEntriesArgs{
				Term:         savedCurrentTerm,
				LeaderId:     cm.id,
				PrevLogIndex: prevLogIndex,
				PrevLogTerm:  prevLogTerm,
				Entries:      entries,
				LeaderCommit: cm.commitIndex,
			}
			cm.mu.Unlock()
			cm.dlog("sending AppendEntries to %v: ni=%d, args=%+v", peerId, ni, args)
			var reply AppendEntriesReply
			if err := cm.server.Call(peerId, "ConsensusModule.AppendEntries", args, &reply); err == nil {
				cm.mu.Lock()
				defer cm.mu.Unlock()
				if reply.Term > savedCurrentTerm {
					cm.dlog("term out of date in heartbeat reply")
					cm.becomeFollower(reply.Term)
					return
				}

				if cm.state == Leader && savedCurrentTerm == reply.Term {
					if reply.Success {
						cm.nextIndex[peerId] = ni + len(entries)
						cm.matchIndex[peerId] = cm.nextIndex[peerId] - 1
						cm.dlog("AppendEntries reply from %d success: nextIndex := %v, matchIndex := %v", peerId, cm.nextIndex, cm.matchIndex)

						savedCommitIndex := cm.commitIndex
						for i := cm.commitIndex + 1; i < len(cm.log); i++ {
							if cm.log[i].Term == cm.currentTerm {
								matchCount := 1
								for _, peerId := range cm.peerIds {
									if cm.matchIndex[peerId] >= i {
										matchCount++
									}
								}
								if matchCount*2 > len(cm.peerIds)+1 {
									cm.commitIndex = i
								}
							}
						}
						if cm.commitIndex != savedCommitIndex {
							cm.dlog("leader sets commitIndex := %d", cm.commitIndex)
							cm.newCommitReadyChan <- struct{}{}
						}
					} else {
						cm.nextIndex[peerId] = ni - 1
						cm.dlog("AppendEntries reply from %d !success: nextIndex := %d", peerId, ni-1)
					}
				}
			}
		}(peerId)
	}
}



// commitChanSender is responsible for sending committed entries on
// cm.commitChan. It watches newCommitReadyChan for notifications and calculates
// which new entries are ready to be sent. This method should run in a separate
// background goroutine; cm.commitChan may be buffered and will limit how fast
// the client consumes new committed entries. Returns when newCommitReadyChan is
// closed.
func (cm *ConsensusModule) commitChanSender() {
	for range cm.newCommitReadyChan {
		// Find which entries we have to apply.
		cm.mu.Lock()
		savedTerm := cm.currentTerm
		savedLastApplied := cm.lastApplied
		var entries []LogEntry
		if cm.commitIndex > cm.lastApplied {
			entries = cm.log[cm.lastApplied+1 : cm.commitIndex+1]
			cm.lastApplied = cm.commitIndex
		}
		cm.mu.Unlock()
		cm.dlog("commitChanSender entries=%v, savedLastApplied=%d", entries, savedLastApplied)

		for i, entry := range entries {
			cm.commitChan <- CommitEntry{
				Command: entry.Command,
				Index:   savedLastApplied + i + 1,
				Term:    savedTerm,
			}
		}
	}
	cm.dlog("commitChanSender done")
}

func intMin(a, b int) int {
	if a < b {
		return a
	}
	return b
}