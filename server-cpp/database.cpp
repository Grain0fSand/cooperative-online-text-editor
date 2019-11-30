#include "database.h"

Database::Database() :
    db(dbUri, SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE)
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

std::string Database::userLogin(std::string username,std::string password)
{
    std::string hashedPass = hashed_pass(password);
    bool isLoginCorrect;
    std::string response = "0";

    std::string sql = "SELECT * FROM users WHERE username='" + username + "' AND password='" + hashedPass + "'";
    SQLite::Statement query(db, sql);
    try {
        isLoginCorrect = query.executeStep();
    } catch (SQLite::Exception &error) {
        std::cout << error.getExtendedErrorCode() << " - " << error.what();
        throw;
    }

    if (isLoginCorrect)
    {
        response = "1";

        int userId = query.getColumn(0);
        std::string encodedUserAvatar = query.getColumn(4);
        std::string token = random_string(40);

        response += ":" + std::to_string(userId);
        response += ":" + token;
        response += ":" + encodedUserAvatar;
        response += ":";

        sql = "SELECT name FROM document";

        SQLite::Statement getDocsName(db, sql);
        while(getDocsName.executeStep()) {
            std::string docName = getDocsName.getColumn(0);
            response += docName + "|";
        }

        sessionLogged[token] = userId;
    }
    return response;
}

int Database::userRegistration(std::string email,std::string username,std::string password,std::string image)
{
    std::string hashedPass = hashed_pass(password);

    std::string sql = "INSERT INTO users(email,username,password,image) VALUES('" + email + "','" + username + "','" + hashedPass + "',?)";
    SQLite::Statement query(db, sql);

    query.bind(1,image);

    try {
        query.exec();
    } catch (SQLite::Exception &error) {
        if(error.getExtendedErrorCode()==2067) { //UNIQUE CONSTRAINT
            std::string details = error.what();
            std::string column = details.substr(details.find_last_of('.')+1);
            if(column=="email")
                return 1;
            else if(column=="username")
                return 2;
        }
        else throw;
    }
    return 0;
}

std::string Database::newDocument(std::string token,std::string docName) {
    std::string query = "INSERT INTO document(name) VALUES('" + docName + "')";
    SQLite::Statement insertStat(db,query);

    try {
        insertStat.exec();
    } catch (SQLite::Exception &error) {
        if(error.getExtendedErrorCode()==2067) { //UNIQUE CONSTRAINT
            return "0";
        }
        else throw;
    }
    query = "SELECT id FROM document WHERE name='" + docName + "'";
    SQLite::Statement selectStat(db,query);
    selectStat.executeStep();

    std::string docId = selectStat.getColumn(0);
    int uid = userLogged(token);

    addPartecipant(docId,std::to_string(uid));
    return "1|" + docId;
}

std::string Database::getPartecipants(std::string token,std::string docName)
{
    int uid = userLogged(token);

    std::string query = "SELECT id FROM document WHERE name='" + docName + "'";
    SQLite::Statement findDocId(db,query);
    findDocId.executeStep();

    std::string docId = findDocId.getColumn(0);

    // why there was that here?
    addPartecipant(docId,std::to_string(uid));

    query = "SELECT id,email,username,image FROM users WHERE id IN "
            "(SELECT idUser FROM user_document_request WHERE idDocument=" + docId + ")";
    SQLite::Statement findUsers(db,query);

    std::string id,email,username,image;
    std::string reply = docId;

    while(findUsers.executeStep()) {
        id = findUsers.getColumn(0).getString();
        email = findUsers.getColumn(1).getString();
        username = findUsers.getColumn(2).getString();
        image = findUsers.getColumn(3).getString();

        reply += "|" + id + ":" + email + ":" + username + ":" + image;
    }
    if(reply.length()==docId.length())
        reply = "0";

    return reply;
}

int Database::updateUserData(std::string token,std::string username,std::string image,std::string password)
{
    int userId = userLogged(token);

    std::string sql = "UPDATE users SET username='" + username + "', image='" + image + "', password='" + hashed_pass(password) + "' WHERE id='" + std::to_string(userId) + "'";
    SQLite::Statement query(db, sql);
    try {
        query.exec();
    } catch (SQLite::Exception &error) {
        if(error.getExtendedErrorCode()==2067) { //UNIQUE CONSTRAINT
            return 0;
        }
        else throw;
    }
    return 1;
}

void Database::addPartecipant(std::string docId,std::string uid)
{
    std::string sql = "INSERT OR IGNORE INTO user_document_request(idUser,idDocument,lastReq) "
                      "VALUES(" + uid + "," + docId + ",datetime('now','localitime'))";

    SQLite::Statement query(db,sql);
    query.exec();

    updateTimestamp(docId,uid);
}

void Database::updateTimestamp(std::string docId,std::string uid)
{
    std::string sql = "UPDATE user_document_request SET lastReq = datetime('now','localtime') "
                      "WHERE idUser = " + uid + " AND idDocument = " + docId;
    SQLite::Statement query(db,sql);
    query.exec();
}

