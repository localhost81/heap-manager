
#include <string.h>
#include <stdio.h>
#include "memory_model.h"
#include "list.h"

Memory *memory_new (size_t size)
{
  // Create the memory structure.
  Memory *mem = malloc (sizeof (Memory));

  if (!mem) 
    return NULL;
 
  // Create the memory's list of free blocks.
  mem->free_blocks = list_new ();

  // Create the list of user's pointers.
  // (empty at first)
  mem->pointers = list_new ();

  // Create the list of segments.
  mem->segments = list_new ();

  // Allocate the initial memory segment.
  void *memseg = malloc (size);

  // Create the initial Segment.
  Segment *seg = malloc (sizeof (Segment));

  // Create the initial Block.
  Block *block = malloc (sizeof (Block));

  // Allocation check...
  if (!memseg || !seg || !block) {
    list_free (mem->pointers);
    list_free (mem->segments);
    list_free (mem->free_blocks);
    free (memseg);
    free (block);
    free (seg);
    free (mem);

    return NULL;
  }

  // Set up the initial segment.
  seg->data = memseg;
  list_insert (&(mem->segments), seg, sizeof (Segment));
  free (seg);

  // Set up the initial block.
  block->memory = mem;
  block->size = size;
  block->address.segment_no = 0;
  block->address.beg = 0;
  // Add that block to the free_blocks list.
  list_insert (&(mem->free_blocks), block, sizeof (Block));
  free (block);

  return mem;
}

void memory_delete (Memory *mem)
{
  //fputs ("memory_delete()\n", stderr);

  //  printf ("size(free blocks) = %d\n", list_size (mem->segments));
  list_free (mem->free_blocks);
  list_free (mem->pointers);

  // Free memory occupied by each segment.
  ListNode *n = mem->segments;
  do {
    //  printf (">del>>  %p %p [%p] %p %p\n", mem->segments->prev->prev, mem->segments->prev, mem->segments, mem->segments->next, mem->segments->next->next);
    //    printf ("%p\n", n->data);
    ListNode *next = n->next;
    free (((Segment*)(n->data))->data);
    n = next;
  } while (n != mem->segments);

  list_free (mem->segments);

  free (mem);
}

pPointer memory_alloc (Memory *mem, size_t size)
{
  // fputs ("memory_alloc()\n", stderr);
  // A rude "first fit" approach, no optimizations yet.

  // Find the first free block of the appropriate size.
  // Split it if it is larger than size, and add one size to the pointers
  // list and return pPointer to it.  Add the other part (if there is any)
  // back to the free_blocks list.
  
  ListNode *h = mem->free_blocks;
  if (((Block*)(h->data))->size < size) {
    h = h->next;
    while (((Block*)(h->data))->size < size && h != mem->free_blocks) {
      h = h->next;
      //  printf ("%p\n", h);
    }
  }
  
  // The suitable block has found...
  if (((Block*)(h->data))->size >= size) {

    Block *b = h->data;

    // Split the block if it's size is larger than needed
    // and return the second part of it to the Memory.
    if (b->size > size) {
      //puts ("Splitting...");

      Block *bt1 = malloc (sizeof (Block)),
        *bt2 = malloc (sizeof (Block));
      
      if (!bt1 || !bt2) {
        // TODO: handle this error
        fputs ("memory_alloc(): !bt1 || !bt2", stderr);
        return NULL;
      }
      
      // Size of the block that should be returned to the Memory.
      size_t size_rem = b->size - size;

      bt1->memory = mem;
      bt1->address.beg = b->address.beg + size;
      bt1->address.segment_no = b->address.segment_no;
      bt1->size = size_rem;

      memcpy (bt2, b, sizeof (Block));
      bt2->size = size;
 
      ListNode *t;
      t = list_insert (&h, bt1, sizeof (Block));
      if (!t) {
        fputs ("memory_alloc(): !t", stderr);
        return NULL;
      }
      t = list_insert (&h, bt2, sizeof (Block));
      mem->free_blocks = list_erase (&h);
      if (!t) {
        fputs ("memory_alloc(): !t", stderr);
        return NULL;
      }
      //puts ("OK");

      h = t;
      b = h->data;

      free (bt1);
      free (bt2);
    } 

    // Add Pointer to the block into the mem->pointers
    // and return pointer to it to the user.
    Pointer *p = malloc (sizeof (Pointer));
    if (!p) {
      fputs ("memory_alloc(): !p", stderr);
      // TODO: handle this error.
      return NULL;
    }
    // Set up the Pointer's fields.

    p->__block = b;
    p->data = ((Segment*)((list_kth (mem->segments, b->address.segment_no))->data))->data + b->address.beg;
    // Remove the allocated block from the mem->free_blocks list.
    mem->free_blocks = list_erase (&h);
    // Insert the Pointer to the block to the mem->pointers list.
    Pointer *r = list_insert (&(mem->pointers), p, sizeof (Pointer))->data;

    free (p);

    return r;
  } else {
    // Can't allocate enough space.
    return NULL;
  }
}

void memory_free (Memory *mem, pPointer p)
{
  // Remove the pointer p beg mem->pointers,
  // add the block at which p has been pointed to the mem->free_blocks.

  // puts ("memory_free()");
  ListNode *s = list_lookup (mem->pointers, p, sizeof (Pointer));
  
  if (s)
    list_erase (&s);
  else {
    // TODO: handle this error.
  }

  list_insert (&(mem->free_blocks), p->__block, sizeof (Block));
}
