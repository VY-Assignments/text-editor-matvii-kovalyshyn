#include <string>
#include <print>
#include <vector>
#include <fstream>
#include "text_editor.hpp"


Action::Action(ActionType actionType, int amount, int line, int index, std::string prevText) {
    this->actionType = actionType;
    this->amount = amount;
    this->line = line;
    this->index = index;
    this->prevText = prevText;
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

void ContactInfoLine::Print() const {
    std::println("Contact - {}, Email: {}", name, email);
}

std::string ContactInfoLine::ToStr() const {
    return std::format("Contact - {}, Email: {}", name, email);
}



int TextEditor::GetCurrentLine() const {
    return currentRow;
}

std::string TextEditor::GetLine(int line) const {
    return lines[line]->ToStr();
}

std::string TextEditor::GetCopied() const {
    return copied;
}

void TextEditor::AddLine(Line* line) {
    lines.push_back(line);
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
        std::string newText = lines[line]->ToStr();
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
}

bool TextEditor::Delete(int line, int index, int symbols) {
    if (TextLine* textLine = dynamic_cast<TextLine*>(lines[line])) {
        textLine->ToStr().erase(index, symbols);
    }
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

void TextEditor::Undo() {

}

void TextEditor::Redo() {

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
        return Action(NoneAction, 0, 0, 0, "");
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

}

void TextEditor::Redo() {

}

TextEditor::~TextEditor() {
    for (size_t i = 0; i < lines.size(); i++) {
        delete lines[i];
    }
}