#include "crdt.h"
#include "symbol_id.h"
#include "../forms/mytextedit.h"
#include "../utility/json_comunicator.h"
#include "../forms/mainwindow.h"
#include <thread>

std::pair<int,int> Crdt::findRelativePosition(int left_pos) {
    int cmp = 0;
    std::pair<int,int> left_sym = list.front().getSymbolId();   // block start tombstone

    //find left side symbol
    for(auto cc : list) {
        if (cmp == left_pos)
            break;

        if (!cc.is_hidden())
            ++cmp;

        left_sym = cc.getSymbolId();
    }

    if (cmp != left_pos)
        throw "error: char not already present in the memory";
    return left_sym;
}


void Crdt::symbolInsertion(const std::pair<int,int>& left_sym, int n, const std::pair<int,int>& symbol, const QString chars) {
    auto it = list.begin();
    //insert in relative position
    while (it != list.end()) {
        if (left_sym == it->getSymbolId())
            break;
        ++it;
    }
    ++it;

    //array with all symbols to insert in list
    SymbolId tmp_arr[n];
    for (int i = 0; i < n; ++i) {
        tmp_arr[i] = SymbolId(i + symbol.second, symbol.first);
        if (chars[i] == '\n')
            tmp_arr[i].setBlockStart();
    }

    list.insert(it, tmp_arr, tmp_arr + n);
}

//delete done by client
std::vector<std::pair<int,int>> Crdt::symbolDeletion(int n, const std::pair<int,int>& first_symbol) {
    auto it = list.begin();
    std::vector<std::pair<int,int>> ret;

    while(it != list.end()) {
        // if first symbol found delete string and break
        if (it->getSymbolId() == first_symbol) {
            while (n-- > 0) {
                it->hide();      //delete  without checking if it was already deleted
                ret.push_back(it->getSymbolId());
                ++it;
            }
            break;
        }
        ++it;
    }
    return ret;
}

//formatting done by client
std::vector<std::pair<int,int>> Crdt::textFormatting(int n, const std::pair<int,int>& first_symbol, int select) {
    auto it = list.begin();
    std::vector<std::pair<int,int>> ret;

    while (it != list.end()) {

        // if first symbol found format string and break
        if (it->getSymbolId() == first_symbol) {

            while (n-- > 0) {
                //format without checking version because it's unnecessary
                if (!it->is_blockStart()) {
                    it->setVersion(list.size(), usr_id, select);
                    ret.push_back(it->getSymbolId());
                }
                ++it;
            }
            return ret;
        }

        ++it;
    }
    throw "Fatal error, symbol couldn't be found";
}

//block formatting done by client
std::vector<std::pair<int,int>> Crdt::blockFormatting(int n, const std::pair<int,int>& first_symbol) {
    auto it = list.begin();
    std::vector<std::pair<int,int>> ret;
    int last_block_starter;

    while (it != list.end()) {

        if (it->is_blockStart())
            last_block_starter = it - list.begin();

        // if first symbol found format string and break
        if (it->getSymbolId() == first_symbol) {
            list[last_block_starter].setVersion(list.size(), usr_id, 0);
            ret.push_back(list[last_block_starter].getSymbolId());
            ++it;      //in case first_symbol is block_starter

            while (n-- > 1) {
                //format without checking version because it's unnecessary
                if (it->is_blockStart()) {
                    it->setVersion(list.size(), usr_id, 0);
                    ret.push_back(it->getSymbolId());
                }
                ++it;
            }
            return ret;
        }

        ++it;
    }
    throw "Fatal error, symbol couldn't be found";
}


