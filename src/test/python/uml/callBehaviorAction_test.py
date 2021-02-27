import unittest
from yuml_python import *

class CallBehaviorActionTest(unittest.TestCase):

    def testSetAndGetBehavior(self):
        c = CallBehaviorAction()
        ob = OpaqueBehavior()
        c.setBehavior(ob)
        self.assertEqual(c.getBehavior(), ob)

if __name__ == '__main__':
    unittest.main()