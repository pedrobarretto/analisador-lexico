// Pedro Galvão Barretto - TIA: 32016591
// Luan Lemos - TIA: 32085109

#pragma warning(disable : 4996)

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0

typedef enum
{
  SEM_ERRO,
  SIN_ERROR,
  PRG_ERROR,
  PRG_OK,
  COMENTARIO_ERROR,
  COMENTARIO_OK,
  SIN_EOS,
  VAR_OP,
  INT_OP,
  IDENT
} SIN_TYPES;

char *sinTokenToString[] = {
    "Sem erro na operacao",
    "Erro sintatico",
    "Erro prg",
    "PRG correto",
    "Erro comentario",
    "Comentario correto",
    "Final de arquivo",
    "Definicao de variaveis",
    "Definicao de inteiros",
    "Identificador generico"};

typedef struct
{
  SIN_TYPES tipo;
  int linha;
  char valor[100];
} SIN_TOKEN;

char *buffer;
int sinLinha = 1;
short debugMode = FALSE;

SIN_TOKEN parser();

int check(char *buffer, char str);
int checkAndAdd(SIN_TOKEN tk, char str);

int main(int argc, char *argv[])
{

  if (argc < 2)
  {
    printf("Analisador sintatico para expressoes aritmeticas parentizadas envolvendo variaveis e numeros\n");
    printf("Uso:\n");
    printf("  main.exe <arquivo_fonte> [/d]\n");
    printf("\n");
    printf("<arquivo_fonte> pode ser qualquer arquivo texto cujo conteudo sera\n");
    printf("analisado quanto a linguagem definida para esse exercicio.\n");
    return 0;
  }
  else
  {
    if ((argc == 3) && (strcmp(argv[2], "/d") == 0))
      debugMode = TRUE;

    printf("Iniciando a analise sintatica do arquivo: %s\n\n", argv[1]);
  }

  FILE *arq = fopen(argv[1], "r");
  if (arq == NULL)
  {
    fprintf(stderr, "Erro ao tentar abrir o aquivo \"%s\"", argv[1]);
    return -1;
  }
  fseek(arq, 0, SEEK_END);
  int tam_arq = ftell(arq);
  fseek(arq, 0, SEEK_SET);

  buffer = (char *)calloc(tam_arq, sizeof(char));
  if (buffer == NULL)
    exit(-1);
  else
    fread(buffer, sizeof(char), tam_arq, arq);

  fclose(arq);

  SIN_TOKEN tk;
  do
  {
    tk = parser();
    if (tk.tipo != IDENT)
      printf("\nLinha:%3d | %-30s | %5s", tk.linha, sinTokenToString[tk.tipo], tk.valor);
  } while ((tk.tipo != SIN_EOS) && (tk.tipo != SIN_ERROR));

  return 0;
}

int checkAndAdd(SIN_TOKEN tk, char str)
{
  if (*buffer == str)
  {
    strncat(tk.valor, buffer, 1);
    buffer++;
    return TRUE;
  }

  strncat(tk.valor, buffer, 1);
  buffer++;
  return FALSE;
}

int check(char *buffer, char str)
{
  if (*buffer == str)
  {
    // printf("Dentro do if - CHECK: %c\n", *buffer);
    return 1;
  }

  // printf("Fora do if - CHECK: %c\n", *buffer);
  return 0;
}

