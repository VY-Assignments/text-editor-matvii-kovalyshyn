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
        std::println("\n1. Append to end.\n2. New line.\n3. Save to file.\n4. Load from file.\n5. Print to console.\n6. Insert.\n7. Insert with replacement.\n8. Change checklist.\n9. Change contact.\n10. Search.\n11. Delete.\n12. Delete line.\n13. Copy.\n14. Paste.\n15. Cut.\n16. Undo.\n17. Redo.\n18. Cipher menu.\n0. Exit.\n");

        std::print("\nChoose the command: ");
        if(!(std::cin >> command)) {
            std::println("Unknown command. Try again.");
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (command == 1) {
            int line = textEditor->GetCurrentLine();
            if (line < 0) {
                std::println("Create a new line first.");
            }
            else {
                std::string text;
                std::print("Enter text to append: ");
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::getline(std::cin, text);

                int index = 0;

                textEditor->AddToEnd(text);
                
                Action action(AddToEndAction, text.size(), line, index, "", text);
                textEditor->HistoryPush(action);
            }
            
        }

        else if (command == 2) {
            ClearConsole();
            short lineType = 0;
            std::print("Choose line type: text (1), checklist(2), contact information (3): ");
            std::cin >> lineType;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::string lineText1 = "";
            std::string lineText2 = "";
            if (lineType == 1) {
                Line* line = new TextLine("");
                textEditor->AddLine(line);

                std::println("New line is started.");
            }
            else if (lineType == 2) {
                std::string item;
                std::print("Insert your item: ");
                
                std::getline(std::cin, item);

                Line* line = new CheckListLine(item, 0);
                textEditor->AddLine(line);

                lineText1 = item;

                std::println("New line is started.");
            }
            else if (lineType == 3) {
                std::string name;
                std::print("Insert contact name and surname: ");
                std::getline(std::cin, name);

                std::string email;
                std::print("Insert contact email: ");
                std::getline(std::cin, email);
                
                Line* line = new ContactInfoLine(name, email);
                textEditor->AddLine(line);

                lineText1 = name;
                lineText2 = email;
                
                std::println("New line is started.");
            }
            
            Action action(AddNewLineAction, lineType, 0, 0, lineText1, lineText2);
            textEditor->HistoryPush(action);
        }
        else if (command == 3) {
            ClearConsole();
            std::print("Enter the file name for saving: ");
            std::string file;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, file);
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
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, file);
            if (textEditor->LoadFromFile(file)) {
                std::println("Text has been loaded successfully.");
            }
        }
        else if (command == 5) {
            ClearConsole();
            std::println("\n-----------------------");
            textEditor->PrintAll();
            std::println("\n-----------------------");
        }
        else if (command == 6) {
            ClearConsole();
            std::print("Choose line and index: ");
            short line;
            short index;
            
            if (!(std::cin >> line >> index)) {
                std::println("Invalid input.");
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            else {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                if (textEditor->GetLineType(line) == "TextLine") {
                    std::string text;
                    std::print("Enter text to insert: ");
                    std::getline(std::cin, text);
                    if(textEditor->Insert(line, index, text)) {
                        Action action(InsertAction, text.size(), line, index, "", text);
                        textEditor->HistoryPush(action);
                    }
                    else {
                        std::println("Invalid line or index.");
                    }
                }
                else {
                    std::println("This type of line does not support insert command.");
                }   
            }
        }
        else if (command == 7) {
            ClearConsole();
            std::print("Choose line and index: ");
            short line;
            short index;
            
            if (!(std::cin >> line >> index)) {
                std::println("Invalid input.");
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            else {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                if (textEditor->GetLineType(line) == "TextLine") {
                    std::string text;
                    std::print("Enter text to insert: ");
                    std::getline(std::cin, text);

                    std::string temp = textEditor->GetLineStr(line).substr(index);
                    
                    if(textEditor->InsertWithReplacement(line, index, text)) {
                        Action action(InsertWithReplacementAction, text.size(), line, index, temp, text);
                        textEditor->HistoryPush(action);
                    }
                    else {
                        std::println("Invalid line or index.");
                    }
                }
            }
        }
        else if (command == 8) {
            ClearConsole();
            std::print("Choose checklist line: ");
            short line;
            if (!(std::cin >> line) || line > textEditor->GetCurrentLine()) {
                std::println("Invalid input.");
            }
            else {
                if (textEditor->GetLineType(line) != "CheckListLine") {
                    std::println("Choosen line is not a CheckList Line.");
                }
                else {
                    std::print("Mark/unmark (1) or change item (2): ");
                    short choice;
                    if (!(std::cin >> choice)) {
                        std::println("Invalid input.");
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                    else {
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        CheckListLine* checkListLine = dynamic_cast<CheckListLine*>(textEditor->GetLine(line));
                        if (choice == 1) {
                            checkListLine->ChangeChecked();
                        }
                        else if (choice == 2) {
                            std::string text;
                            
                            std::print("Enter new item: ");
                            std::getline(std::cin, text);
                            checkListLine->SetItem(text);
                        }   
                    }
                }
            }
        }
        else if (command == 9) {
            ClearConsole();
            std::print("Choose contact line: ");
            short line;
            if (!(std::cin >> line) || line > textEditor->GetCurrentLine()) {
                std::println("Invalid input.");
            }
            else {
                if (textEditor->GetLineType(line) != "ContactInfoLine") {
                    std::println("Choosen line is not a Contact Line.");
                }
                else {
                    std::print("To change: name(1), email(2): ");
                    short choice;
                    if (!(std::cin >> choice)) {
                        std::println("Invalid input.");
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                    else {
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::string text;
                        ContactInfoLine* contactInfoLine = dynamic_cast<ContactInfoLine*>(textEditor->GetLine(line));
                        if (choice == 1) {
                            std::print("Enter new name: ");
                            std::getline(std::cin, text);
                            contactInfoLine->SetName(text);
                        }
                        else if (choice == 2) {
                            std::print("Enter new email: ");
                            std::getline(std::cin, text);
                            contactInfoLine->SetEmail(text);
                        }
                    }
                }
            }
        }
        else if (command == 10) {
            ClearConsole();
            
            std::string text;
            std::print("Enter text to search: ");
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, text);
            
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
        else if (command == 11) {
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
                
                std::string temp = textEditor->GetLineStr(line).substr(index, symbols);

                if (textEditor->Delete(line, index, symbols)) {
                    Action action(DeleteAction, symbols, line, index, temp, "");
                    textEditor->HistoryPush(action);
                }
                else {
                    std::println("Invalid line or index.");
                }
            }
        }
        else if (command == 12) {
            ClearConsole();
            std::print("Choose line: ");
            short line;
            if (!(std::cin >> line)) {
                std::println("Invalid input.");
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            else {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::string temp = textEditor->GetLineStr(line);

                if (textEditor->DeleteLine(line)) {
                    Action action(DeleteLineAction, 0, line, 0, temp, "");
                    textEditor->HistoryPush(action);
                }
                else {
                    std::println("Invalid line or index.");
                }
            }
        }
        else if (command == 13) {
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
        else if (command == 14) {
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
                    Action action(InsertAction, textEditor->GetCopied().size(), line, index, "", textEditor->GetCopied());
                    textEditor->HistoryPush(action);
                }
                else {
                    std::println("Invalid line or index.");
                }
            }
        }
        else if (command == 15) {
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
                
                std::string temp = textEditor->GetLineStr(line).substr(index);
                
                if (textEditor->Cut(line, index, symbols)) {
                    Action action(CutAction, symbols, line, index, temp, textEditor->GetCopied());
                    textEditor->HistoryPush(action);                    
                }
                else {
                    std::println("Invalid line or index.");
                }
            }
        }
        else if (command == 16) {
            ClearConsole();
            textEditor->Undo();
        }
        else if (command == 17) {
            ClearConsole();
            textEditor->Redo();
        }
        else if (command == 18) {
            while (1) {
                std::println("\nCipher menu.\n1. Encrypt the whole text.\n2. Decrypt the whole text.\n3. Save text.\n4. Load text.\n5. Print text.\n0. Exit");
                std::print("\nChoose the command: ");
                short cipherCommand = 0;
                if(!(std::cin >> cipherCommand)) {
                    std::println("Unknown command. Try again.");
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    continue;
                }

                if (cipherCommand == 1) {
                    ClearConsole();
                    int userCipher = 0;
                    std::print("\nChoose a cipher: caeser(1) or vigenere(2): ");
                    if(!(std::cin >> userCipher)) {
                        std::println("Invalid input.");
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        continue;
                    }
                    
                    if (userCipher == 1) {
                        int key = 0;
                        std::print("\nEnter a cipher key: ");
                        if(!(std::cin >> key)) {
                            std::println("Invalid input.");
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            continue;
                        }
                        textEditor->SerializeAll(key);
                    }
                    else if (userCipher == 2) {
                        std::string key = "a";
                        std::print("\nEnter a vigenere key: ");
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::getline(std::cin, key);
                        
                        textEditor->SerializeAll(0, key);
                    }
                    
                    
                }
                else if (cipherCommand == 2) {
                    ClearConsole();
                    ClearConsole();
                    int userCipher = 0;
                    std::print("\nChoose a cipher: caeser(1) or vigenere(2): ");
                    if(!(std::cin >> userCipher)) {
                        std::println("Invalid input.");
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        continue;
                    }
                    
                    if (userCipher == 1) {
                        int key = 0;
                        std::print("\nEnter a cipher key: ");
                        if(!(std::cin >> key)) {
                            std::println("Invalid input.");
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            continue;
                        }
                        textEditor->DeserializeAll(key);
                    }
                    else if (userCipher == 2) {
                        std::string key = "a";
                        std::print("\nEnter a vigenere key: ");
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::getline(std::cin, key);
                        
                        textEditor->DeserializeAll(0, key);
                    }

                }
                else if (cipherCommand == 3) {
                    ClearConsole();
                    std::print("Enter the file name for saving: ");
                    std::string file;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::getline(std::cin, file);
                    if (file.size() == 0 || file[0] == '\n') {
                        std::println("Incorrect file name.");
                    }
                    else {
                        textEditor->SaveToFile(file);
                        std::println("Text has been saved successfully.");
                    }            
                }
                else if (cipherCommand == 4) {
                    ClearConsole();
                    std::print("Enter the file name for loading: ");
                    std::string file;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::getline(std::cin, file);
                    if (textEditor->LoadFromFile(file)) {
                        std::println("Text has been loaded successfully.");
                    }
                }
                else if (cipherCommand == 5) {
                    ClearConsole();
                    std::println("\n-----------------------");
                    textEditor->PrintAll();
                    std::println("\n-----------------------");
                }
                else if (cipherCommand == 0) {
                    ClearConsole();
                    break;
                }
            }
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
    delete textEditor;
    return 0;
}