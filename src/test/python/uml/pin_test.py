import unittest
from yuml_python import *

class PinTest(unittest.TestCase):

    def testType(self):
        p = Pin()
        c = Class()
        p.setType(c)
        self.assertEqual(p.getType().getID(), c.getID())

    def testMultiplicity(self):
        p = Pin()
        p.setLower(0)
        p.setUpper(10)
        self.assertEqual(p.getLower(), 0)
        self.assertEqual(p.getUpper(), 10)

if __name__ == '__main__':
    unittest.main()