/**

Grupo:

 Nome: Gabriela Lopes Francisco
 TIA: 31744591

 Nome: Giulia Barbieri Quagliano
 TIA: 42013070

 Nome: Jenifer Mathias dos Santos
 TIA: 32092946

 Nome: Lucas Martins de Souza
 TIA: 31943187

 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define FILE_READ_MODE "r"

/** funcao auxiliar para encontrar palavra */
int match(char c, char palavra[], int *pos);

/** funcao auxiliar para verificar se a palavra digitada e um digito */
bool eDigito(char c, char palavra[], int *pos);

/** funcao auxiliar para reconhecer letras maiusculas e minusculas */
int reconheceLetras(char c, char palavra[], int *pos);

/** funcao auxiliar para reconhecer a palavra reservada semic */
int semic(char palavra[], int *pos);

/**  Regra 1 - aceitar declaracao de funcoes, semic, identificador e bloco */
int programa(char palavra[], int *pos);

/** Regra 2 - { aceitar parte de declaracao de variaveis e comando composto } */
int bloco(char palavra[], int *pos);

/** Regra 3 - aceitar <declaracao de variaveis> {declaracao de variaveis} */
int parteDeclaracaoVariaveis(char palavra[], int *pos);

/** funcao auxiliar para reconhecer a palavra int e bool */
int tipoVariaveis(char palavra[], int *pos);

/** Regra 4 - aceitar int ou bool, lista de identificadores e ; */
int declaracaoVariaveis(char palavra[], int *pos);

/** Regra 5 - aceitar identidicador, identidicador */
int listaIdentificadores(char palavra[], int *pos);

/** Regra 6 - aceitar declaracao de funcoes */
int declaracaoFuncoes(char palavra[], int *pos);

/** Regra 7 - declara função: void identidicador ( parametro formal ) <bloco> */
int declaraFuncao(char palavra[], int *pos);

/** funcao auxiliar para reconhecer comentarios */
int comentario(char palavra[], int *pos);

/** Regra 8 - aceitar int ou bool e identidicador */
int parametroFormal(char palavra[], int *pos);

/** Regra 9 - aceitar <comando> ; { <comando> } ; */
int comandoComposto(char palavra[], int *pos);

/** funcao auxiliar print */
int imprimir(char palavra[], int *pos);

/** Regra 10 - aceitar <atribuicao> | <chamada de proc.> | <comando cond.> |
 * <comando rep.> | print ( <identificador> ) */
int comando(char palavra[], int *pos);

/** Regra 11 - aceitar <variavel> = <expressao> */
int atribuicao(char palavra[], int *pos);

/** Regra 12 - aceitar proc <identificador> ( [<parametro>] ) */
int chamadaProcedimento(char palavra[], int *pos);

/** Regra 13 -  aceitar [ ( <identificador> | <numero> | <bool> ) ] */
int parametro(char palavra[], int *pos);

/** Regra 14 - aceitar if ( <expressao> ) { <comando composto> } [ else {
 * <comando composto> } ] */
int comandoCondicional(char palavra[], int *pos);

/** Regra 15 - do { comando composto } while ( expressao ) */
int comandoRepetitivo(char palavra[], int *pos);

/** Regra 16 - aceitar expressao simples [ <relacao> <expressao simples> ]  */
int expressao(char palavra[], int *pos);

/**  Regra 17 - aceitar == | <> | < | <= | >= | > */
int relacao(char palavra[], int *pos);

/** Regra 18 - aceitar [ + | - ] <termo> { [ + | - ] <termo> } */
int expressaoSimples(char palavra[], int *pos);

/** Regra 19 - aceitar <fator>  { (  * | / ) <fator> } */
int termo(char palavra[], int *pos);

/** Regra 20 - aceitar <variavel> | <numero> | <bool> | (<expressao simples>) */
int fator(char palavra[], int *pos);

/** Regra 21 - aceitar <identificador> */
int variavel(char palavra[], int *pos);

/** Regra 22 - aceitar true | false */
int valueBool(char palavra[], int *pos);

