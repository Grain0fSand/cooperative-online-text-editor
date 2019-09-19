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

    std::vector<SymbolId> list;
    int usr_id;
    std::queue<ActionWrapper> action_queue;

    Crdt() {}
    Crdt& operator=(const Crdt&) = delete;

public:
    void init(int usr_id)  {
        this->usr_id = usr_id;
        //TODO add loading from server and insert of block starter
        //TODO add nbsp to text editor
        if (list.empty()) {
            list.push_back(SymbolId());
            list.front().setBlockStart();
        }
    }
    static Crdt& getInstance() {
       static Crdt instance;
       return instance;
    }

    //TODO: verify usefulness
    // no race condition because of the single threaded application
    // nb: mainwindow and crdt always work on that object atomically
    std::vector<SymbolId>& getSymbolList(){
        return this->list;
    }

private:

    SymbolId findRelativePosition(int left_pos);

    void symbolInsertion(const SymbolId& left_sym, int n, const SymbolId& symbol, const QString chars);

    // TODO: check if necessary lock shared between local and server updates
    //client
    std::vector<SymbolId> symbolDeletion(int n, const SymbolId& first_symbol);
    std::vector<SymbolId> textFormatting(int n, const SymbolId& first_symbol);
    std::vector<SymbolId> blockFormatting(int n, const SymbolId& first_symbol);

    //server
    // TODO: chiedere a dario se è il caso di salvare la posizione relativa del cursore e ripristinarla dopo
    // TODO: aver applicato le azioni remote
    std::vector<int> symbolInsertionExt(const std::pair<int,int>& left_sym, int n, const std::pair<int,int>& symbol, const QString chars);
    std::vector<int> symbolDeletionExt(const std::vector<std::pair<int,int>>& symbol);
    std::vector<int> formattingExt(const std::pair<int,int>& rel_symbol, const std::vector<std::pair<int,int>>& symbol);

    void sendActionToClient(Action& action, SymbolId symbol);
    void receiveActionFromServer(ActionWrapper& actionWrapper);

public:
    //TODO: remember to save ABSOLUTE position of the cursor before start to apply modification
    //so the action will be coherent with the position of the cursor
    //void actionsArrived(std::list<Action> actions);   //queue non needed

    void sendActionToServer(Action& action, int cursorPos, int numChars);
   // void pushActionQueue(ActionWrapper action_wrapper);       //push server's actions
};


#endif
