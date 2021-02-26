import unittest
from yuml_python import *

class ObjectNodeTest(unittest.TestCase):

    def testSetType(self):
        c = Class()
        o = ObjectNode()
        o.setType(c)
        self.assertEqual(c.getID(), o.getType().getID())

    def testAddAndRemoveIncomingEdge(self):
        n = ObjectNode()
        e = ActivityEdge()
        n.addIncoming(e)
        self.assertEqual(n.incoming[0].getID(), e.getID())
        n.removeIncoming(e)
        self.assertEqual(len(n.incoming), 0)

    def testAddAndRemoveOutgoingEdge(self):
        n = ObjectNode()
        e = ActivityEdge()
        n.addOutgoing(e)
        self.assertEqual(n.outgoing[0].getID(), e.getID())
        n.removeOutgoing(e)
        self.assertEqual(len(n.outgoing), 0)

    def testSetAndGetUpperBound(self):
        n = ObjectNode()
        i = LiteralInt()
        i.setValue(0)
        n.setUpperBound(i)
        self.assertEqual(n.getUpperBound().getID(), i.getID())
        self.assertEqual(n.getUpperBound().getValue(), 0)

if __name__ == '__main__':
    unittest.main()