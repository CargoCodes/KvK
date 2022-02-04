package com.company;

import java.io.*;
import java.lang.reflect.Array;
import java.net.Proxy;
import java.util.*;

public class KvK{
    Scanner fileReader;
    String filepath;
    File file;
    private int i;
    String[] currentLine;
    String lineString;

    KvK(String filePath){
        filepath = filePath;
        file = new File(filePath);
    }

    public ArrayList<Map<String, Map<String, String>>> read() throws Exception{
        fileReader = new Scanner(file);
        Boolean BOF = false;
        Boolean EOF = false;
        ArrayList<Map<String, Map<String, String>>> res = new ArrayList<Map<String, Map<String, String>>>(); // total
        String start = String.valueOf(((fileReader.nextLine()).split(" "))[0]);
        String SOF = "<#";
        if (start.equals(SOF)) {
            BOF = true;
            String line = fileReader.nextLine();
            lineString = line;
            currentLine = line.split(" ");
            while(/*fileReader.hasNextLine()*/EOF == false){
                if(lineString == "\n"){
                    line = fileReader.nextLine();
                    lineString = line;
                    currentLine = line.split(" ");
                }
                i = 0;
                while(i<currentLine.length){
                    while (currentLine[i].equals("\t\t") || currentLine[i].equals(" ") || currentLine[i].equals("\n") || currentLine[i].equals("\t") || currentLine[i].equals("")) {
                        i += 1;
                        System.out.println("skip in read");
                    }
                    if(currentLine[i].equals("class")){
                        System.out.println("class");
                        res.add(__getClass__());
                        System.out.println("class added");
                        break;
                    }
                    /**/
                    if(currentLine[0].equals("#>")){
                        EOF = true;
                        System.out.println("EOF found");
                        break;
                    }
                }
            }
        }
        else {
            throw new Exception("Expected \"<#\" at the beginning of the file");
        }
        if(BOF == true || EOF == true) {
            return res;
        } else {
            throw new Exception("Expected \"<#\" at the beginning of the file and \"#>\" at the end of the file");
        }
    }

    public void write(ArrayList<Map<String, Map<String, String>>> content) throws Exception {
        int j, k;
        String toWrite = "<#";
        for(j=0; j<content.toArray().length; j++){ // scorre le classi
            Map<String, Map<String, String>> classDict = content.get(j);
            if(classDict.keySet().toArray().length > 1){
                throw new Exception("Only one class name is allowed for each class");
            }
            String className = classDict.keySet().toArray()[0].toString();
            toWrite += "\n\tclass \"" + className + "\" ::>";
            Map<String, String> attrsMap = classDict.get((classDict.keySet()).toArray()[0]);
            String[] attrs = attrsMap.keySet().toArray(new String[0]);
            for(k=0; k< attrs.length; k++){
                String attrContent = attrsMap.get(attrs[k]);
                toWrite += "\n\t\t(" + attrs[k] + ") -> \"" + attrContent + "\"";
            }
        }
        toWrite += "\n#>";
        System.out.println(toWrite);
        BufferedWriter writer = new BufferedWriter(new FileWriter(filepath));
        writer.write(toWrite);
        writer.close();
    }

    public Map<String, String> getclass(String className){
        boolean found = false;
        KvK kvk = new KvK(filepath);
        Map<String, Map<String, String>> current = new HashMap<String, Map<String, String>>();
        try {
            ArrayList<Map<String, Map<String, String>>> content = kvk.read();
            int j = 0;
            for (j = 0; j < content.size(); j++) {
                current = content.get(j);
                if (current.containsKey(className)) {
                    found = true;
                    break;
                }
            }
            if (found) {
                Map<String, String> res = current.get(className);
                return res;
            } else {
                return null;
            }
        } catch (Exception ex){
            ex.printStackTrace();
        }
        return null;
    }

    public String getattr(String attribute, String className){
        KvK kvk = new KvK(filepath);
        boolean found = false;
        Map<String, Map<String, String>> current = new HashMap<String, Map<String, String>>();
        try{
            ArrayList<Map<String, Map<String, String>>> content = kvk.read();
            int j = 0;
            for (j = 0; j < content.size(); j++) {
                current = content.get(j);
                if (current.containsKey(className)) {
                    found = true;
                    break;
                }
            }
            if (found) {
                Map<String, String> tmp = current.get(className);
                String res = tmp.get(attribute);
                return res;
            } else {
                return null;
            }
        } catch (Exception ex){
            ex.printStackTrace();
        }
        return null;
    }

    public void addClass(String className){
        if(file.exists()){
            try {
                fileReader = new Scanner(file);
                String fileContent = fileReader.nextLine();
                while(fileReader.hasNextLine()){
                    fileContent += "\n" + fileReader.nextLine();
                }
                fileContent = fileContent.substring(0, fileContent.length()-2);
                fileContent += "    class \"" + className + "\" ::>\n#>";
                try {
                    BufferedWriter writer = new BufferedWriter(new FileWriter(filepath));
                    writer.write(fileContent);
                    writer.close();
                } catch (IOException ex){
                    ex.printStackTrace();
                }
            } catch (FileNotFoundException ex){
                ex.printStackTrace();
            }
        } else {
            String toWrite = "<#\n    class \"" + className + "\" ::>\n#>";
            try {
                BufferedWriter writer = new BufferedWriter(new FileWriter(filepath));
                writer.write(toWrite);
                writer.close();
            } catch (IOException ex){
                ex.printStackTrace();
            }
        }
    }

