import unittest
from yuml_python import *

class ActivityNodeTest(unittest.TestCase):

    def testAddAndRemoveIncomingEdge(self):
        n = ActivityNode()
        e = ActivityEdge()
        n.addIncoming(e)
        self.assertEqual(n.incoming[0].getID(), e.getID())
        n.removeIncoming(e)
        self.assertEqual(len(n.incoming), 0)

    def testAddAndRemoveOutgoingEdge(self):
        n = ActivityNode()
        e = ActivityEdge()
        n.addOutgoing(e)
        self.assertEqual(n.outgoing[0].getID(), e.getID())
        n.removeOutgoing(e)
        self.assertEqual(len(n.outgoing), 0)

if __name__ == '__main__':
    unittest.main()