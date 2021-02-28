from shutil import copyfile
import pathlib

if __name__ == '__main__':

    #path of this file should be src/python
    copyfile(str(pathlib.Path(__file__).parent.absolute()) +  "/../../build/src/cpp/yuml-python/yuml_python.cpython-38-x86_64-linux-gnu.so", str(pathlib.Path(__file__).parent.absolute()) + "/python_parsers/yuml_python.cpython-38-x86_64-linux-gnu.so")
    copyfile(str(pathlib.Path(__file__).parent.absolute()) +  "/../../build/src/cpp/yuml-python/yuml_python.cpython-38-x86_64-linux-gnu.so", str(pathlib.Path(__file__).parent.absolute()) + "/../../yuml_python.cpython-38-x86_64-linux-gnu.so")
    copyfile(str(pathlib.Path(__file__).parent.absolute()) +  "/../../build/src/cpp/yuml-python/yuml_python.cpython-38-x86_64-linux-gnu.so", str(pathlib.Path(__file__).parent.absolute()) + "/tests/python_parsers/yuml_python.cpython-38-x86_64-linux-gnu.so")
    copyfile(str(pathlib.Path(__file__).parent.absolute()) +  "/../../build/src/cpp/yuml-python/yuml_python.cpython-38-x86_64-linux-gnu.so", str(pathlib.Path(__file__).parent.absolute()) + "/tests/uml/yuml_python.cpython-38-x86_64-linux-gnu.so")
    copyfile(str(pathlib.Path(__file__).parent.absolute()) +  "/../../build/src/cpp/yuml-python/yuml_python.cpython-38-x86_64-linux-gnu.so", str(pathlib.Path(__file__).parent.absolute()) + "/tests/yuml_parsers/yuml_python.cpython-38-x86_64-linux-gnu.so")