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
  VALOR_NUMERICO,
  FIM_DE_ARQUIVO
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
  "Valor numerico",
  "Final do arquivo"
};

// Ao final de tudo, um espaco vazio, menos em comentario e delimitador

int scanner(char lexema[20]);

TOKEN_TYPE tipo;

int main()
{
  char palavra[50] = "if 2345 true ( /* ** _x int */ _x ) >= ";
  char lexema[20];

  char *token; // ponteiro para o token atual
  char delimitador[] = " "; // o espaço em branco é o delimitador
  
  // extrai o primeiro token
  token = strtok(palavra, delimitador);

  // continua enquanto houver mais tokens
  while (token != NULL) {
      strcpy(lexema, token); // copia o token atual para a variável lexema
      printf("%s\n", lexema); // imprime o token atual
      token = strtok(NULL, delimitador); // extrai o próximo token
      scanner(lexema);
  }

	// int tk;
	// do
	// {
	// 	tk = scanner(lexema);
	// 	// printf("\nLinha:%3d | %-30s", linha, tk);
  //   printf("%s\n", tokenToStr[tk]);
  //   bufafer++;
	// } while (tk != ERRO && tk != FIM_DE_ARQUIVO);

	return 0;
}

// 1. Dentro da func scanner não tem loop de repetição OK
// 2. A entrada da main será uma string constante no código OK
// 3. Sem variáveis globais
// 4. Um estado final para cada lexema, retornar o tipo e a palavra/simbolo/num
// 5. O erro não precisa parar o analisador léxico
// 6. Um espaço final depois de todo lexema
// 7. Mudar de Operador XXX para <Palavra reservada, XXX>

// 2 -> char* + counter (msm tamanho do char)
// Na main iteramos sobre a frase até encontrar um espaço,
// mandamos a palabra encontrada para a scanner, o scanner faz seu trabalho.
// void _func ( _a )

