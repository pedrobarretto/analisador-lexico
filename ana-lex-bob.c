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
  OPR_INT,
  OPR_IF
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
    q16:
      if (*buffer == 'e') {
        linha++;
        buffer++;
        goto q26;
      }
    q17:
      if (*buffer == 'n') {
        linha++;
        buffer++;
        goto q30;
      }
      if (*buffer == 'f') {
        linha++;
        buffer++;
        tipo = OPR_IF;
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
    q28:
      if (*buffer == 'c') {
        linha++;
        buffer++;
        tipo = OPR_SEMIC; // semic
        goto q65;
      }
    q30:
      if (*buffer == 't') {
        linha++;
        buffer++;
        tipo = OPR_INT;
        goto q65;
      }
    q65:
      if (*buffer == ' ') {
        linha++;
        buffer++;
        return tipo;
      }
    erro:
      tipo = ERRO;
      printf("%s\n", "Deu erro!");
      return tipo;

  } while (tipo != ERRO);
  
}