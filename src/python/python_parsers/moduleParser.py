import ast
from yuml_python import *
from .classParser import parseClass
from .defParser import parseFunction

# This needs a dict to keep c++ memory not garbage collected for now
# There may be a solution on the pybind11 side with these issues if they implement fixes
# https://github.com/pybind/pybind11/issues/1333
# https://github.com/pybind/pybind11/pull/2839
def parseModule(path, d):
    with open(path) as f:
        moduleNode = ast.parse(f.read())
        moduleUML = Namespace()
        d[moduleUML.getID()] = moduleUML
        for node in moduleNode.body:
            if type(node) is ast.ClassDef:
                c = parseClass(node, d)
                moduleUML.addOwnedElement(c)
                c.setOwner(moduleUML)
                d[c.getID()] = c
            elif type(node) is ast.FunctionDef:
                bhv = parseFunction(node, d, moduleUML)
                moduleUML.addOwnedElement(bhv)
                bhv.setOwner(moduleUML)
            #if type(n) is ast.Import:
        return moduleUML

# if __name__ == '__main__':
#     d = {}
#     n = parseModule('/home/stinky/Projects/yuml_projects/yuml/src/test/python/examples/knapsack.py', d)
#     print('length of module children: ', len(n.ownedElements))
#     m = Model()
#     m.setName('root')
#     m.addOwnedElement(n)
#     emitter = ModelParser()
#     print(emitter.emit(m))