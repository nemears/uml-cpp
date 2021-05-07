#include "uml/parsers/parser.h"

using namespace std;

namespace UML {
namespace Parsers {

Element* parse(string path) {
    ParserMetaData data;
    data.m_path = path;

    return parse(data);
}

Element* parse(ParserMetaData& data) {
    YAML::Node node = YAML::LoadFile(data.m_path);

    if (node["class"]) {
        Class* clazz = new Class;
        parseClass(node["class"], *clazz, data);
        return clazz;
    }

    if (node["dataType"]) {
        DataType* dataType = new DataType;
        parseDataType(node["dataType"], *dataType, data);
        return dataType;
    }

    if (node["enumeration"]) {
        Enumeration* enumeration = new Enumeration;
        parseEnumeration(node["enumeration"], *enumeration, data);
        return enumeration;
    }

    if (node["instanceSpecification"]) {
        InstanceSpecification* inst = new InstanceSpecification;
        parseInstanceSpecification(node["instanceSpecification"], *inst, data);
        return inst;
    }

    if (node["instanceValue"]) {
        InstanceValue* instVal = new InstanceValue;
        parseInstanceValue(node["instanceValue"], *instVal, data);
        return instVal;
    }

    if (node["literalBool"]) {
        LiteralBool* lb = new LiteralBool;
        parseLiteralBool(node["literalBool"], *lb, data);
        return lb;
    }

    if (node["literalInt"]) {
        LiteralInt* li = new LiteralInt;
        parseLiteralInt(node["literalInt"], *li, data);
        return li;
    }

    if (node["literalReal"]) {
        LiteralReal* lr = new LiteralReal;
        parseLiteralReal(node["literalReal"], *lr, data);
        return lr;
    }

    if (node["literalString"]) {
        LiteralString* ls = new LiteralString;
        parseLiteralString(node["literalString"], *ls, data);
        return ls;
    }

    if (node["opaqueBehavior"]) {
        OpaqueBehavior* bhv = new OpaqueBehavior;
        parseOpaqueBehavior(node["opaqueBehavior"], *bhv, data);
        return bhv;
    }

    if (node["operation"]) {
        Operation* op = new Operation;
        parseOperation(node["operation"], *op, data);
        return op;
    }

    if (node["package"]) {
        Package* pckg = new Package;
        UML::Parsers::parsePackage(node["package"], *pckg, data);
        return pckg;
    }

    if (node["parameter"]) {
        Parameter* param = new Parameter;
        parseParameter(node["parameter"], *param, data);
        return param;
    }

    if (node["primitiveType"]) {
        PrimitiveType* type = new PrimitiveType;
        parsePrimitiveType(node["primitiveType"], *type, data);
        return type;
    }

    if (node["property"]) {
        Property* prop = new Property;
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
                throw UmlParserException("Value for id is not a valid UUID4, " + data.m_path.string() + " line " + to_string(node["id"].Mark().line));
            }
        } else {
            throw UmlParserException("Improper YAML node type for id field, must be scalar, " + data.m_path.string() + " line " + to_string(node["id"].Mark().line));
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
            throw UmlParserException("Improper YAML node type for name field, must be scalar, " + data.m_path.string() + " line " + to_string(node["name"].Mark().line));
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
                throw UmlParserException("ID for " + el.getElementTypeString() + " type field is invalid, " + data.m_path.string() + " line " + to_string(node["type"].Mark().line));
            }
        } else {
            throw UmlParserException("Improper YAML node type for type field, must be scalar, " + data.m_path.string() + " line " + to_string(node["type"].Mark().line));
        }
    }
}

void parseNamespace(YAML::Node node, Namespace& nmspc, ParserMetaData& data) {
    parseNamedElement(node, nmspc, data);

    // probably don't need this function
}

