//
// Created by Cargo on 12/05/2022.
//

#include "KvK.h"

void KvK::startingContent() {
    if (subString(this->filePath, this->filePath.length() -4, this->filePath.length()) != ".kvk") // checks the extension of the file
        throw runtime_error("File extension must be \".kvk\"");
    else {
        file.open(this->filePath, ios::in); // tries to open file to read the content
        if(file.is_open()){
            string fileContent;
            while(getline(file, fileContent)){
                this->content += fileContent + "\n"; // reads the content and stores it in "content"
            }
            file.close();
        } else { // if it is impossible to read
            this->content = "<#\n#>"; // sets content to the empty standard
            file.open(this->filePath, ios::out);
            if(file.is_open()){
                file << this->content; // writes the standard empty content in the file
                file.close();
            }
        }
    }
}

map<string, map<string, string>> KvK::getClass_() {
    map<string, map<string, string>> tmpDict; // content of the class variable

    this->pos += 5;

    while(this->text[this->pos] == ' ') // skips blank spaces
        this->pos += 1;

    if(this->text[this->pos] == '"'){ // finds the opening double quotes for class name
        this->pos += 1;
        string className;

        while(this->text[this->pos] != '"'){ // scans until the following double quotes to read class name
            className += this->text[this->pos];
            this->pos += 1;
        }
        this->pos += 1;

        while(this->text[this->pos] == ' ') // skips blank spaces
            this->pos += 1;

        map<string, string> insideDict; // map for attributes
        if(this->text.substr(this->pos, 3) == "::>"){ // looks for the attribute list opening tag
            this->pos += 3;

            while(subString(this->text, this->pos, this->text.length()-1) != "#>"
            and this->text.substr(this->pos, 5) != "class"){ // scans until it finds EOF or new class
                this->nullAdvancement(); // skips blank spaces, blank lines and tabs

                if(this->text[this->pos] == '('){ // looks for attribute opening parentheses
                    vector<string> returnVector = this->getAttr_(); // starts attribute reading, one at the time
                    insideDict.insert(pair<string, string>(returnVector[0], returnVector[1])); // stores the attribute in the map
                    this->pos += 1;
                }

                tmpDict.insert(pair<string, map<string, string>>(className, insideDict)); // pushes the class in the return map
            }
            return tmpDict;
        } else
            throw runtime_error("\"::>\" must follow class name");
    } else
        throw runtime_error("Class name bust be wrapped with \"\"");
}

vector<string> KvK::getAttr_() {
    vector<string> res; // return variable
    string attrName;

    this->pos += 1;
    this->nullAdvancement(); // skips blank spaces, blank lines and tabs

    while(this->text[this->pos] != ')'){ // looks for the ending attribute name tag
        attrName += this->text[this->pos];
        this->pos += 1;
    }

    this->pos += 1;
    this->nullAdvancement(); // skips lbank lines, blank spaces and tabs

    if(this->text.substr(this->pos, 2) == "->"){ // looks for the appartenence tag
        this->pos += 2;

        while(this->text[this->pos] == ' ') // skips blank spaces
            this->pos += 1;

        if(this->text[this->pos] == '"'){ // looks for the attribute content opening double quote
            string attr;
            this->pos += 1;

            while(this->text[this->pos] != '"'){ // looks for the attribute content closing double quote
                attr += this->text[this->pos];
                this->pos += 1;
            }

            res.push_back(attrName);
            res.push_back(attr);

            return res; // returns attribute name and content
        } else
            throw runtime_error("Attribute must be wrapped with \"\"");
    } else
        throw runtime_error("\"->\" must between attribute name and attribute");
}

vector<map<string, map<string, string>>> KvK::read() {
    this->pos = 0;
    file.open(this->filePath, ios::in);
    if(file.is_open()) { // reads the content of the file
        string fileContent;
        while (getline(file, fileContent)) {
            this->text += fileContent + "\n";
        }

        file.close();

        vector<map<string, map<string, string>>> res; // return variable
        if (this->text.substr(this->pos, 2) == "<#"){ // looks for SOF
            this->pos += 2;
            while(this->pos < this->text.length()
            and subString(this->text, this->pos, this->text.length()-1) != "#>"){ // scans until the end of the file
                this->nullAdvancement(); // skips blank spaces, blank lines and tabs

                if(this->text.substr(this->pos, 5) == "class"){ // looks for the class tag
                    map<string, map<string, string>> point = getClass_(); // starts the class reding process
                    res.push_back(point); // stores the class
                }
                if(subString(this->text, this->pos, this->text.length()-1) == "#>") // if it find the EOF, stops scan
                    break;
            }
            return res;
        } else {
            throw runtime_error("File not starting with <#");
        }
    } else
        throw runtime_error("Can't open file \"" + this->filePath + "\"");
}

