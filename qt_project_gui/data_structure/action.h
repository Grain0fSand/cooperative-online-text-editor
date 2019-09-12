#ifndef ACTION_H
#define ACTION_H

#include <iostream>
#include <QString>
#include "symbol_id.h"

enum ActionType {
    NoActionType,
    Insertion,
    Deletion,
    TextFormatting,
    BlockFormatting
};

enum TextFormatType {
    NoTextFormatType,
    Bold,
    Italic,
    Underlined,
    FontFamily,
    FontSize
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
    Action();
    Action(int cursorPos, int numChars, QString chars, ActionType actionType);

    int getCursorPos() const;
    void setCursorPos(int value);

    int getNumChars() const;
    void setNumChars(int value);

    QString getChars() const;
    void setChars(QString value);

    bool getTextFormatBoolean() const;
    void setTextFormatBoolean(bool value);

    int getComboFontIndex() const;
    void setComboFontIndex(int value);

    ActionType getActionType() const;
    void setActionType(const ActionType &value);

    TextFormatType getTextFormatType() const;
    void setTextFormatType(const TextFormatType &value);

    BlockFormatType getBlockFormatType() const;
    void setBlockFormatType(const BlockFormatType &value);

private:
    int cursorPos;
    int numChars;
    QString chars;
    bool textFormatBoolean;
    int comboFontIndex;
    ActionType actionType;
    TextFormatType textFormatType;
    BlockFormatType blockFormatType;
};

class ActionWrapper
{
public:
    Action action;
    SymbolId left_symbol;
    SymbolId symbol;
};

#endif // ACTION_H
