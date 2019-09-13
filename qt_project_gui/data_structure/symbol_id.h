#ifndef TRANSLATOR_SYMBOLID_H
#define TRANSLATOR_SYMBOLID_H

class SymbolId {

    int usr_id;
    int inc_id;
    bool hidden;
    long long text_version[6], block_version[6];

public:
    SymbolId(int usr_id, int inc_id) :  usr_id(usr_id), inc_id(inc_id), hidden(false), text_version(), block_version() {}

    SymbolId() :    usr_id(-1), inc_id(-1), hidden(false), text_version(), block_version() {}

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

    long long getTextVersion(int enum_index) const{
        return this->text_version[enum_index];
    }

    void setTextVersion(int usr_id, int inc_id, int enum_index) {
        text_version[enum_index] = inc_id;
        text_version[enum_index] <<= 32;
        text_version[enum_index] += usr_id;
    }

    long long getBlockVersion(int enum_index) const{
        return this->block_version[enum_index];
    }

    void setBlockVersion(int usr_id, int inc_id, int enum_index) {
        block_version[enum_index] = inc_id;
        block_version[enum_index] <<= 32;
        block_version[enum_index] += usr_id;
    }

    // return if you need to show the char
    bool is_hidden() const{
        return this->hidden;
    }
};
#endif //TRANSLATOR_SYMBOLID_H
