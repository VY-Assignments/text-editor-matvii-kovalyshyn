#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "text_editor.h"


struct ActionInfo* CreateActionInfo(int amount, int line, int index, char* prevText) {
    struct ActionInfo* actionInfo = malloc(sizeof(struct ActionInfo));
    actionInfo->amount = amount;
    actionInfo->line = line;
    actionInfo->index = index;
    
    if (prevText != NULL) {
        char* temp = malloc((strlen(prevText) + 1) * sizeof(char));
        if (temp == NULL) {
            printf("MEMORY ERROR");
            return NULL;
        }
        strcpy(temp, prevText);
        actionInfo->prevText = temp;
    }
    else {
        actionInfo->prevText = NULL;
    }
    
    return actionInfo;
}


struct Row* CreateRows(int rowsCount) {
    struct Row* rows = malloc(rowsCount * sizeof(struct Row));
    if (rows == NULL) {
        printf("MEMORY ERROR");
        return NULL;
    }
    for (int i = 0; i < rowsCount; i++)
    {
        rows[i].data = NULL;
    }
    return rows;
}


struct Array* CreateArray(int rowsCount) {
    struct Array *array = malloc(sizeof(struct Array));
    if (array == NULL) {
        printf("MEMORY ERROR");
        return NULL;
    }
    array->rows = CreateRows(rowsCount);
    array->rowsCount = rowsCount;
    array->currentRow = 0;
    array->undoLastAction = malloc(sizeof(struct HistoryAction));
    array->undoLastAction->value = NoneAction;
    array->undoLastAction->actionInfo = NULL;
    array->redoLastAction = malloc(sizeof(struct HistoryAction));
    array->redoLastAction->value = NoneAction;
    array->undoLastAction->actionInfo = NULL;
    return array;
}


void AddToEnd(struct Array* array, char* text) {
    int endRow = array->currentRow;
    if (endRow >= array->rowsCount) {
        int newRowsCount = array->rowsCount * 2;
        struct Row* temp = realloc(array->rows, newRowsCount * sizeof(struct Row));
        if (temp == NULL) {
            printf("MEMORY ERROR");
            return;
        }
        array->rows = temp;
        for (int i = array->rowsCount; i < newRowsCount; i++) {
            array->rows[i].data = NULL;
        }
        array->rowsCount = newRowsCount;
    }
    if (array->rows[endRow].data == NULL  || strlen(array->rows[endRow].data) == 0) {
        free(array->rows[endRow].data);
        array->rows[endRow].data = strdup(text);
        return;
    }
    int newLength = strlen(array->rows[endRow].data) + strlen(text) + 1;
    char* temp = realloc(array->rows[endRow].data, newLength * sizeof(char));
    if (temp == NULL) {
        printf("MEMORY ERROR");
        return;
    }
    array->rows[endRow].data = temp;
    strcat(array->rows[endRow].data, text);
}


void AddNewLine(struct Array* array) {
    AddToEnd(array, "\n");
    array->currentRow += 1;
    AddToEnd(array, "\0");
}


bool Insert(struct Array* array, short line, short index, char* text) {
    if (array->rows[line].data == NULL)  {
        return 0;
    }
    if (index > strlen(array->rows[line].data)) {
        return 0;
    }
    
    int newLength = strlen(array->rows[line].data) + strlen(text) + 1;
    char* afterIndex = malloc((strlen(array->rows[line].data + index) + 1) * sizeof(char));
    strcpy(afterIndex, array->rows[line].data + index);
    char* temp = realloc(array->rows[line].data, newLength * sizeof(char));
    if (temp == NULL) {
        printf("MEMORY ERROR");
        return 0;
    }
    array->rows[line].data = temp;
    array->rows[line].data[index] = '\0';
    strcat(array->rows[line].data, text);
    strcat(array->rows[line].data, afterIndex);
    
    free(afterIndex);
    return 1;
}


