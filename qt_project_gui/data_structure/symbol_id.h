#ifndef TRANSLATOR_SYMBOLID_H
#define TRANSLATOR_SYMBOLID_H

class SymbolId {

    int usr_id;
    int inc_id;
    bool hidden;
    bool block_start;
    long long version;

public:
    SymbolId(int usr_id, int inc_id) :  usr_id(usr_id), inc_id(inc_id), hidden(), block_start(), version() {}

    SymbolId() : usr_id(-1), inc_id(-1), hidden(), block_start(), version() {}

    // you cannot cycle with < but you must use ==
    bool operator < (SymbolId second) const{
        if (this->inc_id == second.inc_id)
            return second.usr_id > this->usr_id;
        return second.inc_id > this->inc_id;
    }

    // for cycle
    bool operator == (SymbolId second)  const{
        return second.usr_id == this->usr_id && second.inc_id == this->inc_id;
    }

    // never use for cycling and find a specific element
    bool operator > (SymbolId second) const {
        return second < *this;
    }

    /*
     * hide stands for deleted, once deleted cannot be restored
     * but for the algorithm must remain in the code!
     */
    void hide(){
        this->hidden = true;
    }

    SymbolId getSymbolId() const{
        return *this;
    }

    int getIncId() const{
        return this->inc_id;
    }

    int getUsrId() const{
        return this->usr_id;
    }

    long long getVersion() const{
        return this->version;
    }

    void setVersion(int usr_id, int inc_id) {
        version = inc_id;
        version <<= 32;
        version += usr_id;
    }


    void setBlockStart() {
        this->block_start = true;
    }

    bool is_blockStart() const{
        return this->block_start;
    }

    bool is_hidden() const{
        return this->hidden;
    }
};
#endif //TRANSLATOR_SYMBOLID_H