void parseClassifier(YAML::Node node, Classifier& clazz, ParserMetaData& data) {
    parseNamespace(node, clazz, data);

    // TODO attributes is read_only, specified later in ownedAttributes for DataType, and Structured Classifier etc.
    if (node["attributes"]) {
        if (node["attributes"].IsSequence()) {
            for (size_t i=0; i<node["attributes"].size(); i++) {
                if (node["attributes"][i]["property"]) {
                    if (node["attributes"][i]["property"].IsMap()) {
                        Property* prop = new Property;
                        parseProperty(node["attributes"][i]["property"], *prop, data);
                        clazz.getAttributes().add(*prop);
                    } else {
                        throw UmlParserException("Improper YAML node type for property field, must be map, " + data.m_path.string() + " line " + 
                                                 to_string(node["attributes"][i]["property"].Mark().line));
                    }
                }
            }
        } else {
            throw UmlParserException("Improper YAML node type for type field, must be scalar, " + data.m_path.string() + " line " + to_string(node["attributes"].Mark().line));
        }
    }
}

void parseDataType(YAML::Node node, DataType& dataType, ParserMetaData& data) {
    parseClassifier(node, dataType, data);

    if (node["ownedAttribute"]) {
        if (node["ownedAttribute"].IsSequence()) {
            for (size_t i = 0; i < node["ownedAttribute"].size(); i++) {
                if (node["ownedAttribute"][i]["property"]) {
                    if (node["ownedAttribute"][i]["property"].IsMap()) {
                        Property* prop = new Property;
                        parseProperty(node["ownedAttribute"][i]["property"], *prop, data);
                        dataType.getOwnedAttribute().add(*prop);
                    } else {
                        throw UmlParserException("Improper YAML node type for property, must be map, " + data.m_path.string() + " line " + to_string(node["ownedAttribute"][i]["property"].Mark().line));
                    }
                }
            }
        } else {
            throw UmlParserException("Improper YAML node type for dataType ownedAttribute, must be sequence, " + data.m_path.string() + " line " + to_string(node["ownedAttribute"].Mark().line));
        }
    }

    if (node["ownedOperation"]) {
        if (node["ownedOperation"].IsSequence()) {
            for (size_t i = 0; i < node["ownedOperation"].size(); i++) {
                if (node["ownedOperation"][i]["operation"]) {
                    if (node["ownedOperation"][i]["operation"].IsMap()) {
                        Operation* op = new Operation;
                        parseOperation(node["ownedOperation"][i]["operation"], *op, data);
                        dataType.getOwnedOperation().add(*op);
                    } else {
                        throw UmlParserException("Improper YAML node type for operation, must be map, " + data.m_path.string() + " line " + to_string(node["ownedOperation"][i]["operation"].Mark().line));
                    }
                } else {
                    throw UmlParserException("Improper UML node type for ownedOperation sequence, " + data.m_path.string() + " line " + to_string(node["ownedOperation"][i].Mark().line));
                }
            }
        } else {
            throw UmlParserException("Improper YAML node type for dataType ownedOperation, must be sequence, " + data.m_path.string() + " line " + to_string(node["ownedOperation"].Mark().line));
        }
    }
}

void parsePrimitiveType(YAML::Node node, PrimitiveType& type, ParserMetaData& data) {
    parseDataType(node, type, data);
}

void parseStructuredClassifier(YAML::Node node, StructuredClassifier& clazz, ParserMetaData& data) {
    parseClassifier(node, clazz, data);

    if (node["ownedAttributes"]) {
        if (node["ownedAttributes"].IsSequence()) {
            for (size_t i=0; i<node["ownedAttributes"].size(); i++) {
                if (node["ownedAttributes"][i]["property"]) {
                    if (node["ownedAttributes"][i]["property"].IsMap()) {
                        Property* prop = new Property;
                        parseProperty(node["ownedAttributes"][i]["property"], *prop, data);
                        clazz.getOwnedAttributes().add(*prop);
                    } else {
                        throw UmlParserException("Improper YAML node type for property field, must be map, " + data.m_path.string() + " line " + 
                                                 to_string(node["ownedAttributes"][i]["property"].Mark().line));
                    }
                }
            }
        } else {
            throw UmlParserException("Improper YAML node type for ownedAttributes field, must be sequence, " + data.m_path.string() + " line " + to_string(node["ownedAttributes"].Mark().line));
        }
    }
}

