#pragma once

void GetText(char text[]);
void GetKey(char text[], int size);
void Encrypt(char plainText[], char key[], int result[]);
int ConvertCharToNum(char c);
char GetCharFromNum(int num);
char DecryptChar(char c, char keyChar);