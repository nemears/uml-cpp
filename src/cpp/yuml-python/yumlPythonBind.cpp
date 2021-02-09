#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "uml/element.h"
#include "uml/namedElement.h"
#include "uml/type.h"
#include "uml/typedElement.h"
#include "uml/classifier.h"
#include "uml/class.h"

using namespace UML;

// class TypePy : public Type {
//     public:
//         using Type::Type;
//         bool isPrimitive() override {
//             PYBIND11_OVERRIDE_PURE(
//             bool,
//             Type,
//             isPrimitive);
//         }
// };

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
    
    // NamedElement
    py::class_<NamedElement, Element>(m, "NamedElement")
        .def(py::init<>())
        .def("setName", &NamedElement::setName)
        .def("getName", &NamedElement::getName);

    // Type TODO fix isPrimitive bind
    py::class_<Type, NamedElement>(m, "Type")
        .def(py::init<>())
        .def("isPrimitve", &Type::isPrimitive); // this funcion is now registering

    // TypedElement
    py::class_<TypedElement, NamedElement>(m, "TypedElement")
        .def(py::init<>())
        .def("getType", &TypedElement::getType, py::return_value_policy::reference)
        .def("setType", &TypedElement::setType);

    // StructuralFeature
    py::class_<StructuralFeature, TypedElement>(m, "StructuralFeature")
        .def(py::init<>());
    
    // Property
    py::class_<Property, StructuralFeature>(m, "Property")
        .def(py::init<>());

    // Classifier
    py::class_<Classifier, Type>(m, "Classifier")
        .def(py::init<>())
        .def("addAttribute", &Classifier::addAttribute)
        .def("removeAttribute", &Classifier::removeAttribute)
        .def_readonly("attributes", &Classifier::ownedAttributes);

    // Class
    py::class_<Class, Classifier>(m, "Class")
        .def(py::init<>());
    
}