#ifndef VOID_LIST_H
#define VOID_LIST_H

typedef struct s_node *Node;
struct s_node
{
  void *data;
  Node next;
  Node prev;
};

typedef struct s_linked_list *LinkedList;
struct s_linked_list
{
  Node head;
  Node tail;
};

typedef void free_data(void *);
typedef int compare_data(void *, void *);
typedef void print_data(void *);

LinkedList create_linked_list();
void *get_data_in_linked_list(LinkedList, int);
void *find_data_in_linked_list(LinkedList, compare_data, void *);
void print_linked_list(LinkedList, print_data);
int size_linked_list(LinkedList);
void delete_linked_list(LinkedList, free_data);

Node create_node(void *);
void append_node(LinkedList, void *);
void delete_node(LinkedList, compare_data, void *, free_data);

#endif /* VOID_LIST_H */