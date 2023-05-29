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
#include <signal.h>

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
  ERRO_SINTATICO,
  END_OF_FILE
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
    "ERRO_SINTATICO",
    "FIM DO ARQUIVO"};

char *lookahead; /* Excepcionalmente variavel global */

int match(char *t, char *palavra, int *counter, char *erroSintatico);

int S(char *palavra, int *counter, char *erroSintatico);
int DFUNC(char *palavra, int *counter, char *erroSintatico);
int COMENTARIO(char *palavra, int *counter, char *erroSintatico);
int PARAMSF(char *palavra, int *counter, char *erroSintatico);
int TIPO(char *palavra, int *counter, char *erroSintatico);
int DVAR(char *palavra, int *counter, char *erroSintatico);
int DVAR_LIST(char *palavra, int *counter, char *erroSintatico);
int BLOCO(char *palavra, int *counter, char *erroSintatico);
int ATRIB(char *palavra, int *counter, char *erroSintatico);
int PROC(char *palavra, int *counter, char *erroSintatico);
int COND(char *palavra, int *counter, char *erroSintatico);
int REPET(char *palavra, int *counter, char *erroSintatico);
int PRINT(char *palavra, int *counter, char *erroSintatico);
int EXPR(char *palavra, int *counter, char *erroSintatico);
int RELACAO(char *palavra, int *counter, char *erroSintatico);
int IDENTIFICADOR_LIST(char *palavra, int *counter, char *erroSintatico);

char *scanner(char *lexema, int *counter, char *erroSintatico);

int match(char *t, char *palavra, int *counter, char *erroSintatico)
{
  if (lookahead->token == t)
  {
    lookahead = scanner(palavra, counter, erroSintatico);
    if (lookahead->token == tokenToStr[_COMENTARIO])
    {
      COMENTARIO(palavra, counter, erroSintatico);
    }
    return 1;
  }

  // lookahead = scanner(palavra, counter, erroSintatico);
  return 0;
}

// PARAMSF -> TIPO IDENTIFICADOR | NUMERO | true | false | TIPO IDENTIFICADOR , PARAMSF
int PARAMSF(char *palavra, int *counter, char *erroSintatico)
{
  if (lookahead->token == tokenToStr[NUM])
  {
    match(tokenToStr[NUM], palavra, counter, erroSintatico);
  }
  else if (lookahead->token == tokenToStr[TRUE])
  {
    match(tokenToStr[TRUE], palavra, counter, erroSintatico);
  }
  else if (lookahead->token == tokenToStr[FALSE])
  {
    match(tokenToStr[FALSE], palavra, counter, erroSintatico);
  }

  if (lookahead->token == tokenToStr[INT])
  {
    match(tokenToStr[INT], palavra, counter, erroSintatico);
    match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico);
  }
  else if (lookahead->token == tokenToStr[BOOL])
  {
    match(tokenToStr[BOOL], palavra, counter, erroSintatico);
    match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico);
  }

  if (lookahead->token == tokenToStr[VIRGULA])
  {
    match(tokenToStr[VIRGULA], palavra, counter, erroSintatico);
    PARAMSF(palavra, counter, erroSintatico);
  }
  else
  {
    return 1;
  }
}

// TIPO -> int, void, bool, semic
int TIPO(char *palavra, int *counter, char *erroSintatico)
{
  if (lookahead->token == tokenToStr[INT])
  {
    match(tokenToStr[INT], palavra, counter, erroSintatico);
  }
  else if (lookahead->token == tokenToStr[VOID])
  {
    match(tokenToStr[VOID], palavra, counter, erroSintatico);
  }
  else if (lookahead->token == tokenToStr[SEMIC])
  {
    match(tokenToStr[SEMIC], palavra, counter, erroSintatico);
  }
  else if (lookahead->token == tokenToStr[BOOL])
  {
    match(tokenToStr[BOOL], palavra, counter, erroSintatico);
  }
}

// IDENTIFICADOR_LIST -> IDENTIFICADOR | IDENTIFICADOR , IDENTIFICADOR_LIST
int IDENTIFICADOR_LIST(char *palavra, int *counter, char *erroSintatico)
{
  match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico);
  if (lookahead->token == tokenToStr[VIRGULA])
  {
    match(tokenToStr[VIRGULA], palavra, counter, erroSintatico);
    match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico);
  }
}

// DVAR -> TIPO IDENTIFICADOR_LIST ;
int DVAR(char *palavra, int *counter, char *erroSintatico)
{
  TIPO(palavra, counter, erroSintatico);
  match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico);

  if (lookahead->token == tokenToStr[VIRGULA])
  {
    match(tokenToStr[VIRGULA], palavra, counter, erroSintatico);
    IDENTIFICADOR_LIST(palavra, counter, erroSintatico);
  }

  match(tokenToStr[PONTO_E_VIRGULA], palavra, counter, erroSintatico);
}

