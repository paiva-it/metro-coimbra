#include <stdio.h>
#include <stdlib.h>

#include "dynamic-array.h"

DynamicArray create_dynamic_array()
{
  DynamicArray dynamicArray = (DynamicArray)malloc(sizeof(struct s_dynamic_array));

  if (dynamicArray == NULL)
  {
    fprintf(stderr, "Error: Failed to allocate memory for DynamicArray in create_dynamic_array\n");
    exit(1);
  }

  dynamicArray->size = 0;
  dynamicArray->data = NULL;

  return dynamicArray;
}

void delete_dynamic_array(DynamicArray dynamicArray, free_data free_fn)
{
  if (dynamicArray == NULL)
    return;

  if (free_fn != NULL)
  {
    for (int i = 0; i < dynamicArray->size; i++)
      free_fn(dynamicArray->data[i]);
  }

  free(dynamicArray->data);
  free(dynamicArray);
}

void resize_dynamic_array(DynamicArray dynamicArray)
{
  dynamicArray->data = (void **)realloc(dynamicArray->data, sizeof(void *) * dynamicArray->size);
}

void add_to_dynamic_array(DynamicArray dynamicArray, void *data)
{
  dynamicArray->size++;
  resize_dynamic_array(dynamicArray);
  dynamicArray->data[dynamicArray->size - 1] = data;
}

void delete_from_dynamic_array(DynamicArray dynamicArray, int index, free_data free_fn)
{
  if (index < 0 || index >= dynamicArray->size)
  {
    printf("Índice inválido\n");
    return;
  }

  if (free_fn != NULL)
    free_fn(dynamicArray->data[index]);

  dynamicArray->size--;

  for (int i = index; i < dynamicArray->size; i++)
    dynamicArray->data[i] = dynamicArray->data[i + 1];

  resize_dynamic_array(dynamicArray);
}

void *find_data_in_dynamic_array(DynamicArray dynamicArray, compare_data compare_fn, void *ele)
{
  for (int i = 0; i < dynamicArray->size; i++)
  {
    if (compare_fn(dynamicArray->data[i], ele))
      return dynamicArray->data[i];
  }

  return NULL;
}

void print_dynamic_array(DynamicArray dynamicArray, print_data print_fn)
{
  for (int i = 0; i < dynamicArray->size; i++)
    print_fn(dynamicArray->data[i]);
}