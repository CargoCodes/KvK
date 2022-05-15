# KvK

Inter-language transmission meta-language. Useful also for internal software databases and internal data saving.

Interpreter available for:
- [Python](https://github.com/CargoCodes/KvK#python-methods)
- [C++](https://github.com/CargoCodes/KvK#c-methods)
- [Java](https://github.com/CargoCodes/KvK#java-methods)

Example file

    <#
        class "example" ::>
            (id) -> "1"
            (available) -> "true"
        class "anotherExample" ::>
            (id) -> "2"
            (accessible) -> "false"
    #>


# Python Methods

At first create a new kvk object

    kvkFile = kvk.KvK(filePath='file.kvk')           

Read kvk file and return the content as an array containing dicts

    kvkFile.read()                          

    # returns [{"example":{"id":"1", "available":"true"}, {"anotherExample":{"id":"2", "accessbile":"false"}}]

Clear file and rewrite it

    kvkFile.write(content=[{"newExample":{"id":"3", "available":"true"}}, {"anotherNewExample":{"id":"4", "available":"false"}}])
    
Get an attribute or the content of a class

    kvkFile.get(element="newExample")
    
    # returns {"id":"3", "available":"true"}
    
    kvkFile.get(element="id", className="anotherNewExample")
    
    # returns "4"
    
Add an empty class

    kvkFile.addClass(className="newClass")
    
Add an attribute to a class

    kvkFile.addAttr(className="newClass", attrName="id", attrContent="5")
    
Edit a class

    kvkFile.editClass(oldClassName="newClass", newClassName="lastClass")
    
Edit an attribute

    kvkFile.editAttr(className="lastClass", oldAttrName="id", newAttrName="position") 
    # without editing the content of an attribute
    
    kvkFile.editAttr(className="lastClass", oldAttrName="position", newAttrName="id", attrContent="6") 
    # with editing the content of an attribute
    
Remove a class

    kvkFile.removeClass(className="lastClass")
    
Remove an attribute

    kvkFile.removeAttr(className="anotherNewExample", attrName="available")
    

# C++ Methods
    
At first create a new kvk object
    
    KvK kvk("file.kvk");
    
Read the kvk file and returns an array list containing maps

    vector<map<std::string, map<std::string, std::string>>> content = kvk.read();

Clear the file and rewrite it

    kvk.write(newContent); // newContent must be vector<map<std::string, map<std::string, std::string>>> type
    
Get the content of a class

    map<std::string, std::String> classContent = kvk.getClass("example");
    
Get an attribute from a class

    std::string attribute = kvk.getattr("example", "id"); // in getAttr you must pass as parameters the class name, and then the attribute name
    
    // returns 1
    
Add a class

    kvk.addClass("newClass");
    
Add an attribute

    kvk.addAttr("newClass", "id", "3"); 
    // you must eneter first the class name, then the attribute name, and then the attrbute content
    
Edit a class

    kvk.editClass("newClass", "lastAddedClass"); 
    // you must enter first the class name, and then the new class name
    
Edit an attribute

    kvk.editAttr("lastAddedClass", "id", "position");
    // you must enter first the class name, then the attribute name, and then the noew attribute name
    
Edit the content of an attribute

    kvk.editAttrContent("lastAddedClass", "position", "0");
    // you must enter first the class name, then the attribute name, and then the new attribute content
    
Remove a class

    kvk.removeClass("lastAddedClass");
    
Remove an attribute

    kvk.removeAttr("example", "id");
    // you must enter first the class name, and then the attribute name

    
# Java Methods
At first create a new kvk object
    
    KvK kvk = new KvK("file.kvk");
    
Read the kvk file and returns an array list containing maps

    ArrayList<Map<String, Map<String, String>>> content = kvk.read();
    
    // returns [{example={available=true, id=1}}, {anotherExample={accessible=false, id=2}}]
    
Clear the file and rewrite it

    kvk.write(newContent); // newContent must be ArrayList<Map<String, Map<String, String>>> type
    
Get the content of a class

    Map<String, String> classContent = kvk.getclass("example");
    
    // returns {available=true, id=1}
    
Get an attribute from a class

    String attribute = kvk.getattr("example", "id");    // in getattr you must pass as parameters the class name, and then the attribute name
    
    // returns 1
    
Add a class

    kvk.addClass("newClass");
    
Add an attribute

    kvk.addAttr("newClass", "id", "3"); 
    // you must eneter first the class name, then the attribute name, and then the attrbute content
    
Edit a class

    kvk.editClass("newClass", "lastAddedClass"); 
    // you must enter first the class name, and then the new class name
    
Edit an attribute

    kvk.editAttr("lastAddedClass", "id", "position");
    // you must enter first the class name, then the attribute name, and then the noew attribute name
    
Edit the content of an attribute

    kvk.editAttrContent("lastAddedClass", "position", "0");
    // you must enter first the class name, then the attribute name, and then the new attribute content
    
Delete a class

    kvk.deleteClass("lastAddedClass");
    
Delete an attribute

    kvk.deleteAttribute("example", "id");
    // you must enter first the class name, and then the attribute name
