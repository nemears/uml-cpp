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
        self.assertEqual(len(noParam.edges), 2)
        self.assertEqual(type(noParam.edges[0]), ControlFlow)
        self.assertEqual(noParam.edges[0].getSource(), noParam.nodes[0])
        self.assertEqual(noParam.edges[0].getTarget(), noParam.nodes[2])
        self.assertEqual(type(noParam.edges[1]), ControlFlow)
        self.assertEqual(noParam.edges[1].getSource(), noParam.nodes[2])
        self.assertEqual(noParam.edges[1].getTarget(), noParam.nodes[4])


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
        self.assertEqual(len(m.ownedElements[2].nodes), 6)
        decision = m.ownedElements[2]

        #Parameters
        self.assertEqual(len(decision.parameters), 2)
        self.assertTrue(decision.parameters[0].getName() != '')
        self.assertEqual(decision.parameters[0].getName(), 'b')
        self.assertTrue(decision.parameters[0].getType() != None)
        self.assertEqual(type(decision.parameters[0].getType()), PrimitiveType)
        self.assertEqual(decision.parameters[0].getType().getPrimitiveType(), 'BOOL')
        self.assertEqual(decision.parameters[1].getDirection(), 'RETURN')
        # TODO return type test

        # Initial node
        self.assertEqual(type(decision.nodes[0]), InitialNode)
        initNode = decision.nodes[0]
        self.assertEqual(len(initNode.incoming), 0)
        self.assertEqual(len(initNode.outgoing), 1)
        self.assertEqual(type(initNode.outgoing[0]), ControlFlow)

        #decision node
        self.assertEqual(type(initNode.outgoing[0].getTarget()), DecisionNode)

if __name__ == '__main__':
    unittest.main()