import unittest
from moduleParser import parseModule2
from yuml_python import Namespace, Class, Activity, Operation

class parseModuleTest(unittest.TestCase):

    def testParseClass(self):
        d = {}
        m = parseModule2('/home/stinky/Projects/yuml_projects/yuml/src/test/python/examples/class.py', d)
        self.assertEqual(type(m), Namespace)
        self.assertEqual(len(m.ownedElements), 1)
        self.assertEqual(type(m.ownedElements[0]), Class)
        self.assertEqual(m.ownedElements[0].getName(), 'Test')
        self.assertEqual(len(m.ownedElements[0].operations), 1)
        self.assertEqual(type(m.ownedElements[0].operations[0]), Operation)
        self.assertEqual(m.ownedElements[0].operations[0].getName(), 'foo')
        self.assertEqual(len(m.ownedElements[0].operations[0].parameters), 1)
        self.assertEqual(m.ownedElements[0].operations[0].parameters[0].getDirection(), 'RETURN')

    def testParseFunc(self):
        d = {}
        m = parseModule2('/home/stinky/Projects/yuml_projects/yuml/src/test/python/examples/fun.py', d)
        self.assertEqual(type(m), Namespace)
        self.assertEqual(len(m.ownedElements), 2)
        self.assertEqual(type(m.ownedElements[0]), Activity)


if __name__ == '__main__':
    unittest.main()