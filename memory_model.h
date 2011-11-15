//
//  "memory_model.h"
//  
//  Contains declarations of data types that used
//  in the memory model (Block, Pointer) and data 
//  structures to maintain them.  
//  
//  The data structure is a list of free blocks, and each block is 
//  a list of segments.
//
//  Author: Dmitry Borody
//  Date: 11.2011
//

#ifndef MEM_MANAGER_MEMORY_MODEL_H
#define MEM_MANAGER_MEMORY_MODEL_H

#include "types.h"
#include "list.h"

struct Block;
typedef struct Block Block;
struct BlockAddress;
typedef struct BlockAddress BlockAddress;
struct Memory;
typedef struct Memory Memory;
struct Pointer;
typedef struct Pointer Pointer;
struct Segment;
typedef struct Segment Segment;

// Every pointer that a user creates stores in the Memory's
// pointers list.
struct Pointer {
  // pointer to Block to which the Pointer points.
  Block *__block;
  // The memory location to which the Pointer points.
  void *data;
};

// pPointer is what a user gets when he calls memory_alloc() and 
// memory_realloc() functions.
typedef Pointer* pPointer;

// The address of the block, that issegment's number 
// and the address inside the block.
struct BlockAddress {
  // The number of segment to which the block belongs, counted from zero.
  addr_t segment_no;
  // The address (i.e. the number of the first byte, counted from zero)
  // of the block's beginning inside the segment.
  addr_t beg;
};

struct Block {
  // The Memory to which the block belongs.
  Memory *memory;
  // The address of the Block.
  BlockAddress address;
  // The block's size.
  size_t size;
};

struct Memory {
  // The list of free blocks.
  ListNode *free_blocks;
  // The list of pointers (to the memory blocks) that the 
  // client creates.
  ListNode *pointers;
  // Segments of memory in which the data is actually stored.
  ListNode *segments;
};

struct Segment {
  void *data;
};

//
// Creates a new Memory that consists of one segment of
// the given size and initialises it.
// 
// Returns the pointer to the newly created Memory.
//
Memory *memory_new (size_t size);

//
// Deletes the Memory to which the pointer points and frees memory.
//
void memory_delete (Memory *mem);

// 
// Allocates specified amount of bytes in the given Memory. 
//
// Returns a pointer to (sorry for a pun) Pointer to the allocated space
// in Memory if the allocation was successful.  If it was not, 
// returns NULL.
//
pPointer memory_alloc (Memory *memory, size_t bytes);

//
// Frees the memory segment in Memory to which the pointer points.
//
void memory_free (Memory *memory, pPointer pointer);

#endif // #ifndef MEM_MANAGER_MEMORY_MODEL_H
