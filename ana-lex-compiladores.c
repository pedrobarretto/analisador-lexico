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

Token generateToken(const char *str, const char *lex);

Token generateToken(const char *str, const char *lexema)
{
  Token token;
  int str_len = strlen(str);
  token.string = malloc(str_len + 1);
  strcpy(token.string, str);
  token.lexema = lexema ? strdup(lexema) : NULL; // se lexema for nulo, usa NULL como padrão
  return token;
}

Token scanner(char lexema[20], int counter);

int main()
{
  // Entrada de teste
  char palavra[100] = "if 2345 true ( /* ** _x int */ _x ) >= ";
  char *delimitador = " ";
  char *lexema;
  Token token;
  int counter = 0;

  // Inicializa a função strtok com a string a ser dividida e o delimitador
  lexema = strtok(palavra, delimitador);

  // Loop que itera sobre as substrings geradas pelo strtok
  while (lexema != NULL)
  {
    // Adiciona um espaço em branco após o lexema atual
    char lexema_com_espaco[strlen(lexema) + 2];
    sprintf(lexema_com_espaco, "%s ", lexema);

    // Chama a função scanner com o lexema atual
    token = scanner(lexema_com_espaco, counter);

    // Imprime o lexema e a string correspondente na estrutura Token
    if (token.lexema)
    {
      printf("LEXEMA: %s, TOKEN: %s\n", token.lexema, token.string);
    }
    else
    {
      printf("TOKEN: %s\n", token.string);
    }

    // Atualiza o ponteiro para a próxima substring
    lexema = strtok(NULL, delimitador);
  }

  return 0;
}