SIN_TOKEN parser()
{
  SIN_TOKEN tk;
  int contFechaComent = 0;
  int contAbreComent = 0;
  char *variables[40];

  tk.tipo = SIN_ERROR;
  strcpy(tk.valor, "");
  tk.linha = sinLinha;

  // Tabulação e espaços
  while ((*buffer == ' ') || (*buffer == '\t') || (*buffer == '\r') || (*buffer == '\n'))
  {
    if (*buffer == '\n')
      sinLinha++;

    buffer++;
  }

  // Comentário
  if (*buffer == '{')
  {
    contAbreComent++;

    strncat(tk.valor, buffer, 1);
    buffer++;

    while (*buffer != '}')
    {
      strncat(tk.valor, buffer, 1);
      buffer++;
      if (*buffer == '\n')
      {
        tk.tipo = SIN_ERROR;
        tk.linha = sinLinha;
        return tk;
      }
    }
    if (*buffer == '}')
    {
      strncat(tk.valor, buffer, 1);
      buffer++;
      tk.tipo = SEM_ERRO;
      tk.linha = sinLinha;
      return tk;
    }
    else
    {
      strncat(tk.valor, buffer, 1);
      buffer++;
      tk.tipo = SIN_ERROR;
      tk.linha = sinLinha;
      return tk;
    }
  }

  if (isalpha(*buffer))
  {
    while (isalpha(*buffer) || isdigit(*buffer) || (*buffer == '_'))
    {
      // prg
      if (check(buffer, 'p') && check(buffer + 1, 'r') && check(buffer + 2, 'g') && check(buffer + 3, ' '))
      {
        strncat(tk.valor, buffer, 1);
        buffer++;

        strncat(tk.valor, buffer, 1);
        buffer++;

        strncat(tk.valor, buffer, 1);
        buffer++;

        strncat(tk.valor, buffer, 1);
        buffer++;

        while (isalpha(*buffer) || isdigit(*buffer) || (*buffer == '_') || (*buffer == ';'))
        {
          if ((*buffer == ' ') || (*buffer == '\t') || (*buffer == '\r') || (*buffer == '\n'))
          {
            tk.tipo = SIN_ERROR;
            return tk;
          }
          if (check(buffer, ';'))
          {
            strncat(tk.valor, buffer, 1);
            buffer++;
            if ((*buffer == ' ') || (*buffer == '\t') || (*buffer == '\r') || (*buffer == '\n'))
            {
              tk.tipo = SEM_ERRO;
              buffer++;
              return tk;
            }
            else
            {
              tk.tipo = SIN_ERROR;
              return tk;
            }
          }

          tk.tipo = SIN_ERROR;
          strncat(tk.valor, buffer, 1);
          buffer++;
        }
        break;
      }

      // var
      if (check(buffer, 'v') && check(buffer + 1, 'a') && check(buffer + 2, 'r') && check(buffer + 3, '\n') && check(buffer + 4, ' '))
      {
        strncat(tk.valor, buffer, 3);
        buffer = buffer + 5;
        tk.linha = sinLinha;
        tk.tipo = SEM_ERRO;
        return tk;
      }

      // int
      if (check(buffer, 'i') && check(buffer + 1, 'n') && check(buffer + 2, 't'))
      {
        strncat(tk.valor, buffer, 3);
        buffer = buffer + 3;

        while (!check(buffer, ';'))
        {
          strncat(tk.valor, buffer, 1);
          buffer++;

          if (isalpha(*buffer) && check(buffer + 1, ' '))
          {
            strncat(tk.valor, buffer, 1);
            buffer++;
            tk.tipo = SIN_ERROR;
            tk.linha = sinLinha;
            return tk;
          }

          if (*buffer == '\n')
          {
            tk.tipo = SIN_ERROR;
            tk.linha = sinLinha;
            return tk;
          }
        }

        if (*buffer == ';')
        {
          strncat(tk.valor, buffer, 1);
          buffer++;
          tk.tipo = SEM_ERRO;
          tk.linha = sinLinha;

          return tk;
        }
        else
        {
          strncat(tk.valor, buffer, 1);
          buffer++;
          tk.tipo = SIN_ERROR;
          tk.linha = sinLinha;
          return tk;
        }
      }

      // float
      if (check(buffer, 'f') && check(buffer + 1, 'l') && check(buffer + 2, 'o') && check(buffer + 3, 'a') && check(buffer + 4, 't'))
      {
        strncat(tk.valor, buffer, 5);
        buffer = buffer + 5;

        while (!check(buffer, ';'))
        {
          strncat(tk.valor, buffer, 1);
          buffer++;

          if (isalpha(*buffer) && check(buffer + 1, ' '))
          {
            strncat(tk.valor, buffer, 1);
            buffer++;
            tk.tipo = SIN_ERROR;
            tk.linha = sinLinha;
            return tk;
          }

          if (*buffer == '\n')
          {
            tk.tipo = SIN_ERROR;
            tk.linha = sinLinha;
            return tk;
          }
        }

        if (*buffer == ';')
        {
          strncat(tk.valor, buffer, 1);
          buffer++;
          tk.tipo = SEM_ERRO;
          tk.linha = sinLinha;
          return tk;
        }
        else
        {
          strncat(tk.valor, buffer, 1);
          buffer++;
          tk.tipo = SIN_ERROR;
          tk.linha = sinLinha;
          return tk;
        }
      }

      // char
      if (check(buffer, 'c') && check(buffer + 1, 'h') && check(buffer + 2, 'a') && check(buffer + 3, 'r'))
      {
        strncat(tk.valor, buffer, 4);
        buffer = buffer + 4;

        while (!check(buffer, ';'))
        {
          strncat(tk.valor, buffer, 1);
          buffer++;

          if (isalpha(*buffer) && check(buffer + 1, ' '))
          {
            strncat(tk.valor, buffer, 1);
            buffer++;
            tk.tipo = SIN_ERROR;
            tk.linha = sinLinha;
            return tk;
          }

          if (*buffer == '\n')
          {
            tk.tipo = SIN_ERROR;
            tk.linha = sinLinha;
            return tk;
          }
        }

        if (*buffer == ';')
        {
          strncat(tk.valor, buffer, 1);
          buffer++;
          tk.tipo = SEM_ERRO;
          tk.linha = sinLinha;
          return tk;
        }
        else
        {
          strncat(tk.valor, buffer, 1);
          buffer++;
          tk.tipo = SIN_ERROR;
          tk.linha = sinLinha;
          return tk;
        }
      }

      // begin
      if (check(buffer, 'b') && check(buffer + 1, 'e') && check(buffer + 2, 'g') && check(buffer + 3, 'i') && check(buffer + 4, 'n') && check(buffer + 5, '\n'))
      {
        strncat(tk.valor, buffer, 5);
        buffer = buffer + 6;
        tk.linha = sinLinha;
        tk.tipo = SEM_ERRO;
        return tk;
      }

      // read
      if (check(buffer, 'r') && check(buffer + 1, 'e') && check(buffer + 2, 'a') && check(buffer + 3, 'd') && check(buffer + 4, '('))
      {
        strncat(tk.valor, buffer, 5);
        buffer = buffer + 5;

        while (!check(buffer, ')'))
        {
          strncat(tk.valor, buffer, 1);
          buffer++;
        }

        if (check(buffer, ')') && check(buffer + 1, ';') && check(buffer + 2, '\n'))
        {
          strncat(tk.valor, buffer, 2);
          buffer = buffer + 3;
          tk.linha = sinLinha;
          tk.tipo = SEM_ERRO;
          return tk;
        }
        else
        {
          strncat(tk.valor, buffer, 1);
          buffer++;
          tk.tipo = SIN_ERROR;
          tk.linha = sinLinha;
          return tk;
        }
      }

      // write
      if (check(buffer, 'w') && check(buffer + 1, 'r') && check(buffer + 2, 'i') && check(buffer + 3, 't') && check(buffer + 4, 'e') && check(buffer + 5, '('))
      {
        strncat(tk.valor, buffer, 6);
        buffer = buffer + 6;

        while (!check(buffer, ')'))
        {
          strncat(tk.valor, buffer, 1);
          buffer++;
        }

        if (check(buffer, ')') && check(buffer + 1, ';') && check(buffer + 2, '\n'))
        {
          strncat(tk.valor, buffer, 2);
          buffer = buffer + 2;
          tk.linha = sinLinha;
          tk.tipo = SEM_ERRO;
          return tk;
        }
        else
        {
          strncat(tk.valor, buffer, 1);
          buffer++;
          tk.tipo = SIN_ERROR;
          tk.linha = sinLinha;
          return tk;
        }
      }

      // while
      if (check(buffer, 'w') && check(buffer + 1, 'h') && check(buffer + 2, 'i') && check(buffer + 3, 'l') && check(buffer + 4, 'e'))
      {
        strncat(tk.valor, buffer, 5);
        buffer = buffer + 5;

        int abreP = 0;
        int fechaP = 0;

        if (check(buffer, '('))
        {
          abreP++;
          strncat(tk.valor, buffer, 1);
          buffer++;
        }
        else if (check(buffer, ' ') && check(buffer + 1, '('))
        {
          abreP++;
          strncat(tk.valor, buffer, 2);
          buffer = buffer + 2;
        }

        while (!check(buffer, '\n'))
        {
          if (check(buffer, '(') && check(buffer + 1, ')'))
          {
            strncat(tk.valor, buffer, 1);
            buffer++;
            tk.tipo = SIN_ERROR;
            tk.linha = sinLinha;
            return tk;
          }

          if (check(buffer, '('))
          {
            abreP++;
          }
          if (check(buffer, ')'))
          {
            fechaP++;
          }

          strncat(tk.valor, buffer, 1);
          buffer++;
        }

        if (fechaP - abreP == 0)
        {
          // strncat(tk.valor, buffer, 1);
          buffer++;
          tk.tipo = SEM_ERRO;
          tk.linha = sinLinha;
          return tk;
        }
        else
        {
          strncat(tk.valor, buffer, 1);
          buffer++;
          tk.tipo = SIN_ERROR;
          tk.linha = sinLinha;
          return tk;
        }
      }

      // if
      if (check(buffer, 'i') && check(buffer + 1, 'f'))
      {
        strncat(tk.valor, buffer, 2);
        buffer = buffer + 2;

        int abreP = 0;
        int fechaP = 0;

        if (check(buffer, '('))
        {
          abreP++;
          strncat(tk.valor, buffer, 1);
          buffer++;
        }
        else if (check(buffer, ' ') && check(buffer + 1, '('))
        {
          abreP++;
          strncat(tk.valor, buffer, 2);
          buffer = buffer + 2;
        }

        while (!check(buffer, '\n'))
        {
          if (check(buffer, '(') && check(buffer + 1, ')'))
          {
            strncat(tk.valor, buffer, 1);
            buffer++;
            tk.tipo = SIN_ERROR;
            tk.linha = sinLinha;
            return tk;
          }

          if (check(buffer, '('))
          {
            abreP++;
          }
          if (check(buffer, ')'))
          {
            fechaP++;
          }

          strncat(tk.valor, buffer, 1);
          buffer++;
        }

        if (
            fechaP - abreP == 0 &&
            check(buffer - 1, 'n') &&
            check(buffer - 2, 'e') &&
            check(buffer - 3, 'h') &&
            check(buffer - 4, 't'))
        {
          // strncat(tk.valor, buffer, 1);
          buffer++;
          tk.tipo = SEM_ERRO;
          tk.linha = sinLinha;
          return tk;
        }
        else
        {
          strncat(tk.valor, buffer, 1);
          buffer++;
          tk.tipo = SIN_ERROR;
          tk.linha = sinLinha;
          return tk;
        }
      }

      // else if
      if (
          check(buffer, 'e') &&
          check(buffer + 1, 'l') &&
          check(buffer + 2, 's') &&
          check(buffer + 3, 'e') &&
          check(buffer + 4, ' ') &&
          check(buffer + 5, 'i') &&
          check(buffer + 6, 'f'))
      {
        strncat(tk.valor, buffer, 7);
        buffer = buffer + 7;

        int abreP = 0;
        int fechaP = 0;

        if (check(buffer, '('))
        {
          abreP++;
          strncat(tk.valor, buffer, 1);
          buffer++;
        }
        else if (check(buffer, ' ') && check(buffer + 1, '('))
        {
          abreP++;
          strncat(tk.valor, buffer, 2);
          buffer = buffer + 2;
        }

        while (!check(buffer, '\n'))
        {
          if (check(buffer, '(') && check(buffer + 1, ')'))
          {
            strncat(tk.valor, buffer, 1);
            buffer++;
            tk.tipo = SIN_ERROR;
            tk.linha = sinLinha;
            return tk;
          }

          if (check(buffer, '('))
          {
            abreP++;
          }
          if (check(buffer, ')'))
          {
            fechaP++;
          }

          strncat(tk.valor, buffer, 1);
          buffer++;
        }

        if (
            fechaP - abreP == 0 &&
            check(buffer - 1, 'n') &&
            check(buffer - 2, 'e') &&
            check(buffer - 3, 'h') &&
            check(buffer - 4, 't'))
        {
          // strncat(tk.valor, buffer, 1);
          buffer++;
          tk.tipo = SEM_ERRO;
          tk.linha = sinLinha;
          return tk;
        }
        else
        {
          strncat(tk.valor, buffer, 1);
          buffer++;
          tk.tipo = SIN_ERROR;
          tk.linha = sinLinha;
          return tk;
        }
      }

      // else
      if (check(buffer, 'e') && check(buffer + 1, 'l') && check(buffer + 2, 's') && check(buffer + 3, 'e') && check(buffer + 4, '\n'))
      {
        strncat(tk.valor, buffer, 4);
        buffer = buffer + 4;
        tk.linha = sinLinha;
        tk.tipo = SEM_ERRO;
        return tk;
      }

      // atribuição de vars
      if (isalpha(*buffer) && check(buffer + 1, ' ') && check(buffer + 2, '<') && check(buffer + 3, '-') && check(buffer + 4, ' '))
      {
        strncat(tk.valor, buffer, 5);
        buffer = buffer + 5;

        while (!check(buffer, ';'))
        {
          if (check(buffer, '\n'))
          {
            strncat(tk.valor, buffer, 1);
            buffer++;
            tk.tipo = SIN_ERROR;
            tk.linha = sinLinha;
            return tk;
          }

          strncat(tk.valor, buffer, 1);
          buffer++;
        }

        if (check(buffer, ';') && check(buffer + 1, '\n'))
        {
          strncat(tk.valor, buffer, 1);
          buffer = buffer + 2;
          tk.tipo = SEM_ERRO;
          tk.linha = sinLinha;
          return tk;
        }
        else
        {
          strncat(tk.valor, buffer, 1);
          buffer++;
          tk.tipo = SIN_ERROR;
          tk.linha = sinLinha;
          return tk;
        }
      }

      // end;
      if (check(buffer, 'e') && check(buffer + 1, 'n') && check(buffer + 2, 'd') && check(buffer + 3, ';'))
      {
        strncat(tk.valor, buffer, 4);
        buffer = buffer + 5;
        tk.tipo = SEM_ERRO;
        tk.linha = sinLinha;
        return tk;
      }

      // end.
      if (check(buffer, 'e') && check(buffer + 1, 'n') && check(buffer + 2, 'd') && check(buffer + 3, '.'))
      {
        strncat(tk.valor, buffer, 4);
        buffer = buffer + 4;
        tk.tipo = SEM_ERRO;
        tk.linha = sinLinha;
        return tk;
      }

      // error
      else
      {
        tk.tipo = SIN_ERROR;
      }
      strncat(tk.valor, buffer, 1);
      buffer++;
    }
  }
  else if ((*buffer == EOF) || (*buffer == '\x0'))
  {
    if (debugMode)
      strncat(tk.valor, "EOF", 1);
    tk.tipo = SIN_EOS;
  }
  else
  {
    strncat(tk.valor, buffer, 1);
    tk.tipo = SIN_ERROR;
  }

  tk.linha = sinLinha;
  return tk;
}