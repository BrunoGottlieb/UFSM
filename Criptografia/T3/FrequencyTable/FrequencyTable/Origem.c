#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include"Header.h"
#pragma warning(disable:4996)

int main()
{
    FILE* file = NULL;
    char plainText[MAXCHAR];
    int table[26] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    char* plainTextDirectory = "c:\\temp\\Frequency\\ExtractionTexts\\ExtractionText_";
    char* tableDirectory = "c:\\temp\\Frequency\\FrequencyTables\\FrequencyTable_";

    for (int i = 1; i < 100; i++)
    {
        // esse trecho eh para iterar os diretorios dos textos
        char fullDirectory[100];
        strcpy(fullDirectory, plainTextDirectory);
        char index[3];
        sprintf(index, "%d", i);
        strcat(fullDirectory, index);
        strcat(fullDirectory, ".txt");

        file = fopen(fullDirectory, "r");

        if (file != NULL)
        {
            GetPlainText(file, fullDirectory, plainText);
            ToLowerConversion(plainText);
            puts("\n");
            GetFrequency(plainText, table);
            SetTable(file, tableDirectory, table, i);
        }
        else
        {
            printf("\nTotal of %d Frequency tables sucessfully generated\n", i-1);
            exit(1);
        }
    }
}
