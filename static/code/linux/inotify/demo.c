#include <sys/inotify.h>
#include <limits.h>
#include "tlpi_hdr.h"
static void /* Display information from inotify_event structure */
displayInotifyEvent(struct inotify_event *i)
{
 printf(" wd =%2d; ", i->wd);
 if (i->cookie > 0)
 printf("cookie =%4d; ", i->cookie);
 printf("mask = ");
 if (i->mask & IN_ACCESS) printf("IN_ACCESS ");
 if (i->mask & IN_ATTRIB) p

// TODO 382 