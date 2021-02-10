import unittest
from yuml_python import *

class ParameterTest(unittest.TestCase):

    def testSetDirectionProper(self):
        p = Parameter()
        p.setDirection("IN")
        self.assertEqual(p.getDirection(), "IN")

    def testSetDirectionImProper(self):
        p = Parameter()
        with self.assertRaises(InvalidDirectionException):
            p.setDirection("INt")

if __name__ == '__main__':
    unittest.main()