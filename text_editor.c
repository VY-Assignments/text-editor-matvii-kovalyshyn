#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "text_editor.h"


struct Row {
    char* data;
};


struct Array {
    struct Row* rows;
    int rowsCount;
    int currentRow;
};


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
    
}


bool Insert(struct Array* array, short line, short index, char* text) {
   
    if (array->rows[line].data == NULL || strlen(array->rows[line].data) == 0)  {
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
    for (int i = 0; i < array->rowsCount; i++) {
        if (array->rows[i].data == NULL) {
            if (i >= array->currentRow) {
                break;
            }
        }
        else {
            printf("%s", array->rows[i].data);
        }
        
    }
}

bool Delete(struct Array* array, short line, short index, int symbols ) {
    if (array->rows[line].data == NULL || strlen(array->rows[line].data) == 0)  {
        return 0;
    }
    if (index > strlen(array->rows[line].data)) {
        return 0;
    }

    int newLength = strlen(array->rows[line].data) - symbols + 1;
    char* endRow = malloc((strlen(array->rows[line].data + symbols) + 1) * sizeof(char));
    strcpy(endRow, array->rows[line].data + index + symbols + 1);
    printf("endRow = %s\n", endRow);
    char* temp = realloc(array->rows[line].data, newLength * sizeof(char));
    if (temp == NULL) {
        printf("MEMORY ERROR");
        return 0;
    }
    array->rows[line].data = temp;
    printf("row = %s\n", array->rows[line].data);
    array->rows[line].data[index] = '\0';
    strcat(array->rows[line].data, endRow);
    
    free(endRow);

    printf("result = %s\n", array->rows[line].data);
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


void ClearConsole() {
    // \e[1J — очищає екран від курсора вгору
    // \e[H  — повертає курсор у лівий верхній кут (0,0)
    printf("\e[1J\e[H");
}

