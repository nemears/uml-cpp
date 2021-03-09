import ast
from yuml_python import *

def parseFunction(node, owner, d):

    # create activity
    fun = Activity()
    d[fun.getID()] = fun
    fun.setOwner(owner)
    fun.setName(defNode.name)

    # create initial node
    initialNode = InitialNode()
    d[initialNode.getID()] = initialNode
    fun.addNode(initialNode)

    # Go through input parameters
    for arg in node.args.args:
        p = Parameter()
        p.setName(arg.arg)
        d[p.getID()] = p
        p.setDirection("IN")
        fun.addParameter(p) 
        pNode = ParameterNode()
        pNode.setName(arg.arg)
        d[pNode.getID()] = pNode
        pNode.setParameter(p)
        fun.addNode(pNode)

    lnode = iterateThroughBody(node.body, fun, initialNode, d)
    
    # final node
    final = FinalNode()
    d[final.getID()] = final
    fun.addNode(final)

    # Control flow to final
    flow = ControlFlow()
    d[flow.getID()] = flow
    setSourceAndTarget(flow, lNode, final)
    fun.addEdge(flow)

    # return function
    return fun

def iterateThroughBody(nodeList, activity, initialNode, d):
    # this variable will store the current node parsed in the graph
    cNode = initialNode
    # this variable stores the last node parsed in the graph
    lNode = initialNode
    # this variable stores the temporary node for the next lastNode
    tNode = initialNode
    
    # go through body to map control flow
    for bodyNode in nodeList:

        # parse body
        if type(bodyNode) is ast.Assign:
            tNode = cNode = parseAssign(bodyNode, fun, d)
        #elif type(bodyNode) is ast.Expr:
            # TODO parseExpression
        elif type(node) is ast.If:
            cNode, tNode = parseIF(bodyNode, fun, d)
        elif type(node) is ast.Return:
            cNode = tNode = parseReturn(bodyNode, fun, d)
        else:
            raise RuntimeError(bodyNode)

        # Control flow
        if cNode != None:
            flow = ControlFlow()
            d[flow.getID()] = flow
            setSourceAndTarget(flow, lNode, cNode)
            activity.addEdge(flow)
            lNode = tNode
    
    # return the last node in the body
    return lNode

def setSourceAndTarget(flow, source, target):
    flow.setSource(source)
    source.addOutgoing(flow)
    flow.setTarget(target)
    target.addIncoming(flow)

def parseAssign(assignNode, activity, d):
    for p in assignNode.targets:
        if type(p) is ast.Name:

            # get object node from parsed nodes
            foundNode = False
            for parsedNode in activity.nodes:
                if parsedNode.getName() == p.id:

                    # found the node we need to assign
                    if type(node.value) is ast.Constant:

                        # create object node representing value we will assign the node
                        valNode = CreateObjectAction()
                        d[valNode.getID()] = valNode
                        valPin = OutputPin()
                        d[valPin.getID()] = valPin
                        valNode.addOutput(valPin)
                        activity.addNode(valNode)
                        activity.addNode(valPin)
                        # get type and upper bound
                        setNodeTypeLiteral(valPin, node, d)
                        valNode.setClassifier(valPin.getType())

                        # give it a name why not
                        if valPin.getUpperBound != None:
                            valNode.setName(str(valPin.getUpperBound().getValue()))

                        # if other node didn't know type yet let it know
                        if issubclass(parsedNode.__class__, ObjectNode):
                            if parsedNode.getType() == None:
                                parsedNode.setType(valPin.getType())
                                if type(parsedNode) is ParameterNode:
                                    parsedNode.getParameter().setType(valPin.getType())
                        elif type(parsedNode) is CreateObjectAction:
                            if parsedNode.getClassifier() == None:
                                parsedNode.setClassifier(valPin.getType())

                        # map flow
                        objFlow = ObjectFlow()
                        d[objFlow.getID()] = objFlow
                        setSourceAndTarget(objFlow, valPin, parsedNode)
                        activity.addEdge(objFlow)

                        # Override lastNode
                        return valNode
            if not foundNode:
                # it is a new object
                createObject = CreateObjectAction()
                d[createObject.getID()] = createObject
                outPin = OutputPin()
                d[outPin.getID()] = outPin
                createObject.addOutput(outPin)
                activity.addNode(createObject)
                activity.addNode(outPin)

                setNodeTypeLiteral(outPin, node, d)
                createObject.setClassifier(outPin.getType())

                # Create Object node to store value of ret
                objNode = ObjectNode()
                d[objNode.getID()] = objNode
                objNode.setType(outPin.getType())
                objNode.setName(p.id)
                uml.addNode(objNode)

                # Create flow from create to obj
                objFlow = ObjectFlow()
                d[objFlow.getID()] = objFlow
                setSourceAndTarget(objFlow, outPin, objNode)
                uml.addEdge(objFlow)

                # last node for control flow is create
                return createObject

