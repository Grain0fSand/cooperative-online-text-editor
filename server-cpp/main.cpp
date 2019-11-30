#include <nlohmann/json.hpp>
#include "database.h"
#include "crow_all.h"
#include <algorithm>

std::string Database::dbUri = "../pds.db";

/*
 * function to prevent input to be dangerous for the db
 */

std::vector<std::string> split(const std::string& s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}


void str_replace(std::string & data, std::string toSearch, std::string replaceStr)
{
    // Get the first occurrence
    size_t pos = data.find(toSearch);

    // Repeat till end is reached
    while( pos != std::string::npos)
    {
        // Replace this occurrence of Sub String
        data.replace(pos, toSearch.size(), replaceStr);
        // Get the next occurrence from the current position
        pos =data.find(toSearch, pos + replaceStr.size());
    }
}

void sanitize(std::string &stringValue)
{
    // Add backslashes.
    for (auto i = stringValue.begin();;) {
        auto const pos = std::find_if(
                i, stringValue.end(),
                [](char const c) { return '\\' == c || '\'' == c || '"' == c; }
        );
        if (pos == stringValue.end()) {
            break;
        }
        i = std::next(stringValue.insert(pos, '\\'), 2);
    }

    // Removes others.
    stringValue.erase(
            std::remove_if(
                    stringValue.begin(), stringValue.end(), [](char const c) {
                        return '\n' == c || '\r' == c || '\0' == c || '\x1A' == c;
                    }
            ),
            stringValue.end()
    );
}