void parseClass(YAML::Node node, Class& clazz, ParserMetaData& data) {
    parseStructuredClassifier(node, clazz, data);

    if (node["operations"]) {
        if (node["operations"].IsSequence()) {
            for (size_t i=0; i<node["operations"].size(); i++) {
                if (node["operations"][i]["operation"]) {
                    Operation * op = new Operation;
                    parseOperation(node["operations"][i]["operation"], *op, data);
                    clazz.getOperations().add(*op);
                } else {
                    throw UmlParserException("Could not identify operation to parse, " + data.m_path.string() + " line " + to_string(node["operations"][i].Mark().line));
                }
            }
        } else {
            throw UmlParserException("Improper YAML node type for operations field, must be scalar, " + data.m_path.string() + " line " + to_string(node["attributes"].Mark().line));
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
            throw UmlParserException("Improper YAML node type for parameters field, must be scalar, " + data.m_path.string() + " line " + to_string(node["parameters"].Mark().line));
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
                    throw UmlParserException("Improper YAML node type for bodies sequence, must be scalar or map, " + data.m_path.string() + " line " + to_string(node["bodies"][i].Mark().line));
                }
            }
        } else {
            throw UmlParserException("Improper YAML node type for bodies, must be sequence, " + data.m_path.string() + " line " + to_string(node["bodies"].Mark().line));
        }
    }
}

