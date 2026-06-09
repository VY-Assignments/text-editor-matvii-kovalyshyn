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
    
    char* temp = malloc((strlen(prevText) + 1) * sizeof(char));
    if (temp == NULL) {
        printf("MEMORY ERROR");
        return NULL;
    }
    strcpy(temp, prevText);
    actionInfo->prevText = temp;
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
    array->actionCount = 0;
    array->lastAction = malloc(sizeof(struct HistoryAction));
    array->lastAction->value = NoneAction;
    array->lastAction->actionInfo = NULL;
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
    if (array->rows[endRow].data == NULL) {
        array->rows[endRow].data = strdup(text);
        return;
    }
    if (strlen(array->rows[endRow].data) == 0) {
        strcpy(array->rows[endRow].data, text);
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
    if ((array->rows[line].data == NULL || strlen(array->rows[line].data) == 0) && line > array->currentRow)  {
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
    if (array->rows[line].data == NULL || strlen(array->rows[line].data) == 0)  {
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


void HistoryPush(struct Array* array, enum Action action, struct ActionInfo* actionInfo) {
    struct HistoryAction* newAction = malloc(sizeof(struct HistoryAction));
    if (newAction == NULL) {
        printf("MEMORY ERROR");
        return;
    }

    newAction->value = action;
    newAction->next = array->lastAction;
    newAction->actionInfo = actionInfo;
    array->lastAction = newAction;
    array->actionCount++;
}


void HistoryPop(struct Array* array) {
    if (array->lastAction == NoneAction) {
        return;
    }
    struct HistoryAction* last = array->lastAction;
    array->lastAction = array->lastAction->next;
    free(last->actionInfo->prevText);
    free(last);
    array->actionCount--;
}


void Undo(struct Array* array) {
    switch(array->lastAction->value) {
        case AddToEndAction:
            Delete(array, array->currentRow, strlen(array->rows[array->currentRow].data) - array->lastAction->actionInfo->amount, array->lastAction->actionInfo->amount);
            HistoryPop(array);
            break;
        case AddNewLineAction:
            free(array->rows[array->currentRow].data);
            if (array->currentRow < array->rowsCount - 1) {
                array->rows[array->currentRow] = array->rows[array->rowsCount];
            }
            array->currentRow--;
            array->rows[array->rowsCount].data = NULL;
            Delete(array, array->currentRow ,strlen(array->rows[array->currentRow].data) - 1, 1);
            HistoryPop(array);
            break;
        case InsertAction:
            Delete(array, array->lastAction->actionInfo->line, array->lastAction->actionInfo->index, array->lastAction->actionInfo->amount);
            HistoryPop(array);
            break;
        case InsertWithReplacementAction:
            Delete(array, array->lastAction->actionInfo->line, array->lastAction->actionInfo->index, array->lastAction->actionInfo->amount);
            Insert(array, array->lastAction->actionInfo->line, array->lastAction->actionInfo->index, array->lastAction->actionInfo->prevText);
            HistoryPop(array);
            break;
        
        
    }
}


void Redo() {

}