//called by text editor
void Crdt::sendActionToServer(Action& action, int cursorPos, int numChars) {
    std::pair<int,int> rel_symbol = findRelativePosition(cursorPos);

    std::pair<int,int> symbol(list.size(), usr_id);
    ActionWrapper action_wrapper(action);  //to send to json serializer

    switch (action.getActionType()) {
        case Insertion:
            symbolInsertion(rel_symbol, numChars, symbol, action.getChars());  //insert in crdt structure
            action_wrapper.rel_symbol = rel_symbol; //left symbol
            action_wrapper.symbol.push_back(symbol);
            break;
        case Deletion:
            action_wrapper.rel_symbol = rel_symbol; //first symbol
            action_wrapper.symbol = symbolDeletion(numChars, rel_symbol);
            break;
        case TextFormatting:
            action_wrapper.symbol = textFormatting(numChars, rel_symbol, action.getSelection());
            action_wrapper.rel_symbol = symbol;  //for setting version
            break;
        case BlockFormatting:
            action_wrapper.symbol = blockFormatting(numChars, rel_symbol);
            action_wrapper.rel_symbol = symbol;  //for setting version
            break;
        default:
            break;
    }
    //send to server
    std::string crdt_str = json_serializer::sendAction(action_wrapper);

    auto docId = MainWindow::getInstance().sessionData.docId;
    auto token = MainWindow::getInstance().sessionData.token;

    // no need to free: https://stackoverflow.com/questions/26714492/how-to-release-memory-of-qthread-object
    QThread* sender = new OnlineSender{crdt_str,docId,token};
    sender->start();
    //std::cout << crdt_str;

//    //flush server buffer,  the documents only gets refreshed AFTER local action
//    while (!action_queue.empty()) {
//        receiveActionFromServer(action_queue.front());
//        action_queue.pop();
//    }
}

std::vector<int> Crdt::symbolInsertionExt(const std::pair<int,int>& left_sym, int n, const std::pair<int,int>& symbol, const QString chars) {
    auto it = list.begin();
    int j = 0;
    //insert in relative position
    while (it != list.end()) {
        if (left_sym == it->getSymbolId())
            break;

        if (!it->is_hidden())
            ++j;
        ++it;
    }
    ++it;
    ++j;

    //must check that doing it once is sufficient
    {
        while (it->getSymbolId() > symbol) {  //skip symbols with greater ID
            ++it;
            ++j;
        }
    }

    //insert symbols
    SymbolId tmp_arr[n];
    for (int i = 0; i < n; ++i) {
        tmp_arr[i] = SymbolId(symbol.first, i + symbol.second);
        if (chars[i] == '\n')
            tmp_arr[i].setBlockStart();
    }

    list.insert(it, tmp_arr, tmp_arr + n);

    return std::vector<int>(j);
}

//delete done by server
std::vector<int> Crdt::symbolDeletionExt(const std::vector<std::pair<int,int>>& symbol) {
    auto it = list.begin();
    auto del_it = symbol.begin();  //symbols to delete
    int i = 0;
    std::vector<int> ret;
    while(it != list.end() && del_it != symbol.end()) {
        if (it->getSymbolId() == *del_it) {
            ++del_it;
            if (!it->is_hidden()) {
                ret.push_back(i);  //get absolute position
                it->hide();
            }
        }
        ++it;
        if (!it->is_hidden())
             ++i;
    }
    return ret;
}

std::vector<int> Crdt::formattingExt(const std::pair<int,int>& rel_symbol, const std::vector<std::pair<int,int>>& symbol, int select) {
    auto it = list.begin();
    auto form_it = symbol.begin();
    std::vector<int> ret;
    int i = 0;
    while (it != list.end() && form_it != symbol.end()) {
        //found symbol to format
        if (it->getSymbolId() == *form_it) {
            ++form_it;
            //check hidden and version
            if (!it->is_hidden() && it->compareVersion(rel_symbol.first, rel_symbol.second, select)) {
                ret.push_back(i);  //get absolute position
                it->setVersion(rel_symbol.first, rel_symbol.second, select);
            }
        }
        ++it;
        if (!it->is_hidden())
            ++i;
    }
    return ret;
}

void Crdt::receiveActionFromServer(ActionWrapper& action_wrapper) {
    Action action = action_wrapper.action;
    std::vector<int> all_pos;
    switch (action.getActionType()) {
        case Insertion:
            all_pos = symbolInsertionExt(action_wrapper.rel_symbol, action.getChars().size(), action_wrapper.symbol.front(), action.getChars());
            break;
        case Deletion:
            all_pos = symbolDeletionExt(action_wrapper.symbol);
            break;
        case TextFormatting:
        case BlockFormatting:
            all_pos = formattingExt(action_wrapper.rel_symbol, action_wrapper.symbol, action.getSelection());
            break;
        default:
            break;
    }
   // myTextEdit::getInstance()->doReceivedAction(action, all_pos);

//    for (SymbolId s : list)
//        std::cout << s.getIncId() << s.getUsrId() << s.is_hidden() << ' ';
//    std::cout << std::endl;
}