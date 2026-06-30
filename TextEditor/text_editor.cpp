#include <iostream>
#include <string>
#include <print>
#include <vector>
#include <fstream>
#include <sstream>
#include <dlfcn.h>
#include "text_editor.hpp"
#include "../Encryption/cipher_api.h"

typedef void* cipher_t;
typedef cipher_t* (*create_caesar_fn)(int);
typedef cipher_t* (*create_vigenere_fn)(const char*);
typedef char* (*encrypt_fn)(cipher_t*, const char*);
typedef char* (*decrypt_fn)(cipher_t*, const char*);
typedef void (*destroy_fn)(cipher_t*);
typedef void (*free_fn)(char*);



Action::Action(ActionType actionType, int amount, int line, int index, std::string prevText, std::string newText) {
    this->actionType = actionType;
    this->amount = amount;
    this->line = line;
    this->index = index;
    this->prevText = prevText;
    this->newText = newText;
}

HistoryNode::HistoryNode(const Action& action) : action(action) {}


TextLine::TextLine(const std::string& text) {
    prefix = "Text:";
    this->text = text;
}

void TextLine::Print() const {
    std::println("{}", GetFormatted());
}

std::string TextLine::ToStr() const {
    return text;
}

std::string TextLine::GetText() const {
    return text;
}

std::string TextLine::GetPrefix() const {
    return prefix;
}

std::string TextLine::GetFormatted() const {
    return std::format("{} {}", prefix, text);
}

void TextLine::SetText(const std::string& text) {
    this->text = text;
}

void TextLine::Serialize(int ckey, std::string vkey) {
    void* handle = dlopen("Encryption/libcipher.so", RTLD_LAZY);
    if (!handle) {
        std::cerr << "Error library loading: " << dlerror() << std::endl;
        return;
    }

    auto cipher_create_caesar = (create_caesar_fn)dlsym(handle, "cipher_create_caesar");
    auto cipher_create_vigenere = (create_vigenere_fn)dlsym(handle, "cipher_create_vigenere");
    auto cipher_encrypt = (encrypt_fn)dlsym(handle, "cipher_encrypt");
    auto cipher_decrypt = (decrypt_fn)dlsym(handle, "cipher_decrypt");
    auto cipher_destroy = (destroy_fn)dlsym(handle, "cipher_destroy");
    auto cipher_free = (free_fn)dlsym(handle, "cipher_free");

    if (!cipher_create_caesar || !cipher_create_vigenere || !cipher_encrypt || 
        !cipher_decrypt || !cipher_destroy || !cipher_free) {
        std::cerr << "Functions are not found." << std::endl;
        dlclose(handle);
        return;
    }
    
    
    cipher_t* caesar = cipher_create_caesar(ckey);
    
    char* raw_encrypted_text_c = cipher_encrypt(caesar, GetText().c_str());

    std::string encrypted_text_c(raw_encrypted_text_c);
    SetText(encrypted_text_c);

    delete[] raw_encrypted_text_c;

    cipher_t* vigenere = cipher_create_vigenere(vkey.c_str());

    char* raw_encrypted_text_v = cipher_encrypt(vigenere, GetText().c_str());

    std::string encrypted_text_v(raw_encrypted_text_v);
    SetText(encrypted_text_v);

    delete[] raw_encrypted_text_v;
    
    cipher_destroy(caesar);
    cipher_destroy(vigenere);

    dlclose(handle);
}

void TextLine::Deserialize(int ckey, std::string vkey) {
    void* handle = dlopen("Encryption/libcipher.so", RTLD_LAZY);
    if (!handle) {
        std::cerr << "Error library loading: " << dlerror() << std::endl;
        return;
    }

    auto cipher_create_caesar = (create_caesar_fn)dlsym(handle, "cipher_create_caesar");
    auto cipher_create_vigenere = (create_vigenere_fn)dlsym(handle, "cipher_create_vigenere");
    auto cipher_encrypt = (encrypt_fn)dlsym(handle, "cipher_encrypt");
    auto cipher_decrypt = (decrypt_fn)dlsym(handle, "cipher_decrypt");
    auto cipher_destroy = (destroy_fn)dlsym(handle, "cipher_destroy");
    auto cipher_free = (free_fn)dlsym(handle, "cipher_free");

    if (!cipher_create_caesar || !cipher_create_vigenere || !cipher_encrypt || 
        !cipher_decrypt || !cipher_destroy || !cipher_free) {
        std::cerr << "Functions are not found." << std::endl;
        dlclose(handle);
        return;
    }
    
    
    cipher_t* caesar = cipher_create_caesar(ckey);

    char* raw_decrypted_text_c = cipher_decrypt(caesar, GetText().c_str());
    
    std::string decrypted_text_c(raw_decrypted_text_c);
    SetText(decrypted_text_c);

    delete[] raw_decrypted_text_c;

    cipher_t* vigenere = cipher_create_vigenere(vkey.c_str());

    char* raw_decrypted_text_v = cipher_decrypt(vigenere, GetText().c_str());

    std::string decrypted_text_v(raw_decrypted_text_v);
    SetText(decrypted_text_v);

    delete[] raw_decrypted_text_v;
    
    cipher_destroy(caesar);
    cipher_destroy(vigenere);

    dlclose(handle);
}


