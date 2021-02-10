import unittest
from yuml_python import *

class ClassTest(unittest.TestCase):

    def testAddAndRemoveOperation(self):
        c = Class()
        o = Operation()
        c.addOperation(o)
        self.assertEqual(c.operations[0].getID(), o.getID())
        c.removeOperation(o)
        self.assertEqual(len(c.operations), 0)

if __name__ == '__main__':
    unittest.main()