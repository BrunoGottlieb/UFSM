#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:4996)

void Substring(char* plainText, char* targetText, int start, int end);
void IO(char* plainText, char* key, int rounds);
void F(char* text, char* key);
void Xor(char* output, char* key);
void Shift(char* half, int len);
void KeyGenerator(char* key, char* subKey, char* right, int i);

#define TAM 9

int main()
{
	char plainText[TAM] = { "11001101" }; // dado a ser criptografado
	char key[TAM] = { "11001010" }; // chave
	char subKey[TAM]; // usada para gerar uma nova chave a cada rodada
	int rounds = 4; // numero de rounds

	int answer = 0;
	puts("Choose an option:");
	puts("[1] Use user input");
	puts("[2] Use example input");
	scanf("%d", &answer);

	if (answer == 1)
	{
		IO(plainText, key, &rounds); // coleta os dados do usuario
	}

	int size = strlen(plainText); // tamanho do dado inserido
	int blockSize = size / 2; // tamanho do bloco, dado dividido na metade
	char left[TAM]; // lado esquerdo 
	char right[TAM]; // lado direito

	printf("\n\nText: %s\n", plainText);
	printf("Key: %s\n\n", key);

	Substring(plainText, left, 0, blockSize); // preenche o lado esquerdo
	Substring(plainText, right, blockSize, size); // preenche o lado direito
	strcpy(subKey, key); // passa a chave para a sub-chave
	subKey[size] = '\0';

	for (int i = 0; i < rounds; i++) // laco que executa os rounds
	{
		puts("---------------");
		printf("ROUND %i\n", i + 1);
		puts("---------------");
		printf("Left: %s\n", left);
		printf("Right: %s\n", right);

		char rightCpy[100];
		strcpy(rightCpy, right);

		KeyGenerator(key, subKey, right, i);

		printf("SubKey: %s\n", subKey);

		F(rightCpy, subKey);

		Xor(rightCpy, left);

		strcpy(left, right); // esquerda passa a ser a direita original
		strcpy(right, rightCpy); // direita passa a ser a esquerda (mix)

		printf("Round result: %s%s\n", left, right);
	}

	strcpy(plainText, right);
	strcat(plainText, left);
	printf("\n\nFinal result: %s\n\n", plainText);

}

void F(char* half, char* key) // round function
{
	Xor(half, key); // realiza um xor e retorna para half
	Shift(half, 1);
}

void KeyGenerator(char* key, char* subKey, char* right, int i) // gera uma nova sub-chave a cada rodada
{
	Xor(subKey, right);
	Shift(subKey, 3);
	Xor(subKey, key);
	Shift(subKey, 1);
}

void Shift(char* output, int shiftBy) // deslocamento de bits
{
	int size = strlen(output);
	if (shiftBy > size)
	{
		shiftBy = shiftBy - size;
	}
	if (size == 1)
	{
		return;
	}
	else
	{
		char temp;
		for (int i = 0; i < size - shiftBy; i++)
		{
			temp = output[i];
			output[i] = output[i + shiftBy];
			output[i + shiftBy] = temp;
		}
	}
}

void Xor(char* output, char* key) // Realiza um xor entre dois bits
{
	for (int i = 0; i < strlen(output); i++)
	{
		output[i] = output[i] == key[i] ? '0' : '1';
	}
}

void IO(char* plainText, char* key, int rounds) // coleta os dados do usuario
{
	puts("Insert plainText: [Binary] [Up to 8 bits]");
	scanf("%s", plainText);

	puts("Insert key: [Binary] [At least plainText size]");
	scanf("%s", key);

	if (strlen(key) < strlen(plainText))
	{
		puts("Key size must be at least plainText size. Exiting.\n");
		exit(0);
	}

	puts("Insert number of rounds:");
	scanf("%d", rounds);
}

void Substring(char* plainText, char* targetText, int start, int end) // Divide a funcao entre esquerda e direita
{
	int i = 0; int j = 0;
	for (i = start; i < end; i++)
	{
		targetText[j] = plainText[i]; j++;
	}
	targetText[j] = '\0';
}