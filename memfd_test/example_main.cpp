#include "memfd.h"


int
main(void)
{
   const char* message="ACGT";
   int fd = new_memfd_region(message, "dummy");

   printf("PID: %d; fd: %d; /proc/%d/fd/%d\n",
           (int) getpid(), fd, (int) getpid(), fd);

   /* Keep running, so that the file created by memfd_create()
      continues to exist */
   pause();

   close(fd);

   exit(EXIT_SUCCESS);
}
