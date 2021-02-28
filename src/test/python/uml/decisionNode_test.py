import unittest
from yuml_python import *

class DecisionNodeTest(unittest.TestCase):

    def testSetAndGetDecisionInputFlow(self):
        decisionNode = DecisionNode()
        inputFlow = ObjectFlow()
        decisionNode.setDecisionInputFlow(inputFlow)
        self.assertEqual(decisionNode.getDecisionInputFlow(), inputFlow)

if __name__ == '__main__':
    unittest.main()