/** Regra 23 - aceitar numeros */
int num(char palavra[], int *pos);

/** Regra 24 - aceitar identificadores */
int identificador(char palavra[], int *pos);

/** funcao para tratamento de erro: lexico ou sintatico */
// void trataErro(bool *isLexico, const char *inputFilePath, char *palavra, int
// pos, int linha);
void trataErro(bool isLexico, char *token, char *palavra, int pos, int linha);

/** funcao auxiliar para verificar espacos nas palavreas */
// int verificaEspacos(char palavra[], int *pos);

char lookahead; /** Excepcionalmente variavel global */

int linha = 1;

/** funcao auxiliar para encontrar palavra */
int match(char c, char palavra[], int *pos) {
  if (lookahead == c) {
    lookahead = palavra[++(*pos)];
    return 1;
  }
  return 0;
}

/** funcao auxiliar para verificar se a palavra digitada e um digito */
bool eDigito(char c, char palavra[], int *pos) {
  if (c >= '0' && c <= '9') {
    lookahead = palavra[++(*pos)];
    if (lookahead >= '0' && lookahead <= '9') {
      if (eDigito(lookahead, palavra, pos)) {
        return true;
      }
    }
    return true;
  }
  return false;
}

/** funcao auxiliar para reconhecer letras maiusculas e minusculas */
int reconheceLetras(char c, char palavra[], int *pos) {
  if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
    lookahead = palavra[++(*pos)];
    if ((lookahead >= 'a' && lookahead <= 'z') ||
        (lookahead >= 'A' && lookahead <= 'Z')) {
      if (reconheceLetras(lookahead, palavra, pos)) {
        return 1;
      }
    } else if (lookahead >= '0' && lookahead <= '9') {
      if (eDigito(lookahead, palavra, pos)) {
        return 1;
      }
    }
    return 1;
  } else {
    return 0;
  }
}

/** funcao auxiliar para reconhecer a palavra reservada semic */
int semic(char palavra[], int *pos) {
  if (lookahead == 's') {
    if (match('s', palavra, pos) && match('e', palavra, pos) &&
        match('m', palavra, pos) && match('i', palavra, pos) &&
        match('c', palavra, pos) && identificador(palavra, pos) &&
        bloco(palavra, pos) && !reconheceLetras(lookahead, palavra, pos) &&
        !eDigito(lookahead, palavra, pos)) {
      return 1;
    }
  }
  return 0;
}

// /** funcao auxiliar para verificar espacos nas palavreas */
// int verificaEspacos(char palavra[], int* pos) {
//     if (lookahead == ' ' || lookahead == '\n' || lookahead == '\t' ||
//     lookahead == '\0') {
//         if (match(' ', palavra, pos) || match('\n', palavra, pos) ||
//         match('\t', palavra, pos) || match('\0', palavra, pos)) {
//             // Remove espaços extras
//           if(lookahead == ' ' || lookahead == '\n' || lookahead == '\t' ||
//           lookahead == '\0') {
//                 verificaEspacos(palavra, pos);
//             }
//             return 1;
//         }
//         return 1;
//     }
//     return 0;
// }

/**  Regra 1 - aceitar declaracao de funcoes, semic, identificador e bloco */
int programa(char palavra[], int *pos) {
  if (lookahead == 'v') {
    if (declaracaoFuncoes(palavra, pos) && semic(palavra, pos)) {
      return 1;
    }
  } else if (lookahead == 's') {
    if (semic(palavra, pos)) {
      return 1;
    }
  }
  return 0;
}

/** Regra 2 - { aceitar parte de declaracao de variaveis e comando composto } */
int bloco(char palavra[], int *pos) {
  if (lookahead == '{') {
    match('{', palavra, pos) && parteDeclaracaoVariaveis(palavra, pos);
    if(lookahead=='/' && lookahead=='*'){
      comentario(palavra, pos);
    }
    comandoComposto(palavra, pos);
    match('}', palavra, pos);
      return 1;
  }
  return 0;
}