bool InsertWithReplacement(struct Array* array, short line, short index, char* text) {
    if (array->rows[line].data == NULL)  {
        return 0;
    }
    if (index > strlen(array->rows[line].data)) {
        return 0;
    }

    int textLength = strlen(text);
    int startLength = strlen(array->rows[line].data);
    int afterLength = startLength - index;
    
    char* afterIndex = NULL;
    if (textLength > afterLength) {
        int newLength = textLength + index;
        char* temp = realloc(array->rows[line].data, (newLength + 1) * sizeof(char));
        if (temp == NULL) {
            printf("MEMORY ERROR");
            free(afterIndex);
            return 0;
        }
        array->rows[line].data = temp;
    }
    memcpy(array->rows[line].data + index, text, textLength);
    if (textLength > afterLength) {
        array->rows[line].data[textLength + index] = '\0';
    }
    return 1;
}


int Search(struct Array* array, char* text, int** indexes) {
    
    int lastIndex = 0;

    for (int i = 0; i <= array->currentRow; i++) {
        
        
        if (i == array->currentRow && (array->rows[array->currentRow].data == NULL || strlen(array->rows[array->currentRow].data) == 0)) {
            break;
        }

        int textLen = strlen(text);
        int rowLen = strlen(array->rows[i].data);
        
        if (array->rows[i].data == NULL || rowLen == 0) {
            continue;
        }

        int lenCounter = 0;
        
        for (int j = 0; j < rowLen; j++) {
            
            if (array->rows[i].data[j] == '\0') {
                break;
            }

            if (array->rows[i].data[j] == text[lenCounter]) {
                lenCounter ++;
            }
            
            else {
                j -= lenCounter;
                lenCounter = 0;
            }
            
            if (lenCounter == textLen) {

                int* temp = realloc(*indexes, (lastIndex + 2) * sizeof(int));
                if (temp == NULL) {
                    printf("MEMORY ERROR");
                    return lastIndex;
                }
                
                *indexes = temp;                
                
                (*indexes)[lastIndex] = i;
                (*indexes)[lastIndex + 1] = j - (strlen(text) - 1);

                lastIndex += 2;

                j -= lenCounter - 1;
                lenCounter = 0;
                
            }
        }
    }
    return lastIndex;
}


void PrintFoundIndexes(int* indexes, int lastIndex) {
    
    for (int i = 0; i < lastIndex; i += 2) {
        printf("(%d, %d) ", indexes[i], indexes[i + 1]);
    }
}


void Print(struct Array* array) {
    for (int i = 0; i <= array->currentRow; i++) {
        if (array->rows[i].data == NULL) {
            continue;
        }
        printf("%s", array->rows[i].data);
    }
}


bool Delete(struct Array* array, short line, short index, int symbols) {
    if (line > array->currentRow) {
        return 0;
    }
    if (array->rows[line].data == NULL || strlen(array->rows[line].data) == 0)  {
        return 0;
    }
    if (index > strlen(array->rows[line].data)) {
        return 0;
    }
    if (symbols > strlen(array->rows[line].data + index)) {
        symbols = strlen(array->rows[line].data + index);
    }

    int newLength = strlen(array->rows[line].data) - symbols + 1;
    char* endRow = malloc((strlen(array->rows[line].data + index + symbols) + 1) * sizeof(char));
    strcpy(endRow, array->rows[line].data + index + symbols);
    char* temp = realloc(array->rows[line].data, newLength * sizeof(char));
    if (temp == NULL) {
        printf("MEMORY ERROR[DELETE]");
        return 0;
    }
    array->rows[line].data = temp;
    array->rows[line].data[index] = '\0';
    strcat(array->rows[line].data, endRow);
    
    free(endRow);

    return 1;
}


void Free(struct Array* array) {
    for (int i = 0; i < array->rowsCount; i++) {
        free(array->rows[i].data);
    }
    free(array->rows);
    free(array);
}


void SaveToFile(struct Array* array, char* fileName) {
    FILE* file;
    file = fopen(fileName, "w");
    if (file != NULL) {
        for (int i = 0; i < array->rowsCount; i++) {
            if (array->rows[i].data != NULL) {
                fputs(array->rows[i].data, file);
            }
        }
        fclose(file);
    }
}


bool LoadFromFile(struct Array** array, char* fileName) {
    FILE* file;
    char row[100];
    file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error opening file.");
        return 0;
    }
    else {
        *array = CreateArray(10);
        int i = 0;
        while (fgets(row, sizeof(row), file) != NULL) {
            AddToEnd(*array, row);
            (*array)->currentRow++;
            i++;
        }
        fclose(file);
    }
    return 1;
}


