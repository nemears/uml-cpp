import ast
from yuml_python import CallBehaviorAction, ControlFlow, ObjectFlow, DecisionNode, MergeNode, ObjectNode, CreateObjectAction, Activity, InitialNode, FinalNode, Parameter, ParameterNode, Behavior, PrimitiveType, LiteralBool, LiteralInt, LiteralReal, LiteralString, Action, Expression, OutputPin

def parseFunctionBody(bodyNode, d, uml, owner, lastNode):
    initNode = lastNode
    init = True
    # go through body
    for node in bodyNode:
        if type(node) is ast.Assign:
            for p in node.targets:
                if type(p) is ast.Name:

                    # get object node from parsed nodes
                    foundNode = False
                    for parsedNode in uml.nodes:
                        if parsedNode.getName() == p.id:

                            # found the node we need to assign
                            if type(node.value) is ast.Constant:

                                # create object node representing value we will assign the node
                                valNode = CreateObjectAction()
                                d[valNode.getID()] = valNode
                                valPin = OutputPin()
                                d[valPin.getID()] = valPin
                                valNode.addOutput(valPin)
                                uml.addNode(valNode)
                                uml.addNode(valPin)

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
                                uml.addEdge(objFlow)

                                # Override lastNode
                                lastNode = valNode
                                foundNode = True
                            break
                    if not foundNode:
                        # it is a new object
                        createObject = CreateObjectAction()
                        d[createObject.getID()] = createObject
                        outPin = OutputPin()
                        d[outPin.getID()] = outPin
                        createObject.addOutput(outPin)
                        uml.addNode(createObject)
                        uml.addNode(outPin)

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
                        lastNode = createObject


        elif type(node) is ast.Expr:
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
            uml.addNode(dec)

            # map control flow to decision
            if not init:
                flow = ControlFlow()
                d[flow.getID()] = flow
                uml.addEdge(flow)
                setSourceAndTarget(flow, lastNode, dec)

            # parse body of decision
            firstAndLastNodes1 = parseFunctionBody(node.body, d, uml, owner, dec)
            firstAndLastNodes2 = parseFunctionBody(node.orelse, d, uml, owner, dec)

            # create join node now
            merge = MergeNode()
            d[merge.getID()] = merge

            #map outgoing control flow
            outFlow1 = ControlFlow()
            outFlow2 = ControlFlow()
            d[outFlow1.getID()] = outFlow1
            d[outFlow2.getID()] = outFlow2
            setSourceAndTarget(outFlow1, dec, firstAndLastNodes1[0])
            setSourceAndTarget(outFlow2, dec, joinNode  if firstAndLastNodes2[0].getID() == dec.getID() else firstAndLastNodes2[0])
            uml.addEdge(outFlow1)
            uml.addEdge(outFlow2)

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
            lastNode = merge

        # find return param
        elif type(node) is ast.Return:

            # placeholder
            retParamNode = None
            
            # determine if return has already been determined
            defineParameter = True
            for param in uml.parameters:
                if param.getDirection() == 'RETURN':
                    defineParameter = False
                    for umlNode in uml.nodes:
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
                uml.addParameter(retParam)

                #create parameterNode
                retParamNode = ParameterNode()
                d[retParamNode.getID()] = retParamNode
                retParamNode.setParameter(retParam)
                uml.addNode(retParamNode)
                #retParamNode.setActivity(uml)

                # get type of return/ objectnode
                if type(node.value) is ast.Constant:
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
                        uml.addNode(boolOb)
                        uml.addNode(boolPin)

                        # map object flow
                        obFlow = ObjectFlow()
                        d[obFlow.getID()] = obFlow
                        setSourceAndTarget(obFlow, boolPin, retParamNode)
                        uml.addEdge(obFlow)
                        lastNode = boolOb

                elif type(node.value) is ast.Name:
                    # find that node in parsed nodes
                    for parsedNode in uml.nodes:
                        if parsedNode.getName() == node.value.id:
                            if issubclass(parsedNode.__class__, ObjectNode):
                                retParamNode.setType(parsedNode.getType())
                                retParam.setType(parsedNode.getType())
                                obFlow = ObjectFlow()
                                d[obFlow.getID()] = obFlow
                                setSourceAndTarget(obFlow, parsedNode, retParamNode)
                                uml.addEdge(obFlow)
                                # don't override last node
                                lastNode = lastNode
                                break
        if init:
            initNode = lastNode
            init = False

    return (initNode, lastNode)

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

def setSourceAndTarget(flow, source, target):
    flow.setSource(source)
    source.addOutgoing(flow)
    flow.setTarget(target)
    target.addIncoming(flow)

def setNodeTypeLiteral(typedElement, astNode, d):
    if type(astNode.value.value) is bool:
        boolType = PrimitiveType()
        d[boolType.getID()] = boolType
        boolType.setPrimitiveType("BOOL")
        typedElement.setType(boolType)
        upperBound = LiteralBool()
        d[upperBound.getID()] = upperBound
        upperBound.setValue(astNode.value.value)
        typedElement.setUpperBound(upperBound)
    elif type(astNode.value.value) is int:
        intType = PrimitiveType()
        d[intType.getID()] = intType
        intType.setPrimitiveType("INT")
        typedElement.setType(intType)
        upperBound = LiteralInt()
        d[upperBound.getID()] = upperBound
        upperBound.setValue(astNode.value.value)
        typedElement.setUpperBound(upperBound)
    elif type(astNode.value.value) is float:
        realType = PrimitiveType()
        d[realType.getID()] = realType
        realType.setPrimitiveType("REAL")
        typedElement.setType(realType)
        upperBound = LiteralReal()
        d[upperBound.getID()] = upperBound
        upperBound.setValue(astNode.value.value)
        typedElement.setUpperBound(upperBound)
    elif type(astNode.value.value) is str:
        strType = PrimitiveType()
        d[strType.getID()] = strType
        strType.setPrimitiveType("STRING")
        typedElement.setType(strType)
        upperBound = LiteralString()
        d[upperBound.getID()] = upperBound
        upperBound.setValue(astNode.value.value)
        typedElement.setUpperBound(upperBound)
    else:
        #TODO throw exception
        print('could not get type of constant return param')

def parseFunction(defNode, d, owner):

    # create action and initial node
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
        pNode.setName(arg.arg)
        d[pNode.getID()] = pNode
        pNode.setParameter(p)
        fun.addNode(pNode)

    # parse body
    firstAndLastNodes = parseFunctionBody(defNode.body, d, fun, owner, initialNode)

    # map control flow
    initToFirstFlow = ControlFlow()
    fun.addEdge(initToFirstFlow)
    d[initToFirstFlow.getID()] = initToFirstFlow
    setSourceAndTarget(initToFirstFlow, initialNode, firstAndLastNodes[0])

    # create final node and flow to final if return node was not last item parsed
    # TODO is this right behavior, or is there always finalNode (CHECK)
    # if (type(firstAndLastNodes[1]) is not ParameterNode):
    #     if firstAndLastNodes[1].getDirection() != 'RETURN':
    lastToFinalFlow = ControlFlow()
    d[lastToFinalFlow.getID()] = lastToFinalFlow
    finalNode = FinalNode() 
    d[finalNode.getID()] = finalNode
    fun.addNode(finalNode)
    fun.addEdge(lastToFinalFlow)
    setSourceAndTarget(lastToFinalFlow, firstAndLastNodes[1], finalNode)

    return fun