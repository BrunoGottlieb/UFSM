/*
********************************************************************************
* Algumas funcoes uteis para o problema das maos de poquer.
* Este codigo tem fins educacionais e pode conter erros. Nao ha garantias de que
* o mesmo funcione sempre como esperado.
* Versao: 0.2 candia@inf.ufsm.br
********************************************************************************
*/
#include <stdio.h>
#include <stdlib.h>
#include "maosPoquer.h"
#include <stdbool.h>

bool Verifica_Fim_de_Jogo(char Anulador[], int CartasNaMao, char ValidarPilha) {
	int i, cont = 0;
	//puts("");
	for (i = 1;i < 8;i++) {
		//printf("Anulador %d: %c\n", i, Anulador[i]);
		if (Anulador[i] == 'O')
			cont++;
	}
	//printf("Validar Pilha: %c\n", ValidarPilha);
	if (ValidarPilha == 'O')
		cont++;
	//printf("CONT: %d\n", cont);
	//printf("Total mao e cont: %d\n", cont + CartasNaMao);
	if (cont + CartasNaMao >= 5)
		return false;
	else return true;
}

void criar_lista(Mao** l) {
	*l = NULL;
}

void inserirListaOrdem(Mao** mao, int valor[]){
	Mao* novo;
	Mao* ant = NULL; /* ponteiro para elemento anterior */
	Mao* p = *mao; /* ponteiro para percorrer a lista */
	/* procura posição de inserção */
	while (p != NULL && p->valor < valor[0])
	{
		ant = p; p = p->next;
	}
	/* cria novo elemento */
	novo = (Mao*)malloc(sizeof(Mao));
	novo->valor = valor[0];
	novo->naipe = valor[1];
	/* encadeia elemento */
	if (ant == NULL)
	{ /* insere elemento no início */
		novo->next = *mao; *mao = novo;
	}
	else { /* insere elemento no meio da lista */
		novo->next = ant->next;
		ant->next = novo;
	}
	return;
}

void imprimir_lista(Mao* mao) {
	Mao* p;
	int aux = 0, i=0;
	for (p = mao; p != NULL; p = p->next) {
		aux++; i++;
		printf("%d|%d ", p->valor, p->naipe);
	}
	if (aux == 0)
		puts("Nao ha cartas na mao");
	else
		printf("\n");
}

int Carta_Mais_Alta(Mao* mao) {
	Mao* p;
	int aux=0, carta;
	for (p = mao; p != NULL; p = p->next) {
		carta = p->valor;
		if (carta == 1) //Converte A em 14
			carta = 14;
		if (carta > aux) {
			aux= carta;
		}
	}
	return aux; //Retorna a carta mais alta
}

void liberar_lista(Mao** mao) {
	Mao* p = *mao;
	while (p != NULL) {
		Mao* t = p->next; // guarda referência p/ próx. elemento 
		free(p); // libera a memória apontada por p 
		p = t; // faz p apontar para o próximo 
	}
	*mao = NULL;
}

//######################################PILHAS###################################
Pilha* pilha_cria(void)
{
	Pilha* p = (Pilha*)malloc(sizeof(Pilha));
	p->first = NULL;
	return p;
}

void pilha_push(Pilha* p, int cartas[][2], int valor[])
{
	Monte* n = (Monte*)malloc(sizeof(Monte));
	n->valor = valor[0];
	n->naipe = valor[1];
	n->next = p->first;
	p->first = n;
}

void Primeiro_da_pilha(Pilha* p, char v) {
	if (pilha_vazia(p)) {
		printf("\n\n %c | Pilha vazia.\n\n", v);
		return;
	}
	int aux[2];
	aux[0] = p->first->valor;
	aux[1] = p->first->naipe;
	printf("\n\n %c | Topo da pilha: %d|%d\n\n", v, aux[0],aux[1]);
	return;
}

void pilha_pop(int valor[], Pilha* p)
{
	Monte* t;
	if (pilha_vazia(p)) {
		printf("\n\nPilha vazia.\n\n");
		return;
	}
	t = p->first;
	valor[0] = t->valor;
	valor[1] = t->naipe;
	p->first = t->next;
	free(t);
	return;
}

int pilha_vazia(Pilha* p) {
	return(p->first == NULL);
}
//###################################FILEIRAS############################################

ListOfRows *CreateList(void) {
	ListOfRows* list = (Row*)malloc(sizeof(ListOfRows));
	list->Row = NULL;
	return list;
}

