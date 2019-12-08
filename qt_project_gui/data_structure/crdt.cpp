#include "crdt.h"
#include "symbol_id.h"
#include "../forms/mytextedit.h"
#include "../utility/json_comunicator.h"
#include "../forms/mainwindow.h"
#include <thread>
#include "exchangeable.h"
#include <exception>



class CrdtMonotonicityException : public std::exception
{
private:
    std::string message;
    ActionWrapper actionWrapper;

public:
    CrdtMonotonicityException(std::string message,ActionWrapper& actionWrapper) :
    actionWrapper(actionWrapper),message(message)
    {}

    ActionWrapper& getActionWrapper(){
        return actionWrapper;
    }

    const char* what () const throw ()
    {
        return message.c_str();
    }
};

std::pair<int, int> Crdt::findRelativePosition(int left_pos) {
    int cmp = 0;
    std::pair<int, int> left_sym = list.front().getSymbolId();   // block start tombstone

    //find left side symbol
    for (auto cc : list) {
        if (cmp == left_pos)
            break;

        if (!cc.is_hidden())
            ++cmp;

        left_sym = cc.getSymbolId();
    }

    if (cmp != left_pos)
        throw "error: char not present in the memory";
    return left_sym;
}


void Crdt::symbolInsertion(const std::pair<int, int> &left_sym, int n, const std::pair<int, int> &symbol,
                           const QString chars) {
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
    if (n > 0) --op; //balance the ++op before call
    for (int i = 0; i < n; ++i) {
        tmp_arr[i] = SymbolId(++op, symbol.second);
        if (chars[i] == '\n')
            tmp_arr[i].setBlockStart();
    }

    list.insert(it, tmp_arr, tmp_arr + n);
}

//delete done by client
std::vector<std::pair<int, int>> Crdt::symbolDeletion(int n, const std::pair<int, int> &first_symbol) {
    auto it = list.begin();
    std::vector<std::pair<int, int>> ret;

    while (it != list.end()) {
        // if first symbol found delete string and break
        if (it->getSymbolId() == first_symbol) {
            while (n-- > 0 && it != list.end())  {
                if (it->is_hidden()) {
                    ++n;
                }
                else {
                    it->hide();
                    ret.push_back(it->getSymbolId());
                }
                ++it;
            }
            break;
        }
        ++it;
    }
    return ret;
}