/** Regra 3 - aceitar <declaracao de variaveis> {declaracao de variaveis} */
int parteDeclaracaoVariaveis(char palavra[], int *pos) {
  return declaracaoVariaveis(palavra, pos);
}

/** funcao auxiliar para reconhecer a palavra int e bool */
int tipoVariaveis(char palavra[], int *pos) {
  if (lookahead == 'i') {
    if (match('i', palavra, pos) && match('n', palavra, pos) &&
        match('t', palavra, pos) && !eDigito(lookahead, palavra, pos) &&
        !reconheceLetras(lookahead, palavra, pos)) {
      return 1;
    }
  } else if (lookahead == 'b') {
    if (match('b', palavra, pos) && match('o', palavra, pos) &&
        match('o', palavra, pos) && match('l', palavra, pos) &&
        !eDigito(lookahead, palavra, pos) &&
        !reconheceLetras(lookahead, palavra, pos)) {
      return 1;
    }
  }
  return 0;
}

/** Regra 4 - aceitar int ou bool, lista de identificadores e ; */
int declaracaoVariaveis(char palavra[], int *pos) {
  if (tipoVariaveis(palavra, pos) && listaIdentificadores(palavra, pos) &&
      match(';', palavra, pos)) {
    if(lookahead=='b' || (lookahead=='i' && lookahead=='n')){
      declaracaoVariaveis(palavra, pos);
      return 1;
    }
    return 1;
  }
  return 0;
}

/** Regra 5 - aceitar <identidicador>, <identidicador> */
int listaIdentificadores(char palavra[], int *pos) {
  if (lookahead == '_') {
    identificador(palavra, pos);
    if (lookahead == ',') {
      match(',', palavra, pos) && listaIdentificadores(palavra, pos);
      return 1;
    }
    return 1;
  }
  return 0;
}

/** Regra 6 - <declaracao de funcoes> */
int declaracaoFuncoes(char palavra[], int *pos) {
  declaraFuncao(palavra, pos);
  if(lookahead=='v'){
    declaracaoFuncoes(palavra, pos);
  }
  return 1;
}

/** Regra 7 - declara função: void <identidicador> ( <parametro formal> )
 * <bloco> */
int declaraFuncao(char palavra[], int *pos) {
  if (lookahead == 'v') {
    if (match('v', palavra, pos) && match('o', palavra, pos) &&
        match('i', palavra, pos) && match('d', palavra, pos) &&
        !eDigito(lookahead, palavra, pos) &&
        !reconheceLetras(lookahead, palavra, pos) &&
        identificador(palavra, pos) && match('(', palavra, pos) &&
        parametroFormal(palavra, pos) && match(')', palavra, pos) &&
        bloco(palavra, pos)) {
      return 1;
    }
  }
  return 0;
}

/** funcao auxiliar para reconhecer comentarios */
int comentario(char palavra[], int *pos) {
  if (palavra[*pos] == '/' && palavra[*pos + 1] == '*') {
    *pos += 2; // Avança para o próximo caractere após '/*'
    while (palavra[*pos] != '\0') {
      if (palavra[*pos] == '*' && palavra[*pos + 1] == '/') {
        *pos += 2;   // Avança para o próximo caractere após '*/'
        return *pos; // Retorna a posição final do comentário
      }
      (*pos)++;
    }
    trataErro(false, palavra, &palavra[*pos], *pos, linha);
  }
  return 0;
}

/** Regra 8 - aceitar int ou bool e identidicador */
int parametroFormal(char palavra[], int *pos) {
  if (lookahead == 'i' || lookahead == 'b') {
    tipoVariaveis(palavra, pos) && identificador(palavra, pos);
    return 1;
  } else if (lookahead == 'i' || lookahead == 'b') {
    tipoVariaveis(palavra, pos) && identificador(palavra, pos) &&
        comentario(palavra, pos);
    return 1;
  }
  return 0;
}

