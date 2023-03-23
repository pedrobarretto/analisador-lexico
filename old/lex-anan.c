// Pedro Galvão Barretto - TIA: 32016591
// Luan Lemos - TIA: 32085109

#pragma warning(disable : 4996)

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0

/*
 * Tipos de tokens (classificação)
 */
typedef enum
{
	ERRO,					// símbolo inválido
	IDENTIF,			// identificador
	OP_ATRIB,			// <-
	OP_SOMA,			// +
	OP_SUB,				// -
	OP_MULT,			// *
	OP_DIV,				// /
	INTEIRO,			// números inteiros
	REAL,					// números reais
	ABRE_PARENT,	// (
	FECHA_PARENT, // )
	EOS,					// fim de sentença/arquivo
	INTEGER,
	CARACTER,
	FLOAT,
	PONTO_E_VIRGULA,
	PONTO,
	VIRGULA,
	DOIS_PONTOS,
	MAIOR_QUE,
	MENOR_QUE,
	EXCLAMACAO,
	INTERROGACAO,
	IGUAL,		 // =
	IGUALDADE, // ==
	MENOR_OU_IGUAL,
	MAIOR_OU_IGUAL,
	OP_DIFERENCA,
	ABRE_CHAVE,
	FECHA_CHAVE,
	ABRE_COLCHETES,
	FECHA_COLCHETES,
	OP_BEGIN,
	OP_END,
	OP_WRITE,
	OP_READ,
	OP_IF,
	OP_ELSE,
	OP_THEN,
	OP_FOR,
	OP_WHILE,
	OP_REPEAT,
	OP_UNTIL,
	OP_NOT,
	OP_OR,
	OP_AND,
	OP_PRG,
	OP_RETURN,
	OP_SUBROT,
	OP_VAR,
	VOID,
	VARIAVEIS,
	COMENTARIOS,
	PALAVRAS_STRING
} TOKEN_TYPE;

char *tokenToStr[] = {
		"Erro léxico",
		"Identificador",
		"Operador de atribuicao",
		"Operador de adicao",
		"Operador de subtracao",
		"Operador de multiplicacao",
		"Operador de divisao",
		"Numero inteiro",
		"Numero real",
		"Abre parenteses",
		"Fecha parenteses",
		"Fim de conteudo",
		"Tipagem Inteiro",
		"Tipagem Char",
		"Tipagem float",
		"Ponto e Virgula",
		"Ponto",
		"Virgula",
		"Dois Pontos",
		"Maior que",
		"Menor que",
		"Exclamacao",
		"Interrogacao",
		"Sinal de Igual",
		"Igualdade",
		"Menor ou igual",
		"Maior ou igual",
		"Operador de diferenca",
		"Abre chaves",
		"Fecha chaves",
		"Abre colchetes",
		"Fecha colchetes",
		"Operacao begin",
		"Operacao end",
		"Operacao write",
		"Operacao read",
		"Operacao if",
		"Operacao else",
		"Operacao then",
		"Operacao for",
		"Operacao while",
		"Operacao repeat",
		"Operacao until",
		"Operacao not",
		"Operacao or",
		"Operacao and",
		"Operacao prg",
		"Operacao return",
		"Operacao subrot",
		"Operacao var",
		"Tipagem void",
		"Variaveis ou identificadores",
		"Comentarios",
		"Strings"};

/*
 * Registro do token
 */
typedef struct
{
	TOKEN_TYPE tipo;
	int linha;
	char valor[15];
} TOKEN;

/*
 * Variáveis globais
 */
char *buffer;	 // Buffer de memória com o conteúdo lido do arquivo
int linha = 1; // Contador de linhas
short debugMode = FALSE;
TOKEN_TYPE tokenStep = ERRO;

/*
 * Protótipos de funções
 */
TOKEN scanner();

/*
 * Função principal (programa)
 *
 * Implementa um analisador léxico simples para processamento de expressões
 * aritméticas parentizadas que envolvam variáveis e elementos numéricos.
 */
