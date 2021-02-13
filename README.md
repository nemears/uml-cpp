# yuml
Lightweight UML based MBSE application built on yaml syntax UML config files    
currently two directories hold two different code bases:    
  * yuml-parsers: parse yaml config files for uml representation    
  * uml: cpp uml api


## Currently Implemented

### yaml parsing
Currently Implemented    
  * Element    
    * id and child    
  * NamedElement    
    * name    
  * Classifier    
    * attributes list    
  * Property    
    * type (started default values)    
  * Type    
  * Class  
    * operations  
  * PrimitveType (not parsed)    
    * created enum    
  * ValueSpecification (not parsed)    
  * Namespace    
  * Model    
  * Literals    
  * Operations    
  * OpaqueBehaviors


## TODO
There is a lot to do right now this is just the next couple items I plan on looking into:  
  * longterm:    
    * Jenkins integration    
    * front-end implementation (look into options, ask for opinions)    
      * possibilities : Qt, react js, python libs(short term)    
    * cpp parsers    
      * [use clang](https://shaharmike.com/cpp/libclang/?fbclid=IwAR1Y9PBig4Hd6bxmNERySpGAk2V09iCThrWuZ3Vb31LFAMG33pa1_kGVQZo), [1](https://clang.llvm.org/get_started.html)    
    * python parsers  
      * [use pybind11](https://realpython.com/python-bindings-overview/) to port our uml code to python and [use ast to parse python](https://docs.python.org/3/library/ast.html#module-ast) with it  
      * this will help in making a [front end](https://wiki.python.org/moin/WebFrameworks) quickly with some ptyhon library for MVP demo
    * further definition of uml    
  * short-term:    
    * test test test    
    