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
#include "uml/action.h"
#include "uml/objectNode.h"
#include "uml/pin.h"
#include "uml/inputPin.h"
#include "uml/outputPin.h"
#include "uml/controlFlow.h"
#include "uml/objectFlow.h"
#include "uml/initialNode.h"
#include "uml/finalNode.h"
#include "uml/decisionNode.h"
#include "uml/joinNode.h"
#include "uml/forkNode.h"
#include "uml/mergeNode.h"
#include "uml/parameterNode.h"
#include "uml/callBehaviorAction.h"
#include "uml/createObjectAction.h"
#include "uml/expression.h"
#include "yuml-parsers/modelParser.h"

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
template <class PrimitiveTypeBase = PrimitiveType> class PrimitiveTypePy : public ClassifierPy<PrimitiveTypeBase> {
    public:
        using ClassifierPy<PrimitiveTypeBase>::ClassifierPy;
        bool isPrimitive() override {
                PYBIND11_OVERRIDE(
                bool,
                PrimitiveTypeBase,
                isPrimitive,

            );
        }
};

class ModelParserPy {
    protected:
        ModelParser modelParser;
    public:
        ModelParserPy () : modelParser(new map<boost::uuids::uuid, Element*>, new map<boost::uuids::uuid, PostParser*>) {};
        void parse(string f) {
            modelParser.parse(YAML::LoadFile(f));
        }
        Element* getParsedEl() {
            return modelParser.theEl;
        };
        string emit(Model* m) {
            return modelParser.emitDocument(m);
        }

};

namespace py = pybind11;