int main(int argc, char *argv[])
{

	if (argc < 2)
	{
		printf("Analisador lexico para expressoes aritmeticas parentizadas envolvendo variaveis e numeros\n");
		printf("Uso:\n");
		printf("  MiniAnaLex.exe <arquivo_fonte> [/d]\n");
		printf("\n");
		printf("<arquivo_fonte> pode ser qualquer arquivo texto cujo conteudo sera\n");
		printf("analisado quanto a linguagem definida para esse exercicio.\n");
		return 0;
	}
	else
	{
		if ((argc == 3) && (strcmp(argv[2], "/d") == 0))
			debugMode = TRUE;

		printf("Iniciando a analise lexica do arquivo: %s\n\n", argv[1]);
	}

	// Abertura do arquivo e carregamento do seu conteúdo para o buffer
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

	TOKEN tk;
	do
	{
		tk = scanner();
		if (tk.tipo != IDENTIF)
		{
			printf("\nLinha:%3d | %-30s | %5s", tk.linha, tokenToStr[tk.tipo], tk.valor);
		}
	} while ((tk.tipo != EOS) && (tk.tipo != ERRO));

	return 0;
}

/*
 * Analisador Léxico de expressões envolvendo variáveis, números e alguns operadores.
 */
TOKEN scanner()
{

	TOKEN tk;
	int contFechaComent = 0;

	tk.tipo = ERRO;
	strcpy(tk.valor, "");
	tk.linha = linha;

	if (tokenStep == OP_PRG)
	{
		while (*buffer != ';')
		{
			strncat(tk.valor, buffer, 1);
			buffer++;
		}
		tk.tipo = VARIAVEIS;
		tk.linha = linha;
		tokenStep = ERRO;
		return tk;
	}

	else if (
			tokenStep == INTEGER ||
			tokenStep == CARACTER ||
			tokenStep == FLOAT)
	{
		tk.tipo = VARIAVEIS;
		tk.linha = linha;
		tokenStep = ERRO;
		return tk;
	}

	// Espaços, tabulações e outros elementos decorativos (comentários)
	while ((*buffer == ' ') || (*buffer == '\t') || (*buffer == '\r') || (*buffer == '\n'))
	{
		if (*buffer == '\n')
			linha++;
		buffer++;
	}
	// Operadores: soma, incremento e somaAtrib
	if (*buffer == '+')
	{
		if (debugMode)
			strncat(tk.valor, buffer, 1);
		buffer++;
		tk.tipo = OP_SOMA;
	}
	// Operador de subtração
	else if (*buffer == '-')
	{
		if (debugMode)
			strncat(tk.valor, buffer, 1);
		buffer++;
		tk.tipo = OP_SUB;
	}
	// Operador de multiplicação
	else if (*buffer == '*')
	{
		if (debugMode)
			strncat(tk.valor, buffer, 1);
		buffer++;
		tk.tipo = OP_MULT;
	}
	// Operador de multiplicação
	else if (*buffer == '/')
	{
		if (debugMode)
			strncat(tk.valor, buffer, 1);
		buffer++;
		tk.tipo = OP_DIV;
	}
	// Parenteses
	else if (*buffer == '(')
	{
		if (debugMode)
			strncat(tk.valor, buffer, 1);
		tk.tipo = ABRE_PARENT;
		buffer++;
	}
	else if (*buffer == ')')
	{
		if (debugMode)
			strncat(tk.valor, buffer, 1);
		tk.tipo = FECHA_PARENT;
		buffer++;
	}
	else if (*buffer == '{')
	{
		if (debugMode)
			strncat(tk.valor, buffer, 1);
		tk.tipo = ABRE_CHAVE;
		buffer++;
	}
	else if (*buffer == '}')
	{
		if (debugMode)
			strncat(tk.valor, buffer, 1);
		tk.tipo = FECHA_CHAVE;
		buffer++;
	}
	else if (*buffer == '[')
	{
		if (debugMode)
			strncat(tk.valor, buffer, 1);
		tk.tipo = ABRE_COLCHETES;
		buffer++;
	}
	else if (*buffer == ']')
	{
		if (debugMode)
			strncat(tk.valor, buffer, 1);
		tk.tipo = FECHA_COLCHETES;
		buffer++;
	}
	else if (*buffer == '"')
	{
		if (debugMode)
			strncat(tk.valor, buffer, 1);
		buffer++;
		while (*buffer != '"')
		{
			buffer++;
		}
		if (*buffer == '"')
		{
			tk.tipo = PALAVRAS_STRING;
			buffer++;
		}
	}
	// Pontuacao
	else if (*buffer == ':')
	{
		if (debugMode)
			strncat(tk.valor, buffer, 1);
		tk.tipo = DOIS_PONTOS;
		buffer++;
	}
	else if (*buffer == ';')
	{
		if (debugMode)
			strncat(tk.valor, buffer, 1);
		tk.tipo = PONTO_E_VIRGULA;
		buffer++;
	}
	else if (*buffer == '!')
	{
		if (debugMode)
			strncat(tk.valor, buffer, 1);
		tk.tipo = EXCLAMACAO;
		buffer++;
		if (*buffer == '=')
		{
			if (debugMode)
				strncat(tk.valor, buffer, 1);
			buffer++;
			tk.tipo = OP_DIFERENCA;
		}
	}
	else if (*buffer == '?')
	{
		if (debugMode)
			strncat(tk.valor, buffer, 1);
		tk.tipo = INTERROGACAO;
		buffer++;
	}
	else if (*buffer == ',')
	{
		if (debugMode)
			strncat(tk.valor, buffer, 1);
		tk.tipo = VIRGULA;
		buffer++;
	}
	else if (*buffer == '.')
	{
		if (debugMode)
			strncat(tk.valor, buffer, 1);
		tk.tipo = PONTO;
		buffer++;
	}
	else if (*buffer == '>')
	{
		if (debugMode)
			strncat(tk.valor, buffer, 1);
		tk.tipo = MAIOR_QUE;
		buffer++;
	}
	else if (*buffer == '<')
	{
		if (debugMode)
			strncat(tk.valor, buffer, 1);
		buffer++;
		tk.tipo = MENOR_QUE;
		if (*buffer == '=')
		{
			if (debugMode)
				strncat(tk.valor, buffer, 1);
			buffer++;
			tk.tipo = MENOR_OU_IGUAL;
		}
		else if (*buffer == '-')
		{
			if (debugMode)
				strncat(tk.valor, buffer, 1);
			buffer++;
			tk.tipo = OP_ATRIB;
		}
	}
	else if (*buffer == '=')
	{
		if (debugMode)
			strncat(tk.valor, buffer, 1);
		tk.tipo = IGUAL;
		buffer++;
		if (*buffer == '=')
		{
			if (debugMode)
				strncat(tk.valor, buffer, 1);
			buffer++;
			tk.tipo = IGUALDADE;
		}
	}
	// Números
	else if (isdigit(*buffer))
	{
		while ((*buffer >= '0') && (*buffer <= '9'))
		{
			strncat(tk.valor, buffer, 1);
			buffer++;
		}
		tk.tipo = INTEIRO;
		if (*buffer == '.')
		{
			strncat(tk.valor, buffer, 1);
			buffer++;
			if ((*buffer >= '0') && (*buffer <= '9'))
				while ((*buffer >= '0') && (*buffer <= '9'))
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
				}
			else
				strcat(tk.valor, "0");
			tk.tipo = REAL;
		}
	}
	// Identificadores
	else if (isalpha(*buffer))
	{
		while (isalpha(*buffer) || isdigit(*buffer) || (*buffer == '_'))
		{
			if (*buffer == 'i') // int e if
			{
				strncat(tk.valor, buffer, 1);
				buffer++;
				if (*buffer == 'n')
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					if (*buffer == 't')
					{
						strncat(tk.valor, buffer, 1);
						buffer++;
						tk.tipo = INTEGER;
						tokenStep = INTEGER;
						break;
					}
				}
				else if (*buffer == 'f')
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					tk.tipo = OP_IF;
					break;
				}
				else
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					tk.tipo = IDENTIF;
					break;
				}
			}
			else if (*buffer == 'f') // float e for
			{
				strncat(tk.valor, buffer, 1);
				buffer++;
				if (*buffer == 'l')
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					if (*buffer == 'o')
					{
						strncat(tk.valor, buffer, 1);
						buffer++;
						if (*buffer == 'a')
						{
							strncat(tk.valor, buffer, 1);
							buffer++;
							if (*buffer == 't')
							{
								strncat(tk.valor, buffer, 1);
								buffer++;
								tk.tipo = FLOAT;
								tokenStep = FLOAT;
								break;
							}
						}
					}
				}
				else if (*buffer == 'o')
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					if (*buffer == 'r')
					{
						strncat(tk.valor, buffer, 1);
						buffer++;
						if (*buffer == '(')
						{
							strncat(tk.valor, buffer, 1);
							buffer++;
							tk.tipo = OP_FOR;
							break;
						}
					}
				}
				else
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					tk.tipo = IDENTIF;
					break;
				}
			}
			else if (*buffer == 'c') // char
			{
				strncat(tk.valor, buffer, 1);
				buffer++;
				if (*buffer == 'h')
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					if (*buffer == 'a')
					{
						strncat(tk.valor, buffer, 1);
						buffer++;
						if (*buffer == 'r')
						{
							strncat(tk.valor, buffer, 1);
							buffer++;
							tk.tipo = CARACTER;
							tokenStep = CARACTER;
							break;
						}
					}
				}
				else
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					tk.tipo = IDENTIF;
					break;
				}
			}
			else if (*buffer == 'b') // begin
			{
				strncat(tk.valor, buffer, 1);
				buffer++;
				if (*buffer == 'e')
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					if (*buffer == 'g')
					{
						strncat(tk.valor, buffer, 1);
						buffer++;
						if (*buffer == 'i')
						{
							strncat(tk.valor, buffer, 1);
							buffer++;
							if (*buffer == 'n')
							{
								strncat(tk.valor, buffer, 1);
								buffer++;
								tk.tipo = OP_BEGIN;
								break;
							}
						}
					}
				}
				else
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					tk.tipo = IDENTIF;
					break;
				}
			}
			else if (*buffer == 'r') // read, repeat e return
			{
				strncat(tk.valor, buffer, 1);
				buffer++;
				if (*buffer == 'e')
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					if (*buffer == 'a')
					{
						strncat(tk.valor, buffer, 1);
						buffer++;
						if (*buffer == 'd')
						{
							strncat(tk.valor, buffer, 1);
							buffer++;
							if (*buffer == '(')
							{
								tk.tipo = OP_READ;
								break;
							}
						}
					}
				}
				else if (*buffer == 'p')
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					if (*buffer == 'e')
					{
						strncat(tk.valor, buffer, 1);
						buffer++;
						if (*buffer == 'a')
						{
							strncat(tk.valor, buffer, 1);
							buffer++;
							if (*buffer == 't')
							{
								strncat(tk.valor, buffer, 1);
								buffer++;
								tk.tipo = OP_REPEAT;
								break;
							}
						}
					}
				}
				else if (*buffer == 't')
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					if (*buffer == 'u')
					{
						strncat(tk.valor, buffer, 1);
						buffer++;
						if (*buffer == 'r')
						{
							strncat(tk.valor, buffer, 1);
							buffer++;
							if (*buffer == 'n')
							{
								strncat(tk.valor, buffer, 1);
								buffer++;
								tk.tipo = OP_RETURN;
								break;
							}
						}
					}
				}
				else
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					tk.tipo = IDENTIF;
					break;
				}
			}
			else if (*buffer == 'a') // and
			{
				strncat(tk.valor, buffer, 1);
				buffer++;
				if (*buffer == 'n')
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					if (*buffer == 'd')
					{
						strncat(tk.valor, buffer, 1);
						buffer++;
						if (*buffer == '(')
						{
							strncat(tk.valor, buffer, 1);
							buffer++;
							tk.tipo = OP_AND;
							break;
						}
					}
				}
				else
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					tk.tipo = IDENTIF;
					break;
				}
			}
			else if (*buffer == 'e') // else e end
			{
				strncat(tk.valor, buffer, 1);
				buffer++;
				if (*buffer == 'l')
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					if (*buffer == 's')
					{
						strncat(tk.valor, buffer, 1);
						buffer++;
						if (*buffer == 'e')
						{
							strncat(tk.valor, buffer, 1);
							buffer++;
							tk.tipo = OP_ELSE;
							break;
						}
					}
				}
				else if (*buffer == 'n')
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					if (*buffer == 'd')
					{
						strncat(tk.valor, buffer, 1);
						buffer++;
						tk.tipo = OP_END;
						break;
					}
				}
				else
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					tk.tipo = IDENTIF;
					break;
				}
			}
			else if (*buffer == 'n') // not
			{
				strncat(tk.valor, buffer, 1);
				buffer++;
				if (*buffer == 'o')
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					if (*buffer == 't')
					{
						strncat(tk.valor, buffer, 1);
						buffer++;
						tk.tipo = OP_NOT;
						break;
					}
				}
				else
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					tk.tipo = IDENTIF;
					break;
				}
			}
			else if (*buffer == 'o') // or
			{
				strncat(tk.valor, buffer, 1);
				buffer++;
				if (*buffer == 'r')
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					if (*buffer == '(')
					{
						strncat(tk.valor, buffer, 1);
						buffer++;
						tk.tipo = OP_OR;
						break;
					}
				}
				else
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					tk.tipo = IDENTIF;
					break;
				}
			}
			else if (*buffer == 'p') // prg
			{
				strncat(tk.valor, buffer, 1);
				buffer++;
				if (*buffer == 'r')
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					if (*buffer == 'g')
					{
						strncat(tk.valor, buffer, 1);
						buffer++;
						tk.tipo = OP_PRG;
						tokenStep = OP_PRG;
						break;
					}
				}
				else
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					tk.tipo = IDENTIF;
					break;
				}
			}
			else if (*buffer == 's') // subrot
			{
				strncat(tk.valor, buffer, 1);
				buffer++;
				if (*buffer == 'u')
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					if (*buffer == 'b')
					{
						strncat(tk.valor, buffer, 1);
						buffer++;
						if (*buffer == 'r')
						{
							strncat(tk.valor, buffer, 1);
							buffer++;
							if (*buffer == 'o')
							{
								strncat(tk.valor, buffer, 1);
								buffer++;
								if (*buffer == 't')
								{
									strncat(tk.valor, buffer, 1);
									buffer++;
									tk.tipo = OP_READ;
									break;
								}
							}
						}
					}
				}
				else
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					tk.tipo = IDENTIF;
					break;
				}
			}
			else if (*buffer == 't') // then
			{
				strncat(tk.valor, buffer, 1);
				buffer++;
				if (*buffer == 'h')
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					if (*buffer == 'e')
					{
						strncat(tk.valor, buffer, 1);
						buffer++;
						if (*buffer == 'n')
						{
							strncat(tk.valor, buffer, 1);
							buffer++;
							tk.tipo = OP_THEN;
							break;
						}
					}
				}
				else
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					tk.tipo = IDENTIF;
					break;
				}
			}
			else if (*buffer == 'u') // until
			{
				strncat(tk.valor, buffer, 1);
				buffer++;
				if (*buffer == 'n')
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					if (*buffer == 't')
					{
						strncat(tk.valor, buffer, 1);
						buffer++;
						if (*buffer == 'i')
						{
							strncat(tk.valor, buffer, 1);
							buffer++;
							if (*buffer == 'l')
							{
								strncat(tk.valor, buffer, 1);
								buffer++;
								tk.tipo = OP_REPEAT;
								break;
							}
						}
					}
				}
				else
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					tk.tipo = IDENTIF;
					break;
				}
			}
			else if (*buffer == 'v') // var e void
			{
				strncat(tk.valor, buffer, 1);
				buffer++;
				if (*buffer == 'a')
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					if (*buffer == 'r')
					{
						strncat(tk.valor, buffer, 1);
						buffer++;
						tk.tipo = OP_VAR;
						break;
					}
				}
				else if (*buffer == 'o')
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					if (*buffer == 'i')
					{
						strncat(tk.valor, buffer, 1);
						buffer++;
						if (*buffer == 'd')
						{
							strncat(tk.valor, buffer, 1);
							buffer++;
							tk.tipo = VOID;
							break;
						}
					}
				}
				else
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					tk.tipo = IDENTIF;
					break;
				}
			}
			else if (*buffer == 'w') // while e write
			{
				strncat(tk.valor, buffer, 1);
				buffer++;
				if (*buffer == 'h')
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					if (*buffer == 'i')
					{
						strncat(tk.valor, buffer, 1);
						buffer++;
						if (*buffer == 'l')
						{
							strncat(tk.valor, buffer, 1);
							buffer++;
							if (*buffer == 'e')
							{
								strncat(tk.valor, buffer, 1);
								buffer++;
								tk.tipo = OP_WHILE;
								break;
							}
						}
					}
				}
				else if (*buffer == 'r')
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					if (*buffer == 'i')
					{
						strncat(tk.valor, buffer, 1);
						buffer++;
						if (*buffer == 't')
						{
							strncat(tk.valor, buffer, 1);
							buffer++;
							if (*buffer == 'e')
							{
								strncat(tk.valor, buffer, 1);
								buffer++;
								if (*buffer == '(')
								{
									tk.tipo = OP_WRITE;
									break;
								}
							}
						}
					}
				}
				else
				{
					strncat(tk.valor, buffer, 1);
					buffer++;
					tk.tipo = IDENTIF;
					break;
				}
			}
			else // identificador
			{
				tk.tipo = IDENTIF;
			}
			strncat(tk.valor, buffer, 1);
			buffer++;
		}
	}
	else if ((*buffer == EOF) || (*buffer == '\x0'))
	{
		if (debugMode)
			strncat(tk.valor, "EOF", 1);
		tk.tipo = EOS;
	}
	else
	{
		strncat(tk.valor, buffer, 1);
		tk.tipo = ERRO;
	}

	tk.linha = linha;
	return tk;
}