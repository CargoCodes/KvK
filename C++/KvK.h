//
// Created by Cargo on 12/05/2022.
//

#ifndef KVK_KVK_H
#define KVK_KVK_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

class KvK {
public:
    explicit KvK(const string& filePath) : filePath(filePath) {
        this->startingContent();
    }

    KvK(const KvK& other) {
        this->filePath = other.filePath;
        this->content = other.content;
    }

    KvK& operator=(const KvK& other) {
        this->filePath = other.filePath;
        this->content = other.content;
        return *this;
    }

    vector<map<string, map<string, string>>> read();

    void write(const vector<map<string, map<string, string>>>& content);

    map<string, string> getClass(const string &className);

    string getAttr(const string &className, const string &attrName);

    bool addClass(const string& className);

    bool addAttr(const string& className, const string& attrName, const string&attrContent);

    bool editAttr(const string& className, const string& attrName, const string& newAttrName, const string& attrContent="");

    bool editClass(const string& className, const string& newClassName);

    bool removeClass(const string& className);

    bool removeAttr(const string& className, const string& attrName);

    bool isEmpty();

private:
    string filePath;
    int pos = 0;

    string content;
    fstream file;
    string text;

    void startingContent();

    map<string, map<string, string>> getClass_();

    vector<string> getAttr_();

    void nullAdvancement() {
        while(this->text[this->pos] == ' ' or this->text[this->pos] == '\t' or this->text[this->pos] == '\n')
            this->pos += 1;
    }

    static vector<string> split(string str, char separator){
        vector<string> list;

        while(str.find(separator)){
            list.push_back(str.substr(0, str.find(separator)));
            str = str.substr(str.find(separator)+1, str.length());
        }

        return list;
    }

    static vector<string> pop(const vector<string> &list, int index) {
        vector<string> returnList;
        for(int i = 0; i < list.size(); i++){
            if(i != index){
                returnList.push_back(list[i]);
            }
        }
        return returnList;
    }

    class ClassNotFoundError : runtime_error {
    public:
        explicit ClassNotFoundError(const string& message) : runtime_error(message.c_str()) {}
    };

    class AttributeNotFoundError : runtime_error {
    public:
        explicit AttributeNotFoundError(const string& message) : runtime_error(message) {}
    };

    static string subString(const string& string, int startIndex, int endIndex) {
        if(startIndex >= 0 and startIndex <= string.length() and endIndex >= 0 and endIndex <= string.length()){
            std::string returnstring;

            for(int i = startIndex; i < endIndex; i++){
                returnstring += string[i];
            }
            return returnstring;
        } else
            throw std::out_of_range("Index out of range");
    }
};


#endif //KVK_KVK_H
