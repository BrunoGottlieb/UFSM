#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include"Header.h"
#pragma warning(disable:4996)

void InitializeLetters(struct Letter* plainTextletter, struct Letter* tableLetter)
{
    for (int i = 0; i < 26; i++)
    {
        plainTextletter[i].frequency = 0;
        tableLetter[i].frequency = 0;
        plainTextletter[i].letter = (char)i + 97;
        tableLetter[i].letter = (char)i + 97;
    }
}

void PrintLetters(struct Letter* letter)
{
    for (int i = 0; i < 26; i++)
    {
        printf("Letter: %c | Frequency: %d\n", letter[i].letter, letter[i].frequency);
    }
}

void ToLowerConversion(char* plainText)
{
    for (int i = 0; i < strlen(plainText); i++)
        plainText[i] = tolower(plainText[i]);
}

void GetPlainText(FILE* file, char* plainTextDirectory, char* plainText)
{
    file = fopen(plainTextDirectory, "r");
    if (file == NULL)
    {
        printf("\n\nCould not open file %s\n\n", plainTextDirectory);
        exit(0);
    }
    puts("Getting PlainText");
    while (fgets(plainText, MAXCHAR, file) != NULL);
        //printf("%s", plainText);
    fclose(file);
}

void GetFrequencyFromPlainText(char* plainText, struct Letter* letter)
{
    puts("\n\nGetting frequency from PlainText");
    //printf("Length: %d", strlen(plainText));
    for (int i = 0; i < strlen(plainText); i++)
    {
        if (plainText[i] >= 'a' && (int)plainText[i] <= 'z')
            AddFrequencyChar(plainText[i], letter, i);
    }
}

void AddFrequencyChar(char c, struct Letter* letter, int i)
{
    int pos = (int)c - 97;
    letter[pos].frequency = letter[pos].frequency + 1;
}

void GetFrequencyFromFile(FILE* file, char* tableDirectory, struct Letter* letter) // pega os valores do arquivo
{
    size_t len = 26;
    size_t read;
    file = fopen(tableDirectory, "r");
    if (file == NULL)
    {
        printf("\n\nCould not open file %s\n\n", tableDirectory);
        exit(0);
    }
    puts("\n\nGetting FrequencyText:");
    int aux = 0;
    int i = 0; int c = 97;

    fscanf(file, "%d", &aux);
    while (!feof(file))
    {
        printf("%d ", aux);
        letter[i].frequency = aux;
        fscanf(file, "%d", &aux);
        i++;
    }
    fclose(file);
}

void UpdateText(char* plainText, char* resultText, struct Letter* plainTextLetter, struct Letter* tableLetter)
{
    for (int i = 25; i > 0; i--)
    {
        ChangeLetters(plainText, resultText, plainTextLetter[i].letter, tableLetter[i].letter);
    }
}

void ChangeLetters(char* plainText, char* resultText, char before, char after)
{
    for (int i = 0; i < strlen(plainText); i++)
    {
        if (plainText[i] == before)
        {
            resultText[i] = after;
        }
    }
}

void SaveResult(FILE* file, char* ResultTextDirectory, char* resultText, int i)
{
    char fullDirectory[100];
    DirectoryIteration(fullDirectory, ResultTextDirectory, i);
    // ---------------------------------------------------

    file = fopen(fullDirectory, "w");
    if (file == NULL)
    {
        printf("\n\nCould not open file %s\n\n", fullDirectory);
        exit(0);
    }

    for (int i = 0; i < strlen(resultText); i++)
    {
        fprintf(file, "%c", resultText[i]);
    }
    printf("\n\nDecrypted text file was saved at %s", fullDirectory);

    fclose(file);
}

void DirectoryIteration(char* fullDirectory, char* targetTextDirectory, int i)
{
    strcpy(fullDirectory, targetTextDirectory);
    char index[3];
    sprintf(index, "%d", i);
    strcat(fullDirectory, index);
    strcat(fullDirectory, ".txt");
}

void CompareWithDictionary(FILE* file, char* ResultTextDirectory, char* dictionaryDirectory, char* matchDirectory, char* resultText)
{
    int counter = 0; // contar o numero de matchs

    for (int i = 1; i < 100; i++)
    {
        // esse trecho eh para iterar os diretorios dos textos
        char fullDirectory[100];
        DirectoryIteration(fullDirectory, ResultTextDirectory, i);
        
        // ---------------------------------------------------
        file = fopen(fullDirectory, "r");
        if (file != NULL)
        {
            printf("\nprocessing text %d... [It can take a minute]", i);
            while (fgets(resultText, MAXCHAR, file) != NULL); // extrai o texto do arquivo
            fclose(file);
            // ------------------
            //printf("%s", resultText);
            puts("\n\n");

            DirectoryIteration(fullDirectory, matchDirectory, i);
            remove(fullDirectory);

            char* token = strtok(resultText, " ");
            while (token != NULL) // loop por todas as palavras do result text
            {
                char str[40];
                file = fopen(dictionaryDirectory, "r"); // abre o arquivo de dicionario
                while (!feof(file)) // enquanto o dicionario nao tiver chego ao fim
                {
                    if (fgets(str, 40, file) != NULL) // loop por cada linha (palavra) do dicionario
                    {
                        CheckWord(token); CheckWord(str); // ajusta as palavras para a comparacao
                        //printf("\nToken: %s and dictionary: %s\n", token, str);
                        if (str == token || strcmp(str, token) == 0) // MATCH
                        {
                            counter++;
                            //printf("\nMATCH: %s\n", token);

                            FILE* ftp = NULL;
                            ftp = fopen(fullDirectory, "a"); // abre o arquivo de dicionario
                            for (int i = 0; i < strlen(str); i++)
                            {
                                fprintf(ftp, "%c", str[i]);
                            }
                            fprintf(ftp, "%c", '\n');
                            fclose(ftp);
                        }
                    }
                }
                fclose(file);
                token = strtok(NULL, " ");
            }

            printf("\nTotal of matchs with the dictionary: %d\n\n", counter);

        }

    }
}

void CheckWord(char* word)
{
    for (int i = 0; i < strlen(word); i++)
    {
        if (word[i] == '\n' || word[i] == ' ')
        {
            word[i] = '\0';
            return;
        }
    }
}

void QuickSort(struct Letter* letter, int first, int last)
{
    int i, j, pivot, temp;
    char temp1;

    if (first < last)
    {
        pivot = first;
        i = first;
        j = last;

        while (i < j)
        {
            while (letter[i].frequency <= letter[pivot].frequency && i < last)
                i++;
            while (letter[j].frequency > letter[pivot].frequency)
                j--;
            if (i < j)
            {
                int temp = letter[i].frequency; char temp1 = letter[i].letter;
                letter[i].frequency = letter[j].frequency;
                letter[i].letter = letter[j].letter;
                letter[j].frequency = temp;
                letter[j].letter = temp1;
            }
        }

        temp = letter[pivot].frequency; temp1 = letter[pivot].letter;
        letter[pivot].frequency = letter[j].frequency;
        letter[pivot].letter = letter[j].letter;
        letter[j].frequency = temp;
        letter[j].letter = temp1;
        QuickSort(letter, first, j - 1);
        QuickSort(letter, j + 1, last);
    }
}