// DVAR_LIST -> DVAR | DVAR DVAR_LIST | ATRIB | e
int DVAR_LIST(char *palavra, int *counter, char *erroSintatico)
{
  if (lookahead->token == tokenToStr[INT] || lookahead->token == tokenToStr[BOOL])
  {
    DVAR(palavra, counter, erroSintatico);
  }
  else if (lookahead->token == tokenToStr[IDENTIFICADOR])
  {
    ATRIB(palavra, counter, erroSintatico);
    DVAR_LIST(palavra, counter, erroSintatico);
  }
  else
  {
    return 0;
  }
}

// COMENTARIO -> /* [a, z] [1, 9]  */
int COMENTARIO(char *palavra, int *counter, char *erroSintatico)
{
  match(tokenToStr[_COMENTARIO], palavra, counter, erroSintatico);
}

// ATRIB -> IDENTIFICADOR = IDENTIFICADOR ; | IDENTIFICADOR = NUMERO ; | IDENTIFICADOR = IDENTIFICADOR + NUMERO ; | IDENTIFICADOR = NUMERO + IDENTIFICADOR ; | IDENTIFICADOR = true ; | IDENTIFICADOR = false ;
int ATRIB(char *palavra, int *counter, char *erroSintatico)
{
  match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico);
  match(tokenToStr[ATRIBUICAO], palavra, counter, erroSintatico);
  // Primeiro operando
  if (lookahead->token == tokenToStr[NUM])
  {
    match(tokenToStr[NUM], palavra, counter, erroSintatico);
  }
  else if (lookahead->token == tokenToStr[IDENTIFICADOR])
  {
    match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico);
  }

  // Operadores
  if (lookahead->token == tokenToStr[OPR_ADICAO])
  {
    match(tokenToStr[OPR_ADICAO], palavra, counter, erroSintatico);
  }
  else if (lookahead->token == tokenToStr[OPR_SUBTRACAO])
  {
    match(tokenToStr[OPR_SUBTRACAO], palavra, counter, erroSintatico);
  }
  else if (lookahead->token == tokenToStr[OPR_MULTIPLICACAO])
  {
    match(tokenToStr[OPR_MULTIPLICACAO], palavra, counter, erroSintatico);
  }
  else if (lookahead->token == tokenToStr[OPR_DIVISAO])
  {
    match(tokenToStr[OPR_DIVISAO], palavra, counter, erroSintatico);
  }

  // Segundo operando
  if (lookahead->token == tokenToStr[NUM])
  {
    match(tokenToStr[NUM], palavra, counter, erroSintatico);
  }
  else if (lookahead->token == tokenToStr[IDENTIFICADOR])
  {
    match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico);
  }

  else if (lookahead->token == tokenToStr[FALSE])
  {
    match(tokenToStr[FALSE], palavra, counter, erroSintatico);
  }
  else if (lookahead->token == tokenToStr[TRUE])
  {
    match(tokenToStr[TRUE], palavra, counter, erroSintatico);
  }

  match(tokenToStr[PONTO_E_VIRGULA], palavra, counter, erroSintatico);
}

// PROC -> proc IDENTIFICADOR | “proc” IDENTIFICADOR ( PARAMSF ) ;
int PROC(char *palavra, int *counter, char *erroSintatico)
{
  match(tokenToStr[_PROC], palavra, counter, erroSintatico);
  match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico);
  if (lookahead->token == tokenToStr[ABRE_PARENTESIS])
  {
    match(tokenToStr[ABRE_PARENTESIS], palavra, counter, erroSintatico);
    PARAMSF(palavra, counter, erroSintatico);
    match(tokenToStr[FECHA_PARENTESIS], palavra, counter, erroSintatico);
  }
  match(tokenToStr[PONTO_E_VIRGULA], palavra, counter, erroSintatico);
}

// PRINT -> print ( IDENTIFICADOR ) ;
int PRINT(char *palavra, int *counter, char *erroSintatico)
{
  match(tokenToStr[_PRINT], palavra, counter, erroSintatico);
  match(tokenToStr[ABRE_PARENTESIS], palavra, counter, erroSintatico);
  match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico);
  match(tokenToStr[FECHA_PARENTESIS], palavra, counter, erroSintatico);
  match(tokenToStr[PONTO_E_VIRGULA], palavra, counter, erroSintatico);
}

