import unittest
from yuml_python import *

class ClassifierTest(unittest.TestCase):

    def testAddAttribute(self):
        c = Classifier()
        p = Property()
        c.addAttribute(p)
        self.assertEqual(c.attributes[0].getID(), p.getID())

if __name__ == '__main__':
    unittest.main()