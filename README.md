# KvK

Inter-language transmission meta-language. Useful also for internal software databases and internal data saving.

Example file

<#
\n\t    class "example" ::>
\n\t\t        (id) -> "1"
        (available) -> "true"
        (accessible) -> "true"
    class "anotherExamole" ::>
        (id) -> "2"
        (available) -> "true"
        (accessible) -> "false"
#>

Python

kvkFile = kvk.KvK('file.kvk')           At first create a new kvk object

kvkFile.read()                          Returns a dict, wich represents the content of the file

    output: [{"example":{"id":"1", "available":"true", "accessible":"true"}, {"anotherExample":{"id":"2", "available":"true", "accessbile":"false"}}]
