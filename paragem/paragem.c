#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "paragem.h"

Paragem *create_paragem()
{
  Paragem *paragem = malloc(sizeof(Paragem));

  if (paragem == NULL)
  {
    fprintf(stderr, "Error: Failed to allocate memory for Paragem in create_paragem\n");
    exit(1);
  }

  paragem->name = NULL;
  paragem->code = NULL;

  return paragem;
}

void free_paragem(void *paragem)
{
  if (paragem == NULL || ((Paragem *)paragem)->name == NULL || ((Paragem *)paragem)->code == NULL)
    return;

  free(((Paragem *)paragem)->name);
  free(((Paragem *)paragem)->code);
  free(paragem);
}

int compare_paragem_name(void *paragem, void *name)
{
  return name != NULL && paragem != NULL && ((Paragem *)paragem)->name != NULL && strcmp(((Paragem *)paragem)->name, name) == 0;
}

int compare_paragem_code(void *paragem, void *code)
{
  return code != NULL && paragem != NULL && ((Paragem *)paragem)->code != NULL && strcmp(((Paragem *)paragem)->code, code) == 0;
}

int compare_paragem_name_code(void *paragem, void *user_input)
{
  char *original = strdup(user_input);
  char *name = strtok(original, "#");
  char *code = strtok(NULL, "#");

  return compare_paragem_name(paragem, name) && compare_paragem_code(paragem, code);
}

void print_paragem(void *paragem)
{
  if (paragem == NULL || ((Paragem *)paragem)->name == NULL || ((Paragem *)paragem)->code == NULL)
    return;

  printf("Paragem: %s - %s\n", ((Paragem *)paragem)->code, ((Paragem *)paragem)->name);
}