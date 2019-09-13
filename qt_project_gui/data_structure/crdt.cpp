#include "crdt.h"
#include "symbol_id.h"
#include <thread>

void sendAction(ActionWrapper action_wrapper) {

}

void receiveAction(Action action, int usr_id) {

}

SymbolId Crdt::findRelativePosition(int left_pos) {
    int cmp = 0;
    SymbolId left_sym = SymbolId(); //initialized -1 -1

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

int Crdt::symbolInsertion(const SymbolId& left_sym, int n, const SymbolId& symbol) {
    bool found = false;
    auto it = list.begin();
    int i = 0;
    //insert in relative position
    if (left_sym.getIncId() >= 0) {
        //find correct absolute position
        while(!found && it != list.end()) {
            if (it->getSymbolId() == left_sym.getSymbolId()) {
                //for crdt synchronization
                ++it;
                while (symbol < *it)  {
                    ++it;
                    ++i;
                }
                --it;
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

//formatting done by client, doesn't change symbol id
std::pair<SymbolId, SymbolId> Crdt::textFormatting(int n, const SymbolId& first_symbol, TextFormatType format_type) {
    auto it = list.begin();

    while (it != list.end()) {
        // if first symbol found format string and break
        if (it->getSymbolId() == first_symbol) {
            SymbolId tmp = it->getSymbolId();
            while (n-- > 0) {
                //format without checking version because it's unnecessary
                it->setTextVersion(usr_id, list.size(), format_type);
                if (n < 1)

                ++it;
            }
            return std::make_pair(tmp, it->getSymbolId());
        }
        ++it;
    }
    throw "Fatal error, symbol couldn't be found";
}

//block formatting done by client, doesn't change symbol id
std::pair<SymbolId, SymbolId> Crdt::blockFormatting(int n, const SymbolId& first_symbol, BlockFormatType format_type) {
    auto it = list.begin();
    std::pair<SymbolId, SymbolId> ret;

    while (it != list.end()) {
        // if first symbol found format string and break
        if (it->getSymbolId() == first_symbol) {
            SymbolId tmp = it->getSymbolId();
            while (n-- > 0) {
                //format without checking version because it's unnecessary
                it->setTextVersion(usr_id, list.size(), format_type);
                if (n < 1)
                    ret = std::make_pair(tmp, it->getSymbolId());
                ++it;
            }
            break;
        }
        ++it;
    }
    return ret;
}

//TODO: insertion of rich text and insertion of plain text to thr rhs of rich text
//called by text editor
void Crdt::sendActionToServer(Action& action) {
    SymbolId rel_symbol = findRelativePosition(action.getCursorPos());

    SymbolId symbol(usr_id, list.size());
    ActionWrapper action_wrapper(action);  //to send to json serializer
    std::pair<SymbolId,SymbolId> pr;

    switch (action.getActionType()) {
        case Insertion:
            symbolInsertion(rel_symbol, action.getNumChars(), symbol);  //insert in crdt structure
            action_wrapper.rel_symbol = rel_symbol; //left symbol
            action_wrapper.symbol.push_back(symbol);
            break;
        case Deletion:
            action_wrapper.rel_symbol = rel_symbol; //first symbol
            action_wrapper.symbol = symbolDeletion(action.getNumChars(), rel_symbol);
            break;
        case TextFormatting:
            pr = textFormatting(action.getNumChars(), rel_symbol, action.getTextFormatType());
            action_wrapper.rel_symbol = symbol;  //for new version
            action_wrapper.symbol.push_back(pr.first);
            action_wrapper.symbol.push_back(pr.second);
            break;
        case BlockFormatting:
            pr = blockFormatting(action.getNumChars(), rel_symbol, action.getBlockFormatType());
            action_wrapper.rel_symbol = symbol;  //for new version
            action_wrapper.symbol.push_back(pr.first);
            action_wrapper.symbol.push_back(pr.second);
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

    //for debugging
//    for (SymbolId s : list)
//        std::cout << s.getIncId() << s.getUsrId() << s.is_hidden() << '-' << s.getTextVersion(1) << '-' << s.getBlockVersion(1) << ' ';
//    std::cout << std::endl;
//    if (list.size()==2)
//    {Action a;
//    a = Action(1, 3, "XXX", Insertion);
//    SymbolId left_s(0,0);
//    SymbolId s(2,3);
//    std::vector<SymbolId> v;
//    v.push_back(s);
//    ActionWrapper aw(a, left_s, v);
//    receiveActionFromServer(aw);}
}



//delete done by server
int Crdt::symbolDeletion(const std::vector<SymbolId>& symbol) {
    auto it = list.begin();
    auto del_it = symbol.begin();  //symbols to delete
    int i = 0, ret = -1;
    while(it != list.end() && del_it != symbol.end()) {
        if (it->getSymbolId() == del_it->getSymbolId()) {
            it->hide();
            ++del_it;
            //get absolute position of first symbol
            if(ret == -1)
                ret = i;
        }
        ++it;
        ++i;
    }
    return ret;    //return only the absolute position     TODO:return position of each character in a vector
}

//TODO: insertion of rich text
void Crdt::receiveActionFromServer(ActionWrapper& action_wrapper) {
    int left_pos;
    Action action = action_wrapper.action;
    switch (action.getActionType()) {
        case Insertion:
            left_pos = symbolInsertion(action_wrapper.rel_symbol, action.getNumChars(), action_wrapper.symbol.front());
            action.setCursorPos(left_pos);
            receiveAction(action, usr_id);
            break;
        case Deletion:
            symbolDeletion(action_wrapper.symbol);
            break;
        case TextFormatting:
            //TODO: return absolute position and modify NumChars
            break;
        case BlockFormatting:
            break;
        default:
            break;
    }


//    for (SymbolId s : list)
//        std::cout << s.getIncId() << s.getUsrId() << s.is_hidden() << ' ';
//    std::cout << std::endl;
}
