#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include "../void-list/void-list.h"

typedef struct s_dynamic_array *DynamicArray;
struct s_dynamic_array
{
  int size;
  void **data;
};

void resize_dynamic_array(DynamicArray);
DynamicArray create_dynamic_array();
void delete_dynamic_array(DynamicArray, free_data);
void add_to_dynamic_array(DynamicArray, void *);
void delete_from_dynamic_array(DynamicArray, int, free_data);
void *find_data_in_dynamic_array(DynamicArray, compare_data, void *);
void print_dynamic_array(DynamicArray, print_data);

#endif /* DYNAMIC_ARRAY_H */