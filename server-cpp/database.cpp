#include "database.h"

Database::Database() :
    db(dbUri, SQLite::OPEN_READWRITE)
{
    // TODO: only for test, remove it
    std::cout << "db opened";
    sessionLogged["1"] = 1;
    sessionLogged["2"] = 2;
}

int Database::userLogged(std::string token)
{
    if (sessionLogged.find(token) != sessionLogged.end())
        return sessionLogged[token];

    return -1;
}

void Database::userLogin(std::string username,std::string password)
{
    std::string hashedPass = hashed_pass(password);
    bool isLoginCorrect = true;


    // TODO: check login is correct
    if (isLoginCorrect) {
        std::string token = random_string(40);
        int id = 15;

        sessionLogged[token] = id;
    }
}

void Database::userRegistration(std::string email,std::string username,std::string password)
{
    std::string hashedPass = hashed_pass(password);

    std::string sql = "INSERT INTO users(email,username,password) VALUES('" + email + "','" + username + "','" + hashedPass + "')";
    SQLite::Statement query(db,sql);
    query.exec();
}

//TODO: ricordati di inviare tutto lo storico dei crdt
void Database::addPartecipant(std::string docId,std::string uid)
{
    std::string sql = "INSERT INTO user_document_request VALUES(" + uid + "," + docId + ",datetime('now'))";
    SQLite::Statement query(db,sql);
    query.exec();
}

void Database::updateTimestamp(std::string docId,std::string uid)
{
    std::string sql = "UPDATE user_document_request SET lastReq = datetime('now') WHERE idUser = " + uid + " AND idDocument = " + docId + ";)";
    SQLite::Statement query(db,sql);
    query.exec();
}

/*
     * the operation of insert crdt create the crdt, and after create a notification
     * for each user subscribed to the document, the subscription is seen if a user
     * has a row inside the table user_document_request
     */
void Database::insertCrdt(std::string crdt_json,std::string uid,std::string docId)
{
    std::string sql = "INSERT INTO crdt(idDoc,idUser,crdt_json)  VALUES(" + docId + "," + uid + ",'" + crdt_json + "');";

    SQLite::Statement queryUpdate(db,sql);
    auto res = queryUpdate.exec();
    int idCrdtInserted = db.getLastInsertRowid();

    sql = "SELECT idUser FROM user_document_request WHERE idDocument=" + docId + ";";
    SQLite::Statement idGetter(db,sql);
    std::vector<int> ids;


    while (idGetter.executeStep()){
        ids.push_back(idGetter.getColumn(0));
    }

    std::stringstream ss{uid};

    int creator_id;

    ss >> creator_id;

    ids.erase(std::remove(ids.begin(),ids.end(),creator_id),ids.end());
    std::string crdtId = std::to_string(idCrdtInserted);

    for(int usrId : ids){
        sql = "INSERT INTO crdt_delvery(idDoc,idCrdt,idUser) VALUES(" + docId + "," + crdtId + "," + std::to_string(usrId) + ")";
        SQLite::Statement insertNotification(db,sql);
        insertNotification.exec();
    }
}

/*
     * it returns the list of unseen crdt given a user, a document and the last
     * seen document; it ensure also the statistics of witch crdt has been seen
     * by a user
     */
std::vector<exchangeable_data::send_data> Database::getCrdtUser(std::string lastCrdtId,std::string uid,std::string docId)
{
    std::string sql;

    if (lastCrdtId.compare("")!=0){
        sql = "UPDATE crdt_delvery SET delivered = '1' WHERE idDoc=" + docId + " AND idCrdt <=" + lastCrdtId + " AND idUser = " + uid + ";";

        SQLite::Statement queryUpdate(db,sql);
        queryUpdate.exec();


        sql = "SELECT id,crdt_json FROM crdt WHERE id IN (SELECT idCrdt from crdt_delvery WHERE idUser=" + uid + " AND idDoc=" + docId + "  AND delivered=0)";
    } else {
        sql = "SELECT id,crdt_json FROM crdt WHERE id IN (SELECT idCrdt from crdt_delvery WHERE idUser=" + uid + " AND idDoc=" + docId + ")";
    }

    std::cout << sql;

    SQLite::Statement query(db, sql);
    std::vector<exchangeable_data::send_data> vect;

    while (query.executeStep()){
        std::string id = query.getColumn(0);
        std::string crdt_json = query.getColumn(1);

        vect.push_back(exchangeable_data::send_data(id,crdt_json));
    }

    return vect;
}

std::string Database::hashed_pass(std::string pass)
{
    return sha256(pass + "my crazy random salt porcamadonna PDS");
}

std::string Database::random_string(size_t length)
{
    auto randchar = []() -> char
    {
        const char charset[] =
                "0123456789"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}