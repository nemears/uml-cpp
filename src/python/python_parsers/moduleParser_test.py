import unittest
from moduleParser import parseModule2
from yuml_python import Namespace, Class

class parseModuleTest(unittest.TestCase):

    def testParseClass(self):
        d = {}
        m = parseModule2('/home/stinky/Projects/yuml_projects/yuml/src/test/python/examples/class.py', d)
        self.assertEqual(type(m), Namespace)
        self.assertEqual(len(m.ownedElements), 1)
        self.assertEqual(type(m.ownedElements[0]), Class)

if __name__ == '__main__':
    unittest.main()