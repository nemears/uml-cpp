import unittest
from yuml_python import *

class PropertyTest(unittest.TestCase):

    def testSetAndGetImproperDefaultValue(self):
        p = Property()
        v = ValueSpecification()
        with self.assertRaises(InvalidValueException):
            p.setDefaultValue(v)
    
    def testSetAndGetProperDefaultValueInstance(self):
        c = Class()
        i = InstanceSpecification()
        i.setClassifier(c)
        p = Property()
        p.setType(c)
        iv = InstanceValue()
        iv.setInstance(i)
        p.setDefaultValue(iv)
        self.assertEqual(p.getDefaultValue().getID(), iv.getID())

    def testSetAndGetProperDefaultValueBool(self):
        p = Property()
        t = PrimitiveType()
        t.setPrimitiveType("BOOL")
        p.setType(t)
        b = LiteralBool()
        b.setValue(True)
        p.setDefaultValue(b)
        self.assertTrue(p.getDefaultValue().getValue())

    def testSetAndGetProperDefaultValueInt(self):
        p = Property()
        t = PrimitiveType()
        t.setPrimitiveType("INT")
        p.setType(t)
        i = LiteralInt()
        i.setValue(1)
        p.setDefaultValue(i)
        self.assertEqual(p.getDefaultValue().getValue(), 1)

    def testSetAndGetProperDefaultValueReal(self):
        p = Property()
        t = PrimitiveType()
        t.setPrimitiveType("REAL")
        p.setType(t)
        r = LiteralReal()
        r.setValue(1.1)
        p.setDefaultValue(r)
        self.assertEqual(p.getDefaultValue().getValue(), 1.1)

    def testSetAndGetProperDefaultValueString(self):
        p = Property()
        t = PrimitiveType()
        t.setPrimitiveType("STRING")
        p.setType(t)
        s = LiteralString()
        s.setValue('cat')
        p.setDefaultValue(s)
        self.assertEqual(p.getDefaultValue().getValue(), 'cat')

if __name__ == '__main__':
    unittest.main()