int scanner(char lexema[20]) {
  char digit = lexema[0];
  int counter = 0;
  // printf("%s\n", palavra);

  // Desconsiderar identações do começo das linhas
  // if ((digit == ' ') || (digit == '\t') || (digit == '\r') || (digit == '\n')) {
	// 	counter++;
  // }


  q0:
    // if ((digit == EOF) || (digit == '\x0')) {
    //   tipo = FIM_DE_ARQUIVO;
    //   return tipo;
    // }
    if (isdigit(digit)) {
      counter++;
      goto q71;
    }
    if (digit == '*') {
      counter++;
      tipo = OPR_TIMES;
      return tipo;
    }
    if (digit == '+') {
      counter++;
      tipo = OPR_ADD;
      return tipo;
    }
    if (digit == '-') {
      counter++;
      tipo = OPR_SUB;
      return tipo;
    }
    if (digit == '/') {
      // b_uffer++;
      goto q5;
    }
    if (digit == ';') {
      counter++;
      tipo = OPR_PONTO_E_VIRGULA;
      return tipo;
    }
    if (digit == ',') {
    counter++;
    tipo = OPR_VIRGULA;
    goto q65;
    }
    if (digit == '(') {
      counter++;
      tipo = OPR_ABRE_PARENTESIS;
      goto q65;
    }
    if (digit == ')') {
      counter++;
      tipo = OPR_FECHA_PARENTESIS;
      return tipo;
    }
    if (digit == '{') {
      counter++;
      // buffser++; // FIXME: Só colocando isso por conta da identação que tem na outra linha
      tipo = OPR_ABRE_CHAVES;
      return tipo;
    }
    if (digit == '}') {
      counter++;
      tipo = OPR_FECHA_CHAVES;
      return tipo;
    }
    if (digit == '_') {
      counter++;
      goto q2;
    }
    if (digit == '=') {
      counter++;
      goto q9;
    }
    if (digit == '<') {
      counter++;
      goto q7;
    }
    if (digit == '>') {
      counter++;
      goto q8;
    }
    if (digit == 's') {
      counter++;
      goto q16;
    }
    if (digit == 'i') {
      counter++;
      goto q17;
    }
    if (digit == 'b') {
      counter++;
      goto q18;
    }
    if (digit == 'd') {
      counter++;
      goto q19;
    }
    if (digit == 't') {
      counter++;
      goto q37;
    }
    if (digit == 'f') {
      counter++;
      goto q38;
    }
    if (digit == 'e') {
      counter++;
      goto q39;
    }
    if (digit == 'w') {
      counter++;
      goto q40;
    }
    if (digit == 'v') {
      counter++;
      goto q41;
    }
    if (digit == 'p') {
      counter++;
      goto q42;
    }
  q16:
    if (digit == 'e') {
      counter++;
      goto q26;
    }
  q17: // if
    if (digit == 'n') {
      counter++;
      goto q30;
    }
    if (digit == 'f') {
      counter++;
      tipo = OPR_IF;
      goto q65;
    }
  q18:
    if (digit == 'o') {
      counter++;
      goto q33;
    }
  q19: // do
    if (digit == 'o') {
      counter++;
      tipo = OPR_DO;
      goto q65;
    }
  q26:
    if (digit == 'm') {
      counter++;
      goto q27;
    }
  q27:
    if (digit == 'i') {
      counter++;
      goto q28;
    }
  q28: // semic
    if (digit == 'c') {
      counter++;
      tipo = OPR_SEMIC;
      goto q65;
    }
  q30: // int
    if (digit == 't') {
      counter++;
      tipo = OPR_INT;
      goto q65;
    }
  q33:
    if (digit == 'o') {
      counter++;
      goto q34;
    }
  q34: // bool
    if (digit == 'l') {
      counter++;
      tipo = OPR_BOOL;
      goto q65;
    }
  q37:
    if (digit == 'r') {
      counter++;
      goto q43;
    }
  q38:
    if (digit == 'a') {
      counter++;
      goto q46;
    }
  q39:
    if (digit == 'l') {
      counter++;
      goto q50;
    }
  q40:
    if (digit == 'h') {
      counter++;
      goto q53;
    }
  q41:
    if (digit == 'o') {
      counter++;
      goto q57;
    }
  q42:
    if (digit == 'r') {
      counter++;
      goto q60;
    }
  q43:
    if (digit == 'u') {
      counter++;
      goto q44;
    }
  q44: // true
    if (digit == 'e') {
      counter++;
      tipo = OPR_TRUE;
      goto q65;
    }
  q46:
    if (digit == 'l') {
      counter++;
      goto q47;
    }
  q47:
    if (digit == 's') {
      counter++;
      goto q48;
    }
  q48: // false
    if (digit == 'e') {
      counter++;
      tipo = OPR_FALSE;
      goto q65;
    }
  q50:
    if (digit == 's') {
      counter++;
      goto q51;
    }
  q51:
    if (digit == 'e') {
      counter++;
      tipo = OPR_ELSE;
      goto q65;
    }
  q53:
    if (digit == 'i') {
      counter++;
      goto q54;
    }
  q54:
    if (digit == 'l') {
      counter++;
      goto q55;
    }
  q55: // while
    if (digit == 'e') {
      counter++;
      tipo = OPR_WHILE;
      goto q65;
    }
  q57:
    if (digit == 'i') {
      counter++;
      goto q58;
    }
  q58: // void
    if (digit == 'd') {
      counter++;
      tipo = OPR_VOID;
      goto q65;
    }
  q60:
    if (digit == 'i') {
      counter++;
      goto q61;
    }
    if (digit == 'o') {
      counter++;
      goto q64;
    }
  q61:
    if (digit == 'n') {
      counter++;
      goto q62;
    }
  q62:
    if (digit == 't') {
      counter++;
      tipo = OPR_PRINT;
      goto q65;
    }
  q64:
    if (digit == 'c') {
      counter++;
      tipo = OPR_PROC;
      goto q65;
    }
  q65: // final
    if (digit == ' ') {
      // busffer++; // TODO: Precisa desse buffer++?
      return tipo;
    } else {
      goto erro;
    }
  q9:
    if (digit == '=') { // Igualdade
      counter++;
      tipo = OPR_EQUAL_TO;
      return tipo;
    } else {
      // busffer++;
      tipo = OPR_EQUAL;
      goto q65;
    }
  q7:
    if (digit == '=') { // Menor ou igual
      counter++;
      tipo = OPR_LESS_OR_EQUAL_THAN;
      goto q65;
    }
    else if (digit == '>') { // DIferença
      counter++;
      tipo = OPR_DIFERENT;
      goto q65;
    }
    else { // Menor que
      counter++;
      tipo = OPR_LESS_THAN;
      goto q65;
    }
  q8:
    if (digit == '=') { // Maior ou igual
      counter++;
      tipo = OPR_HIGHER_OR_EQUAL_THAN;
      goto q65;
    }
    else { // Maior que
      counter++;
      tipo = OPR_HIGHER_THAN;
      goto q65;
    }
  q5:
    counter++;

    if (digit == '*') {
      counter++;
      goto q66;
    }

    tipo = OPR_DIV;
    
    goto q65;
  q66:
    if (digit == '*') {
      counter++;
      goto q69;
    }
    if (
        isalpha(digit) ||
        isdigit(digit) ||
        digit == ' ' ||
        (digit == '\t') ||
        (digit == '\r') ||
        (digit == '\n')
      ) {
      counter++;
      goto q66;
    }
  q69:
    if (digit == '/') {
      counter++;
      tipo = OPR_COMENT;
      return tipo;
    }
  q71:
    if (isdigit(digit)) {
      counter++;
      goto q71;
    }

    tipo = VALOR_NUMERICO;
    goto q65;
  q2: // identificador
    if (isalpha(digit) || isdigit(digit)) { // FIXME: Assim, uma var _123 pode ser aceita
      counter++;
      tipo = IDENTIFICADOR;
      goto q2;
    }
    goto q65;
  erro: // erro
    tipo = ERRO;
    return tipo;
}