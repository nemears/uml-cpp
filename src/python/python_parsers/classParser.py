import ast
from yuml_python import Class, Property, PrimitiveType, LiteralInt, LiteralBool, LiteralString, LiteralReal, Operation
from .defParser import parseFunction

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
                    d[defaultValue.getID()] = defaultValue
                    defaultValue.setValue(node.value.value)
                    attributeHelper(props, realType, defaultValue)
        elif type(node) is ast.FunctionDef:
            fun = Operation()
            d[fun.getID()] = fun
            fun.setName(node.name)
            bhv = parseFunction(node, d, clazz)
            for param in bhv.parameters:
                fun.addParameter(param)
            fun.addMethod(bhv)
            clazz.addOperation(fun)
            
    return clazz

def attributeHelper(props, primitive, defaultValue):
    for prop in props:
        prop.setType(primitive)
        prop.setDefaultValue(defaultValue)