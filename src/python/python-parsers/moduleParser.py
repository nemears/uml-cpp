import ast
from yuml_python import *

def parseModule(path):
    with open(path) as f:
        moduleNode = ast.parse(f.read())
        moduleUML = Namespace()
        for node in moduleNode.body:
            if type(node) is ast.ClassDef:
                classUML = Class()
                moduleUML.addOwnedElement(parseClass(node))
            #if type(n) is ast.Import:
                # TODO standard libraries and other module recursion
        return moduleUML

def parseClass(clazzNode):
    # TODO parse
    c = Class()
    return c

if __name__ == '__main__':
    n = parseModule('/home/stinky/Projects/yuml_projects/yuml/src/test/python/yuml-parsers/modelParser_test.py')
    print('length of module children: ', len(n.ownedElements))