bool Copy(struct Array* array, short line, short index, int symbols) {
    if (array->rows[line].data == NULL || strlen(array->rows[line].data) == 0)  {
        return 0;
    }
    if (index > strlen(array->rows[line].data)) {
        return 0;
    }

    int actualSymbols = 0;
    if (symbols > strlen(array->rows[line].data + index)) {
        actualSymbols = strlen(array->rows[line].data + index);
    }
    else {
        actualSymbols = symbols;
    }

    char* temp = malloc(actualSymbols * sizeof(char) + 1);
    if (temp == NULL) {
        printf("MEMORY ERROR[COPY]");
        return 0;
    }
    char* copied = temp;
    strncpy(copied, array->rows[line].data + index, actualSymbols);
    copied[actualSymbols] = '\0';
    array->copied = copied;
    return 1;
} 


bool Paste(struct Array* array, short line, short index) {
    return Insert(array, line, index, array->copied);
}


bool Cut(struct Array* array, short line, short index, int symbols) {
    return Copy(array, line, index, symbols) && Delete(array, line, index, symbols);
}


void ClearConsole() {
    // \e[1J — очищає екран від курсора вгору
    // \e[H  — повертає курсор у лівий верхній кут (0,0)
    // printf("\e[1J\e[H");
}


void HistoryPush(struct Array* array, enum Action action, struct ActionInfo* actionInfo, bool isUndo) {
    struct HistoryAction* newAction = malloc(sizeof(struct HistoryAction));
    if (newAction == NULL) {
        printf("MEMORY ERROR");
        return;
    }

    if (isUndo) {
        newAction->value = action;
        newAction->next = array->undoLastAction;
        newAction->actionInfo = actionInfo;
        array->undoLastAction = newAction;
    } 
    else {
        newAction->value = action;
        newAction->next = array->redoLastAction;
        newAction->actionInfo = actionInfo;
        array->redoLastAction = newAction;
    }
}


void HistoryPop(struct Array* array, bool isUndo) {
    if (isUndo) {
        if (array->undoLastAction == NoneAction && array->undoLastAction == NULL) {
            return;
        }
        struct HistoryAction* last = array->undoLastAction;
        array->undoLastAction = array->undoLastAction->next;
        if (last->actionInfo != NULL) {
            free(last->actionInfo);
            free(last->actionInfo->prevText);
        }
        free(last);
    }
    else {
        if (array->redoLastAction == NoneAction && array->redoLastAction == NULL) {
            return;
        }
        struct HistoryAction* last = array->redoLastAction;
        array->redoLastAction = array->redoLastAction->next;
        if (last->actionInfo != NULL) {
            free(last->actionInfo);
            free(last->actionInfo->prevText);
        }
        free(last);
    }
    
}


void HistoryClear(struct Array* array, bool isUndo) {
    printf("text\n");
    if (isUndo) {
        while(array->undoLastAction != NoneAction || array->undoLastAction != NULL) {
           HistoryPop(array, isUndo);
        }
    }
    else {
        while(array->redoLastAction != NoneAction || array->redoLastAction != NULL) {
            printf("text2\n");
            HistoryPop(array, isUndo);
            printf("text3\n");
        }
    }
}