void parseProperty(YAML::Node node, Property& prop, ParserMetaData& data) {
    parseTypedElement(node, prop, data);
    // TODO structural feature parser?
    parseMultiplicityElement(node, prop, data);

    if (node["aggregation"]) {
        if (node["aggregation"].IsScalar()) {
            string aggregation = node["aggregation"].as<string>();
            if (aggregation.compare("COMPOSITE") == 0) {
                prop.setAggregation(AggregationKind::COMPOSITE);
            } else if (aggregation.compare("SHARED") == 0) {
                prop.setAggregation(AggregationKind::SHARED);
            } else if (aggregation.compare("NONE") == 0) {
                prop.setAggregation(AggregationKind::NONE);
            }
        } else {
            throw UmlParserException("Improper YAML node type for bodies, must be scalar, " + data.m_path.string() + " line " + to_string(node["aggregation"].Mark().line));
        }
    }

    if (node["defaultValue"]) {
        if (node["defaultValue"].IsMap()) {
            if (node["defaultValue"]["literalBool"]) {
                if (node["defaultValue"]["literalBool"].IsMap()) {
                    LiteralBool* lb = new LiteralBool;
                    parseLiteralBool(node["defaultValue"]["literalBool"], *lb, data);
                    prop.setDefaultValue(lb);
                } else {
                    throw UmlParserException("Improper YAML node type for Properties defaultValue field, " + data.m_path.string() + " line " + to_string(node["defaultValue"]["literalBool"].Mark().line));
                }
            } else if (node["defaultValue"]["literalInt"]) {
                if (node["defaultValue"]["literalInt"].IsMap()) {
                    LiteralInt* li = new LiteralInt;
                    parseLiteralInt(node["defaultValue"]["literalInt"], *li, data);
                    prop.setDefaultValue(li);
                } else {
                    throw UmlParserException("Improper YAML node type for Properties defaultValue field, " + data.m_path.string() + " line " + to_string(node["defaultValue"]["literalInt"].Mark().line));
                }
            } else if (node["defaultValue"]["literalReal"]) {
                if (node["defaultValue"]["literalReal"].IsMap()) {
                    LiteralReal* lr = new LiteralReal;
                    parseLiteralReal(node["defaultValue"]["literalReal"], *lr, data);
                    prop.setDefaultValue(lr);
                } else {
                    throw UmlParserException("Improper YAML node type for Properties defaultValue field, " + data.m_path.string() + " line " + to_string(node["defaultValue"]["literalReal"].Mark().line));
                }
            } else if (node["defaultValue"]["literalString"]) {
                if (node["defaultValue"]["literalString"].IsMap()) {
                    LiteralString* ls = new LiteralString;
                    parseLiteralString(node["defaultValue"]["literalString"], *ls, data);
                    prop.setDefaultValue(ls);
                } else {
                    throw UmlParserException("Improper YAML node type for Properties defaultValue field, " + data.m_path.string() + " line " + to_string(node["defaultValue"]["literalString"].Mark().line));
                }
            } else if (node["defaultValue"]["instanceValue"]) {
                if (node["defaultValue"]["instanceValue"].IsMap()) {
                    InstanceValue* iv = new InstanceValue;
                    parseInstanceValue(node["defaultValue"]["instanceValue"], *iv, data);
                    prop.setDefaultValue(iv);
                } else {
                    throw UmlParserException("Improper YAML node type for Properties defaultValue field, " + data.m_path.string() + " line " + to_string(node["defaultValue"]["instanceValue"].Mark().line));
                }
            } else {
                throw UmlParserException("Unknown Value Specification for default value field, " + data.m_path.string() + " line " + to_string(node["defaultValue"].Mark().line));
            }
        }
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
                throw UmlParserException("Invalid direction provided for Parameter, " + data.m_path.string() + " line " + to_string(node["direction"].Mark().line));
            }
        } else {
            throw UmlParserException("Improper YAML node type for direction, must be scalar, " + data.m_path.string() + " line " + to_string(node["direction"].Mark().line));
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
                    throw UmlParserException("Invalid behavior type for operation methods, " + data.m_path.string() + " line " + to_string(node["methods"][i].Mark().line));
                } 
            }
        } else {
            throw UmlParserException("Improper YAML node type for methods, must be sequence, " + data.m_path.string() + " line " + to_string(node["methods"].Mark().line));
        }
    }

    // TODO Parameters
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

    if (node["packageMerge"]) {
        if (node["packageMerge"].IsSequence()) {
            for (size_t i = 0; i < node["packageMerge"].size(); i++) {
                if (node["packageMerge"][i]["packageMerge"]) {
                    if (node["packageMerge"][i]["packageMerge"].IsMap()) {
                        PackageMerge* merge = new PackageMerge;
                        parsePackageMerge(node["packageMerge"][i]["packageMerge"], *merge, data);
                        pckg.getPackageMerge().add(*merge);
                    } else {
                        throw UmlParserException("Invalid YAML Node type for packageMerge definition with in Package, " + data.m_path.string() + " line " + to_string(node["packageMerge"][i]["packageMerge"].Mark().line));
                    }
                } else {
                    throw UmlParserException("Invalid identifier in Package packageMerge list, " + data.m_path.string() + " line " + to_string(node["packageMerge"][i]["packageMerge"].Mark().line));
                }
            }
        } else {
            throw UmlParserException("Invalid YAML Node type for Package field packageMerge, " + data.m_path.string() + " line " + to_string(node["packageMerge"].Mark().line));
        }
    }

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
                    } else if (node["packagedElements"][i]["dataType"]) {
                        DataType* dataType = new DataType;
                        parseDataType(node["packagedElements"][i]["dataType"], *dataType, data);
                        pckg.getPackagedElements().add(*dataType);
                    } else if (node["packagedElements"][i]["enumeration"]) {
                        Enumeration* enumeration = new Enumeration;
                        parseEnumeration(node["packagedElements"][i]["enumeration"], *enumeration, data);
                        pckg.getPackagedElements().add(*enumeration);
                    } else if (node["packagedElements"][i]["instanceSpecification"]) {
                        InstanceSpecification* inst = new InstanceSpecification;
                        parseInstanceSpecification(node["packagedElements"][i]["instanceSpecification"], *inst, data);
                        pckg.getPackagedElements().add(*inst);
                    } else if (node["packagedElements"][i]["instanceValue"]) {
                        InstanceValue* instVal = new InstanceValue;
                        parseInstanceValue(node["packagedElements"][i]["instanceValue"], *instVal, data);
                        pckg.getPackagedElements().add(*instVal);
                    } else if (node["packagedElements"][i]["literalBool"]) {
                        LiteralBool* lb = new LiteralBool;
                        parseLiteralBool(node["packagedElements"][i]["literalBool"], *lb, data);
                        pckg.getPackagedElements().add(*lb);
                    } else if (node["packagedElements"][i]["literalInt"]) {
                        LiteralInt* li = new LiteralInt;
                        parseLiteralInt(node["packagedElements"][i]["literalInt"], *li, data);
                        pckg.getPackagedElements().add(*li);
                    } else if (node["packagedElements"][i]["literalReal"]) {
                        LiteralReal* lr = new LiteralReal;
                        parseLiteralReal(node["packagedElements"][i]["literalReal"], *lr, data);
                        pckg.getPackagedElements().add(*lr);
                    } else if (node["packagedElements"][i]["literalString"]) {
                        LiteralString* ls = new LiteralString;
                        parseLiteralString(node["packagedElements"][i]["literalString"], *ls, data);
                        pckg.getPackagedElements().add(*ls);
                    } else if (node["packagedElements"][i]["package"]) {
                        Package* package = new Package;
                        parsePackage(node["packagedElements"][i]["package"], *package, data);
                        package->setOwningPackage(&pckg);
                    } else if (node["packagedElements"][i]["primitiveType"]) {
                        PrimitiveType* type = new PrimitiveType;
                        parsePrimitiveType(node["packagedElements"][i]["primitiveType"], *type, data);
                        pckg.getPackagedElements().add(*type);
                    } else {
                        throw UmlParserException("Invalid identifier for packagedElements, " + data.m_path.string() + " line " + to_string(node["packagedElements"][i].Mark().line));
                    }
                } else {
                    throw UmlParserException("Invalid YAML node type for field packagedElements sequence, must be map, " + data.m_path.string() + " line " 
                                             + to_string(node["packagedElements"][i].Mark().line));
                }
            }
        } else {
            throw UmlParserException("Invalid YAML node type for field packagedElements, must be sequence, " + data.m_path.string() + " line " + to_string(node["packagedElements"].Mark().line));
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
            throw UmlParserException("Invalid YAML node type for field lower, must be scalar or map, " + data.m_path.string() + " line " + to_string(node["lower"].Mark().line));
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
            throw UmlParserException("Invalid YAML node type for field upper, must be scalar or map, " + data.m_path.string() + " line " + to_string(node["upper"].Mark().line));
        }
    }
}

