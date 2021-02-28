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
                c.setOwner(moduleUML)
                d[c.getID()] = c
            elif type(node) is ast.FunctionDef:
                bhv = parseFunction(node, d, moduleUML)
                moduleUML.addOwnedElement(bhv)
                bhv.setOwner(moduleUML)
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

def parseBody(bodyNode, d, uml, owner, lastNode):
    initNode = lastNode
    init = True
    # go through body
    for node in bodyNode:
        if type(node) is ast.Expr:
            if type(node.value) is ast.Call:
                if type(node.value.func) is ast.Name:
                    # find the behavior it is referencing
                    # so we need to go through:
                    # 1) all owners functions
                    # 2) all module owned functions
                    # 3) all python generic functions
                    # 4) all imported functions

                    # owner's functions
                    for el in owner.ownedElements:
                        # filter for behaviors
                        if issubclass(el.__class__, Behavior):
                            if el.getName() == node.value.func.id:
                                behaviorNode = CallBehaviorAction()
                                d[behaviorNode.getID()] = behaviorNode
                                behaviorNode.setBehavior(el)
                                controlFlow = ControlFlow()
                                d[controlFlow.getID()] = controlFlow
                                controlFlow.setSource(lastNode)
                                controlFlow.setTarget(behaviorNode)
                                lastNode.addOutgoing(controlFlow)
                                behaviorNode.addIncoming(controlFlow)
                                lastNode = behaviorNode
                                break
                                
        elif type(node) is ast.If:
            dec = DecisionNode()
            d[dec.getID()] = dec

            # map control flow to decision
            flow = ControlFlow()
            d[flow.getID()] = flow
            uml.addEdge(flow)
            flow.setSource(lastNode)
            flow.setTarget(dec)
            dec.addIncoming(flow)
            lastNode.addOutgoing(flow)
            uml.addNode(dec)

            # get object input to decision node
            # if it is a name node it is referencing a previously defined variable, so search through all
            # of the objectNode type nodes defined so far for a match and set flow
            if type(node.test) is ast.Name:
                for funNode in uml.nodes:
                    if issubclass(funNode.__class__, ObjectNode):
                        inputFlow = ObjectFlow()
                        d[inputFlow.getID()] = inputFlow
                        uml.addEdge(inputFlow)
                        inputFlow.setSource(funNode)
                        inputFlow.setTarget(dec)
                        funNode.addOutgoing(inputFlow)
                        dec.addIncoming(inputFlow)
                        dec.setDecisionInputFlow(inputFlow)
            # other possibilites are an expression
            elif type(node.test) is ast.Expr:
                # TODO
                print('TODO: expressions in if statements')
            # other possibilites are functions (ast.Call) and maybe more

            # parse body of decision
            firstAndLastNodes1 = parseBody(node.body, d, uml, owner, dec)
            firstAndLastNodes2 = parseBody(node.orelse, d, uml, owner, dec)

            #map outgoing control flow
            outFlow1 = ControlFlow()
            outFlow2 = ControlFlow()
            d[outFlow1.getID()] = outFlow1
            d[outFlow2.getID()] = outFlow2
            outFlow1.setSource(dec)
            outFlow2.setSource(dec)
            dec.addOutgoing(outFlow1)
            dec.addOutgoing(outFlow1)
            outFlow1.setTarget(firstAndLastNodes1[0])
            outFlow2.setTarget(firstAndLastNodes2[0])

            # map join node
            join = JoinNode()
            d[join.getID()] = join
            joinFlow1 = ControlFlow()
            joinFlow2 = ControlFlow()
            d[joinFlow1.getID()] = joinFlow1
            d[joinFlow2.getID()] = joinFlow2
            join.addIncoming(joinFlow1)
            join.addIncoming(joinFlow2)
            joinFlow1.setSource(firstAndLastNodes1[1])
            joinFlow2.setSource(firstAndLastNodes2[1])
            joinFlow1.setTarget(join)
            joinFlow2.setTarget(join)

            # override lastNode
            lastNode = join

        # find return param
        elif type(node) is ast.Return:
            retParam = Parameter()
            d[retParam.getID()] = retParam
            retParam.setDirection('RETURN')
            uml.addParameter(retParam)

        if init:
            initNode = lastNode
            init = False

    return (initNode, lastNode)

def parseFunction(defNode, d, owner):
    fun = Activity()
    d[fun.getID()] = fun
    fun.setName(defNode.name)
    initialNode = InitialNode()
    d[initialNode.getID()] = initialNode
    fun.addNode(initialNode)

    # Go through input parameters
    for arg in defNode.args.args:
        p = Parameter()
        p.setName(arg.arg)
        d[p.getID()] = p
        p.setDirection("IN")
        fun.addParameter(p) 
        pNode = ParameterNode()
        d[pNode.getID()] = pNode
        pNode.setParameter(p)
        fun.addNode(pNode)

    parseBody(defNode.body, d, fun, owner, initialNode)

    return fun

if __name__ == '__main__':
    d = {}
    n = parseModule('/home/stinky/Projects/yuml_projects/yuml/src/test/python/examples/knapsack.py', d)
    print('length of module children: ', len(n.ownedElements))
    m = Model()
    m.setName('root')
    m.addOwnedElement(n)
    emitter = ModelParser()
    print(emitter.emit(m))