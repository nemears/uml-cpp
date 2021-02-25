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
                c = parseClass2(node, d)
                moduleUML.addOwnedElement(c)
                d[c.getID()] = c
            if type(node) is ast.FunctionDef:
                bhv = parseFunction(node, d)
                moduleUML.addOwnedElement(bhv)
            #if type(n) is ast.Import:
        return moduleUML

def parseClass2(clazzNode, d):
    clazz = Class()
    d[clazz.getID()] = clazz
    for node in clazzNode.body:
        if type(node) is ast.FunctionDef:
            fun = Operation()
            d[fun.getID()] = fun
            bhv = parseFunction(node, d)
            fun.addMethod(bhv)
            clazz.addOperation(fun)
            
    return clazz

def parseFunction(defNode, d):
    fun = Activity()
    d[fun.getID()] = fun
    
    for node in defNode.body:
        if type(node) is ast.If:
            dec = DecisionNode()
        elif type(node) is ast.Return:
            retParam = Parameter()
            d[retParam.getID()] = retParam
            retParam.setDirection('RETURN')
            fun.addParameter(retParam)

    return fun

if __name__ == '__main__':
    n = parseModule('/home/stinky/Projects/yuml_projects/yuml/src/test/python/yuml_parsers/modelParser_test.py')
    print('length of module children: ', len(n.ownedElements))