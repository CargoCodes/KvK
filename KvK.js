class KvK{
	constructor(filePath){
		var this.filePath = filePath
		var this.content = '<#\n#>'
		var this.fs = require('fs')
		__getContent__()
	}

	__getContent__(){
		try{
			this.fs.readFile(filePath, 'r', (err, data) => {
				if(err) throw err
					this.content = data.toSteing()
			})
		} catch(){
			this.content = '<#\n#>'
		}
	}

	get(element, className=None){
		var this.pos = 0
		var dict = read() if this.content == '<#\n#>' else this.content
		var found = false
		if className == None{
			for(let classCont in dict){
				for(let classname in classCont){
					if(classname == element){
						found = true
						return classCont[classname]
					}
				}
			}
		} else {
			for(let classCont in dict){
				for(let classname in classCont){
					if(classname == className){
						for(let attr in element){
							if(attr == element){
								return classCont[classname][attr]
							} else {
								return found
							}
						}
					}
				}
			}
		}
	}

	write(content){
		var toWrite = '<#\n'
		for(var classCont in content){
			for(var className in classCont){
				toWrite += '    class \"' + className + '\" ::>\n'
				for(var attr in classCont[className]){
					var tmp = classCont[className]
					var cont = ''
					for(var attrContent in classCont[className][attr]){
						cont += attrContent
					}
					toWrite += '        (' + attr + ') -> \"' + cont + '\"\n'

				}
			}
		}
		toWrite += '\n#>'
		this.fs.writeFile(this.filePath, toWrite, (err) => {
			if (err) throw err
		})
	}

	addClass(className){
		var newContent = this.content.substring(0, len(this.content)-2)
		newContent += 'class \"' + className + '\" ::>\n#>'
		this.content = newContent
		this.fs.writeFile(filePath, newContent, (err) =>{
			if(err) throw err
		})
	}

	addAttr(className, attrName, attrContent){
		var classDef = 'class \"' + className + '\" ::>'
		var index = this.content.indexOf(classDef)
		if(index != -1){
			var endIndex = (index+(length(classDef)))
			var newContent = this.content.substring(0, endIndex) + '\n        (' + attrName + ') -> \"' + attrContent + '\"' + this.content.substring(endIndex, length(this.content))
			this.content = newContent
			this.fs.writeFile(filePath, newContent, (err) =>{
				if(err) throw err
			})
		} else {
			throw 'Error: Class not found'
		}
	}

	editClass(oldClassName, newClassName){
		var classDef = 'class \"' + oldClassName + '\" ::>'
		var index = this.content.indexOf(classDef)
		if(index != -1){
			var startIndex = index+7
			var endIndex = startIndex+length(oldClassName)
			this.content = this.content.substring(0, startIndex) + newClassName + this.content.substring(endIndex, length(this.content))
			this.fs.writeFile(filePath, this.content, (err) =>{
				if(err) throw err
			})
		} else {
			throw 'Error: Class not found'
		}
	}

	removeClass(className){
		var classDef = 'class \"' + className + '\" ::>'
		var classIndex = this.content.indexOf(classDef)
		if(classIndex == -1){
			
		}
	}
}
