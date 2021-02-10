import unittest
from yuml_python import *

class InstanceSpecificationTest(unittest.TestCase):

    def testSetAndGetClassifier(self):
        c = Class()
        i = InstanceSpecification()
        i.setClassifier(c)
        self.assertEqual(i.getClassifier().getID(), c.getID())

    def testAddAndRemoveSlot(self):
        i = InstanceSpecification()
        s = Slot()
        i.addSlot(s)
        self.assertEqual(i.slots[0].getID(), s.getID())

if __name__ == '__main__':
    unittest.main()