void CreateRow(ListOfRows *list) {
	Row* row = (Row*)malloc(sizeof(Row));
	row->firstCard = NULL;
	row->endCard = NULL;
	row->nextRow = NULL;
	Row* tmp = list->Row;
	int index = 2;
	if (tmp != NULL) {
		while (tmp->nextRow != NULL) {
			tmp = tmp->nextRow;
			index++;
		}
		tmp->nextRow = row;
	}
	else {
		list->Row = row;
		index = 1;
	}
	//printf("Row %i criada com sucesso.\n", index);
	return;
}

void imprimir(ListOfRows *list, char Anulador[]) {
	int i, j = 0, cont = 0, Inversor[8][2];
	Row* tmp = list->Row;
	Card *aux = list->Row->firstCard;
	while(tmp->nextRow != NULL){
		for (i = 0; i < 7; i++) {
			cont++;
			printf("\n\n %c |Fileira %d: ", Anulador[i+1], i+1);
			while (aux != NULL) {
				//printf(" %d|%d ", aux->value, aux->naipe);
				Inversor[j][0] = aux->value;
				Inversor[j][1] = aux->naipe;
				aux = aux->next;
				j++;
			}
			Inversor[j][0] = '\0';
			for (j = 0;Inversor[j][0] != '\0';j++);
			j--;
			tmp = tmp->nextRow;
			while(j>=0){
				printf(" %d|%d ", Inversor[j][0], Inversor[j][1]);
				j--;
			}
			j = 0;
			if (cont < 7)
				aux = tmp->firstCard;
			else 
				return;
		}
	}
}

void fila_insere(ListOfRows *list, int cartas[][2])
{
	int a = 0, b = 7;
	Row *aux= list->Row;
	//puts("");
	for (int j = 0; j < 7; j++) {
		//printf("\nFileira %d: ", j+1);
		for (int i = a; i < b; i++) {
			Card *carta = (Card*)malloc(sizeof(Card));
			carta->value = cartas[i][0];
			carta->naipe = cartas[i][1];
			carta->next = NULL;
			if (aux->endCard != NULL)
				aux->endCard->next = carta;
			else
				aux->firstCard = carta;
			aux->endCard = carta;
			carta = carta->next;
			//printf(" %d|%d ", cartas[i][0], cartas[i][1]);
		}
		aux = aux->nextRow;
		a = b;
		b += 7;
	}
}

void Adquirir_Valor(int valor[], ListOfRows *list, int target) {
	int i;
	Row* tmp = list->Row;
	for (i = 0;i < target - 1;i++)
		tmp = tmp->nextRow;
	Card* t = tmp->firstCard;
	valor[0] = t->value;
	valor[1] = t->naipe;
	return;
}

void fila_retira(int valor[], ListOfRows *list, int target, char Anulador[], int resp) {
	int i;
	Row* tmp = list->Row;
	if (tmp == NULL) {
		printf("\nNao ha cartas nesta fileira.\n");
		return;
	}
	for (i = 0;i < target-1;i++)
		tmp = tmp->nextRow;
	if(resp==1)
		Anulador[i + 1] = 'X';
	Card* t = tmp->firstCard;
	valor[0] = t->value;
	valor[1] = t->naipe;
	tmp->firstCard = t->next;
	if (tmp->firstCard == NULL) {
		tmp->endCard = NULL;
		Anulador[i + 1] = 'T';
		//puts("\nFILEIRA ANULADA TOTAL\n");
	}
	free(t);
	return;
}

int fila_vazia(ListOfRows *list)
{
	return(list->Row->firstCard == NULL);
}

//###########################################BARALHOS###############################################

void imprimeBaralhoVetor(int cartas[][2]){
	int i;
	for(i=0; i<TAM_DEQUE; i++)
		printf("[%i %i]", cartas[i][0], cartas[i][1]);
	return;
}

