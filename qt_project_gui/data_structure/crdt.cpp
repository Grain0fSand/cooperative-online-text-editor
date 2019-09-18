#include "crdt.h"
#include "symbol_id.h"
#include <thread>

void sendAction(ActionWrapper action_wrapper) {
}

void receiveAction(std::vector<SymbolId> all_pos, Action action) {
}

SymbolId Crdt::findRelativePosition(int left_pos) {
    int cmp = 0;
    SymbolId left_sym = list.front();   //invisible block tombstone

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


//TODO: multiple block copy
int Crdt::symbolInsertion(const SymbolId& left_sym, int n, const SymbolId& symbol, const QString chars) {
    auto it = list.begin();
    //insert in relative position
    while (it != list.end()) {
        if (left_sym == it->getSymbolId())
            break;
        ++it;
    }
    int ret = it - list.begin();
    ++it;

    //insert symbols
    for (int i = 0; i < n; ++i) {
        it = list.insert(it, SymbolId(symbol.getUsrId(), i + symbol.getIncId()));
        if (chars[i] == '\n')  it->setBlockStart();
        ++it;
    }
    return ret;
}

//delete done by client
std::vector<SymbolId> Crdt::symbolDeletion(int n, const SymbolId& first_symbol) {
    auto it = list.begin();
    std::vector<SymbolId> ret;

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
std::vector<SymbolId> Crdt::textFormatting(int n, const SymbolId& first_symbol) {
    auto it = list.begin();
    std::vector<SymbolId> ret;

    while (it != list.end()) {

        // if first symbol found format string and break
        if (it->getSymbolId() == first_symbol) {

            while (n-- > 0) {
                //format without checking version because it's unnecessary
                if (!it->is_blockStart()) {
                    it->setVersion(usr_id, list.size());
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
std::vector<SymbolId> Crdt::blockFormatting(int n, const SymbolId& first_symbol) {
    auto it = list.begin();
    std::vector<SymbolId> ret;
    int last_block_starter;

    while (it != list.end()) {

        if (it->is_blockStart())
            last_block_starter = it - list.begin();

        // if first symbol found format string and break
        if (it->getSymbolId() == first_symbol) {
            list[last_block_starter].setVersion(usr_id, list.size());
            ret.push_back(list[last_block_starter]);
            ++it;      //in case first_symbol is block_starter

            while (n-- > 1) {
                //format without checking version because it's unnecessary
                if (it->is_blockStart()) {
                    it->setVersion(usr_id, list.size());
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
void Crdt::sendActionToServer(Action& action, int numChars) {
    SymbolId rel_symbol = findRelativePosition(action.getCursorPos());

    SymbolId symbol(usr_id, list.size());
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
            action_wrapper.symbol = textFormatting(numChars, rel_symbol);
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
    sendAction(action_wrapper);

    //flush server buffer,  the documents only gets refreshed AFTER local action
    while (!action_queue.empty()) {
        receiveActionFromServer(action_queue.front());
        action_queue.pop();
    }
}


std::vector<int> Crdt::symbolInsertionExt(const SymbolId& left_sym, int n, const SymbolId& symbol, const QString chars) {
    auto it = list.begin();
    std::vector<int> ret;
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

    //insert symbols
    for (int i = 0; i < n; ++i) {
        SymbolId s(SymbolId(symbol.getUsrId(), i + symbol.getIncId()));
        while (it->getSymbolId() > s) {
            ++it;
            ++j;
        }
        ret.push_back(j + i);   //absolute position
        it = list.insert(it, s);
        if (chars[i] == '\n')  it->setBlockStart();
        ++it;
    }
    return ret;
}

//delete done by server
std::vector<int> Crdt::symbolDeletionExt(const std::vector<SymbolId>& symbol) {
    auto it = list.begin();
    auto del_it = symbol.begin();  //symbols to delete
    int i = 0;
    std::vector<int> ret;
    while(it != list.end() && del_it != symbol.end()) {
        if (it->getSymbolId() == del_it->getSymbolId()) {
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

std::vector<int> Crdt::formattingExt(const SymbolId& rel_symbol, const std::vector<SymbolId>& symbol) {
    auto it = list.begin();
    auto form_it = symbol.begin();
    std::vector<int> ret;
    int i = 0;
    while (it != list.end() && form_it != symbol.end()) {
        if (it->getSymbolId() == form_it->getSymbolId()) {
            ++form_it;
            if (!it->is_hidden()) {
                ret.push_back(i);  //get absolute position
                it->setVersion(rel_symbol.getUsrId(), rel_symbol.getIncId());
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
            all_pos = symbolInsertionExt(action_wrapper.rel_symbol, action.getNumChars(), action_wrapper.symbol.front(), action.getChars());
            break;
        case Deletion:
            all_pos = symbolDeletionExt(action_wrapper.symbol);
            break;
        case TextFormatting:
        case BlockFormatting:
            all_pos = formattingExt(action_wrapper.rel_symbol, action_wrapper.symbol);
            break;
        default:
            break;
    }
    //receive_action(all_pos, action);

//    for (SymbolId s : list)
//        std::cout << s.getIncId() << s.getUsrId() << s.is_hidden() << ' ';
//    std::cout << std::endl;
}
