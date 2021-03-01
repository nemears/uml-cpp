import ast
from yuml_python import CallBehaviorAction, ControlFlow, ObjectFlow, DecisionNode, JoinNode, ObjectNode, Activity, InitialNode, FinalNode, Parameter, ParameterNode, Behavior, PrimitiveType, LiteralBool, LiteralInt, LiteralReal, LiteralString, Action

def parseFunctionBody(bodyNode, d, uml, owner, lastNode):
    initNode = lastNode
    init = True
    # go through body
    for node in bodyNode:
        if type(node) is ast.Assign:
            for p in node.targets:
                if type(p) is ast.Name:

                    # get object node from parsed nodes
                    for parsedNode in uml.nodes:
                        if parsedNode.getName() == p.id:

                            # found the node we need to assign
                            if type(node.value) is ast.Constant:

                                # create object node representing value we will assign the node
                                valNode = ObjectNode()
                                d[valNode.getID()] = valNode

                                # get type and upper bound
                                setNodeTypeLiteral(valNode, node, d)

                                # give it a name why not
                                if valNode.getUpperBound != None:
                                    valNode.setName(str(valNode.getUpperBound().getValue()))

                                # map flow
                                objFlow = ObjectFlow()
                                d[objFlow.getID()] = objFlow
                                setSourceAndTarget(objFlow, valNode, parsedNode)
                                uml.addEdge(objFlow)

                                # Override lastNode
                                lastNode = parsedNode
                            break
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
            firstAndLastNodes1 = parseFunctionBody(node.body, d, uml, owner, dec)
            firstAndLastNodes2 = parseFunctionBody(node.orelse, d, uml, owner, dec)

            #map outgoing control flow
            outFlow1 = ControlFlow()
            outFlow2 = ControlFlow()
            d[outFlow1.getID()] = outFlow1
            d[outFlow2.getID()] = outFlow2
            setSourceAndTarget(outFlow1, dec, firstAndLastNodes1[0])
            setSourceAndTarget(outFlow2, dec, firstAndLastNodes2[0])
            uml.addEdge(outFlow1)
            uml.addEdge(outFlow2)

            # map join node
            join = JoinNode()
            d[join.getID()] = join
            joinFlow1 = ControlFlow()
            joinFlow2 = ControlFlow()
            d[joinFlow1.getID()] = joinFlow1
            d[joinFlow2.getID()] = joinFlow2
            setSourceAndTarget(joinFlow1, firstAndLastNodes1[1], join)
            setSourceAndTarget(joinFlow2, firstAndLastNodes2[1], join)
            uml.addNode(join)
            uml.addEdge(joinFlow1)
            uml.addEdge(joinFlow2)

            # override lastNode
            lastNode = join

        # find return param
        elif type(node) is ast.Return:
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
            if issubclass(lastNode.__class__, ObjectNode):
                returnFlow = ObjectFlow()
                d[returnFlow.getID()] = returnFlow
                uml.addEdge(returnFlow)
                setSourceAndTarget(returnFlow, lastNode, retParamNode)
            else:
                returnFlow = ControlFlow()
                d[returnFlow.getID()] = returnFlow
                uml.addEdge(returnFlow)
                setSourceAndTarget(returnFlow, lastNode, retParamNode)
            
            # get type of return/ objectnode
            if type(node.value) is ast.Constant:
                setNodeTypeLiteral(retParamNode, node, d)
                retParam.setType(retParamNode.getType())
            lastNode = retParamNode
        if init:
            initNode = lastNode
            init = False

    return (initNode, lastNode)

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
        d[pNode.getID()] = pNode
        pNode.setParameter(p)
        fun.addNode(pNode)

    # parse body
    firstAndLastNodes = parseFunctionBody(defNode.body, d, fun, owner, initialNode)

    # map control flow
    initToFirstFlow = ControlFlow()
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
    setSourceAndTarget(lastToFinalFlow, firstAndLastNodes[1], finalNode)

    return fun