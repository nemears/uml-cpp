import unittest
from yuml_python import *

class OperationTest(unittest.TestCase):

    def testGetAndSetType(self):
        t = Classifier()
        o = Operation()
        o.setType(t)
        self.assertEqual(t.getID(), o.getType().getID())

    def testAddAndRemoveParameter(self):
        p = Parameter()
        o = Operation()
        o.addParameter(p)
        self.assertEqual(o.parameters[0].getID(), p.getID())
        o.removeParameter(p)
        self.assertEqual(len(o.parameters) , 0)

if __name__ == '__main__':
    unittest.main()