CheckListLine::CheckListLine(const std::string& item, bool checked) {
    this->item = item;
    this->checked = checked;
    if (checked == 0) {
        prefix = "[ ]";
    }
    else {
        prefix = "[x]";
    }
}

std::string CheckListLine::GetItem() const {
    return item;
} 

void CheckListLine::SetItem(std::string item) {
    this->item = item;
}

void CheckListLine::ChangeChecked() {
    if (checked == 0) {
        checked = 1;
        prefix = "[x]";
    }
    else {
        checked = 0;
        prefix = "[ ]";
    }
}

void CheckListLine::Print() const {
    std::println("{}", GetFormatted());
}

std::string CheckListLine::ToStr() const {
    return item;
}

std::string CheckListLine::GetPrefix() const {
    return prefix;
}

std::string CheckListLine::GetFormatted() const {
    return std::format("{} {}", prefix, item);
}

void CheckListLine::Serialize(int ckey, std::string vkey) {
    void* handle = dlopen("Encryption/libcipher.so", RTLD_LAZY);
    if (!handle) {
        std::cerr << "Error library loading: " << dlerror() << std::endl;
        return;
    }

    auto cipher_create_caesar = (create_caesar_fn)dlsym(handle, "cipher_create_caesar");
    auto cipher_create_vigenere = (create_vigenere_fn)dlsym(handle, "cipher_create_vigenere");
    auto cipher_encrypt = (encrypt_fn)dlsym(handle, "cipher_encrypt");
    auto cipher_decrypt = (decrypt_fn)dlsym(handle, "cipher_decrypt");
    auto cipher_destroy = (destroy_fn)dlsym(handle, "cipher_destroy");
    auto cipher_free = (free_fn)dlsym(handle, "cipher_free");

    if (!cipher_create_caesar || !cipher_create_vigenere || !cipher_encrypt || 
        !cipher_decrypt || !cipher_destroy || !cipher_free) {
        std::cerr << "Functions are not found." << std::endl;
        dlclose(handle);
        return;
    }
    
    
    cipher_t* caesar = cipher_create_caesar(ckey);
    
    char* raw_encrypted_item_c = cipher_encrypt(caesar, GetItem().c_str());

    std::string encrypted_item_c(raw_encrypted_item_c);
    SetItem(encrypted_item_c);
    
    delete[] raw_encrypted_item_c;

    cipher_t* vigenere = cipher_create_vigenere(vkey.c_str());

    char* raw_encrypted_item_v = cipher_encrypt(vigenere, GetItem().c_str());

    std::string encrypted_item_v(raw_encrypted_item_v);
    SetItem(encrypted_item_v);

    delete[] raw_encrypted_item_v;
    
    cipher_destroy(caesar);
    cipher_destroy(vigenere);

    dlclose(handle);
}

