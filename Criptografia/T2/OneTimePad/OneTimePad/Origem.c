#include<stdio.h>
#include<string.h>
#include <ctype.h>
#include"Header.h"
#pragma warning(disable:4996)

/// One-Time Pad
/// Code by: Bruno Gottlieb
int main()
{
    char plainText[500]; // the original text
    char key[500]; // the key used to encrypt
    char encryptedText[500]; // the final message, encrypted
    int choice;

    puts("[Spaces and uppercase are allowed for the plainText]\n");
    GetText(plainText); // receive the plainText
    GetKey(key, strlen(plainText)); // receive the password key

    puts(" ");
    puts("What do you want do to?");
    puts("[1] Encrypt");
    puts("[2] Decrypt");
    puts("[3] Encrypt and Decrypt");
    scanf("%d", &choice);
    puts(" ");

    switch (choice)
    {
        case 1:
            Encrypt(plainText, key, encryptedText); // Encrypt the message
            puts("Encrypted message:");
            for (int i = 0; i < strlen(encryptedText); i++)
            {
                printf("%c", encryptedText[i]);
            }
            puts(" ");
        break;
        case 2:
            puts("\nDecrypted message:");
            for (int i = 0; i < strlen(plainText); i++)
            {
                if (plainText[i] == '\0') break;
                char ch = DecryptChar(tolower(plainText[i]), key[i]);
                printf("%c", ch);
            }
            puts("\n");
            break;
        case 3:
            Encrypt(plainText, key, encryptedText); // Encrypt the message
            puts("\nEncrypted message:");
            for (int i = 0; i < strlen(encryptedText); i++)
            {
                printf("%c", encryptedText[i]);
            }
            puts(" ");
            puts("\nNow decrypting:");
            for (int i = 0; i < strlen(encryptedText); i++)
            {
                if (encryptedText[i] == '\0') break;
                char ch = DecryptChar(encryptedText[i], key[i]);
                printf("%c", ch);
            }
            puts("\n");
            break;
    }
    system("pause");
}