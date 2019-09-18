#include "action.h"

Action::Action() :
    actionType(NoActionType),
    chars(""),
    comboFontIndex(-1),
    is_bold(),
    is_italic(),
    is_underlined(),
    blockFormatType(NoBlockFormatType)
{
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

BlockFormatType Action::getBlockFormatType() const
{
    return blockFormatType;
}

void Action::setBlockFormatType(const BlockFormatType &value)
{
    blockFormatType = value;
}


