TODO
11: System Limits and Options
12: System and Process Information
13: File I/O Buffering

25: Process Termination
26: Monitoring Child Processes
27: Program Execution

31: Threads: Thread Safety and Per-Thread Storage
32: Threads: Thread Cancellation
33: Threads: Further Details

36: Process Resources

60: Sockets: Server Design
61: Sockets: Advanced Topics
63: Alternative I/O Models


NEXT
5: File I/O: Further Details
30: Threads: Thread Synchronization



COPIED
20: Signals: Fundamental Concepts
21: Signals: Signal Handlers
22: Signals: Advanced Features


DONE
4: File I/O: The Universal I/O Model
7: Memory Allocation
19: Monitoring File Events
24: Process Creation
29: Threads: Introduction
56: Sockets: Introduction
59: Sockets: Internet Domains


SKIP 1
6: Processes
10: Time
18: Directories and Links
23: Timers and Sleeping
37: Daemons
41: Fundamentals of Shared Libraries
42: Advanced Features of Shared Libraries
43: Interprocess Communication Overview
44: Pipes and FIFOs
50: Virtual Memory Operations
55: File Locking


SKIP 2
1: History and Standards
2: Fundamental Concepts
3: System Programming Concepts
8: Users and Groups
9: Process Credentials
14: File Systems
15: File Attributes
16: Extended Attributes
17: Access Control Lists
28: Process Creation and Program Execution in More Detail
34: Process Groups, Sessions, and Job Control
35: Process Priorities and Scheduling
38: Writing Secure Privileged Programs
39: Capabilities
40: Login Accounting
45: Introduction to System V IPC
46: System V Message Queues
47: System V Semaphores
48: System V Shared Memory
51: Introduction to POSIX IPC
52: POSIX Message Queues
53: POSIX Semaphores
54: POSIX Shared Memory
57: Sockets: UNIX Domain
58: Sockets: Fundamentals of TCP/IP Networks
62: Terminals
64: Pseudoterminals





MISC

Atomicity is a concept that we’ll encounter repeatedly when discussing the operation of system calls. All system calls are executed atomically. By this, we mean that the kernel guarantees that all of the steps in a system call are completed as a single operation, without being interrupted by another process or thread.

Atomicity is essential to the successful completion of some operations. In particular, it allows us to avoid race conditions (sometimes known as race hazards). A race condition is a situation where the result produced by two processes (or threads) operating on shared resources depends in an unexpected way on the relative order in which the processes gain access to the CPU(s).



NOW: 49: Memory Mappings

The offset argument specifies the starting byte of the region to be mapped from the
file, and must be a multiple of the system page size. Specifying offset as 0 causes the file
to be mapped from the beginning.

Attempts to access bytes beyond the end of the mapping result in the generation of a SIGSEGV signal (assuming that there is no other mapping at that location). The default action for this signal is to terminate the process with a core dump.


One point that we have not so far explained in detail is the interaction between the
memory protection specified in the mmap() prot argument and the mode in which the
mapped file is opened. As a general principle, we can say that the PROT_READ and
PROT_EXEC protections require that the mapped file is opened O_RDONLY or O_RDWR, and that
the PROT_WRITE protection requires that the mapped file is opened O_WRONLY or O_RDWR.




 



#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "tlpi_hdr.h"
int
main(int argc, char *argv[])
{
 int *addr; /* Pointer to shared memory region */
#ifdef USE_MAP_ANON /* Use MAP_ANONYMOUS */
 addr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
 MAP_SHARED | MAP_ANONYMOUS, -1, 0);
 if (addr == MAP_FAILED)
 errExit("mmap");
#else /* Map /dev/zero */
 int fd;
 fd = open("/dev/zero", O_RDWR);
 if (fd == -1)
 errExit("open");
 addr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
 if (addr == MAP_FAILED)
 errExit("mmap");
 if (close(fd) == -1) /* No longer needed */
 errExit("close");
#endif
 *addr = 1; /* Initialize integer in mapped region */
 switch (fork()) { /* Parent and child share mapping */
 case -1:
 errExit("fork");
 case 0: /* Child: increment shared integer and exit */
 printf("Child started, value = %d\n", *addr);
 (*addr)++;
 if (munmap(addr, sizeof(int)) == -1)
 errExit("munmap");
 exit(EXIT_SUCCESS);
 default: /* Parent: wait for child to terminate */
 if (wait(NULL) == -1)
 errExit("wait");
 printf("In parent, value = %d\n", *addr);
Memory Mappings 1037
 if (munmap(addr, sizeof(int)) == -1)
 errExit("munmap");
 exit(EXIT_SUCCESS);
 }
}







