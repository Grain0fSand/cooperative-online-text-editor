#ifndef PDSPROJECT_GUI_CDRT_H
#define PDSPROJECT_GUI_CDRT_H
#include <iostream>
#include <list>
#include "action.h"

class Symbol {

    int usr_id;
    int inc_id;
    bool hidden;

public:
    Symbol(int usr_id, int inc_id) : usr_id(usr_id), inc_id(inc_id), hidden(false) {}

    // you cannot cycle with < but you must use ==
    bool operator < (Symbol second) {
        if (this->inc_id == second.inc_id)
                return second.usr_id > this->usr_id;
        return second.inc_id > this->inc_id;
    }

    // for cycle
    bool operator == (Symbol second) {
        return second.usr_id == this->usr_id && second.inc_id == this->inc_id;
    }

    // never use for cycling and find a specific element
    bool operator > (Symbol second) {
        return second < *this;
    }

    /*
     * hide stands for deleted, once deleted cannot be restored
     * but for the algorithm must remain in the code!
     */
    void hide(){
        this->hidden = true;
    }


    Symbol getSymbol(){
        return *this;
    }

    int getIncId(){
        return this->inc_id;
    }

    int getUsrId(){
        return this->usr_id;
    }

    // return if you need to show the char
    bool is_hidden(){
        return this->hidden;
    }
};

class Crdt {
    std::list<Symbol> list;
    int usr_id;

    Crdt(int usr_id): usr_id(usr_id) {}
    Crdt& operator=(const Crdt&) = delete;

    Symbol findRelativePosition(int left_pos);
    int symbolInsertion(Symbol& left_sym, int n, Symbol& symbol);

public:
    static Crdt& getInstance(int usr_id){
       static Crdt istance(usr_id);
        return istance;
    }

    void sendActionToServer(Action& action);
    void sendActionToClient(Action& action, int usr_id, int inc_id);
    void receiveActionFromServer(Action& action, int usr_id, int inc_id, int left_usr_id, int left_inc_id);
};


#endif //PDSPROJECT_GUI_CDRT_H
