import unittest
from yuml_python import *

class SlotTest(unittest.TestCase):

    def testSetAndGetDefiningFeature(self):
        p = Property()
        s = Slot()
        s.setDefiningFeature(p)
        self.assertEqual(s.getDefiningFeature().getID(), p.getID())
    
    def testAddAndRemoveValue(self):
        p = Property()
        pt = PrimitiveType()
        pt.setPrimitiveType("INT")
        p.setType(pt)
        p.setLower(0)
        p.setUpper(3)
        s = Slot()
        s.setDefiningFeature(p)
        i = LiteralInt()
        i.setValue(1)
        s.addValue(i)
        self.assertEqual(s.values[0].getID(), i.getID())
        s.removeValue(i)
        self.assertEqual(len(s.values), 0)
        

if __name__ == '__main__':
    unittest.main()