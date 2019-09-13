#ifndef CRDT_H
#define CRDT_H
#include <iostream>
#include <list>
#include <queue>
#include <QtCore/QArgument>
#include "action.h"
#include "symbol_id.h"

class Crdt {
    //Q_OBJECT

    std::list<SymbolId> list;
    int usr_id;
    std::queue<ActionWrapper> action_queue;

    Crdt() {}
    Crdt& operator=(const Crdt&) = delete;

public:
    void init(int usr_id)  {
        this->usr_id = usr_id;
    }
    static Crdt& getInstance() {
       static Crdt instance;
       return instance;
    }

    //TODO: verify usefulness
    // no race condition because of the single threaded application
    // nb: mainwindow and crdt always work on that object atomically
    std::list<SymbolId>& getSymbolList(){
        return this->list;
    }

private:

    SymbolId findRelativePosition(int left_pos);

    int symbolInsertion(const SymbolId& left_sym, int n, const SymbolId& symbol);

    //client
    std::vector<SymbolId> symbolDeletion(int n, const SymbolId& first_symbol);
    //return first and last symbol only. if only one symbol is formatted return first symbol twice
    std::pair<SymbolId, SymbolId> textFormatting(int n, const SymbolId& first_symbol, TextFormatType format_type);
    std::pair<SymbolId, SymbolId> blockFormatting(int n, const SymbolId& first_symbol, BlockFormatType format_type);

    //server
    int symbolDeletion(const std::vector<SymbolId>& symbol);

    void sendActionToClient(Action& action, SymbolId symbol);
    void receiveActionFromServer(ActionWrapper& actionWrapper);

public:
    //TODO: remember to save ABSOLUTE position of the cursor before start to apply modification
    //so the action will be coherent with the position of the cursor
    //void actionsArrived(std::list<Action> actions);   //queue non needed

    void sendActionToServer(Action& action);
   // void pushActionQueue(ActionWrapper action_wrapper);       //push server's actions
};


#endif
