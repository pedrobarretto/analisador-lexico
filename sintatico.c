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

typedef enum
{
  _PRINT,
  _PROC,
  VOID,
  WHILE,
  ELSE,
  FALSE,
  TRUE,
  DO,
  BOOL,
  IF,
  INT,
  SEMIC,
  FECHA_CHAVES,
  ABRE_CHAVES,
  ABRE_PARENTESIS,
  FECHA_PARENTESIS,
  VIRGULA,
  PONTO_E_VIRGULA,
  OPR_ADICAO,
  OPR_SUBTRACAO,
  OPR_MULTIPLICACAO,
  OPR_DIVISAO,
  MENOR_QUE,
  DIFERENCA,
  MENOR_OU_IGUAL_QUE,
  MAIOR_QUE,
  MAIOR_OU_IGUAL_QUE,
  ATRIBUICAO,
  IGUALDADE,
  IDENTIFICADOR,
  NUM,
  _COMENTARIO,
  ERRO_LEXICO
} Token;

char *tokenToStr[] = {
    "PRINT",
    "PROC",
    "VOID",
    "WHILE",
    "ELSE",
    "FALSE",
    "TRUE",
    "DO",
    "BOOL",
    "IF",
    "INT",
    "SEMIC",
    "FECHA_CHAVES",
    "ABRE_CHAVES",
    "ABRE_PARENTESIS",
    "FECHA_PARENTESIS",
    "VIRGULA",
    "PONTO_E_VIRGULA",
    "OPR_ADICAO",
    "OPR_SUBTRACAO",
    "OPR_MULTIPLICACAO",
    "OPR_DIVISAO",
    "MENOR_QUE",
    "DIFERENCA",
    "MENOR_OU_IGUAL_QUE",
    "MAIOR_QUE",
    "MAIOR_OU_IGUAL_QUE",
    "ATRIBUICAO",
    "IGUALDADE",
    "IDENTIFICADOR",
    "NUM",
    "COMENTARIO",
    "ERRO_LEXICO"};

char *lookahead; /* Excepcionalmente variavel global */

int match(char *t, char *palavra, int *counter);

int S(char *palavra, int *counter);

int DFUNC(char *palavra, int *counter);

int COMENTARIO(char *palavra, int *counter);
int PARAMSF(char *palavra, int *counter);
int TIPO(char *palavra, int *counter);
int DVAR(char *palavra, int *counter);
int DVAR_LIST(char *palavra, int *counter);
int BLOCO(char *palavra, int *counter);
int COMANDO_COMPOSTO(char *palavra, int *counter);
int COMANDO(char *palavra, int *counter);
int ATRIB(char *palavra, int *counter);
int PROC(char *palavra, int *counter);
int COND(char *palavra, int *counter);
int REPET(char *palavra, int *counter);
int PRINT(char *palavra, int *counter);

char *scanner(char *lexema, int *counter);

int match(char *t, char *palavra, int *counter)
{
  printf("lookahead: %s ", lookahead);
  printf("   t: %s\n", t);

  if (lookahead == t)
  {
    lookahead = scanner(palavra, counter);
    printf("Match!\n");
    return (1);
  }

  lookahead = scanner(palavra, counter);
  printf("Not Match!\n");
  return (0);
}

int PARAMSF(char *palavra, int *counter)
{
  if (lookahead == tokenToStr[INT])
  {
    match(tokenToStr[INT], palavra, counter);
    match(tokenToStr[IDENTIFICADOR], palavra, counter);
  }
  else if (lookahead == tokenToStr[BOOL])
  {
    match(tokenToStr[BOOL], palavra, counter);
    match(tokenToStr[IDENTIFICADOR], palavra, counter);
  }

  // Params do proc
  // if (lookahead == tokenToStr[NUM])
  // {
  //   match(tokenToStr[NUM], palavra, counter);
  // }
  // else if (lookahead == tokenToStr[TRUE])
  // {
  //   match(tokenToStr[TRUE], palavra, counter);
  // }
  // else if (lookahead == tokenToStr[FALSE])
  // {
  //   match(tokenToStr[FALSE], palavra, counter);
  // }

  if (lookahead == tokenToStr[VIRGULA])
  {
    match(tokenToStr[VIRGULA], palavra, counter);
    PARAMSF(palavra, counter);
  }
  else
  {
    return 1;
  }
}