void CheckListLine::Deserialize(int ckey, std::string vkey) {
    void* handle = dlopen("Encryption/libcipher.so", RTLD_LAZY);
    if (!handle) {
        std::cerr << "Error library loading: " << dlerror() << std::endl;
        return;
    }

    auto cipher_create_caesar = (create_caesar_fn)dlsym(handle, "cipher_create_caesar");
    auto cipher_create_vigenere = (create_vigenere_fn)dlsym(handle, "cipher_create_vigenere");
    auto cipher_encrypt = (encrypt_fn)dlsym(handle, "cipher_encrypt");
    auto cipher_decrypt = (decrypt_fn)dlsym(handle, "cipher_decrypt");
    auto cipher_destroy = (destroy_fn)dlsym(handle, "cipher_destroy");
    auto cipher_free = (free_fn)dlsym(handle, "cipher_free");

    if (!cipher_create_caesar || !cipher_create_vigenere || !cipher_encrypt || 
        !cipher_decrypt || !cipher_destroy || !cipher_free) {
        std::cerr << "Functions are not found." << std::endl;
        dlclose(handle);
        return;
    }
    
    
    cipher_t* caesar = cipher_create_caesar(ckey);
    
    char* raw_decrypted_item_c = cipher_decrypt(caesar, GetItem().c_str());

    std::string decrypted_item_c(raw_decrypted_item_c);
    SetItem(decrypted_item_c);

    delete[] raw_decrypted_item_c;

    cipher_t* vigenere = cipher_create_vigenere(vkey.c_str());

    char* raw_decrypted_item_v = cipher_decrypt(vigenere, GetItem().c_str());

    std::string decrypted_item_v(raw_decrypted_item_v);
    SetItem(decrypted_item_v);

    delete[] raw_decrypted_item_v;
    
    cipher_destroy(caesar);
    cipher_destroy(vigenere);

    dlclose(handle);
}


ContactInfoLine::ContactInfoLine(const std::string& name, const std::string& email) {
    prefix = "Contact -";
    this->name = name;
    this->email = email;
}

std::string ContactInfoLine::GetName() const {
    return name;
}
std::string ContactInfoLine::GetEmail() const {
    return email;
}

void ContactInfoLine::SetName(std::string name) {
    this->name = name;
}
    
void ContactInfoLine::SetEmail(std::string email) {
    this->email = email;
}

void ContactInfoLine::Print() const {
    std::println("{}", GetFormatted());
}

std::string ContactInfoLine::GetPrefix() const {
    return prefix;
}

std::string ContactInfoLine::ToStr() const {
    return std::format("{} {}", name, email);
}

std::string ContactInfoLine::GetFormatted() const {
    return std::format("{} {}, Email: {}", prefix, name, email);
}

void ContactInfoLine::Serialize(int ckey, std::string vkey) {
    void* handle = dlopen("Encryption/libcipher.so", RTLD_LAZY);
    if (!handle) {
        std::cerr << "Error library loading: " << dlerror() << std::endl;
        return;
    }

    auto cipher_create_caesar = (create_caesar_fn)dlsym(handle, "cipher_create_caesar");
    auto cipher_create_vigenere = (create_vigenere_fn)dlsym(handle, "cipher_create_vigenere");
    auto cipher_encrypt = (encrypt_fn)dlsym(handle, "cipher_encrypt");
    auto cipher_decrypt = (decrypt_fn)dlsym(handle, "cipher_decrypt");
    auto cipher_destroy = (destroy_fn)dlsym(handle, "cipher_destroy");
    auto cipher_free = (free_fn)dlsym(handle, "cipher_free");

    if (!cipher_create_caesar || !cipher_create_vigenere || !cipher_encrypt || 
        !cipher_decrypt || !cipher_destroy || !cipher_free) {
        std::cerr << "Functions are not found." << std::endl;
        dlclose(handle);
        return;
    }
    
    
    cipher_t* caesar = cipher_create_caesar(ckey);

    char* raw_encrypted_name_c = cipher_encrypt(caesar, GetName().c_str());
    
    std::string encrypted_name_c(raw_encrypted_name_c);
    SetName(encrypted_name_c);

    delete[] raw_encrypted_name_c;

    char* raw_encrypted_email_c = cipher_encrypt(caesar, GetEmail().c_str());

    std::string encrypted_email_c(raw_encrypted_email_c);
    SetEmail(encrypted_email_c);

    delete[] raw_encrypted_email_c;

    cipher_t* vigenere = cipher_create_vigenere(vkey.c_str());

    char* raw_encrypted_name_v = cipher_encrypt(vigenere, GetName().c_str());

    std::string encrypted_name_v(raw_encrypted_name_v);
    SetName(encrypted_name_v);

    delete[] raw_encrypted_name_v;

    char* raw_encrypted_email_v = cipher_encrypt(vigenere, GetEmail().c_str());

    std::string encrypted_email_v(raw_encrypted_email_v);
    SetEmail(encrypted_email_v);
    
    delete[] raw_encrypted_email_v;

    cipher_destroy(caesar);
    cipher_destroy(vigenere);

    dlclose(handle);
}

