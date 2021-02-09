import unittest
from yuml_python import *

class ClassifierTest(unittest.TestCase):

    def testAddAttribute(self):
        c = Classifier()
        p = Property()
        c.addAttribute(p)
        self.assertEqual(c.attributes[0].getID(), p.getID())

    def testRemoveAttribute(self):
        c = Classifier()
        p = Property()
        c.addAttribute(p)
        c.removeAttribute(p)
        self.assertTrue(len(c.attributes) == 0)

if __name__ == '__main__':
    unittest.main()