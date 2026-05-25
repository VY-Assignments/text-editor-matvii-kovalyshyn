#include <stdio.h>
#include <string.h>
#include <stdlib.h>


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


// int FindLastRow(struct Array* array) {
//     printf("last row 0\n");
//     if (array->rows[0].data == NULL || strlen(array->rows[0].data) == 0) {
//         return 0;
//     }
//     printf("last row 1\n");

//     for (int i = 0; i < array->rowsCount; i++) {
//         if (array->rows[i].data == NULL || strlen(array->rows[i].data) == 0) {
//             return i - 1;
//         }
//     }
    
//     // for (int i = 0; i < array->rowsCount; i++) {
//     //     if (array->rows[i].data[strlen(array->rows[i].data)] - 1 == '\n') {
//     //         return i;
//     //     }
//     // }
//     printf("last row 2\n");

//     return -1;

// }


void AddToEnd(struct Array* array, char* text) {
    int endRow = array->currentRow;
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
    array->currentRow += 1;
}


void Insert(struct Array* array, short line, short index, char* text) {
   
    char* afterIndex = strcat(text, array->rows[line].data + index);
    
    int newLength = strlen(array->rows[line].data) + strlen(afterIndex) + 1;
    char* temp = realloc(array->rows[line].data, newLength * sizeof(char));
    if (temp == NULL) {
        printf("MEMORY ERROR");
        return;
    }
    array->rows[line].data = temp;
    array->rows[line].data[index] = '\0';
    strcat(array->rows[line].data, afterIndex);
    
}


int Search(struct Array* array, char* text, int** indexes) {
    
    int lastIndex = 0;

    for (int i = 0; i <= array->currentRow; i++) {
        int textLen = strlen(text);
        int rowLen = strlen(array->rows[i].data);
        
        if (i == array->currentRow && (array->rows[array->currentRow].data == NULL || strlen(array->rows[array->currentRow].data) == 0)) {
            break;
        }
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
            else {
                printf("\n");
            }
        }
        else {
            printf("%s\n", array->rows[i].data);
        }
        
    }
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
            if (array->rows[i].data == NULL) {
                if (i >= array->currentRow) {
                    break;
                }
                else {
                    fputs("\n", file);
                }
            }
            else {
                fputs(array->rows[i].data, file);
                fputs("\n", file);
                
            }
        }
        fclose(file);
    }
}


_Bool LoadFromFile(struct Array** array, char* fileName) {
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


int main() {
    
    short command = 0;
    
    struct Array *textArray = CreateArray(10);
    
    while (1) {

        printf("\nSupported commands.");
        printf("\n1. Append to end.\n2. New line.\n3. Save to file.\n4. Load from file.\n5. Print to console.\n6. Insert.\n7. Search.\n8. Exit.\n");

        printf("\nChoose the command: ");
        scanf("%d", &command);
        while(getchar() != '\n');

        if (command == 1) {
            // ClearConsole();
            char text[100];
            printf("Enter text to append: ");
            fgets(text, sizeof(text), stdin);
            text[strcspn(text, "\n")] = '\0';
            AddToEnd(textArray, text);
        }
        else if (command == 2) {
            // ClearConsole();
            AddNewLine(textArray);
            printf("New line is started.");
        }
        else if (command == 3) {
            printf("Enter the file name for saving: ");
            char file[100];
            fgets(file, sizeof(file), stdin);
            SaveToFile(textArray, file);
            printf("Text has been saved successfully.");
        }
        else if (command == 4) {
            printf("Enter the file name for loading: ");
            char file[100];
            fgets(file, sizeof(file), stdin);
            if (LoadFromFile(&textArray, file)) {
                printf("Text has been loaded successfully.");
            }
        }
        else if (command == 5) {
            // ClearConsole();
            printf("\nText:\n");
            Print(textArray);
        }
        else if (command == 6) {
            printf("Choose line and index: ");
            short line;
            short index;
            char text[100];
            scanf("%d", &line);
            scanf("%d", &index);
            while(getchar() != '\n');
            printf("Enter text to insert: ");
            fgets(text, sizeof(text), stdin);
            text[strcspn(text, "\n")] = '\0';
            Insert(textArray, line, index, text);
        }
        else if (command == 7) {
            // ClearConsole();
            
            char text[100];
            printf("Enter text to search: ");
            fgets(text, sizeof(text), stdin);
            text[strcspn(text, "\n")] = '\0';
            
            int* textPosition = malloc(2 * sizeof(int));
            if (textPosition == NULL) {
                printf("MEMORY ERROR");
                break;
            }
            
            int lastIndex = Search(textArray, text, &textPosition);
            
            if (lastIndex == 0) {
                printf("Text is not found.");
            }
            
            else {
                printf("Text is present in this position: ");
                PrintFoundIndexes(textPosition, lastIndex);
            }
        }
        else if (command == 8) {
            // ClearConsole();
            break;
        }
        else {
            // ClearConsole();
            printf("Unknown command");
        }
    }
    
    Free(textArray);
    return 0;
}