#ifndef ACTION_H
#define ACTION_H

#include <iostream>
#include <QString>
#include <vector>
#include "symbol_id.h"
#include <nlohmann/json.hpp>
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

    Action(QString chars, int fontIndex, int fontSize, bool bold, bool italic, bool underlined, BlockFormatType blockFormatType);   //for insertion
    Action(); //for deletion
    Action(BlockFormatType blockFormatType);  //for block formatting
    Action(int select, int value);  //for text formatting

    QString getChars() const;
    void setChars(QString value);

    bool getBold() const;
    bool getItalic() const;
    bool getUnderlined() const;
    void setTextFormat(bool bold, bool italic, bool underlined);
    int getComboFontIndex() const;
    void setComboFontIndex(int value);
    int getSelection() const;

    ActionType getActionType() const;
    void setActionType(const ActionType &value);

    BlockFormatType getBlockFormat() const;
    void setBlockFormat(const BlockFormatType &value);

private:
    //TODO: inserted strings must have uniform formatting
    ActionType actionType;
    QString chars;
    int select;  //indicates which style feature is being changed
    int comboFontIndex;
    int fontSize;
public:
    int getFontSize() const;

private:
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

    // NON TOGLIERE, SERVE PER RISOLVERE I CONFLITTI DOVUTI ALL'ORDINE
    bool operator <(const ActionWrapper other) {
        std::pair<int,int> sym_a = this->rel_symbol;
        std::pair<int,int> sym_b = other.rel_symbol;

        int ret = sym_a.first-sym_b.first;

        if (ret==0)
            ret = sym_a.second-sym_b.second;

        return ret < 0;
    }

    ActionWrapper(){}

    ActionWrapper(Action action) : action(action){
    }

    using json = nlohmann::json;

    json get_json() {
        json j;
        const ActionWrapper& actionWrapper = *this;
        j.push_back(actionWrapper.rel_symbol.first);
        j.push_back(actionWrapper.rel_symbol.second);
        j.push_back(actionWrapper.symbol.size());
        for (auto p : actionWrapper.symbol) {
            j.push_back(p.first);
            j.push_back(p.second);
        }

        const Action &a = actionWrapper.action;
        ActionType type = a.getActionType();
        j.push_back(type);

        switch(type) {
            case Insertion:
                j.push_back(a.getChars().toUtf8().constData());
                j.push_back(a.getComboFontIndex());
                j.push_back(a.getFontSize());
                j.push_back(a.getBold());
                j.push_back(a.getItalic());
                j.push_back(a.getUnderlined());
                j.push_back(a.getBlockFormat());

                break;

            case  TextFormatting:
                j.push_back(a.getSelection());
                switch (a.getSelection()) {
                    case 0:
                        j.push_back(a.getBold());
                        break;
                    case 1:
                        j.push_back(a.getItalic());
                        break;
                    case 2:
                        j.push_back(a.getUnderlined());
                        break;
                    case 3:
                        j.push_back(a.getComboFontIndex());
                        break;
                    case 4:
                        j.push_back(a.getFontSize());
                        break;
                }
                break;
            case  BlockFormatting:
                j.push_back(a.getBlockFormat());
                break;
            default:
                break;
        }
        return j;
    }

    static void action_from_json(ActionWrapper& w,const json& j) {
        std::string s = j.dump();
        std::pair<int,int> p;
        int i = 0;
        p.first = j[i].get<int>();
        p.second = j[++i].get<int>();
        int sz = j[++i].get<int>();
        std::vector<std::pair<int,int>> vc(sz);
        for (int z = 0; z < sz; ++z) {
            vc[z].first = j[++i].get<int>();
            vc[z].second = j[++i].get<int>();
        }

        int type = j[++i].get<int>();
        Action a;

        switch(type) {
            case Insertion:
                //yuck
                a = Action(QString::fromStdString(j[i+1].get<std::string>()), j[i+2].get<int>(), j[i+3].get<int>(), j[i+4].get<bool>(), j[i+5].get<bool>(), j[i+6].get<bool>(),
                           static_cast<BlockFormatType>(j[i+7].get<int>()));
                break;
            case  TextFormatting:
                a = Action(j[i+1].get<int>(), j[i+2].get<int>());
                break;
            case  BlockFormatting:
                a = Action(static_cast<BlockFormatType>(j[i+1].get<int>()));
                break;
            default:
                break;
        }
        w = ActionWrapper(a, p, vc);
    }
};

#endif // ACTION_H
