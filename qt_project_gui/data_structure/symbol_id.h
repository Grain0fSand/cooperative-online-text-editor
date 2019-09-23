#ifndef TRANSLATOR_SYMBOLID_H
#define TRANSLATOR_SYMBOLID_H


//TODO: version for every style?
class SymbolId {
    int inc_id;
    int usr_id;

    bool hidden;
    bool block_start;
    long long version[5];

public:
    SymbolId(int inc_id, int usr_id) :  inc_id(inc_id), usr_id(usr_id), hidden(), block_start(), version() {}

    SymbolId() : inc_id(-1), usr_id(-1), hidden(), block_start(), version() {}

//    // you cannot cycle with < but you must use ==
//    bool operator < (SymbolId second) const{
//        if (this->inc_id == second.inc_id)
//            return second.usr_id > this->usr_id;
//        return second.inc_id > this->inc_id;
//    }
//
//    // for cycle
//    bool operator == (SymbolId second)  const{
//        return second.usr_id == this->usr_id && second.inc_id == this->inc_id;
//    }
//
//    // never use for cycling and find a specific element
//    bool operator > (SymbolId second) const {
//        return second < *this;
//    }

    /*
     * hide stands for deleted, once deleted cannot be restored
     * but for the algorithm must remain in the code!
     */
    void hide(){
        this->hidden = true;
    }

    std::pair<int,int> getSymbolId() const{
        return std::make_pair(inc_id, usr_id);
    }

    int getIncId() const{
        return this->inc_id;
    }

    int getUsrId() const{
        return this->usr_id;
    }

    void setVersion(int inc_id, int usr_id, int select) {
        version[select] = inc_id;
        version[select] <<= 32;
        version[select] += usr_id;
    }

    bool compareVersion(int inc_id, int usr_id, int select) {
        long long version_tmp = inc_id;
        version_tmp <<= 32;
        version_tmp += usr_id;
        return version_tmp > version[select];
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
