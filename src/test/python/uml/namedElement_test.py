import unittest
from yuml_python import *

class NamedElementTest(unittest.TestCase):

    def testSetNameAndGetName(self):
        n = NamedElement()
        n.setName("test")
        self.assertEqual(n.getName(), "test")
    
    # this is just my sanity check that inheritance works
    def testElementConstructor(self):
        n = NamedElement()
        self.assertTrue(isValidUUID4(n.getID())) 

if __name__ == '__main__':
    unittest.main()