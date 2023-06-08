#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

#include "main.h"

#define MAX_INPUT_SIZE 1025
#define PARAGENS_FILE "db/paragens.bin"
#define LINHAS_FILE "db/linhas.bin"

char user_input[MAX_INPUT_SIZE];
DynamicArray paragens;
LinkedList linhas;
Paragem *paragem, *paragem_files;
Linha *linha, *linha_files;
char *name, *code, *paragem_inicio, *paragem_fim;
FILE *file;
int n_paragens, n_linhas, err;
size_t name_len, code_len;

typedef void process_input(int);

void flush()
{
  fflush(stdin);
}

int init_from_files()
{
  file = fopen(PARAGENS_FILE, "rb");
  if (!file)
    return 1;

  paragens = create_dynamic_array();

  fread(&n_paragens, sizeof(int), 1, file);
  for (int i = 0; i < n_paragens; i++)
  {
    fread(&name_len, sizeof(size_t), 1, file);
    name = malloc(sizeof(char) * name_len);
    fread(name, sizeof(char), name_len, file);

    fread(&code_len, sizeof(size_t), 1, file);
    code = malloc(sizeof(char) * code_len);
    fread(code, sizeof(char), code_len, file);

    paragem = create_paragem();
    paragem->name = name;
    paragem->code = code;

    add_to_dynamic_array(paragens, paragem);
  }

  fclose(file);

  file = fopen(LINHAS_FILE, "rb");
  if (!file)
  {
    delete_dynamic_array(paragens, free_paragem);
    return 1;
  }

  linhas = create_linked_list();

  fread(&n_linhas, sizeof(int), 1, file);
  for (int i = 0; i < n_linhas; i++)
  {
    fread(&name_len, sizeof(size_t), 1, file);
    name = malloc(sizeof(char) * name_len);
    fread(name, sizeof(char), name_len, file);

    linha = create_linha();
    linha->name = name;

    fread(&n_paragens, sizeof(int), 1, file);
    for (int j = 0; j < n_paragens; j++)
    {
      fread(&code_len, sizeof(size_t), 1, file);
      code = malloc(sizeof(char) * code_len);
      fread(code, sizeof(char), code_len, file);

      paragem = find_data_in_dynamic_array(paragens, compare_paragem_code, code);
      append_node(linha->paragens, paragem);
    }

    append_node(linhas, linha);
  }

  fclose(file);

  return 0;
}

void write_to_files()
{

  file = fopen(PARAGENS_FILE, "wb");
  if (!file)
  {
    fprintf(stderr, "Error: couldn't open file %s\n", PARAGENS_FILE);
    return;
  }
  n_paragens = paragens->size;
  fwrite(&n_paragens, sizeof(int), 1, file);

  for (int i = 0; i < n_paragens; i++)
  {
    paragem_files = paragens->data[i];
    name_len = strlen(paragem_files->name) + 1;
    code_len = strlen(paragem_files->code) + 1;

    fwrite(&name_len, sizeof(size_t), 1, file);
    fwrite(paragem_files->name, sizeof(char), name_len, file);

    fwrite(&code_len, sizeof(size_t), 1, file);
    fwrite(paragem_files->code, sizeof(char), code_len, file);
  }

  fclose(file);

  file = fopen(LINHAS_FILE, "wb");
  if (!file)
  {
    fprintf(stderr, "Error: couldn't open file %s\n", LINHAS_FILE);
    return;
  }

  n_linhas = size_linked_list(linhas);
  fwrite(&n_linhas, sizeof(int), 1, file);

  for (int i = 0; i < n_linhas; i++)
  {
    linha_files = get_data_in_linked_list(linhas, i);
    name_len = strlen(linha_files->name) + 1;

    fwrite(&name_len, sizeof(size_t), 1, file);
    fwrite(linha_files->name, sizeof(char), name_len, file);

    n_paragens = size_linked_list(linha_files->paragens);
    fwrite(&n_paragens, sizeof(int), 1, file);

    for (int j = 0; j < n_paragens; j++)
    {
      paragem_files = get_data_in_linked_list(linha_files->paragens, j);
      code_len = strlen(paragem_files->code) + 1;

      fwrite(&code_len, sizeof(size_t), 1, file);
      fwrite(paragem_files->code, sizeof(char), code_len, file);
    }
  }

  fclose(file);
}

