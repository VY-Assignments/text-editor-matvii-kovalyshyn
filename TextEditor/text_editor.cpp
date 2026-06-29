#include <string>
#include <print>
#include <vector>
#include <fstream>
#include "text_editor.hpp"


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
    this->text = text;
}

void TextLine::Print() const {
    std::println("Text: {}", text);
}

std::string TextLine::ToStr() const {
    return text;
}

void TextLine::SetText(const std::string& text) {
    this->text = text;
}


CheckListLine::CheckListLine(const std::string& item, bool checked) {
    this->item = item;
    this->checked = checked;
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
    }
    else {
        checked = 0;
    }
}

void CheckListLine::Print() const {
    std::println("[{}] {}", (checked ? "x" : " "), item);
}

std::string CheckListLine::ToStr() const {
    return std::format("[{}] {}", (checked ? "x" : " "), item);
}


ContactInfoLine::ContactInfoLine(const std::string& name, const std::string& email) {
    this->name = name;
    this->email = email;
}

void ContactInfoLine::SetName(std::string name) {
    this->name = name;
}
    
void ContactInfoLine::SetEmail(std::string email) {
    this->email = email;
}

void ContactInfoLine::Print() const {
    std::println("Contact - {}, Email: {}", name, email);
}

std::string ContactInfoLine::ToStr() const {
    return std::format("Contact - {}, Email: {}", name, email);
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
        file << lines[i]->ToStr() << std::endl;
    }

    file.close();
    return 1;
}

bool TextEditor::LoadFromFile(std::string filename) { // TO COMPLETE
    std::ifstream file (filename);
    
    if (!file.is_open()) {
        return 0;
    }
    std::string fileLine;
    int i = 0;
    while (std::getline(file, fileLine)) {
        Line* line = new TextLine(fileLine);
        lines[i] = line;
        i++;
    }

    file.close();
    return 1;
}

void TextEditor::HistoryPush(Action& action) {
    HistoryNode* newNode = new HistoryNode(action);
    if (head == nullptr) {
        head = newNode;
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
        
        case AddNewLineAction: {
            short lineType = currentAction->action.amount;
            if (lineType == 1) { // TextLine
                if (!lines.empty() && currentRow >= 0 && currentRow < lines.size()) {
                    delete lines[currentRow];
                    lines.erase(lines.begin() + currentRow);
                }

                currentRow = lines.size() - 1;

            }
            else if (lineType == 2 || lineType == 3) { // CheckListLine and ContactInfoLine
                if (!lines.empty() && currentRow >= 0 && currentRow < lines.size()) {
                    delete lines[currentRow];
                    lines.erase(lines.begin() + currentRow);
                }

                currentRow = lines.size() - 1;
                if (currentRow < 0) {
                    currentRow = 0;
                }
            }
            
            break;
        }

        case InsertWithReplacementAction:
            Delete(currentAction->action.line, currentAction->action.index, currentAction->action.newText.size());
            Insert(currentAction->action.line, currentAction->action.index, currentAction->action.prevText);
            break;

        case DeleteAction:
        case CutAction:
            Insert(currentAction->action.line, currentAction->action.index, currentAction->action.prevText);
            break;
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
    }
    currentAction = nextNode;
}


TextEditor::~TextEditor() {
    for (size_t i = 0; i < lines.size(); i++) {
        delete lines[i];
    }
}