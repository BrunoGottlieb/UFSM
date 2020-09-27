#pragma once

#define MAXCHAR 20000

void GetPlainText(FILE* file, char* plainTextDirectory, char* plainText);
void GetTable(FILE* file, char* tableDirectory, char* table);
void Encrypt(char* plainText, char* table);
char EncryptChar(char c, char* table);
void ToLowerConversion(char* plainText);
void Decrypt(char* plainText, char* table);
char DecryptChar(char c, char* table);
void SaveEcryptedText(FILE* file, char* textDirectory, char* text);
void SaveDecryptedText(FILE* file, char* textDirectory, char* text);