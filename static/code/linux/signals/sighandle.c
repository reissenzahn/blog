
#include <signal.h>

void handler(int sig) {
 printf("ouch!\n");  // unsafe
}

void handler2(int sig) {

}

int main(int argc, char *argv[]) {

  // establish a handler for hte SIGINT signal (generated b Control-C)
  if (signal(SIGINT, sigHandler) == SIG_ERR) {
    fprintf(stderr, "signal: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  int i = 0;
  while (true) {
    printf("%d\n", i);
    sleep(1);
    i++;
  }


}

static void
sigHandler(int sig)
{
 static int count = 0;
 /* UNSAFE: This handler uses non-async-signal-safe functions
 (printf(), exit(); see Section 21.1.2) */
 if (sig == SIGINT) {
 count++;
 printf("Caught SIGINT (%d)\n", count);
 return; /* Resume execution at point of interruption */
 }
 /* Must be SIGQUIT - print a message and terminate the process */
 printf("Caught SIGQUIT - that's all folks!\n");
 exit(EXIT_SUCCESS);
}
int
main(int argc, char *argv[])
{
 /* Establish same handler for SIGINT and SIGQUIT */
 if (signal(SIGINT, sigHandler) == SIG_ERR)
 errExit("signal");
 if (signal(SIGQUIT, sigHandler) == SIG_ERR)
 errExit("signal");
 for (;;) /* Loop forever, waiting for signals */
 pause(); /* Block until a signal is caught */
}
