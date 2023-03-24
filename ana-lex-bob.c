#pragma warning(disable : 4996)

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define _ACEITA_ 1
#define _REJEITA_ 0

#define TRUE 1
#define FALSE 0

typedef enum {
  ERRO,
  OPR_VOID,
  OPR_SEMIC,
  OPR_IF,
  OPR_ELSE,
  OPR_PRINT,
  OPR_TRUE,
  OPR_FALSE,
  OPR_DO,
  OPR_WHILE,
  OPR_PROC,
  OPR_INT,
  OPR_BOOL,
  IDENTIFICADOR,
  OPR_ADD,
  OPR_SUB,
  OPR_TIMES,
  OPR_DIV,
  OPR_LESS_THEN,
  OPR_LESS_OR_EQUAL_THEN,
  OPR_HIGHER_THEN,
  OPR_HIGHER_OR_EQUAL_THEN,
  OPR_EQUAL_TO,
  OPR_DIFERENT,
  OPR_EQUAL,
  OPR_VIRGULA,
  OPR_PONTO_E_VIRGULA,
  OPR_COMENT
} TOKEN_TYPE;

typedef struct
{
	TOKEN_TYPE tipo;
	int linha;
	char valor[15];
} TOKEN;

// Ao final de tudo, um espaco vazio, menos em comentario e delimitador

// char *prvoid[4] = {"v", "o", "i", "d"}; 
// for (int i = 0; i < 4; i++) {
//   printf("%s\n", "aqui");
//   if (prvoid[i] != palavra[linha]) {
//     printf("%s\n", "erro!");
//     goto erro;
//   }
//   linha++;
// }
// printf("%s\n", "passei");

int scanner(char *palavra, TOKEN_TYPE tipo);

int linha = 0; // Contador de linhas
char *buffer;
TOKEN_TYPE tipo;

int main(int argc, char *argv[])
{
  char *prvoid[4] = {"v", "o", "i", "d"};

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

	int tk;
	do
	{
		tk = scanner(buffer, tipo);
		// printf("\nLinha:%3d | %-30s", linha, tk);
    printf("%i\n", tk);
    buffer++;
	} while (tk != ERRO);

	return 0;
}

int scanner(char *palavra, TOKEN_TYPE tipo) {
  char c;
  // printf("%s\n", "dentro");
  LOOP:do {
    q0:
      if (*buffer == 's') {
        linha++;
        buffer++;
        goto q16;
      }
      if (*buffer == 'i') {
        linha++;
        buffer++;
        goto q17;
      }
      if (*buffer == 'b') {
        linha++;
        buffer++;
        goto q18;
      }
      if (*buffer == 'd') {
        linha++;
        buffer++;
        goto q19;
      }
      if (*buffer == 't') {
        linha++;
        buffer++;
        goto q37;
      }
    q16:
      if (*buffer == 'e') {
        linha++;
        buffer++;
        goto q26;
      }
    q17: // if
      if (*buffer == 'n') {
        linha++;
        buffer++;
        goto q30;
      }
      if (*buffer == 'f' && *buffer + 1 == ' ') {
        linha++;
        buffer++;
        tipo = OPR_IF;
        goto q65;
      }
    q18:
      if (*buffer == 'o') {
        linha++;
        buffer++;
        goto q33;
      }
    q19: // do
      if (*buffer == 'o') {
        linha++;
        buffer++;
        tipo = OPR_DO;
        goto q65;
      }
    q26:
      if (*buffer == 'm') {
        linha++;
        buffer++;
        goto q27;
      }
    q27:
      if (*buffer == 'i') {
        linha++;
        buffer++;
        goto q28;
      }
    q28: // semic
      if (*buffer == 'c' && *buffer + 1 == ' ') {
        linha++;
        buffer++;
        tipo = OPR_SEMIC;
        goto q65;
      }
    q30: // int
      if (*buffer == 't' && *buffer + 1 == ' ') {
        linha++;
        buffer++;
        tipo = OPR_INT;
        goto q65;
      }
    q33:
      if (*buffer == 'o') {
        linha++;
        buffer++;
        goto q34;
      }
    q34: // bool
      if (*buffer == 'l' && *buffer + 1 == ' ') {
        linha++;
        buffer++;
        tipo = OPR_BOOL;
        goto q65;
      }
    q37:
      if (*buffer == 'r') {
        linha++;
        buffer++;
        goto q43;
      }
    q43:
      if (*buffer == 'u') {
        linha++;
        buffer++;
        goto q44;
      }
    q44: // true
      if (*buffer == 'e' && *buffer + 1 == ' ') {
        linha++;
        buffer++;
        tipo = OPR_TRUE;
        goto q65;
      }
    q65: // final
      if (*buffer == ' ') {
        linha++;
        buffer++;
        return tipo;
      }
    erro: // erro
      tipo = ERRO;
      // printf("%s\n", "Deu erro!");
      return tipo;

  } while (tipo != ERRO);
  
}