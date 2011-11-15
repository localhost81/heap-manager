/*
 * Memory manager.
 *
 * A driver for the memory managment library.
 */

#include <stdlib.h>
#include <stdio.h>
#include "memory_model.h"

const int x = 42;

int main()
{
  Memory *mem = memory_new (1000);
  printf ("%d\n", list_size ((mem->free_blocks)));
  
  pPointer p1 = memory_alloc (mem, 100);
  if (!p1) {
    puts ("Can't allocate enough memory!");
    return 1;
  } 
  
  printf ("%d\n", list_size ((mem->free_blocks)));

  pPointer p2 = memory_alloc (mem, 900);
  if (!p2) {
    puts ("Can't allocate enough memory!");
    return 1;
  }

  memcpy (p1->data, &x, sizeof (int));
  printf ("%d\n", p1->__block->size);
  printf ("THE NUMBER IS %d\n", *(int*)(p1->data));
  
  printf ("%d\n", list_size (mem->free_blocks));  
  memory_free (mem, &p1);
  memory_free (mem, &p2);

  memory_delete (mem);

  return 0;
}