def mapNodeTypeFromDecision(dec, typeName, d):
    if dec.getDecisionInputFlow() != None:
        if dec.getDecisionInputFlow().getSource().getType() == None:
            decType = PrimitiveType()
            d[decType.getID()] = decType
            decType.setPrimitiveType(typeName)
            dec.getDecisionInputFlow().getSource().setType(decType)
            if issubclass(dec.getDecisionInputFlow().getSource().__class__, ParameterNode):
                dec.getDecisionInputFlow().getSource().getParameter().setType(decType)

def mapDecisionInputFlow(dec, uml, name, d):
    for funNode in uml.nodes:
        if funNode.getName() == name:
            if issubclass(funNode.__class__, ObjectNode):
                inputFlow = ObjectFlow()
                d[inputFlow.getID()] = inputFlow
                uml.addEdge(inputFlow)
                inputFlow.setSource(funNode)
                inputFlow.setTarget(dec)
                funNode.addOutgoing(inputFlow)
                dec.addIncoming(inputFlow)
                dec.setDecisionInputFlow(inputFlow)

def parseIF(ifNode, activity, d):
    dec = DecisionNode()
    d[dec.getID()] = dec
    activity.addNode(dec)

    ifNode = iterateThroughBody(ifNode.body, activity, dec, d)
    outFlow1 = ifNode.incoming[0]
    elseNode = iterateThroughBody(ifNode.orelse, activity, dec, d)
    outFlow2 = elseNode.incoming[0]

    # get object input to decision node
    # if it is a name node it is referencing a previously defined variable, so search through all
    # of the objectNode type nodes defined so far for a match and set flow
    if type(node.test) is ast.Compare:
        ifExpr = Expression()
        d[ifExpr.getID()] = ifExpr
        boolType = PrimitiveType()
        d[boolType.getID()] = boolType
        boolType.setPrimitiveType('BOOL')
        ifExpr.setType(boolType)

        # map input
        if type(node.test.left) is ast.Name:
            mapDecisionInputFlow(dec, uml, node.test.left.id, d)

        # map guards
        for comp in node.test.comparators:
            if type(comp.value) is bool:
                opBool = LiteralBool()
                d[opBool.getID()] = opBool
                opBool.setValue(comp.value)
                ifExpr.addOperand(opBool)
                mapNodeTypeFromDecision(dec, 'BOOL', d)
            elif type(comp.value) is int:
                opInt = LiteralInt()
                d[opInt.getID()] = opInt
                opInt.setValue(comp.value)
                ifExpr.addOperand(opInt)
                mapNodeTypeFromDecision(dec, 'INT', d)
            elif type(comp.value) is float:
                opReal = LiteralReal()
                d[opReal.getID()] = opReal
                opReal.setValue(comp.value)
                ifExpr.addOperand(opReal)
                mapNodeTypeFromDecision(dec, 'REAL', d)
            elif type(comp.value) is str:
                opString = LiteralString()
                d[opString.getID()] = opString
                opString.setValue(comp.value)
                ifExpr.addOperand(opString)
                mapNodeTypeFromDecision(dec, 'STRING', d)
        
        # map symbol
        for sym in node.test.ops:
            if type(sym) is ast.Eq:
                ifExpr.setSymbol('==')
        outFlow1.setGuard(ifExpr)

    elif type(node.test) is ast.Name:
        mapDecisionInputFlow(dec, uml, node.test.id, d)
    elif type(node.test) is ast.Expr:
        # TODO
        print('TODO: expressions in if statements')
    # other possibilites are functions (ast.Call) and maybe more

    # handle other bodies
    if firstAndLastNodes2[0].getID() != dec.getID():
        # ControlFlow to body with else guard
        elseExpr = Expression()
        d[elseExpr.getID()] = elseExpr
        elseExpr.setSymbol('else')
        outFlow2.setGuard(elseExpr)

    # map join node
    mergeFlow1 = ControlFlow()
    mergeFlow2 = ControlFlow()
    d[mergeFlow1.getID()] = mergeFlow1
    d[mergeFlow2.getID()] = mergeFlow2
    setSourceAndTarget(mergeFlow1, firstAndLastNodes1[1], merge)
    setSourceAndTarget(mergeFlow2, firstAndLastNodes2[1], merge)
    uml.addNode(merge)
    uml.addEdge(mergeFlow1)
    uml.addEdge(mergeFlow2)

    # override lastNode
    return dec, merge

