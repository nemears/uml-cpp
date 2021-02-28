import unittest
from yuml_python import *

class ModelParserTest(unittest.TestCase):
    
    def testParseAndEmit(self):
        parser = ModelParser()
        parser.parse('src/test/yml/controlFlowTests/actionToSelf.yml')
        expectedEmit = """---
model:
  id: fa3e6863-9ca0-4a9e-a288-da3e99045f3f
  children:
    - activity:
        id: 48ab55ad-62d0-4282-b624-c46e0de94486
        nodes:
          - action:
              id: 2c2fc607-48c6-4879-800b-7e659441cba1
              incoming:
                - 9cdae5be-6b75-4284-b1e3-445fcb3dd071
              outgoing:
                - 9cdae5be-6b75-4284-b1e3-445fcb3dd071
        edges:
          - controlFlow:
              id: 9cdae5be-6b75-4284-b1e3-445fcb3dd071
              source: 2c2fc607-48c6-4879-800b-7e659441cba1
              target: 2c2fc607-48c6-4879-800b-7e659441cba1"""
        self.maxDiff = 1000000
        self.assertEqual(parser.emit(parser.getParsedElement()), expectedEmit)

if __name__ == '__main__':
    unittest.main()