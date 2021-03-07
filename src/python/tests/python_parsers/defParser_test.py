import unittest
import sys
import os
sys.path.append(os.path.join(os.path.dirname(__file__), "..", ".."))
from python_parsers.moduleParser import parseModule
from yuml_python import *

class defParserTest(unittest.TestCase):

    def testParseFuncNoParam(self):
        d = {}
        m = parseModule('/home/stinky/Projects/yuml_projects/yuml/src/python/tests/examples/fun.py', d)
        self.assertEqual(type(m), Namespace)
        self.assertEqual(len(m.ownedElements), 4)
        self.assertEqual(type(m.ownedElements[0]), Activity)

        # test parameters
        self.assertEqual(m.ownedElements[0].getOwner().getID(), m.getID())
        self.assertEqual(m.ownedElements[0].getName(), 'noParam')
        self.assertEqual(len(m.ownedElements[0].parameters), 1)
        self.assertEqual(m.ownedElements[0].parameters[0].getDirection(), 'RETURN')
        self.assertFalse(m.ownedElements[0].parameters[0].getType() == None)
        self.assertEqual(issubclass(m.ownedElements[0].parameters[0].getType().__class__, PrimitiveType), True)
        self.assertEqual(m.ownedElements[0].parameters[0].getType().getPrimitiveType(), 'BOOL')

        #test nodes
        self.assertEqual(len(m.ownedElements[0].nodes), 5)
        noParam = m.ownedElements[0]
        self.assertEqual(type(noParam.nodes[0]), InitialNode)
        self.assertEqual(type(noParam.nodes[1]), ParameterNode)
        self.assertEqual(noParam.nodes[1].getParameter(), noParam.parameters[0])
        self.assertTrue(noParam.nodes[1].getType() != None)
        self.assertTrue(issubclass(noParam.nodes[1].getType().__class__, PrimitiveType))
        self.assertEqual(noParam.nodes[1].getType().getPrimitiveType(), 'BOOL')
        self.assertTrue(noParam.nodes[1].getUpperBound().getValue())

        # Create Object Action
        self.assertEqual(type(noParam.nodes[2]), CreateObjectAction)
        self.assertTrue(noParam.nodes[2].getClassifier() != None)
        self.assertEqual(type(noParam.nodes[2].getClassifier()), PrimitiveType)
        self.assertEqual(noParam.nodes[2].getClassifier().getPrimitiveType(), 'BOOL')
        self.assertTrue(len(noParam.nodes[2].outputs) == 1)
        self.assertEqual(noParam.nodes[3], noParam.nodes[2].outputs[0])
        self.assertEqual(type(noParam.nodes[3].getType()), PrimitiveType)
        self.assertEqual(noParam.nodes[3].getType().getPrimitiveType(), 'BOOL')
        self.assertTrue(noParam.nodes[3].getUpperBound() != None)
        self.assertTrue(noParam.nodes[3].getUpperBound().getValue())
        self.assertEqual(len(noParam.nodes[2].incoming), 1)
        self.assertEqual(len(noParam.nodes[2].outgoing), 1)

        # Final node
        self.assertEqual(type(noParam.nodes[4]), FinalNode)
        self.assertEqual(len(noParam.nodes[4].incoming), 1)

        #test edges
        self.assertEqual(len(noParam.edges), 3)
        self.assertEqual(type(noParam.edges[0]), ObjectFlow)
        self.assertEqual(noParam.edges[0].getSource(), noParam.nodes[3])
        self.assertEqual(noParam.edges[0].getTarget(), noParam.nodes[1])
        self.assertEqual(type(noParam.edges[1]), ControlFlow)
        self.assertEqual(noParam.edges[1].getSource(), noParam.nodes[0])
        self.assertEqual(noParam.edges[1].getTarget(), noParam.nodes[2])
        self.assertEqual(type(noParam.edges[2]), ControlFlow)
        self.assertEqual(noParam.edges[2].getSource(), noParam.nodes[2])
        self.assertEqual(noParam.edges[2].getTarget(), noParam.nodes[4])

        uml = Model()
        uml.addOwnedElement(m)
        emitter = ModelParser()
        with open('/home/stinky/Projects/yuml_projects/yuml/src/python/tests/python_parsers/output/defParserTestOutput.yml', 'w') as f:
            f.write(emitter.emit(uml))
            f.close()

    def testParseFuncNumParam(self):
        d = {}
        m = parseModule('/home/stinky/Projects/yuml_projects/yuml/src/python/tests/examples/fun.py', d)
        self.assertEqual(type(m), Namespace)
        self.assertEqual(len(m.ownedElements), 4)

        # test activity
        self.assertEqual(type(m.ownedElements[1]), Activity)
        self.assertEqual(m.ownedElements[1].getName(), 'numParam')
        numFunc = m.ownedElements[1]
        self.assertEqual(len(numFunc.nodes), 6)
        self.assertEqual(len(numFunc.edges), 4)

        # parameters
        self.assertEqual(len(numFunc.parameters), 2)
        self.assertEqual(numFunc.parameters[0].getName(), 'f')
        self.assertEqual(numFunc.parameters[0].getDirection(), 'IN')
        self.assertTrue(numFunc.parameters[0].getType() != None)
        self.assertTrue(issubclass(numFunc.parameters[0].getType().__class__, PrimitiveType))
        self.assertEqual(numFunc.parameters[0].getType().getPrimitiveType(), 'INT')
        self.assertEqual(numFunc.parameters[1].getDirection(), 'RETURN')
        self.assertTrue(numFunc.parameters[1].getType() != None)
        self.assertTrue(issubclass(numFunc.parameters[1].getType().__class__, PrimitiveType))
        self.assertEqual(numFunc.parameters[1].getType().getPrimitiveType(), 'INT')

        # nodes
        self.assertEqual(type(numFunc.nodes[0]), InitialNode)
        self.assertEqual(len(numFunc.nodes[0].outgoing), 1)
        self.assertEqual(type(numFunc.nodes[1]), ParameterNode)
        self.assertEqual(numFunc.nodes[1].getParameter(), numFunc.parameters[0])
        self.assertEqual(len(numFunc.nodes[1].incoming), 1)
        self.assertEqual(numFunc.nodes[1].incoming[0].getSource(), numFunc.nodes[3])
        
        # createObjectAction
        self.assertEqual(type(numFunc.nodes[2]), CreateObjectAction)
        coa = numFunc.nodes[2]
        self.assertEqual(len(coa.incoming), 1)
        self.assertEqual(type(coa.incoming[0]), ControlFlow)
        self.assertEqual(len(coa.outgoing), 1)
        self.assertEqual(type(coa.outgoing[0]), ControlFlow)
        self.assertEqual(len(coa.inputs), 0)
        self.assertEqual(len(coa.outputs), 1)
        self.assertTrue(coa.getClassifier() != None)
        self.assertEqual(type(coa.getClassifier()), PrimitiveType)
        self.assertEqual(coa.getClassifier().getPrimitiveType(), 'INT')
        self.assertEqual(coa.outputs[0], numFunc.nodes[3])
        
        # outputPin
        self.assertEqual(type(coa.outputs[0]), OutputPin)
        out = coa.outputs[0]
        self.assertEqual(len(out.incoming), 0)
        self.assertEqual(len(out.outgoing), 1)
        self.assertTrue(out.getType() != None)
        self.assertEqual(type(out.getType()), PrimitiveType)
        self.assertEqual(out.getType().getPrimitiveType(), 'INT')
        self.assertTrue(out.getUpperBound() != None)
        self.assertEqual(type(out.getUpperBound()), LiteralInt)
        self.assertEqual(out.getUpperBound().getValue(), 1)

        # Return Node
        self.assertEqual(type(numFunc.nodes[4]), ParameterNode)
        retNode = numFunc.nodes[4]
        self.assertEqual(len(retNode.incoming), 1)
        self.assertEqual(len(retNode.outgoing), 0)

        # Final Node
        self.assertEqual(type(numFunc.nodes[5]), FinalNode)
        finNode = numFunc.nodes[5]
        self.assertEqual(len(finNode.incoming), 1)
        self.assertEqual(len(finNode.outgoing), 0)

    def testParseDecisionNode(self):
        d = {}
        m = parseModule('/home/stinky/Projects/yuml_projects/yuml/src/python/tests/examples/fun.py', d)
        self.assertEqual(m.ownedElements[2].getOwner().getID(), m.getID())
        self.assertEqual(type(m.ownedElements[2]), Activity)
        self.assertEqual(len(m.ownedElements[2].nodes), 13)
        decision = m.ownedElements[2]

        # Test nodes one at a time
        # initial node
        self.assertEqual(type(decision.nodes[0]), InitialNode)
        initNode = decision.nodes[0]
        self.assertEqual(len(initNode.incoming), 0)
        self.assertEqual(len(initNode.outgoing), 1)

        # parameter node
        self.assertEqual(type(decision.nodes[1]), ParameterNode)
        inParamNode = decision.nodes[1]
        self.assertEqual(decision.parameters[0], inParamNode.getParameter())
        self.assertEqual(len(inParamNode.incoming), 0)
        self.assertEqual(len(inParamNode.outgoing), 1)
        self.assertTrue(inParamNode.getType() != None)
        self.assertEqual(type(inParamNode.getType()), PrimitiveType)
        self.assertEqual(inParamNode.getType().getPrimitiveType(), 'INT')
        
        # create object action 1
        self.assertEqual(type(decision.nodes[2]), CreateObjectAction)
        coa1 = decision.nodes[2]
        self.assertEqual(len(coa1.incoming), 1)
        self.assertEqual(len(coa1.outgoing), 1)
        self.assertTrue(coa1.getClassifier() != None)
        self.assertEqual(type(coa1.getClassifier()), PrimitiveType)
        self.assertEqual(coa1.getClassifier().getPrimitiveType(), 'STRING')
        self.assertEqual(len(coa1.inputs), 0)
        self.assertEqual(len(coa1.outputs), 1)

        # outputPin 1
        self.assertEqual(coa1.outputs[0], decision.nodes[3])
        self.assertEqual(type(coa1.outputs[0]), OutputPin)
        outPin1 = coa1.outputs[0]
        self.assertEqual(len(outPin1.incoming), 0)
        self.assertEqual(len(outPin1.outgoing), 1)
        self.assertTrue(outPin1.getType() != None)
        self.assertEqual(type(outPin1.getType()), PrimitiveType)
        self.assertEqual(outPin1.getType().getPrimitiveType(), 'STRING')
        self.assertTrue(outPin1.getUpperBound() != None)
        self.assertEqual(type(outPin1.getUpperBound()), LiteralString)
        self.assertEqual(outPin1.getUpperBound().getValue(), 'c')

        # ObjectNode for ret
        self.assertEqual(outPin1.outgoing[0].getTarget(), decision.nodes[4])
        self.assertEqual(type(decision.nodes[4]), ObjectNode)
        retObNode = decision.nodes[4]
        self.assertEqual(len(retObNode.incoming), 3)
        self.assertEqual(len(retObNode.outgoing), 1)
        self.assertTrue(retObNode.getType() != None)
        self.assertEqual(type(retObNode.getType()), PrimitiveType)
        self.assertEqual(retObNode.getType().getPrimitiveType(), 'STRING')
        
        # decision node
        self.assertEqual(decision.nodes[5], coa1.outgoing[0].getTarget())
        self.assertEqual(type(decision.nodes[5]), DecisionNode)
        decisionNode = decision.nodes[5]
        self.assertEqual(len(decisionNode.incoming), 2)
        self.assertEqual(len(decisionNode.outgoing), 2)
        self.assertTrue(decisionNode.getDecisionInputFlow() != None)
        self.assertEqual(decisionNode.getDecisionInputFlow().getSource(), decision.nodes[1])
        self.assertTrue(decisionNode.outgoing[0].getGuard() != None)
        self.assertEqual(type(decisionNode.outgoing[0].getGuard()), Expression)
        self.assertEqual(decisionNode.outgoing[0].getGuard().getSymbol(), '==')
        self.assertEqual(len(decisionNode.outgoing[0].getGuard().operands), 1)
        self.assertTrue(decisionNode.outgoing[1].getGuard() != None)
        self.assertEqual(type(decisionNode.outgoing[1].getGuard()), Expression)
        self.assertEqual(len(decisionNode.outgoing[1].getGuard().operands), 0)
        self.assertEqual(decisionNode.outgoing[1].getGuard().getSymbol(), 'else')

        # CreateObjectAction2 (positive)
        self.assertEqual(decision.nodes[6], decisionNode.outgoing[0].getTarget())
        self.assertEqual(type(decision.nodes[6]), CreateObjectAction)

        #CreateObjectAction3 (els)
        self.assertEqual(decision.nodes[8], decisionNode.outgoing[1].getTarget())
        self.assertEqual(type(decision.nodes[8]), CreateObjectAction)

        # merge
        self.assertEqual(decision.nodes[6].outgoing[0].getTarget(), decision.nodes[8].outgoing[0].getTarget())
        self.assertEqual(decision.nodes[10], decision.nodes[6].outgoing[0].getTarget())
        mergeNode = decision.nodes[10]
        self.assertEqual(type(mergeNode), MergeNode)
        self.assertEqual(len(mergeNode.incoming), 2)
        self.assertEqual(len(mergeNode.outgoing), 1)
        
        # return parameter
        self.assertEqual(type(decision.nodes[11]), ParameterNode)
        retNode = decision.nodes[11]
        self.assertTrue(retNode.getParameter() != None)
        self.assertEqual(retNode.getParameter(), decision.parameters[1])
        self.assertTrue(retNode.getType() != None)
        self.assertEqual(type(retNode.getType()), PrimitiveType)
        self.assertEqual(retNode.getType().getPrimitiveType(), 'STRING')

        # final node
        self.assertEqual(decision.nodes[12], mergeNode.outgoing[0].getTarget())
        self.assertEqual(type(decision.nodes[12]), FinalNode)
        finalNode = decision.nodes[12]
        self.assertEqual(len(finalNode.incoming), 1)
        self.assertEqual(len(finalNode.outgoing), 0)

    def testCallBehavior(self):
        d = {}
        m = parseModule('/home/stinky/Projects/yuml_projects/yuml/src/python/tests/examples/fun.py', d)
        callBhv = m.ownedElements[3]
        self.assertEqual(type(callBhv), Activity)
        self.assertEqual(len(callBhv.nodes), 8)
        self.assertEqual(len(callBhv.edges), 4)

if __name__ == '__main__':
    unittest.main()