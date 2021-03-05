import unittest
from yuml_python import *

class CreateObjectActionTest(unittest.TestCase):

    def testSetAndGetClassifier(self):
        coa = CreateObjectAction()
        clazz = Class()
        coa.setClassifier(clazz)
        self.assertEqual(coa.getClassifier(), clazz)

if __name__ == '__main__':
    unittest.main()