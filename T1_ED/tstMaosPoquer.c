/*
********************************************************************************
* Casos de teste simples para o problema das maos de poquer.
* Este codigo tem fins educacionais e pode conter erros. Nao ha garantias de que
* o mesmo funcione sempre como esperado.
* Versao: 0.2 candia@inf.ufsm.br
********************************************************************************
*/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "maosPoquer.h"


int main() {
	int cartas[TAM_DEQUE][2];
	int i = 0, target, pontos, CartasNaMao = 0, valor[2], resp, Disponiveis=49, Total=0;
	char ValidarPilha = 'O', Anulador[8] = "OOOOOOOO";
	bool EndGame = false;

	printf("\nCriando baralho padrao.\n\n");
	criaBaralho(cartas);
	imprimeBaralhoVetor(cartas);
	printf("\n\nEmbaralhando com semente %d.\n\n", SEMENTE);
	embaralha(cartas, SEMENTE);
	imprimeBaralhoVetor(cartas);

	ListOfRows *list = CreateList();
	Pilha *Monte = pilha_cria();
	Mao* mao;
	criar_lista(&mao);

	puts("\n");
	for (int i = 0; i < 7; i++) //Fara as fileiras de cartas iniciais
		CreateRow(list, cartas);

	for (i = 49;i < 52;i++) {         //Insere as 3 cartas do monte
		valor[0] = cartas[i][0];
		valor[1] = cartas[i][1];
		pilha_push(Monte, cartas, valor);
	}

	fila_insere(list, cartas);
	puts("-------------------------------------------------------");
	puts("Como jogar: Insira o numero da fileira na qual desejar retirar uma carta. Digite 8 para retirar a carta da pilha.");
	puts("O jogo possui alguns sinais indicadores ao usuario:");
	puts("O - Opcao disponivel para ser escolhida.");
	puts("X - Opcao indisponivel nesta rodada.");
	puts("T - Opcao indisponivel definitivamente.");
	puts("-------------------------------------------------------");
	while(EndGame == false){
		imprimir(list, Anulador);
		EndGame = Verifica_Fim_de_Jogo(Anulador, CartasNaMao, ValidarPilha);
		while (CartasNaMao < 5 && EndGame == false) {
			Primeiro_da_pilha(Monte, ValidarPilha);
			puts("-------------------------------------------------------");
			printf("Cartas na sua mao: ");
			imprimir_lista(mao);
			puts("-------------------------------------------------------");

			printf("\nEscolha uma fileira para pegar uma carta: ");
			scanf_s("%d", &target);

			if (target < 8 && Anulador[target] != 'X' && Anulador[target] != 'T') {
				Adquirir_Valor(valor, list, target);
				printf("\nO que desejas fazer com a carta %d|%d?\n", valor[0], valor[1]);
				printf("1. Colocar na mao\n");
				printf("2. Colocar na pilha\n");
				printf("3. Voltar\n");
				scanf_s("%d", &resp);
				switch (resp) {
				case 1:
					fila_retira(valor, list, target, Anulador, resp);
					inserirListaOrdem(&mao, valor);
					CartasNaMao++;
					break;
				case 2:
					fila_retira(valor, list, target, Anulador, resp);
					pilha_push(Monte, cartas, valor);
					break;
				}
			}
			else if (target == 8 && !pilha_vazia(Monte) && ValidarPilha == 'O') {
				pilha_pop(valor, Monte);
				ValidarPilha = 'X';
				CartasNaMao++;
				inserirListaOrdem(&mao, valor);
			}
			else {
				puts("\nNumero Invalido!");
			}
			imprimir(list, Anulador);
			EndGame = Verifica_Fim_de_Jogo(Anulador, CartasNaMao, ValidarPilha);
		}
		Primeiro_da_pilha(Monte, ValidarPilha);
		puts("-------------------------------------------------------");
		printf("Cartas na sua mao: ");
		imprimir_lista(mao);
		puts("-------------------------------------------------------");
		printf("Contagem de pontos: ");
		ValidarPilha = 'O';
		for (i = 0;i < 8;i++) {
			if (Anulador[i] == 'X')
				Anulador[i] = 'O';
		}
		if (CartasNaMao == 0)
			pontos = 0;
		else
			pontos = contaPontos(mao);
		Total += pontos;
		CartasNaMao = 0;
		printf("%d pontos | ", pontos);
		Confere_Resultado(pontos);
		printf(" | Total: %d", Total);
		puts("\n-------------------------------------------------------");
		system("pause");
		liberar_lista(&mao);
	}
	puts("---------------------FIM DE JOGO-----------------------");
	system("Pause");
	exit(0);
}
