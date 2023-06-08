#ifndef LINHA_H
#define LINHA_H

#include "../void-list/void-list.h"
#include "../paragem/paragem.h"

typedef struct
{
  char *name;
  LinkedList paragens;
} Linha;

Linha *create_linha();
void free_linha(void *);
int compare_linha_name(void *, void *);
void print_linha(void *);

#endif /* LINHA_H */