int main() {
    Database db;
    crow::SimpleApp app;

    CROW_ROUTE(app,"/try_registration")
            .methods("GET"_method)
                ([&](const crow::request& req){
                    auto params = req.url_params;

                    if(params.get("email") == nullptr || params.get("username") == nullptr
                       || params.get("password") == nullptr || params.get("image")==nullptr)
                        return crow::response(500);

                    std::string email = params.get("email");
                    std::string username = params.get("username");
                    std::string password = params.get("password");
                    std::string image = params.get("image");

                    sanitize(email);
                    sanitize(username);
                    sanitize(password);
                    // sanitize(image); maybe sanitize can lose information, better binding

                    int replyCode = db.userRegistration(email,username,password,image);

                    return crow::response{std::to_string(replyCode)};
                });

    CROW_ROUTE(app,"/new_doc")
            .methods("GET"_method)
                    ([&](const crow::request& req){
                        auto params = req.url_params;

                        if(params.get("token") == nullptr || params.get("docName") == nullptr)
                            return crow::response(500);

                        std::string token = params.get("token");
                        std::string docName = params.get("docName");

                        sanitize(token);
                        sanitize(docName);

                        std::string replyString = db.newDocument(token,docName);

                        return crow::response{replyString};
                    });

    CROW_ROUTE(app,"/get_partecipants")
            .methods("GET"_method)
                    ([&](const crow::request& req){
                        auto params = req.url_params;

                        if(params.get("token") == nullptr || params.get("docName") == nullptr)
                            return crow::response(500);

                        std::string token = params.get("token");
                        std::string docName = params.get("docName");

                        sanitize(token);
                        sanitize(docName);

                        std::string dbReply = db.getPartecipants(token,docName);

                        return crow::response{dbReply};
                    });

    CROW_ROUTE(app,"/try_login")
            .methods("GET"_method)
                    ([&](const crow::request& req){
                        auto params = req.url_params;

                        if(params.get("username") == nullptr || params.get("password") == nullptr)
                            return crow::response(500);

                        std::string username = params.get("username");
                        std::string password = params.get("password");

                        sanitize(username);
                        sanitize(password);

                        std::string dbReply = db.userLogin(username,password);

                        return crow::response{dbReply};
                    });

    CROW_ROUTE(app,"/update_user_data")
           .methods("GET"_method)
                ([&](const crow::request& req){

                    auto params = req.url_params;

                    if(params.get("token") == nullptr || params.get("username") == nullptr
                       || params.get("avatar") == nullptr || params.get("password") == nullptr)
                        return crow::response(500);

                    std::string token = params.get("token");
                    std::string username = params.get("username");
                    std::string avatar = params.get("avatar");
                    std::string password = params.get("password");

                    sanitize(token);
                    sanitize(username);
                    // sanitize(avatar); maybe lose information, better binding
                    sanitize(password);

                    int dbReply = db.updateUserData(token,username,avatar,password);

                    return crow::response{std::to_string(dbReply)};

                });

    // to test launch and open
    // http://localhost:6969/get_crdt?token=1&lastcrdt=&docId=1&remoteCursor=0
    CROW_ROUTE(app,"/get_crdt")
            .methods("GET"_method)
                    ([&](const crow::request& req){
                        auto params = req.url_params;

                        if(params.get("token") == nullptr || params.get("lastcrdt") == nullptr
                            || params.get("docId") == nullptr || params.get("remoteCursor") == nullptr)
                            // TODO: see remoteCursor param
                            return crow::response(500);

                        std::string token = params.get("token");
                        std::string lastcrdt = params.get("lastcrdt");
                        std::string docId = params.get("docId");
                        std::string remoteCursor = params.get("remoteCursor");

                        sanitize(token);
                        sanitize(lastcrdt);
                        sanitize(docId);
                        // sanitize(remoteCursor); the remote cursor is an json value so it must not be sanitized

                        int idUser = db.userLogged(token);

                        if(idUser<0)
                            return crow::response(403);

                        std::vector<exchangeable_data::send_data> d = db.getCrdtUser(lastcrdt,std::to_string(idUser),docId,remoteCursor);
                        // TODO: check if now gui crash
                        json j;

                        for(exchangeable_data::send_data t : d){
                            j.push_back(t.get_json());
                        }

                        json k;

                        std::vector<exchangeable_data::user> u = db.getOnlineUsers();

                        for(exchangeable_data::user t : u){
                            k.push_back(t.get_json());
                        }

                        json l;
                        l.push_back(j);
                        l.push_back(k);


                        std::ostringstream os;

                        // fill the reply
                        os << l;

                        return crow::response{os.str()};
                    });

    CROW_ROUTE(app,"/format_db")
            .methods("GET"_method)
                    ([&](const crow::request& req){
                        auto params = req.url_params;

                        if(params.get("pass") == nullptr)
                            // TODO: see remoteCursor param
                            return crow::response(500);

                        std::string pass = params.get("pass");

                        if(pass.compare(std::string("porcodio"))!=0)
                            return crow::response(500);


                        db.eraseDB();


                        return crow::response{"ti eraso il db"};
                    });

    // to test launch and open
    // TODO: add remoteCursor parameter
    // http://localhost:8080/push_crdt?token=1&crdt=mannaggia il porco&docId=1
    CROW_ROUTE(app,"/push_crdt")
            .methods("POST"_method)
                    ([&](const crow::request& req){
                        std::string all_params = req.body;
                        std::vector<std::string> params = split(all_params,'&');
                        std::unordered_map<std::string, std::string> parameters;

                        for(auto it = std::begin(params);it != std::end(params);it++){
                            std::vector<std::string> param = split(*it,'=');
                            parameters[param[0]] = param[1];
                        }

                        if(!(parameters.find(std::string("token")) != parameters.end() && parameters.find("crdt")!=parameters.end() && parameters.find("docId") != parameters.end()))
                            return crow::response(500);

                        std::string uid = parameters["token"];
                        std::string crdt = parameters["crdt"];
                        str_replace(crdt,"%3D","=");
                        std::string docId = parameters["docId"];

                        sanitize(uid);
                        // sanitize(crdt); nb: crdt can contain all the charapters, so it must not be sanitized!!!!
                        sanitize(docId);

                        std::cout << crdt << std::endl;

                        int idUser = db.userLogged(uid);
                        if(idUser<0)
                            return crow::response(403);

                        db.insertCrdt(crdt,std::to_string(idUser),docId);

                        std::ostringstream os;

                        // fill the reply
                        os << "1";

                        return crow::response{os.str()};
                    });

    app.port(6969)
    //.multithreaded() // active only if you need more performance
    .run();

    return 0;
}




/*
    QUERY INSERIMENTO DB:
    INSERT INTO user VALUES(NULL,'user','5e884898da28047151d0e56f8dc6292773603d0d6aabbdd62a11ef721d1542d8','dario.patti@elia.migliore.com','image');
    INSERT INTO user_document_request VALUES(1,1,datetime('now'),'aaa')
    INSERT INTO document VALUES(NULL,'documento di prova')
    INSERT INTO crdt(idDoc,idUser,crdt_json)  VALUES(NULL,1,'crdt_json')
    insert into crdt_delvery(idCrdt,idUser,idDocument) values(1,1,1)
 */

