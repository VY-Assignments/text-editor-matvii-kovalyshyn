#include <iostream>
#include <print>
#include <limits>
#include <vector>
#include "text_editor.hpp"

void ClearConsole() {
    // \e[1J — очищає екран від курсора вгору
    // \e[H  — повертає курсор у лівий верхній кут (0,0)
    // printf("\e[1J\e[H");
}

int main() {

    short command = 0;
    TextEditor* textEditor = new TextEditor();

    while (1) {
        ClearConsole();
        std::println("\nSupported commands.");
        std::println("\n1. Append to end.\n2. New line.\n3. Save to file.\n4. Load from file.\n5. Print to console.\n6. Insert.\n7. Insert with replacement.\n8. Search.\n9. Delete.\n10. Copy.\n11. Paste.\n12. Cut.\n13. Undo.\n14. Redo.\n0. Exit.\n");

        std::print("\nChoose the command: ");
        std::cin >> command;

        if (command == 1) {
            std::string text;
            std::print("Enter text to append: ");
            std::cin >> text;
            
            int line = textEditor->GetCurrentLine();
            int index = 0;

            textEditor->AddToEnd(text);
            
            Action action(AddToEndAction, text.size(), line, index, "");
            textEditor->HistoryPush(action);
        }

        else if (command == 2) {
            ClearConsole();
            short lineType = 0;
            std::print("Choose line type: text (1), checklist(2), contact information (3): ");
            std::cin >> lineType;
            if (lineType == 1) {
                Line* line = new TextLine("");
                textEditor->AddLine(line);
            }
            else if (lineType == 2) {
                std::string item;
                std::print("Insert your item: ");
                std::cin >> item;

                Line* line = new CheckListLine(item, 0);
                textEditor->AddLine(line);
            }
            else if (lineType == 3) {
                std::string name;
                std::print("Insert contact name and surname: ");
                std::cin >> name;

                std::string email;
                std::print("Insert contact email: ");
                std::cin >> email;
                
                Line* line = new ContactInfoLine(name, email);
                textEditor->AddLine(line);
            }
            
            std::println("New line is started.");
            
            Action action(AddNewLineAction, 0, 0, 0, "");
            textEditor->HistoryPush(action);
        }
        else if (command == 3) {
            ClearConsole();
            std::print("Enter the file name for saving: ");
            std::string file;
            std::cin >> file;
            if (file.size() == 0 || file[0] == '\n') {
                std::println("Incorrect file name.");
            }
            else {
                textEditor->SaveToFile(file);
                std::println("Text has been saved successfully.");
            }            
        }
        else if (command == 4) {
            ClearConsole();
            std::print("Enter the file name for loading: ");
            std::string file;
            std::cin >> file;
            if (textEditor->LoadFromFile(file)) {
                std::println("Text has been loaded successfully.");
            }
        }
        else if (command == 5) {
            ClearConsole();
            std::println("\nText:");
            textEditor->PrintAll();
            std::println();
        }
        else if (command == 6) {
            ClearConsole();
            std::print("Choose line and index: ");
            short line;
            short index;
            std::string text;
            if (!(std::cin >> line >> index)) {
                std::println("Invalid input.");
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            else {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                std::print("Enter text to insert: ");
                std::cin >> text;

                if(textEditor->Insert(line, index, text)) {
                    Action action(InsertAction, text.size(), line, index, "");
                    textEditor->HistoryPush(action);
                    break;
                }
                else {
                    std::println("Invalid line or index.");
                    break;
                }
                       
            }
            
        }
        else if (command == 7) {
            ClearConsole();
            std::println("Choose line and index: ");
            short line;
            short index;
            std::string text;
            if (!(std::cin >> line >> index)) {
                std::println("Invalid input.");
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            else {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                std::print("Enter text to insert: ");
                std::cin >> text;

                std::string temp = textEditor->GetLine(line).substr(index);
                
                if(textEditor->InsertWithReplacement(line, index, text)) {
                    Action action(InsertWithReplacementAction, text.size(), line, index, temp);
                    textEditor->HistoryPush(action);
                    break;
                }
                else {
                    std::println("Invalid line or index.");
                    break;
                }
            }
        }
        else if (command == 8) {
            ClearConsole();
            
            std::string text;
            std::print("Enter text to search: ");
            std::cin >> text;
            
            std::vector<int> indexes = textEditor->Search(text);
            
            if (indexes.size() == 0) {
                printf("Text is not found.");
            }
            
            else {
                printf("Text is present in this position: ");
                for (size_t i = 0; i < indexes.size(); i+=2) {
                    std::println("({}, {})", indexes[i], indexes[i + 1]);
                }
            }
        }
        else if (command == 9) {
            ClearConsole();
            std::print("Choose line, index and number of symbols: ");
            short line;
            short index;
            int symbols;
            if (!(std::cin >> line >> index >> symbols)) {
                std::println("Invalid input.");
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            else {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                std::string temp = textEditor->GetLine(line).substr(index, symbols);

                if (textEditor->Delete(line, index, symbols)) {
                    Action action(DeleteAction, symbols, line, index, temp);
                    textEditor->HistoryPush(action);
                }
                else {
                    std::println("Invalid line or index.");
                }
            }
        }
        else if (command == 10) {
            ClearConsole();
            std::print("Choose line, index and number of symbols: ");
            short line;
            short index;
            int symbols;
            if (!(std::cin >> line >> index >> symbols)) {
                std::println("Invalid input.");
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            else {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (!textEditor->Copy(line, index, symbols)) {
                    std::println("Invalid line or index.");
                }
            }
        }
        else if (command == 11) {
            ClearConsole();
            std::print("Choose line, index: ");
            short line;
            short index;
            if (!(std::cin >> line >> index)) {
                std::println("Invalid input.");
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            else {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (textEditor->Paste(line, index)) {
                    Action action(InsertAction, textEditor->GetCopied().size(), line, index, "");
                    textEditor->HistoryPush(action);
                }
                else {
                    std::println("Invalid line or index.");
                }
            }
        }
        else if (command == 12) {
            ClearConsole();
            std::print("Choose line, index and number of symbols: ");
            short line;
            short index;
            int symbols;
            if (!(std::cin >> line >> index >> symbols)) {
                std::println("Invalid input.");
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            else {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                std::string temp = textEditor->GetLine(line).substr(index);
                
                if (textEditor->Cut(line, index, symbols)) {
                    Action action(CutAction, symbols, line, index, temp);
                    textEditor->HistoryPush(action);                    
                }
                else {
                    std::println("Invalid line or index.");
                }
            }
        }
        else if (command == 13) {
            ClearConsole();
            textEditor->Undo();
        }
        else if (command == 14) {
            ClearConsole();
            textEditor->Redo();
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


    return 0;
}