void KvK::write(const vector<map<string, map<string, string>>> &toWriteContent) {
    string toWrite = "<#\n"; // SOF

    for(auto& classcont:toWriteContent){ // for each class container in the vector
        for(auto& internalClass: classcont){ // for each pair in class container
            auto className = internalClass.first; // className is the first element of the pair
            toWrite += "    class \"" + className + "\" ::>\n";
            auto attributeList = internalClass.second; // map of attributes
            for(auto& attribute : attributeList) { // gets pair from map
                auto attributeName = attribute.first;
                auto attributeContent = attribute.second; // gets the attribute content, which is the second element of the pair
                toWrite += "        (" + attributeName + ") -> \"" + attributeContent + "\"\n";
            }
        }
        toWrite += "#>"; // EOF
    }
    file.open(this->filePath, ios::out);
    if(file.is_open()){ // writes the content in the file
        file << this->content;
        file.close();
    }
}

map<string, string> KvK::getClass(const string &className) {
    this->pos = 0;
    auto fileContent = this->read(); // reads the file

    try {
        for(const auto& classCont:fileContent){ // scans the class containers
            for(const auto& classname:classCont){ // scans teh classes
                if(classname.first == className){ // if the class name is equal to the wanted one
                    return classname.second; // return sthe class content
                }
            }
        }
    } catch (...){
        map<string, string> empty;
        return empty;
    }
    map<string, string> empty;
    return empty;
}

string KvK::getAttr(const string &className, const string &attrName) {
    this->pos = 0;
    auto fileContent = this->read();

    try {
        for(const auto& classCont:fileContent){ // scans the class containers
            for(const auto& classname:classCont){ // scans the classes
                if(classname.first == className){ // if the class name is equal to the wanted one
                    auto attrs = classname.second;
                    for(const auto& attr:attrs){ // scans the attributes
                        if(attr.first == attrName) // if the attribute name is equal to the wanted one
                            return attr.second; // returns the attribute content
                    }
                }
            }
        }
    } catch (...){
        return "";
    }
    return "";
}

bool KvK::addClass(const string &className) {
    int classIndex = this->content.find("    class \"" + className + "\" ::>");
    if(classIndex < 0){ // cheks if the class already exists
        string oldContent = subString(this->content, 0, this->content.length()-3); // takes teh content but removes the EOF
        string eof = "#>";

        this->content = oldContent + "    class \"" + className + "\" ::>\n" + eof; // adds the new class and then the EOF

        file.open(this->filePath, ios::out);
        if(file.is_open()){ // writes on the file the new content
            file << this->content;
            file.close();
            return true;
        }
        return false;
    }
    return false;
}

bool KvK::addAttr(const string &className, const string &attrName, const string &attrContent) {
    int index = this->content.find("class \"" + className + "\" ::>");
    if (index > 0) { // checks if the class exists
        int endIndex = index + ("class \"" + className + "\" ::>").length()+1; // takes teh index of the class

        int endClassIndex = endIndex+1;
        try { // scans until EOF or new class
            while (this->content.substr(endClassIndex, 2) != "#>"
                   and this->content.substr(endClassIndex,5) != "class")
                endClassIndex += 1;
        } catch(...) {
            endClassIndex = this->content.length()-2;
        }

        string isolatedClass = subString(this->content, index, endClassIndex); // isolates the wanted class
        int attrIndex = isolatedClass.find("        (" + attrName + ") -> \"" + attrContent + "\"");
        if(attrIndex < 0) { // checks if attribute already exists
            this->content = subString(this->content, 0, endIndex) + "        (" + attrName + ") -> \""
                    + attrContent + "\"\n"
                    + subString(this->content, endIndex, this->content.length()); // adds the attribute

            file.open(this->filePath, ios::out);
            if (file.is_open()) { // writes on the file the new content
                file << this->content;
                file.close();

                return true;
            }
            return false;
        }
    } else
        throw ClassNotFoundError("Class not found");
}

bool KvK::editClass(const string &className, const string &newClassName) {
    int startIndex = this->content.find("class \"" + className + "\" ::>");
    if(startIndex > 0) { // checks if the class exists
        int endIndex = startIndex + ("class \"" + className + "\" ::>").length(); // takes the ending index of teh class declaration

        this->content = this->content.substr(0, startIndex) + "class \"" + newClassName + "\" ::>"
                + subString(this->content, endIndex, this->content.length()); // edits the class name

        file.open(this->filePath, ios::out);
        if(file.is_open()){ // writes the new content on the file
            file << this->content;
            file.close();

            return true;
        }
        return false;
    } else
        throw ClassNotFoundError("Class \"" + className + "\" not found");
}

