import unittest
from yuml_python import *

class PropertyTest(unittest.TestCase):

    def testSetAndGetImproperDefaultValue(self):
        p = Property()
        v = ValueSpecification()
        with self.assertRaises(InvalidValueException):
            p.setDefaultValue(v)

if __name__ == '__main__':
    unittest.main()