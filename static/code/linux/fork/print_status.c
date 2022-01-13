
// get strsignal() declaration from <string.h>
#define _GNU_SOURCE

#include <string.h>
#include <sys/wait.h>

void print_wait_status(const char *msg, int status) {
  if (WIFEXITED(status)) {

  }
}

void printWaitStatus(const char *msg, int status)
{
 if (msg != NULL)
 printf("%s", msg);
 if (WIFEXITED(status)) {
 printf("child exited, status=%d\n", WEXITSTATUS(status));
 } else if (WIFSIGNALED(status)) {
 printf("child killed by signal %d (%s)",
 WTERMSIG(status), strsignal(WTERMSIG(status)));
 printf("\n");
 } else if (WIFSTOPPED(status)) {
 printf("child stopped by signal %d (%s)\n",
 WSTOPSIG(status), strsignal(WSTOPSIG(status)));
 } else if (WIFCONTINUED(status)) {
 printf("child continued\n");
 } else { /* Should never happen */
 printf("what happened to this child? (status=%x)\n",
 (unsigned int) status);
 }
}

int
main(int argc, char *argv[])
{
 int status;
 pid_t childPid;

 if (argc > 1 && strcmp(argv[1], "--help") == 0)
 usageErr("%s [exit-status]\n", argv[0]);
 switch (fork()) {
 case -1: errExit("fork");
 case 0: /* Child: either exits immediately with given
 status or loops waiting for signals */
 printf("Child started with PID = %ld\n", (long) getpid());
 if (argc > 1) /* Status supplied on command line? */
 exit(getInt(argv[1], 0, "exit-status"));
 else /* Otherwise, wait for signals */
 for (;;)
 pause();
 exit(EXIT_FAILURE); /* Not reached, but good practice */
 default: /* Parent: repeatedly wait on child until it
 either exits or is terminated by a signal */
 for (;;) {
 childPid = waitpid(-1, &status, WUNTRACED | WCONTINUED
 );
 if (childPid == -1)
 errExit("waitpid");
 /* Print status in hex, and as separate decimal bytes */
 printf("waitpid() returned: PID=%ld; status=0x%04x (%d,%d)\n",
 (long) childPid,
 (unsigned int) status, status >> 8, status & 0xff);
 printWaitStatus(NULL, status);
 if (WIFEXITED(status) || WIFSIGNALED(status))
 exit(EXIT_SUCCESS);
 }
 }
}
