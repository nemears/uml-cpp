import unittest
from yuml_python import *

class ModelTest(unittest.TestCase):

    def testModel(self):
        m = Model()
        m.setName("model")
        self.assertEqual(m.getName(), 'model')

if __name__ == '__main__':
    unittest.main()