/** Regra 9 - aceitar <comando> ; { <comando> } ; */
int comandoComposto(char palavra[], int *pos) {
  if (lookahead == '_' || lookahead == 'i' || lookahead == 'p' ||
      lookahead == 'd') {
    comando(palavra, pos) && match(';', palavra, pos);
    if(lookahead == '_' || lookahead == 'i' || lookahead == 'p' ||
      lookahead == 'd'){
      comandoComposto(palavra, pos);
      return 1;
      }
    return 1;
  }
  return 0;
}

/** Regra 10 - aceitar <atribuicao> | <chamada de proc.> | <comando cond.> |
 * <comando rep.> | print ( <identificador> ) */
int comando(char palavra[], int *pos) {
  if (lookahead == '_') {
    if (atribuicao(palavra, pos)) {
      return 1;
    }
  } else if (match('proc', palavra, pos)) {
    if (chamadaProcedimento(palavra, pos)) {
      return 1;
    }
  } else if (lookahead == 'i') {
    if (comandoCondicional(palavra, pos)) {
      return 1;
    }
  } else if (lookahead == 'd') {
    if (comandoRepetitivo(palavra, pos)) {
      return 1;
    }
  } else if (lookahead == 'p') {
    if (imprimir(palavra, pos)) {
      return 1;
    }
  } else {
    if (comandoComposto(palavra, pos)) {
      return 1;
    }
  }
  return 0;
}

/** funcao auxiliar print */
int imprimir(char palavra[], int *pos) {
  if (lookahead == 'p') {
    if (match('p', palavra, pos) && match('r', palavra, pos) &&
        match('i', palavra, pos) && match('n', palavra, pos) &&
        match('t', palavra, pos) && match('(', palavra, pos) &&
        identificador(palavra, pos) && match(')', palavra, pos)) {
      return 1;
    } else {
      match('p', palavra, pos) && match('r', palavra, pos) &&
          match('i', palavra, pos) && match('n', palavra, pos) &&
          match('t', palavra, pos) && match('(', palavra, pos) &&
          identificador(palavra, pos) && match(')', palavra, pos) &&
          comentario(palavra, pos);
    }
  }
  return 0;
}

/** Regra 11 - aceitar <variavel> = <expressao> */
int atribuicao(char palavra[], int *pos) {
  if (variavel(palavra, pos) && match('=', palavra, pos) &&
      expressao(palavra, pos) && !reconheceLetras(lookahead, palavra, pos) &&
      !eDigito(lookahead, palavra, pos)) {
    return 1;
  }
  return 0;
}

/** Regra 12 - aceitar proc <identificador> ( [<parametro>] ) */
int chamadaProcedimento(char palavra[], int *pos) {
  if (lookahead == 'p') {
    if (match('p', palavra, pos) && match('r', palavra, pos) &&
        match('o', palavra, pos) && match('c', palavra, pos) &&
        identificador(palavra, pos) && match('(', palavra, pos) &&
        match(')', palavra, pos)) {
      return 1;
    } else if (match('p', palavra, pos) && match('r', palavra, pos) &&
               match('o', palavra, pos) && match('c', palavra, pos) &&
               identificador(palavra, pos) && match('(', palavra, pos) &&
               parametro(palavra, pos) && match(')', palavra, pos)) {
      return 1;
    } else
      return 0;
  } else
    return 0;
}

/** Regra 13 -  aceitar [ ( <identificador> | <numero> | <bool> ) ] */
int parametro(char palavra[], int *pos) {
  if (identificador(palavra, pos) || num(palavra, pos) ||
      valueBool(palavra, pos)) {
    return 1;
  } else
    return 0;
}

/** Regra 14 - aceitar if ( <expressao> ) { <comando composto> } [ else {
 * <comando composto> } ] */
int comandoCondicional(char palavra[], int *pos) {
  if (lookahead == 'i') {
    if (match('i', palavra, pos) && match('f', palavra, pos) &&
        match('(', palavra, pos) && expressao(palavra, pos) &&
        match(')', palavra, pos) && match('{', palavra, pos) &&
        comandoComposto(palavra, pos) && match('}', palavra, pos)) {
      return 1;
    } else if (match('i', palavra, pos) && match('f', palavra, pos) &&
               match('(', palavra, pos) && expressao(palavra, pos) &&
               match(')', palavra, pos) && match('{', palavra, pos) &&
               comandoComposto(palavra, pos) && match('}', palavra, pos) &&
               match('e', palavra, pos) && match('l', palavra, pos) &&
               match('s', palavra, pos) && match('e', palavra, pos) &&
               match('{', palavra, pos) && comandoComposto(palavra, pos) &&
               match('}', palavra, pos)) {
      return 1;
    }
  }
  return 0;
}

