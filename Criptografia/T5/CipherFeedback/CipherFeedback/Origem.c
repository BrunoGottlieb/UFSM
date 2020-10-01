#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#pragma warning(disable:4996)

void Xor(char* output, char* key);
void Encryption(char* output, char* key);
void Shift(char* output, int shiftBy);
void IO(char* plainText, char* key, char* initializationVector, int rounds);
void CipherFeedback(char* plainText, char* cipherText, char* key, int rounds);

#define TAM 9

int main()
{
	char plainText[TAM] = { "10010101" }; // dado a ser criptografado
	char initializationVector[TAM] = { "11011101" }; // vetor pseudo-aleatorio
	char key[TAM] = { "11011000" }; // chave
	char cipherText[TAM]; // usado para propagar o resultado a cada rodada
	char plainTextCpy[TAM]; // guarda o plainText original
	int rounds = 4; // numero de rounds

	int answer = 0;
	puts("Choose an option:");
	puts("[1] Use user input");
	puts("[2] Use example input");
	scanf("%d", &answer);

	if (answer == 1)
	{
		IO(plainText, key, initializationVector , &rounds); // coleta os dados do usuario
	}
	else
	{
		printf("\nPlainText: %s\n", plainText);
		printf("Key: %s\n", key);
		printf("InitializationVector: %s\n", initializationVector);
		printf("Rounds: %i\n", rounds);
	}

	strcpy(plainTextCpy, plainText); // guarda uma copia do plainText original
	strcpy(cipherText, initializationVector); // passa o vetor pseudo-aleatorio para a array cipherText
	CipherFeedback(plainText, cipherText, key, rounds); // realiza a criptografia

	printf("\n\nFinal result: %s\n\n", cipherText);

	puts("Would you like to decrypt now? [Y/N]");
	char ch = 'n';
	scanf(" %c", &ch);
	if (ch == 'Y' || ch == 'y' || ch == '1')
	{
		strcpy(plainText, cipherText); // passa o resultado para o plainText
		strcpy(cipherText, plainTextCpy); // passa o input original para o cipherText

		printf("\nPlainText: %s\n", plainText);
		printf("Key: %s\n", key);
		printf("InitializationVector: %s\n", cipherText);
		printf("Rounds: %i\n", rounds);

		CipherFeedback(plainText, cipherText, key, rounds);
		printf("\n\nFinal result: %s\n\n", cipherText);
	}
}

void CipherFeedback(char* plainText, char* cipherText, char* key, int rounds)
{
	for (int i = 0; i < rounds; i++) // laco que executa os rounds
	{
		Encryption(cipherText, key);
		Xor(cipherText, plainText);
	}
}

void Xor(char* output, char* key) // funcao xor entre dois bits
{
	for (int i = 0; i < strlen(output); i++)
	{
		output[i] = output[i] == key[i] ? '0' : '1';
	}
}

void Encryption(char* output, char* key) // block cipher encryption simulator
{
	Xor(output, key);
	Shift(output, 4);
	Xor(output, key);
	Shift(output, 2);
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

void IO(char* plainText, char* key, char* initializationVector, int rounds) // coleta os dados do usuario
{
	puts("Insert plainText: [Binary] [Up to 8 bits]");
	scanf("%s", plainText);

	puts("Insert key: [Binary] [At least plainText size]");
	scanf("%s", key);

	puts("Insert initialization vector: [Binary] [At least plainText size]");
	scanf("%s", initializationVector);

	if (strlen(key) < strlen(plainText))
	{
		puts("Key size must be at least plainText size. Exiting.\n");
		exit(0);
	}

	if (strlen(initializationVector) < strlen(plainText))
	{
		puts("Initialization Vector size must be at least plainText size. Exiting.\n");
		exit(0);
	}

	puts("Insert number of rounds:");
	scanf("%d", rounds);
}