Token scanner(char lexema[20], int counter)
{
  char digit = lexema[0];

q0:
  if (isdigit(digit))
  {
    counter++;
    digit = lexema[counter];
    goto q99;
  }
  if (digit == '*')
  {
    counter++;
    digit = lexema[counter];
    goto q73;
  }
  if (digit == '+')
  {
    counter++;
    digit = lexema[counter];
    goto q69;
  }
  if (digit == '-')
  {
    counter++;
    digit = lexema[counter];
    goto q71;
  }
  if (digit == '/')
  {
    counter++;
    digit = lexema[counter];
    goto q75;
  }
  if (digit == ';')
  {
    counter++;
    digit = lexema[counter];
    goto q67;
  }
  if (digit == ',')
  {
    counter++;
    digit = lexema[counter];
    goto q65;
  }
  if (digit == '(')
  {
    counter++;
    digit = lexema[counter];
    goto q61;
  }
  if (digit == ')')
  {
    counter++;
    digit = lexema[counter];
    goto q63;
  }
  if (digit == '{')
  {
    counter++;
    digit = lexema[counter];
    goto q59;
  }
  if (digit == '}')
  {
    counter++;
    digit = lexema[counter];
    goto q57;
  }
  if (digit == '_')
  {
    counter++;
    digit = lexema[counter];
    goto q93;
  }
  if (digit == '=')
  {
    counter++;
    digit = lexema[counter];
    goto q89;
  }
  if (digit == '<')
  {
    counter++;
    digit = lexema[counter];
    goto q79;
  }
  if (digit == '>')
  {
    counter++;
    digit = lexema[counter];
    goto q85;
  }
  if (digit == 's')
  {
    counter++;
    digit = lexema[counter];
    goto q51;
  }
  if (digit == 'i')
  {
    counter++;
    digit = lexema[counter];
    goto q45;
  }
  if (digit == 'b')
  {
    counter++;
    digit = lexema[counter];
    goto q40;
  }
  if (digit == 'd')
  {
    counter++;
    digit = lexema[counter];
    goto q37;
  }
  if (digit == 't')
  {
    counter++;
    digit = lexema[counter];
    goto q32;
  }
  if (digit == 'f')
  {
    counter++;
    digit = lexema[counter];
    goto q26;
  }
  if (digit == 'e')
  {
    counter++;
    digit = lexema[counter];
    goto q21;
  }
  if (digit == 'w')
  {
    counter++;
    digit = lexema[counter];
    goto q15;
  }
  if (digit == 'v')
  {
    counter++;
    digit = lexema[counter];
    goto q10;
  }
  if (digit == 'p')
  {
    counter++;
    digit = lexema[counter];
    goto q1;
  }
  else
  {
    goto erro;
  }
q1:
  if (digit == 'r')
  {
    counter++;
    digit = lexema[counter];
    goto q2;
  }
q2:
  if (digit == 'i')
  {
    counter++;
    digit = lexema[counter];
    goto q3;
  }
  if (digit == 'o')
  {
    counter++;
    digit = lexema[counter];
    goto q7;
  }
q3:
  if (digit == 'n')
  {
    counter++;
    digit = lexema[counter];
    goto q4;
  }
q4:
  if (digit == 't')
  {
    counter++;
    digit = lexema[counter];
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
  return generateToken("PRINT", NULL);
q7:
  if (digit == 'c')
  {
    counter++;
    digit = lexema[counter];
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
  return generateToken("PROC", NULL);
q10:
  if (digit == 'o')
  {
    counter++;
    digit = lexema[counter];
    goto q11;
  }
q11:
  if (digit == 'i')
  {
    counter++;
    digit = lexema[counter];
    goto q12;
  }
q12:
  if (digit == 'd')
  {
    counter++;
    digit = lexema[counter];
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
  return generateToken("VOID", NULL);
q15:
  if (digit == 'h')
  {
    counter++;
    digit = lexema[counter];
    goto q16;
  }
q16:
  if (digit == 'i')
  {
    counter++;
    digit = lexema[counter];
    goto q17;
  }
q17:
  if (digit == 'l')
  {
    counter++;
    digit = lexema[counter];
    goto q18;
  }
q18:
  if (digit == 'e')
  {
    counter++;
    digit = lexema[counter];
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
  return generateToken("WHILE", NULL);
q21:
  if (digit == 'l')
  {
    counter++;
    digit = lexema[counter];
    goto q22;
  }
q22:
  if (digit == 's')
  {
    counter++;
    digit = lexema[counter];
    goto q23;
  }
q23:
  if (digit == 'e')
  {
    counter++;
    digit = lexema[counter];
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
  return generateToken("ELSE", NULL);
q26:
  if (digit == 'a')
  {
    counter++;
    digit = lexema[counter];
    goto q27;
  }
q27:
  if (digit == 'l')
  {
    counter++;
    digit = lexema[counter];
    goto q28;
  }
q28:
  if (digit == 's')
  {
    counter++;
    digit = lexema[counter];
    goto q29;
  }
q29:
  if (digit == 'e')
  {
    counter++;
    digit = lexema[counter];
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
  return generateToken("FALSE", NULL);
q32:
  if (digit == 'r')
  {
    counter++;
    digit = lexema[counter];
    goto q33;
  }
q33:
  if (digit == 'u')
  {
    counter++;
    digit = lexema[counter];
    goto q34;
  }
q34:
  if (digit == 'e')
  {
    counter++;
    digit = lexema[counter];
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
  return generateToken("TRUE", NULL);
q37:
  if (digit == 'o')
  {
    counter++;
    digit = lexema[counter];
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
  return generateToken("DO", NULL);
q40:
  if (digit == 'o')
  {
    counter++;
    digit = lexema[counter];
    goto q41;
  }
q41:
  if (digit == 'o')
  {
    counter++;
    digit = lexema[counter];
    goto q42;
  }
q42:
  if (digit == 'l')
  {
    counter++;
    digit = lexema[counter];
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
  return generateToken("BOOL", NULL);
q45:
  if (digit == 'n')
  {
    counter++;
    digit = lexema[counter];
    goto q48;
  }
  if (digit == 'f')
  {
    counter++;
    digit = lexema[counter];
    goto q46;
  }
q46:
  if (digit == ' ')
  {
    goto q47;
  }
  else
  {
    goto erro;
  }
q47: // if
  return generateToken("IF", NULL);
q48:
  if (digit == 't')
  {
    counter++;
    digit = lexema[counter];
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
  return generateToken("INT", NULL);
q51:
  if (digit == 'e')
  {
    counter++;
    digit = lexema[counter];
    goto q52;
  }
q52:
  if (digit == 'm')
  {
    counter++;
    digit = lexema[counter];
    goto q53;
  }
q53:
  if (digit == 'i')
  {
    counter++;
    digit = lexema[counter];
    goto q54;
  }
q54:
  if (digit == 'c')
  {
    counter++;
    digit = lexema[counter];
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
  return generateToken("SEMIC", NULL);
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
  return generateToken("FECHA_CHAVES", NULL);
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
  return generateToken("ABRE_CHAVES", NULL);
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
  return generateToken("ABRE_PARENTESIS", NULL);
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
  return generateToken("FECHA_PARENTESIS", NULL);
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
  return generateToken("VIRGULA", NULL);
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
  return generateToken("PONTO_E_VIRGULA", NULL);
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
  return generateToken("OPR_ADICAO", NULL);
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
  return generateToken("OPR_SUBTRACAO", NULL);
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
  return generateToken("OPR_MULTIPLICACAO", NULL);
q75:
  if (digit == ' ')
  {
    goto q76;
  }
  if (digit == '*')
  {
    counter++;
    digit = lexema[counter];
    goto q77;
  }
  else
  {
    goto erro;
  }
q76: // /
  return generateToken("OPR_DIVISAO", NULL);
q77:
  if (digit == ' ')
  {
    counter++;
    digit = lexema[counter];
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
    counter++;
    digit = lexema[counter];
    goto q94;
  }
q79:
  if (digit == ' ')
  {
    goto q80;
  }
  if (digit == '=')
  {
    counter++;
    digit = lexema[counter];
    goto q83;
  }
  if (digit == '>')
  {
    counter++;
    digit = lexema[counter];
    goto q81;
  }
  else
  {
    goto erro;
  }
q80: // <
  return generateToken("MENOR_QUE", NULL);
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
  return generateToken("DIFERENCA", NULL);
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
  return generateToken("MENOR_OU_IGUAL_QUE", NULL);
q85:
  if (digit == '=')
  {
    counter++;
    digit = lexema[counter];
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
  return generateToken("MAIOR_QUE", NULL);
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
  return generateToken("MAIOR_OU_IGUAL_QUE", NULL);
q89:
  if (digit == '=')
  {
    counter++;
    digit = lexema[counter];
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
  return generateToken("ATRIBUICAO", NULL);
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
  return generateToken("IGUALDADE", NULL);
q93:
  if (digit == ' ')
  {
    goto q98;
  }
  if (isalpha(digit))
  {
    counter++;
    digit = lexema[counter];
    goto q93;
  }
  else
  {
    goto erro;
  }
q94:
  if (digit == '*' || digit == ' ')
  {
    counter++;
    digit = lexema[counter];
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
    counter++;
    digit = lexema[counter];
    goto q78;
  }
q95:
  if (digit == '*' || digit == ' ')
  {
    counter++;
    digit = lexema[counter];
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
    counter++;
    digit = lexema[counter];
    goto q94;
  }
  else
  {
    goto erro;
  }
q96:
  if (digit == '*')
  {
    counter++;
    digit = lexema[counter];
    goto q96;
  }
  if (digit == '/')
  {
    counter++;
    digit = lexema[counter];
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
    counter++;
    digit = lexema[counter];
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
  return generateToken("IDENTIFICADOR ", lexema);
q99:
  if (isdigit(digit))
  {
    counter++;
    digit = lexema[counter];
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
  return generateToken("NUM ", lexema);
q101: // Comentario
  return generateToken("COMENTARIO ", lexema);
erro: // Erro léxico
  return generateToken("ERRO_LEXICO ", lexema);
}