/** Regra 15 - do { comando composto } while ( expressao ) */
int comandoRepetitivo(char palavra[], int *pos) {
  if (lookahead == 'd') {
    if (match('d', palavra, pos) && match('o', palavra, pos) &&
        match('{', palavra, pos) && comandoComposto(palavra, pos) &&
        match('}', palavra, pos) && match('w', palavra, pos) &&
        match('h', palavra, pos) && match('i', palavra, pos) &&
        match('l', palavra, pos) && match('e', palavra, pos) &&
        match('(', palavra, pos) && expressao(palavra, pos) &&
        match(')', palavra, pos)) {
      return 1;
    }
  }
  return 0;
}

/** Regra 16 - aceitar expressao simples [ <relacao> <expressao simples> ]  */
int expressao(char palavra[], int *pos) {
  if (expressaoSimples(palavra, pos)) {
    if (lookahead == '=' || lookahead == '<' || lookahead == '>') {
      if (relacao(palavra, pos) && expressaoSimples(palavra, pos)) {
        return 1;
      }
    }
    return 1;
  }
  return 0;
}

/**  Regra 17 - aceitar == | <> | < | <= | >= | > */
int relacao(char palavra[], int *pos) {
  if (lookahead == '=') {
    if (match('=', palavra, pos) && match('=', palavra, pos)) {
      return 1;
    }
  } else if (lookahead == '<') {
    match('<', palavra, pos);
    if(lookahead=='='){
      match('=', palavra, pos);
      return 1;
    }
    else if(lookahead=='>'){
      match('>', palavra, pos);
      return 1;
    }
      return 1;
  } else if (lookahead == '>') {
    match('>', palavra, pos);
      if(lookahead=='=') {
      match('=', palavra, pos);
      return 1;
    }
      return 1;
  }
  return 0;
}

/** Regra 18 - aceitar [ + | - ] <termo> { [ + | - ] <termo> } */
int expressaoSimples(char palavra[], int *pos) {
  if (lookahead == '-') {
    if (match('-', palavra, pos) && termo(palavra, pos)) {
      if (lookahead != ';' || lookahead != ')' || lookahead != '=' ||
          lookahead != '<' || lookahead != '>') {
          return 1;
        }
      return 1;
    }
  } else if (lookahead == '+') {
    if (match('+', palavra, pos) && termo(palavra, pos)) {
      if (lookahead != ';' || lookahead != ')' || lookahead != '=' ||
          lookahead != '<' || lookahead != '>') {
        if (expressaoSimples(palavra, pos)) {
          return 1;
        }
      }
      return 1;
    }
  } else if (termo(palavra, pos)) {
    return 1;
  }
  return 0;
}

/** Regra 19 - aceitar <fator>  { (  * | / ) <fator> } */
int termo(char palavra[], int *pos) {
  if (fator(palavra, pos)) {
    if (lookahead == '*') {
      if (match('*', palavra, pos) && fator(palavra, pos)) {
        return 1;
      }
    } else if (lookahead == '/') {
      if (match('/', palavra, pos) && fator(palavra, pos)) {
        return 1;
      }
    }
    return 1;
  }
  return 0;
}

/** Regra 20 - aceitar <variavel> | <numero> | <bool> | (<expressao simples>) */
int fator(char palavra[], int *pos) {
  if (lookahead == '(') {
    if (match('(', palavra, pos) && expressaoSimples(palavra, pos) &&
        match(')', palavra, pos)) {
      return 1;
    }
  } else if (lookahead == '_') {
    if (variavel(palavra, pos)) {
      return 1;
    }
  } else if (lookahead == 't' || lookahead == 'f') {
    if (valueBool(palavra, pos)) {
      return 1;
    }
  } else if (eDigito(lookahead, palavra, pos)) {
    return 1;
  }
  return 0;
}

