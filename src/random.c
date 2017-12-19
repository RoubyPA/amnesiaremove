#include <stdlib.h>
#include <limits.h>
#include <time.h>

#include "random.h"

/* Get a new random char */
char
get_random_char ()
{
  static float tmp;
  srand (time(NULL) + rand ()); /* change seed */
  tmp = (float)rand () / (float)RAND_MAX; /* random number between 0 and 1 */ 

  return ((char)(tmp * CHAR_MAX));
}