void SetClassifierFunctor::operator()(Element& el) const {
    if (el.isSubClassOf(ElementType::CLASSIFIER)) {
        dynamic_cast<InstanceSpecification*>(m_el)->setClassifier(&dynamic_cast<Classifier&>(el));
    } else {
        throw UmlParserException(m_el->getElementTypeString() + " id: " + boost::lexical_cast<string>(m_el->getID()) + 
                                 " assigned classifier is not a classifer! line " + to_string(m_node.Mark().line));
    }
}

void parseInstanceSpecification(YAML::Node node, InstanceSpecification& inst, ParserMetaData& data) {
    parseNamedElement(node, inst, data);

    if (node["classifier"]) {
        if (node["classifier"].IsScalar()) {
            string classifierID = node["classifier"].as<string>();
            if (isValidUUID4(classifierID)) {
                boost::uuids::uuid id = boost::lexical_cast<boost::uuids::uuid>(classifierID);
                applyFunctor(data, id, new SetClassifierFunctor(&inst, node["classifier"]));
            }
        } else {
            throw UmlParserException("Invalid YAML node type for InstanceSpecification field classifier," + data.m_path.string() + " line " + to_string(node["classifier"].Mark().line));
        }
    }

    if (node["slots"]) {
        if (node["slots"].IsSequence()) {
            for (size_t i = 0; i < node["slots"].size(); i++) {
                if (node["slots"][i]["slot"]) {
                    if (node["slots"][i]["slot"].IsMap()) {
                        Slot* slot = new Slot;
                        parseSlot(node["slots"][i]["slot"], *slot, data);
                        inst.getSlots().add(*slot);
                    }
                }
            }
        } else {
            throw UmlParserException("Invalid YAML node type for InstanceSpecification field slots, expected sequence, " + data.m_path.string() + " line " + to_string(node["slots"].Mark().line));
        }
    }
}

void SetDefiningFeatureFunctor::operator()(Element& el) const {
    if (el.isSubClassOf(ElementType::STRUCTURAL_FEATURE)) {
        dynamic_cast<Slot*>(m_el)->setDefiningFeature(&dynamic_cast<StructuralFeature&>(el));
    } else {
        throw UmlParserException(m_el->getElementTypeString() + " id: " + boost::lexical_cast<string>(m_el->getID()) + 
                                 " assigned definingFeature is not a structuralFeature! line " + to_string(m_node.Mark().line));
    }
}