PYBIND11_MODULE(yuml_python, m) {

    m.def("isValidUUID4", isValidUUID4);

    // Element
    py::class_<Element, ElementPy<>>(m, "Element")
        .def(py::init<>())
        .def("getID", &Element::getIDstring)
        .def("setID", [](Element& me, string id) { me.setID(id); })
        //.def_readonly("ownedElements", [] (Element& me) { me.getOwnedElements().copyToVector(); })
        .def("addOwnedElement", [](Element& me, Element& el) { me.getOwnedElements().add(el); })
        .def("removeOwnedElement", [](Element& me, Element& el) { me.getOwnedElements().remove(el); })
        .def("getOwner", &Element::getOwner)
        .def("setOwner", &Element::setOwner);

    // InvalidID_Exception
    static py::exception<InvalidID_Exception>excID(m, "InvalidID_Exception");
    py::register_exception_translator([](std::exception_ptr p) {
        try {
            if (p) std::rethrow_exception(p);
        } catch (const InvalidID_Exception &e) {
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

    // Expression
    py::class_<Expression, ValueSpecification, ElementPy<Expression>>(m, "Expression")
        .def(py::init<>())
        .def("addOperand", [](Expression& me, ValueSpecification& operand) { me.operands.push_back(&operand); })
        .def("removeOperand", [](Expression& me, ValueSpecification& operand) {
            list<ValueSpecification*>::iterator i = me.operands.begin();
            while (i != me.operands.end()) {
                if ((*i)->getID() == operand.getID()) {
                    me.operands.erase(i);
                    break;
                }
                ++i;
            }
        })
        .def_readonly("operands", &Expression::operands)
        .def("getSymbol", &Expression::getSymbol)
        .def("setSymbol", &Expression::setSymbol);

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
                if ((*i)->getID() == val.getID()) {
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
                if ((*i)->getID() == slot.getID()) {
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
                if ((*i)->getID() == prop.getID()) {
                    me.ownedAttributes.erase(i);
                    break;
                }
                ++i;
            }
        })
        .def_readonly("attributes", &Classifier::ownedAttributes)
        .def("isPrimitive", &Classifier::isPrimitive);
    
    // PrimitiveType
    py::class_<PrimitiveType,  Classifier, PrimitiveTypePy<>> primitiveType (m, "PrimitiveType");
        primitiveType.def(py::init<>())
        .def("setPrimitiveType", &PrimitiveType::setPrimitiveTypeString)
        .def("getPrimitiveType", &PrimitiveType::getPrimitiveTypeString)
        .def("isPrimitive", &PrimitiveType::isPrimitive);

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
        .def("addParameter", [] (Operation& me, Parameter& param) { me.parameters.push_back(&param); })
        .def("removeParameter", [] (Operation& me, Parameter& param) {
            list<Parameter*>::iterator i = me.parameters.begin();
            while (i != me.parameters.end()) {
                if ((*i)->getID() == param.getID()) {
                    me.parameters.erase(i);
                    break;
                }
                ++i;
            }
        })
        .def_readonly("parameters", &Operation::parameters)
        .def("addMethod", [] (Operation& me, Behavior& method) { me.methods.push_back(&method); })
        .def("removeMethod", [] (Operation& me, Behavior& method) {
            list<Behavior*>::iterator i = me.methods.begin();
            while (i != me.methods.end()) {
                if ((*i)->getID() == method.getID()) {
                    me.methods.erase(i);
                    break;
                }
                ++i;
            }
        })
        .def_readonly("methods", &Operation::methods);

    // Class
    py::class_<Class, Classifier, ClassifierPy<Class>>(m, "Class")
        .def(py::init<>())
        .def("addOperation", [] (Class& me, Operation& op) { me.operations.push_back(&op); })
        .def("removeOperation", [] (Class& me, Operation& op) {
            list<Operation*>::iterator i = me.operations.begin();
            while (i != me.operations.end()) {
                if ((*i)->getID() == op.getID()) {
                    me.operations.erase(i);
                    break;
                }
                ++i;
            }
        })
        .def_readonly("operations", &Class::operations);
    
    // Behavior
    py::class_<Behavior, Class, ClassifierPy<Behavior>> (m, "Behavior")
        .def(py::init<>())
        .def_readonly("parameters", &Behavior::parameters)
        .def("addParameter", [] (Behavior& me, Parameter& param) { me.parameters.push_back(&param); })
        .def("removeParameter", [] (Behavior& me, Parameter& param) {
            list<Parameter*>::iterator i = me.parameters.begin();
            while (i != me.parameters.end()) {
                if ((*i)->getID() == param.getID()) {
                    me.parameters.erase(i);
                    break;
                }
                ++i;
            }
        });

    // Opaque Behavior
    py::class_<OpaqueBehavior, Behavior, ClassifierPy<OpaqueBehavior>>(m, "OpaqueBehavior")
        .def(py::init<>())
        .def("setBody", &OpaqueBehavior::setSingletonBody)
        .def("getBody", &OpaqueBehavior::getSingletonBody);

    // Activity
    py::class_<Activity, Behavior, ClassifierPy<Activity>>(m, "Activity")
        .def(py::init<>())
        .def("addNode", [] (Activity& me, ActivityNode& node) { me.nodes.push_back(&node); })
        .def("removeNode", [] (Activity& me, ActivityNode& node) {
            list<ActivityNode*>::iterator i = me.nodes.begin();
            while (i != me.nodes.end()) {
                if ((*i)->getID() == node.getID()) {
                    me.nodes.erase(i);
                    break;
                }
                ++i;
            }
        })
        .def_readwrite("nodes", &Activity::nodes)
        .def("addEdge", [] (Activity& me, ActivityEdge& edge) { me.edges.push_back(&edge); })
        .def("removeEdge", [] (Activity&me, ActivityEdge& edge) {
            list<ActivityEdge*>::iterator i = me.edges.begin();
            while (i != me.edges.end()) {
                if ((*i)->getID() == edge.getID()) {
                    me.edges.erase(i);
                    break;
                }
                ++i;
            }
        })
        .def_readonly("edges", &Activity::edges);
    
    // ActivityEdge
    py::class_<ActivityEdge, NamedElement, ElementPy<ActivityEdge>> (m, "ActivityEdge")
        .def(py::init<>())
        .def("getSource", &ActivityEdge::getSource)
        .def("setSource", &ActivityEdge::setSource)
        .def("getTarget", &ActivityEdge::getTarget)
        .def("setTarget", &ActivityEdge::setTarget)
        .def("getGuard", &ActivityEdge::getGuard)
        .def("setGuard", &ActivityEdge::setGuard);

    // ActivityNode
    py::class_<ActivityNode, NamedElement, ElementPy<ActivityNode>>(m, "ActivityNode")
        .def(py::init<>())
        .def_readonly("incoming", &ActivityNode::incoming)
        .def_readonly("outgoing", &ActivityNode::outgoing)
        .def("addIncoming", [] (ActivityNode& me, ActivityEdge& edge) { me.incoming.push_back(&edge); })
        .def("addOutgoing", [] (ActivityNode& me, ActivityEdge& edge) { me.outgoing.push_back(&edge); })
        .def("removeIncoming", [] (ActivityNode& me, ActivityEdge& edge) {
            list<ActivityEdge*>::iterator i = me.incoming.begin();
            while (i != me.incoming.end()) {
                if ((*i)->getID() == edge.getID()) {
                    me.incoming.erase(i);
                    break;
                }
                ++i;
            }
        })
        .def("removeOutgoing", [] (ActivityNode& me, ActivityEdge& edge) {
            list<ActivityEdge*>::iterator i = me.outgoing.begin();
            while (i != me.outgoing.end()) {
                if ((*i)->getID() == edge.getID()) {
                    me.outgoing.erase(i);
                    break;
                }
                ++i;
            }
        })
        .def("getActivity", &ActivityNode::getActivity)
        .def("setActivity", &ActivityNode::setActivity);

    // ObjectNode
    py::class_<ObjectNode, TypedElement, ActivityNode, ElementPy<ObjectNode>>(m, "ObjectNode")
        .def(py::init<>())
        .def("getUpperBound", &ObjectNode::getUpperBound)
        .def("setUpperBound", &ObjectNode::setUpperBound);

    // Pin
    py::class_<Pin, ObjectNode, MultiplicityElement, ElementPy<Pin>>(m, "Pin")
        .def(py::init<>());

    // InputPin
    py::class_<InputPin, Pin, ElementPy<InputPin>>(m, "InputPin")
        .def(py::init<>());

    // OutputPin
    py::class_<OutputPin, Pin, ElementPy<OutputPin>>(m, "OutputPin")
        .def(py::init<>());
    
    // Action
    py::class_<Action, ActivityNode, ElementPy<Action>>(m, "Action")
        .def(py::init<>())
        .def_readonly("inputs", &Action::inputs)
        .def_readonly("outputs", &Action::outputs)
        .def("addInput", [] (Action& me, InputPin& in) { me.inputs.push_back(&in); } )
        .def("removeInput", [] (Action& me, InputPin& in) {
            list<InputPin*>::iterator i = me.inputs.begin();
            while (i != me.inputs.end()) {
                if ((*i)->getID() == in.getID()) {
                    me.inputs.erase(i);
                    break;
                }
                ++i;
            }
        })
        .def("addOutput", [] (Action& me, OutputPin& out) { me.outputs.push_back(&out); })
        .def("removeOutput", [] (Action& me, OutputPin& out) {
            list<OutputPin*>::iterator i = me.outputs.begin();
            while (i != me.outputs.end()) {
                if ((*i)->getID() == out.getID()) {
                    me.outputs.erase(i);
                    break;
                }
                ++i;
            }
        });

        // ControlFlow
        py::class_<ControlFlow, ActivityEdge, ElementPy<ControlFlow>>(m, "ControlFlow")
            .def(py::init<>());

        // ObjectFlow
        py::class_<ObjectFlow, ActivityEdge, ElementPy<ObjectFlow>>(m, "ObjectFlow")
            .def(py::init<>());

        // InitialNode
        py::class_<InitialNode, ActivityNode, ElementPy<InitialNode>>(m, "InitialNode")
            .def(py::init<>());
        
        // FinalNode
        py::class_<FinalNode, ActivityNode, ElementPy<FinalNode>>(m, "FinalNode")
            .def(py::init<>());

        // DecisionNode
        py::class_<DecisionNode, ActivityNode, ElementPy<DecisionNode>>(m, "DecisionNode")
            .def(py::init<>())
            .def("getDecisionInputFlow", &DecisionNode::getDecisionInputFlow)
            .def("setDecisionInputFlow", &DecisionNode::setDecisionInputFlow);

        // JoinNode
        py::class_<JoinNode, ActivityNode, ElementPy<JoinNode>>(m, "JoinNode")
            .def(py::init<>());

        // ForkNode
        py::class_<ForkNode, ActivityNode, ElementPy<ForkNode>>(m, "ForkNode")
            .def(py::init<>());

        // MergeNode
        py::class_<MergeNode, ActivityNode, ElementPy<MergeNode>>(m, "MergeNode")
            .def(py::init<>());

        // ParameterNode
        py::class_<ParameterNode, ObjectNode, ElementPy<ParameterNode>>(m, "ParameterNode")
            .def(py::init<>())
            .def("getParameter", &ParameterNode::getParameter)
            .def("setParameter", &ParameterNode::setParameter);

        // CallBehaviorAction
        py::class_<CallBehaviorAction, Action, ElementPy<CallBehaviorAction>>(m, "CallBehaviorAction")
            .def(py::init<>())
            .def("getBehavior", &CallBehaviorAction::getBehavior)
            .def("setBehavior", &CallBehaviorAction::setBehavior);

        // CreateObjectAction
        py::class_<CreateObjectAction, Action, ElementPy<CreateObjectAction>>(m, "CreateObjectAction")
            .def(py::init<>())
            .def("getClassifier", &CreateObjectAction::getClassifier)
            .def("setClassifier", &CreateObjectAction::setClassifier);

        // ModelParser
        py::class_<ModelParserPy>(m, "ModelParser")
            .def(py::init<>())
            .def("parse", &ModelParserPy::parse)
            .def("emit", &ModelParserPy::emit)
            .def("getParsedElement", &ModelParserPy::getParsedEl);
}