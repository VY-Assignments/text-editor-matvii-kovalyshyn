#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct Row {
    char* data;
    int length;
};


struct Array {
    struct Row* rows;
    int rowsCount;
};


struct Row* CreateRows(int rowsCount, int rowsLength) {
    struct Row* rows = malloc(rowsCount * sizeof(struct Row));
    if (rows == NULL) {
        return NULL;
    }
    for (int i = 0; i < rowsCount; i++)
    {
        rows[i].data = malloc(rowsLength * sizeof(char));
        rows[i].length = rowsLength;
    }
    return rows;
}


struct Array* CreateArray(int rowsCount, int rowsLength) {
    struct Array *array = malloc(sizeof(struct Array));
    if (array == NULL) {
        return NULL;
    }
    array->rows = CreateRows(rowsCount, rowsLength);
    array->rowsCount = rowsCount;
    return array;
}


void Free(struct Array* array) {
    for (int i = 0; i < array->rowsCount; i++) {
        free(array->rows[i].data);
    }
    free(array->rows);
    free(array);
}


int main() {
    
    char input[100];
    int startRows = 10;
    int startColumns = 100;
    struct Array *text = CreateArray(startRows, startColumns);
    
    while (1) {
        printf("\nSupported commands.");
        printf("\n1. Append to end.\n2. New line.\n3. Save to file.\n4. Load from file.\n5. Print to console.\n6. Insert.\n7. Search.\n8. Exit.\n");


        printf("\nChoose the command: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "1") == 0) {
            printf("Enter text to append: ");
            fgets(input, sizeof(input), stdin);
            
            
        }
        else if (strcmp(input, "5") == 0) {
            PrintArray(text, 5);
        }
        else if (strcmp(input, "8") == 0) {
            break;
        }
        else {
            printf("Unknown command");
        }
    }
    
    Free(text);
    return 0;
}