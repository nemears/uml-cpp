import unittest
from yuml_python import *

class TypedElementTest(unittest.TestCase):

    def testSetType(self):
        typedEl = TypedElement()
        t = Type()
        typedEl.setType(t)
        self.assertEqual(typedEl.getType().getID(), t.getID())

if __name__ == '__main__':
    unittest.main()