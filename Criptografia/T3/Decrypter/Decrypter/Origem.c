#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include"Header.h"
#pragma warning(disable:4996)

int main() 
{
    struct Letter plainTextletter[26];
    struct Letter tableLetter[26];
    FILE* file = NULL;
    char plainText[MAXCHAR];
    char resultText[MAXCHAR];
    int fileFrequency[26];
    char* EncryptedTextDirectory = "c:\\temp\\Encrypted\\EncryptedText.txt";
    char* ResultTextDirectory = "c:\\temp\\Result\\ResultText_";
    char* tableDirectory = "c:\\temp\\Frequency\\FrequencyTables\\FrequencyTable_";
    char* dictionaryDirectory = "c:\\temp\\Dictionary.txt";
    char* matchDirectory = "c:\\temp\\Result\\Matchs\\MatchsTable_";

    GetPlainText(file, EncryptedTextDirectory, plainText); // pega o texto encriptado
    ToLowerConversion(plainText); // assegura que todos os characteres sao minusculos

    for (int i = 1; i < 100; i++)
    {
        // esse trecho eh para iterar os diretorios dos textos
        char fullDirectory[100];
        DirectoryIteration(fullDirectory, tableDirectory, i);
        // ---------------------------------------------------
        InitializeLetters(plainTextletter, tableLetter); // prepara a struct
        GetFrequencyFromPlainText(plainText, plainTextletter); // extrai a frequencia do texto encriptado
        strcpy(resultText, plainText); // passa o texto para outra string

        file = fopen(fullDirectory, "r");
        if (file != NULL)
        {
            GetFrequencyFromFile(file, fullDirectory, tableLetter); // extrai a frequencia da tabela

            GetPlainText(file, EncryptedTextDirectory, plainText); // pega o texto encriptado
            ToLowerConversion(plainText); // assegura que todos os characteres sao minusculos

            QuickSort(tableLetter, 0, 25);

            puts("\ntableLetter ordered:");
            PrintLetters(tableLetter);

            QuickSort(plainTextletter, 0, 25);

            puts("\nplainTex ordered:");
            PrintLetters(plainTextletter);

            UpdateText(plainText, resultText, plainTextletter, tableLetter);

            SaveResult(file, ResultTextDirectory, resultText, i);

            puts(" ");
        }
        else
        {
            printf("\nTotal of %d result files sucessfully generated at: %s\n\n", i - 1, "c:\\temp\\Result");

            puts("Compare results with the dictionary? [Y/N]");
            char answer;
            scanf("%c", &answer);

            if (answer == 'Y' || answer == 'y') // usuario quer comparar com o dicionario
                CompareWithDictionary(file, ResultTextDirectory, dictionaryDirectory, matchDirectory, resultText);
            
            exit(1);
        }
        
    }

    return 0;
}