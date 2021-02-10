import unittest
from yuml_python import *

class InstanceSpecificationTest(unittest.TestCase):

    def testSetAndGetClassifier(self):
        c = Class()
        i = InstanceSpecification()
        i.setClassifier(c)
        self.assertEqual(i.getClassifier().getID(), c.getID())

if __name__ == '__main__':
    unittest.main()