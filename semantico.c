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

// Estrutura do nó da linked list de identificadores
typedef struct IdentifierNode
{
  Token token;
  Token type;
  char *lexema;
  int isFunction;
  struct IdentifierNode *next;
} IdentifierNode;

// Estrutura do bloco
typedef struct Block
{
  int blockIdentifier;
  IdentifierNode *identifiers;
  struct Block *next;
} Block;

// Estrutura da tabela de símbolos
typedef struct SymbolTable
{
  Block *head;
} SymbolTable;

typedef struct Scanner
{
  char *token;
  char *lexema;
} Scanner;

Scanner *lookahead; /* Excepcionalmente variavel global */
int blockId = 0;

SymbolTable *createSymbolTable();
void createBlock(SymbolTable *symbolTable, int blockIdentifier);
void addIdentifier(SymbolTable *symbolTable, int blockIdentifier, Token token, Token type, char *lexema, int isFunction);
void editIdentifier(SymbolTable *symbolTable, int blockIdentifier, Token oldToken, Token newToken, Token newType, char *newLexema);
void removeIdentifier(SymbolTable *symbolTable, int blockIdentifier, Token token);
IdentifierNode *searchIdentifier(SymbolTable *symbolTable, char *lexema);
void deleteBlock(SymbolTable *symbolTable, int blockIdentifier);
void freeSymbolTable(SymbolTable *symbolTable);
void printSymbolTable(SymbolTable *symbolTable);

// Funções do Análisador Sintático
int match(char *t, char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int *blockCounter);
int S(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int *blockCounter);
int DFUNC(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int *blockCounter);
int COMENTARIO(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int *blockCounter);
int PARAMSF(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int *blockCounter);
int TIPO(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int *blockCounter);
int DVAR(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int *blockCounter, Token type);
int DVAR_LIST(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int *blockCounter, Token type);
int BLOCO(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int *blockCounter);
int ATRIB(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int *blockCounter);
int PROC(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int *blockCounter);
int COND(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int *blockCounter);
int REPET(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int *blockCounter);
int PRINT(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int *blockCounter);
int EXPR(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int *blockCounter);
int RELACAO(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int *blockCounter);
int IDENTIFICADOR_LIST(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int *blockCounter, Token type);

void adicionarCaractere(char **string, char novoCaractere)
{
  int tamanhoAtual = 0;
  if (*string != NULL)
  {
    tamanhoAtual = strlen(*string);
  }

  // Redimensiona a memória alocada para a string (incluindo espaço para o novo caractere e o caractere nulo de terminação)
  *string = realloc(*string, (tamanhoAtual + 2) * sizeof(char));

  // Adiciona o novo caractere à string
  (*string)[tamanhoAtual] = novoCaractere;
  (*string)[tamanhoAtual + 1] = '\0';
}

int *convertIntToChar(int *intPtr)
{
  if (intPtr == NULL)
  {
    return 0; // Valor padrão para um int
  }

  int intValue = *intPtr;
  // return intValue;
  return intPtr;
}

// Função scanner do análisador léxico
Scanner *scanner(char *lexema, int *counter, char *erroSintatico);

int match(char *t, char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int *blockCounter)
{
  // printf("lookahead->lexema: %s\n", lookahead->lexema);
  if (lookahead->token == t)
  {
    lookahead = scanner(palavra, counter, erroSintatico);
    if (lookahead->token == tokenToStr[_COMENTARIO])
    {
      COMENTARIO(palavra, counter, erroSintatico, symbolTable, blockCounter);
    }
    return 1;
  }

  return 0;
}

