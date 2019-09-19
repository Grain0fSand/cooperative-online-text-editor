#ifndef ACTION_H
#define ACTION_H

#include <iostream>
#include <QString>
#include <vector>
#include "symbol_id.h"

enum ActionType {
    NoActionType,
    Insertion,
    Deletion,
    TextFormatting,
    BlockFormatting
};

enum BlockFormatType {
    NoBlockFormatType,
    AlignLeft,
    AlignCenter,
    AlignRight,
    AlignJustify
};

class Action
{
public:


    //TODO:write contructors
    Action(QString chars, int fontIndex, int fontSize, bool bold, bool italic, bool underlined, BlockFormatType blockFormatType);   //for insertion
    Action(); //for deletion
    Action(ActionType actionType, BlockFormatType blockFormatType);
    Action(ActionType actionType, int select, int value);  //for text formatting

    int getCursorPos() const;
    void setCursorPos(int value);

    int getNumChars() const;
    void setNumChars(int value);

    QString getChars() const;
    void setChars(QString value);

    bool getBold() const;
    bool getItalic() const;
    bool getUnderlined() const;
    void setTextFormat(bool bold, bool italic, bool underlined);

    int getComboFontIndex() const;
    void setComboFontIndex(int value);

    ActionType getActionType() const;
    void setActionType(const ActionType &value);

    BlockFormatType getBlockFormat() const;
    void setBlockFormat(const BlockFormatType &value);

private:
    //TODO: inserted strings must have uniform formatting
    ActionType actionType;
    QString chars;
    int comboFontIndex;
    int fontSize;
    bool is_bold, is_italic, is_underlined;
    BlockFormatType blockFormatType;

};

class ActionWrapper
{

public:
    Action action;
    std::pair<int,int> rel_symbol;
    std::vector<std::pair<int,int>> symbol;     //all symbols to change for deletion and formatting

    ActionWrapper(Action action, std::pair<int,int> rel_symbol, std::vector<std::pair<int,int>> symbol):
            action(action),
            rel_symbol(rel_symbol),
            symbol(std::move(symbol))
    {}

    ActionWrapper(Action action) : action(action){
    }

};

#endif // ACTION_H