void ContactInfoLine::Deserialize(int ckey, std::string vkey) {
    void* handle = dlopen("Encryption/libcipher.so", RTLD_LAZY);
    if (!handle) {
        std::cerr << "Error library loading: " << dlerror() << std::endl;
        return;
    }

    auto cipher_create_caesar = (create_caesar_fn)dlsym(handle, "cipher_create_caesar");
    auto cipher_create_vigenere = (create_vigenere_fn)dlsym(handle, "cipher_create_vigenere");
    auto cipher_encrypt = (encrypt_fn)dlsym(handle, "cipher_encrypt");
    auto cipher_decrypt = (decrypt_fn)dlsym(handle, "cipher_decrypt");
    auto cipher_destroy = (destroy_fn)dlsym(handle, "cipher_destroy");
    auto cipher_free = (free_fn)dlsym(handle, "cipher_free");

    if (!cipher_create_caesar || !cipher_create_vigenere || !cipher_encrypt || 
        !cipher_decrypt || !cipher_destroy || !cipher_free) {
        std::cerr << "Functions are not found." << std::endl;
        dlclose(handle);
        return;
    }
    
    
      
    cipher_t* caesar = cipher_create_caesar(ckey);

    char* raw_decrypted_name_c = cipher_decrypt(caesar, GetName().c_str());
    
    std::string decrypted_name_c(raw_decrypted_name_c);
    SetName(decrypted_name_c);

    delete[] raw_decrypted_name_c;

    char* raw_decrypted_email_c = cipher_decrypt(caesar, GetEmail().c_str());

    std::string decrypted_email_c(raw_decrypted_email_c);
    SetEmail(decrypted_email_c);

    delete[] raw_decrypted_email_c;

    cipher_t* vigenere = cipher_create_vigenere(vkey.c_str());

    char* raw_decrypted_name_v = cipher_decrypt(vigenere, GetName().c_str());

    std::string decrypted_name_v(raw_decrypted_name_v);
    SetName(decrypted_name_v);

    delete[] raw_decrypted_name_v;

    char* raw_decrypted_email_v = cipher_decrypt(vigenere, GetEmail().c_str());

    std::string decrypted_email_v(raw_decrypted_email_v);
    SetEmail(decrypted_email_v);
    
    delete[] raw_decrypted_email_v;
    
    cipher_destroy(caesar);
    cipher_destroy(vigenere);

    dlclose(handle);
}


int TextEditor::GetCurrentLine() const {
    return currentRow;
}

Line* TextEditor::GetLine(int line) {
    return lines[line];
}

std::string TextEditor::GetLineStr(int line) const {
    return lines[line]->ToStr();
}

std::string TextEditor::GetLineType(int line) const {
    if (TextLine* l = dynamic_cast<TextLine*>(lines[line])) {
        return "TextLine";
    }
    else if (CheckListLine* l = dynamic_cast<CheckListLine*>(lines[line])) {
        return "CheckListLine";
    }
    else if (ContactInfoLine* l = dynamic_cast<ContactInfoLine*>(lines[line])) {
        return "ContactInfoLine";
    }
    return "None";
}

std::string TextEditor::GetCopied() const {
    return copied;
}

void TextEditor::AddLine(Line* line) {
    lines.push_back(line);
    currentRow++;
}

void TextEditor::AddToEnd(const std::string& text) {
    if (TextLine* textLine = dynamic_cast<TextLine*>(lines[currentRow])) {
        std::string newText = textLine->ToStr() + text;
        textLine->SetText(newText);
    }
    else {
        Line* line = new TextLine(text);
        AddLine(line);
    }
}

bool TextEditor::Insert(int line, int index, const std::string& text) {
    if (TextLine* textLine = dynamic_cast<TextLine*>(lines[line])) {
        std::string newText = textLine->ToStr();
        if (index <= newText.size()) {
            newText.insert(index, text);
            textLine->SetText(newText);
            return 1;
        }
    }
    return 0;    
}

bool TextEditor::InsertWithReplacement(int line, int index, const std::string& text) {
    if (TextLine* textLine = dynamic_cast<TextLine*>(lines[line])) {
        std::string newText = lines[line]->ToStr();
        if (index <= newText.size()) {
            newText.replace(index, std::min(text.size(), newText.size() - index), text);
            textLine->SetText(newText);
            return 1;
        }
    }
    return 0;
}

