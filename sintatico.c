// Renan Clepf Martins - TIA: 32020368
// Pedro Galvão Barretto - TIA: 32016591
// Jones Santos Czinczel - TIA: 32089041
// Luan Lemos - TIA: 32085109

#pragma warning(disable : 4996)

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

char lookahead; /* Excepcionalmente variavel global */

int match(char t, char palavra[], int *pos);
int parseInit();
int parseType();
int parseIdentifier();
int parseOpenParentesis();
int parseFuncParameters();
int parseCloseParantesis();

int parseInit()
{
  if (lookahead == 'SEMIC')
  {
    parseIdentifier();
  }
  parseType();
  parseIdentifier();
  parseOpenParentesis();
  parseFuncParameters();
  parseCloseParantesis();
  parseOpenBrackets();
}

int match(char t, char palavra[], int *pos)
{
  if (lookahead == t)
  {
    lookahead = palavra[++(*pos)];
    return (1);
  }
  return (0);
}

int main()
{

  FILE *arq = fopen("entrada.txt", "r");
  if (arq == NULL)
  {
    fprintf(stderr, "Erro ao tentar abrir o aquivo \"%s\"", "entrada.txt");
    return -1;
  }

  return 0;
}