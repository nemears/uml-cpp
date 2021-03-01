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
        typeP = m.ownedElements[0].parameters[0].getType()
        self.assertEqual(issubclass(m.ownedElements[0].parameters[0].getType().__class__, PrimitiveType), True)
        self.assertEqual(m.ownedElements[0].parameters[0].getType().getPrimitiveType(), 'BOOL')

        #test nodes
        self.assertEqual(len(m.ownedElements[0].nodes), 2)
        noParam = m.ownedElements[0]
        self.assertEqual(type(noParam.nodes[0]), InitialNode)
        self.assertEqual(type(noParam.nodes[1]), ParameterNode)
        self.assertEqual(noParam.nodes[1].getParameter(), noParam.parameters[0])

        #test edges
        self.assertEqual(len(noParam.edges), 1)
        self.assertEqual(type(noParam.edges[0]), ControlFlow)
        self.assertEqual(noParam.edges[0].getSource(), noParam.nodes[0])
        self.assertEqual(noParam.edges[0].getTarget(), noParam.nodes[1])


    def testParseFuncNumParam(self):
        d = {}
        m = parseModule('/home/stinky/Projects/yuml_projects/yuml/src/python/tests/examples/fun.py', d)
        self.assertEqual(type(m), Namespace)
        self.assertEqual(len(m.ownedElements), 4)
        self.assertEqual(type(m.ownedElements[0]), Activity)
        self.assertEqual(type(m.ownedElements[1]), Activity)

        #this tests numParam function
        self.assertEqual(m.ownedElements[1].getOwner().getID(), m.getID())
        self.assertEqual(m.ownedElements[1].getName(), 'numParam')
        self.assertEqual(len(m.ownedElements[1].parameters), 2)
        self.assertEqual(m.ownedElements[1].parameters[0].getName(), 'f')
        self.assertEqual(m.ownedElements[1].parameters[0].getDirection(), 'IN')
        self.assertEqual(m.ownedElements[1].parameters[1].getDirection(), 'RETURN')

        self.assertEqual(m.ownedElements[2].getOwner().getID(), m.getID())
        self.assertEqual(type(m.ownedElements[2]), Activity)
        self.assertEqual(len(m.ownedElements[2].nodes), 3)

if __name__ == '__main__':
    unittest.main()