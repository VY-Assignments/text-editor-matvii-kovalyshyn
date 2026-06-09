#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "text_editor.h"


int main() {
    
    short command = 0;
    
    struct Array *textArray = CreateArray(10);
    
    while (1) {
        printf("\nSupported commands.");
        printf("\n1. Append to end.\n2. New line.\n3. Save to file.\n4. Load from file.\n5. Print to console.\n6. Insert.\n7. Insert with replacement.\n8. Search.\n9. Delete.\n10. Copy.\n11. Paste.\n12. Cut.\n13. Undo.\n14. Redo.\n0. Exit.\n");

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
            struct ActionInfo* actionInfo = CreateActionInfo(strlen(text), 0, 0, "");
            HistoryPush(textArray, AddToEndAction, actionInfo);
        }
        else if (command == 2) {
            ClearConsole();
            AddNewLine(textArray);
            printf("New line is started.");
            struct ActionInfo* actionInfo = CreateActionInfo(0, 0, 0, "");
            HistoryPush(textArray, AddNewLineAction, actionInfo);
        }
        else if (command == 3) {
            ClearConsole();
            printf("Enter the file name for saving: ");
            char file[100];
            fgets(file, sizeof(file), stdin);
            if (file == NULL || strlen(file) == 0 || file[0] == '\n') {
                printf("Incorrect file name.");
            }
            else {
                SaveToFile(textArray, file);
                printf("Text has been saved successfully.");
            }            
        }
        else if (command == 4) {
            ClearConsole();
            printf("Enter the file name for loading: ");
            char file[100];
            fgets(file, sizeof(file), stdin);
            if (LoadFromFile(&textArray, file)) {
                printf("Text has been loaded successfully.");
            }
        }
        else if (command == 5) {
            ClearConsole();
            printf("\nText:\n");
            Print(textArray);
            printf("\n");
        }
        else if (command == 6) {
            ClearConsole();
            printf("Choose line and index: ");
            short line;
            short index;
            char text[100];
            if (scanf("%d %d", &line, &index) != 2 || line < 0 || index < 0) {
                printf("Invalid input.");
                while(getchar() != '\n');
            }
            else {
                while(getchar() != '\n');
                printf("Enter text to insert: ");
                fgets(text, sizeof(text), stdin);
                text[strcspn(text, "\n")] = '\0';
                if(!Insert(textArray, line, index, text)) {
                    printf("Invalid line or index.");
                }
                struct ActionInfo* actionInfo = CreateActionInfo(strlen(text), line, index, "");
                HistoryPush(textArray, InsertAction, actionInfo);    
            }
        }
        else if (command == 7) {
            ClearConsole();
            printf("Choose line and index: ");
            short line;
            short index;
            char text[100];
            if (scanf("%d %d", &line, &index) != 2 || line < 0 || index < 0) {
                printf("Invalid input.");
                while(getchar() != '\n');
            }
            else {
                while(getchar() != '\n');
                printf("Enter text to insert: ");
                fgets(text, sizeof(text), stdin);
                text[strcspn(text, "\n")] = '\0';
                if(!InsertWithReplacement(textArray, line, index, text)) {
                    printf("Invalid line or index.");
                }
                char* temp = malloc(strlen(text) * sizeof(char));
                if (temp == NULL) {
                    printf("MEMORY ERROR");
                }
                strcpy(temp, textArray->rows[line].data + index);
                temp[strlen(text) - 1] = '\0'; 
                struct ActionInfo* actionInfo = CreateActionInfo(strlen(text), line, index, temp);    
                HistoryPush(textArray, InsertWithReplacementAction, actionInfo);
            }
        }
        else if (command == 8) {
            ClearConsole();
            
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

            free(textPosition);
        }
        else if (command == 9) {
            ClearConsole();
            printf("Choose line, index and number of symbols: ");
            short line;
            short index;
            int symbols;
            if (scanf("%d %d %d", &line, &index, &symbols) != 3 || line < 0 || index < 0 || symbols < 0) {
                printf("Invalid input.");
                while(getchar() != '\n');
            }
            else {
                while(getchar() != '\n');
                if (!Delete(textArray, line, index, symbols)) {
                    printf("Invalid line or index.");
                }
            }

        }
        else if (command == 10) {
            ClearConsole();
            printf("Choose line, index and number of symbols: ");
            short line;
            short index;
            int symbols;
            if (scanf("%d %d %d", &line, &index, &symbols) != 3 || line < 0 || index < 0 || symbols < 0) {
                printf("Invalid input.");
                while(getchar() != '\n');
            }
            else {
                while(getchar() != '\n');
                if (!Copy(textArray, line, index, symbols)) {
                    printf("Invalid line or index.");
                }
            }
        }
        else if (command == 11) {
            ClearConsole();
            printf("Choose line, index: ");
            short line;
            short index;
            if (scanf("%d %d", &line, &index) != 2 || line < 0 || index < 0) {
                printf("Invalid input.");
                while(getchar() != '\n');
            }
            else {
                while(getchar() != '\n');
                if (!Paste(textArray, line, index)) {
                    printf("Invalid line or index.");
                }
            }
        }
        else if (command == 12) {
            ClearConsole();
            printf("Choose line, index and number of symbols: ");
            short line;
            short index;
            int symbols;
            if (scanf("%d %d %d", &line, &index, &symbols) != 3 || line < 0 || index < 0 || symbols < 0) {
                printf("Invalid input.");
                while(getchar() != '\n');
            }
            else {
                while(getchar() != '\n');
                if (!Cut(textArray, line, index, symbols)) {
                    printf("Invalid line or index.");
                }
            }
        }
        else if (command == 13) {
            ClearConsole();
            Undo(textArray);
        }
        else if (command == 0) {
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