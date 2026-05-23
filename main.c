#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char** SelectMemory(int size, int size2) {
    char **array = malloc(size * sizeof(char*));
    for (int i = 0; i < size; i++)
    {
        array[i] = malloc(size2 * sizeof(char));
    }
    return array;
}

void FillArray(char **array, int size, int size2) {
    for (int i = 0; i < size; i++) {
        if (array[i + 1] == NULL) {

        }
    }
}

void Free(char **array, int size) {
    for (int i = 0; i < size; i++) {
        free(array[i]);
    }
    free(array);
}

void PrintArray(char **array, int size) {
    for (int i = 0; i < size; i++) {
        printf("%s ", array[i]);
        printf("\n");
    }
}

int main() {
    
    char input[100];
    char **text;
    
    while (1) {
        printf("\nSupported commands.");
        printf("\n1. Append to end.\n2. New line.\n3. Save to file.\n4. Load from file.\n5. Print to console.\n6. Insert.\n7. Search.\n8. Exit.\n");


        printf("\nChoose the command: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "1") == 0) {
            printf("Enter text to append: ");
            fgets(input, sizeof(input), stdin);
            text = SelectMemory(5, 10);
            
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
    
    free(text);
    return 0;
}