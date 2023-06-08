#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linha.h"

Linha *create_linha()
{
  Linha *linha = (Linha *)malloc(sizeof(Linha));

  if (linha == NULL)
  {
    fprintf(stderr, "Error: Erro ao alocar memória para Linha\n");
    exit(1);
  }

  linha->name = NULL;
  linha->paragens = create_linked_list();

  return linha;
}

void free_linha(void *linha)
{
  if (linha == NULL || ((Linha *)linha)->name == NULL)
    return;

  delete_linked_list(((Linha *)linha)->paragens, NULL);
  free(((Linha *)linha)->name);
  free(linha);
}

int compare_linha_name(void *linha, void *name)
{
  return name != NULL && linha != NULL && ((Linha *)linha)->name != NULL && strcmp(((Linha *)linha)->name, name) == 0;
}

void print_linha(void *linha)
{
  if (linha == NULL || ((Linha *)linha)->name == NULL)
    return;

  printf("Linha: %s\n", ((Linha *)linha)->name);
  int n_paragens = size_linked_list(((Linha *)linha)->paragens);
  for (int i = 0; i < n_paragens; i++)
  {
    if (i > 0)
      printf(" => ");

    Paragem *paragem = get_data_in_linked_list(((Linha *)linha)->paragens, i);
    printf("%s", paragem->name);
  }
  printf("\n");
  for (int i = n_paragens - 1; i >= 0; i--)
  {
    if (i < n_paragens - 1)
      printf(" => ");

    Paragem *paragem = get_data_in_linked_list(((Linha *)linha)->paragens, i);
    printf("%s", paragem->name);
  }
  printf("\n\n");
}