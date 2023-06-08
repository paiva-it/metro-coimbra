#ifndef PARAGEM_H
#define PARAGEM_H

typedef struct
{
  char *name;
  char *code;
} Paragem;

Paragem *create_paragem();
void free_paragem(void *);
int compare_paragem_name(void *, void *);
int compare_paragem_code(void *, void *);
int compare_paragem_name_code(void *, void *);
void print_paragem(void *);

#endif /* PARAGEM_H */