bool KvK::editAttr(const string &className, const string &attrName, const string &newAttrName, const string &attrContent) {
    int classIndex = this->content.find("class \"" + className + "\" ::>");
    if (classIndex > 0) { // checks if the class exists
        string preClass = this->content.substr(0, classIndex); // isolates what's before the class
        string tmp = subString(this->content, classIndex+5, this->content.length());

        int eofClassIndex = tmp.find("class");
        int endClassIndex = tmp.find("#>");

        int endIndex;
        if(eofClassIndex > 0)
            endIndex = eofClassIndex;
        else if(endClassIndex > 0)
            endIndex = endClassIndex;

        string isolatedClass = subString(this->content, classIndex, endClassIndex+classIndex+5); // isolates the class
        string afterClass = tmp.substr(endClassIndex, tmp.length()); // isolates what's after the class

        int oldAttrIndex = isolatedClass.find(attrName);
        if(oldAttrIndex > 0) { // checks if the attri bute exists
            isolatedClass = isolatedClass.substr(0, oldAttrIndex) + newAttrName + // updates the attribute name
                            subString(isolatedClass, oldAttrIndex+attrName.length(), isolatedClass.length());

            if(not attrContent.empty()){ // if attribute name is not empty
                int virgStart = oldAttrIndex;

                while(isolatedClass[virgStart] != '"')
                    virgStart += 1;

                int virgEnd = virgStart+1;
                while(isolatedClass[virgEnd] != '"')
                    virgEnd += 1;

                isolatedClass = isolatedClass.substr(0, virgStart+1) + attrContent // updates the attribute content
                                    + subString(isolatedClass, virgEnd, isolatedClass.length());
            }
            this->content = preClass + isolatedClass + afterClass;

            file.open(this->filePath, ios::out);
            if(file.is_open()){ // writes the new content in the file
                file << this->content;
                file.close();

                return true;
            }
            return false;

        } else
            throw AttributeNotFoundError("Attribute \"" + attrName + "\" of class \""
                                                                + className + "\" not found");
    } else
        throw ClassNotFoundError("Class \"" + className + "\" not found");
}

bool KvK::removeClass(const string &className) {
    int classIndex = this->content.find("    class \"" + className + "\" ::>");
    if (classIndex > 0) { // checks if the class exists
        string preClass = subString(this->content, 0, classIndex); // isolates what's before the class
        string tmp = subString(this->content, classIndex+5, this->content.length());

        int endClassIndex = tmp.find("class");
        int eofClassIndex = tmp.find("#>");

        int endIndex;
        string afterClass;

        if(endClassIndex > 0 and endClassIndex < eofClassIndex) // isolates what's after the class
            afterClass = subString(tmp, endClassIndex-4, tmp.length());
        else if(eofClassIndex > 0)
            afterClass = "#>";

        this->content = preClass + afterClass; // updates the content

        file.open(this->filePath, ios::out);
        if(file.is_open()){ // writes the new content in the file
            file << this->content;
            file.close();

            return true;
        }
        return false;
    } else
        throw ClassNotFoundError("Class \"" + className + "\" not found");
}

bool KvK::removeAttr(const string &className, const string &attrName) {
    int classIndex = this->content.find("class \"" + className + "\" ::>");
    if (classIndex > 0) { // checks if the class exists
        string preClass = subString(this->content, 0, classIndex); // isolates what's after the class
        string tmp = subString(this->content, classIndex+5, this->content.length());

        int eofClassIndex = tmp.find("#>");
        int endClassIndex = tmp.find("class");

        int endIndex;

        if(endClassIndex > 0 and endClassIndex < eofClassIndex)
            endIndex = endClassIndex;
        else
            endIndex = eofClassIndex;

        string isolatedClass = subString(this->content, classIndex, endClassIndex+classIndex+5); // isolates the class
        string afterClass = subString(tmp, endClassIndex, tmp.length()); // isolates what's after the class

        int oldAttrIndex = isolatedClass.find("(" + attrName + ")");

        if(oldAttrIndex >= 0) { // checks if the attribute exists
            int virgStart = oldAttrIndex;
            while(isolatedClass[virgStart] != '"')
                virgStart += 1;

            int virgEnd = virgStart+1;
            while(isolatedClass[virgEnd] != '"')
                virgEnd += 1;

            isolatedClass = subString(isolatedClass, 0, oldAttrIndex) // updates the class
                          + subString(isolatedClass, virgEnd+1, isolatedClass.length());

            this->content = preClass + isolatedClass + afterClass;

            file.open(this->filePath, ios::out);
            if(file.is_open()){ // writes the new content in the file
                file.clear();
                file << this->content;
                file.close();

                return true;
            }
            return false;

        } else
            throw AttributeNotFoundError("Attribute \"" + attrName + "\" of class \""
                                         + className + "\" not found");
    } else
        throw ClassNotFoundError("Class \"" + className + "\" not found");
}

bool KvK::isEmpty() {
    string actualContent = this->content;
    auto contentList = split(actualContent, ' '); // splits the content, removing blank spaces

    int i = 0;
    for(const auto& element:contentList){ // removes all the blank speces
        if(element.empty() or element == " "){
            contentList = pop(contentList, i);
        }
        i += 1;
    }
    try{ // checks if the file is litterally empty, or contains the empty standard
        if(contentList[0] == "<#" and contentList[1] == "#>")
            return true;
        return false;
    } catch (...) {
        if(contentList.empty())
            return true;
        return false;
    }
}