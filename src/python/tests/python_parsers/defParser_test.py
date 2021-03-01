import unittest
import sys
import os
sys.path.append(os.path.join(os.path.dirname(__file__), "..", ".."))
from python_parsers.moduleParser import parseModule
from yuml_python import *

class defParserTest(unittest.TestCase):

    def testParseFunc(self):
        d = {}
        m = parseModule('/home/stinky/Projects/yuml_projects/yuml/src/python/tests/examples/fun.py', d)
        self.assertEqual(type(m), Namespace)
        self.assertEqual(len(m.ownedElements), 4)
        self.assertEqual(type(m.ownedElements[0]), Activity)
        self.assertEqual(type(m.ownedElements[1]), Activity)
        self.assertEqual(m.ownedElements[0].getOwner().getID(), m.getID())
        self.assertEqual(m.ownedElements[0].getName(), 'noParam')
        self.assertEqual(len(m.ownedElements[0].parameters), 1)
        self.assertEqual(m.ownedElements[0].parameters[0].getDirection(), 'RETURN')
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