void parseSlot(YAML::Node node, Slot& slot, ParserMetaData& data) {
    parseElement(node, slot, data);

    if (node["definingFeature"]) {
        if (node["definingFeature"].IsScalar()) {
            string stringID = node["definingFeature"].as<string>();
            if (isValidUUID4(stringID)) {
                boost::uuids::uuid definingFeatureID = boost::lexical_cast<boost::uuids::uuid>(stringID);
                applyFunctor(data, definingFeatureID, new SetDefiningFeatureFunctor(&slot, node["definingFeature"]));
            }
        } else {
            throw UmlParserException("Invalid YAML node type for Slot field definingFeature, expected scalar, " + data.m_path.string() + " line " + to_string(node["definingFeature"].Mark().line));
        }
    }

    // TODO values
    if (node["values"]) {
        if (node["values"].IsSequence()) {
            for (size_t i = 0; i < node["values"].size(); i++) {
                if (node["values"][i]["instanceValue"]) {
                    if (node["values"][i]["instanceValue"].IsMap()) {
                        InstanceValue* instVal = new InstanceValue;
                        parseInstanceValue(node["values"][i]["instanceValue"], *instVal, data);
                        slot.getValues().add(*instVal);
                    } else {
                        UmlParserException("InstanceValue must be map Node type! " + data.m_path.string() + " line " + to_string(node["values"][i]["instanceValue"].Mark().line));
                    }
                } else {
                    throw UmlParserException("unknown value type for slot, " + data.m_path.string() + " line " + to_string(node["values"][i].Mark().line));
                }
            }
        } else {
            throw UmlParserException("Invalid YAML node type for Slot field values, expected Sequence, " + data.m_path.string() + " line " + to_string(node["values"].Mark().line));
        }
    }
}

void parseEnumeration(YAML::Node node, Enumeration& enumeration, ParserMetaData& data) {
    parseDataType(node, enumeration, data);

    if (node["ownedLiteral"]) {
        if (node["ownedLiteral"].IsSequence()) {
            for (size_t i = 0; i < node["ownedLiteral"].size(); i++) {
                if (node["ownedLiteral"][i]["enumerationLiteral"]) {
                    if (node["ownedLiteral"][i]["enumerationLiteral"].IsMap()) {
                        EnumerationLiteral* literal = new EnumerationLiteral;
                        parseEnumerationLiteral(node["ownedLiteral"][i]["enumerationLiteral"], *literal, data);
                        enumeration.getOwnedLiteral().add(*literal);
                    } else {
                        throw UmlParserException("Invalid YAML node type enumerationLiteral definition, should be map, " + data.m_path.string() + " line " + to_string(node["ownedLiteral"][i]["enumerationLiteral"].Mark().line));
                    }
                } else {
                    throw UmlParserException("Invalid UML type in enumeration ownedLiteral list, must be literal," + data.m_path.string() + " line " + to_string(node["ownedLiteral"].Mark().line));
                }
            }
        } else {
            throw UmlParserException("Invalid YAML node type for Enumeration field ownedLiteral, expected sequence, " + data.m_path.string() + " line " + to_string(node["ownedLiteral"].Mark().line));
        }
    }
}

void parseEnumerationLiteral(YAML::Node node, EnumerationLiteral& literal, ParserMetaData& data) {
    parseInstanceSpecification(node, literal, data);
}

void SetInstanceFunctor::operator()(Element& el) const {
    if (el.isSubClassOf(ElementType::INSTANCE_SPECIFICATION)) {
        dynamic_cast<InstanceValue*>(m_el)->setInstance(&dynamic_cast<InstanceSpecification&>(el));
    } else {
        throw UmlParserException(m_el->getElementTypeString() + " id: " + boost::lexical_cast<string>(m_el->getID()) + 
                                 " assigned instance is not an instanceSpecification! line " + to_string(m_node.Mark().line));
    }
}

void parseInstanceValue(YAML::Node node, InstanceValue& val, ParserMetaData& data) {
    parseTypedElement(node, val, data);

    if (node["instance"]) {
        if (node["instance"].IsScalar()) {
            string instID = node["instance"].as<string>();
            if (isValidUUID4(instID)) {
                boost::uuids::uuid id = boost::lexical_cast<boost::uuids::uuid>(instID);
                applyFunctor(data, id, new SetInstanceFunctor(&val, node["instance"]));
            } else {
                throw UmlParserException("Scalar YAML node for InstanceValue field instance is not a valid uuid4, " + data.m_path.string() + " line " + to_string(node["instance"].Mark().line));
            }
        } else {
            throw UmlParserException("Invalid YAML node type for InstanceValue field instance, expect scalar, " + data.m_path.string() + " line " + to_string(node["instance"].Mark().line));
        }
    }
}

