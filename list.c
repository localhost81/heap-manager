
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "list.h"

struct ListNode *list_new ()
{
  ListNode *list = malloc (sizeof (ListNode*));
  
  return list = NULL;
}

void list_free (ListNode *node)
{
  //  printf ("list_free(): %p <- [%p] -> %p\n", node->prev, node, node->next);
  
  if (!node)
    return;

  // The list is empty.
  if (!node->next) {
    free (node);
    return;
  }
  
  ListNode *h = node->next;
  while (h != node) {
    //printf ("%p %p\n", h, h->data);
    free (h->data);

    ListNode *next = h->next;

    free (h);

    h = next;
  }

  if (node->data)
    free (node->data);

  free (node);
}

ListNode *list_erase (ListNode **node)
{
  //  puts ("list_erase()");
  //printf ("%p\n", *node);
  
  if (!(*node)) 
    return NULL;

  //printf (">>>  %p %p [%p] %p %p\n", (*node)->prev->prev, (*node)->prev, (*node), (*node)->next, (*node)->next->next);
 
  // If there is more than one node in the list
  // change the links in the next and previous nodes.
  if ((*node)->next != *node) {
    (*node)->next->prev = (*node)->prev;
    (*node)->prev->next = (*node)->next;
  } 
  
  free ((*node)->data);

  if ((*node)->next == *node) {
    (*node)->prev = (*node)->next = NULL;
    return *node = NULL;
  }

  ListNode *next_node = (*node)->next;
  //printf ("next_node: %p\n", next_node);

  *node = next_node;
  //printf ("result>>>  %p %p [%p] %p %p\n", (*node)->prev->prev, (*node)->prev, (*node), (*node)->next, (*node)->next->next);
   
  return *node = next_node;
}

ListNode *list_insert (ListNode **node, void *data, size_t size)
{
  //puts ("list_insert()");
  //printf ("%p %p %d\n", *node, data, size);
  
  ListNode *new_node = malloc (sizeof (ListNode));

  if (!new_node) 
    return NULL;

  new_node->data = malloc (size);

  if (!(new_node->data)) {
    free (new_node);
    return NULL;
  }

  memcpy (new_node->data, data, size);
  new_node->size = size;

  // If the list is not empty...
  if (*node) {
    new_node->prev = (*node)->prev;
    new_node->next = *node;
    (*node)->prev->next = new_node;
    (*node)->prev = new_node;
  }
  // i.e. the list is empty...
  else {
    *node = new_node;
    (*node)->next = (*node)->prev = *node;
  }
  
  //  printf ("%p <- %p -> %p\n", (*node)->prev, (*node), (*node)->next);

  return new_node;
}

ListNode *list_kth (ListNode *l, size_t k)
{
  if (!k)
    return l;
  return list_kth (l->next, k - 1);
}

ListNode *list_lookup (ListNode *l, const void *data, size_t size)
{
  ListNode *p = l;

  do {
    if (!memcmp (p->data, data, size))
      return p;
    p = p->next;
  } while (p != l);
  
  return NULL;
}

size_t list_size (ListNode *h)
{
  // printf ("%p <- %p -> %p\n", h->prev, h, h->next);
  //fputs ("list_size()\n", stderr);
  if (!h || !h->next)
    return 0;
 
  size_t res = 1;
  ListNode *p = h->next;
  while (p != h) {
    // printf ("%p %p\n", p, h);
    res++;
    p = p->next;
  }

  return res;
}
