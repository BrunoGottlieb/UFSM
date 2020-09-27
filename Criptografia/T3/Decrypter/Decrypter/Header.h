#pragma once

#define MAXCHAR 20000

struct Letter
{
    int frequency;
    char letter;
};

void InitializeLetters(struct Letter* plainTextletter, struct Letter* tableLetter);
void ToLowerConversion(char* plainText);
void GetPlainText(FILE* file, char* plainTextDirectory, char* plainText);
void GetFrequencyFromFile(FILE* file, char* tableDirectory, struct Letter* letter);
void GetFrequencyFromPlainText(char* plainText, struct Letter* letter);
void AddFrequencyChar(char c, struct Letter* letter, int i);
void PrintLetters(struct Letter* letter);
void QuickSort(struct Letter* letter, int first, int last);
void UpdateText(char* plainText, char* resultText, struct Letter* plainTextLetter, struct Letter* tableLetter);
void ChangeLetters(char* plainText, char* resultText, char before, char after);
void SaveResult(FILE* file, char* ResultTextDirectory, char* resultText, int i);
void CompareWithDictionary(FILE* file, char* ResultTextDirectory, char* dictionaryDirectory, char* matchDirectory, char* resultText);
void CheckWord(char* word);
void DirectoryIteration(char* fullDirectory, char* ResultTextDirectory, int i);

