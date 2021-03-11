#ifndef CRDT_H
#define CRDT_H
#include <iostream>
#include <list>
#include <queue>
#include <QtCore/QArgument>
#include <QtCore/QObject>
#include "action.h"
#include "symbol_id.h"
#include "../web_requests/online_sender.h"

class Crdt : public QObject {
    Q_OBJECT

    std::vector<SymbolId> list;
    int usr_id;
    unsigned int op = 0; //incremental counter
    // for managing the non monotonicity problem in the messages
    std::vector<ActionWrapper> action_unresolved;

    Crdt() {}
    Crdt& operator=(const Crdt&) = delete;

public:
    void init(int usr_id)  {
        this->usr_id = usr_id;
        if (list.empty()) {
            list.push_back(SymbolId());
            list.front().setBlockStart();
            list.front().hide();
        }
    }
    static Crdt& getInstance() {
       static Crdt instance;
       return instance;
    }

    void reset();
    std::vector<SymbolId>& getSymbolList(){
        return this->list;
    }


public slots:
    void update_income(std::vector<ActionWrapper> actions);

signals:
    void needToResetLastCrdtId();

private:

    //client
    void symbolInsertion(const std::pair<int,int>& left_sym, int n, const std::pair<int,int>& symbol, const QString chars);
    std::vector<std::pair<int,int>> symbolDeletion(int n, const std::pair<int,int>& first_symbol);
    std::vector<std::pair<int,int>> textFormatting(int n, const std::pair<int,int>& first_symbol, int select);
    std::vector<std::pair<int,int>> blockFormatting(int n, const std::pair<int,int>& first_symbol);

    //server
    std::vector<int> symbolInsertionExt(const std::pair<int,int>& left_sym, int n, const std::pair<int,int>& symbol, const QString chars);
    std::vector<int> symbolDeletionExt(const std::vector<std::pair<int,int>>& symbol);
    std::vector<int> formattingExt(const std::pair<int,int>& rel_symbol, const std::vector<std::pair<int,int>>& symbol, int select);

public:
    //so the action will be coherent with the position of the cursor
    //void actionsArrived(std::list<Action> actions);   //queue non needed
    void receiveActionFromServer(ActionWrapper& actionWrapper);
    void sendActionToServer(Action& action, int cursorPos, int numChars);
    std::pair<int,int> findRelativePosition(int left_pos);
    int findAbsolutePosition(std::pair<int,int>);

};


#endif
