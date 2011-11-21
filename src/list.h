//
// "list.h"
//
// Implements the dobule linked circular list data structure.
// List is said to be empty if it's field 'head' equals NULL 
// and 'size' equals 0.
//
// Author: Dmitry Borody
// Date: 11.2011
//

#ifndef MEM_MANAGER_LIST_H
#define MEM_MANAGER_LIST_H

#include <stdlib.h>

// Node of a double-linked circular list.
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

// List data structure.
struct List {
  ListNode* head;
  size_t size;
};
typedef struct List List;

// 
// Creates an new empty list.
//
// Returns the pointer to the head of created list if the 
// creation was successful or NULL otherwise.
// 
List *list_new ();

//
// Deletes a list, frees the memory.
//
void list_free (List *l);

//
// Inserts a data of a given size to the beginning of a given list.
//
// Returns a pointer to the consed node (i.e. list's new head) if the 
// insertion was successful and NULL otherwise.
//
ListNode *list_cons (List *l, void *data, size_t size);

//
// Inserts a data of a given size to the end of a given list.
//
// Returns a pointer to the inserted node if the insertion was
// successful and NULL otherwise.
//
ListNode *list_append (List *l, void *data, size_t size);
//
// Inserts a data of a given size before the node.  If the *node points is
// NULL, the data is consed (i.e. list_cons() is called) to the list.
//
// Returns a pointer to the inserted node if the insertion was
// successful and NULL otherwise.
//
ListNode *list_insert (List *l, ListNode *node, void *data, size_t size);

//
// Assumes that list is ordered (increasing) according to the 
// function cmp, which specification is the same as in the 'compar' 
// function that used in qsort() from stdlib.h.
//
// Inserts the element to the list. The position for the element
// is chosen so that the list elements remain ordered.
//
// Returns a pointer to the inserted node if the insertion was 
// successful and NULL otherwise.
//
ListNode *list_insert_in_order (List *l, void *data, size_t size, 
                                int (*cmp)(const void *a, const void *b));

//
// Removes the node from the list.
//
// Returns the pointer to the next node after removed one or 
// NULL if there is no more nodes in the list or the given node is NULL.
//
ListNode *list_erase (List *l, ListNode *node);

//
// Returns a pointer to the k-th element in the list.
//
ListNode *list_kth (List *l, size_t k);

//
// Returns a pointer to the node whose content equals to the specified
// data. If there is no such element, returns NULL.
//
// Assumes that the data and each node in the list is size bytes long.
//
ListNode *list_lookup (List *l, const void *data, size_t size);

//
// Returns 1 if the list is empty and 0 otherwise.
//
int list_is_empty (const List *l);

//
// Returns the size of the list.
//
size_t list_size (const List *l);

#endif // #ifndef MEM_MANAGER_LIST_H
