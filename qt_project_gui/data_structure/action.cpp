#include "action.h"

Action::Action() :
    actionType(Deletion),
    chars(""),
    comboFontIndex(-1),
    is_bold(),
    is_italic(),
    is_underlined(),
    blockFormatType(NoBlockFormatType)
{
}

Action::Action(QString chars, int fontIndex, int fontSize, bool bold, bool italic, bool underlined, BlockFormatType blockFormatType) :
    actionType(Insertion),
    chars(chars),
    comboFontIndex(fontIndex),
    fontSize(fontSize),
    is_bold(bold),
    is_italic(italic),
    is_underlined(underlined),
    blockFormatType(blockFormatType)
{
}

Action::Action(ActionType actionType, BlockFormatType blockFormatType) :
    actionType(BlockFormatting),
    blockFormatType(blockFormatType)
{
}

Action::Action(ActionType actionType, int select, int value) : actionType(actionType){
    switch (select) {
        case 0:
            is_bold = value;
            break;
        case 1:
            is_italic = value;
            break;
        case 2:
            is_underlined = value;
            break;
        case 3:
            comboFontIndex = value;
            break;
        case 4:
            fontSize = value;
            break;
    }
}

QString Action::getChars() const
{
    return chars;
}

void Action::setChars(QString value)
{
    chars = value;
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

bool Action::getBold() const
{
    return is_bold;
}

bool Action::getItalic() const
{
    return is_italic;
}

bool Action::getUnderlined() const
{
    return is_underlined;
}

void Action::setTextFormat(bool bold, bool italic, bool underlined)
{
    is_bold = bold;
    is_italic = italic;
    is_underlined = underlined;
}

BlockFormatType Action::getBlockFormat() const
{
    return blockFormatType;
}

void Action::setBlockFormat(const BlockFormatType &value)
{
    blockFormatType = value;
}