int TIPO(char *palavra, int *counter)
{
  if (lookahead == tokenToStr[INT])
  {
    match(tokenToStr[INT], palavra, counter);
  }
  else if (lookahead == tokenToStr[VOID])
  {
    match(tokenToStr[VOID], palavra, counter);
  }
  else if (lookahead == tokenToStr[BOOL])
  {
    match(tokenToStr[BOOL], palavra, counter);
  }
  else if (lookahead == tokenToStr[SEMIC])
  {
    match(tokenToStr[BOOL], palavra, counter);
  }
}

int DVAR(char *palavra, int *counter)
{
  TIPO(palavra, counter);
  match(tokenToStr[IDENTIFICADOR], palavra, counter);
  match(tokenToStr[PONTO_E_VIRGULA], palavra, counter);
}

int DVAR_LIST(char *palavra, int *counter)
{
  if (lookahead == tokenToStr[INT] || lookahead == tokenToStr[BOOL])
  {
    DVAR(palavra, counter);
  }

  if (lookahead == tokenToStr[IDENTIFICADOR])
  {
    // Atribuição de valores a variáveis declaradas previamente
    ATRIB(palavra, counter);
    DVAR_LIST(palavra, counter);
  }
  else if (lookahead == tokenToStr[INT] || lookahead == tokenToStr[BOOL])
  {
    DVAR_LIST(palavra, counter);
  }
  else
  {
    return 0;
  }
}

int COMENTARIO(char *palavra, int *counter)
{
  match(tokenToStr[_COMENTARIO], palavra, counter);
}

int ATRIB(char *palavra, int *counter)
{
  match(tokenToStr[IDENTIFICADOR], palavra, counter);
  match(tokenToStr[ATRIBUICAO], palavra, counter);
  if (lookahead == tokenToStr[NUM])
  {
    match(tokenToStr[NUM], palavra, counter);
  }
  else if (lookahead == tokenToStr[FALSE])
  {
    match(tokenToStr[FALSE], palavra, counter);
  }
  else if (lookahead == tokenToStr[BOOL])
  {
    match(tokenToStr[BOOL], palavra, counter);
  }
  match(tokenToStr[PONTO_E_VIRGULA], palavra, counter);
}

int PROC(char *palavra, int *counter)
{
  match(tokenToStr[_PROC], palavra, counter);
  match(tokenToStr[IDENTIFICADOR], palavra, counter);
  if (lookahead == tokenToStr[ABRE_PARENTESIS])
  {
    match(tokenToStr[ABRE_PARENTESIS], palavra, counter);
    PARAMSF(palavra, counter);
    match(tokenToStr[FECHA_PARENTESIS], palavra, counter);
  }
  match(tokenToStr[PONTO_E_VIRGULA], palavra, counter);
}

int BLOCO(char *palavra, int *counter)
{
  // Bloco aqui
  if (lookahead == tokenToStr[INT] || lookahead == tokenToStr[BOOL] || lookahead == tokenToStr[IDENTIFICADOR])
  {
    DVAR_LIST(palavra, counter);
    BLOCO(palavra, counter);
  }
  if (lookahead == tokenToStr[_PROC])
  {
    PROC(palavra, counter);
    BLOCO(palavra, counter);
  }
}

int DFUNC(char *palavra, int *counter)
{
  if (lookahead == tokenToStr[SEMIC])
  {
    TIPO(palavra, counter);
    match(tokenToStr[IDENTIFICADOR], palavra, counter);
    match(tokenToStr[ABRE_CHAVES], palavra, counter);
    BLOCO(palavra, counter);
    match(tokenToStr[FECHA_CHAVES], palavra, counter);
  }
  else if (lookahead == tokenToStr[VOID] || lookahead == tokenToStr[INT] || lookahead == tokenToStr[BOOL])
  {
    TIPO(palavra, counter);
    match(tokenToStr[IDENTIFICADOR], palavra, counter);
    match(tokenToStr[ABRE_PARENTESIS], palavra, counter);
    PARAMSF(palavra, counter);
    match(tokenToStr[FECHA_PARENTESIS], palavra, counter);
    match(tokenToStr[ABRE_CHAVES], palavra, counter);
    BLOCO(palavra, counter);
    match(tokenToStr[FECHA_CHAVES], palavra, counter);

    // TODO: if lookahead != SEMIC, validar outras funcs.
  }
}

