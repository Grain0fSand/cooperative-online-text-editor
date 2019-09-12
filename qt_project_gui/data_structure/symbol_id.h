//
// Created by lorenzo on 11/09/19.
//

#ifndef TRANSLATOR_SYMBOLID_H
#define TRANSLATOR_SYMBOLID_H

class SymbolId {

    int usr_id;
    int inc_id;
    bool hidden;

public:
    SymbolId(int usr_id, int inc_id) : usr_id(usr_id), inc_id(inc_id), hidden(false) {}

    // you cannot cycle with < but you must use ==
    bool operator < (SymbolId second) {
        if (this->inc_id == second.inc_id)
            return second.usr_id > this->usr_id;
        return second.inc_id > this->inc_id;
    }

    // for cycle
    bool operator == (SymbolId second) {
        return second.usr_id == this->usr_id && second.inc_id == this->inc_id;
    }

    // never use for cycling and find a specific element
    bool operator > (SymbolId second) {
        return second < *this;
    }

    /*
     * hide stands for deleted, once deleted cannot be restored
     * but for the algorithm must remain in the code!
     */
    void hide(){
        this->hidden = true;
    }


    SymbolId getSymbolId(){
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
#endif //TRANSLATOR_SYMBOLID_H
