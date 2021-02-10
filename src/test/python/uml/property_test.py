import unittest
from yuml_python import *

class PropertyTest(unittest.TestCase):

    def testSetAndGetImproperDefaultValue(self):
        p = Property()
        v = ValueSpecification()
        with self.assertRaises(InvalidValueException):
            p.setDefaultValue(v)
    
    def testSetAndGetProperDefaultValue(self):
        c = Class()
        i = InstanceSpecification()
        i.setClassifier(c)
        p = Property()
        p.setType(c)
        iv = InstanceValue()
        iv.setInstance(i)
        p.setDefaultValue(iv)
        self.assertEqual(p.getDefaultValue().getID(), iv.getID())

if __name__ == '__main__':
    unittest.main()