/* Cria um baralho padrao de 52 cartas em um vetor passado por referencia */
void criaBaralho(int cartas[][2]) {
	long int i, j = 0;
	NAIPES_T n;
	/* geração do baralho ordenado */
	for (n = OUROS; n <= BASTOS; n++) {
		for (i = 1; i <= 13; i++) {
			cartas[j][VALOR] = i;
			cartas[j][NAIPE] = n;
			j++;
		}
	}
	return;
}
// Embaralha as 52 cartas constantes em um vetor passado por referencia 
void embaralha(int cartas[][2], unsigned int semente) {
	int i, j, aux_v, aux_n;
	// EMBARALHAMENTO 
	srand(semente);
	for (i = 0; i < TAM_DEQUE; i++) {
		j = (unsigned long int) rand() % TAM_DEQUE;
		aux_v = cartas[i][VALOR];
		aux_n = cartas[i][NAIPE];
		cartas[i][VALOR] = cartas[j][VALOR];
		cartas[i][NAIPE] = cartas[j][NAIPE];
		cartas[j][VALOR] = aux_v;
		cartas[j][NAIPE] = aux_n;
	}
	return;
}
/* Conta os pontos de uma mao de 5 cartas, de acordo com as regras de pontuacao. */
/* A mao deve vir ordenada, por valor da carta, em ordem ascendente. */
/* (IMPORTANTE: para fins desta ordenacao, o As vale 1) */
void Confere_Resultado(int pontos) {
	if (pontos == 25)
		printf("Pair");
	else if (pontos == 50)
		printf("Two Pair");
	else if (pontos == 100)
		printf("Three of a kind");
	else if (pontos == 150)
		printf("Straight");
	else if (pontos == 200)
		printf("Flush");
	else if (pontos == 300)
		printf("Full House");
	else if (pontos == 500)
		printf("Four of a kind");
	else if (pontos == 750)
		printf("Straight Flush");
	else if (pontos == 1000)
		printf("Royal Straight Flush");
	else
		printf("Higher Card");
}
int contaPontos(Mao *mao){
	Mao *ptrAux;
	int cont=0, flush=0, straight=0, royalStraight=0;
	int cont2=0;

	/* verifica Flush */
	for(ptrAux = mao->next; ptrAux != NULL; ptrAux = ptrAux->next){
		if(ptrAux->naipe == mao->naipe)
			cont ++;
	}
	if(cont == 4)
		flush = 1; /* é um flush */

	/* verifica Royal Straight */
	ptrAux = mao;
	if(ptrAux->valor == 1 && ptrAux->next->valor == 10){
		/* somente pode ser royal se a 1a carta é ás e 2a é 10 */
		for(ptrAux = mao->next, cont=0; ptrAux->next != NULL; ptrAux = ptrAux->next){
			if(ptrAux->valor == (ptrAux->next->valor - 1))
				cont ++;
		}
		if(cont == 3)
			royalStraight = 1; /* é um royal straight */
	}

	if(flush && royalStraight) /* mão é um royal straight flush */
		return 1000;

	/* verifica Straight simples */
	for(ptrAux = mao, cont=0; ptrAux->next != NULL; ptrAux = ptrAux->next){
		if(ptrAux->valor == (ptrAux->next->valor - 1))
			cont ++;
	}
	if(cont == 4)
		straight = 1; /* é um straight simples */

	if(flush && straight) /* mão é um straight flush */
		return 750;

	if(royalStraight || straight) /* mão é um straight */
		return 150;

	if(flush) /* mão é flush */
		return 200;

	/* verifica pares, trincas e quadras */
	cont = cont2 = 0;
	ptrAux = mao;
	while((ptrAux->next != NULL) && (ptrAux->valor != ptrAux->next->valor))
		ptrAux = ptrAux->next;
	while((ptrAux->next != NULL) && (ptrAux->valor == ptrAux->next->valor)){
		cont++;
		ptrAux = ptrAux->next;
	}
	while ((ptrAux->next != NULL) && (ptrAux->valor != ptrAux->next->valor))
		ptrAux = ptrAux->next;
	while((ptrAux->next != NULL) && (ptrAux->valor == ptrAux->next->valor)){
		cont2++;
		ptrAux = ptrAux->next;
	}
	/* se foi encontrado algum par, trinca, four ou full house, é necessário */
	/* fazer cont e cont2 conter numero correto de cartas iguais encontradas */
	if(cont > 0) /* normalizando valor do primeiro contador */
		cont++;
	if(cont2 > 0) /* normalizando valor do segundo contador */
		cont2++;
	/* após normalizaçao, cont e cont2 podem ser usados para verificacao */
	switch(cont+cont2){
		case 5: /* mão é um full house */
			return 300;
		case 4: /* four ou dois pares */
			if(cont == 4 || cont2 == 4)
				return 500;
			else
				return 50;
		case 3: /* trinca */
			return 100;
		case 2: /* par simples */
			return 25;
	}
	cont = Carta_Mais_Alta(mao);
	return cont;
}
