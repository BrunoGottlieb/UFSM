/*
********************************************************************************
* Header para o problema das maos de poquer.
* Este codigo tem fins educacionais e pode conter erros. Nao ha garantias de que
* o mesmo funcione sempre como esperado.
* Versao: 0.2 candia@inf.ufsm.br
********************************************************************************
*/#ifndef MAOSPOQUER_H

#define TAM_DEQUE	52
#define SEMENTE		2
#define VALOR		0
#define	NAIPE		1
#include <stdbool.h>
typedef enum {OUROS, ESPADAS, COPAS, BASTOS} NAIPES_T;
//##########################PILHA#############################
struct Monte {
	int valor;
	int naipe;
	struct Monte* next;
};
typedef struct Monte Monte;
/* estrutura da pilha */
struct pilha {
	Monte* first; /* aponta para o topo da pilha */
};
typedef struct pilha Pilha;
//########################LISTA################################
/* tipo a ser usado para a fila correspondente a mão do usuário */
typedef struct n {
	int valor;
	int naipe;
	struct n *next;
} Mao;
//#########################FILA################################
typedef struct Card {
	int value;
	int naipe;
	struct Card *next;
}Card;
typedef struct Row {
	Card *firstCard;
	Card *endCard;
	struct Row *nextRow;
}Row;
typedef struct ListOfRows {
	Row *Row;
}ListOfRows;

bool Verifica_Fim_de_Jogo(char Anulador[], int CartasNaMao, char ValidarPilha);
int Carta_Mais_Alta(Mao* mao);

void criar_lista(Mao** l);
void inserirListaOrdem(Mao** mao, int valor[]);
void imprimir_lista(Mao* mao);
void liberar_lista(Mao** mao);

Pilha* pilha_cria(void);
void pilha_push(Pilha* p, int cartas[][2], int valor[]);
void Primeiro_da_pilha(Pilha* p, char v);
int pilha_vazia(Pilha* p);
void pilha_pop(int valor[], Pilha* p);

int fila_vazia(ListOfRows *list);
void fila_insere(ListOfRows *list, int cartas[][2]);
void CreateRow(ListOfRows *list);
ListOfRows *CreateList(void);
void fila_retira(int valor[], ListOfRows *list, int target, char Anulador[], int resp);
void imprimir(ListOfRows *list, char Anulador[]);
void Adquirir_Valor(int valor[], ListOfRows *list, int target);

void Confere_Resultado(int pontos);
void imprimeBaralhoVetor(int cartas[][2]);
void criaBaralho(int cartas[][2]);
void embaralha(int cartas[][2], unsigned int semente);
int contaPontos(Mao *mao);

#define MAOSPOQUER_H
#endif
