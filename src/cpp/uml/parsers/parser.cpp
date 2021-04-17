#include "uml/parsers/parser.h"

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

    if (node["package"]) {
        Package* pckg = new Package;
        ParserMetaData data;
        UML::Parsers::parsePackage(node["package"], *pckg, data);
        return pckg;
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
                // error
            }
        } else {
            // error
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
            // error
        }
    }
}

void SetTypeFunctor::operator()(Element& el) const {
    if (el.isSubClassOf(ElementType::TYPE)) {
        dynamic_cast<TypedElement*>(m_el)->setType(&dynamic_cast<Type&>(el));
    } else {
        // TODO error
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
                // error
            }
        } else {
            // error
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
                        // error
                    }
                }
            }
        } else {
            // error
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
                    // error
                }
            }
        } else {
            // error
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
            // error
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
                        // error
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
                    // error
                }
            }
        } else {
            // error
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
                // error
            }
        } else {
            // error
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
                    // error
                } 
            }
        } else {
            // error
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
                    }
                } else {
                    //error
                }
            }
        } else {
            // error
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
            // error
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
            // error
        }
    }
}

}
}