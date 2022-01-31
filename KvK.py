class Kvk:
    def __init__(self, filePath):
        self.filePath = filePath
        self.file = open(self.filePath, 'r')
        self.pos = 0
        self.content = '<#\n#>'

    def __getClass__(self):
        tmpDict = {}
        self.pos += 5
        while self.text[self.pos] == ' ':
            self.pos += 1
        if self.text[self.pos] == '"':
            self.pos += 1
            className = ''
            while self.text[self.pos] != '"':
                className += self.text[self.pos]
                self.pos += 1
            self.pos += 2
            self.insideDict = {}
            if self.text[self.pos:self.pos+3] == '::>':
                self.pos += 3
                while self.text[self.pos:self.pos+3] != '#>' and self.text[self.pos:self.pos+5] != 'class':
                    if self.text[self.pos] == ' ' or self.text[self.pos] == '\t' or self.text[self.pos] == '\n':
                        self.pos += 1
                    elif self.text[self.pos] == '(':
                        self.__attr__()
                    tmpDict[className] = self.insideDict
                return tmpDict
            else:
                raise SyntaxError('Expected "::>" after class name.')
        else:
            raise SyntaxError('Expected \'\"\' after class declaration.')

    def __attr__(self):
        self.pos+=1
        attrName = ''
        while self.text[self.pos] == ' ' or self.text[self.pos] == '\t' or self.text[self.pos] == '\n':
            self.pos += 1
        while self.text[self.pos] != ')':
            attrName += self.text[self.pos]
            self.pos += 1
        self.pos += 1
        while self.text[self.pos] == ' ' or self.text[self.pos] == '\t' or self.text[self.pos] == '\n':
            self.pos += 1
        if self.text[self.pos:self.pos + 2] == '->':
            self.pos += 2
            while self.text[self.pos] == ' ':
                self.pos += 1
            if self.text[self.pos] == '"':
                attr = ''
                self.pos += 1
                while self.text[self.pos] != '"':
                    attr += self.text[self.pos]
                    self.pos += 1
                self.insideDict[attrName] = attr
                self.pos += 1
            else:
                raise SyntaxError('Expected '"' after ->")
        else:
            raise SyntaxError('Expected -> after attribute name')

    def __replaceAtIndex__(self, text, index1, index2, newContent):
        return text[0:index1] + newContent + text[index2:len(text)]

    def __insertAtIndex__(self, text, index1, toInsert):
        return text[0:index1+1] + toInsert + text[index1+2:len(text)]

    def read(self):
        self.text = self.file.read()
        res = []
        if self.text[self.pos:self.pos+2] == '<#':
            self.pos += 2
            while self.pos < len(self.text):
                if self.text[self.pos] == ' ' or self.text[self.pos] == '\t' or self.text[self.pos] == '\n':
                    self.pos += 1
                if self.text[self.pos:self.pos+5] == 'class':
                    point = self.__getClass__()
                    res.append(point)
                if self.text[self.pos:len(self.text)] == '#>':
                    break
            return res

    def setContent(self, content):
        self.content = content

    def get(self, element, className=None):
        self.pos = 0
        dict = self.read() if self.content == '<#\n#>' else self.content
        found = False
        if className == None:
            for classCont in dict:
                for classname in classCont:
                    if classname == element:
                        found = True
                        return classCont[classname]
        else:
            for classCont in dict:
                for classname in classCont:
                    if classname == className:
                        for attr in classCont[classname]:
                            if attr == element:
                                return classCont[classname][attr]
                            else:
                                return found

    def write(self, content):
        toWrite = ''
        toWrite += '<#\n'
        for classCont in content:
            for className in classCont:
                toWrite += f'\tclass "{className}" ::>\n'
                for attr in classCont[className]:
                    tmp = classCont[className]
                    cont = ''
                    for content in classCont[className][attr]:
                        cont += content
                    toWrite += f'\t\t({attr}) -> "{cont}"\n'
                #toWrite += '\n'
        toWrite += '#>'
        self.file.write(toWrite)

    def addClass(self, className):
        newContent = self.__replaceAtIndex__(str(self.content), len(self.content)-3, len(self.content)-2, f'\n\tclass "{className}" ::>\n')
        self.content = newContent
        with open(self.filePath, 'w') as file:
            file.write(newContent)

    def addAttr(self, className, attrName, attrContent):
        try:
            index = self.content.index(f'class "{className}" ::>')
        except:
            raise Exception('Class not found')
        else:
            endIndex = (index+(len(f'class "{className}" ::>')))-1
            newContent = self.__insertAtIndex__(self.content, endIndex, f'\n\t\t({attrName}) -> "{attrContent}"\n')
            self.content = newContent
            with open(self.filePath, 'w') as file:
                file.write(newContent)

    def __repr__(self):
        return f'< KvK handle class >'

    def __str__(self):
        return '< KvK handle class >'
