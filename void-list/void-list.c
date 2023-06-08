#include <stdio.h>
#include <stdlib.h>

#include "void-list.h"

LinkedList create_linked_list()
{
  LinkedList list = malloc(sizeof(struct s_linked_list));

  if (list == NULL)
  {
    fprintf(stderr, "Error: Failed to allocate memory for LinkedList in create_linked_list\n");
    exit(1);
  }

  list->head = NULL;
  list->tail = NULL;

  return list;
}

void *get_data_in_linked_list(LinkedList list, int index)
{
  Node current = list->head;

  for (int i = 0; i < index; i++)
  {
    current = current->next;
  }

  return current->data;
}

void *find_data_in_linked_list(LinkedList list, compare_data compare_fn, void *ele)
{
  Node current = list->head;

  while (current)
  {
    if (compare_fn(current->data, ele))
      return current->data;
    current = current->next;
  }

  return NULL;
}

void print_linked_list(LinkedList list, print_data print_fn)
{
  Node current = list->head;

  while (current)
  {
    print_fn(current->data);
    current = current->next;
  }
}

int size_linked_list(LinkedList list)
{
  Node current = list->head;
  int size = 0;

  while (current)
  {
    size++;
    current = current->next;
  }

  return size;
}

void delete_linked_list(LinkedList list, free_data free_fn)
{
  if (list == NULL)
    return;

  Node current = list->head;
  Node next;

  while (current)
  {
    next = current->next;
    if (free_fn != NULL)
      free_fn(current->data);
    free(current);
    current = next;
  }

  free(list);
}

Node create_node(void *data)
{
  Node node = malloc(sizeof(struct s_node));

  if (node == NULL)
    fprintf(stderr, "Error: Failed to allocate memory for Node in create_node\n");

  node->data = data;
  node->next = NULL;
  node->prev = NULL;

  return node;
}

void append_node(LinkedList list, void *data)
{
  if (list == NULL)
  {
    fprintf(stderr, "Error: List is NULL in append_node\n");
    return;
  }

  Node node = create_node(data);

  node->prev = list->tail;

  if (list->tail != NULL)
  {
    list->tail->next = node;
  }

  if (list->head == NULL)
  {
    list->head = node;
  }

  list->tail = node;
}

void delete_node(LinkedList list, compare_data compare_fn, void *ele, free_data free_fn)
{
  if (list == NULL)
  {
    fprintf(stderr, "Error: List is NULL in delete_node\n");
    return;
  }

  Node current = list->head;
  Node next;

  while (current != NULL)
  {
    next = current->next;

    if (compare_fn != NULL && compare_fn(current->data, ele))
    {
      if (current->prev == NULL)
      {
        list->head = current->next;
      }
      else
      {
        current->prev->next = current->next;
      }

      if (current->next == NULL)
      {
        list->tail = current->prev;
      }
      else
      {
        current->next->prev = current->prev;
      }

      if (free_fn != NULL)
      {
        free_fn(current->data);
      }

      free(current);

      return;
    }

    current = next;
  }
}