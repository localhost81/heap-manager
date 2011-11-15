//
// "list.h"
//
// Implements the dobule linked circular list data structure.
//
// Author: Dmitry Borody
// Date: 11.2011
//

#ifndef MEM_MANAGER_LIST_H
#define MEM_MANAGER_LIST_H

#include <stdlib.h>

struct ListNode {
  // pointer to the next node in the list.
  struct ListNode *next;
  // pointer to the previous node in the list.
  struct ListNode *prev;
  // data
  void *data;
  size_t size;
};
typedef struct ListNode ListNode;

// 
// Creates an new empty list.
//
// Returns the pointer to the head of created list if the 
// creation was successful or NULL otherwise.
// 
struct ListNode *list_new ();

//
// Deletes a list, frees the memory.
//
void list_free (ListNode *node);

//
// Inserts the *data of specified size before given list node.
//
// Returns the pointer to the inserted node if the insertion was successful
// and NULL otherwise.
//
ListNode *list_insert (ListNode **node, void *data, size_t size);

//
// Removes the node from the list.
//
// Returns the pointer to the next node after removed one or 
// NULL if there is no more nodes in the list.
//
ListNode *list_erase (ListNode **node);

//
// Returns a pointer to the k-th element in the list.
//
ListNode *list_kth (ListNode *l, size_t k);

//
// Returns a pointer to the node whose content equals to the specified
// data. If there is no such element, returns NULL.
//
// Assumes that the data and each node in the list is size bytes long.
//
ListNode *list_lookup (ListNode *l, const void *data, size_t size);

size_t list_size (ListNode *h);

#endif // #ifndef MEM_MANAGER_LIST_H
