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
  OPR_VOID
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

int scanner(char *palavra[], TOKEN_TYPE tipo);

int linha = 0; // Contador de linhas

int main(int argc, char *argv[])
{
  printf("%s\n", "to rodando");
  char *prvoid[4] = {"v", "o", "i", "d"};
  printf("%s\n", prvoid[0]);
  scanner(prvoid, OPR_VOID);

  return 0;
}

int scanner(char *palavra[], TOKEN_TYPE tipo) {
  char c;

  LOOP:do {
    q0:
      c = palavra[linha];
      return _ACEITA_;
    erro:
      return _REJEITA_;

  } while (tipo != ERRO);
  
}