import unittest
from yuml_python import *

class PrimitiveTypeTest(unittest.TestCase):

    def testSetAndGetPrimitive(self):
        p = PrimitiveType()
        p.setPrimitiveType("STRING")
        self.assertEqual(p.getPrimitiveType(), 'STRING')
        
    def testInheritance(self):
        p = PrimitiveType()
        self.assertTrue(issubclass(p.__class__, Classifier))
        self.assertTrue(issubclass(p.__class__, Type))

if __name__ == '__main__':
    unittest.main()