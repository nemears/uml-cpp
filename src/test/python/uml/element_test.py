import unittest
from yuml_python import *

class ElementTest(unittest.TestCase):
    def testSetValidUUID4(self):
        e = Element()
        e.setID("c0ab87cc-d00b-4afb-9558-538253b442b2")
        self.assertEqual(e.getID(), "c0ab87cc-d00b-4afb-9558-538253b442b2")

    def testSetInvalidUUID4(self):
        # this also tests UML::isValidUUID4
        e = Element()
        with self.assertRaises(InvalidID_Exception):
            e.setID("i am not a valid UUID4")

    def testAddOwnedElement(self):
        parent = Element()
        child = Element()
        parent.addOwnedElement(child)
        self.assertEqual(child, parent.ownedElements[0])

    def testRemoveOwnedElement(self):
        parent = Element()
        child = Element()
        parent.addOwnedElement(child)
        parent.removeOwnedElement(child)
        self.assertEqual(len(parent.ownedElements), 0)

    def testSetAndGetOwner(self):
        parent = Element()
        child = Element()
        child.setOwner(parent)
        self.assertEqual(child.getOwner().getID(), parent.getID())

if __name__ == '__main__':
    unittest.main()