std::vector<int> TextEditor::Search(const std::string text) const {
    std::vector<int> indexes; 
    for (size_t i = 0; i < lines.size(); i++) {
        size_t foundIndex = lines[i]->ToStr().find(text);
        if (foundIndex != std::string::npos) {
            indexes.push_back(i);
            indexes.push_back(foundIndex);
        }
    }
    return indexes;
}

bool TextEditor::Delete(int line, int index, int symbols) {
    if (line < lines.size()) {
        if (TextLine* textLine = dynamic_cast<TextLine*>(lines[line])) {
            std::string currentText = textLine->ToStr();
            if (index >= 0 && index < currentText.size()) {
                currentText.erase(index, symbols);
                textLine->SetText(currentText);
                return 1;
            }    
        }
    }
    return 0;
}

bool TextEditor::DeleteLine(int line) {
    if (line < lines.size()) {
        if (GetLineType(line) == "TextLine") { // TextLine
            if (!lines.empty() && currentRow >= 0 && currentRow < lines.size()) {
                delete lines[currentRow];
                lines.erase(lines.begin() + currentRow);
            }

            currentRow = lines.size() - 1;
            return 1;

        }
        else if (GetLineType(line) == "CheckListLine" || GetLineType(line) == "ContactInfoLine") { // CheckListLine and ContactInfoLine
            if (!lines.empty() && currentRow >= 0 && currentRow < lines.size()) {
                delete lines[currentRow];
                lines.erase(lines.begin() + currentRow);
            }

            currentRow = lines.size() - 1;
            if (currentRow < 0) {
                currentRow = 0;
            }
            return 1;
        }
    }
    return 0;
    
}

bool TextEditor::Copy(int line, int index, int symbols) {
    if (index <= lines[line]->ToStr().size()) {
        copied = lines[line]->ToStr().substr(index, symbols);
        return 1;
    }
    return 0;
}

bool TextEditor::Paste(int line, int index) {
    return Insert(line, index, copied);
}

bool TextEditor::Cut(int line, int index, int symbols) {
    return Copy(line, index, symbols) && Delete(line, index, symbols);
}

void TextEditor::PrintAll() const {
    for (size_t i = 0; i < lines.size(); i++) {
        lines[i]->Print();
    }
}

bool TextEditor::SaveToFile(std::string filename) const {
    std::ofstream file (filename);
    
    if (!file.is_open()) {
        return 0;
    }

    for (size_t i = 0; i < lines.size(); i++) {
        file << lines[i]->GetFormatted() << std::endl;
    }

    file.close();
    return 1;
}

bool TextEditor::LoadFromFile(std::string filename) { // TO COMPLETE
    std::ifstream file (filename);
    
    if (!file.is_open()) {
        return 0;
    }

    for (size_t i = 0; i < lines.size(); i++) {
        delete lines[i];
    }
    lines.clear();

    std::string fileLine;
    int i = 0;
    while (std::getline(file, fileLine)) {
        std::stringstream row(fileLine);
        std::string linePrefix;
        row >> linePrefix;
        Line* line;
        if (linePrefix == "[x]") {
            fileLine.erase(0, fileLine.find("[x]") + 4);
            line = new CheckListLine(fileLine, 1);
        }
        else if (linePrefix == "[") {
            fileLine.erase(0, fileLine.find("[ ]") + 4);
            line = new CheckListLine(fileLine, 0);
        }
        else if (linePrefix == "Contact") {
            fileLine.erase(0, fileLine.find("Contact -") + 10);
            size_t emailIndex = fileLine.find(", Email: ");
            std::string name = fileLine.substr(0, emailIndex);
            std::string email = fileLine.substr(emailIndex + 9);
            line = new ContactInfoLine(name, email);
        }
        else {
            fileLine.erase(0, fileLine.find(" ") + 1);
            line = new TextLine(fileLine);
        }
        
        lines.push_back(line);
        i++;
    }

    file.close();
    return 1;
}

