
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "list.h"

struct List *list_new ()
{
  List *l = malloc (sizeof (List));

  l->head = NULL;
  l->size = 0;

  return l;
}

size_t list_size (const List *l)
{
  return l->size;
}

int list_is_empty (const List *l)
{
  assert ((l->size == 0) == (l->head == NULL));
  return l->size == 0;
}

void list_free (List *l)
{
  #ifndef NDEBUG
  fputs ("list_free()\n", stderr);
  #endif

  if (list_is_empty (l)) {
    free (l);
    return;
  }
  
  ListNode *node = l->head->next;
  while (node != l->head) {
    free (node->data);

    ListNode *next = node->next;
    
    free (node);

    node = next;
  }

  free (l->head->data);
  free (l->head);
  free (l);
}


ListNode *list_erase (List *l, ListNode *node)
{
  #ifndef NDEBUG
  fputs ("list_erase()\n", stderr);
  #endif

  if (!node) {
    return NULL;
  }

  free (node->data);

  // The node being deleted is the list's head.
  if (l->head == node) {

    // The node is the only node in the list.
    if (list_size (l) == 1) {

      free (l->head);
      l->size--;

      return l->head = NULL;
    } else {

      ListNode *next = l->head->next;
      next->prev = l->head->prev;
      l->head->prev->next = next;
      free (node);
      l->size--;

      return l->head = next;
    }

  } else {
    // The node is not the list's head.
    
    node->next->prev = node->prev;
    node->prev->next = node->next;

    ListNode *next = node->next;

    free (node);
    
    l->size--;
    
    return next;
  }
}

ListNode *list_cons (List *l, void *data, size_t size)
{
  #ifndef NDEBUG
  fputs ("list_cons()\n", stderr);
  #endif

  if (list_is_empty (l)) {
    ListNode *new_node = malloc (sizeof (ListNode));

    new_node->data = malloc (size);
    if (!new_node->data) {
      free (new_node);
      return NULL;
    }
    
    memcpy (new_node->data, data, size);
    new_node->size = size;

    l->size++;

    return l->head = new_node->next = new_node->prev = new_node;
  } else {
 
    ListNode *new_node = malloc (sizeof (ListNode));

    if (!new_node) {
      // TODO
    }

    l->head->prev->next = new_node;
    new_node->prev = l->head->prev;
    new_node->next = l->head;
    l->head->prev = new_node;

    new_node->data = malloc (size);
    if (!new_node->data) {
      free (new_node);
      return NULL;
    }

    memcpy (new_node->data, data, size);
    new_node->size = size;

    l->size++;
    
    return l->head = new_node;
  }
}

ListNode *list_append (List *l, void *data, size_t size)
{
  #ifndef NDEBUG
  fputs ("list_append()\n", stderr);
  #endif

  if (l->head) {
    ListNode *new_node = malloc (sizeof (ListNode));

    if (!new_node) {
      // TODO
    }
    
    new_node->next = l->head;
    new_node->prev = l->head->prev;
    l->head->prev->next = new_node;
    l->head->prev = new_node;

    new_node->data = malloc (size);
    if (!new_node->data) {
      //TODO
    }
    
    memcpy (new_node->data, data, size);
    new_node->size = size;

    l->size++;

    return new_node;
  }

  return list_cons (l, data, size);
}

ListNode *list_insert (List *l, ListNode *node, void *data, size_t size)
{
  #ifndef NDEBUG
  fputs ("list_insert()\n", stderr);
  #endif

  if (!l->head || !node || node == l->head) {
    return list_cons (l, data, size);
  }

  ListNode *new_node = malloc (sizeof (ListNode));

  if (!new_node) {
    // TODO
  }

  new_node->next = node;
  new_node->prev = node->prev;
  node->prev->next = new_node;
  node->prev = new_node;

  if (!(new_node->data = malloc (size))) {
    // TODO
    free (new_node);
    return NULL;
  }

  memcpy (new_node->data, data, size);
  new_node->size = size;

  l->size++;

  return new_node;
}

ListNode *list_insert_in_order (List *l, void *data, size_t size,
                                int (*cmp) (const void *, const void *))
{
  #ifndef NDEBUG
  fputs ("list_insert_in_order()\n", stderr);
  #endif

  if (list_is_empty (l)) {
    return list_cons (l, data, size);
  }

  if ((*cmp)(l->head->data, data) > 0) {
    return list_cons (l, data, size);
  }

  ListNode *node = l->head;
  while ((*cmp)(node->data, data) < 0) {
    node = node->next;
    if (node == l->head) {
      break;
    }
  }

  if ((*cmp)(node->data, data) < 0) {
    return list_append (l, data, size);
  }

  return list_insert (l, node, data, size);
}

ListNode *list_kth (List *l, size_t k)
{
  if (list_is_empty (l)) {
    return NULL;
  }

  ListNode *node = l->head;
  while (k--) {
    node = node->next;
  }

  return node;
}

ListNode *list_lookup (List *l, const void *data, size_t size)
{
  #ifndef NDEBUG
  fputs ("list_lookup()\n", stderr);
  #endif
  
  ListNode *node = l->head;

  do {
    if (!memcmp (node->data, data, size))
      return node;
    node = node->next;
  } while (node != l->head);
  
  return NULL;
}
