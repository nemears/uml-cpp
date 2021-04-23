#include "uml/parsers/parser.h"

using namespace std;

namespace UML {
namespace Parsers {

Element* parse(YAML::Node node) {
    if (node["class"]) {
        Class* clazz = new Class;
        ParserMetaData data;
        parseClass(node["class"], *clazz, data);
        return clazz;
    }

    if (node["opaqueBehavior"]) {
        OpaqueBehavior* bhv = new OpaqueBehavior;
        ParserMetaData data;
        parseOpaqueBehavior(node["opaqueBehavior"], *bhv, data);
        return bhv;
    }

    if (node["operation"]) {
        Operation* op = new Operation;
        ParserMetaData data;
        parseOperation(node["operation"], *op, data);
        return op;
    }

    if (node["package"]) {
        Package* pckg = new Package;
        ParserMetaData data;
        UML::Parsers::parsePackage(node["package"], *pckg, data);
        return pckg;
    }

    if (node["parameter"]) {
        Parameter* param = new Parameter;
        ParserMetaData data;
        parseParameter(node["parameter"], *param, data);
        return param;
    }

    if (node["property"]) {
        Property* prop = new Property;
        ParserMetaData data;
        parseProperty(node["property"], *prop, data);
        return prop;
    }

    return 0;
}

void parseElement(YAML::Node node, Element& el, ParserMetaData& data) {
    if (node["id"]) {
        if (node["id"].IsScalar()) {
            string id = node["id"].as<string>();
            if (isValidUUID4(id)) {
                el.setID(boost::lexical_cast<boost::uuids::uuid>(id));
            } else {
                throw UmlParserException("Value for id is not a valid UUID4, line " + node["id"].Mark().line);
            }
        } else {
            throw UmlParserException("Improper YAML node type for id field, must be scalar, line " + node["id"].Mark().line);
        }
    }

    // apply post processing here via functor
    data.elements.add(el);
}

void parseNamedElement(YAML::Node node, NamedElement& el, ParserMetaData& data) {

    parseElement(node, el, data);

    if (node["name"]) {
        if (node["name"].IsScalar()) {
            el.setName(node["name"].as<string>());
        } else {
            throw UmlParserException("Improper YAML node type for name field, must be scalar, line " + node["name"].Mark().line);
        }
    }
}

void SetTypeFunctor::operator()(Element& el) const {
    if (el.isSubClassOf(ElementType::TYPE)) {
        dynamic_cast<TypedElement*>(m_el)->setType(&dynamic_cast<Type&>(el));
    } else {
        throw UmlParserException(m_el->getElementTypeString() + " id: " + boost::lexical_cast<string>(m_el->getID()) + 
                                 " assigned type is not a typed Element! line " + to_string(m_node.Mark().line));
    }
}

void parseTypedElement(YAML::Node node, TypedElement& el, ParserMetaData& data) {

    parseNamedElement(node, el, data);

    if (node["type"]) {
        if (node["type"].IsScalar()) {
            string typeIDstring = node["type"].as<string>();
            if (isValidUUID4(typeIDstring)) {
                boost::uuids::uuid typeID = boost::lexical_cast<boost::uuids::uuid>(typeIDstring);
                applyFunctor(data, typeID, new SetTypeFunctor(&el, node["type"]));
            } else {
                throw UmlParserException("ID for " + el.getElementTypeString() + " type field is invalid, line " + to_string(node["type"].Mark().line));
            }
        } else {
            throw UmlParserException("Improper YAML node type for type field, must be scalar, line " + node["type"].Mark().line);
        }
    }
}

void parseNamespace(YAML::Node node, Namespace& nmspc, ParserMetaData& data) {
    parseNamedElement(node, nmspc, data);

    // probably don't need this function
}

void parseClassifier(YAML::Node node, Classifier& clazz, ParserMetaData& data) {
    parseNamespace(node, clazz, data);

    if (node["attributes"]) {
        if (node["attributes"].IsSequence()) {
            for (size_t i=0; i<node["attributes"].size(); i++) {
                if (node["attributes"][i]["property"]) {
                    if (node["attributes"][i]["property"].IsMap()) {
                        Property* prop = new Property;
                        parseProperty(node["attributes"][i]["property"], *prop, data);
                        clazz.getAttributes().add(*prop);
                    } else {
                        throw UmlParserException("Improper YAML node type for property field, must be map, line " + 
                                                 node["attributes"][i]["property"].Mark().line);
                    }
                }
            }
        } else {
            throw UmlParserException("Improper YAML node type for type field, must be scalar, line " + node["attributes"].Mark().line);
        }
    }
}

void parseClass(YAML::Node node, Class& clazz, ParserMetaData& data) {
    parseClassifier(node, clazz, data);

    if (node["operations"]) {
        if (node["operations"].IsSequence()) {
            for (size_t i=0; i<node["operations"].size(); i++) {
                if (node["operations"][i]["operation"]) {
                    Operation * op = new Operation;
                    parseOperation(node["operations"][i]["operation"], *op, data);
                    clazz.getOperations().add(*op);
                } else {
                    throw UmlParserException("Could not identify operation to parse, line " + to_string(node["operations"][i].Mark().line));
                }
            }
        } else {
            throw UmlParserException("Improper YAML node type for operations field, must be scalar, line " + node["attributes"].Mark().line);
        }
    }
}

void parseBehavior(YAML::Node node, Behavior& bhv, ParserMetaData& data) {
    parseClass(node, bhv, data);

    if (node["parameters"]) {
        if (node["parameters"].IsSequence()) {
            for (size_t i=0; i<node["parameters"].size(); i++) {
                if (node["parameters"][i].IsMap()) {
                    if (node["parameters"][i]["parameter"]) {
                        Parameter* param = new Parameter();
                        parseParameter(node["parameters"][i]["parameter"], *param, data);
                        bhv.getParameters().add(*param);
                    }
                } else {
                    // error? or is scalar allowed parameters shared(?)
                }
            }
        } else {
            throw UmlParserException("Improper YAML node type for parameters field, must be scalar, line " + node["parameters"].Mark().line);
        }
    }
}

void parseOpaqueBehavior(YAML::Node node, OpaqueBehavior& bhv, ParserMetaData& data) {
    parseBehavior(node, bhv, data);

    if (node["bodies"]) {
        if (node["bodies"].IsSequence()) {
            for (size_t i=0; i<node["bodies"].size(); i++) {
                if (node["bodies"][i].IsMap()) {
                    if (node["bodies"][i]["literalString"]) {
                        // TODO literal string parsers
                    } else {
                        throw UmlParserException("No literal string for body entry of a Map Node");
                    }
                } else if (node["bodies"][i].IsScalar()) {
                    string body = node["bodies"][i].as<string>();
                    if (isValidUUID4(body)) {
                        // TODO
                    } else {
                        // make literal string with value
                        LiteralString* bodyUML = new LiteralString;
                        bodyUML->setValue(body);
                        bhv.getBodies().add(*bodyUML);
                    }
                } else {
                    throw UmlParserException("Improper YAML node type for bodies sequence, must be scalar or map, line " + node["bodies"][i].Mark().line);
                }
            }
        } else {
            throw UmlParserException("Improper YAML node type for bodies, must be sequence, line " + node["bodies"].Mark().line);
        }
    }
}

void parseProperty(YAML::Node node, Property& prop, ParserMetaData& data) {
    parseTypedElement(node, prop, data);
    // TODO structural feature parser?
    parseMultiplicityElement(node, prop, data);

    if (node["defaultValue"]) {
        // TODO
    }
}

void parseParameter(YAML::Node node, Parameter& el, ParserMetaData& data) {
    parseTypedElement(node, el, data);
    parseMultiplicityElement(node, el, data);

    if (node["direction"]) {
        if (node["direction"].IsScalar()) {
            if (node["direction"].as<string>().compare("IN") == 0) {
                el.setDirection(ParameterDirectionKind::IN);
            } else if (node["direction"].as<string>().compare("INOUT") == 0) {
                el.setDirection(ParameterDirectionKind::INOUT);
            } else if (node["direction"].as<string>().compare("OUT") == 0) {
                el.setDirection(ParameterDirectionKind::OUT);
            } else if (node["direction"].as<string>().compare("RETURN") == 0) {
                el.setDirection(ParameterDirectionKind::RETURN);
            } else {
                throw UmlParserException("Invalid direction provided for Parameter, line" + node["direction"].Mark().line);
            }
        } else {
            throw UmlParserException("Improper YAML node type for direction, must be scalar, line " + node["direction"].Mark().line);
        }
    }
}

void parseOperation(YAML::Node node, Operation& op, ParserMetaData& data) {
    parseNamedElement(node, op, data);

    if (node["methods"]) {
        if (node["methods"].IsSequence()) {
            for (size_t i=0; i<node["methods"].size(); i++) {
                if (node["methods"][i]["opaqueBehavior"]) {
                    OpaqueBehavior* bhv = new OpaqueBehavior;
                    parseOpaqueBehavior(node["methods"][i]["opaqueBehavior"], *bhv, data);
                    op.getMethods().add(*bhv);
                } else {
                    throw UmlParserException("Invalid behavior type for operation methods, line " + node["methods"][i].Mark().line);
                } 
            }
        } else {
            throw UmlParserException("Improper YAML node type for methods, must be sequence, line " + node["methods"].Mark().line);
        }
    }
}

void parsePackageableElement(YAML::Node node, PackageableElement& el, ParserMetaData& data) {
    // if (node["owningPackage"]) {
    //     // TODO validate
    // }

    // probably don't need this function
}

void parsePackage(YAML::Node node, Package& pckg, ParserMetaData& data) {

    parseNamespace(node, pckg, data);

    parsePackageableElement(node, pckg, data);

    if (node["packagedElements"]) {
        if (node["packagedElements"].IsSequence()) {
            for (size_t i=0; i<node["packagedElements"].size(); i++) {
                if (node["packagedElements"][i].IsMap()) {
                    if (node["packagedElements"][i]["activity"]) {
                        Activity* activity = new Activity;
                        // TODO parse activity
                        activity->setOwningPackage(&pckg);
                    } else if (node["packagedElements"][i]["class"]) {
                        Class* clazz = new Class;
                        parseClass(node["packagedElements"][i]["class"], *clazz, data);
                        pckg.getPackagedElements().add(*clazz);
                    } else if (node["packagedElements"][i]["package"]) {
                        Package* package = new Package;
                        parsePackage(node["packagedElements"][i]["package"], *package, data);
                        package->setOwningPackage(&pckg);
                    } else {
                        throw UmlParserException("Invalid identifier for packagedElements, line" + node["packagedElements"][i].Mark().line);
                    }
                } else {
                    throw UmlParserException("Invalid YAML node type for field packagedElements sequence, must be map, line " 
                                             + node["packagedElements"][i].Mark().line);
                }
            }
        } else {
            throw UmlParserException("Invalid YAML node type for field packagedElements, must be sequence, line " + node["packagedElements"].Mark().line);
        }
    }
}

void parseMultiplicityElement(YAML::Node node, MultiplicityElement& el, ParserMetaData& data) {
    if (node["lower"]) {
        if (node["lower"].IsScalar()) {
            LiteralInt* lower = new LiteralInt;
            lower->setValue(node["lower"].as<int>());
            el.setLowerValue(lower);
        } else if (node["lower"].IsMap()) {
            if (node["lower"]["literalInt"]) {
                // TODO parse literal int
            } else if (node["lower"]["expression"]) {
                // TODO parse and evaluate expression
            }
        } else {
            throw UmlParserException("Invalid YAML node type for field lower, must be scalar or map, line " + node["lower"].Mark().line);
        }
    }

    if (node["upper"]) {
        if (node["upper"].IsScalar()) {
            LiteralInt* upper = new LiteralInt;
            upper->setValue(node["upper"].as<int>());
            el.setUpperValue(upper);
        } else if (node["upper"].IsMap()) {
            if (node["upper"]["literalInt"]) {
                // TODO parse literal int
            } else if (node["upper"]["expression"]) {
                // TODO parse and evaluate expression
            }
        } else {
            throw UmlParserException("Invalid YAML node type for field upper, must be scalar or map, line " + node["upper"].Mark().line);
        }
    }
}

}
}