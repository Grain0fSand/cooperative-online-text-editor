#ifndef JSON_COMUNICATOR_H
#define JSON_COMUNICATOR_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


class JsonComunicator
{
public:
    JsonComunicator();


private:
    QJsonDocument  json;
    QJsonArray recordsArray;
    QJsonObject  recordObject;
};

#endif // JSON_COMUNICATOR_H

/*
----------EXAMPLE-OF-JSON-STRUCURE-------------------------

pre configuration:

document_vector="la vita come sempre,"
version=15

message:

position is referred to the previus position

{
   "inserter_id":12,
   "version":15,
   "inserctions":[
      {
         "position":0,
         "value":"come va "
      },
      {
         "position":7,
         "value":"?"
      }
   ],
   "deletions":[
      "position":20
   ]
}


if the server has a version greater than that version, for example the 18
with this internal status:

document_vector: "La vita è come sempre, amara come il c++"
version=18

it takes from the db the actions that has been committed and the return
message will be:
{
   "status":"rejected",

   "version_to_recovery": [
   {
       "version":16,
       "inserctions":[
          {
             "inserter_id":15,
             "position":0,
             "value":"L"
          },
          {
             "inserter_id":7,
             "position":0,
             "value":" è"
          }
       ],
       "deletions":[
            "position":0,
       ]
   }, // status after that: document_vector="La vita è come sempre,"
   {
       "version":17,
       "inserctions":[
          {
             "inserter_id":15,
             "position":22,
             "value":"amara come il c++"
          }
       ]
   }, // status after that: document_vector="La vita è come sempre,amara come il c++"
   {
       "version":18,
       "inserctions":[
          {
             "inserter_id":15,
             "position":22,
             "value":" "
          }
       ]
   }
   ],
   "online_users" : [
   {
       "username" : "giancarlo",
       "color" : "#ffab20",
       "image" : "https://www.website.com/images/image_18:10:23:10:04:00.png"
   },
   {
       "username" : "pierluigi",
       "color" : "#fabc19",
       "image" : "https://www.website.com/images/image_19:11:21:15:25:24.png"
   }
   ]
}

and the memory of the editor after retriving
 and elaborating will be ereased and replaced by this message:

document_vector: "La vita è come sempre, amara come il c++"
version=18

case of accepted message and reply:
now the modification became visible from the reply to all the users connected,
example of message of reply considering that only one users is connected:


{
    "modifications": [
        "inserter_id": 12
        "inserctions": [
            {
                "position": 0,
                "value": "come va "
            },
            {
                "position": 7,
                "value": "?"
            }
        }
        "deletions": {
            {
                "position": 20
            }
        }
    ],
   "online_users" : [
   {
       "username" : "giancarlo",
       "color" : "#ffab20",
       "image" : "https://www.website.com/images/image_18:10:23:10:04:00.png"
   },
   {
       "username" : "pierluigi",
       "color" : "#fabc19",
       "image" : "https://www.website.com/images/image_19:11:21:15:25:24.png"
   }
   ]
}
 */
