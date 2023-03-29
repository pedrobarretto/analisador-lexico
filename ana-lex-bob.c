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
  OPR_LESS_THAN,
  OPR_LESS_OR_EQUAL_THAN,
  OPR_HIGHER_THAN,
  OPR_HIGHER_OR_EQUAL_THAN,
  OPR_EQUAL_TO,
  OPR_DIFERENT,
  OPR_EQUAL,
  OPR_VIRGULA,
  OPR_PONTO_E_VIRGULA,
  OPR_COMENT,
  OPR_ABRE_PARENTESIS,
  OPR_FECHA_PARENTESIS,
  OPR_ABRE_CHAVES,
  OPR_FECHA_CHAVES,
  VALOR_NUMERICO
} TOKEN_TYPE;

char *tokenToStr[] = {
  "Erro",
  "Operador VOID",
  "Operador SEMIC",
  "Operador IF",
  "Operador ELSE",
  "Operador PRINT",
  "Operador TRUE",
  "Operador FALSE",
  "Operador DO",
  "Operador WHILE",
  "Operador PROC",
  "Tipo INTEIRO",
  "Tipo BOOLEANO",
  "IDENTIFICADOR",
  "Operador ADD",
  "Operador SUB",
  "Operador MULTIPLICACAO",
  "Operador DIVISAO",
  "Operador MENOR QUE",
  "Operador MENOR OU IGUAL QUE",
  "Operador MAIOR QUE",
  "Operador MAIOR OU IGUAL QUE",
  "Operador IGUALDADE",
  "Operador DIFERENCA",
  "Operador ATRIBUICAO",
  "Delimitador VIRGULA",
  "Delimitador PONTO E VIRGULA",
  "Operador COMENTARIO",
  "Operador ABRE PARENTESIS",
  "Operador FECHA PARENTESIS",
  "Operador ABRE CHAVES",
  "Operador FECHA CHAVES",
  "Valor numerico"
};

// Ao final de tudo, um espaco vazio, menos em comentario e delimitador

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
    printf("%s\n", tokenToStr[tk]);
    buffer++;
	} while (tk != ERRO);

	return 0;
}

