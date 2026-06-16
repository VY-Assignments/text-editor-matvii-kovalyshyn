#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H
#include <stdbool.h>


enum Action {
    NoneAction,
    AddToEndAction,
    AddNewLineAction,
    InsertAction,
    InsertWithReplacementAction,
    DeleteAction,
    PasteAction,
    CutAction,
    DeleteLineAction,
    DeleteAndInsertAction,
};
struct ActionInfo {
    int amount;
    int line;
    int index;
    char* prevText;
};
struct HistoryAction {
    enum Action value;
    struct ActionInfo* actionInfo;
    struct HistoryAction* next;
};
struct Row {
    char* data;
};
struct Array {
    struct Row* rows;
    int rowsCount;
    int currentRow;
    char* copied;
    struct HistoryAction* undoLastAction;
    struct HistoryAction* redoLastAction;
};
struct ActionInfo* CreateActionInfo(int amount, int line, int index, char* prevText);
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
void HistoryPush(struct Array* array, enum Action action, struct ActionInfo* actionInfo, bool isUndo);
void HistoryPop(struct Array* array, bool isUndo);
void HistoryClear(struct Array* array, bool isUndo);
void Undo(struct Array* array);
void Redo(struct Array* array);

#endif