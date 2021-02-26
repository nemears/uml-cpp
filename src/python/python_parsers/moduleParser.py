import ast
from yuml_python import *

# This needs a dict to keep c++ memory not garbage collected for now
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
            props = []
            for target in node.targets:
                if type(target) is ast.Name:
                    prop = Property()
                    d[prop.getID()] = prop
                    props.append(prop)
                    prop.setName(target.id)
            clazz.addAttribute(prop) 
            if type(node.value) is ast.Constant:
                # This is the default value of all of the attributes
                # TODO determine type
                if type(node.value.value) is bool:
                    boolType = PrimitiveType()
                    d[boolType.getID()] = boolType
                    boolType.setPrimitiveType('BOOL')
                    defaultValue = LiteralBool()
                    d[defaultValue.getID()] = defaultValue
                    defaultValue.setValue(node.value.value)
                    attributeHelper(props, boolType, defaultValue)
                elif type(node.value.value) is int:
                    intType = PrimitiveType()
                    d[intType.getID()] = intType
                    intType.setPrimitiveType('INT')
                    defaultValue = LiteralInt()
                    d[defaultValue.getID()] = defaultValue
                    defaultValue.setValue(node.value.value)
                    attributeHelper(props, intType, defaultValue)
                elif type(node.value.value) is str:
                    strType = PrimitiveType()
                    d[strType.getID()] = strType
                    strType.setPrimitiveType('STRING')
                    defaultValue = LiteralString()
                    d[defaultValue.getID()] = defaultValue
                    defaultValue.setValue(node.value.value)
                    attributeHelper(props, strType, defaultValue)
                elif type(node.value.value) is float:
                    realType = PrimitiveType()
                    d[realType.getID()] = realType
                    realType.setPrimitiveType('REAL')
                    defaultValue = LiteralReal()
                    defaultValue.setValue(node.value.value)
                    attributeHelper(props, realType, defaultValue)
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

def attributeHelper(props, primitive, defaultValue):
    for prop in props:
        prop.setType(primitive)
        prop.setDefaultValue(defaultValue)

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
            d[dec.getID()] = dec

        # find return param
        elif type(node) is ast.Return:
            retParam = Parameter()
            d[retParam.getID()] = retParam
            retParam.setDirection('RETURN')
            fun.addParameter(retParam)

    return fun

if __name__ == '__main__':
    d = {}
    n = parseModule('/home/stinky/Projects/yuml_projects/yuml/src/test/python/yuml_parsers/modelParser_test.py', d)
    print('length of module children: ', len(n.ownedElements))
    m = Model()
    m.setName('root')
    m.addOwnedElement(n)
    emitter = ModelParser()
    print(emitter.emit(m))