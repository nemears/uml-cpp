import unittest
from yuml_python import *

class OpaqueBehaviorTest(unittest.TestCase):

    def testSetAndGetBody(self):
        b = OpaqueBehavior()
        b.setBody("return true")
        self.assertEqual(b.getBody(), "return true")

if __name__ == '__main__':
    unittest.main()