#pragma once

#define MAXCHAR 20000

void ToLowerConversion(char* plainText);
void GetPlainText(FILE* file, char* plainTextDirectory, char* plainText);
void GetPlainText(FILE* file, char* plainTextDirectory, char* plainText);
void AddFrequencyChar(char c, int* table);
void SetTable(FILE* file, char* tableDirectory, int table[26], int i);
void GetFrequency(char* plainText, int* table);
