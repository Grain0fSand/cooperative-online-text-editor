#include "action.h"

Action::Action() :
    cursorPos(-1),
    numChars(-1),
    chars(""),
    textFormatBoolean(false),
    comboFontIndex(-1),
    actionType(NoActionType),
    textFormatType(NoTextFormatType),
    blockFormatType(NoBlockFormatType)
{
}

Action::Action(int cursorPos, int numChars, QString chars, ActionType actionType) :
        cursorPos(cursorPos),
        numChars(numChars),
        chars(chars),
        textFormatBoolean(false),
        actionType(actionType)
{
}

int Action::getCursorPos() const
{
    return cursorPos;
}

void Action::setCursorPos(int value)
{
    cursorPos = value;
}

int Action::getNumChars() const
{
    return numChars;
}

void Action::setNumChars(int value)
{
    numChars = value;
}

QString Action::getChars() const
{
    return chars;
}

void Action::setChars(QString value)
{
    chars = value;
}

bool Action::getTextFormatBoolean() const
{
    return textFormatBoolean;
}

void Action::setTextFormatBoolean(bool value)
{
    textFormatBoolean = value;
}

int Action::getComboFontIndex() const
{
    return comboFontIndex;
}

void Action::setComboFontIndex(int value)
{
    comboFontIndex = value;
}

ActionType Action::getActionType() const
{
    return actionType;
}

void Action::setActionType(const ActionType &value)
{
    actionType = value;
}

TextFormatType Action::getTextFormatType() const
{
    return textFormatType;
}

void Action::setTextFormatType(const TextFormatType &value)
{
    textFormatType = value;
}

BlockFormatType Action::getBlockFormatType() const
{
    return blockFormatType;
}

void Action::setBlockFormatType(const BlockFormatType &value)
{
    blockFormatType = value;
}
