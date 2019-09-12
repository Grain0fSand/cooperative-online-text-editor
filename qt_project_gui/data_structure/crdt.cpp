#include "crdt.h"
#include "symbol_id.h"
#include <thread>

SymbolId Crdt::findRelativePosition(int left_pos) {
    int cmp = 0;
    SymbolId left_sym(-1,-1);

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

int Crdt::symbolInsertion(SymbolId& left_sym, int n, SymbolId& symbol) {
    bool found = false;
    auto it = list.begin();
    int i = 0;
    //insert in relative position
    if (left_sym.getIncId() >= 0) {
        while(!found && it != list.end()) {
            if (it->getSymbolId() == left_sym.getSymbolId()) {
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
        it = list.insert(it, SymbolId(symbol.getUsrId(), i++));
        ++it;
    }
    return ret;
}

void sendAction(Action action, int usr_id, int inc_id, int left_usr_id, int left_inc_id) {

  
}

void receiveAction(Action action, int usr_id) {

}

void Crdt:: sendActionToServer(Action& action) {
    SymbolId left_symbol = findRelativePosition(action.getCursorPos());
    //std::thread thread(sendAction, usr_id, left_symbol.getIncId(), action);  //need to test if slower
    SymbolId symbol(usr_id, list.size());
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
    for (SymbolId s : list)
        std::cout << s.getIncId() << s.getUsrId() << ' ';
    std::cout << std::endl;
    //thread.join();
}

void Crdt::receiveActionFromServer(Action& action, SymbolId left_symbol, SymbolId symbol) {
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
    for (SymbolId s : list)
        std::cout << s.getIncId() << s.getUsrId() << ' ';
    std::cout << std::endl;
}
