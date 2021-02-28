import unittest
from yuml_python import *

class ActionTest(unittest.TestCase):

    def testAddAndRemoveInput(self):
        a = Action()
        i = InputPin()
        a.addInput(i)
        self.assertEqual(a.inputs[0].getID(), i.getID())
        a.removeInput(i)
        self.assertEqual(len(a.inputs), 0)
    
    def testAddAndRemoveOutput(self):
        a = Action()
        i = OutputPin()
        a.addOutput(i)
        self.assertEqual(a.outputs[0].getID(), i.getID())
        a.removeOutput(i)
        self.assertEqual(len(a.outputs), 0)

if __name__ == '__main__':
    unittest.main()