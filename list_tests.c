
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "test.h"
#include "list.h"

#define N_TESTS 2

void str_test ();
void empty_list_free_test ();

const void (*tests[N_TESTS])(void) = {str_test, empty_list_free_test};

int main()
{
  int i;
  for (i = 1; i < N_TESTS; ++i) {
    puts ("====================================================");
    printf ("Running test #%d.\n", i + 1);

    float start = clock();

    (*(tests[i]))();
    
    printf ("Test is finished in %f s.\n",
            (clock () - start) / CLOCKS_PER_SEC);
  }

  return 0;
}

void empty_list_free_test ()
{
  ListNode *l = list_new ();
  list_free (l);
}

void str_test ()
{
  FILE *str_in = fopen ("str_test.in", "r");
  
  int n;
  fscanf (str_in, "%d\n", &n);
  printf ("The test size is %d.\n", n);
  int i;

  ListNode *l = list_new ();
  
  for (i = 0; i < n; ++i) {
    char line[256];
    fgets (line, 256, str_in);
    list_insert (&l, line, strlen(line) + 1);
  }

  list_free (l);

  close (str_in);
  free (str_in);
}
