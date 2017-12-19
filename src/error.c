#include <stdio.h>
#include <stdlib.h>
/* #include <unistd.h> */
#include <errno.h>
#include <string.h>

#include "error.h"

void
_error (char *msg, const char *src_file, const char *function, int line)
{
  fprintf (stderr, "Error: [%s] %s:%d => %s: (%d)%s\n",
	   src_file, function, line, msg, errno, strerror(errno));
  exit (EXIT_FAILURE);
}