int getch(void)
{
  struct termios oldattr, newattr;
  int ch;
  tcgetattr(STDIN_FILENO, &oldattr);
  newattr = oldattr;
  newattr.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newattr);

  ch = getchar();

  if (ch == 27)
  {
    getchar();
    ch = getchar();
    switch (ch)
    {
    case 'A':
      ch = 72;
      break;
    case 'B':
      ch = 80;
      break;
    }
  }
  else if (ch == '\n')
  {
    ch = 13;
  }

  tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
  return ch;
}

void clear_file_nl(FILE *file)
{
  int c;
  while ((c = fgetc(file)) != '\n' && c != EOF)
    ;
}

void print_underline(int size)
{
  for (int i = 0; i < size; i++)
    printf("-");
  printf("\n");
}

void print_title(char *title)
{
  printf("%s\n", title);
  int title_size = strlen(title);
  print_underline(title_size);
}

int read_var(char *title, char *var, char *err)
{
  system("clear");

  print_title(title);

  user_input[0] = '\0';

  if (err != NULL)
    printf("Erro: %s\n", err);

  printf("%s ('#' para cancelar): ", var);
  scanf("%1024[^\n]", user_input);

  int ch;
  while ((ch = getchar()) != '\n' && ch != EOF)
    ;

  return strcmp(user_input, "#") == 0;
}

void print_options(char **prints, int pos, int length)
{
  for (int i = 0; i < length; i++)
  {
    if (i == pos)
      printf("-->");
    else
      printf("   ");
    printf("%s\n", prints[i]);
  }
}

void free_options(char **prints, int length)
{
  for (int i = 0; i < length; i++)
  {
    free(prints[i]);
  }
  free(prints);
}

void gen_menu(char *title, char **options, int length, process_input *process_fn)
{
  int flag = 1;
  int pos = 0;

  int local_length = length + 2;
  char **local_options = (char **)malloc(sizeof(char *) * local_length);
  for (int i = 0; i < length; i++)
    local_options[i] = strdup(options[i]);

  local_options[length] = strdup("VOLTAR");
  local_options[length + 1] = strdup("SAIR");

  free_options(options, length);

  int title_size = strlen(title);

  while (flag)
  {
    system("clear");

    printf("   %s\n", title);

    printf("   ");
    print_underline(title_size);

    print_options(local_options, pos, local_length);

    flush();
    switch (getch())
    {
    case 72:
      if (--pos < 0)
      {
        pos = local_length - 1;
      }
      break;

    case 80:
      if (++pos > local_length - 1)
      {
        pos = 0;
      }
      break;

    case 13:
      if (pos == length)
      {
        free_options(local_options, local_length);
        flag = 0;
      }
      else if (pos == length + 1)
      {
        free_options(local_options, local_length);
        exit(0);
      }
      else
      {
        process_fn(pos);
      }
      break;
    }
  }
}

void eliminar_paragens()
{
  int flag = 1;
  int pos = 0;

  char *title = strdup("ELIMINAR PARAGENS");
  int title_size = strlen(title);

  while (flag)
  {
    system("clear");

    printf("   %s\n", title);

    printf("   ");
    print_underline(title_size);

    int length = paragens->size;

    for (int i = 0; i < length; i++)
    {
      if (i == pos)
        printf("-->");
      else
        printf("   ");
      printf("%s\n", ((Paragem *)paragens->data[i])->name);
    }

    if (pos == length)
      printf("-->");
    else
      printf("   ");
    printf("VOLTAR\n");

    flush();
    switch (getch())
    {
    case 72:
      if (--pos < 0)
      {
        pos = length;
      }
      break;

    case 80:
      if (++pos > length)
      {
        pos = 0;
      }
      break;

    case 13:
      if (pos == length)
      {
        free(title);
        flag = 0;
      }
      else
      {
        err = 0;

        paragem = paragens->data[pos];

        if (paragem == NULL)
        {
          fprintf(stderr, "Error: paragens->data[pos] is NULL in eliminar_paragem!\n");
          exit(1);
        }

        n_linhas = size_linked_list(linhas);
        for (int i = 0; i < n_linhas && !err; i++)
        {
          linha = get_data_in_linked_list(linhas, i);

          if (find_data_in_linked_list(linha->paragens, compare_paragem_name, paragem->name) != NULL)
            err = 1;
        }

        if (err)
        {
          system("clear");

          printf("Erro: A paragem está associada à linha \"%s\"!.\n", linha->name);

          flush();
          getch();
          flush();
        }
        else
        {
          delete_from_dynamic_array(paragens, pos, free_paragem);
          write_to_files();
        }
      }
      break;
    }
  }
}

