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


void Search(struct Array* array, char* text, int* index) {
    for (int i = 0; i <= array->currentRow; i++) {
        if (i == array->currentRow && array->rows[array->currentRow].data == NULL) {
            break;
        }
        char* textIndex = strstr(array->rows[i].data, text);
        if (textIndex != NULL) {
            index[0] = i;
            index[1] = textIndex - array->rows[i].data;
            return;
        }
    }
    index[0] = -1;
    index[1] = -1;
}

void SearchAll(struct Array* array, char* text) {

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


void ClearConsole() {
    // \e[1J — очищає екран від курсора вгору
    // \e[H  — повертає курсор у лівий верхній кут (0,0)
    printf("\e[1J\e[H");
}


int main() {
    
    int command = 0;
    struct Array *textArray = CreateArray(10);
    
    while (1) {

        printf("\nSupported commands.");
        printf("\n1. Append to end.\n2. New line.\n3. Save to file.\n4. Load from file.\n5. Print to console.\n6. Insert.\n7. Search.\n8. Exit.\n");

        printf("\nChoose the command: ");
        scanf("%d", &command);
        while(getchar() != '\n');

        if (command == 1) {
            ClearConsole();
            char text[100];
            printf("Enter text to append: ");
            fgets(text, sizeof(text), stdin);
            text[strcspn(text, "\n")] = '\0';
            AddToEnd(textArray, text);
        }
        else if (command == 2) {
            ClearConsole();
            AddNewLine(textArray);
            printf("New line is started.");
        }
        else if (command == 5) {
            ClearConsole();
            printf("\nText:\n");
            Print(textArray);
        }
        else if (command == 7) {
            ClearConsole();
            char text[100];
            printf("Enter text to search: ");
            fgets(text, sizeof(text), stdin);
            text[strcspn(text, "\n")] = '\0';
            int textPosition[2];
            
            Search(textArray, text, textPosition);
            if (textPosition[0] == -1 || textPosition[1] == -1) {
                printf("Text is not found.");
            }
            else {
                printf("Text is present in this position: %d %d.", textPosition[0], textPosition[1]);
            }
        }
        else if (command == 8) {
            ClearConsole();
            break;
        }
        else {
            ClearConsole();
            printf("Unknown command");
        }
    }
    
    Free(textArray);
    return 0;
}