int scanner(char *palavra, TOKEN_TYPE tipo) {
  char c;
  // printf("%s\n", palavra);

  // Desconsiderar identações do começo das linhas
  if ((*buffer == ' ') || (*buffer == '\t') || (*buffer == '\r') || (*buffer == '\n')) {
    linha++;
		buffer++;
  }

  // if (isdigit(*buffer)) {
  //   linha++;
  //   buffer++;
  //   tipo = VALOR_NUMERICO;
  // }

  while (isdigit(*buffer)) {
    linha++;
    buffer++;
    tipo = VALOR_NUMERICO;
  }

  LOOP:do {
    q0:
      if (*buffer == '*') {
        linha++;
        buffer++;
        tipo = OPR_TIMES;
        return tipo;
      }
      if (*buffer == '+') {
        linha++;
        buffer++;
        tipo = OPR_ADD;
        return tipo;
      }
      if (*buffer == '-') {
        linha++;
        buffer++;
        tipo = OPR_SUB;
        return tipo;
      }
      if (*buffer == '/') {
        // linha++;
        // buffer++;
        goto q5;
      }
      if (*buffer == ';') {
        linha++;
        buffer++;
        tipo = OPR_PONTO_E_VIRGULA;
        return tipo;
      }
      if (*buffer == ',') {
      linha++;
      buffer++;
      tipo = OPR_VIRGULA;
      goto q65;
      }
      if (*buffer == '(') {
        linha++;
        buffer++;
        tipo = OPR_ABRE_PARENTESIS;
        goto q65;
      }
      if (*buffer == ')') {
        linha++;
        buffer++;
        tipo = OPR_FECHA_PARENTESIS;
        return tipo;
      }
      if (*buffer == '{') {
        linha++;
        buffer++;
        // buffer++; // FIXME: Só colocando isso por conta da identação que tem na outra linha
        tipo = OPR_ABRE_CHAVES;
        return tipo;
      }
      if (*buffer == '}') {
        linha++;
        buffer++;
        tipo = OPR_FECHA_CHAVES;
        return tipo;
      }
      if (*buffer == '_') {
        linha++;
        buffer++;
        goto q2;
      }
      if (*buffer == '=') {
        linha++;
        buffer++;
        goto q9;
      }
      if (*buffer == '<') {
        linha++;
        buffer++;
        goto q7;
      }
      if (*buffer == '>') {
        linha++;
        buffer++;
        goto q8;
      }
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
      if (*buffer == 'f') {
        linha++;
        buffer++;
        goto q38;
      }
      if (*buffer == 'e') {
        linha++;
        buffer++;
        goto q39;
      }
      if (*buffer == 'w') {
        linha++;
        buffer++;
        goto q40;
      }
      if (*buffer == 'v') {
        linha++;
        buffer++;
        goto q41;
      }
      if (*buffer == 'p') {
        linha++;
        buffer++;
        goto q42;
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
      if (*buffer == 'f') {
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
      if (*buffer == 'c') {
        linha++;
        buffer++;
        tipo = OPR_SEMIC;
        goto q65;
      }
    q30: // int
      if (*buffer == 't') {
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
      if (*buffer == 'l') {
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
    q38:
      if (*buffer == 'a') {
        linha++;
        buffer++;
        goto q46;
      }
    q39:
      if (*buffer == 'l') {
        linha++;
        buffer++;
        goto q50;
      }
    q40:
      if (*buffer == 'h') {
        linha++;
        buffer++;
        goto q53;
      }
    q41:
      if (*buffer == 'o') {
        linha++;
        buffer++;
        goto q57;
      }
    q42:
      if (*buffer == 'r') {
        linha++;
        buffer++;
        goto q60;
      }
    q43:
      if (*buffer == 'u') {
        linha++;
        buffer++;
        goto q44;
      }
    q44: // true
      if (*buffer == 'e') {
        linha++;
        buffer++;
        tipo = OPR_TRUE;
        goto q65;
      }
    q46:
      if (*buffer == 'l') {
        linha++;
        buffer++;
        goto q47;
      }
    q47:
      if (*buffer == 's') {
        linha++;
        buffer++;
        goto q48;
      }
    q48: // false
      if (*buffer == 'e') {
        linha++;
        buffer++;
        tipo = OPR_FALSE;
        goto q65;
      }
    q50:
      if (*buffer == 's') {
        linha++;
        buffer++;
        goto q51;
      }
    q51:
      if (*buffer == 'e') {
        linha++;
        buffer++;
        tipo = OPR_ELSE;
        goto q65;
      }
    q53:
      if (*buffer == 'i') {
        linha++;
        buffer++;
        goto q54;
      }
    q54:
      if (*buffer == 'l') {
        linha++;
        buffer++;
        goto q55;
      }
    q55: // while
      if (*buffer == 'e') {
        linha++;
        buffer++;
        tipo = OPR_WHILE;
        goto q65;
      }
    q57:
      if (*buffer == 'i') {
        linha++;
        buffer++;
        goto q58;
      }
    q58: // void
      if (*buffer == 'd') {
        linha++;
        buffer++;
        tipo = OPR_VOID;
        goto q65;
      }
    q60:
      if (*buffer == 'i') {
        linha++;
        buffer++;
        goto q61;
      }
      if (*buffer == 'o') {
        linha++;
        buffer++;
        goto q64;
      }
    q61:
      if (*buffer == 'n') {
        linha++;
        buffer++;
        goto q62;
      }
    q62:
      if (*buffer == 't') {
        linha++;
        buffer++;
        tipo = OPR_PRINT;
        goto q65;
      }
    q64:
      if (*buffer == 'o') {
        linha++;
        buffer++;
        goto q67;
      }
    q65: // final
      if (*buffer == ' ') {
        linha++;
        // buffer++; // TODO: Precisa desse buffer++?
        return tipo;
      } else {
        goto erro;
      }
    q67:
      if (*buffer == 'c') {
        linha++;
        buffer++;
        tipo = OPR_PROC;
        goto q65;
      }
    q9:
      if (*buffer == '=') { // Igualdade
        linha++;
        buffer++;
        tipo = OPR_EQUAL_TO;
        return tipo;
      } else {
        linha++;
        buffer++;
        tipo = OPR_EQUAL;
        return tipo;
      }
    q7:
      if (*buffer == '=') { // Menor ou igual
        linha++;
        buffer;
        tipo = OPR_LESS_OR_EQUAL_THAN;
        goto q65;
      }
      else if (*buffer == '>') { // DIferença
        linha++;
        buffer;
        tipo = OPR_DIFERENT;
        goto q65;
      }
      else { // Menor que
        linha++;
        buffer;
        tipo = OPR_LESS_THAN;
        goto q65;
      }
    q8:
      if (*buffer == '=') { // Maior ou igual
        linha++;
        buffer;
        tipo = OPR_HIGHER_OR_EQUAL_THAN;
        goto q65;
      }
      else { // Maior que
        linha++;
        buffer;
        tipo = OPR_HIGHER_THAN;
        goto q65;
      }
    q5:
      if (*buffer == '/' && *buffer + 1 != '*') {
        printf("%s\n", "dentro de q5 divisao");
        linha++;
        buffer++;
        tipo = OPR_DIV;
        goto q65;
      }

      // Em q0, não fazemos essa add, pra poder cair no primeiro if
      linha++;
      buffer++;

      if (*buffer == '*') {
        printf("%s\n", "dentro de q5 *");
        if (*buffer == '*') {
          printf("%s\n", "dentro de q5 segundo *");
          if (*buffer == '/') {
            printf("%s\n", "dentro de q5 if /");
            linha++;
            buffer++;
            tipo = OPR_COMENT;
            return tipo;
          }
        }
        printf("%s\n", "dentro de q5 palavra ou num");
        linha++;
        buffer++;
        goto q5;
        // else if (isalpha(*buffer) || isalnum(*buffer)) {
        //   printf("%s\n", "dentro de q5 palavra ou num");
        //   linha++;
        //   buffer++;
        //   goto q5;
        // }
        // else {
        //   goto erro;
        // }
      }
    q2: // identificador
      if (isalpha(*buffer) || isdigit(*buffer)) { // FIXME: Assim, uma var _123 pode ser aceita
        linha++;
        buffer++;
        tipo = IDENTIFICADOR;
        goto q2;
      }
      goto q65;
    erro: // erro
      tipo = ERRO;
      return tipo;

  } while (tipo != ERRO);
  
}