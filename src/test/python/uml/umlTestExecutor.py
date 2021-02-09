import unittest
import classifier_test
import element_test
import namedElement_test
import type_test
import typedElement_test

def suite():
    suite = unittest.TestSuite()
    c = classifier_test.ClassifierTest()
    suite.addTest(c)
    return suite

if __name__ == '__main__':
    runner = unittest.TextTestRunner()
    runner.run(suite())