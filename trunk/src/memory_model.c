
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "memory_model.h"
#include "list.h"

//
// Runs through memory looking for adjacent free blocks
// which can be merged in one larger block.
//
void merge_blocks (Memory *mem);

//
// Compares two blocks.  The block with lesser beginning address
// is lesser.
//
// Returns a negative integer if the first block is lesser,
// a positive is the greater and zero if blocks's beginning addresses
// are the same.
//
int block_cmp (const void *a, const void *b)
{
  int a_beg = ((Block*) a)->address.beg;
  int a_seg = ((Block*) a)->address.segment_no;
  int b_beg = ((Block*) b)->address.beg;
  int b_seg = ((Block*) b)->address.segment_no;

  if (a_seg == b_seg) {
    return a_beg - b_beg;
  } else {
    return a_seg - b_seg;
  }
}

Memory *memory_new (size_t size)
{
  #ifndef NDEBUG 
  fputs ("memory_new()\n", stderr);
  #endif

  // Create the memory structure.
  Memory *mem = malloc (sizeof (Memory));

  if (!mem) 
    return NULL;
 
  // Create the memory's list of free blocks.
  mem->free_blocks = list_new ();

  // Create the list of used blocks (empty at first).
  mem->used_blocks = list_new ();

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
  seg->size = size;
  list_cons (mem->segments, seg, sizeof (Segment));
  free (seg);

  // Set up the initial block.
  block->memory = mem;
  block->size = size;
  block->address.segment_no = 0;
  block->address.beg = 0;
  // Add that block to the free_blocks list.
  list_cons (mem->free_blocks, block, sizeof (Block));
  free (block);

  // Setting up operations avaliable for a user.
  mem->ops.alloc = memory_alloc;
  mem->ops.free = memory_free;

  return mem;
}

void memory_delete (Memory *mem)
{
  #ifndef NDEBUG 
  fputs ("memory_delete()\n", stderr);
  #endif

  list_free (mem->free_blocks);

  list_free (mem->used_blocks);

  if (!list_is_empty (mem->pointers)) {
    // Free memory occupied by blocks in the list of pointers.
    ListNode *node = mem->pointers->head->next;
    while (node != mem->pointers->head) {
      ListNode *next = node->next;
      free (((Pointer*)(node->data))->__block);
      node = next;
    }
    free (((Pointer*)(node->data))->__block);
  }

  list_free (mem->pointers);

  if (!list_is_empty (mem->segments)) {
    // Free memory occupied by segments.
    ListNode *node = mem->segments->head->next;
    while (node != mem->segments->head) {
      ListNode *next = node->next;
      free (((Segment*)(node->data))->data);
      node = next;
    }
    free (((Segment*)(node->data))->data);
  }

  list_free (mem->segments);

  free (mem);
}

Pointer* memory_alloc (Memory *mem, size_t size)
{
  #ifndef NDEBUG 
  fputs ("memory_alloc()\n", stderr);
  #endif

  // A rude "first fit" approach, no optimizations yet.

  // Find the first free block of the appropriate size.
  // Split it if it is larger than size, and add one size to the pointers
  // list and return Pointer* to it.  Add the other part (if there is any)
  // back to the free_blocks list.
  
  // List node that will contain the first fittable block of memory
  // to allocate.
  ListNode *cand = mem->free_blocks->head;
  
  // There is no free blocks in memory.
  if (!cand) {
    // puts ("Can't allocate memory for another element!");
    return NULL;
  }
  
  if (((Block*)(cand->data))->size < size) {
    cand = cand->next;
    while (((Block*)(cand->data))->size < size && cand != mem->free_blocks->head) {
      cand = cand->next;
    }
  }
  
  // The suitable block has found...
  if (((Block*)(cand->data))->size >= size) {

    // The first fittable block to allocate.
    Block *block_cand = (Block* )cand->data;

    // Split the block if it's size is larger than needed
    // and return the second part of it to the Memory.
    if (block_cand->size > size) {

      // The part of block which will be remained in the memory.
      Block *block_remain = malloc (sizeof (Block));
      // The part of block which will be returned to the user.
      Block *block_return = malloc (sizeof (Block));
      
      if (!block_remain || !block_return) {
        // TODO: handle this error
        fputs ("memory_alloc(): malloc() error.", stderr);
        return NULL;
      }
      
      // Size of the block that should be returned to the Memory.
      size_t size_rem = block_cand->size - size;

      block_remain->memory = mem;
      block_remain->address.beg = block_cand->address.beg + size;
      block_remain->address.segment_no = block_cand->address.segment_no;
      block_remain->size = size_rem;

      memcpy (block_return, block_cand, sizeof (Block));
      block_return->size = size;
      
      // Remove the first fittable block from the list of free blocks.
      list_erase (mem->free_blocks, cand);
 
      // Add the remaining part of the block to the list of free blocks.
      if (!list_insert_in_order (mem->free_blocks, block_remain, 
                                 sizeof (Block), block_cmp)) {
        fputs ("memory_alloc(): error", stderr);
        return NULL;
      }

      free (block_remain);

      // Add pointer to the returning block to the list of pointers.
      Pointer *p = malloc (sizeof (Pointer));
      assert (p);
      
      ListNode *block_ins_p = 
        list_insert_in_order (mem->used_blocks, block_return, 
                              sizeof (Block), block_cmp);
      
      free (block_return);
      
      if (!block_ins_p) {
        // TODO
      }

      p->__block = (Block* )block_ins_p->data;
      p->data = ((Segment*)((list_kth (mem->segments, block_return->address.segment_no))->data))->data + block_return->address.beg;

      Pointer *p_return = list_cons (mem->pointers, p, sizeof (Pointer))->data;
      free (p);

      return p_return;
    } else {
      // The size of the fittable block is exactly the size demanded by user.

      ListNode *block_ins_p = 
        list_insert_in_order (mem->used_blocks, 
                              block_cand, sizeof (Block), block_cmp);
      if (!block_ins_p) {
        // TODO
      }

      // Remove the fittable block from the list of free blocks.
      list_erase (mem->free_blocks, cand);

      // Add the pointer to the block to the list of pointers.
      Pointer *p = malloc (sizeof (Pointer));
      // TODO: handle malloc
      p->__block = (Block* )block_ins_p->data;
      p->data = ((Segment*)((list_kth (mem->segments, p->__block->address.segment_no))->data))->data + p->__block->address.beg;

      Pointer *p_return = 
        list_insert_in_order (mem->pointers, p, 
                              sizeof (Pointer), block_cmp)->data;

      free (p);
        
      return p_return;
    }
  } else {
    // Can't allocate enough space.
    return NULL;
  }
}

