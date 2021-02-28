import unittest
from yuml_python import *

class ActivityEdgeTest(unittest.TestCase):

    def testSetAndGetGuard(self):
        flow = ControlFlow()
        guard = LiteralBool()
        flow.setGuard(guard)
        self.assertEqual(flow.getGuard(), guard)

if __name__ == '__main__':
    unittest.main()