// REPET -> do { BLOCO } while ( EXPR ) ;
int REPET(char *palavra, int *counter, char *erroSintatico)
{
  match(tokenToStr[DO], palavra, counter, erroSintatico);
  match(tokenToStr[ABRE_CHAVES], palavra, counter, erroSintatico);
  BLOCO(palavra, counter, erroSintatico);
  match(tokenToStr[FECHA_CHAVES], palavra, counter, erroSintatico);
  match(tokenToStr[WHILE], palavra, counter, erroSintatico);
  match(tokenToStr[ABRE_PARENTESIS], palavra, counter, erroSintatico);
  EXPR(palavra, counter, erroSintatico);
  match(tokenToStr[FECHA_PARENTESIS], palavra, counter, erroSintatico);
  match(tokenToStr[PONTO_E_VIRGULA], palavra, counter, erroSintatico);
}

// BLOCO -> DVAR_LIST BLOCO | PROC BLOCO | COND BLOCO | PRINT BLOCO | REPET BLOCO | e
int BLOCO(char *palavra, int *counter, char *erroSintatico)
{
  // Lista de váriáveis + atribuição
  if (lookahead->token == tokenToStr[INT] || lookahead->token == tokenToStr[BOOL] || lookahead->token == tokenToStr[IDENTIFICADOR])
  {
    DVAR_LIST(palavra, counter, erroSintatico);
    BLOCO(palavra, counter, erroSintatico);
  }
  // Proc
  else if (lookahead->token == tokenToStr[_PROC])
  {
    PROC(palavra, counter, erroSintatico);
    BLOCO(palavra, counter, erroSintatico);
  }
  // If
  else if (lookahead->token == tokenToStr[IF])
  {
    COND(palavra, counter, erroSintatico);
    BLOCO(palavra, counter, erroSintatico);
  }
  // Print
  else if (lookahead->token == tokenToStr[_PRINT])
  {
    PRINT(palavra, counter, erroSintatico);
    BLOCO(palavra, counter, erroSintatico);
  }
  // Do while
  else if (lookahead->token == tokenToStr[DO])
  {
    REPET(palavra, counter, erroSintatico);
    BLOCO(palavra, counter, erroSintatico);
  }
  else
  {
    return 0;
  }
}

// RELAÇÃO -> == | <> | < | <= | >= | >
int RELACAO(char *palavra, int *counter, char *erroSintatico)
{
  if (lookahead->token == tokenToStr[IGUALDADE])
  {
    match(tokenToStr[IGUALDADE], palavra, counter, erroSintatico);
  }
  else if (lookahead->token == tokenToStr[DIFERENCA])
  {
    match(tokenToStr[DIFERENCA], palavra, counter, erroSintatico);
  }
  else if (lookahead->token == tokenToStr[MENOR_QUE])
  {
    match(tokenToStr[MENOR_QUE], palavra, counter, erroSintatico);
  }
  else if (lookahead->token == tokenToStr[MENOR_OU_IGUAL_QUE])
  {
    match(tokenToStr[MENOR_OU_IGUAL_QUE], palavra, counter, erroSintatico);
  }
  else if (lookahead->token == tokenToStr[MAIOR_QUE])
  {
    match(tokenToStr[MAIOR_QUE], palavra, counter, erroSintatico);
  }
  else if (lookahead->token == tokenToStr[MAIOR_OU_IGUAL_QUE])
  {
    match(tokenToStr[MAIOR_OU_IGUAL_QUE], palavra, counter, erroSintatico);
  }
}

// EXPR -> true | false | IDENTIFICADOR RELACAO IDENTIFICADOR | IDENTIFICADOR RELACAO NUMERO | IDENTIFICADOR RELACAO false | IDENTIFICADOR RELACAO true
int EXPR(char *palavra, int *counter, char *erroSintatico)
{
  if (lookahead->token == tokenToStr[TRUE])
  {
    match(tokenToStr[TRUE], palavra, counter, erroSintatico);
  }
  else if (lookahead->token == tokenToStr[FALSE])
  {
    match(tokenToStr[FALSE], palavra, counter, erroSintatico);
  }

  if (lookahead->token == tokenToStr[IDENTIFICADOR])
  {
    match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico);
    RELACAO(palavra, counter, erroSintatico);
    if (lookahead->token == tokenToStr[IDENTIFICADOR])
    {
      match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico);
    }
    else if (lookahead->token == tokenToStr[NUM])
    {
      match(tokenToStr[NUM], palavra, counter, erroSintatico);
    }
    else if (lookahead->token == tokenToStr[TRUE])
    {
      match(tokenToStr[TRUE], palavra, counter, erroSintatico);
    }
    else if (lookahead->token == tokenToStr[FALSE])
    {
      match(tokenToStr[FALSE], palavra, counter, erroSintatico);
    }
  }
}

