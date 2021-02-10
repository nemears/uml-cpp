import unittest
from yuml_python import *

class InstanceValueTest(unittest.TestCase):

    def testSetAndGetInstance(self):
        i = InstanceSpecification()
        iv = InstanceValue()
        iv.setInstance(i)
        self.assertEqual(iv.getInstance().getID(), i.getID())

if __name__ == '__main__':
    unittest.main()