void paragens_linha_menu(Linha *linha)
{
  int flag = 1;
  int pos = 0;

  char title[MAX_INPUT_SIZE];
  snprintf(title, sizeof(title), "PARAGENS DA LINHA %s", linha->name);
  int title_size = strlen(title);

  char sub_title[MAX_INPUT_SIZE];
  snprintf(sub_title, sizeof(sub_title), "DIAGRAMA DA LINHA %s", linha->name);
  int sub_title_size = strlen(sub_title);

  while (flag)
  {
    system("clear");

    printf("   %s\n", title);

    printf("   ");
    print_underline(title_size);

    int length = paragens->size;

    for (int i = 0; i < length; i++)
    {
      paragem = paragens->data[i];

      if (i == pos)
        printf("-->");
      else
        printf("   ");
      printf("%s", paragem->name);

      if (find_data_in_linked_list(linha->paragens, compare_paragem_name, paragem->name) != NULL)
        printf(" (X)");
      else
        printf(" ( )");

      printf("\n");
    }

    if (pos == length)
      printf("-->");
    else
      printf("   ");
    printf("VOLTAR\n");

    printf("\n   %s\n", sub_title);

    printf("   ");
    print_underline(sub_title_size);

    n_paragens = size_linked_list(linha->paragens);

    printf("   ");
    for (int i = 0; i < n_paragens; i++)
    {
      if (i > 0)
        printf(" => ");

      paragem = get_data_in_linked_list(linha->paragens, i);
      printf("%s", paragem->name);
    }
    printf("\n");

    printf("   ");
    for (int i = n_paragens - 1; i >= 0; i--)
    {
      if (i < n_paragens - 1)
        printf(" => ");

      paragem = get_data_in_linked_list(linha->paragens, i);
      printf("%s", paragem->name);
    }
    printf("\n");

    flush();
    switch (getch())
    {
    case 72:
      if (--pos < 0)
      {
        pos = length;
      }
      break;

    case 80:
      if (++pos > length)
      {
        pos = 0;
      }
      break;

    case 13:
      if (pos == length)
      {
        flag = 0;
      }
      else
      {
        paragem = paragens->data[pos];

        if (find_data_in_linked_list(linha->paragens, compare_paragem_name, paragem->name) == NULL)
        {
          append_node(linha->paragens, paragem);
          write_to_files();
        }
        else
        {
          delete_node(linha->paragens, compare_paragem_name, paragem->name, NULL);
          write_to_files();
        }
      }
      break;
    }
  }
}

void escolher_linha_menu()
{
  int flag = 1;
  int pos = 0;

  char *title = strdup("EDITAR PARAGENS DE LINHA");
  int title_size = strlen(title);

  while (flag)
  {
    system("clear");

    printf("   %s\n", title);

    printf("   ");
    print_underline(title_size);

    int length = size_linked_list(linhas);

    for (int i = 0; i < length; i++)
    {
      linha = get_data_in_linked_list(linhas, i);

      if (i == pos)
        printf("-->");
      else
        printf("   ");
      printf("%s\n", linha->name);
    }

    if (pos == length)
      printf("-->");
    else
      printf("   ");
    printf("VOLTAR\n");

    flush();
    switch (getch())
    {
    case 72:
      if (--pos < 0)
      {
        pos = length;
      }
      break;

    case 80:
      if (++pos > length)
      {
        pos = 0;
      }
      break;

    case 13:
      if (pos == length)
      {
        free(title);
        flag = 0;
      }
      else
      {
        paragens_linha_menu(get_data_in_linked_list(linhas, pos));
      }
      break;
    }
  }
}

