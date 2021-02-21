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
#include "uml/activity.h"

using namespace UML;

// Element Trampoline
template <class ElementBase = Element> class ElementPy: public ElementBase {
    public:
        using ElementBase::ElementBase;
};

// Type Trampoline
template <class TypeBase = Type> class TypePy : public ElementPy<TypeBase> {
    public:
        using ElementPy<TypeBase>::ElementPy;
        bool isPrimitive() override {
                PYBIND11_OVERRIDE_PURE(
                bool,
                TypeBase,
                isPrimitive,

            );
        }
};

// Classifier Trampoline
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
};

// PrimitiveType Trampoline
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

class ActivityPy : public Activity {
    public:
        void addNode(ActivityNode& node) {
            nodes.push_back(&node);
        }
        void removeNode(ActivityNode& node) {
            list<ActivityNode*>::iterator i = nodes.begin();
            while (i != nodes.end()) {
                if ((*i)->uuid == node.uuid) {
                    nodes.erase(i);
                    break;
                }
                ++i;
            }
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
    py::class_<Element, ElementPy<>>(m, "Element")
        .def(py::init<>())
        .def("getID", &Element::getIDstring)
        .def("setID", &Element::setID)
        .def_readonly("ownedElements", &Element::ownedElements)
        .def("addOwnedElement", [](Element& me, Element& el) { me.ownedElements.push_back(&el); })
        .def("removeOwnedElement", [](Element& me, Element& el) {
            list<Element*>::iterator i = me.ownedElements.begin();
            while (i != me.ownedElements.end()) {
                if ((*i)->uuid == el.uuid) {
                    me.ownedElements.erase(i);
                    break;
                }
                ++i;
            }
        });
    
    // NamedElement
    py::class_<NamedElement, Element, ElementPy<NamedElement>>(m, "NamedElement")
        .def(py::init<>())
        .def("setName", &NamedElement::setName)
        .def("getName", &NamedElement::getName);

    // Namespace
    py::class_<Namespace, NamedElement, ElementPy<Namespace>>(m, "Namespace")
        .def(py::init<>());

    // Model
    py::class_<Model, Namespace, ElementPy<Model>>(m, "Model")
        .def(py::init<>());

    // Type TODO fix isPrimitive bind
    py::class_<Type, NamedElement, TypePy<>>(m, "Type")
        .def(py::init<>())
        .def("isPrimitve", &Type::isPrimitive); // this funcion is not registering

    // PrimitiveType
    py::class_<PrimitiveType, Type, PrimitiveTypePy<>> (m, "PrimitiveType")
        .def(py::init<>())
        .def("setPrimitiveType", &PrimitiveType::setPrimitiveTypeString)
        .def("getPrimitiveType", &PrimitiveType::getPrimitiveTypeString)
        .def("isPrimitive", &PrimitiveType::isPrimitive);

    // TypedElement
    py::class_<TypedElement, NamedElement, ElementPy<TypedElement>>(m, "TypedElement")
        .def(py::init<>())
        .def("getType", &TypedElement::getType, py::return_value_policy::reference)
        .def("setType", &TypedElement::setType);

    // ValueSpecification
    py::class_<ValueSpecification, TypedElement, ElementPy<ValueSpecification>>(m, "ValueSpecification")
        .def(py::init<>());

    // InstanceValue
    py::class_<InstanceValue, ValueSpecification, ElementPy<InstanceValue>>(m, "InstanceValue")
        .def(py::init<>())
        .def("getInstance", &InstanceValue::getInstance)
        .def("setInstance", &InstanceValue::setInstance);

    // LiteralBool
    py::class_<LiteralBool, ValueSpecification, ElementPy<LiteralBool>>(m, "LiteralBool")
        .def(py::init<>())
        .def("setValue", &LiteralBool::setValue)
        .def("getValue", &LiteralBool::getValue);

    // LiteralInt
    py::class_<LiteralInt, ValueSpecification, ElementPy<LiteralInt>>(m, "LiteralInt")
        .def(py::init<>())
        .def("setValue", &LiteralInt::setValue)
        .def("getValue", &LiteralInt::getValue);

    // LiteralReal
    py::class_<LiteralReal, ValueSpecification, ElementPy<LiteralReal>>(m, "LiteralReal")
        .def(py::init<>())
        .def("setValue", &LiteralReal::setValue)
        .def("getValue", &LiteralReal::getValue);

    // LiteralString
    py::class_<LiteralString, ValueSpecification, ElementPy<LiteralString>>(m, "LiteralString")
        .def(py::init<>())
        .def("setValue", &LiteralString::setValue)
        .def("getValue", &LiteralString::getValue);

    // StructuralFeature
    py::class_<StructuralFeature, TypedElement, ElementPy<StructuralFeature>>(m, "StructuralFeature")
        .def(py::init<>());

    // Slot
    py::class_<Slot, Element, ElementPy<Slot>>(m, "Slot")
        .def(py::init<>())
        .def("getDefiningFeature", &Slot::getDefiningFeature)
        .def("setDefiningFeature", &Slot::setDefiningFeature)
        .def_readonly("values", &Slot::values)
        .def("addValue", [](Slot& me, ValueSpecification& val) { me.values.push_back(&val); })
        .def("removeValue", [](Slot& me, ValueSpecification& val) {
            list<ValueSpecification*>::iterator i = me.values.begin();
            while (i != me.values.end()) {
                if ((*i)->uuid == val.uuid) {
                    me.values.erase(i);
                    break;
                }
                ++i;
            }
        });

    // InstanceSpecification
    py::class_<InstanceSpecification, NamedElement, ElementPy<InstanceSpecification>>(m, "InstanceSpecification")
        .def(py::init<>())
        .def("setClassifier", &InstanceSpecification::setClassifier)
        .def("getClassifier", &InstanceSpecification::getClassifier)
        .def("addSlot", [](InstanceSpecification& me, Slot& slot) { me.slots.push_back(&slot); })
        .def("removeSlot", [](InstanceSpecification& me, Slot& slot) {
            list<Slot*>::iterator i = me.slots.begin();
            while (i != me.slots.end()) {
                if ((*i)->uuid == slot.uuid) {
                    me.slots.erase(i);
                    break;
                }
                ++i;
            }
         } )
        .def_readonly("slots", &InstanceSpecification::slots);
    
    // MultiplicityElement
    py::class_<MultiplicityElement, Element, ElementPy<MultiplicityElement>>(m, "MultiplicityElement")
        .def(py::init<>())
        .def("getLower", &MultiplicityElement::getLower)
        .def("setLower", &MultiplicityElement::setLower)
        .def("getUpper", &MultiplicityElement::getUpper)
        .def("setUpper", &MultiplicityElement::setUpper);

    // Property
    py::class_<Property, StructuralFeature, MultiplicityElement, ElementPy<Property>>(m, "Property")
        .def(py::init<>())
        .def("setDefaultValue", &Property::setDefaultValue)
        .def("getDefaultValue", &Property::getDefaultValue);

    // Classifier
    py::class_<Classifier, Type, ClassifierPy<>> classifier (m, "Classifier");
    classifier.def(py::init<>())
        .def("addAttribute", [] (Classifier& me, Property& prop) { me.ownedAttributes.push_back(&prop); })
        .def("removeAttribute", [] (Classifier& me, Property& prop) {
            list<Property*>::iterator i = me.ownedAttributes.begin();
            while (i != me.ownedAttributes.end()) {
                if ((*i)->uuid == prop.uuid) {
                    me.ownedAttributes.erase(i);
                    break;
                }
                ++i;
            }
        })
        .def_readonly("attributes", &Classifier::ownedAttributes)
        .def("isPrimitive", &Classifier::isPrimitive);

    // Parameter
    py::class_<Parameter, TypedElement, MultiplicityElement, ElementPy<Parameter>>(m, "Parameter")
        .def(py::init<>())
        .def("getDirection", &Parameter::getDirectionString)
        .def("setDirection", &Parameter::setDirectionString);
    
    // Operation
    py::class_<Operation, NamedElement, ElementPy<Operation>>(m, "Operation")
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
    py::class_<Behavior, Class, ClassifierPy<Behavior>> (m, "Behavior")
        .def(py::init<>());

    // Opaque Behavior
    py::class_<OpaqueBehavior, Behavior, ClassifierPy<OpaqueBehavior>>(m, "OpaqueBehavior")
        .def(py::init<>())
        .def("setBody", &OpaqueBehavior::setSingletonBody)
        .def("getBody", &OpaqueBehavior::getSingletonBody);

    // Activity
    py::class_<ActivityPy, Behavior, ClassifierPy<ActivityPy>>(m, "Activity")
        .def(py::init<>())
        .def("addNode", &ActivityPy::addNode)
        .def("removeNode", &ActivityPy::removeNode)
        .def_readonly("nodes", &ActivityPy::nodes);
    
    // ActivityNode
    py::class_<ActivityNode, NamedElement, ElementPy<ActivityNode>>(m, "ActivityNode")
        .def(py::init<>());
}