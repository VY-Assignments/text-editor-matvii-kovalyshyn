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
            int capacity = 100;
            int length = 0;
            printf("Enter text to append: ");
            char *text = malloc(capacity * sizeof(char));
            if (text == NULL) {
                printf("MEMORY ERROR");
            }
            char character;
            while(scanf("%c", &character) == 1 && character != '\n') {
                if (length + 1 >= capacity) {
                    capacity *= 2;
                    char* temp = realloc(text, capacity * sizeof(char));
                    if (temp == NULL) {
                        printf("MEMORY ERROR");
                        break;
                    }
                    text = temp;
                }
                text[length] = character;
                length++;
            }
            text[length] = '\0';
            int line = textArray->currentRow;
            int index = 0;
            if (textArray->rows[line].data != NULL) {
                index = strlen(textArray->rows[line].data);
            }
            AddToEnd(textArray, text);
            struct ActionInfo* actionInfo = CreateActionInfo(strlen(text), line, index, "");
            HistoryPush(textArray, AddToEndAction, actionInfo, 1);
        }
        else if (command == 2) {
            ClearConsole();
            AddNewLine(textArray);
            printf("New line is started.");
            struct ActionInfo* actionInfo = CreateActionInfo(0, 0, 0, "");
            HistoryPush(textArray, AddNewLineAction, actionInfo, 1);
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
                while(1) {
                    printf("Enter text to insert: ");
                    if (fgets(text, sizeof(text), stdin) == NULL) {
                        printf("Invalid input.");
                        break;
                    }

                    text[strcspn(text, "\n")] = '\0';

                    if (strlen(text) > 0) {
                        if(Insert(textArray, line, index, text)) {
                            struct ActionInfo* actionInfo = CreateActionInfo(strlen(text), line, index, "");
                            HistoryPush(textArray, InsertAction, actionInfo, 1);    
                            break;
                        }
                        else {
                            printf("Invalid line or index.");
                            break;
                        }
                    }            
                }
            }
        }
        else if (command == 7) {
            ClearConsole();
            printf("Choose line and index: ");
            short line;
            short index;
            char text[100];
            if (scanf("%d %d", &line, &index) != 2 || line < 0 || index < 0 ) {
                printf("Invalid input.");
                while(getchar() != '\n');
            }
            else {
                while(getchar() != '\n');
                while(1) {
                    printf("Enter text to insert: ");
                    if(fgets(text, sizeof(text), stdin) == NULL) {
                        printf("Invalid input.");
                        break;
                    }

                    text[strcspn(text, "\n")] = '\0';
                    if (strlen(text) > 0) {
                        char* temp = NULL;
                        if (textArray->rows[line].data + index != NULL && strlen(textArray->rows[line].data + index) > 0) {
                            char* temp = malloc((strlen(text) + 1) * sizeof(char));
                            if (temp == NULL) {
                                printf("MEMORY ERROR");
                            }
                            strcpy(temp, textArray->rows[line].data + index);
                            if (temp[strlen(text) - 1] == '\n') {
                                temp[strlen(text) - 1] = '\0';
                            }
                        }
                        
                        if(InsertWithReplacement(textArray, line, index, text)) {
                            struct ActionInfo* actionInfo = CreateActionInfo(strlen(text), line, index, temp);    
                            HistoryPush(textArray, InsertWithReplacementAction, actionInfo, 1);    
                            break;
                        }
                        else {
                            printf("Invalid line or index.");
                            break;
                        }
                        
                        
                    }
                    
                }
                
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
            if (scanf("%d %d %d", &line, &index, &symbols) != 3 || line < 0 || index < 0 || symbols < 0 || textArray->rows[line].data == NULL || strlen(textArray->rows[line].data) < index) {
                printf("Invalid input.");
                while(getchar() != '\n');
            }
            else {
                while(getchar() != '\n');
                char* temp = NULL;
                if (textArray->rows[line].data + index != NULL && strlen(textArray->rows[line].data + index) > 0) {
                    temp = malloc((symbols + 1) * sizeof(char));
                    if (temp == NULL) {
                        printf("MEMORY ERROR\n");
                    }
                    strncpy(temp, textArray->rows[line].data + index, symbols);
                    temp[symbols] = '\0';
                    if (symbols > 0 && temp[strlen(temp) - 1] == '\n') {
                        temp[strlen(temp) - 1] = '\0';
                        
                    }
                }
                if (Delete(textArray, line, index, symbols)) {
                    struct ActionInfo* actionInfo = CreateActionInfo(symbols, line, index, temp);
                    HistoryPush(textArray, DeleteAction, actionInfo, 1);
                }
                else {
                    printf("Invalid line or index.");
                }

                free(temp);
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
                if (Paste(textArray, line, index)) {
                    struct ActionInfo* actionInfo = CreateActionInfo(strlen(textArray->copied), line, index, "");
                    HistoryPush(textArray, InsertAction, actionInfo, 1);
                }
                else {
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
            if (scanf("%d %d %d", &line, &index, &symbols) != 3 || line < 0 || index < 0 || symbols < 0 || textArray->rows[line].data == NULL || strlen(textArray->rows[line].data) < index) {
                printf("Invalid input.");
                while(getchar() != '\n');
            }
            else {
                while(getchar() != '\n');
                char* temp = NULL;
                if (textArray->rows[line].data + index != NULL && strlen(textArray->rows[line].data + index) > 0) {
                    temp = malloc((symbols + 1) * sizeof(char));
                    if (temp == NULL) {
                        printf("MEMORY ERROR");
                    }
                    strcpy(temp, textArray->rows[line].data + index);
                    if (temp[strlen(temp) - 1] == '\n') {
                        temp[strlen(temp) - 1] = '\0';
                    }
                }
                
                
                if (Cut(textArray, line, index, symbols)) {
                    struct ActionInfo* actionInfo = CreateActionInfo(symbols, line, index, temp);
                    HistoryPush(textArray, CutAction, actionInfo, 1);    
                }
                else {
                    printf("Invalid line or index.");
                }

                 
            }
        }
        else if (command == 13) {
            ClearConsole();
            Undo(textArray);
        }
        else if (command == 14) {
            ClearConsole();
            Redo(textArray);
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