void SetMergedPackageFunctor::operator()(Element& el) const {
    if (el.isSubClassOf(ElementType::PACKAGE)) {
        dynamic_cast<PackageMerge*>(m_el)->setMergedPackage(&dynamic_cast<Package&>(el));
    } else {
        throw UmlParserException(m_el->getElementTypeString() + " id: " + boost::lexical_cast<string>(m_el->getID()) + 
                                        " assigned mergedPackage that is not a Package! line " + to_string(m_node.Mark().line));
    }
}

void parsePackageMerge(YAML::Node node, PackageMerge& merge, ParserMetaData& data) {
    parseElement(node, merge, data);

    if (node["receivingPackage"]) {
        // This won't be called cause it is always defined from within receiving Package body
        throw UmlParserException("TODO");
    }

    if (node["mergedPackage"]) {
        if (node["mergedPackage"].IsScalar()) {
            string pckgString = node["mergedPackage"].as<string>();
            if (isValidUUID4(pckgString)) {
                boost::uuids::uuid pckgID = boost::lexical_cast<boost::uuids::uuid>(pckgString);
                applyFunctor(data, pckgID, new SetMergedPackageFunctor(&merge, node["mergedPackage"]));
            } else {
                if (filesystem::exists(data.m_path.parent_path() / pckgString)) {
                    filesystem::path cPath = data.m_path;
                    data.m_path = cPath.parent_path() / pckgString;;
                    Element* mergedPackage = parse(data);
                    data.m_path = cPath;
                    if (mergedPackage->isSubClassOf(ElementType::PACKAGE)) {
                        merge.setMergedPackage(dynamic_cast<Package*>(mergedPackage));
                    } else {
                        throw UmlParserException("mergedPackage is not a package, " + data.m_path.string() + " line " + to_string(node["mergedPackage"].Mark().line));
                    }
                } else {
                    throw UmlParserException("Could not parse external mergedPackage, " + data.m_path.string() + " line " + to_string(node["mergedPackage"].Mark().line));
                }
            }
        } else {
            throw UmlParserException("Invalid YAML node type for PackageMerge field mergedPackage, expected scalar, " + data.m_path.string() + " line " + to_string(node["mergedPackage"].Mark().line));
        }
    }
}

void parseLiteralBool(YAML::Node node, LiteralBool& lb, ParserMetaData& data) {
    parseTypedElement(node, lb, data);

    if (node["value"]) {
        if (node["value"].IsScalar()) {
            bool val = node["value"].as<bool>();
            lb.setValue(val);
        } else {
            throw UmlParserException("Invalid YAML node type for LiteralBool field value, expected scalar, " + data.m_path.string() + " line " + to_string(node["value"].Mark().line));
        }
    }
}

void parseLiteralInt(YAML::Node node, LiteralInt& li, ParserMetaData& data) {
    parseTypedElement(node, li, data);

    if (node["value"]) {
        if (node["value"].IsScalar()) {
            int val = node["value"].as<int>();
            li.setValue(val);
        } else {
            throw UmlParserException("Invalid YAML node type for LiteralInt field value, expected scalar, " + data.m_path.string() + " line " + to_string(node["value"].Mark().line));
        }
    }
}

void parseLiteralReal(YAML::Node node, LiteralReal& lr, ParserMetaData& data) {
    parseTypedElement(node, lr, data);

    if (node["value"]) {
        if (node["value"].IsScalar()) {
            double val = node["value"].as<double>();
            lr.setValue(val);
        } else {
            throw UmlParserException("Invalid YAML node type for LiteralReal field value, expected scalar, " + data.m_path.string() + " line " + to_string(node["value"].Mark().line));
        }
    }
}

void parseLiteralString(YAML::Node node, LiteralString& ls, ParserMetaData& data) {
    parseTypedElement(node, ls, data);

    if (node["value"]) {
        if (node["value"].IsScalar()) {
            string val = node["value"].as<string>();
            ls.setValue(val);
        } else {
            throw UmlParserException("Invalid YAML node type for LiteralString field value, expected scalar, " + data.m_path.string() + " line " + to_string(node["value"].Mark().line));
        }
    }
}

}
}