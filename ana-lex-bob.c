#pragma warning(disable : 4996)

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct {
  char* string;
  char* lexema;
} Token;

Token generateToken(const char* str, const char* lex);

Token generateToken(const char* str, const char* lex) {
  Token token;
  int str_len = strlen(str);
  int lex_len = strlen(lex);
  token.string = malloc(str_len + 1);
  token.lexema = malloc(lex_len + 1);
  strcpy(token.string, str);
  strcpy(token.lexema, lex);
  return token;
}

Token scanner(char lexema[20]);

int main() {
  char palavra[50] = "if 2345 true ( /* ** _x int * */ _x ) >= ";
  char lexema[20];
  char *token;
  char delimitador[] = " ";
  int in_comment = 0; // variável para indicar se está dentro de um comentário ou não
  char comment[20] = ""; // variável temporária para armazenar comentários que não terminam em "*/"

  token = strtok(palavra, delimitador);
  
  while (token != NULL) {
    if (in_comment) {
      strcat(comment, " ");
      strcat(comment, token);
      if (strstr(comment, "*/") != NULL) {
        in_comment = 0;
        strcat(comment, " "); // adiciona espaço em branco após "*/"
        Token tk = scanner(comment);
        printf("%s, %s>\n", tk.string, tk.lexema);
        free(tk.string);
        free(tk.lexema);
        comment[0] = '\0';
      }
    } else if (strstr(token, "/*") != NULL) {
      in_comment = 1;
      lexema[0] = '\0';
      strcat(lexema, token);
      if (strstr(token, "*/") == NULL) {
        strcpy(comment, token); // armazena o comentário na variável temporária
      }
    } else {
      sprintf(lexema, "%s ", token);
      Token tk = scanner(lexema);
      if (strcmp(token, "*/") != 0) {
        printf("%s, %s>\n", tk.string, tk.lexema);
        free(tk.string);
        free(tk.lexema);
      }
    }
    token = strtok(NULL, delimitador);
  }

  return 0;
}