def parseReturn(node, activity, d):
    # placeholder
    retParamNode = None
    
    # determine if return has already been determined
    defineParameter = True
    for param in activity.parameters:
        if param.getDirection() == 'RETURN':
            defineParameter = False
            for umlNode in activity.nodes:
                if type(umlNode) is ParameterNode:
                    if umlNode.getParameter().getDirection() == 'RETURN':
                        retParamNode = umlNode
            break
    
    # if its not determined create it
    if defineParameter:
        # create parameter
        retParam = Parameter()
        d[retParam.getID()] = retParam
        retParam.setDirection('RETURN')
        activity.addParameter(retParam)

        #create parameterNode
        retParamNode = ParameterNode()
        d[retParamNode.getID()] = retParamNode
        retParamNode.setParameter(retParam)
        activity.addNode(retParamNode)
        #retParamNode.setActivity(uml)

        # get type of return/ objectnode
        if type(node.value) is ast.Call:
            for parsedNode in activity.getOwner().ownedElements:
                if issubclass(parsedNode.__class__, Behavior):
                    if parsedNode.getName() == node.value.func.id:
                        cba = CallBehaviorAction()
                        d[cba.getID()] = cba
                        cba.setBehavior(parsedNode)
                        activity.addNode(cba)
                        #loop through input params (zip should cut off return)
                        for param in zip(parsedNode.parameters, node.value.args):
                            pNode = InputPin()
                            d[pNode.getID()] = pNode
                            pNode.setType(param[0].getType())
                            cba.addInput(pNode)
                            activity.addNode(pNode)
                            if type(param[1]) is ast.Constant:
                                coa = CreateObjectAction()
                                d[coa.getID()] = coa
                                coa.setClassifier(param[0].getType())
                                outPin = OutputPin()
                                d[outPin.getID()] = outPin
                                coa.addOutput(outPin)
                                outPin.setType(param[0].getType())
                                if type(param[1].value) is int:
                                    intVal = LiteralInt()
                                    d[intVal.getID()] = intVal
                                    intVal.setValue(param[1].value)
                                    outPin.setUpperBound(intVal)
                                elif type(param[1].value) is bool:
                                    boolVal = LiteralBool()
                                    d[boolVal.getID()] = boolVal
                                    boolVal.setValue(param[1].value)
                                    outPin.setUpperBound(boolVal)
                                elif type(param[1].value) is float:
                                    realVal = LiteralReal()
                                    d[realVal.getID()] = realVal
                                    realVal.setValue(param[1].value)
                                    outPin.setUpperBound(realVal)
                                elif type(param[1].value) is str:
                                    strVal = LiteralString()
                                    d[strVal.getID()] = strVal
                                    strVal.setValue(param[1].value)
                                    outPin.setUpperBound(strVal)
                                activity.addNode(coa)
                                activity.addNode(outPin)
                                paramFlow = ObjectFlow()
                                d[paramFlow.getID()] = paramFlow
                                setSourceAndTarget(paramFlow, outPin, pNode)
                                activity.addEdge(paramFlow)
                        for param in parsedNode.parameters:
                            if param.getDirection() == 'RETURN':
                                pNode = OutputPin()
                                d[pNode.getID()] = pNode
                                cba.addOutput(pNode)
                                pNode.setType(param.getType())
                                retParamNode.setType(param.getType())
                                retParam.setType(param.getType())
                                activity.addNode(pNode)
                                retFlow = ObjectFlow()
                                d[retFlow.getID()] = retFlow
                                setSourceAndTarget(retFlow, pNode, retParamNode)
                                activity.addEdge(retFlow)
                        return cba

        elif type(node.value) is ast.Constant:
            setNodeTypeLiteral(retParamNode, node, d)
            retParam.setType(retParamNode.getType())
            if type(node.value.value) is bool:
                boolOb = CreateObjectAction()
                d[boolOb.getID()] = boolOb
                boolType = PrimitiveType()
                d[boolType.getID()] = boolType
                boolType.setPrimitiveType('BOOL')
                boolPin = OutputPin()
                d[boolPin.getID()] = boolPin
                boolPin.setType(boolType)
                boolOb.setClassifier(boolType)
                boolVal = LiteralBool()
                d[boolVal.getID()] = boolVal
                boolVal.setValue(node.value.value)
                boolPin.setUpperBound(boolVal)
                boolOb.addOutput(boolPin)
                activity.addNode(boolOb)
                activity.addNode(boolPin)

                # map object flow
                obFlow = ObjectFlow()
                d[obFlow.getID()] = obFlow
                setSourceAndTarget(obFlow, boolPin, retParamNode)
                activity.addEdge(obFlow)
                return boolOb

        elif type(node.value) is ast.Name:
            # find that node in parsed nodes
            for parsedNode in activity.nodes:
                if parsedNode.getName() == node.value.id:
                    if issubclass(parsedNode.__class__, ObjectNode):
                        retParamNode.setType(parsedNode.getType())
                        retParam.setType(parsedNode.getType())
                        obFlow = ObjectFlow()
                        d[obFlow.getID()] = obFlow
                        setSourceAndTarget(obFlow, parsedNode, retParamNode)
                        activity.addEdge(obFlow)
                        # don't override last node
                        return None