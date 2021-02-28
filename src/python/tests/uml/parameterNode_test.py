import unittest
from yuml_python import *

class ParameterNodeTest(unittest.TestCase):

    def testSetAndGetParameter(self):
        param = Parameter()
        paramNode = ParameterNode()
        paramNode.setParameter(param)
        self.assertEqual(param.getID(), paramNode.getParameter().getID())

if __name__ == '__main__':
    unittest.main()