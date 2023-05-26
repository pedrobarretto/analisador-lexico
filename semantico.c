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
  int isFunction;
  struct IdentifierNode *next;
} IdentifierNode;

// Estrutura do bloco
typedef struct Block
{
  char blockIdentifier;
  IdentifierNode *identifiers;
  struct Block *next;
} Block;

// Estrutura da tabela de símbolos
typedef struct SymbolTable
{
  Block *head;
} SymbolTable;

char *lookahead; /* Excepcionalmente variavel global */

SymbolTable *createSymbolTable();
void createBlock(SymbolTable *symbolTable, char blockIdentifier);
void addIdentifier(SymbolTable *symbolTable, char blockIdentifier, Token token, int isFunction);
void editIdentifier(SymbolTable *symbolTable, char blockIdentifier, Token oldToken, Token newToken);
void removeIdentifier(SymbolTable *symbolTable, char blockIdentifier, Token token);
IdentifierNode *searchIdentifier(SymbolTable *symbolTable, Token token);
void deleteBlock(SymbolTable *symbolTable, char blockIdentifier);
void freeSymbolTable(SymbolTable *symbolTable, int blockCounter);
void printSymbolTable(SymbolTable *symbolTable, int blockCounter);

// Funções do Análisador Sintático
int match(char *t, char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int blockCounter);
int S(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int blockCounter);
int DFUNC(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int blockCounter);
int COMENTARIO(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int blockCounter);
int PARAMSF(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int blockCounter);
int TIPO(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int blockCounter);
int DVAR(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int blockCounter);
int DVAR_LIST(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int blockCounter);
int BLOCO(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int blockCounter);
int ATRIB(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int blockCounter);
int PROC(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int blockCounter);
int COND(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int blockCounter);
int REPET(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int blockCounter);
int PRINT(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int blockCounter);
int EXPR(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int blockCounter);
int RELACAO(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int blockCounter);
int IDENTIFICADOR_LIST(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int blockCounter);

// Função scanner do análisador léxico
char *scanner(char *lexema, int *counter, char *erroSintatico);

int match(char *t, char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int blockCounter)
{
  if (lookahead == t)
  {
    lookahead = scanner(palavra, counter, erroSintatico);
    if (lookahead == tokenToStr[_COMENTARIO])
    {
      COMENTARIO(palavra, counter, erroSintatico, symbolTable, blockCounter);
    }
    return 1;
  }

  // lookahead = scanner(palavra, counter, erroSintatico);
  return 0;
}