// COND -> if ( EXPR ) { BLOCO } | if ( EXPR ) { BLOCO } else { BLOCO } ;
int COND(char *palavra, int *counter, char *erroSintatico)
{
  match(tokenToStr[IF], palavra, counter, erroSintatico);
  match(tokenToStr[ABRE_PARENTESIS], palavra, counter, erroSintatico);
  EXPR(palavra, counter, erroSintatico);
  match(tokenToStr[FECHA_PARENTESIS], palavra, counter, erroSintatico);
  match(tokenToStr[ABRE_CHAVES], palavra, counter, erroSintatico);
  BLOCO(palavra, counter, erroSintatico);
  match(tokenToStr[FECHA_CHAVES], palavra, counter, erroSintatico);

  if (lookahead->token == tokenToStr[ELSE])
  {
    match(tokenToStr[ELSE], palavra, counter, erroSintatico);
    match(tokenToStr[ABRE_CHAVES], palavra, counter, erroSintatico);
    BLOCO(palavra, counter, erroSintatico);
    match(tokenToStr[FECHA_CHAVES], palavra, counter, erroSintatico);
  }

  match(tokenToStr[PONTO_E_VIRGULA], palavra, counter, erroSintatico);
}

// DFUNC -> TIPO IDENTIFICADOR ( PARAMSF ) BLOCO | TIPO IDENTIFICADOR ( PARAMSF ) BLOCO DFUNC | e
int DFUNC(char *palavra, int *counter, char *erroSintatico)
{
  if (lookahead->token == tokenToStr[SEMIC])
  {
    TIPO(palavra, counter, erroSintatico);
    match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico);
    match(tokenToStr[ABRE_CHAVES], palavra, counter, erroSintatico);
    BLOCO(palavra, counter, erroSintatico);
    match(tokenToStr[FECHA_CHAVES], palavra, counter, erroSintatico);
  }
  else if (lookahead->token == tokenToStr[VOID] || lookahead->token == tokenToStr[INT] || lookahead->token == tokenToStr[BOOL])
  {
    TIPO(palavra, counter, erroSintatico);
    match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico);
    match(tokenToStr[ABRE_PARENTESIS], palavra, counter, erroSintatico);
    PARAMSF(palavra, counter, erroSintatico);
    match(tokenToStr[FECHA_PARENTESIS], palavra, counter, erroSintatico);
    match(tokenToStr[ABRE_CHAVES], palavra, counter, erroSintatico);
    BLOCO(palavra, counter, erroSintatico);
    match(tokenToStr[FECHA_CHAVES], palavra, counter, erroSintatico);

    if (lookahead->token == tokenToStr[SEMIC])
    {
      DFUNC(palavra, counter, erroSintatico);
    }

    // TODO: if lookahead != SEMIC, validar outras funcs.
  }
}

// S -> DFUNC | e
int S(char *palavra, int *counter, char *erroSintatico)
{
  return DFUNC(palavra, counter, erroSintatico);
}

int main()
{

  FILE *entrada;
  entrada = fopen("entrada.txt", "r");

  if (entrada == NULL)
  {
    printf("Erro ao tentar abrir arquivo...\n");
    return 1;
  }

  fseek(entrada, 0L, SEEK_END);
  int tamanho = ftell(entrada);
  rewind(entrada);

  char *palavra = (char *)malloc(sizeof(char) * (tamanho + 1));
  fread(palavra, sizeof(char), tamanho, entrada);
  palavra[tamanho] = '\0';

  fclose(entrada);

  char erroSintatico[100];

  int counter = 0;
  lookahead = scanner(palavra, &counter, erroSintatico);
  int result = S(palavra, &counter, erroSintatico);
  // printf("Result: %i\n", result);
  // printf("lookahead: %s \n", lookahead);
  if (lookahead->token == tokenToStr[ERRO_SINTATICO])
  {
    printf("Programa apresenta erros sintáticos em:\n");
    int i;
    for (i = 0; i < strlen(erroSintatico); i++)
    {
      printf("%c", erroSintatico[i]);
    }
    printf("\n");
  }
  else
  {
    printf("Programa sem erros sintáticos!\n");
  }

  return 0;
}

char *scanner(char *lexema, int *counter, char *erroSintatico)
{
  char digit = lexema[*counter];

  if (digit == EOF || digit == '\x0')
  {
    return tokenToStr[END_OF_FILE];
  }

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
  if (*counter > 20)
  {
    int start = 0;
    int end = *counter;

    if (*counter > 40)
    {
      start = *counter - (2 * 100 / 3);
      if (start < 0)
      {
        start = 0;
      }
    }

    int size = end - start + 5;

    strncpy(erroSintatico, lexema + start, size);
    erroSintatico[size] = '\0';
  }

  return tokenToStr[ERRO_SINTATICO];
}