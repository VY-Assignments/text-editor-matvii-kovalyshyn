#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H
#include <stdbool.h>

struct Row;
struct Array;
struct Row* CreateRows(int rowsCount);
struct Array* CreateArray(int rowsCount);
void AddToEnd(struct Array* array, char* text);
void AddNewLine(struct Array* array);
bool Insert(struct Array* array, short line, short index, char* text);
bool InsertWithReplacement(struct Array* array, short line, short index, char* text);
int Search(struct Array* array, char* text, int** indexes);
void PrintFoundIndexes(int* indexes, int lastIndex);
void Print(struct Array* array);
bool Delete(struct Array* array, short line, short index, int symbols );
void Free(struct Array* array);
void SaveToFile(struct Array* array, char* fileName);
bool LoadFromFile(struct Array** array, char* fileName);
bool Copy(struct Array* array, short line, short index, int symbols);
bool Paste(struct Array* array, short line, short index);
bool Cut(struct Array* array, short line, short index, int symbols);
void ClearConsole();

#endif