void Undo(struct Array* array) {
    if (array->undoLastAction == NULL || array->undoLastAction->value == NoneAction) {
        return;
    }
    struct ActionInfo* actionInfo = NULL;
    char* temp = NULL;
    switch(array->undoLastAction->value) {
        case AddToEndAction:
        case InsertAction:
        case PasteAction:
            temp = malloc((array->undoLastAction->actionInfo->amount + 1) * sizeof(char));
            if (temp == NULL) {
                printf("MEMORY ERROR");
                return;
            }
            strncpy(temp, array->rows[array->undoLastAction->actionInfo->line].data + array->undoLastAction->actionInfo->index, array->undoLastAction->actionInfo->amount);
            temp[array->undoLastAction->actionInfo->amount] = '\0';

            Delete(array, array->undoLastAction->actionInfo->line, array->undoLastAction->actionInfo->index, array->undoLastAction->actionInfo->amount);

            actionInfo = CreateActionInfo(array->undoLastAction->actionInfo->amount, array->undoLastAction->actionInfo->line, array->undoLastAction->actionInfo->index, temp);
            HistoryPush(array, DeleteAction, actionInfo, 0);
            break;

        case AddNewLineAction:
            free(array->rows[array->currentRow].data);
            if (array->currentRow < array->rowsCount - 1) {
                array->rows[array->currentRow] = array->rows[array->rowsCount];
            }
            array->currentRow--;
            array->rows[array->rowsCount].data = NULL;
            Delete(array, array->currentRow, strlen(array->rows[array->currentRow].data) - 1, 1);
            
            actionInfo = CreateActionInfo(1, array->currentRow, 0, "");
            HistoryPush(array, DeleteLineAction, actionInfo, 0);
            break;
        
        case InsertWithReplacementAction:
            temp = malloc((array->undoLastAction->actionInfo->amount + 1) * sizeof(char));
            if (temp == NULL) {
                printf("MEMORY ERROR");
                return;
            }
            strncpy(temp, array->rows[array->undoLastAction->actionInfo->line].data + array->undoLastAction->actionInfo->index, array->undoLastAction->actionInfo->amount);
            temp[array->undoLastAction->actionInfo->amount] = '\0';

            Delete(array, array->undoLastAction->actionInfo->line, array->undoLastAction->actionInfo->index, array->undoLastAction->actionInfo->amount);
            Insert(array, array->undoLastAction->actionInfo->line, array->undoLastAction->actionInfo->index, array->undoLastAction->actionInfo->prevText);
            
            actionInfo = CreateActionInfo(array->undoLastAction->actionInfo->amount, array->undoLastAction->actionInfo->line, array->undoLastAction->actionInfo->index, temp);
            HistoryPush(array, InsertWithReplacementAction, actionInfo, 0);
            break;

        case DeleteAction:
        case CutAction:
            Insert(array, array->undoLastAction->actionInfo->line, array->undoLastAction->actionInfo->index, array->undoLastAction->actionInfo->prevText);
            
            actionInfo = CreateActionInfo(array->undoLastAction->actionInfo->amount, array->undoLastAction->actionInfo->line, array->undoLastAction->actionInfo->index, array->undoLastAction->actionInfo->prevText);
            HistoryPush(array, InsertAction, actionInfo, 0);
            break;
    }
    
    HistoryPop(array, 1);
    free(temp);
}


void Redo(struct Array* array) {
    if (array->redoLastAction == NULL || array->redoLastAction->value == NoneAction) {
        return;
    }
    struct ActionInfo* actionInfo = NULL;
    switch(array->redoLastAction->value) {
        case InsertAction:
            Delete(array, array->redoLastAction->actionInfo->line, array->redoLastAction->actionInfo->index, array->redoLastAction->actionInfo->amount);
            
            actionInfo = CreateActionInfo(array->redoLastAction->actionInfo->amount, array->redoLastAction->actionInfo->line, array->redoLastAction->actionInfo->index, array->redoLastAction->actionInfo->prevText);
            HistoryPush(array, DeleteAction, actionInfo, 1);
            break;
        case InsertWithReplacementAction:
            Delete(array, array->redoLastAction->actionInfo->line, array->redoLastAction->actionInfo->index, array->redoLastAction->actionInfo->amount);
            Insert(array, array->redoLastAction->actionInfo->line, array->redoLastAction->actionInfo->index, array->redoLastAction->actionInfo->prevText);
            
            actionInfo = CreateActionInfo(array->redoLastAction->actionInfo->amount, array->redoLastAction->actionInfo->line, array->redoLastAction->actionInfo->index, array->redoLastAction->actionInfo->prevText);
            HistoryPush(array, InsertWithReplacementAction, actionInfo, 1);
            break;
        case DeleteAction:
            Insert(array, array->redoLastAction->actionInfo->line, array->redoLastAction->actionInfo->index, array->redoLastAction->actionInfo->prevText);
            
            actionInfo = CreateActionInfo(array->redoLastAction->actionInfo->amount, array->redoLastAction->actionInfo->line, array->redoLastAction->actionInfo->index, array->redoLastAction->actionInfo->prevText);
            HistoryPush(array, InsertAction, actionInfo, 1);
            break;
        case DeleteLineAction:
            AddNewLine(array);
            
            actionInfo = CreateActionInfo(0, 0, 0, "");
            HistoryPush(array, AddNewLineAction, actionInfo, 1);
            break;

    }
    HistoryPop(array, 0);
    
}
