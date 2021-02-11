#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "uml/element.h"
#include "uml/namedElement.h"
#include "uml/type.h"
#include "uml/typedElement.h"
#include "uml/classifier.h"
#include "uml/class.h"
#include "uml/operation.h"
#include "uml/opaqueBehavior.h"
#include "uml/instanceSpecification.h"
#include "uml/instanceValue.h"
#include "uml/primitiveType.h"
#include "uml/literalBool.h"
#include "uml/literalInt.h"
#include "uml/literalReal.h"
#include "uml/literalString.h"
#include "uml/model.h"

using namespace UML;

template <class TypeBase = Type> class TypePy : public TypeBase {
    public:
        using TypeBase::TypeBase;
        bool isPrimitive() override {
                PYBIND11_OVERRIDE_PURE(
                bool,
                TypeBase,
                isPrimitive,
            );
        }
        string getName() override {
            PYBIND11_OVERRIDE(
                string,
                TypeBase,
                getName,
            );
        }
        void setName(const string& name) override {
            PYBIND11_OVERRIDE(
                void,
                TypeBase,
                setName,
                name
            );
        }
        virtual string getIDstring() override {
            PYBIND11_OVERRIDE(
                string,
                TypeBase,
                getIDstring,
            );
        }
        virtual void setID(string id) override {
            PYBIND11_OVERRIDE(
                void,
                TypeBase,
                setID,
                id
            );
        }
};

template <class ClassifierBase = Classifier> class ClassifierPy : public TypePy<ClassifierBase> {
    public:
        using TypePy<ClassifierBase>::TypePy;
        bool isPrimitive() override {
                PYBIND11_OVERRIDE(
                bool,
                ClassifierBase,
                isPrimitive,
            );
        }
        string getName() override {
            PYBIND11_OVERRIDE(
                string,
                ClassifierBase,
                getName,
            );
        }
        void setName(const string& name) override {
            PYBIND11_OVERRIDE(
                void,
                ClassifierBase,
                setName,
                name
            );
        }
        virtual string getIDstring() override {
            PYBIND11_OVERRIDE(
                string,
                ClassifierBase,
                getIDstring,
            );
        }
        virtual void setID(string id) override {
            PYBIND11_OVERRIDE(
                void,
                ClassifierBase,
                setID,
                id
            );
        }
};

template <class PrimitiveTypeBase = PrimitiveType> class PrimitiveTypePy : public TypePy<PrimitiveTypeBase> {
    public:
        using TypePy<PrimitiveTypeBase>::TypePy;
        bool isPrimitive() override {
                PYBIND11_OVERRIDE(
                bool,
                PrimitiveTypeBase,
                isPrimitive,
            );
        }
};

namespace py = pybind11;

