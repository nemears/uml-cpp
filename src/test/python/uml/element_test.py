import unittest
from yuml_python import *

class ElementTest(unittest.TestCase):
    def testSetValidUUID4(self):
        e = Element()
        e.setID("c0ab87cc-d00b-4afb-9558-538253b442b2")
        self.assertEqual(e.getID(), "c0ab87cc-d00b-4afb-9558-538253b442b2")

    def testSetInvalidUUID4(self):
        e = Element()
        with self.assertRaises(InvalidID_Exception):
            e.setID("i am not a valid UUID4")

if __name__ == '__main__':
    unittest.main()