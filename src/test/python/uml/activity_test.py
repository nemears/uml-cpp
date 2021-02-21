import unittest
from yuml_python import *

class ActivityTest(unittest.TestCase):

    def testAddAndRemoveNode(self):
        a = Activity()
        node = ActivityNode()
        a.addNode(node)
        self.assertEqual(node.getID(), a.nodes[0].getID())
        a.removeNode(node)
        self.assertEqual(len(a.nodes), 0)

if __name__ == '__main__':
    unittest.main()