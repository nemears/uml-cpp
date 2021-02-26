import ast
from yuml_python import *

# This is needs a dict to keep c++ memory not garbage collected for now
# There may be a solution on the pybind11 side with these issues if they implement fixes
# https://github.com/pybind/pybind11/issues/1333
# https://github.com/pybind/pybind11/pull/2839
def parseModule(path, d):
    with open(path) as f:
        moduleNode = ast.parse(f.read())
        moduleUML = Namespace()
        d[moduleUML.getID()] = moduleUML
        for node in moduleNode.body:
            if type(node) is ast.ClassDef:
                c = parseClass(node, d)
                moduleUML.addOwnedElement(c)
                d[c.getID()] = c
            if type(node) is ast.FunctionDef:
                bhv = parseFunction(node, d)
                moduleUML.addOwnedElement(bhv)
            #if type(n) is ast.Import:
        return moduleUML

def parseClass(clazzNode, d):
    clazz = Class()
    d[clazz.getID()] = clazz
    clazz.setName(clazzNode.name)
    for node in clazzNode.body:

        # property with default value
        if type(node) is ast.Assign:
            prop = Property()
            d[prop.getID()] = prop
            for target in node.targets:
                if type(target) is ast.Name:
                    prop.setName(target.id)
            clazz.addAttribute(prop) 
        elif type(node) is ast.FunctionDef:
            fun = Operation()
            d[fun.getID()] = fun
            fun.setName(node.name)
            bhv = parseFunction(node, d)
            for param in bhv.parameters:
                fun.addParameter(param)
            fun.addMethod(bhv)
            clazz.addOperation(fun)
            
    return clazz

def parseFunction(defNode, d):
    fun = Activity()
    d[fun.getID()] = fun
    fun.setName(defNode.name)

    # Go through input parameters
    for arg in defNode.args.args:
        p = Parameter()
        p.setName(arg.arg)
        d[p.getID()] = p
        p.setDirection("IN")
        fun.addParameter(p)
    
    for node in defNode.body:
        if type(node) is ast.If:
            dec = DecisionNode()

        # find return param
        elif type(node) is ast.Return:
            retParam = Parameter()
            d[retParam.getID()] = retParam
            retParam.setDirection('RETURN')
            fun.addParameter(retParam)

    return fun

if __name__ == '__main__':
    n = parseModule('/home/stinky/Projects/yuml_projects/yuml/src/test/python/yuml_parsers/modelParser_test.py')
    print('length of module children: ', len(n.ownedElements))