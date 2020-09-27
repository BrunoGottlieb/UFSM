#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include"Header.h"
#pragma warning(disable:4996)

int main() 
{
    FILE* file = NULL;
    char plainText[MAXCHAR];
    char table[27];
    char* plainTextDirectory = "c:\\temp\\PlainText\\PlainText.txt";
    char* tableDirectory = "c:\\temp\\PlainText\\ChangerTable.txt";
    char* encryptedTextDirectory = "c:\\temp\\Encrypted\\EncryptedText.txt";
    char* decryptedTextDirectory = "c:\\temp\\Decrypted\\DecryptedText.txt";
    int choice;

    GetPlainText(file, plainTextDirectory, plainText);
    ToLowerConversion(plainText);
    GetTable(file, tableDirectory, table);
    printf("\n\nLen: %d\n\n", strlen(plainText));
    puts("\n\nWhat do you want to do?");
    puts("[1] Encrypt");
    puts("[2] Decrypt");
    puts("[3] Encrypt and Decrypt");
    scanf("%d", &choice);
    switch (choice)
    {
        case 1:
            Encrypt(plainText, table);
            puts("\nEncrypted:");
            printf("%s", plainText);
            SaveEcryptedText(file, encryptedTextDirectory, plainText);
            break;
        case 2:
            Decrypt(plainText, table);
            puts("\nDecrypted:");
            printf("%s", plainText);
            SaveDecryptedText(file, decryptedTextDirectory, plainText);
            break;
        case 3:
            Encrypt(plainText, table);
            puts("\nEncrypted:");
            printf("%s", plainText);
            SaveEcryptedText(file, encryptedTextDirectory, plainText);
            puts("");
            Decrypt(plainText, table);
            puts("\nDecrypted:");
            printf("%s", plainText);
            SaveDecryptedText(file, decryptedTextDirectory, plainText);
    }
    puts("\n");
    return 0;
}