void linhas_menu_process(int pos)
{
  switch (pos)
  {
  case 0:
    err = read_var("ADICIONAR LINHA", "Nome", NULL);

    while (strlen(user_input) == 0 || find_data_in_linked_list(linhas, compare_linha_name, user_input) != NULL)
      err = read_var("ADICIONAR LINHA", "Nome", strlen(user_input) == 0 ? "Por favor insira um nome!" : "Já existe uma linha com esse nome!");

    if (err)
      break;

    linha = create_linha();
    linha->name = strdup(user_input);

    paragens_linha_menu(linha);

    append_node(linhas, linha);
    write_to_files();
    break;

  case 1:
    err = read_var("ADICIONAR LINHA (FICHEIRO)", "Path do Ficheiro", NULL);

    while (strlen(user_input) == 0)
      err = read_var("ADICIONAR LINHA (FICHEIRO)", "Path do Ficheiro", "Por favor insira um path de ficheiro!");

    if (err)
      break;

    FILE *file = fopen(user_input, "r");

    if (file == NULL)
    {
      printf("Erro: Não foi possível abrir o ficheiro!\n");

      flush();
      getch();
      flush();
      break;
    }

    linha = create_linha();

    err = 0;

    user_input[0] = '\0';
    if (fscanf(file, "%1024[^\n]", user_input) != EOF)
    {
      clear_file_nl(file);
      if (find_data_in_linked_list(linhas, compare_linha_name, user_input) != NULL)
      {
        printf("Erro: Já existe uma linha com o nome \"%s\"!\n", user_input);

        flush();
        getch();
        flush();

        err = 1;
      }

      if (!err)
        linha->name = strdup(user_input);
    }
    else
    {
      printf("Erro: Não foi possível ler o nome da linha!\n");

      flush();
      getch();
      flush();

      err = 1;
    }

    user_input[0] = '\0';
    while (!err && fscanf(file, "%1024[^\n]", user_input) != EOF)
    {
      clear_file_nl(file);
      paragem = find_data_in_dynamic_array(paragens, compare_paragem_name_code, user_input);

      if (paragem == NULL)
      {
        printf("Erro: Não foi possível encontrar a paragem \"%s\"!\n", user_input);

        flush();
        getch();
        flush();

        err = 1;
      }

      if (!err)
        append_node(linha->paragens, paragem);
    }

    fclose(file);

    if (err)
      free_linha(linha);
    else
    {
      append_node(linhas, linha);
      write_to_files();

      printf("Linha \"%s\" adicionada com sucesso!\n", linha->name);

      flush();
      getch();
      flush();
    }

    break;

  case 2:
    escolher_linha_menu();
    break;

  case 3:
    system("clear");
    print_title("VISUALIZAR LINHAS");

    if (size_linked_list(linhas) == 0)
      printf("Erro: Não existem linhas!\n");
    else
      print_linked_list(linhas, print_linha);

    flush();
    getch();
    flush();

    break;
  }
}

void linhas_menu()
{
  int length = 4;

  char **prints = (char **)malloc(sizeof(char *) * length);
  prints[0] = strdup("ADICIONAR");
  prints[1] = strdup("ADICIONAR (FICHEIRO)");
  prints[2] = strdup("ATUALIZAR");
  prints[3] = strdup("VISUALIZAR");

  gen_menu("LINHAS", prints, length, linhas_menu_process);
}

