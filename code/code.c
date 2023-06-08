#include <stdio.h>
#include <stdlib.h>

const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

char random_alphanumeric_character()
{
  return charset[rand() % (sizeof(charset) - 1)];
}

char *gen_alphanumeric_code(int length)
{
  char *code = malloc(sizeof(char) * (length + 1));

  if (code == NULL)
  {
    fprintf(stderr, "Error: Failed to allocate memory for code in gen_alphanumeric_code\n");
    exit(1);
  }

  for (int i = 0; i < length; i++)
  {
    code[i] = random_alphanumeric_character();
  }

  code[length] = '\0';

  return code;
}
