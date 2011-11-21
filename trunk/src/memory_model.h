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

// The address of the block, that issegment's number 
// and the address inside the block.
struct BlockAddress {
  // The number of segment to which the block belongs, counted from zero.
  size_t segment_no;
  // The address (i.e. the number of the first byte, counted from zero)
  // of the block's beginning inside the segment.
  size_t beg;
};

struct Block {
  // The Memory to which the block belongs.
  Memory *memory;
  // The address of the Block.
  BlockAddress address;
  // The block's size.
  size_t size;
};

// Operations available for a user

struct Memory {
  // The list of free blocks.
  List *free_blocks;
  // Teh list of used blocks.
  List *used_blocks;
  // The list of pointers (to the memory blocks) that the 
  // client creates.
  List *pointers;
  // Segments of memory in which the data is actually stored.
  List *segments;

  // Operations available for a user.
  struct {
    Pointer* (*alloc)(Memory *, size_t);
    void (*free) (Memory *, Pointer *);
  } ops;
};

struct Segment {
  void *data;
  size_t size;
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
// Returns a pointer to Pointer to the allocated space
// in Memory if the allocation was successful.  If it was not, 
// returns NULL.
//
Pointer* memory_alloc (Memory *memory, size_t bytes);

//
// Frees the memory segment in Memory to which the pointer points.
//
void memory_free (Memory *memory, Pointer* pointer);

//
// Prints information like how many segments and blocks are used,
// how much free space there is and how it is distributed, etc.
//
void memory_dump (Memory *mem);

#endif // #ifndef MEM_MANAGER_MEMORY_MODEL_H