    public void addAttr(String className, String attrName, String attrContent) throws Exception{
        if(file.exists()){
            try {
                fileReader = new Scanner(file);
                String fileContent = fileReader.nextLine();
                while(fileReader.hasNextLine()){
                    fileContent += "\n" + fileReader.nextLine();
                }
                int classLength = ("class \"" + className + "\" ::>").length();
                int classIndex = fileContent.indexOf("class \"" + className + "\" ::>");
                if(classIndex != -1){
                    int index = classIndex+classLength;
                    int attrIndex = fileContent.indexOf("\n        (" + attrName + ") -> \"" + attrContent + "\"");
                    if(attrIndex != -1){
                        throw new Exception("Attribute already in class.");
                    }
                    String preAttr = fileContent.substring(0, index);
                    String afterAttr = fileContent.substring(index, fileContent.length());
                    String toWrite = preAttr + "\n        (" + attrName + ") -> \"" + attrContent + "\"" + afterAttr;
                    try {
                        BufferedWriter writer = new BufferedWriter(new FileWriter(filepath));
                        writer.write(toWrite);
                        writer.close();
                    } catch (IOException ex){
                        ex.printStackTrace();
                    }
                } else {
                    throw new Exception("Class not found in file");
                }

            } catch (FileNotFoundException ex){
                ex.printStackTrace();
            }
        } else {
            throw new Exception("You must create at least a class before adding atributes.");
        }
    }

    public void editClass(String className, String newClasName){
        fileReader = new Scanner(file);
        String fileContent = fileReader.nextLine();
        while(fileReader.hasNextLine()){
            fileContent += "\n" + fileReader.nextLine();
        }
        int startIndex = (fileContent.indexOf("class \"" + className + "\" ::>"));
        if(startIndex == -1){
            throw new Exception("Class not found")
        }
        startIndex += 7;
        int endIndex = startIndex+className.length();
        fileContent = fileContent[0:startIndex] + newClasName + fileContent[endIndex:fileContent.length()];
        try {
            BufferedWriter writer = new BufferedWriter(new FileWriter(filepath));
            writer.write(fileContent);
            writer.close();
        } catch (IOException ex){
            ex.printStackTrace();
        }
    }

    public void editAttr(String className, String attrName, String newAttrName){
        fileReader = new Scanner(file);
        String fileContent = fileReader.nextLine();
        while(fileReader.hasNextLine()){
            fileContent += "\n" + fileReader.nextLine();
        }
        int startIndex = (fileContent.indexOf("class \"" + className + "\" ::>"));
        if(startIndex == -1){
            throw new Exception("Class not found")
        }
        startIndex += 7;
        
    }

    private Map<String, Map<String, String>> __getClass__() throws Exception{
        Map<String, Map<String, String>> tmpRes = new HashMap<String, Map<String, String>>(); // classes
        String className = new String();
        i += 1;
        className = currentLine[i];
        className = className.substring(1, (className.length())-1);
        System.out.println("className: " + className);
        i += 1;
        if(!currentLine[i].equals("::>")){
            throw new Exception("Expected \"::>\" after class declaration");
        }
        String line = fileReader.nextLine();
        lineString = line;
        currentLine = line.split(" ");
        while (currentLine[i].equals("\t\t") || currentLine[i].equals(" ") || currentLine[i].equals("\n") || currentLine[i].equals("\t") || currentLine[i].equals("") || currentLine[i].equals("\t\t")) {
            i += 1;
            System.out.println("skip in class");
        }
        if(((currentLine[i]).substring(0, 1)).equals("(")){
            while (currentLine[i].equals("\t\t") || currentLine[i].equals(" ") || currentLine[i].equals("\n") || currentLine[i].equals("\t") || currentLine[i].equals("")) {
                i += 1;
            }
            System.out.println("attr");
            tmpRes.put(className, __getAttr__());
            System.out.println("attrs received");
        }
        return tmpRes;
    }

    private Map<String, String> __getAttr__() throws Exception {
        Map<String, String> stringMap = new HashMap<String, String>();
        while(((currentLine[i]).substring(0, 1)).equals("(")) {
            if (!(currentLine[i]).substring((currentLine[i].length()) - 1, (currentLine[i].length())).equals(")")) {
                throw new Exception("Expected \")\" after attribute name declaratino");
            }
            String attrName = (currentLine[i]).substring(1, (currentLine[i]).length() - 1);
            System.out.println("attrName: " + attrName);
            i += 1;
            if (currentLine[i].equals("->")) {
                i += 1;
                String attrContent = (currentLine[i]).substring(1, (currentLine[i]).length() - 1);
                stringMap.put(attrName, attrContent);
                System.out.println("attrContent: " + attrContent);
            } else {
                throw new Exception("Expected \"(\" before attribute name declaration");
            }
            i = 0;
            String line = fileReader.nextLine();
            lineString = line;
            currentLine = line.split(" ");
            while (currentLine[i].equals("\t\t") || currentLine[i].equals(" ") || currentLine[i].equals("\n") || currentLine[i].equals("\t") || currentLine[i].equals("")) {
                i += 1;
                System.out.println("a");
            }
            System.out.println("end of attr mainloop");
        }
        return stringMap;
    }
}