// PARAMSF -> TIPO IDENTIFICADOR | NUMERO | true | false | TIPO IDENTIFICADOR , PARAMSF
int PARAMSF(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int *blockCounter)
{
  if (lookahead->token == tokenToStr[NUM])
  {
    match(tokenToStr[NUM], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead->token == tokenToStr[TRUE])
  {
    match(tokenToStr[TRUE], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead->token == tokenToStr[FALSE])
  {
    match(tokenToStr[FALSE], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }

  if (lookahead->token == tokenToStr[INT])
  {
    match(tokenToStr[INT], palavra, counter, erroSintatico, symbolTable, blockCounter);
    addIdentifier(symbolTable, blockId, IDENTIFICADOR, INT, lookahead->lexema, 0);
    match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead->token == tokenToStr[BOOL])
  {
    match(tokenToStr[BOOL], palavra, counter, erroSintatico, symbolTable, blockCounter);
    addIdentifier(symbolTable, blockId, IDENTIFICADOR, BOOL, lookahead->lexema, 0);
    match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }

  if (lookahead->token == tokenToStr[VIRGULA])
  {
    match(tokenToStr[VIRGULA], palavra, counter, erroSintatico, symbolTable, blockCounter);
    PARAMSF(palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else
  {
    return 1;
  }
}

// TIPO -> int, void, bool, semic
int TIPO(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int *blockCounter)
{
  if (lookahead->token == tokenToStr[INT])
  {
    match(tokenToStr[INT], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead->token == tokenToStr[VOID])
  {
    match(tokenToStr[VOID], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead->token == tokenToStr[SEMIC])
  {
    match(tokenToStr[SEMIC], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead->token == tokenToStr[BOOL])
  {
    match(tokenToStr[BOOL], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
}

// IDENTIFICADOR_LIST -> IDENTIFICADOR | IDENTIFICADOR , IDENTIFICADOR_LIST
int IDENTIFICADOR_LIST(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int *blockCounter, Token type)
{
  addIdentifier(symbolTable, blockId, IDENTIFICADOR, type, lookahead->lexema, 0);
  match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico, symbolTable, blockCounter);
  if (lookahead->token == tokenToStr[VIRGULA])
  {
    match(tokenToStr[VIRGULA], palavra, counter, erroSintatico, symbolTable, blockCounter);
    addIdentifier(symbolTable, blockId, IDENTIFICADOR, type, lookahead->lexema, 0);
    match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
}

// DVAR -> TIPO IDENTIFICADOR_LIST ;
int DVAR(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int *blockCounter, Token type)
{
  TIPO(palavra, counter, erroSintatico, symbolTable, blockCounter);
  addIdentifier(symbolTable, blockId, IDENTIFICADOR, type, lookahead->lexema, 0);
  match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico, symbolTable, blockCounter);

  if (lookahead->token == tokenToStr[VIRGULA])
  {
    match(tokenToStr[VIRGULA], palavra, counter, erroSintatico, symbolTable, blockCounter);
    IDENTIFICADOR_LIST(palavra, counter, erroSintatico, symbolTable, blockCounter, type);
  }

  match(tokenToStr[PONTO_E_VIRGULA], palavra, counter, erroSintatico, symbolTable, blockCounter);
}

// DVAR_LIST -> DVAR | DVAR DVAR_LIST | ATRIB | e
int DVAR_LIST(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int *blockCounter, Token type)
{
  if (lookahead->token == tokenToStr[INT] || lookahead->token == tokenToStr[BOOL])
  {
    DVAR(palavra, counter, erroSintatico, symbolTable, blockCounter, type);
  }
  else if (lookahead->token == tokenToStr[IDENTIFICADOR])
  {
    ATRIB(palavra, counter, erroSintatico, symbolTable, blockCounter);
    DVAR_LIST(palavra, counter, erroSintatico, symbolTable, blockCounter, type);
  }
  else
  {
    return 0;
  }
}

// COMENTARIO -> /* [a, z] [1, 9]  */
int COMENTARIO(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int *blockCounter)
{
  match(tokenToStr[_COMENTARIO], palavra, counter, erroSintatico, symbolTable, blockCounter);
}

// ATRIB -> IDENTIFICADOR = IDENTIFICADOR ; | IDENTIFICADOR = NUMERO ; | IDENTIFICADOR = IDENTIFICADOR + NUMERO ; | IDENTIFICADOR = NUMERO + IDENTIFICADOR ; | IDENTIFICADOR = true ; | IDENTIFICADOR = false ;
int ATRIB(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int *blockCounter)
{
  searchIdentifier(symbolTable, lookahead->lexema);
  match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico, symbolTable, blockCounter);
  match(tokenToStr[ATRIBUICAO], palavra, counter, erroSintatico, symbolTable, blockCounter);
  // Primeiro operando
  if (lookahead->token == tokenToStr[NUM])
  {
    match(tokenToStr[NUM], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead->token == tokenToStr[IDENTIFICADOR])
  {
    searchIdentifier(symbolTable, lookahead->lexema);
    match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }

  // Operadores
  if (lookahead->token == tokenToStr[OPR_ADICAO])
  {
    match(tokenToStr[OPR_ADICAO], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead->token == tokenToStr[OPR_SUBTRACAO])
  {
    match(tokenToStr[OPR_SUBTRACAO], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead->token == tokenToStr[OPR_MULTIPLICACAO])
  {
    match(tokenToStr[OPR_MULTIPLICACAO], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead->token == tokenToStr[OPR_DIVISAO])
  {
    match(tokenToStr[OPR_DIVISAO], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }

  // Segundo operando
  if (lookahead->token == tokenToStr[NUM])
  {
    match(tokenToStr[NUM], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead->token == tokenToStr[IDENTIFICADOR])
  {
    searchIdentifier(symbolTable, lookahead->lexema);
    match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }

  else if (lookahead->token == tokenToStr[FALSE])
  {
    match(tokenToStr[FALSE], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead->token == tokenToStr[TRUE])
  {
    match(tokenToStr[TRUE], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }

  match(tokenToStr[PONTO_E_VIRGULA], palavra, counter, erroSintatico, symbolTable, blockCounter);
}

// PROC -> proc IDENTIFICADOR | “proc” IDENTIFICADOR ( PARAMSF ) ;
int PROC(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int *blockCounter)
{
  match(tokenToStr[_PROC], palavra, counter, erroSintatico, symbolTable, blockCounter);
  searchIdentifier(symbolTable, lookahead->lexema);
  match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico, symbolTable, blockCounter);
  if (lookahead->token == tokenToStr[ABRE_PARENTESIS])
  {
    match(tokenToStr[ABRE_PARENTESIS], palavra, counter, erroSintatico, symbolTable, blockCounter);
    PARAMSF(palavra, counter, erroSintatico, symbolTable, blockCounter);
    match(tokenToStr[FECHA_PARENTESIS], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  match(tokenToStr[PONTO_E_VIRGULA], palavra, counter, erroSintatico, symbolTable, blockCounter);
}

// PRINT -> print ( IDENTIFICADOR ) ;
int PRINT(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int *blockCounter)
{
  match(tokenToStr[_PRINT], palavra, counter, erroSintatico, symbolTable, blockCounter);
  match(tokenToStr[ABRE_PARENTESIS], palavra, counter, erroSintatico, symbolTable, blockCounter);
  searchIdentifier(symbolTable, lookahead->lexema);
  match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico, symbolTable, blockCounter);
  match(tokenToStr[FECHA_PARENTESIS], palavra, counter, erroSintatico, symbolTable, blockCounter);
  match(tokenToStr[PONTO_E_VIRGULA], palavra, counter, erroSintatico, symbolTable, blockCounter);
}

// REPET -> do { BLOCO } while ( EXPR ) ;
int REPET(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int *blockCounter)
{
  match(tokenToStr[DO], palavra, counter, erroSintatico, symbolTable, blockCounter);
  match(tokenToStr[ABRE_CHAVES], palavra, counter, erroSintatico, symbolTable, blockCounter);
  blockId++;
  createBlock(symbolTable, blockId);
  BLOCO(palavra, counter, erroSintatico, symbolTable, blockCounter);
  match(tokenToStr[FECHA_CHAVES], palavra, counter, erroSintatico, symbolTable, blockCounter);
  match(tokenToStr[WHILE], palavra, counter, erroSintatico, symbolTable, blockCounter);
  match(tokenToStr[ABRE_PARENTESIS], palavra, counter, erroSintatico, symbolTable, blockCounter);
  EXPR(palavra, counter, erroSintatico, symbolTable, blockCounter);
  match(tokenToStr[FECHA_PARENTESIS], palavra, counter, erroSintatico, symbolTable, blockCounter);
  match(tokenToStr[PONTO_E_VIRGULA], palavra, counter, erroSintatico, symbolTable, blockCounter);
}

// BLOCO -> DVAR_LIST BLOCO | PROC BLOCO | COND BLOCO | PRINT BLOCO | REPET BLOCO | e
int BLOCO(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int *blockCounter)
{
  // Lista de váriáveis + atribuição
  if (lookahead->token == tokenToStr[INT] || lookahead->token == tokenToStr[BOOL] || lookahead->token == tokenToStr[IDENTIFICADOR])
  {
    DVAR_LIST(palavra, counter, erroSintatico, symbolTable, blockCounter, lookahead->token == tokenToStr[INT] ? INT : BOOL);
    BLOCO(palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  // Proc
  else if (lookahead->token == tokenToStr[_PROC])
  {
    PROC(palavra, counter, erroSintatico, symbolTable, blockCounter);
    BLOCO(palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  // If
  else if (lookahead->token == tokenToStr[IF])
  {
    COND(palavra, counter, erroSintatico, symbolTable, blockCounter);
    BLOCO(palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  // Print
  else if (lookahead->token == tokenToStr[_PRINT])
  {
    PRINT(palavra, counter, erroSintatico, symbolTable, blockCounter);
    BLOCO(palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  // Do while
  else if (lookahead->token == tokenToStr[DO])
  {
    REPET(palavra, counter, erroSintatico, symbolTable, blockCounter);
    BLOCO(palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else
  {
    return 0;
  }
}

// RELAÇÃO -> == | <> | < | <= | >= | >
int RELACAO(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int *blockCounter)
{
  if (lookahead->token == tokenToStr[IGUALDADE])
  {
    match(tokenToStr[IGUALDADE], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead->token == tokenToStr[DIFERENCA])
  {
    match(tokenToStr[DIFERENCA], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead->token == tokenToStr[MENOR_QUE])
  {
    match(tokenToStr[MENOR_QUE], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead->token == tokenToStr[MENOR_OU_IGUAL_QUE])
  {
    match(tokenToStr[MENOR_OU_IGUAL_QUE], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead->token == tokenToStr[MAIOR_QUE])
  {
    match(tokenToStr[MAIOR_QUE], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead->token == tokenToStr[MAIOR_OU_IGUAL_QUE])
  {
    match(tokenToStr[MAIOR_OU_IGUAL_QUE], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
}

// EXPR -> true | false | IDENTIFICADOR RELACAO IDENTIFICADOR | IDENTIFICADOR RELACAO NUMERO | IDENTIFICADOR RELACAO false | IDENTIFICADOR RELACAO true
int EXPR(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int *blockCounter)
{
  if (lookahead->token == tokenToStr[TRUE])
  {
    match(tokenToStr[TRUE], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead->token == tokenToStr[FALSE])
  {
    match(tokenToStr[FALSE], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }

  if (lookahead->token == tokenToStr[IDENTIFICADOR])
  {
    searchIdentifier(symbolTable, lookahead->lexema);
    match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico, symbolTable, blockCounter);
    RELACAO(palavra, counter, erroSintatico, symbolTable, blockCounter);
    if (lookahead->token == tokenToStr[IDENTIFICADOR])
    {
      searchIdentifier(symbolTable, lookahead->lexema);
      match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico, symbolTable, blockCounter);
    }
    else if (lookahead->token == tokenToStr[NUM])
    {
      match(tokenToStr[NUM], palavra, counter, erroSintatico, symbolTable, blockCounter);
    }
    else if (lookahead->token == tokenToStr[TRUE])
    {
      match(tokenToStr[TRUE], palavra, counter, erroSintatico, symbolTable, blockCounter);
    }
    else if (lookahead->token == tokenToStr[FALSE])
    {
      match(tokenToStr[FALSE], palavra, counter, erroSintatico, symbolTable, blockCounter);
    }
  }
}

// COND -> if ( EXPR ) { BLOCO } | if ( EXPR ) { BLOCO } else { BLOCO } ;
int COND(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int *blockCounter)
{
  match(tokenToStr[IF], palavra, counter, erroSintatico, symbolTable, blockCounter);
  match(tokenToStr[ABRE_PARENTESIS], palavra, counter, erroSintatico, symbolTable, blockCounter);
  EXPR(palavra, counter, erroSintatico, symbolTable, blockCounter);
  match(tokenToStr[FECHA_PARENTESIS], palavra, counter, erroSintatico, symbolTable, blockCounter);
  match(tokenToStr[ABRE_CHAVES], palavra, counter, erroSintatico, symbolTable, blockCounter);
  blockId++;
  createBlock(symbolTable, blockId);
  BLOCO(palavra, counter, erroSintatico, symbolTable, blockCounter);
  match(tokenToStr[FECHA_CHAVES], palavra, counter, erroSintatico, symbolTable, blockCounter);

  if (lookahead->token == tokenToStr[ELSE])
  {
    match(tokenToStr[ELSE], palavra, counter, erroSintatico, symbolTable, blockCounter);
    match(tokenToStr[ABRE_CHAVES], palavra, counter, erroSintatico, symbolTable, blockCounter);
    blockId++;
    createBlock(symbolTable, blockId);
    BLOCO(palavra, counter, erroSintatico, symbolTable, blockCounter);
    match(tokenToStr[FECHA_CHAVES], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }

  match(tokenToStr[PONTO_E_VIRGULA], palavra, counter, erroSintatico, symbolTable, blockCounter);
}

// DFUNC -> TIPO IDENTIFICADOR ( PARAMSF ) BLOCO | TIPO IDENTIFICADOR ( PARAMSF ) BLOCO DFUNC | e
int DFUNC(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int *blockCounter)
{
  if (lookahead->token == tokenToStr[SEMIC])
  {
    TIPO(palavra, counter, erroSintatico, symbolTable, blockCounter);
    addIdentifier(symbolTable, 0, IDENTIFICADOR, SEMIC, lookahead->lexema, 1);
    match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico, symbolTable, blockCounter);
    match(tokenToStr[ABRE_CHAVES], palavra, counter, erroSintatico, symbolTable, blockCounter);
    blockId++;
    createBlock(symbolTable, blockId);
    BLOCO(palavra, counter, erroSintatico, symbolTable, blockCounter);
    match(tokenToStr[FECHA_CHAVES], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead->token == tokenToStr[VOID] || lookahead->token == tokenToStr[INT] || lookahead->token == tokenToStr[BOOL])
  {
    TIPO(palavra, counter, erroSintatico, symbolTable, blockCounter);
    addIdentifier(symbolTable, blockId, IDENTIFICADOR, lookahead->token == tokenToStr[VOID] ? VOID : lookahead->token == tokenToStr[INT] ? INT
                                                                                                                                         : BOOL,
                  lookahead->lexema, 0);
    match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico, symbolTable, blockCounter);
    match(tokenToStr[ABRE_PARENTESIS], palavra, counter, erroSintatico, symbolTable, blockCounter);
    blockId++;
    createBlock(symbolTable, blockId);
    PARAMSF(palavra, counter, erroSintatico, symbolTable, blockCounter);
    match(tokenToStr[FECHA_PARENTESIS], palavra, counter, erroSintatico, symbolTable, blockCounter);
    match(tokenToStr[ABRE_CHAVES], palavra, counter, erroSintatico, symbolTable, blockCounter);
    BLOCO(palavra, counter, erroSintatico, symbolTable, blockCounter);
    match(tokenToStr[FECHA_CHAVES], palavra, counter, erroSintatico, symbolTable, blockCounter);

    if (lookahead->token == tokenToStr[SEMIC])
    {
      DFUNC(palavra, counter, erroSintatico, symbolTable, blockCounter);
    }
  }
}

// S -> DFUNC | e
int S(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int *blockCounter)
{
  return DFUNC(palavra, counter, erroSintatico, symbolTable, blockCounter);
}

// Função para criar uma nova tabela de símbolos vazia
SymbolTable *createSymbolTable()
{
  SymbolTable *symbolTable = (SymbolTable *)malloc(sizeof(SymbolTable));
  symbolTable->head = NULL;
  return symbolTable;
}

// Função para criar um novo bloco na tabela de símbolos
void createBlock(SymbolTable *symbolTable, int blockIdentifier)
{
  Block *newBlock = (Block *)malloc(sizeof(Block));
  // newBlock->blockIdentifier = blockIdentifier;
  newBlock->blockIdentifier = blockId;
  newBlock->identifiers = NULL;
  newBlock->next = NULL;

  if (symbolTable->head == NULL)
  {
    symbolTable->head = newBlock;
  }
  else
  {
    Block *currentBlock = symbolTable->head;
    while (currentBlock->next != NULL)
    {
      currentBlock = currentBlock->next;
    }
    currentBlock->next = newBlock;
  }
}

// Função para adicionar um identificador a um bloco existente
void addIdentifier(SymbolTable *symbolTable, int blockIdentifier, Token token, Token type, char *lexema, int isFunction)
{
  Block *currentBlock = symbolTable->head;
  printf("addIdentifier -> blockId: %i", blockIdentifier);
  printf("   lexema: %s\n", lexema);

  while (currentBlock != NULL)
  {
    if (currentBlock->blockIdentifier == blockIdentifier) // blockIdentifier
    {
      IdentifierNode *newIdentifier = (IdentifierNode *)malloc(sizeof(IdentifierNode));
      newIdentifier->token = token;
      newIdentifier->type = type;
      newIdentifier->lexema = strdup(lexema);
      newIdentifier->isFunction = isFunction;
      newIdentifier->next = NULL;

      if (currentBlock->identifiers == NULL)
      {
        currentBlock->identifiers = newIdentifier;
      }
      else
      {
        IdentifierNode *currentIdentifier = currentBlock->identifiers;
        while (currentIdentifier->next != NULL)
        {
          currentIdentifier = currentIdentifier->next;
        }
        currentIdentifier->next = newIdentifier;
      }

      break;
    }
    currentBlock = currentBlock->next;
  }
}

// Função para editar um identificador em um bloco existente
void editIdentifier(SymbolTable *symbolTable, int blockIdentifier, Token oldToken, Token newToken, Token newType, char *newLexema)
{
  Block *currentBlock = symbolTable->head;
  while (currentBlock != NULL)
  {
    if (currentBlock->blockIdentifier == blockIdentifier)
    {
      IdentifierNode *currentIdentifier = currentBlock->identifiers;
      while (currentIdentifier != NULL)
      {
        if (currentIdentifier->token == oldToken)
        {
          currentIdentifier->token = newToken;
          currentIdentifier->type = newType;
          free(currentIdentifier->lexema);
          currentIdentifier->lexema = strdup(newLexema);
          break;
        }
        currentIdentifier = currentIdentifier->next;
      }
      break;
    }
    currentBlock = currentBlock->next;
  }
}

// Função para remover um identificador de um bloco existente
void removeIdentifier(SymbolTable *symbolTable, int blockIdentifier, Token token)
{
  Block *currentBlock = symbolTable->head;
  while (currentBlock != NULL)
  {
    if (currentBlock->blockIdentifier == blockIdentifier)
    {
      IdentifierNode *currentIdentifier = currentBlock->identifiers;
      IdentifierNode *previousIdentifier = NULL;

      while (currentIdentifier != NULL)
      {
        if (currentIdentifier->token == token)
        {
          if (previousIdentifier == NULL)
          {
            currentBlock->identifiers = currentIdentifier->next;
          }
          else
          {
            previousIdentifier->next = currentIdentifier->next;
          }
          free(currentIdentifier->lexema);
          free(currentIdentifier);
          break;
        }

        previousIdentifier = currentIdentifier;
        currentIdentifier = currentIdentifier->next;
      }

      break;
    }
    currentBlock = currentBlock->next;
  }
}

IdentifierNode *searchIdentifier(SymbolTable *symbolTable, char *lexema)
{
  Block *currentBlock = symbolTable->head;
  // printf("searchIdentifier -> lexema: %s\n", lexema);
  while (currentBlock != NULL)
  {
    IdentifierNode *currentIdentifier = currentBlock->identifiers;
    while (currentIdentifier != NULL)
    {
      int result = strcmp(lexema, currentIdentifier->lexema);
      if (result == 0)
      {
        return currentIdentifier;
      }
      currentIdentifier = currentIdentifier->next;
    }
    currentBlock = currentBlock->next;
  }
  printf("searchIdentifier -> ERRO SEMÂNTICO\n");
  return NULL; // Valor não encontrado
}

// Função para deletar um bloco da tabela de símbolos
void deleteBlock(SymbolTable *symbolTable, int blockIdentifier)
{
  Block *currentBlock = symbolTable->head;
  Block *previousBlock = NULL;

  while (currentBlock != NULL)
  {
    if (currentBlock->blockIdentifier == blockIdentifier)
    {
      if (previousBlock == NULL)
      {
        symbolTable->head = currentBlock->next;
      }
      else
      {
        previousBlock->next = currentBlock->next;
      }

      // Libera a memória dos identificadores no bloco
      IdentifierNode *currentIdentifier = currentBlock->identifiers;
      while (currentIdentifier != NULL)
      {
        IdentifierNode *nextIdentifier = currentIdentifier->next;
        free(currentIdentifier->lexema);
        free(currentIdentifier);
        currentIdentifier = nextIdentifier;
      }

      free(currentBlock);
      break;
    }

    previousBlock = currentBlock;
    currentBlock = currentBlock->next;
  }
}

// Função para liberar toda a memória da tabela de símbolos
void freeSymbolTable(SymbolTable *symbolTable)
{
  Block *currentBlock = symbolTable->head;
  while (currentBlock != NULL)
  {
    Block *nextBlock = currentBlock->next;

    // Libera a memória dos identificadores no bloco
    IdentifierNode *currentIdentifier = currentBlock->identifiers;
    while (currentIdentifier != NULL)
    {
      IdentifierNode *nextIdentifier = currentIdentifier->next;
      free(currentIdentifier->lexema);
      free(currentIdentifier);
      currentIdentifier = nextIdentifier;
    }

    free(currentBlock);
    currentBlock = nextBlock;
  }

  free(symbolTable);
}

// Função auxiliar para exibir a tabela de símbolos
void printSymbolTable(SymbolTable *symbolTable)
{
  Block *currentBlock = symbolTable->head;
  while (currentBlock != NULL)
  {
    printf("blockId %i:\n", currentBlock->blockIdentifier);
    IdentifierNode *currentIdentifier = currentBlock->identifiers;
    while (currentIdentifier != NULL)
    {
      printf("  Token: %d, Type: %d, Lexema: %s, isFunction: %d\n",
             currentIdentifier->token, currentIdentifier->type, currentIdentifier->lexema,
             currentIdentifier->isFunction);
      currentIdentifier = currentIdentifier->next;
    }
    currentBlock = currentBlock->next;
  }
}

int main()
{
  SymbolTable *symbolTable = createSymbolTable();

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

  int blockCounter = 0;

  createBlock(symbolTable, blockId);

  char *palavra = (char *)malloc(sizeof(char) * (tamanho + 1));
  fread(palavra, sizeof(char), tamanho, entrada);
  palavra[tamanho] = '\0';

  fclose(entrada);

  char erroSintatico[100];

  int counter = 0;
  lookahead = scanner(palavra, &counter, erroSintatico);
  int result = S(palavra, &counter, erroSintatico, symbolTable, &blockCounter);
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

  printSymbolTable(symbolTable);

  freeSymbolTable(symbolTable);

  return 0;
}

Scanner *scanner(char *lexema, int *counter, char *erroSintatico)
{
  Scanner *scan = malloc(sizeof(Scanner));
  char digit = lexema[*counter];
  char *palavra = NULL;

  if (digit == EOF || digit == '\x0')
  {
    scan->token = tokenToStr[END_OF_FILE];
    scan->lexema = "";
    return scan;
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
    adicionarCaractere(&palavra, '_');
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
  scan->token = tokenToStr[_PRINT];
  scan->lexema = "";
  return scan;
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
  scan->token = tokenToStr[_PROC];
  scan->lexema = "";
  return scan;
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
  scan->token = tokenToStr[VOID];
  scan->lexema = "";
  return scan;
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
  scan->token = tokenToStr[WHILE];
  scan->lexema = "";
  return scan;
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
  scan->token = tokenToStr[ELSE];
  scan->lexema = "";
  return scan;
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
  scan->token = tokenToStr[FALSE];
  scan->lexema = "";
  return scan;
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
  scan->token = tokenToStr[TRUE];
  scan->lexema = "";
  return scan;
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
  scan->token = tokenToStr[DO];
  scan->lexema = "";
  return scan;
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
  scan->token = tokenToStr[BOOL];
  scan->lexema = "";
  return scan;
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
  scan->token = tokenToStr[IF];
  scan->lexema = "";
  return scan;
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
  scan->token = tokenToStr[INT];
  scan->lexema = "";
  return scan;
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
  scan->token = tokenToStr[SEMIC];
  scan->lexema = "";
  return scan;
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
  scan->token = tokenToStr[FECHA_CHAVES];
  scan->lexema = "";
  return scan;
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
  scan->token = tokenToStr[ABRE_CHAVES];
  scan->lexema = "";
  return scan;
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
  scan->token = tokenToStr[ABRE_PARENTESIS];
  scan->lexema = "";
  return scan;
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
  scan->token = tokenToStr[FECHA_PARENTESIS];
  scan->lexema = "";
  return scan;
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
  scan->token = tokenToStr[VIRGULA];
  scan->lexema = "";
  return scan;
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
  scan->token = tokenToStr[PONTO_E_VIRGULA];
  scan->lexema = "";
  return scan;
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
  scan->token = tokenToStr[OPR_ADICAO];
  scan->lexema = "";
  return scan;
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
  scan->token = tokenToStr[OPR_SUBTRACAO];
  scan->lexema = "";
  return scan;
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
  scan->token = tokenToStr[OPR_MULTIPLICACAO];
  scan->lexema = "";
  return scan;
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
  scan->token = tokenToStr[OPR_DIVISAO];
  scan->lexema = "";
  return scan;
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
  scan->token = tokenToStr[MENOR_QUE];
  scan->lexema = "";
  return scan;
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
  scan->token = tokenToStr[DIFERENCA];
  scan->lexema = "";
  return scan;
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
  scan->token = tokenToStr[MENOR_OU_IGUAL_QUE];
  scan->lexema = "";
  return scan;
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
  scan->token = tokenToStr[MAIOR_QUE];
  scan->lexema = "";
  return scan;
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
  scan->token = tokenToStr[MAIOR_OU_IGUAL_QUE];
  scan->lexema = "";
  return scan;
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
  scan->token = tokenToStr[ATRIBUICAO];
  scan->lexema = "";
  return scan;
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
  scan->token = tokenToStr[IGUALDADE];
  scan->lexema = "";
  return scan;
q93:
  if (digit == ' ')
  {
    (*counter)++;
    digit = lexema[*counter];
    goto q98;
  }
  if (isalpha(digit))
  {
    adicionarCaractere(&palavra, digit);
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
  scan->token = tokenToStr[IDENTIFICADOR];
  scan->lexema = palavra;
  return scan;
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
  scan->token = tokenToStr[NUM];
  scan->lexema = "";
  return scan;
q101: // Comentario
  scan->token = tokenToStr[_COMENTARIO];
  scan->lexema = "";
  return scan;
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

  scan->token = tokenToStr[ERRO_SINTATICO];
  scan->lexema = "";
  return scan;
}