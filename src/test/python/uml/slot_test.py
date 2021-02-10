import unittest
from yuml_python import *

class SlotTest(unittest.TestCase):

    def testSetAndGetDefiningFeature(self):
        p = Property()
        s = Slot()
        s.setDefiningFeature(p)
        self.assertEqual(s.getDefiningFeature().getID(), p.getID())

if __name__ == '__main__':
    unittest.main()