Token scanner(char lexema[20]) {
  char digit = lexema[0];
  int counter = 0;

  q0:
    if (isdigit(digit)) {
      counter++;
      digit = lexema[counter];
      goto q99;
    }
    if (digit == '*') {
      counter++;
      digit = lexema[counter];
      goto q73;
    }
    if (digit == '+') {
      counter++;
      digit = lexema[counter];
      goto q69;
    }
    if (digit == '-') {
      counter++;
      digit = lexema[counter];
      goto q71;
    }
    if (digit == '/') {
      counter++;
      digit = lexema[counter];
      goto q75;
    }
    if (digit == ';') {
      counter++;
      digit = lexema[counter];
      goto q67;
    }
    if (digit == ',') {
      counter++;
      digit = lexema[counter];
      goto q65;
    }
    if (digit == '(') {
      counter++;
      digit = lexema[counter];
      goto q61;
    }
    if (digit == ')') {
      counter++;
      digit = lexema[counter];
      goto q63;
    }
    if (digit == '{') {
      counter++;
      digit = lexema[counter];
      goto q59;
    }
    if (digit == '}') {
      counter++;
      digit = lexema[counter];
      goto q57;
    }
    if (digit == '_') {
      counter++;
      digit = lexema[counter];
      goto q93;
    }
    if (digit == '=') {
      counter++;
      digit = lexema[counter];
      goto q89;
    }
    if (digit == '<') {
      counter++;
      digit = lexema[counter];
      goto q79;
    }
    if (digit == '>') {
      counter++;
      digit = lexema[counter];
      goto q85;
    }
    if (digit == 's') {
      counter++;
      digit = lexema[counter];
      goto q51;
    }
    if (digit == 'i') {
      counter++;
      digit = lexema[counter];
      goto q45;
    }
    if (digit == 'b') {
      counter++;
      digit = lexema[counter];
      goto q40;
    }
    if (digit == 'd') {
      counter++;
      digit = lexema[counter];
      goto q37;
    }
    if (digit == 't') {
      counter++;
      digit = lexema[counter];
      goto q32;
    }
    if (digit == 'f') {
      counter++;
      digit = lexema[counter];
      goto q26;
    }
    if (digit == 'e') {
      counter++;
      digit = lexema[counter];
      goto q21;
    }
    if (digit == 'w') {
      counter++;
      digit = lexema[counter];
      goto q15;
    }
    if (digit == 'v') {
      counter++;
      digit = lexema[counter];
      goto q10;
    }
    if (digit == 'p') {
      counter++;
      digit = lexema[counter];
      goto q1;
    }
    else {
      goto erro;
    }
  q1:
    if (digit == 'r') {
      counter++;
      digit = lexema[counter];
      goto q2;
    }
  q2:
    if (digit == 'i') {
      counter++;
      digit = lexema[counter];
      goto q3;
    }
    if (digit == 'o') {
      counter++;
      digit = lexema[counter];
      goto q7;
    }
  q3:
    if (digit == 'n') {
      counter++;
      digit = lexema[counter];
      goto q4;
    }
  q4:
    if (digit == 't') {
      counter++;
      digit = lexema[counter];
      goto q5;
    }
  q5:
    if (digit == ' ') {
      goto q6;
    } else {
      goto erro;
    }
  q6: // print
    return generateToken("<Palavra reservada ", lexema);
  q7:
    if (digit == 'c') {
      counter++;
      digit = lexema[counter];
      goto q8;
    }
  q8:
    if (digit == ' ') {
      goto q9;
    } else {
      goto erro;
    }
  q9: // proc
    return generateToken("<Palavra reservada ", lexema);
  q10:
    if (digit == 'o') {
      counter++;
      digit = lexema[counter];
      goto q11;
    }
  q11:
    if (digit == 'i') {
      counter++;
      digit = lexema[counter];
      goto q12;
    }
  q12:
    if (digit == 'd') {
      counter++;
      digit = lexema[counter];
      goto q13;
    }
  q13:
    if (digit == ' ') {
      goto q14;
    } else {
      goto erro;
    }
  q14: // void
    return generateToken("<Palavra reservada ", lexema);
  q15:
    if (digit == 'h') {
      counter++;
      digit = lexema[counter];
      goto q16;
    }
  q16:
    if (digit == 'i') {
      counter++;
      digit = lexema[counter];
      goto q17;
    }
  q17:
    if (digit == 'l') {
      counter++;
      digit = lexema[counter];
      goto q18;
    }
  q18:
    if (digit == 'e') {
      counter++;
      digit = lexema[counter];
      goto q19;
    }
  q19:
    if (digit == ' ') {
      goto q20;
    } else {
      goto erro;
    }
  q20: // while
    return generateToken("<Palavra reservada ", lexema);
  q21:
    if (digit == 'l') {
      counter++;
      digit = lexema[counter];
      goto q22;
    }
  q22:
    if (digit == 's') {
      counter++;
      digit = lexema[counter];
      goto q23;
    }
  q23:
    if (digit == 'e') {
      counter++;
      digit = lexema[counter];
      goto q24;
    }
  q24:
    if (digit == ' ') {
      goto q25;
    } else {
      goto erro;
    }
  q25: // else
    return generateToken("<Palavra reservada ", lexema);
  q26:
    if (digit == 'a') {
      counter++;
      digit = lexema[counter];
      goto q27;
    }
  q27:
    if (digit == 'l') {
      counter++;
      digit = lexema[counter];
      goto q28;
    }
  q28:
    if (digit == 's') {
      counter++;
      digit = lexema[counter];
      goto q29;
    }
  q29:
    if (digit == 'e') {
      counter++;
      digit = lexema[counter];
      goto q30;
    }
  q30:
    if (digit == ' ') {
      goto q31;
    } else {
      goto erro;
    }
  q31: // false
    return generateToken("<Valor booleano ", lexema);
  q32:
    if (digit == 'r') {
      counter++;
      digit = lexema[counter];
      goto q33;
    }
  q33:
    if (digit == 'u') {
      counter++;
      digit = lexema[counter];
      goto q34;
    }
  q34:
    if (digit == 'e') {
      counter++;
      digit = lexema[counter];
      goto q35;
    }
  q35:
    if (digit == ' ') {
      goto q36;
    } else {
      goto erro;
    }
  q36: // true
    return generateToken("<Valor booleano ", lexema);
  q37:
    if (digit == 'o') {
      counter++;
      digit = lexema[counter];
      goto q38;
    }
  q38:
    if (digit == ' ') {
      goto q39;
    } else {
      goto erro;
    }
  q39: // do
    return generateToken("<Palavra reservada ", lexema);
  q40:
    if (digit == 'o') {
      counter++;
      digit = lexema[counter];
      goto q41;
    }
  q41:
    if (digit == 'o') {
      counter++;
      digit = lexema[counter];
      goto q42;
    }
  q42:
    if (digit == 'l') {
      counter++;
      digit = lexema[counter];
      goto q43;
    }
  q43:
    if (digit == ' ') {
      goto q44;
    } else {
      goto erro;
    }
  q44: // bool
    return generateToken("<Palavra reservada ", lexema);
  q45:
    if (digit == 'n') {
      counter++;
      digit = lexema[counter];
      goto q48;
    }
    if (digit == 'f') {
      counter++;
      digit = lexema[counter];
      goto q46;
    }
  q46:
    if (digit == ' ') {
      goto q47;
    } else {
      goto erro;
    }
  q47: // if
    return generateToken("<Palavra reservada ", lexema);
  q48:
    if (digit == 't') {
      counter++;
      digit = lexema[counter];
      goto q49;
    }
  q49:
    if (digit == ' ') {
      goto q50;
    } else {
      goto erro;
    }
  q50: // int
    return generateToken("<Palavra reservada ", lexema);
  q51:
    if (digit == 'e') {
      counter++;
      digit = lexema[counter];
      goto q52;
    }
  q52:
    if (digit == 'm') {
      counter++;
      digit = lexema[counter];
      goto q53;
    }
  q53:
    if (digit == 'i') {
      counter++;
      digit = lexema[counter];
      goto q54;
    }
  q54:
    if (digit == 'c') {
      counter++;
      digit = lexema[counter];
      goto q55;
    }
  q55:
    if (digit == ' ') {
      goto q56;
    } else {
      goto erro;
    }
  q56: // semic
    return generateToken("<Palavra reservada ", lexema);
  q57:
    if (digit == ' ') {
      goto q58;
    } else {
      goto erro;
    }
  q58: // }
    return generateToken("<Delimitador fecha chaves ", lexema);
  q59:
    if (digit == ' ') {
      goto q60;
    } else {
      goto erro;
    }
  q60: // {
    return generateToken("<Delimitador abre chaves ", lexema);
  q61:
    if (digit == ' ') {
      goto q62;
    } else {
      goto erro;
    }
  q62: // (
    return generateToken("<Delimitador abre parentesis ", lexema);
  q63:
    if (digit == ' ') {
      goto q64;
    } else {
      goto erro;
    }
  q64: // )
    return generateToken("<Delimitador fecha parentesis ", lexema);
  q65:
    if (digit == ' ') {
      goto q66;
    } else {
      goto erro;
    }
  q66: // ,
    return generateToken("<Delimitador virgula ", lexema);
  q67:
    if (digit == ' ') {
      goto q68;
    } else {
      goto erro;
    }
  q68: // ;
    return generateToken("<Delimitador ponto e virgula ", lexema);
  q69:
    if (digit == ' ') {
      goto q70;
    } else {
      goto erro;
    }
  q70: // +
    return generateToken("<Operador de adicao ", lexema);
  q71:
    if (digit == ' ') {
      goto q72;
    } else {
      goto erro;
    }
  q72: // -
    return generateToken("<Operador de subtracao ", lexema);
  q73:
    if (digit == ' ') {
      goto q74;
    } else {
      goto erro;
    }
  q74: // *
    return generateToken("<Operador de multiplicacao ", lexema);
  q75:
    if (digit == ' ') {
      goto q76;
    }
    if (digit == '*') {
      counter++;
      digit = lexema[counter];
      goto q77;
    } else {
      goto erro;
    }
  q76: // /
    return generateToken("<Operador de divisao ", lexema);
  q77:
    if (digit == ' ') {
      counter++;
      digit = lexema[counter];
      goto q78;
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
      digit == ' '
    ) {
      counter++;
      digit = lexema[counter];
      goto q94;
    }
  q79:
    if (digit == ' ') {
      goto q80;
    }
    if (digit == '=') {
      counter++;
      digit = lexema[counter];
      goto q83;
    }
    if (digit == '>') {
      counter++;
      digit = lexema[counter];
      goto q81;
    } else {
      goto erro;
    }
  q80: // <
    return generateToken("<Operador menor que ", lexema);
  q81:
    if (digit == ' ') {
      goto q82;
    } else {
      goto erro;
    }
  q82: // <>
    return generateToken("<Operador de diferenca ", lexema);
  q83:
    if (digit == ' ') {
      goto q84;
    } else {
      goto erro;
    }
  q84: // <=
    return generateToken("<Operador menor ou igual que ", lexema);
  q85:
    if (digit == '=') {
      counter++;
      digit = lexema[counter];
      goto q87;
    }
    if (digit == ' ') {
      goto q86;
    } else {
      goto erro;
    }
  q86: // >
    return generateToken("<Operador maior que ", lexema);
  q87:
    if (digit == ' ') {
      goto q88;
    } else {
      goto erro;
    }
  q88: // >=
    return generateToken("<Operador maior ou igual que ", lexema);
  q89:
    if (digit == '=') {
      counter++;
      digit = lexema[counter];
      goto q91;
    }
    if (digit == ' ') {
      goto q90;
    } else {
      goto erro;
    }
  q90: // =
    return generateToken("<Operador de atribuicao ", lexema);
  q91:
    if (digit == ' ') {
      goto q92;
    } else {
      goto erro;
    }
  q92: // ==
    return generateToken("<Operador de igualdade ", lexema);
  q93:
    if (digit == ' ') {
      goto q98;
    }
    if (isalpha(digit)) {
      counter++;
      digit = lexema[counter];
      goto q93;
    } else {
      goto erro;
    }
  q94:
    if (digit == '*') {
      counter++;
      digit = lexema[counter];
      goto q94;
    }
    if (digit == '/' || digit == ' ') {
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
      digit == '{'
    ) {
      counter++;
      digit = lexema[counter];
      goto q78;
    }
  q95:
    if (digit == ' ') {
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
      digit == '/' ||
      digit == '*'
    ) {
      counter++;
      digit = lexema[counter];
      goto q94;
    } else {
      goto erro;
    }
  q96: // Comentario
    return generateToken("<Comentário ", lexema);
  q98: // Identificador
    return generateToken("<Identificador ", lexema);
  q99:
    if (isdigit(digit)) {
      counter++;
      digit = lexema[counter];
      goto q99;
    }
    if (digit == ' ') {
      goto q100;
    } else {
      goto erro;
    }
  q100: // Digitos
    return generateToken("<Operador numerico ", lexema);
  erro: // Erro léxico
    return generateToken("<Erro lexico ", lexema);
}