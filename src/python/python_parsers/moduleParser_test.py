import unittest
from moduleParser import parseModule
from yuml_python import Namespace, Class, Activity, Operation

class parseModuleTest(unittest.TestCase):

    def testParseClass(self):
        d = {}
        m = parseModule('/home/stinky/Projects/yuml_projects/yuml/src/test/python/examples/class.py', d)
        self.assertEqual(type(m), Namespace)
        self.assertEqual(len(m.ownedElements), 1)
        self.assertEqual(type(m.ownedElements[0]), Class)
        self.assertEqual(m.ownedElements[0].getName(), 'Test')

        #attributes
        #int
        self.assertEqual(len(m.ownedElements[0].attributes), 3)
        self.assertEqual(m.ownedElements[0].attributes[0].getName(), 'i')
        self.assertEqual(m.ownedElements[0].attributes[0].getType().isPrimitive(), True)
        self.assertEqual(m.ownedElements[0].attributes[0].getType().getPrimitiveType(), 'INT')
        self.assertEqual(m.ownedElements[0].attributes[0].getDefaultValue().getValue(), 1)
        #bool
        self.assertEqual(m.ownedElements[0].attributes[1].getName(), 'b')
        self.assertEqual(m.ownedElements[0].attributes[1].getType().isPrimitive(), True)
        self.assertEqual(m.ownedElements[0].attributes[1].getType().getPrimitiveType(), 'BOOL')
        self.assertEqual(m.ownedElements[0].attributes[1].getDefaultValue().getValue(), False)
        
        # operation
        self.assertEqual(len(m.ownedElements[0].operations), 1)
        self.assertEqual(type(m.ownedElements[0].operations[0]), Operation)
        self.assertEqual(m.ownedElements[0].operations[0].getName(), 'foo')
        self.assertEqual(len(m.ownedElements[0].operations[0].parameters), 1)
        self.assertEqual(m.ownedElements[0].operations[0].parameters[0].getDirection(), 'RETURN')

    def testParseFunc(self):
        d = {}
        m = parseModule('/home/stinky/Projects/yuml_projects/yuml/src/test/python/examples/fun.py', d)
        self.assertEqual(type(m), Namespace)
        self.assertEqual(len(m.ownedElements), 2)
        self.assertEqual(type(m.ownedElements[0]), Activity)
        self.assertEqual(type(m.ownedElements[1]), Activity)
        self.assertEqual(m.ownedElements[0].getName(), 'noParam')
        self.assertEqual(len(m.ownedElements[0].parameters), 1)
        self.assertEqual(m.ownedElements[0].parameters[0].getDirection(), 'RETURN')
        self.assertEqual(m.ownedElements[1].getName(), 'numParam')
        self.assertEqual(len(m.ownedElements[1].parameters), 2)
        self.assertEqual(m.ownedElements[1].parameters[0].getName(), 'f')
        self.assertEqual(m.ownedElements[1].parameters[0].getDirection(), 'IN')
        self.assertEqual(m.ownedElements[1].parameters[1].getDirection(), 'RETURN')


if __name__ == '__main__':
    unittest.main()