void memory_free (Memory *mem, Pointer* p)
{
  #ifndef NDEBUG 
  fputs ("memory_free()\n", stderr);
  #endif
  
  // Look for a pointer p in the list of pointers.
  ListNode *s = list_lookup (mem->pointers, p, sizeof (Pointer));

  // Add the block at which the pointer points to the list of free blocks. 
  list_insert_in_order (mem->free_blocks, p->__block, 
                        sizeof (Block), block_cmp);

  // Remove the block from the list of used blocks.
  list_erase (mem->used_blocks, 
              list_lookup (mem->used_blocks, p->__block, sizeof (Block)));

  // Look for adjacent blocks to merge them.
  merge_blocks (mem);

  if (s) {
    list_erase (mem->pointers, s);
  }
  else {
    // TODO
  }
}

void memory_dump (Memory *mem)
{
  size_t s_num = list_size (mem->segments);
  size_t fb_num = list_size (mem->free_blocks);
  size_t ub_num = list_size (mem->used_blocks);
  fprintf (stderr, "*** Memory dump ***\nTotal: %u segments, %u free blocks, "
           "%u used blocks.\n", s_num, fb_num, ub_num);

  if (!mem->segments)
    return;
  ListNode *sp = mem->segments->head;
  for (int i = 0; i < s_num; ++i) {
    fprintf (stderr, "Segment #%d, size: %u\n", i,
             ((Segment*) sp->data)->size);

    ListNode *free_p = mem->free_blocks->head;
    ListNode *used_p = mem->used_blocks->head;

    if (!free_p && !used_p) {
      return;
    }

    int free_cnt = 0, used_cnt = 0;
    while (free_cnt < fb_num || used_cnt < ub_num) {

      if (used_cnt >= ub_num || (free_cnt < fb_num && block_cmp (free_p->data, used_p->data) < 0)) {

        Block *b = free_p->data;
        fprintf (stderr, "  (FREE SPACE) size: %-7u [%7u, %7u]\n", b->size, 
                 b->address.beg, b->size + b->address.beg);
        free_p = free_p->next;
        free_cnt++;
      } 
      else {

        Block *b = used_p->data;        
        fprintf (stderr, "X (USED SPACE) size: %-7u [%7u, %7u]\n", b->size, 
                 b->address.beg, b->size + b->address.beg);
        used_p = used_p->next;
        used_cnt++;
      }
    }
  }

  sp = sp->next;
}
  
void merge_blocks (Memory *mem)
{
  #ifndef NDEBUG
  fputs ("merge_block()\n", stderr);
  #endif
  
  ListNode *bp = mem->free_blocks->head;

  if (!bp)
    return;

  int back_step = 0;

  do {
    ListNode *nbp = bp->next;
    Block *b = (Block* )bp->data;
    Block *nb = (Block* )nbp->data;

    ListNode *next_node = nbp;

    if (b->address.segment_no == nb->address.segment_no &&
        b->address.beg + b->size == nb->address.beg) {
      // These are blocks to be merged.
      Block *new_block = malloc (sizeof (Block));
      
      if (!new_block) {
        // TODO
      }

      new_block->address.segment_no = b->address.segment_no;
      new_block->address.beg = b->address.beg;
      new_block->memory = b->memory;
      new_block->size = b->size + nb->size;

      ListNode *node_inserted = 
        list_insert_in_order (mem->free_blocks, new_block, 
                              sizeof (Block), block_cmp);

      list_erase (mem->free_blocks, bp);
      list_erase (mem->free_blocks, nbp);

      bp = node_inserted;

      free (new_block);

      back_step = 1;
    } else {
      bp = next_node;
      back_step = 0;
    }
  } while (back_step || bp != mem->free_blocks->head);

}
