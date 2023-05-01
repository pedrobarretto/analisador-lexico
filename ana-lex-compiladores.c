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

typedef struct
{
  char *string;
  char *lexema;
} Token;

Token *generateToken(const char *str, const char *lexema, Token *token);

Token *generateToken(const char *str, const char *lexema, Token *token)
{
  int str_len = strlen(str);
  token->string = malloc(str_len + 1); // access the string field using ->
  strcpy(token->string, str);
  token->lexema = lexema ? strdup(lexema) : NULL; // access the lexema field using ->
  return token;
}

Token scanner(char lexema[100], int *counter, Token *token);

int main()
{
  char palavra[100] = "if 2345 true ( /* ** _x int */ _x ) >= ";
  Token *token = NULL;
  int counter = 0;

  while (palavra[counter] != '\0')
  {
    scanner(palavra, &counter, token);

    if (token->lexema)
    {
      printf("LEXEMA: %s, TOKEN: %s\n", token->lexema, token->string);
    }
    else
    {
      printf("TOKEN: %s\n", token->string);
    }
  }

  return 0;
}

Token scanner(char lexema[100], int *counter, Token *token)
{
  char digit = lexema[*counter];
  printf("Valor de counter no comeco: %d\n", *counter);
  printf("Valor de digit no comeco: %c\n", digit);
  printf("lexema: %s\n", lexema);

q0:
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
    printf("Valor de counter antes do incremento: %d\n", *counter);
    (*counter)++;
    printf("Valor de counter depois do incremento: %d\n", *counter);
    digit = lexema[*counter];
    printf("Valor de digit: %c\n", digit);
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
    goto q6;
  }
  else
  {
    goto erro;
  }
q6: // print
  *token = generateToken("PRINT", NULL, token);
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
    goto q9;
  }
  else
  {
    goto erro;
  }
q9: // proc
  *token = generateToken("PROC", NULL, token);
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
    goto q14;
  }
  else
  {
    goto erro;
  }
q14: // void
  *token = generateToken("VOID", NULL, token);
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
    goto q20;
  }
  else
  {
    goto erro;
  }
q20: // while
  *token = generateToken("WHILE", NULL, token);
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
    goto q25;
  }
  else
  {
    goto erro;
  }
q25: // else
  *token = generateToken("ELSE", NULL, token);
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
    goto q31;
  }
  else
  {
    goto erro;
  }
q31: // false
  *token = generateToken("FALSE", NULL, token);
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
    goto q36;
  }
  else
  {
    goto erro;
  }
q36: // true
  *token = generateToken("TRUE", NULL, token);
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
    goto q39;
  }
  else
  {
    goto erro;
  }
q39: // do
  *token = generateToken("DO", NULL, token);
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
    goto q44;
  }
  else
  {
    goto erro;
  }
q44: // bool
  *token = generateToken("BOOL", NULL, token);
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
  printf("dentro de q46\n");
  if (digit == ' ')
  {
    printf("dentro de q46 if espaco\n");
    goto q47;
  }
  else
  {
    printf("dentro de erro\n");
    goto erro;
  }
q47: // if
  printf("dentro de q47\n");
  *token = generateToken("IF", NULL, token);
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
    goto q50;
  }
  else
  {
    goto erro;
  }
q50: // int
  *token = generateToken("INT", NULL, token);
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
    goto q56;
  }
  else
  {
    goto erro;
  }
q56: // semic
  *token = generateToken("SEMIC", NULL, token);
q57:
  if (digit == ' ')
  {
    goto q58;
  }
  else
  {
    goto erro;
  }
q58: // }
  *token = generateToken("FECHA_CHAVES", NULL, token);
q59:
  if (digit == ' ')
  {
    goto q60;
  }
  else
  {
    goto erro;
  }
q60: // {
  *token = generateToken("ABRE_CHAVES", NULL, token);
q61:
  if (digit == ' ')
  {
    goto q62;
  }
  else
  {
    goto erro;
  }
q62: // (
  *token = generateToken("ABRE_PARENTESIS", NULL, token);
q63:
  if (digit == ' ')
  {
    goto q64;
  }
  else
  {
    goto erro;
  }
q64: // )
  *token = generateToken("FECHA_PARENTESIS", NULL, token);
q65:
  if (digit == ' ')
  {
    goto q66;
  }
  else
  {
    goto erro;
  }
q66: // ,
  *token = generateToken("VIRGULA", NULL, token);
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
  *token = generateToken("PONTO_E_VIRGULA", NULL, token);
q69:
  if (digit == ' ')
  {
    goto q70;
  }
  else
  {
    goto erro;
  }
q70: // +
  *token = generateToken("OPR_ADICAO", NULL, token);
q71:
  if (digit == ' ')
  {
    goto q72;
  }
  else
  {
    goto erro;
  }
q72: // -
  *token = generateToken("OPR_SUBTRACAO", NULL, token);
q73:
  if (digit == ' ')
  {
    goto q74;
  }
  else
  {
    goto erro;
  }
q74: // *
  *token = generateToken("OPR_MULTIPLICACAO", NULL, token);
q75:
  if (digit == ' ')
  {
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
  *token = generateToken("OPR_DIVISAO", NULL, token);
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
      digit == ' ')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q94;
  }
q79:
  if (digit == ' ')
  {
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
  *token = generateToken("MENOR_QUE", NULL, token);
q81:
  if (digit == ' ')
  {
    goto q82;
  }
  else
  {
    goto erro;
  }
q82: // <>
  *token = generateToken("DIFERENCA", NULL, token);
q83:
  if (digit == ' ')
  {
    goto q84;
  }
  else
  {
    goto erro;
  }
q84: // <=
  *token = generateToken("MENOR_OU_IGUAL_QUE", NULL, token);
q85:
  if (digit == '=')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q87;
  }
  if (digit == ' ')
  {
    goto q86;
  }
  else
  {
    goto erro;
  }
q86: // >
  *token = generateToken("MAIOR_QUE", NULL, token);
q87:
  if (digit == ' ')
  {
    goto q88;
  }
  else
  {
    goto erro;
  }
q88: // >=
  *token = generateToken("MAIOR_OU_IGUAL_QUE", NULL, token);
q89:
  if (digit == '=')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q91;
  }
  if (digit == ' ')
  {
    goto q90;
  }
  else
  {
    goto erro;
  }
q90: // =
  *token = generateToken("ATRIBUICAO", NULL, token);
q91:
  if (digit == ' ')
  {
    goto q92;
  }
  else
  {
    goto erro;
  }
q92: // ==
  *token = generateToken("IGUALDADE", NULL, token);
q93:
  if (digit == ' ')
  {
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
  if (digit == '*' || digit == ' ')
  {
    (*counter)++;
    digit = lexema[*counter];
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
  if (digit == '*' || digit == ' ')
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
      digit == ' ')
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
    goto q101;
  }
  else
  {
    goto erro;
  }
q98: // Identificador
  *token = generateToken("IDENTIFICADOR ", lexema, token);
q99:
  if (isdigit(digit))
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q99;
  }
  if (digit == ' ')
  {
    goto q100;
  }
  else
  {
    goto erro;
  }
q100: // Digitos
  *token = generateToken("NUM ", lexema, token);
q101: // Comentario
  *token = generateToken("COMENTARIO ", lexema, token);
erro: // Erro léxico
  *token = generateToken("ERRO_LEXICO ", lexema, token);
}