void Database::updateTimestamp(std::string docId,std::string uid,std::string remoteCursor)
{
    std::string sql = "UPDATE user_document_request SET lastReq = datetime('now','localtime'), cursor_position_json = ? WHERE idUser = " + uid + " AND idDocument = " + docId;
    SQLite::Statement query(db,sql);
    query.bind(1,remoteCursor);
    query.exec();
}

/*
     * the operation of insert crdt create the crdt, and after create a notification
     * for each user subscribed to the document, the subscription is seen if a user
     * has a row inside the table user_document_request
     */
void Database::insertCrdt(std::string crdt_json,std::string uid,std::string docId)
{
    std::string sql = "INSERT INTO crdt(idDoc,idUser,crdt_json)  VALUES(" + docId + "," + uid + ",?);";

    SQLite::Statement queryUpdate(db,sql);
    queryUpdate.bind(1,crdt_json);
    auto res = queryUpdate.exec();
//    int idCrdtInserted = db.getLastInsertRowid();
//
//    sql = "SELECT idUser FROM user_document_request WHERE idDocument=" + docId + ";";
//    SQLite::Statement idGetter(db,sql);
//    std::vector<int> ids;
//
//
//    while (idGetter.executeStep()){
//        ids.push_back(idGetter.getColumn(0));
//    }
//
//    std::stringstream ss{uid};
//
//    int creator_id;
//
//    ss >> creator_id;
//
//    ids.erase(std::remove(ids.begin(),ids.end(),creator_id),ids.end());
//    std::string crdtId = std::to_string(idCrdtInserted);
//
//    for(int usrId : ids){
//        sql = "INSERT INTO crdt_delvery(idDoc,idCrdt,idUser) VALUES(" + docId + "," + crdtId + "," + std::to_string(usrId) + ")";
//        SQLite::Statement insertNotification(db,sql);
//        insertNotification.exec();
//    }

    updateTimestamp(docId,uid);
}

/*
     * it returns the list of unseen crdt given a user, a document and the last
     * seen document; it ensure also the statistics of witch crdt has been seen
     * by a user
     */
std::vector<exchangeable_data::send_data> Database::getCrdtUser(std::string lastCrdtId,std::string uid,std::string docId,std::string remoteCursor)
{
    // TODO: add lorenzo's parameter cursorPos

    std::string sql;

//    if (lastCrdtId!=""){
//        sql = "UPDATE crdt_delvery SET delivered = '1' WHERE idDoc=" + docId + " AND idCrdt <=" + lastCrdtId + " AND idUser = " + uid + ";";
//
//        SQLite::Statement queryUpdate(db,sql);
//        queryUpdate.exec();
//
//
//        sql = "SELECT id,crdt_json FROM crdt WHERE id IN (SELECT idCrdt from crdt_delvery WHERE idUser=" + uid + " AND idDoc=" + docId + "  AND delivered=0)";
//    } else {
//        sql = "SELECT id,crdt_json FROM crdt WHERE id IN (SELECT idCrdt from crdt_delvery WHERE idUser=" + uid + " AND idDoc=" + docId + ")";
//    }

    if (lastCrdtId == "")
        sql = "SELECT id, crdt_json FROM crdt WHERE idDoc = " + docId;
    else
        sql = "SELECT id, crdt_json FROM crdt WHERE idDoc = " + docId + " AND id > " + lastCrdtId + " AND idUser != " + uid;

    SQLite::Statement query(db, sql);
    std::vector<exchangeable_data::send_data> vect;

    while (query.executeStep()){
        std::string id = query.getColumn(0);
        std::string crdt_json = query.getColumn(1);

        vect.push_back(exchangeable_data::send_data(id,crdt_json));
    }

    updateTimestamp(docId,uid,remoteCursor);

    return vect;
}

std::string Database::hashed_pass(std::string pass)
{
    //TODO: io dico che questo ci dimentichiamo di modificarlo
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

std::vector<exchangeable_data::user>
Database::getOnlineUsers() {

    // TODO: replace 90000 with 30, is the seconds, note that there are 2 90000 inside the query
    std::string sql = "select id,email,username,image,(select cursor_position_json from user_document_request where lastReq >= datetime((strftime('%s','now')-90000),'unixepoch','localtime')) as json_cursor from users where id in (select idUser from user_document_request where lastReq >= datetime((strftime('%s','now')-90000),'unixepoch','localtime'))";

    SQLite::Statement query(db, sql);
    std::vector<exchangeable_data::user> vect;

    while (query.executeStep()) {
        std::string id = query.getColumn(0);
        std::string email = query.getColumn(1);
        std::string username = query.getColumn(2);
        std::string image = query.getColumn(3);
        std::string json_cursor = query.getColumn(4);

        vect.push_back(exchangeable_data::user(id, email, username, image, json_cursor));
    }


    return vect;
}

void Database::eraseDB() {
    std::string sql = "delete from crdt;delete from crdt_delvery;delete from document;delete from user_document_request;";
    db.exec(sql);
}
