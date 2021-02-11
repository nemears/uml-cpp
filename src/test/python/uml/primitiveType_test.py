import unittest
from yuml_python import *

class PrimitiveTypeTest(unittest.TestCase):

    def testSetAndGetPrimitive(self):
        p = PrimitiveType()
        p.setPrimitiveType("STRING")
        self.assertEqual(p.getPrimitiveType(), "STRING")

if __name__ == '__main__':
    unittest.main()