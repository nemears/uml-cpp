import unittest
import sys
import os
import pathlib
sys.path.append(os.path.join(os.path.dirname(__file__), "..", ".."))
from python_parsers.moduleParser import parseModule
from yuml_python import *

class classParserTest(unittest.TestCase):

    def testParseClass(self):
        d = {}
        m = parseModule(str(pathlib.Path(__file__).parent.absolute()) + '/../examples/classParser_test/class.py', d)
        self.assertEqual(type(m), Namespace)
        self.assertEqual(len(m.ownedElements), 1)
        self.assertEqual(type(m.ownedElements[0]), Class)
        self.assertEqual(m.ownedElements[0].getName(), 'Test')
        self.assertEqual(m.ownedElements[0].getOwner().getID(), m.getID())

        #attributes
        #int
        self.assertEqual(len(m.ownedElements[0].attributes), 4)
        self.assertEqual(m.ownedElements[0].attributes[0].getName(), 'i')
        self.assertEqual(m.ownedElements[0].attributes[0].getType().isPrimitive(), True)
        self.assertEqual(m.ownedElements[0].attributes[0].getType().getPrimitiveType(), 'INT')
        self.assertEqual(m.ownedElements[0].attributes[0].getDefaultValue().getValue(), 1)
        #bool
        self.assertEqual(m.ownedElements[0].attributes[1].getName(), 'b')
        self.assertEqual(m.ownedElements[0].attributes[1].getType().isPrimitive(), True)
        self.assertEqual(m.ownedElements[0].attributes[1].getType().getPrimitiveType(), 'BOOL')
        self.assertEqual(m.ownedElements[0].attributes[1].getDefaultValue().getValue(), False)
        #str
        self.assertEqual(m.ownedElements[0].attributes[2].getName(), 's')
        self.assertEqual(m.ownedElements[0].attributes[2].getType().isPrimitive(), True)
        self.assertEqual(m.ownedElements[0].attributes[2].getType().getPrimitiveType(), 'STRING')
        self.assertEqual(m.ownedElements[0].attributes[2].getDefaultValue().getValue(), 'a')
        #real
        self.assertEqual(m.ownedElements[0].attributes[3].getName(), 'r')
        self.assertEqual(m.ownedElements[0].attributes[3].getType().isPrimitive(), True)
        self.assertEqual(m.ownedElements[0].attributes[3].getType().getPrimitiveType(), 'REAL')
        self.assertEqual(m.ownedElements[0].attributes[3].getDefaultValue().getValue(), 3.14)
        
        # operation
        self.assertEqual(len(m.ownedElements[0].operations), 1)
        self.assertEqual(type(m.ownedElements[0].operations[0]), Operation)
        self.assertEqual(m.ownedElements[0].operations[0].getName(), 'foo')
        self.assertEqual(len(m.ownedElements[0].operations[0].parameters), 1)
        self.assertEqual(m.ownedElements[0].operations[0].parameters[0].getDirection(), 'RETURN')

        uml = Model()
        uml.addOwnedElement(m)
        emitter = ModelParser()
        with open(str(pathlib.Path(__file__).parent.absolute()) + '/output/classParserTestOutput.yml', 'w') as f:
            f.write(emitter.emit(uml))
            f.close()

if __name__ == '__main__':
    unittest.main()