PYBIND11_MODULE(yuml_python, m) {

    m.def("isValidUUID4", isValidUUID4);

    // InvalidID_Exception
    static py::exception<Element::InvalidID_Exception>excID(m, "InvalidID_Exception");
    py::register_exception_translator([](std::exception_ptr p) {
        try {
            if (p) std::rethrow_exception(p);
        } catch (const Element::InvalidID_Exception &e) {
            excID(e.what());
        } catch (const py::error_already_set &e) {
            PyErr_SetString(PyExc_RuntimeError, e.what());
        }
    });

    //InvalidValueException
    static py::exception<StructuralFeature::InvalidValueException>excVal(m, "InvalidValueException");
    py::register_exception_translator([](std::exception_ptr p) {
        try {
            if (p) std::rethrow_exception(p);
        } catch (const StructuralFeature::InvalidValueException &e) {
            excVal(e.what());
        } catch (const py::error_already_set &e) {
            PyErr_SetString(PyExc_RuntimeError, e.what());
        }
    });

    // InvalidDirectionException
    static py::exception<Parameter::InvalidDirectionException>excDir(m, "InvalidDirectionException");
    py::register_exception_translator([](std::exception_ptr p) {
        try {
            if (p) std::rethrow_exception(p);
        } catch (const Parameter::InvalidDirectionException &e) {
            excDir(e.what());
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

    // Namespace
    py::class_<Namespace, NamedElement>(m, "Namespace")
        .def(py::init<>());

    // Model
    py::class_<Model, Namespace>(m, "Model")
        .def(py::init<>());

    // Type TODO fix isPrimitive bind
    py::class_<Type, TypePy<>> type(m, "Type");
    type.def(py::init<>())
        .def("isPrimitve", &Type::isPrimitive); // this funcion is not registering

    // PrimitiveType
    py::class_<PrimitiveType, Type, PrimitiveTypePy<>> pType(m, "PrimitiveType");
    pType.def(py::init<>())
        .def("setPrimitiveType", &PrimitiveType::setPrimitiveTypeString)
        .def("getPrimitiveType", &PrimitiveType::getPrimitiveTypeString);

    // TypedElement
    py::class_<TypedElement, NamedElement>(m, "TypedElement")
        .def(py::init<>())
        .def("getType", &TypedElement::getType, py::return_value_policy::reference)
        .def("setType", &TypedElement::setType);

    // ValueSpecification
    py::class_<ValueSpecification, TypedElement>(m, "ValueSpecification")
        .def(py::init<>());

    // InstanceValue
    py::class_<InstanceValue, ValueSpecification>(m, "InstanceValue")
        .def(py::init<>())
        .def("getInstance", &InstanceValue::getInstance)
        .def("setInstance", &InstanceValue::setInstance);

    // LiteralBool
    py::class_<LiteralBool, ValueSpecification>(m, "LiteralBool")
        .def(py::init<>())
        .def("setValue", &LiteralBool::setValue)
        .def("getValue", &LiteralBool::getValue);

    // LiteralInt
    py::class_<LiteralInt, ValueSpecification>(m, "LiteralInt")
        .def(py::init<>())
        .def("setValue", &LiteralInt::setValue)
        .def("getValue", &LiteralInt::getValue);

    // LiteralReal
    py::class_<LiteralReal, ValueSpecification>(m, "LiteralReal")
        .def(py::init<>())
        .def("setValue", &LiteralReal::setValue)
        .def("getValue", &LiteralReal::getValue);

    // LiteralString
    py::class_<LiteralString, ValueSpecification>(m, "LiteralString")
        .def(py::init<>())
        .def("setValue", &LiteralString::setValue)
        .def("getValue", &LiteralString::getValue);

    // StructuralFeature
    py::class_<StructuralFeature, TypedElement>(m, "StructuralFeature")
        .def(py::init<>());

    // Slot
    py::class_<Slot, Element>(m, "Slot")
        .def(py::init<>())
        .def("getDefiningFeature", &Slot::getDefiningFeature)
        .def("setDefiningFeature", &Slot::setDefiningFeature);

    // InstanceSpecification
    py::class_<InstanceSpecification, NamedElement>(m, "InstanceSpecification")
        .def(py::init<>())
        .def("setClassifier", &InstanceSpecification::setClassifier)
        .def("getClassifier", &InstanceSpecification::getClassifier)
        .def("addSlot", &InstanceSpecification::addSlot)
        .def("removeSlot", &InstanceSpecification::removeSlot)
        .def_readonly("slots", &InstanceSpecification::slots);
    
    // Property
    py::class_<Property, StructuralFeature>(m, "Property")
        .def(py::init<>())
        .def("setDefaultValue", &Property::setDefaultValue)
        .def("getDefaultValue", &Property::getDefaultValue);

    // Classifier
    py::class_<Classifier, Type, ClassifierPy<>> classifier (m, "Classifier");
    classifier.def(py::init<>())
        .def("addAttribute", &Classifier::addAttribute)
        .def("removeAttribute", &Classifier::removeAttribute)
        .def_readonly("attributes", &Classifier::ownedAttributes);

    // Parameter
    py::class_<Parameter, TypedElement>(m, "Parameter")
        .def(py::init<>())
        .def("getDirection", &Parameter::getDirectionString)
        .def("setDirection", &Parameter::setDirectionString);
    
    // Operation
    py::class_<Operation, NamedElement>(m, "Operation")
        .def(py::init<>())
        .def("getType", &Operation::getType)
        .def("setType", &Operation::setType)
        .def("addParameter", &Operation::addParameter)
        .def("removeParameter", &Operation::removeParameter)
        .def_readonly("parameters", &Operation::parameters)
        .def("addMethod", &Operation::addMethod)
        .def("removeMethod", &Operation::removeMethod)
        .def_readonly("methods", &Operation::methods);

    // Class
    py::class_<Class, Classifier, ClassifierPy<Class>>(m, "Class")
        .def(py::init<>())
        .def("addOperation", &Class::addOperation)
        .def("removeOperation", &Class::removeOperation)
        .def_readonly("operations", &Class::operations);
    
    // Behavior
    py::class_<Behavior, Class> (m, "Behavior")
        .def(py::init<>());

    // Opaque Behavior
    py::class_<OpaqueBehavior, Behavior>(m, "OpaqueBehavior")
        .def(py::init<>())
        .def("setBody", &OpaqueBehavior::setSingletonBody)
        .def("getBody", &OpaqueBehavior::getSingletonBody);
}