void paragens_menu_process(int pos)
{
  switch (pos)
  {
  case 0:
    err = read_var("REGISTAR PARAGEM", "Nome", NULL);

    while (strlen(user_input) == 0 || find_data_in_dynamic_array(paragens, compare_paragem_name, user_input) != NULL)
      err = read_var("REGISTAR PARAGEM", "Nome", strlen(user_input) == 0 ? "Por favor insira um nome!" : "Já existe uma paragem com esse nome!");

    if (err)
      break;

    paragem = create_paragem();
    paragem->name = strdup(user_input);

    code = gen_alphanumeric_code(4);

    while (find_data_in_dynamic_array(paragens, compare_paragem_code, code) != NULL)
    {
      free(code);
      code = gen_alphanumeric_code(4);
    }

    paragem->code = code;

    add_to_dynamic_array(paragens, paragem);
    write_to_files();
    break;

  case 1:
    err = read_var("REGISTAR PARAGENS (FICHEIRO)", "Path do Ficheiro", NULL);

    while (strlen(user_input) == 0)
      err = read_var("REGISTAR PARAGENS (FICHEIRO)", "Path do Ficheiro", "Por favor insira um path de ficheiro!");

    if (err)
      break;

    FILE *file = fopen(user_input, "r");

    if (file == NULL)
    {
      printf("Erro: Não foi possível abrir o ficheiro!\n");

      flush();
      getch();
      flush();
      break;
    }

    user_input[0] = '\0';
    while (!err && fscanf(file, "%1024[^\n]", user_input) != EOF)
    {
      clear_file_nl(file);
      if (find_data_in_dynamic_array(paragens, compare_paragem_name, user_input) != NULL)
      {
        printf("Erro: Já existe uma paragem com o nome \"%s\"!\n", user_input);

        flush();
        getch();
        flush();
        err = 1;
      }

      if (!err)
      {
        paragem = create_paragem();
        paragem->name = strdup(user_input);

        code = gen_alphanumeric_code(4);

        while (find_data_in_dynamic_array(paragens, compare_paragem_code, code) != NULL)
        {
          free(code);
          code = gen_alphanumeric_code(4);
        }

        paragem->code = code;

        add_to_dynamic_array(paragens, paragem);
        write_to_files();
      }
    }

    if (!err)
    {
      printf("Paragens adicionadas com sucesso!\n");

      flush();
      getch();
      flush();
    }

    break;

  case 2:
    eliminar_paragens();
    break;

  case 3:
    system("clear");
    print_title("VISUALIZAR PARAGENS");

    if (paragens->size == 0)
      printf("Erro: Não existem paragens!\n");
    else
      print_dynamic_array(paragens, print_paragem);

    flush();
    getch();
    flush();
    break;
  }
}

void paragens_menu()
{
  int length = 4;

  char **prints = (char **)malloc(sizeof(char *) * length);
  prints[0] = strdup("REGISTAR");
  prints[1] = strdup("REGISTAR (FICHEIRO)");
  prints[2] = strdup("ELIMINAR");
  prints[3] = strdup("VISUALIZAR");

  gen_menu("PARAGENS", prints, length, paragens_menu_process);
}

void find_all_paths()
{
  printf("NOT IMPLEMENTED");
}

void main_menu_process(int pos)
{
  switch (pos)
  {
  case 0:
    paragens_menu();
    break;

  case 1:
    linhas_menu();
    break;

  case 2:
    err = read_var("PARAGEM DE INÍCIO", "Nome", NULL);

    while (strlen(user_input) == 0 || find_data_in_dynamic_array(paragens, compare_paragem_name, user_input) == NULL)
      err = read_var("PARAGEM DE INÍCIO", "Nome", strlen(user_input) == 0 ? "Por favor insira um nome!" : "Não existe uma paragem com esse nome!");

    paragem_inicio = strdup(user_input);

    if (err)
      break;

    err = read_var("PARAGEM DE FIM", "Nome", NULL);

    while (strlen(user_input) == 0 || strcmp(paragem_inicio, user_input) == 0 || find_data_in_dynamic_array(paragens, compare_paragem_name, user_input) == NULL)
      err = read_var(
          "PARAGEM DE FIM",
          "Nome",
          strlen(user_input) == 0                   ? "Por favor insira um nome!"
          : strcmp(paragem_inicio, user_input) == 0 ? "Paragem de fim não pode ser igual à paragem de início!"
                                                    : "Não existe uma paragem com esse nome!");

    paragem_fim = strdup(user_input);

    if (err)
      break;

    find_all_paths();

    flush();
    getch();
    flush();

    break;
  }
}

void main_menu()
{
  int length = 3;

  char **prints = (char **)malloc(sizeof(char *) * length);
  prints[0] = strdup("PARAGENS");
  prints[1] = strdup("LINHAS");
  prints[2] = strdup("PERCURSO");

  gen_menu("MENU", prints, length, main_menu_process);
}

int main()
{
  srand((unsigned int)time(NULL));

  if (init_from_files())
  {
    paragens = create_dynamic_array();
    linhas = create_linked_list();
  }

  main_menu();

  return 0;
}