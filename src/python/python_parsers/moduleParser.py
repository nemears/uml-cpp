import ast
from yuml_python import *

def parseModule(path):
    with open(path) as f:
        moduleNode = ast.parse(f.read())
        moduleUML = Namespace()
        for node in moduleNode.body:
            if type(node) is ast.ClassDef:
                c = Class()#parseClass(node)
                moduleUML.addOwnedElement(c)
            #if type(n) is ast.Import:
               
        moduleUML.addOwnedElement(Class())
        return moduleUML

# This is 2 because we need a dict to keep c++ memory not garbage collected for now
# There may be a solution on the pybind11 side with these issues if they implement fixes
# https://github.com/pybind/pybind11/issues/1333
# https://github.com/pybind/pybind11/pull/2839
def parseModule2(path, d):
    with open(path) as f:
        moduleNode = ast.parse(f.read())
        moduleUML = Namespace()
        d[moduleUML.getID()] = moduleUML
        for node in moduleNode.body:
            if type(node) is ast.ClassDef:
                c = Class()#parseClass(node)
                moduleUML.addOwnedElement(c)
                d[c.getID()] = c
            #if type(n) is ast.Import:
        return moduleUML

def parseClass2(clazzNode, d):
    clazz = Class()
    d[clazz.getID()] = clazz
    for node in clazzNode.body:
        if type(node) is ast.FunctionDef:
            fun = Operation()
    return clazz

if __name__ == '__main__':
    n = parseModule('/home/stinky/Projects/yuml_projects/yuml/src/test/python/yuml_parsers/modelParser_test.py')
    print('length of module children: ', len(n.ownedElements))