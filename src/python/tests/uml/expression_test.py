import unittest
from yuml_python import *

class ExpressionTest(unittest.TestCase):

    def testAddAndRemoveOperand(self):
        e = Expression()
        i = LiteralInt()
        e.addOperand(i)
        self.assertEqual(e.operands[0], i)
        e.removeOperand(i)
        self.assertEqual(len(e.operands), 0)
    
    def testSetAndGetSymbol(self):
        e = Expression()
        e.setSymbol("+")
        self.assertEqual(e.getSymbol(), "+")

if __name__ == '__main__':
    unittest.main()