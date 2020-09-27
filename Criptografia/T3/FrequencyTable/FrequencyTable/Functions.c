#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include"Header.h"
#pragma warning(disable:4996)

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
    puts("Getting PlainText:");
    while (fgets(plainText, MAXCHAR, file) != NULL)
        printf("%s", plainText);
    fclose(file);
}

void SetTable(FILE* file, char* tableDirectory, int table[26], int i)
{
    char fullDirectory[100];
    strcpy(fullDirectory, tableDirectory);
    char index[3];
    sprintf(index, "%d", i);
    strcat(fullDirectory, index);
    strcat(fullDirectory, ".txt");

    printf("\n%s\n", fullDirectory);

    file = fopen(fullDirectory, "w");
    if (file == NULL)
    {
        printf("\n\nCould not open file %s\n\n", fullDirectory);
        exit(0);
    }

    for (int i = 0; i < 26; i++)
    {
        printf("%d\n", table[i]);
    }

    for (int i = 0; i < 26; i++)
    {
        fprintf(file, "%d\n", table[i]);
    }

    fclose(file);
}

void GetFrequency(char* plainText, int* table)
{
    for (int i = 0; i < 26; i++) table[i] = 0;
    printf("Length: %d", strlen(plainText));
    puts("\n");
    for (int i = 0; i < strlen(plainText); i++)
    {
        if (plainText[i] >= 'a' && (int)plainText[i] <= 'z')
            AddFrequencyChar(plainText[i], table);
    }
}

void AddFrequencyChar(char c, int* table)
{
    int pos = (int)c - 97;
    table[pos] = table[pos] + 1;
}