int S(char *palavra, int *counter)
{
  DFUNC(palavra, counter);
}

int main()
{

  FILE *entrada;
  entrada = fopen("entrada.txt", "r");

  if (entrada == NULL)
  {
    printf("Error opening file.\n");
    return 1;
  }

  fseek(entrada, 0L, SEEK_END);
  int tamanho = ftell(entrada);
  rewind(entrada);

  char *palavra = (char *)malloc(sizeof(char) * (tamanho + 1));
  fread(palavra, sizeof(char), tamanho, entrada);
  palavra[tamanho] = '\0';

  fclose(entrada);

  int counter = 0;
  lookahead = scanner(palavra, &counter);
  S(palavra, &counter);

  return 0;
}

char *scanner(char *lexema, int *counter)
{
  char digit = lexema[*counter];

q0:
  if (digit == ' ' || digit == '\t' || digit == '\r' || digit == '\n')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q0;
  }
  if (isdigit(digit))
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q99;
  }
  if (digit == '*')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q73;
  }
  if (digit == '+')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q69;
  }
  if (digit == '-')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q71;
  }
  if (digit == '/')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q75;
  }
  if (digit == ';')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q67;
  }
  if (digit == ',')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q65;
  }
  if (digit == '(')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q61;
  }
  if (digit == ')')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q63;
  }
  if (digit == '{')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q59;
  }
  if (digit == '}')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q57;
  }
  if (digit == '_')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q93;
  }
  if (digit == '=')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q89;
  }
  if (digit == '<')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q79;
  }
  if (digit == '>')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q85;
  }
  if (digit == 's')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q51;
  }
  if (digit == 'i')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q45;
  }
  if (digit == 'b')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q40;
  }
  if (digit == 'd')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q37;
  }
  if (digit == 't')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q32;
  }
  if (digit == 'f')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q26;
  }
  if (digit == 'e')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q21;
  }
  if (digit == 'w')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q15;
  }
  if (digit == 'v')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q10;
  }
  if (digit == 'p')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q1;
  }
  else
  {
    goto erro;
  }
q1:
  if (digit == 'r')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q2;
  }
q2:
  if (digit == 'i')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q3;
  }
  if (digit == 'o')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q7;
  }
q3:
  if (digit == 'n')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q4;
  }
q4:
  if (digit == 't')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q5;
  }
q5:
  if (digit == ' ')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q6;
  }
  else
  {
    goto erro;
  }
q6: // print
  return tokenToStr[_PRINT];
q7:
  if (digit == 'c')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q8;
  }
q8:
  if (digit == ' ')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q9;
  }
  else
  {
    goto erro;
  }
q9: // proc
  return tokenToStr[_PROC];
q10:
  if (digit == 'o')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q11;
  }
q11:
  if (digit == 'i')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q12;
  }
q12:
  if (digit == 'd')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q13;
  }
q13:
  if (digit == ' ')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q14;
  }
  else
  {
    goto erro;
  }
q14: // void
  return tokenToStr[VOID];
q15:
  if (digit == 'h')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q16;
  }
q16:
  if (digit == 'i')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q17;
  }
q17:
  if (digit == 'l')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q18;
  }
q18:
  if (digit == 'e')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q19;
  }
q19:
  if (digit == ' ')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q20;
  }
  else
  {
    goto erro;
  }
q20: // while
  return tokenToStr[WHILE];
q21:
  if (digit == 'l')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q22;
  }
q22:
  if (digit == 's')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q23;
  }
q23:
  if (digit == 'e')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q24;
  }
q24:
  if (digit == ' ')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q25;
  }
  else
  {
    goto erro;
  }
q25: // else
  return tokenToStr[ELSE];
q26:
  if (digit == 'a')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q27;
  }
