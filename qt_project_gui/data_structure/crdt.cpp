#include "crdt.h"
#include <thread>

Symbol Crdt::findRelativePosition(int left_pos) {
    int cmp = 0;
    Symbol left_sym(-1,-1);

    //find left side symbol
    for(auto cc : list) {
        if (cmp == left_pos)
            break;

        if (!cc.is_hidden())
            ++cmp;

        left_sym = cc.getSymbol();
    }

    if (cmp != left_pos)
        throw "error: char not already present in the memory";
    return left_sym;
}

int Crdt::symbolInsertion(Symbol& left_sym, int n, Symbol& symbol) {
    bool found = false;
    auto it = list.begin();
    int i = 0;
    //insert in relative position
    if (left_sym.getIncId() >= 0) {
        while(!found && it != list.end()) {
            if (it->getSymbol() == left_sym.getSymbol()) {
                while (left_sym < *it){
                    ++it;
                    ++i;
                }
                while (symbol < *it)  {
                    ++it;
                    ++i;
                }
                found = true;
            }
            ++it;
        }
    }
    int ret = i;
    i = symbol.getIncId();

    //insert symbols
    while (n-->0) {
        it = list.insert(it, Symbol(symbol.getUsrId(), i++));
        ++it;
    }
    return ret;
}

void sendAction(Action action, int usr_id, int inc_id, int left_usr_id, int left_inc_id) {

}

void receiveAction(Action action, int usr_id) {

}

void Crdt:: sendActionToServer(Action& action) {
    Symbol left_symbol = findRelativePosition(action.getCursorPos());
    //std::thread thread(sendAction, usr_id, left_symbol.getIncId(), action);  //need to test if slower
    Symbol symbol(usr_id, list.size());
    switch (action.getActionType()) {
        case Insertion:
            symbolInsertion(left_symbol, action.getNumChars(), symbol);
            break;
        case Deletion:
           // symbolDeletion(left_symbol, action.getNumChars(), symbol);
            break;
        case TextFormatting:
            break;
        case BlockFormatting:
            break;
        default:
            break;
    }
    for (Symbol s : list)
        std::cout << s.getIncId() << s.getUsrId() << ' ';
    std::cout << std::endl;
    //thread.join();
}

void Crdt::receiveActionFromServer(Action& action, int usr_id, int inc_id, int left_usr_id, int left_inc_id) {
    Symbol symbol(usr_id, inc_id);
    Symbol left_symbol(left_usr_id, left_inc_id);
    int left_pos;
    switch (action.getActionType()) {
        case Insertion:
            left_pos = symbolInsertion(left_symbol, action.getNumChars(), symbol);
            action.setCursorPos(left_pos);
            receiveAction(action, usr_id);
            break;
        case Deletion:
            break;
        case TextFormatting:
            break;
        case BlockFormatting:
            break;
        default:
            break;
    }
    for (Symbol s : list)
        std::cout << s.getIncId() << s.getUsrId() << ' ';
    std::cout << std::endl;
}