// PARAMSF -> TIPO IDENTIFICADOR | NUMERO | true | false | TIPO IDENTIFICADOR , PARAMSF
int PARAMSF(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int blockCounter)
{
  if (lookahead == tokenToStr[NUM])
  {
    match(tokenToStr[NUM], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead == tokenToStr[TRUE])
  {
    match(tokenToStr[TRUE], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead == tokenToStr[FALSE])
  {
    match(tokenToStr[FALSE], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }

  if (lookahead == tokenToStr[INT])
  {
    match(tokenToStr[INT], palavra, counter, erroSintatico, symbolTable, blockCounter);
    match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead == tokenToStr[BOOL])
  {
    match(tokenToStr[BOOL], palavra, counter, erroSintatico, symbolTable, blockCounter);
    match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }

  if (lookahead == tokenToStr[VIRGULA])
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
int TIPO(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int blockCounter)
{
  if (lookahead == tokenToStr[INT])
  {
    match(tokenToStr[INT], palavra, counter, erroSintatico, symbolTable, blockCounter);
    addIdentifier(symbolTable, blockCounter, INT, 0);
  }
  else if (lookahead == tokenToStr[VOID])
  {
    match(tokenToStr[VOID], palavra, counter, erroSintatico, symbolTable, blockCounter);
    addIdentifier(symbolTable, blockCounter, VOID, 0);
  }
  else if (lookahead == tokenToStr[SEMIC])
  {
    match(tokenToStr[SEMIC], palavra, counter, erroSintatico, symbolTable, blockCounter);
    addIdentifier(symbolTable, blockCounter, SEMIC, 0);
  }
  else if (lookahead == tokenToStr[BOOL])
  {
    match(tokenToStr[BOOL], palavra, counter, erroSintatico, symbolTable, blockCounter);
    addIdentifier(symbolTable, blockCounter, BOOL, 0);
  }
}

// IDENTIFICADOR_LIST -> IDENTIFICADOR | IDENTIFICADOR , IDENTIFICADOR_LIST
int IDENTIFICADOR_LIST(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int blockCounter)
{
  match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico, symbolTable, blockCounter);
  if (lookahead == tokenToStr[VIRGULA])
  {
    match(tokenToStr[VIRGULA], palavra, counter, erroSintatico, symbolTable, blockCounter);
    match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
}

// DVAR -> TIPO IDENTIFICADOR_LIST ;
int DVAR(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int blockCounter)
{
  TIPO(palavra, counter, erroSintatico, symbolTable, blockCounter);
  match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico, symbolTable, blockCounter);

  if (lookahead == tokenToStr[VIRGULA])
  {
    match(tokenToStr[VIRGULA], palavra, counter, erroSintatico, symbolTable, blockCounter);
    IDENTIFICADOR_LIST(palavra, counter, erroSintatico, symbolTable, blockCounter);
  }

  match(tokenToStr[PONTO_E_VIRGULA], palavra, counter, erroSintatico, symbolTable, blockCounter);
}

// DVAR_LIST -> DVAR | DVAR DVAR_LIST | ATRIB | e
int DVAR_LIST(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int blockCounter)
{
  if (lookahead == tokenToStr[INT] || lookahead == tokenToStr[BOOL])
  {
    DVAR(palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead == tokenToStr[IDENTIFICADOR])
  {
    ATRIB(palavra, counter, erroSintatico, symbolTable, blockCounter);
    DVAR_LIST(palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else
  {
    return 0;
  }
}

// COMENTARIO -> /* [a, z] [1, 9]  */
int COMENTARIO(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int blockCounter)
{
  match(tokenToStr[_COMENTARIO], palavra, counter, erroSintatico, symbolTable, blockCounter);
}

// ATRIB -> IDENTIFICADOR = IDENTIFICADOR ; | IDENTIFICADOR = NUMERO ; | IDENTIFICADOR = IDENTIFICADOR + NUMERO ; | IDENTIFICADOR = NUMERO + IDENTIFICADOR ; | IDENTIFICADOR = true ; | IDENTIFICADOR = false ;
int ATRIB(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int blockCounter)
{
  match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico, symbolTable, blockCounter);
  match(tokenToStr[ATRIBUICAO], palavra, counter, erroSintatico, symbolTable, blockCounter);
  // Primeiro operando
  if (lookahead == tokenToStr[NUM])
  {
    match(tokenToStr[NUM], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead == tokenToStr[IDENTIFICADOR])
  {
    match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }

  // Operadores
  if (lookahead == tokenToStr[OPR_ADICAO])
  {
    match(tokenToStr[OPR_ADICAO], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead == tokenToStr[OPR_SUBTRACAO])
  {
    match(tokenToStr[OPR_SUBTRACAO], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead == tokenToStr[OPR_MULTIPLICACAO])
  {
    match(tokenToStr[OPR_MULTIPLICACAO], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead == tokenToStr[OPR_DIVISAO])
  {
    match(tokenToStr[OPR_DIVISAO], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }

  // Segundo operando
  if (lookahead == tokenToStr[NUM])
  {
    match(tokenToStr[NUM], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead == tokenToStr[IDENTIFICADOR])
  {
    match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }

  else if (lookahead == tokenToStr[FALSE])
  {
    match(tokenToStr[FALSE], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead == tokenToStr[TRUE])
  {
    match(tokenToStr[TRUE], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }

  match(tokenToStr[PONTO_E_VIRGULA], palavra, counter, erroSintatico, symbolTable, blockCounter);
}

// PROC -> proc IDENTIFICADOR | “proc” IDENTIFICADOR ( PARAMSF ) ;
int PROC(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int blockCounter)
{
  match(tokenToStr[_PROC], palavra, counter, erroSintatico, symbolTable, blockCounter);
  match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico, symbolTable, blockCounter);
  if (lookahead == tokenToStr[ABRE_PARENTESIS])
  {
    match(tokenToStr[ABRE_PARENTESIS], palavra, counter, erroSintatico, symbolTable, blockCounter);
    PARAMSF(palavra, counter, erroSintatico, symbolTable, blockCounter);
    match(tokenToStr[FECHA_PARENTESIS], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  match(tokenToStr[PONTO_E_VIRGULA], palavra, counter, erroSintatico, symbolTable, blockCounter);
}

// PRINT -> print ( IDENTIFICADOR ) ;
int PRINT(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int blockCounter)
{
  match(tokenToStr[_PRINT], palavra, counter, erroSintatico, symbolTable, blockCounter);
  match(tokenToStr[ABRE_PARENTESIS], palavra, counter, erroSintatico, symbolTable, blockCounter);
  match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico, symbolTable, blockCounter);
  match(tokenToStr[FECHA_PARENTESIS], palavra, counter, erroSintatico, symbolTable, blockCounter);
  match(tokenToStr[PONTO_E_VIRGULA], palavra, counter, erroSintatico, symbolTable, blockCounter);
}

// REPET -> do { BLOCO } while ( EXPR ) ;
int REPET(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int blockCounter)
{
  match(tokenToStr[DO], palavra, counter, erroSintatico, symbolTable, blockCounter);
  match(tokenToStr[ABRE_CHAVES], palavra, counter, erroSintatico, symbolTable, blockCounter);
  BLOCO(palavra, counter, erroSintatico, symbolTable, blockCounter);
  match(tokenToStr[FECHA_CHAVES], palavra, counter, erroSintatico, symbolTable, blockCounter);
  match(tokenToStr[WHILE], palavra, counter, erroSintatico, symbolTable, blockCounter);
  match(tokenToStr[ABRE_PARENTESIS], palavra, counter, erroSintatico, symbolTable, blockCounter);
  EXPR(palavra, counter, erroSintatico, symbolTable, blockCounter);
  match(tokenToStr[FECHA_PARENTESIS], palavra, counter, erroSintatico, symbolTable, blockCounter);
  match(tokenToStr[PONTO_E_VIRGULA], palavra, counter, erroSintatico, symbolTable, blockCounter);
}

// BLOCO -> DVAR_LIST BLOCO | PROC BLOCO | COND BLOCO | PRINT BLOCO | REPET BLOCO | e
int BLOCO(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int blockCounter)
{
  // Lista de váriáveis + atribuição
  if (lookahead == tokenToStr[INT] || lookahead == tokenToStr[BOOL] || lookahead == tokenToStr[IDENTIFICADOR])
  {
    DVAR_LIST(palavra, counter, erroSintatico, symbolTable, blockCounter);
    BLOCO(palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  // Proc
  else if (lookahead == tokenToStr[_PROC])
  {
    PROC(palavra, counter, erroSintatico, symbolTable, blockCounter);
    BLOCO(palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  // If
  else if (lookahead == tokenToStr[IF])
  {
    COND(palavra, counter, erroSintatico, symbolTable, blockCounter);
    BLOCO(palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  // Print
  else if (lookahead == tokenToStr[_PRINT])
  {
    PRINT(palavra, counter, erroSintatico, symbolTable, blockCounter);
    BLOCO(palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  // Do while
  else if (lookahead == tokenToStr[DO])
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
int RELACAO(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int blockCounter)
{
  if (lookahead == tokenToStr[IGUALDADE])
  {
    match(tokenToStr[IGUALDADE], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead == tokenToStr[DIFERENCA])
  {
    match(tokenToStr[DIFERENCA], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead == tokenToStr[MENOR_QUE])
  {
    match(tokenToStr[MENOR_QUE], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead == tokenToStr[MENOR_OU_IGUAL_QUE])
  {
    match(tokenToStr[MENOR_OU_IGUAL_QUE], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead == tokenToStr[MAIOR_QUE])
  {
    match(tokenToStr[MAIOR_QUE], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead == tokenToStr[MAIOR_OU_IGUAL_QUE])
  {
    match(tokenToStr[MAIOR_OU_IGUAL_QUE], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
}

// EXPR -> true | false | IDENTIFICADOR RELACAO IDENTIFICADOR | IDENTIFICADOR RELACAO NUMERO | IDENTIFICADOR RELACAO false | IDENTIFICADOR RELACAO true
int EXPR(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int blockCounter)
{
  if (lookahead == tokenToStr[TRUE])
  {
    match(tokenToStr[TRUE], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead == tokenToStr[FALSE])
  {
    match(tokenToStr[FALSE], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }

  if (lookahead == tokenToStr[IDENTIFICADOR])
  {
    match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico, symbolTable, blockCounter);
    RELACAO(palavra, counter, erroSintatico, symbolTable, blockCounter);
    if (lookahead == tokenToStr[IDENTIFICADOR])
    {
      match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico, symbolTable, blockCounter);
    }
    else if (lookahead == tokenToStr[NUM])
    {
      match(tokenToStr[NUM], palavra, counter, erroSintatico, symbolTable, blockCounter);
    }
    else if (lookahead == tokenToStr[TRUE])
    {
      match(tokenToStr[TRUE], palavra, counter, erroSintatico, symbolTable, blockCounter);
    }
    else if (lookahead == tokenToStr[FALSE])
    {
      match(tokenToStr[FALSE], palavra, counter, erroSintatico, symbolTable, blockCounter);
    }
  }
}

// COND -> if ( EXPR ) { BLOCO } | if ( EXPR ) { BLOCO } else { BLOCO } ;
int COND(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int blockCounter)
{
  match(tokenToStr[IF], palavra, counter, erroSintatico, symbolTable, blockCounter);
  match(tokenToStr[ABRE_PARENTESIS], palavra, counter, erroSintatico, symbolTable, blockCounter);
  EXPR(palavra, counter, erroSintatico, symbolTable, blockCounter);
  match(tokenToStr[FECHA_PARENTESIS], palavra, counter, erroSintatico, symbolTable, blockCounter);
  match(tokenToStr[ABRE_CHAVES], palavra, counter, erroSintatico, symbolTable, blockCounter);
  BLOCO(palavra, counter, erroSintatico, symbolTable, blockCounter);
  match(tokenToStr[FECHA_CHAVES], palavra, counter, erroSintatico, symbolTable, blockCounter);

  if (lookahead == tokenToStr[ELSE])
  {
    match(tokenToStr[ELSE], palavra, counter, erroSintatico, symbolTable, blockCounter);
    match(tokenToStr[ABRE_CHAVES], palavra, counter, erroSintatico, symbolTable, blockCounter);
    BLOCO(palavra, counter, erroSintatico, symbolTable, blockCounter);
    match(tokenToStr[FECHA_CHAVES], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }

  match(tokenToStr[PONTO_E_VIRGULA], palavra, counter, erroSintatico, symbolTable, blockCounter);
}

// DFUNC -> TIPO IDENTIFICADOR ( PARAMSF ) BLOCO | TIPO IDENTIFICADOR ( PARAMSF ) BLOCO DFUNC | e
int DFUNC(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int blockCounter)
{
  if (lookahead == tokenToStr[SEMIC])
  {
    createBlock(symbolTable, blockCounter);
    TIPO(palavra, counter, erroSintatico, symbolTable, blockCounter);
    match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico, symbolTable, blockCounter);
    addIdentifier(symbolTable, blockCounter, IDENTIFICADOR, 0);
    match(tokenToStr[ABRE_CHAVES], palavra, counter, erroSintatico, symbolTable, blockCounter);
    BLOCO(palavra, counter, erroSintatico, symbolTable, blockCounter);
    match(tokenToStr[FECHA_CHAVES], palavra, counter, erroSintatico, symbolTable, blockCounter);
  }
  else if (lookahead == tokenToStr[VOID] || lookahead == tokenToStr[INT] || lookahead == tokenToStr[BOOL])
  {
    TIPO(palavra, counter, erroSintatico, symbolTable, blockCounter);
    match(tokenToStr[IDENTIFICADOR], palavra, counter, erroSintatico, symbolTable, blockCounter);
    match(tokenToStr[ABRE_PARENTESIS], palavra, counter, erroSintatico, symbolTable, blockCounter);
    PARAMSF(palavra, counter, erroSintatico, symbolTable, blockCounter);
    match(tokenToStr[FECHA_PARENTESIS], palavra, counter, erroSintatico, symbolTable, blockCounter);
    match(tokenToStr[ABRE_CHAVES], palavra, counter, erroSintatico, symbolTable, blockCounter);
    BLOCO(palavra, counter, erroSintatico, symbolTable, blockCounter);
    match(tokenToStr[FECHA_CHAVES], palavra, counter, erroSintatico, symbolTable, blockCounter);

    if (lookahead == tokenToStr[SEMIC])
    {
      DFUNC(palavra, counter, erroSintatico, symbolTable, blockCounter);
    }

    // TODO: if lookahead != SEMIC, validar outras funcs.
  }
}

// S -> DFUNC | e
int S(char *palavra, int *counter, char *erroSintatico, SymbolTable *symbolTable, int blockCounter)
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
void createBlock(SymbolTable *symbolTable, char blockIdentifier)
{
  Block *newBlock = (Block *)malloc(sizeof(Block));
  newBlock->blockIdentifier = blockIdentifier;
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
void addIdentifier(SymbolTable *symbolTable, char blockIdentifier, Token token, int isFunction)
{
  Block *currentBlock = symbolTable->head;
  while (currentBlock != NULL)
  {
    if (currentBlock->blockIdentifier == blockIdentifier)
    {
      IdentifierNode *newIdentifier = (IdentifierNode *)malloc(sizeof(IdentifierNode));
      newIdentifier->token = token;
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
void editIdentifier(SymbolTable *symbolTable, char blockIdentifier, Token oldToken, Token newToken)
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
void removeIdentifier(SymbolTable *symbolTable, char blockIdentifier, Token token)
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

IdentifierNode *searchIdentifier(SymbolTable *symbolTable, Token token)
{
  Block *currentBlock = symbolTable->head;
  while (currentBlock != NULL)
  {
    IdentifierNode *currentIdentifier = currentBlock->identifiers;
    while (currentIdentifier != NULL)
    {
      if (currentIdentifier->token == token)
      {
        return currentIdentifier;
      }
      currentIdentifier = currentIdentifier->next;
    }
    currentBlock = currentBlock->next;
  }
  return NULL; // Valor não encontrado
}

// Função para deletar um bloco da tabela de símbolos
void deleteBlock(SymbolTable *symbolTable, char blockIdentifier)
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
void freeSymbolTable(SymbolTable *symbolTable, int blockCounter)
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
      free(currentIdentifier);
      currentIdentifier = nextIdentifier;
    }

    free(currentBlock);
    currentBlock = nextBlock;
  }

  free(symbolTable);
}

// Função auxiliar para exibir a tabela de símbolos
void printSymbolTable(SymbolTable *symbolTable, int blockCounter)
{
  Block *currentBlock = symbolTable->head;
  while (currentBlock != NULL)
  {
    printf("Block %c:\n", currentBlock->blockIdentifier);
    IdentifierNode *currentIdentifier = currentBlock->identifiers;
    while (currentIdentifier != NULL)
    {
      printf("  Token: %d, isFunction: %d\n", currentIdentifier->token, currentIdentifier->isFunction);
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

  char *palavra = (char *)malloc(sizeof(char) * (tamanho + 1));
  fread(palavra, sizeof(char), tamanho, entrada);
  palavra[tamanho] = '\0';

  fclose(entrada);

  char erroSintatico[100];

  int counter = 0;
  lookahead = scanner(palavra, &counter, erroSintatico);
  int result = S(palavra, &counter, erroSintatico, symbolTable, blockCounter);
  // printf("Result: %i\n", result);
  // printf("lookahead: %s \n", lookahead);
  if (lookahead == tokenToStr[ERRO_SINTATICO])
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

  printSymbolTable(symbolTable, blockCounter);

  freeSymbolTable(symbolTable, blockCounter);

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