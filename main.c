#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct Row {
    char* data;
};


struct Array {
    struct Row* rows;
    int rowsCount;
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
    return array;
}


int FindLastRow(struct Array* array) {
    if (array->rows[0].data == NULL || strlen(array->rows[0].data) == 0) {
        return 0;
    }

    for (int i = 0; i < array->rowsCount; i++) {
        if (array->rows[i].data == NULL || strlen(array->rows[i].data) == 0) {
            return i - 1;
        }
    }
    
    return -1;
}


void AddToEnd(struct Array* array, char* text) {
    int endRow = FindLastRow(array);
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


void Print(struct Array* array) {
    for (int i = 0; i < array->rowsCount; i++) {
        if (array->rows[i].data == NULL) {
            break;
        }
        printf("%s\n", array->rows[i].data);
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
    
    char input[256];
    struct Array *textArray = CreateArray(10);
    // textArray->rows[0].data = "123";
    
    while (1) {

        printf("\nSupported commands.");
        printf("\n1. Append to end.\n2. New line.\n3. Save to file.\n4. Load from file.\n5. Print to console.\n6. Insert.\n7. Search.\n8. Exit.\n");

        printf("\nChoose the command: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "1") == 0) {
            // ClearConsole();
            char text[100];
            printf("Enter text to append: ");
            fgets(text, sizeof(text), stdin);
            text[strcspn(text, "\n")] = '\0';
            AddToEnd(textArray, text);
        }
        else if (strcmp(input, "5") == 0) {
            // ClearConsole();
            printf("Text:\n");
            Print(textArray);
        }
        else if (strcmp(input, "8") == 0) {
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