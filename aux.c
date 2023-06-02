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