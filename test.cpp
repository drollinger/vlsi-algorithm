/* srand example */
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

int main ()
{
  srand(time(NULL));
  printf ("Random number: %f\n", (rand()/(float)RAND_MAX));
  return 0;
}
