# KvK

Inter-language transmission meta-language. Useful also for internal software databases and internal data saving.

Example file

    <#
        class "example" ::>
            (id) -> "1"
            (available) -> "true"
        class "anotherExamole" ::>
            (id) -> "2"
            (accessible) -> "false"
    #>


Python Methods

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
