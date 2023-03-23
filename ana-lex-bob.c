#pragma warning(disable : 4996)

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0

typedef enum {
  ERRO
} TOKEN_TYPE;

typedef struct
{
	TOKEN_TYPE tipo;
	int linha;
	char valor[15];
} TOKEN;

char *buffer;
int linha = 1;
short debugMode = FALSE;
TOKEN_TYPE tokenStep = ERRO;

TOKEN scanner();

int main(int argc, char *argv[]) {

  return 0;
}

TOKEN scanner() {
  TOKEN tk;

  char c;


  return tk;
}