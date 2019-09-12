#ifndef CRDT_H
#define CRDT_H
#include <iostream>
#include <list>
#include <QtCore/QArgument>
#include "action.h"
#include "symbol_id.h"

class Crdt {
    //Q_OBJECT

    std::list<SymbolId> list;
    int usr_id;

    Crdt(int usr_id): usr_id(usr_id) {}
    Crdt& operator=(const Crdt&) = delete;

    SymbolId findRelativePosition(int left_pos);
    int symbolInsertion(SymbolId& left_sym, int n, SymbolId& symbol);

public:
    static Crdt& getInstance(int usr_id){
       static Crdt instance(usr_id);
        return instance;
    }

    void sendActionToServer(Action& action);
    void sendActionToClient(Action& action, int usr_id, int inc_id);
    // todo: replace with SymbolId
    void receiveActionFromServer(Action& action, SymbolId left_symbol, SymbolId symbol);

public slots:
    //TODO: remember to save ABSOLUTE position of the cursor before start to apply modification
    //so the action will be coherent with the position ofthe cursor
    void actionsArrived(std::list<Action> actions);
};


#endif
