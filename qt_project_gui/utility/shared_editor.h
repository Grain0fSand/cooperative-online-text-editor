#ifndef SHARED_EDITOR_H
#define SHARED_EDITOR_H

#include <QObject>
#include <memory>

class Shared_editor
{
public:
    Shared_editor(const Shared_editor&) = delete;
    Shared_editor& operator=(const Shared_editor&) = delete;

    static Shared_editor& getInstance(){
        static Shared_editor instance;

        return instance;
    }

    void addCharapter(int position,QChar c){
        position = position+initial_index;
        this->str.insert(position,c);
    }

    void addString(int position,QString str){
        position = position+initial_index;
        this->str.insert(position,str);
    }

    void removeCharapter(int position){
        position = position+initial_index;
        str.remove(position,1);
    }


    void removeString(int position,int lenght){
        position = position+initial_index;
        str.remove(position,1-lenght);
    }

    void initString(QString string){
        str = string;
        initial_index = str.indexOf("<br />");

        if (initial_index < 0)
            throw "the string isn't parsable by the qt editor";
    }

    QString toString(){
        return str;
    }

private:
    Shared_editor() = default;
    ~Shared_editor() = default;
    QString str;
    int initial_index;

};

#endif // SHARED_EDITOR_H
