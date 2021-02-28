import unittest
from yuml_python import *

class TypeTest(unittest.TestCase):

    def testIsPrimitive(self):
        t = Class()
        t.getID()
        self.assertTrue(t.isPrimitive() == False)

if __name__ == '__main__':
    unittest.main()