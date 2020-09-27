#include<stdio.h>
#include<stdlib.h>
#include <ctype.h>
#include"Header.h"
#pragma warning(disable:4996)

void GetText(char text[]) // user insert plain text
{
    puts("Insert the message :");
    scanf("%[^\n]%*c", text);
}

void GetKey(char keyText[], int size) // User insert password key
{
    printf("\nInsert the key [It must be at least %i characters long]:\n", size);
    scanf("%s", keyText);
    if (strlen(keyText) < size)
    {
        puts("\nKeyText is too small. Operation aborted");
        exit(0);
    }
}

void Encrypt(char plainText[], char keyChar[], char result[]) // sum plainText and passwordKey
{
    int i = 0;
    for (i = 0; i < strlen(plainText); i++)
    {
        char ch = tolower(plainText[i]);
        char key = tolower(keyChar[i]);
        int sum = ConvertCharToNum(ch) + ConvertCharToNum(key);
        //printf("%c: Sum %i with %i = %i\n", ch, ConvertCharToNum(ch), ConvertCharToNum(key), sum);
        if (sum >= 100) result[i] = ' '; // keep the space character
        else result[i] = GetCharFromNum(sum-1);
    }
    result[i] = '\0';
}

int ConvertCharToNum(char ch)
{
    // space character
    if ((int)ch == 32) return 100;

    int counter = 1;
    for (int i = 97; i <= 122; i++)
    {
        if ((int)ch == i)
        {
            return counter;
        }
        else
        {
            counter++;
        }
    }
}

char GetCharFromNum(int num)
{
    int counter = 1;
    for (int i = 97; i <= 122; i++)
    {
        if (num % 26 == counter)
        {
            return i;
        }
        else if (num % 26 == 0)
        {
            return 'z';
        }
        else
        {
            counter++;
        }
    }
}

char DecryptChar(char c, char keyChar)
{
    char key = tolower(keyChar);
    if (c == ' ') return ' ';
    int value = ((int)c - (int)key + 26) % 26;
    return GetCharFromNum(value+1);
}