q27:
  if (digit == 'l')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q28;
  }
q28:
  if (digit == 's')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q29;
  }
q29:
  if (digit == 'e')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q30;
  }
q30:
  if (digit == ' ')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q31;
  }
  else
  {
    goto erro;
  }
q31: // false
  return tokenToStr[FALSE];
q32:
  if (digit == 'r')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q33;
  }
q33:
  if (digit == 'u')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q34;
  }
q34:
  if (digit == 'e')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q35;
  }
q35:
  if (digit == ' ')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q36;
  }
  else
  {
    goto erro;
  }
q36: // true
  return tokenToStr[TRUE];
q37:
  if (digit == 'o')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q38;
  }
q38:
  if (digit == ' ')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q39;
  }
  else
  {
    goto erro;
  }
q39: // do
  return tokenToStr[DO];
q40:
  if (digit == 'o')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q41;
  }
q41:
  if (digit == 'o')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q42;
  }
q42:
  if (digit == 'l')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q43;
  }
q43:
  if (digit == ' ')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q44;
  }
  else
  {
    goto erro;
  }
q44: // bool
  return tokenToStr[BOOL];
q45:
  if (digit == 'n')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q48;
  }
  if (digit == 'f')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q46;
  }
q46:
  if (digit == ' ')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q47;
  }
  else
  {
    goto erro;
  }
q47: // if
  return tokenToStr[IF];
q48:
  if (digit == 't')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q49;
  }
q49:
  if (digit == ' ')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q50;
  }
  else
  {
    goto erro;
  }
q50: // int
  return tokenToStr[INT];
q51:
  if (digit == 'e')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q52;
  }
q52:
  if (digit == 'm')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q53;
  }
q53:
  if (digit == 'i')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q54;
  }
q54:
  if (digit == 'c')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q55;
  }
q55:
  if (digit == ' ')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q56;
  }
  else
  {
    goto erro;
  }
q56: // semic
  return tokenToStr[SEMIC];
q57:
  if (digit == ' ')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q58;
  }
  else
  {
    goto erro;
  }
q58: // }
  return tokenToStr[FECHA_CHAVES];
q59:
  if (digit == ' ')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q60;
  }
  else
  {
    goto erro;
  }
q60: // {
  return tokenToStr[ABRE_CHAVES];
q61:
  if (digit == ' ')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q62;
  }
  else
  {
    goto erro;
  }
q62: // (
  return tokenToStr[ABRE_PARENTESIS];
q63:
  if (digit == ' ')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q64;
  }
  else
  {
    goto erro;
  }
q64: // )
  return tokenToStr[FECHA_PARENTESIS];
q65:
  if (digit == ' ')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q66;
  }
  else
  {
    goto erro;
  }
q66: // ,
  return tokenToStr[VIRGULA];
q67:
  if (digit == ' ')
  {
    goto q68;
  }
  else
  {
    goto erro;
  }
q68: // ;
  return tokenToStr[PONTO_E_VIRGULA];
q69:
  if (digit == ' ')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q70;
  }
  else
  {
    goto erro;
  }
q70: // +
  return tokenToStr[OPR_ADICAO];
q71:
  if (digit == ' ')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q72;
  }
  else
  {
    goto erro;
  }
q72: // -
  return tokenToStr[OPR_SUBTRACAO];
q73:
  if (digit == ' ')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q74;
  }
  else
  {
    goto erro;
  }
q74: // *
  return tokenToStr[OPR_MULTIPLICACAO];
q75:
  if (digit == ' ')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q76;
  }
  if (digit == '*')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q77;
  }
  else
  {
    goto erro;
  }
q76: // /
  return tokenToStr[OPR_DIVISAO];
q77:
  if (digit == ' ')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q78;
  }
  else
  {
    goto erro;
  }
q78:
  if (
      isalpha(digit) ||
      isalnum(digit) ||
      digit == '(' ||
      digit == ')' ||
      digit == '*' ||
      digit == '+' ||
      digit == '-' ||
      digit == ',' ||
      digit == '/' ||
      digit == ';' ||
      digit == '<' ||
      digit == '=' ||
      digit == '>' ||
      digit == '_' ||
      digit == '}' ||
      digit == '{' ||
      digit == ' ' || digit == '\t' || digit == '\r' || digit == '\n')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q94;
  }