//formatting done by client
std::vector<std::pair<int, int>> Crdt::textFormatting(int n, const std::pair<int, int> &first_symbol, int select) {
    auto it = list.begin();
    std::vector<std::pair<int, int>> ret;

    while (it != list.end()) {

        // if first symbol found format string and break
        if (it->getSymbolId() == first_symbol) {
            while (n-- > 0) {
                if (it->is_hidden())
                    ++n;
                else {
                    //format without checking version because it's unnecessary
                    if (!it->is_blockStart()) {
                        it->setVersion(op, usr_id, select);
                        ret.push_back(it->getSymbolId());
                    }
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
std::vector<std::pair<int, int>> Crdt::blockFormatting(int n, const std::pair<int, int> &first_symbol) {
    auto it = list.begin();
    std::vector<std::pair<int, int>> ret;
    int last_block_starter;

    while (it != list.end()) {

        if (it->is_blockStart())
            last_block_starter = it - list.begin();

        // if first symbol found format string and break
        if (it->getSymbolId() == first_symbol) {
            list[last_block_starter].setVersion(op, usr_id, 0);
            ret.push_back(list[last_block_starter].getSymbolId());
            ++it;      //in case first_symbol is block_starter

            while (n-- > 1) {
                if (it->is_hidden()) {
                    ++n;
                }
                else {
                    //format without checking version because it's unnecessary
                    if (it->is_blockStart()) {
                        it->setVersion(op, usr_id, 0);
                        ret.push_back(it->getSymbolId());
                    }
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
void Crdt::sendActionToServer(Action &action, int cursorPos, int numChars) {
    std::pair<int, int> rel_symbol = findRelativePosition(cursorPos);

    std::pair<int, int> symbol(++op, usr_id);
    ActionWrapper action_wrapper(action);  //to send to json serializer

    switch (action.getActionType()) {
        case Insertion:
            symbolInsertion(rel_symbol, numChars, symbol, action.getChars());  //insert in crdt structure
            action_wrapper.rel_symbol = rel_symbol; //left symbol
            action_wrapper.symbol.push_back(symbol);
            break;
        case Deletion:
            action_wrapper.symbol = symbolDeletion(numChars, rel_symbol);
            action_wrapper.rel_symbol = symbol;
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
    json str = action_wrapper.get_json();

    auto docId = SessionData::accessToSessionData().docId;
    auto token = SessionData::accessToSessionData().token;

    // no need to free: https://stackoverflow.com/questions/26714492/how-to-release-memory-of-qthread-object
    QThread *sender = new OnlineSender(str.dump(), docId, token);
    sender->start();
//    std::cout << str;

//    //flush server buffer,  the documents only gets refreshed AFTER local action
//    while (!action_queue.empty()) {
//        receiveActionFromServer(action_queue.front());
//        action_queue.pop();
//    }
    std::cout << "Action done: " << action_wrapper.get_json() << std::endl;
}

std::vector<int> Crdt::symbolInsertionExt(const std::pair<int, int> &left_sym, int n, const std::pair<int, int> &symbol,
                                          const QString chars) {
    auto it = list.begin();
    int j = -1;

    //insert in relative position
    while (it != list.end()) {
        if (!it->is_hidden())
            ++j;
        if (left_sym == it->getSymbolId())

            break;
        ++it;
    }

    //must check that doing it once is sufficient
    if (it == list.end())
        throw "Error in database";
    ++it;
    ++j;
    while (it != list.end() && it->getSymbolId() > symbol) {  //skip symbols with greater ID
        ++it;
        ++j;
    }

    if (it != list.end() && it->getSymbolId() == symbol) return std::vector<int>();
    //insert symbols
    SymbolId tmp_arr[n];
    for (int i = 0; i < n; ++i) {
        tmp_arr[i] = SymbolId(symbol.first + i, symbol.second);
        if (chars[i] == '\n')
            tmp_arr[i].setBlockStart();
    }

    list.insert(it, tmp_arr, tmp_arr + n);

    return std::vector<int>{j};
}

//delete done by server
std::vector<int> Crdt::symbolDeletionExt(const std::vector<std::pair<int, int>> &symbol) {
    auto it = list.begin();
    auto del_it = symbol.begin();  //symbols to delete
    int i = 0;

    int a = 0;
    for (auto x : list)
        if (!x.is_hidden())
            ++a;


    std::vector<int> ret;
    while (it != list.end() && del_it != symbol.end()) {
        if (it->getSymbolId() == *del_it) {
            ++del_it;
            if (!it->is_hidden()) {
                ret.push_back(i - 1);  //get absolute position
                it->hide();
            }
        }

        ++it;
        if (it != list.end() && !it->is_hidden())
            ++i;
    }
    if (del_it != symbol.end()) throw "Not all characters could be deleted";
    return ret;
}

std::vector<int>
Crdt::formattingExt(const std::pair<int, int> &rel_symbol, const std::vector<std::pair<int, int>> &symbol, int select) {
    auto it = list.begin();
    auto form_it = symbol.begin();
    std::vector<int> ret;
    int i = 0;
    while (it != list.end() && form_it != symbol.end()) {
        //found symbol to format
        if (it->getSymbolId() == *form_it) {
            ++form_it;
            //version
            if (!it->is_hidden() && it->compareVersion(rel_symbol.first, rel_symbol.second, select)) {
                ret.push_back(i - 1);  //get absolute position
                it->setVersion(rel_symbol.first, rel_symbol.second, select);
            }
        }
         ++it;
        if (!it->is_hidden())
            ++i;
    }
    if (form_it != symbol.end()) throw "Not all characters could be formatted";
    return ret;
}

void Crdt::update_income(std::vector<ActionWrapper> actions) {

    int  b;

    for(ActionWrapper actionWrapper : action_unresolved){
        actions.push_back(actionWrapper);
    }
    // now the cycle continue again, the action_unresolved are clear and
    // after if are not already resolved are filled again
    action_unresolved.clear();

    if (actions.empty())
        return;
    //catch action wrappers sent mistakenly and belonging to the user who received them
    if (list.size() > 1) {
        int u = usr_id;
        actions.erase(std::remove_if(actions.begin(), actions.end(), [u](ActionWrapper action_wrapper) {
            if (action_wrapper.action.getActionType() == Insertion)
                return (action_wrapper.symbol.empty() || action_wrapper.symbol.front().second == u);
            return action_wrapper.rel_symbol.second == u;
        }), actions.end());
    }

    // solving the monoticity problem!!!!!
    std::sort(std::begin(actions),std::end(actions));

    for (ActionWrapper action_wrapper : actions) {
        Action &action = action_wrapper.action;
        std::vector<int> all_pos;
        try {
            switch (action.getActionType()) {
                case Insertion:
                    all_pos = symbolInsertionExt(action_wrapper.rel_symbol, action.getChars().size(),
                                                 action_wrapper.symbol.front(), action.getChars());
                    op += action.getChars().size();
                    break;
                case Deletion:
                    all_pos = symbolDeletionExt(action_wrapper.symbol);
                    ++op;
                    break;
                case TextFormatting:
                case BlockFormatting:
                    all_pos = formattingExt(action_wrapper.rel_symbol, action_wrapper.symbol,
                                            action.getSelection());
                    ++op;
                    break;
                default:
                    break;
            }
            //TODO: delete following part
            int a = 0;
            for (auto x : list)
                if (!x.is_hidden())
                    ++a;

            if (!all_pos.empty()) {
                int ownerId = action_wrapper.symbol[0].second;
                b = SessionData::accessToSessionData().myTextEditPointer->doReceivedAction(action, ownerId, all_pos);
                if (a != b)
                    qDebug() << "BIIIIG ERROR CONTACT LORENZO IF YOU SEE THIS. IMPORTANT!!!";
            }
        } catch (...) {
            // TODO: leave here for debugging purpose until the last control to understand why that error is happened
            std::cout << "DEBUG: cannot insert symbol " << action_wrapper.get_json() << std::endl;
            action_unresolved.push_back(action_wrapper);
        }

    }
//    for (SymbolId s : list)
//        std::cout << s.getIncId() << s.getUsrId() << s.is_hidden() << ' ';
//    std::cout << std::endl;
}