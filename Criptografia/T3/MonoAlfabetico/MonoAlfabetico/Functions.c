#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include"Header.h"
#pragma warning(disable:4996)

void Encrypt(char* plainText, char* table)
{
    for (int i = 0; i < strlen(plainText); i++)
    {
        if (plainText[i] >= 'a' && (int)plainText[i] <= 'z')
            plainText[i] = EncryptChar(plainText[i], table);
    }
}

char EncryptChar(char c, char* table)
{
    int pos = (int)c - 97;
    return table[pos];
}

void SaveEcryptedText(FILE* file, char* textDirectory, char* text)
{
    file = fopen(textDirectory, "w");
    if (file == NULL)
    {
        printf("\n\nCould not open file %s\n\n", textDirectory);
        exit(0);
    }

    for (int i = 0; i < strlen(text); i++)
    {
        fprintf(file, "%c", text[i]);
    }
    printf("\n\nEncrypted text file was saved at %s", textDirectory);

    fclose(file);
}

void SaveDecryptedText(FILE* file, char* textDirectory, char* text)
{
    file = fopen(textDirectory, "w");
    if (file == NULL)
    {
        printf("\n\nCould not open file %s\n\n", textDirectory);
        exit(0);
    }

    for (int i = 0; i < strlen(text); i++)
    {
        fprintf(file, "%c", text[i]);
    }
    printf("\n\nDecrypted text file was saved at %s", textDirectory);

    fclose(file);
}

void Decrypt(char* plainText, char* table)
{
    int counter = 0;
    for (int i = 0; i < strlen(plainText); i++)
    {
        if (plainText[i] >= 'a' && (int)plainText[i] <= 'z')
            plainText[i] = DecryptChar(plainText[i], table);
    }
}

char DecryptChar(char c, char* table)
{
    int counter = 97;

    for (int i = 0; i < strlen(table); i++)
    {
        //printf("Comparing: %c and %c with i: %d\n", c, table[i], i);
        if (c == table[i])
        {
            return (char)counter;
        }
        counter++;
    }
    return (char)counter;
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
    puts("Getting PlainText:");
    while (fgets(plainText, MAXCHAR, file) != NULL)
        printf("%s", plainText);
    fclose(file);
}

void GetTable(FILE* file, char* tableDirectory, char* table)
{
    file = fopen(tableDirectory, "r");
    if (file == NULL)
    {
        printf("\n\nCould not open file %s\n\n", tableDirectory);
        exit(0);
    }
    puts("\n\nGetting Table:");
    while (fgets(table, 27, file) != NULL)
        printf("%s", table);
    fclose(file);
}