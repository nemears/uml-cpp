#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "uml/element.h"
#include "uml/namedElement.h"

using namespace UML;

namespace py = pybind11;

PYBIND11_MODULE(yuml_python, m) {

    m.def("isValidUUID4", isValidUUID4);

    // InvalidID_Exception
    static py::exception<Element::InvalidID_Exception>exc(m, "InvalidID_Exception");
    py::register_exception_translator([](std::exception_ptr p) {
        try {
            if (p) std::rethrow_exception(p);
        } catch (const Element::InvalidID_Exception &e) {
            exc(e.what());
        } catch (const py::error_already_set &e) {
            PyErr_SetString(PyExc_RuntimeError, e.what());
        }
    });

    // Element
    py::class_<Element>(m, "Element")
        .def(py::init<>())
        .def("getID", &Element::getIDstring)
        .def("setID", &Element::setID)
        .def_readonly("ownedElements", &Element::ownedElements)
        .def("addOwnedElement", &Element::addOwnedElement)
        .def("removeOwnedElement", &Element::removeOwnedElement);
    
    py::class_<NamedElement, Element>(m, "NamedElement")
        .def(py::init<>())
        .def("setName", &NamedElement::setName)
        .def("getName", &NamedElement::getName);
}