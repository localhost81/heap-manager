/*
 * A driver for the the heap manager.
 */

#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "memory_model.h"

void list_utest ();
void memory_utest ();

typedef char Item;

int main()
{
  memory_utest ();
  //list_utest ();

  return 0;
}

void ilist_print (List *l)
{
  putchar ('[');
  ListNode *node = l->head;
  for (int i = 0; i < l->size; ++i) {
    printf ("%d%s", *(int*)(node->data), (i == l->size - 1? "]" : ", "));
    node = node->next;
  }
  if (!l->size)
    putchar (']');
}

void dump_list (List *l)
{
  ListNode *node = l->head;
  for (int i = 0; i < l->size; ++i) {
    printf ("%d ", *(int*)node->data);
    node = node->next;
  }
  putchar ('\n');
}

void memory_utest ()
{
  const int N = 7;

  Pointer *pts[N];

  Memory *mem = memory_new (sizeof (Item) * (N));

  // Allocate memory for a few chars.
  for (int i = 0; i < N; ++i) {
    char a = 'a';
    pts[i] = mem->ops.alloc (mem, sizeof (Item));
    if (pts[i])
      memcpy (pts[i]->data, &a, sizeof (Item));
    else 
      puts ("Can't allocate memory for a char.");
    memory_dump (mem);
    puts ("");
  }

  // Freesome memory, check that adjacent blocks 
  // are properly merged.
  mem->ops.free (mem, pts[3]);
  memory_dump (mem);
  mem->ops.free (mem, pts[4]);
  memory_dump (mem);
  mem->ops.free (mem, pts[5]);
  memory_dump (mem);

  // Free the remaining memory.
  for (int i = 0; i < N; ++i) {
    if (i < 3 || i > 5) {
      //printf ("%d\n", *(Item*)(pts[i]->data));
      mem->ops.free (mem, pts[i]);
      memory_dump (mem);
      puts ("");
    }
  }

  memory_delete (mem);
}

int cmp (const void *a, const void *b)
{
  return *(int*)a - *(int*)b;
}

void list_utest ()
{
  List *l = list_new ();

  assert (list_is_empty (l));

  int n = 30;
  for (int i = 0; i < n; ++i) {
    int t = i % 10;
    list_insert_in_order (l, &t, sizeof (t), cmp);
    dump_list (l);
  }

  assert (!list_is_empty (l));
  assert (list_size (l) == n);

  dump_list (l);

  list_free (l);
}
