import unittest
from yuml_python import *

class NamespaceTest(unittest.TestCase):
    
    def testNamespaceGeneral(self):
        n = Namespace()
        n.setName('name')
        self.assertEqual(n.getName(), 'name')

if __name__ == '__main__':
    unittest.main()