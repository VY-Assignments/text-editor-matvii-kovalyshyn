#include <string>
#include <print>
#include <vector>

enum ActionType {
    NoneAction,
    AddToEndAction,
    AddNewLineAction,
    InsertAction,
    InsertWithReplacementAction,
    DeleteAction,
    PasteAction,
    CutAction,
    DeleteLineAction,
    DeleteAndInsertAction,
};

class Action {
public:
    ActionType actionType;
    int amount;
    int line;
    int index;
    std::string prevText;
    std::string newText;
    Action(ActionType actionType, int amount, int line, int index, std::string prevText, std::string newText);
};

class HistoryNode {
public:    
    Action action;
    HistoryNode* prev = nullptr;
    HistoryNode* next = nullptr;
    HistoryNode(const Action& action);
};


class Line {
public:
    virtual void Print() const = 0;
    virtual std::string ToStr() const = 0;
    virtual ~Line() = default;
};

class TextLine : public Line {
private:    
    std::string text;
public:
    TextLine(const std::string& text);
    void Print() const override;
    std::string ToStr() const override;
    void SetText(const std::string& text);
};

class CheckListLine : public Line {
private:
    std::string item;
    bool checked;
public:
    CheckListLine(const std::string& item, bool checked);
    std::string GetItem() const;
    void SetItem(std::string item);
    void ChangeChecked();
    void Print() const override;
    std::string ToStr() const override;
};

class ContactInfoLine : public Line {
private:
    std::string name;
    std::string email;
public:
    ContactInfoLine(const std::string& name, const std::string& email);
    void SetName(std::string name);
    void SetEmail(std::string email);
    void Print() const override;
    std::string ToStr() const override;
};


class TextEditor {
private:
    std::vector<Line*> lines;
    std::string copied = ""; 
    int currentRow = -1;
    HistoryNode* currentAction = nullptr;
    HistoryNode* head = nullptr;
public:
    int GetCurrentLine() const;
    Line* GetLine(int line);    
    std::string GetLineStr(int line) const;
    std::string GetLineType(int line) const;
    std::string GetCopied() const;
    void AddLine(Line* line);
    void AddToEnd(const std::string& text);
    bool Insert(int line, int index, const std::string& text);
    bool InsertWithReplacement(int line, int index, const std::string& text);
    std::vector<int> Search(const std::string text) const; 
    bool Delete(int line, int index, int symbols);
    bool Copy(int line, int index, int symbols);
    bool Paste(int line, int index);
    bool Cut(int line, int index, int symbols);
    void PrintAll() const;
    bool SaveToFile(std::string filename) const;
    bool LoadFromFile(std::string filename);
    void HistoryPush(Action& action);
    Action HistoryPop();
    void Undo();
    void Redo();
    ~TextEditor();
};