q79:
  if (digit == ' ')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q80;
  }
  if (digit == '=')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q83;
  }
  if (digit == '>')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q81;
  }
  else
  {
    goto erro;
  }
q80: // <
  return tokenToStr[MENOR_QUE];
q81:
  if (digit == ' ')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q82;
  }
  else
  {
    goto erro;
  }
q82: // <>
  return tokenToStr[DIFERENCA];
q83:
  if (digit == ' ')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q84;
  }
  else
  {
    goto erro;
  }
q84: // <=
  return tokenToStr[MENOR_OU_IGUAL_QUE];
q85:
  if (digit == '=')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q87;
  }
  if (digit == ' ')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q86;
  }
  else
  {
    goto erro;
  }
q86: // >
  return tokenToStr[MAIOR_QUE];
q87:
  if (digit == ' ')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q88;
  }
  else
  {
    goto erro;
  }
q88: // >=
  return tokenToStr[MAIOR_OU_IGUAL_QUE];
q89:
  if (digit == '=')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q91;
  }
  if (digit == ' ')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q90;
  }
  else
  {
    goto erro;
  }
q90: // =
  return tokenToStr[ATRIBUICAO];
q91:
  if (digit == ' ')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q92;
  }
  else
  {
    goto erro;
  }
q92: // ==
  return tokenToStr[IGUALDADE];
q93:
  if (digit == ' ')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q98;
  }
  if (isalpha(digit))
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q93;
  }
  else
  {
    goto erro;
  }
q94:
  if (digit == '*' || digit == ' ' || digit == '\t' || digit == '\r' || digit == '\n') // Precisa dos espaços extras?
  {
    // TODO: Removi esse incremento e os comentários passaram. Avaliar depois
    // (*counter)++;
    // digit = lexema[*counter];
    goto q95;
  }
  if (
      isalpha(digit) ||
      isalnum(digit) ||
      digit == '(' ||
      digit == ')' ||
      digit == '+' ||
      digit == '-' ||
      digit == ',' ||
      digit == ';' ||
      digit == '<' ||
      digit == '=' ||
      digit == '>' ||
      digit == '_' ||
      digit == '}' ||
      digit == '{' ||
      digit == '/')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q78;
  }
q95:
  if (digit == '*' || digit == ' ' || digit == '\t' || digit == '\r' || digit == '\n')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q96;
  }
  if (
      isalpha(digit) ||
      isalnum(digit) ||
      digit == '(' ||
      digit == ')' ||
      digit == '+' ||
      digit == '-' ||
      digit == ',' ||
      digit == ';' ||
      digit == '<' ||
      digit == '=' ||
      digit == '>' ||
      digit == '_' ||
      digit == '}' ||
      digit == '{' ||
      digit == '*' ||
      digit == '/')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q94;
  }
  else
  {
    goto erro;
  }
q96:
  if (digit == '*')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q96;
  }
  if (digit == '/')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q97;
  }
  if (
      isalpha(digit) ||
      isalnum(digit) ||
      digit == '(' ||
      digit == ')' ||
      digit == '+' ||
      digit == '-' ||
      digit == ',' ||
      digit == ';' ||
      digit == '<' ||
      digit == '=' ||
      digit == '>' ||
      digit == '_' ||
      digit == '}' ||
      digit == '{' ||
      digit == ' ' || (digit == '\t') || (digit == '\r') || (digit == '\n'))
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q95;
  }
  else
  {
    goto erro;
  }
q97:
  if (digit == ' ')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q101;
  }
  else
  {
    goto erro;
  }
q98: // Identificador
  return tokenToStr[IDENTIFICADOR];
q99:
  if (isdigit(digit))
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q99;
  }
  if (digit == ' ')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q100;
  }
  else
  {
    goto erro;
  }
q100: // Digitos
  return tokenToStr[NUM];
q101: // Comentario
  return tokenToStr[_COMENTARIO];
erro: // Erro léxico
  return tokenToStr[ERRO_LEXICO];
}