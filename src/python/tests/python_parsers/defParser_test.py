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
        self.assertEqual(type(m.ownedElements[1]), Activity)

        # test parameters
        self.assertEqual(m.ownedElements[0].getOwner().getID(), m.getID())
        self.assertEqual(m.ownedElements[0].getName(), 'noParam')
        self.assertEqual(len(m.ownedElements[0].parameters), 1)
        self.assertEqual(m.ownedElements[0].parameters[0].getDirection(), 'RETURN')
        self.assertFalse(m.ownedElements[0].parameters[0].getType() == None)
        self.assertEqual(issubclass(m.ownedElements[0].parameters[0].getType().__class__, PrimitiveType), True)
        self.assertEqual(m.ownedElements[0].parameters[0].getType().getPrimitiveType(), 'BOOL')

        #test nodes
        self.assertEqual(len(m.ownedElements[0].nodes), 3)
        noParam = m.ownedElements[0]
        self.assertEqual(type(noParam.nodes[0]), InitialNode)
        self.assertEqual(type(noParam.nodes[1]), ParameterNode)
        self.assertEqual(noParam.nodes[1].getParameter(), noParam.parameters[0])
        self.assertTrue(noParam.nodes[1].getType() != None)
        self.assertTrue(issubclass(noParam.nodes[1].getType().__class__, PrimitiveType))
        self.assertEqual(noParam.nodes[1].getType().getPrimitiveType(), 'BOOL')
        self.assertTrue(noParam.nodes[1].getUpperBound().getValue())

        #test edges
        self.assertEqual(len(noParam.edges), 2)
        self.assertEqual(type(noParam.edges[0]), ControlFlow)
        self.assertEqual(noParam.edges[0].getSource(), noParam.nodes[0])
        self.assertEqual(noParam.edges[0].getTarget(), noParam.nodes[1])
        self.assertEqual(type(noParam.edges[1]), ControlFlow)
        self.assertEqual(noParam.edges[1].getSource(), noParam.nodes[1])
        self.assertEqual(noParam.edges[1].getTarget(), noParam.nodes[2])


    def testParseFuncNumParam(self):
        d = {}
        m = parseModule('/home/stinky/Projects/yuml_projects/yuml/src/python/tests/examples/fun.py', d)
        self.assertEqual(type(m), Namespace)
        self.assertEqual(len(m.ownedElements), 4)

        # test activity
        self.assertEqual(type(m.ownedElements[1]), Activity)
        self.assertEqual(m.ownedElements[1].getName(), 'numParam')
        numFunc = m.ownedElements[1]
        self.assertEqual(len(numFunc.nodes), 5)
        self.assertEqual(len(numFunc.edges), 4)

        t = numFunc.nodes[2].getType()

        # parameters
        self.assertEqual(len(numFunc.parameters), 2)
        self.assertEqual(numFunc.parameters[0].getName(), 'f')
        self.assertEqual(numFunc.parameters[0].getDirection(), 'IN')
        self.assertTrue(numFunc.parameters[0].getType() != None)
        self.assertTrue(issubclass(numFunc.parameters[0].getType().__class__, PrimitiveType))
        self.assertEqual(numFunc.parameters[0].getType().getPrimitiveType(), 'INT')
        self.assertEqual(numFunc.parameters[1].getDirection(), 'RETURN')
        self.assertTrue(numFunc.parameters[1].getType() != None)

    def testParseDecisionNode(self):
        d = {}
        m = parseModule('/home/stinky/Projects/yuml_projects/yuml/src/python/tests/examples/fun.py', d)
        self.assertEqual(m.ownedElements[2].getOwner().getID(), m.getID())
        self.assertEqual(type(m.ownedElements[2]), Activity)
        self.assertEqual(len(m.ownedElements[2].nodes), 5)

if __name__ == '__main__':
    unittest.main()