/** Regra 21 - aceitar <identificador> */
int variavel(char palavra[], int *pos) { return identificador(palavra, pos); }

/** Regra 22 - aceitar true ou false */
int valueBool(char palavra[], int *pos) {
  if (lookahead == 't') {
    if (match('t', palavra, pos) && match('r', palavra, pos) &&
        match('u', palavra, pos) && match('e', palavra, pos) &&
        !reconheceLetras(lookahead, palavra, pos) &&
        !eDigito(lookahead, palavra, pos)) {
      return 1;
    }
  } else if (lookahead == 'f') {
    if (match('f', palavra, pos) && match('a', palavra, pos) &&
        match('l', palavra, pos) && match('s', palavra, pos) &&
        match('e', palavra, pos) && !reconheceLetras(lookahead, palavra, pos) &&
        !eDigito(lookahead, palavra, pos)) {
      return 1;
    }
  }
  return 0; /** A palavra não é "true" nem "false" */
}

/** Regra 23 - aceitar numeros */
int num(char palavra[], int *pos) {
  if ((match('0', palavra, pos) || match('1', palavra, pos) ||
       match('2', palavra, pos) || match('3', palavra, pos) ||
       match('4', palavra, pos) || match('5', palavra, pos) ||
       match('6', palavra, pos) || match('7', palavra, pos) ||
       match('8', palavra, pos) || match('9', palavra, pos)) &&
      !reconheceLetras(lookahead, palavra, pos)) {
    return 1;
  } else {
    return 0;
  }
}

/** Regra 24 - aceitar identificadores */
int identificador(char palavra[], int *pos) {
  if (lookahead == '_') {
    match('_', palavra, pos) && reconheceLetras(lookahead, palavra, pos);
    return 1;
  } else if (lookahead == listaIdentificadores(palavra, pos)) {
    match('_', palavra, pos) && reconheceLetras(lookahead, palavra, pos) &&
        match(',', palavra, pos);
    return 1;
  } else {
    return 0;
  }
}

/** funcao para tratamento de erro: lexico ou sintatico */
void trataErro(bool isLexico, char *token, char *palavra, int pos, int linha) {
  if (isLexico) {
    printf("\nErro léxico encontrado!\n");
  } else {
    printf("\nErro sintático encontrado na linha %d:\n\n%s\n", linha, token);
  }
  printf("Posição do erro: %d\n", pos);
  printf("Caractere com erro: %c\n", palavra[pos]);
}

int main() {
  char inputFilePath[] = "/home/runner/Comp/entrada.txt";
  bool isLexico;
  int pos = 0;
  int linha = 1; // Variável para acompanhar a linha atual

  FILE *file = fopen(inputFilePath, FILE_READ_MODE);
  if (file == NULL) {
    printf("\nArquivo não encontrado!\n");
    return 1;
  }

  /** Cálculo do tamanho do arquivo */
  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  fseek(file, 0, SEEK_SET);

  /** Alocação dinâmica */
  char *palavra = (char *)malloc((fileSize + 1) * sizeof(char));
  if (palavra == NULL) {
    printf("Erro na alocação de memória\n");
    fclose(file);
    return 1;
  }

  /** Leitura do conteúdo do arquivo */
  int c;
  while ((c = fgetc(file)) != EOF) {
    if (c == ' ' || c == '\t' || c == '\n' || c == ' ') {
      continue;
    }
    palavra[pos] = c;
    pos++;
  }
  palavra[pos] = '\0';

  fclose(file);
  lookahead = palavra[0];
  pos = 0;

  if (programa(palavra, &pos)) {
    printf("\nPalavra reconhecida!\n\n");
    printf("Conteúdo do arquivo:\n%s\n", palavra);
  } else {
    trataErro(isLexico, inputFilePath, palavra, pos, linha);
  }

  /** Libera memória alocada */
  free(palavra);

  return 0;
}