void TextEditor::HistoryPush(Action& action) {
    HistoryNode* newNode = new HistoryNode(action);
    if (head == nullptr) {
        head = newNode;
        currentAction = newNode;
        return;
    }

    if (currentAction != nullptr) {
        HistoryNode* clearNode = currentAction->next;
        while (clearNode != nullptr) {
            HistoryNode* temp = clearNode->next;
            delete clearNode;
            clearNode = temp;
        }

        currentAction->next = newNode;
        newNode->prev = currentAction;
    }
    else {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
    currentAction = newNode;
}

Action TextEditor::HistoryPop() {
    if (currentAction == nullptr) {
        return Action(NoneAction, 0, 0, 0, "", "");
    }

    Action poppedAction = currentAction->action;
    HistoryNode* nodeToDelete = currentAction;
    currentAction = currentAction->prev;
    
    if (currentAction != nullptr) {
        currentAction->next = nullptr;
    }

    delete nodeToDelete;
    return poppedAction;
    
}

void TextEditor::Undo() {
    if (currentAction == nullptr) {
        return;
    }
    
    switch(currentAction->action.actionType) {
        case AddToEndAction:
        case InsertAction:
        case PasteAction:
            Delete(currentAction->action.line, currentAction->action.index, currentAction->action.amount);
            break;
        
        case AddNewLineAction:
            DeleteLine(currentAction->action.line);
            break;

        case InsertWithReplacementAction:
            Delete(currentAction->action.line, currentAction->action.index, currentAction->action.newText.size());
            Insert(currentAction->action.line, currentAction->action.index, currentAction->action.prevText);
            break;

        case DeleteAction:
        case CutAction:
            Insert(currentAction->action.line, currentAction->action.index, currentAction->action.prevText);
            break;
        // case DeleteLineAction:
        //     Line* line = nullptr;
        //     short lineType = currentAction->action.amount;
        //     if (lineType == 1) { // TextLine
        //         line = new TextLine("");
        //     }    
        //     else if (lineType == 2) { // CheckListLine
        //         std::string item = currentAction->action.prevText;
        //         line = new CheckListLine(item, 0); 
        //     }
        //     else if (lineType == 3) { // ContactInfoLine
        //         std::string name = currentAction->action.prevText;
        //         std::string email = currentAction->action.newText;
        //         line = new ContactInfoLine(name, email);
        //     }
        //     if (line != nullptr) {
        //         AddLine(line);
        //         currentRow = lines.size() - 1;
        //     }
        //     break;
    }
    currentAction = currentAction->prev;
}

void TextEditor::Redo() {
    HistoryNode* nextNode = nullptr;
    if (currentAction == nullptr) {
        nextNode = head;
    }
    else {
        nextNode = currentAction->next;
    }

    if (nextNode == nullptr) {
        return;
    }

    switch(nextNode->action.actionType) {
        case AddToEndAction:
        case InsertAction:
        case PasteAction:
            Insert(nextNode->action.line, nextNode->action.index, nextNode->action.newText);
            break;

        case AddNewLineAction: {
            Line* line = nullptr;
            short lineType = nextNode->action.amount;
            if (lineType == 1) { // TextLine
                line = new TextLine("");
            }    
            else if (lineType == 2) { // CheckListLine
                std::string item = nextNode->action.prevText;
                line = new CheckListLine(item, 0); 
            }
            else if (lineType == 3) { // ContactInfoLine
                std::string name = nextNode->action.prevText;
                std::string email = nextNode->action.newText;
                line = new ContactInfoLine(name, email);
            }
            if (line != nullptr) {
                AddLine(line);
                currentRow = lines.size() - 1;
            }
            break;
        }
        case InsertWithReplacementAction:
            Delete(nextNode->action.line, nextNode->action.index, nextNode->action.prevText.size());
            Insert(nextNode->action.line, nextNode->action.index, nextNode->action.newText);
            break;

        case DeleteAction:
        case CutAction:
            Delete(nextNode->action.line, nextNode->action.index, nextNode->action.amount);
            break;
        // case DeleteLineAction:
        //     DeleteLine(nextNode->action.line);
        //     break;
    }
    currentAction = nextNode;
}

void TextEditor::SerializeAll(int ckey, std::string vkey) {
    for (size_t i = 0; i < lines.size(); i++) {
        lines[i]->Serialize(ckey, vkey);
    }
}

void TextEditor::DeserializeAll(int ckey, std::string vkey) {
    for (size_t i = 0; i < lines.size(); i++) {
        lines[i]->Deserialize(ckey, vkey);
    }
}

TextEditor::~TextEditor() {
    for (size_t i = 0; i < lines.size(); i++) {
        delete lines[i];
    }
    HistoryNode* current = head;
    while (current != nullptr) {
        HistoryNode* nextNode = current->next;
        
        delete current;         
        current = nextNode;
    }
}