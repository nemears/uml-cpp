# yuml
Lightweight UML based MBSE application built on yaml syntax UML files

## Currently Implemented

### yaml parsing
Currently Implemented
    -Element
        -id and child
    -NamedElement
        -name
    -Classifier
        -attributes list
    -Property
        -type (started default values)
    -Type
    -Class
    -PrimitveType (not parsed)
        -created enum
    -ValueSpecification (not parsed)
    -Namespace
    -Model

### emf uml2 implementation
Have some representation of Element, NamedElement, Class, Namespace and Model Value

## TODO
There is a lot to do right now this is just the next couple items I plan on looking into:
    longterm:
        Jenkins integration
        yaml-cpp parser (can also have json source files and automatic emit)
        submodule setup for gtest and eventually yaml-cpp
    short-term:
        value implementation
    