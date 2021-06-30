#include "uml/parsers/parser.h"
#include "uml/model.h"
#include "uml/umlManager.h"
#include "uml/literalNull.h"

using namespace std;

namespace UML {
namespace Parsers {

string emit(Element& el) {
    YAML::Emitter emitter;

    emit(emitter, el);

    return emitter.c_str();
}

void ManagerFriendFunctor::operator()(UmlManager* manager, Model* model) const {
    manager->m_model = model;
}

Model* parseModel(UmlManager* manager) {
    ParserMetaData data(manager);
    YAML::Node node = YAML::LoadFile(data.m_path);
    if (node["model"]) {
        Model& m = manager->create<Model>();
        parsePackage(node["model"], m, data);
        ManagerFriendFunctor setModel;
        setModel(manager, &m);
        return &m;
    } else {
        throw UmlParserException("base node in " + data.m_path.string() + " is not a model!");
        return 0;
    }
}

Model* parseModel(ParserMetaData& data) {
    YAML::Node node = YAML::LoadFile(data.m_path);
    if (node["model"]) {
        Model& m = data.m_manager->create<Model>();
        parsePackage(node["model"], m, data);
        return &m;
    } else {
        throw UmlParserException("base node in " + data.m_path.string() + " is not a model!");
        return 0;
    }
}

UmlManager* parse(string path) {
    UmlManager* ret = new UmlManager;
    ret->parse(path);
    return ret;
}

Element* parse(ParserMetaData& data) {
    YAML::Node node = YAML::LoadFile(data.m_path);
    return parseNode(node, data);
}

Element* parseNode(YAML::Node node, ParserMetaData& data) {
    if (node["class"]) {
        Class& clazz = data.m_manager->create<Class>();
        parseClass(node["class"], clazz, data);
        return &clazz;
    }

    if (node["dataType"]) {
        DataType& dataType = data.m_manager->create<DataType>();
        parseDataType(node["dataType"], dataType, data);
        return &dataType;
    }

    if (node["enumeration"]) {
        Enumeration& enumeration = data.m_manager->create<Enumeration>();
        parseEnumeration(node["enumeration"], enumeration, data);
        return &enumeration;
    }

    if (node["expression"]) {
        Expression& exp = data.m_manager->create<Expression>();
        parseExpression(node["expression"], exp, data); 
        return &exp;
    }

    if (node["instanceSpecification"]) {
        InstanceSpecification& inst = data.m_manager->create<InstanceSpecification>();
        parseInstanceSpecification(node["instanceSpecification"], inst, data);
        return &inst;
    }

    if (node["instanceValue"]) {
        InstanceValue& instVal = data.m_manager->create<InstanceValue>();
        parseInstanceValue(node["instanceValue"], instVal, data);
        return &instVal;
    }

    if (node["literalBool"]) {
        LiteralBool& lb = data.m_manager->create<LiteralBool>();
        parseLiteralBool(node["literalBool"], lb, data);
        return &lb;
    }

    if (node["literalInt"]) {
        LiteralInt& li = data.m_manager->create<LiteralInt>();
        parseLiteralInt(node["literalInt"], li, data);
        return &li;
    }

    if (node["literalNull"]) {
        LiteralNull& ln = data.m_manager->create<LiteralNull>();
        parseTypedElement(node["literalNull"], ln, data);
        return &ln;
    }

    if (node["literalReal"]) {
        LiteralReal& lr = data.m_manager->create<LiteralReal>();
        parseLiteralReal(node["literalReal"], lr, data);
        return &lr;
    }

    if (node["literalString"]) {
        LiteralString& ls = data.m_manager->create<LiteralString>();
        parseLiteralString(node["literalString"], ls, data);
        return &ls;
    }

    if (node["literalUnlimitedNatural"]) {
        LiteralUnlimitedNatural& ln = data.m_manager->create<LiteralUnlimitedNatural>();
        parseLiteralUnlimitedNatural(node["literalUnlimitedNatural"], ln, data);
        return &ln;
    }

    if (node["model"]) {
        Model& model = data.m_manager->create<Model>();
        parsePackage(node["model"], model, data);
        return &model;
    }

    if (node["opaqueBehavior"]) {
        OpaqueBehavior& bhv = data.m_manager->create<OpaqueBehavior>();
        parseOpaqueBehavior(node["opaqueBehavior"], bhv, data);
        return &bhv;
    }

    if (node["operation"]) {
        Operation& op = data.m_manager->create<Operation>();
        parseOperation(node["operation"], op, data);
        return &op;
    }

    if (node["package"]) {
        Package& pckg = data.m_manager->create<Package>();
        UML::Parsers::parsePackage(node["package"], pckg, data);
        return &pckg;
    }

    if (node["parameter"]) {
        Parameter& param = data.m_manager->create<Parameter>();
        parseParameter(node["parameter"], param, data);
        return &param;
    }

    if (node["primitiveType"]) {
        PrimitiveType& type = data.m_manager->create<PrimitiveType>();
        parsePrimitiveType(node["primitiveType"], type, data);
        return &type;
    }

    if (node["property"]) {
        Property& prop = data.m_manager->create<Property>();
        parseProperty(node["property"], prop, data);
        return &prop;
    }

    return 0;
}

void emit(YAML::Emitter& emitter, Element& el) {
    switch(el.getElementType()) {
        case ElementType::CLASS : {
            emitClass(emitter, dynamic_cast<Class&>(el));
            break;
        }
        case ElementType::DATA_TYPE : {
            emitDataType(emitter, dynamic_cast<DataType&>(el));
            break;
        }
        case ElementType::ENUMERATION : {
            emitEnumeration(emitter, dynamic_cast<Enumeration&>(el));
            break;
        }
        case ElementType::EXPRESSION : {
            emitExpression(emitter, dynamic_cast<Expression&>(el));
            break;
        }
        case ElementType::INSTANCE_SPECIFICATION : {
            emitInstanceSpecification(emitter, dynamic_cast<InstanceSpecification&>(el));
            break;
        }
        case ElementType::LITERAL_BOOL : {
            emitLiteralBool(emitter, dynamic_cast<LiteralBool&>(el));
            break;
        }
        case ElementType::LITERAL_INT : {
            emitLiteralInt(emitter, dynamic_cast<LiteralInt&>(el));
            break;
        }
        case ElementType::LITERAL_NULL : {
            emitter << YAML::BeginMap << YAML::Key << "literalNull" << YAML::Value << YAML::BeginMap;
            emitTypedElement(emitter, dynamic_cast<TypedElement&>(el));
            emitter << YAML::EndMap << YAML::EndMap;
            break;
        }
        case ElementType::LITERAL_REAL : {
            emitLiteralReal(emitter, dynamic_cast<LiteralReal&>(el));
            break;
        }
        case ElementType::LITERAL_STRING : {
            emitLiteralString(emitter, dynamic_cast<LiteralString&>(el));
            break;
        }
        case ElementType::LITERAL_UNLIMITED_NATURAL : {
            emitLiteralUnlimitedNatural(emitter, dynamic_cast<LiteralUnlimitedNatural&>(el));
            break;
        }
        case ElementType::MODEL : {
            emitModel(emitter, dynamic_cast<Model&>(el));
            break;
        }
        case ElementType::OPAQUE_BEHAVIOR : {
            emitOpaqueBehavior(emitter, dynamic_cast<OpaqueBehavior&>(el));
            break;
        }
        case ElementType::PACKAGE : {
            emitPackage(emitter, dynamic_cast<Package&>(el));
            break;
        }
        case ElementType::PRIMITIVE_TYPE : {
            emitPrimitiveType(emitter, dynamic_cast<PrimitiveType&>(el));
            break;
        }
        default: {
            throw UmlParserException("Error emitting element, element type " + el.getElementTypeString() + " is abstract and cannot be emit");
            break;
        }
    }
}

namespace{

void emitModel(YAML::Emitter& emitter, Model& model) {
    if (model.getElementType() == ElementType::MODEL) {
        emitter << YAML::BeginMap << YAML::Key << "model" << YAML::Value << YAML::BeginMap;
    }

    emitPackage(emitter, model);

    if (model.getElementType() == ElementType::MODEL) {
        emitter << YAML::EndMap << YAML::EndMap;
    }
}

void parseElement(YAML::Node node, Element& el, ParserMetaData& data) {
    if (node["id"]) {
        if (node["id"].IsScalar()) {
            string id = node["id"].as<string>();
            if (isValidID(id)) {
                el.setID(ID::fromString(id));
            } else {
                throw UmlParserException("Value for id is not a valid ID, must be base64 url safe encoded 28 character string, " + data.m_path.string() + " line " + to_string(node["id"].Mark().line));
            }
        } else {
            throw UmlParserException("Improper YAML node type for id field, must be scalar, " + data.m_path.string() + " line " + to_string(node["id"].Mark().line));
        }
    }

    // apply post processing here via functor
    data.elements.add(el);
}

void emitElement(YAML::Emitter& emitter, Element& el) {
    emitter << YAML::Key << "id" << YAML::Value << el.getID().string();
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

    if (node["visibility"]) {
        if (node["visibility"].IsScalar()) {
            string visibilityString = node["visibility"].as<string>();
            if (visibilityString.compare("PRIVATE") == 0) {
                el.setVisibility(VisibilityKind::PRIVATE);
            } else if (visibilityString.compare("PROTECTED") == 0) {
                el.setVisibility(VisibilityKind::PROTECTED);
            } else if (visibilityString.compare("PACKAGE") == 0) {
                el.setVisibility(VisibilityKind::PACKAGE);
            }
        } else {
            throw UmlParserException("Improper YAML node type for NamedElement field visibility, must be a scalar, " + data.m_path.string() + " line " + to_string(node["visibility"].Mark().line));
        }
    }
}

void emitNamedElement(YAML::Emitter& emitter, NamedElement& el) {
    emitElement(emitter, el);
    if (!el.getName().empty()) {
        emitter << YAML::Key << "name" << YAML::Value << el.getName();
    }
    if (el.getVisibility() != VisibilityKind::PUBLIC) {
        string visibility = "";
        switch(el.getVisibility()) {
            case VisibilityKind::PACKAGE : {
                visibility = "PACKAGE";
                break;
            }
            case VisibilityKind::PRIVATE : {
                visibility = "PRIVATE";
                break;
            }
            case VisibilityKind::PROTECTED : {
                visibility = "PROTECTED";
                break;
            }
        }
        emitter << YAML::Key << "visibility" << YAML::Value << visibility;
    }
}

void SetTypeFunctor::operator()(Element& el) const {
    if (el.isSubClassOf(ElementType::TYPE)) {
        dynamic_cast<TypedElement*>(m_el)->setType(&dynamic_cast<Type&>(el));
    } else {
        throw UmlParserException(m_el->getElementTypeString() + " id: " + m_el->getID().string() + 
                                 " assigned type is not a typed Element! line " + to_string(m_node.Mark().line));
    }
}

void parseTypedElement(YAML::Node node, TypedElement& el, ParserMetaData& data) {

    parseNamedElement(node, el, data);

    if (node["type"]) {
        if (node["type"].IsScalar()) {
            string typeIDstring = node["type"].as<string>();
            if (isValidID(typeIDstring)) {
                ID typeID = ID::fromString(typeIDstring);
                applyFunctor(data, typeID, new SetTypeFunctor(&el, node["type"]));
            } else {
                throw UmlParserException("ID for " + el.getElementTypeString() + " type field is invalid, " + data.m_path.string() + " line " + to_string(node["type"].Mark().line));
            }
        } else {
            throw UmlParserException("Improper YAML node type for type field, must be scalar, " + data.m_path.string() + " line " + to_string(node["type"].Mark().line));
        }
    }
}

void emitTypedElement(YAML::Emitter& emitter, TypedElement& el) {
    emitNamedElement(emitter, el);

    if (el.getType()) {
        emitter << YAML::Key << "type" << YAML::Value << el.getType()->getID().string();
    }
}

void parseClassifier(YAML::Node node, Classifier& clazz, ParserMetaData& data) {
    parseNamedElement(node, clazz, data);
    parseTemplateableElement(node, clazz, data);

    if (node["generalizations"]) {
        if (node["generalizations"].IsSequence()) {
            for (size_t i = 0; i < node["generalizations"].size(); i++) {
                if (node["generalizations"][i]["generalization"]) {
                    if (node["generalizations"][i]["generalization"].IsMap()) {
                        Generalization& gen = data.m_manager->create<Generalization>();
                        parseGeneralization(node["generalizations"][i]["generalization"], gen, data);
                        clazz.getGeneralizations().add(gen);
                    } else {
                        throw UmlParserException("Improper YAML node type for Generalization definition, " + data.m_path.string() + to_string(node["generalizations"][i]["generalization"].Mark().line));
                    }
                }
            }
        } else {
            throw UmlParserException("Improper YAML node type for Classifier field generalizations, must be sequence " + data.m_path.string() + to_string(node["generalizations"].Mark().line));
        }
    }
}

void emitClassifier(YAML::Emitter& emitter, Classifier& clazz) {
    emitNamedElement(emitter, clazz);
    
    if (!clazz.getGeneralizations().empty()) {
        emitter << YAML::Key << "generalizations" << YAML::Value << YAML::BeginSeq;
        for (auto& generalization: clazz.getGeneralizations()) {
            emitGeneralization(emitter, generalization);
        }
        emitter << YAML::EndSeq;
    }
}

void SetGeneralFunctor::operator()(Element& el) const {
    if (el.isSubClassOf(ElementType::CLASSIFIER)) {
        dynamic_cast<Generalization*>(m_el)->setGeneral(&dynamic_cast<Classifier&>(el));
    }
}

void parseGeneralization(YAML::Node node, Generalization& general, ParserMetaData& data) {
    if (node["specific"]) {
        // TODO error? should be specified in specific
    }

    if (node["general"]) {
        if (node["general"].IsScalar()) {
            string generalString = node["general"].as<string>();
            if (isValidID(generalString)) {
                ID generalID = ID::fromString(generalString);
                applyFunctor(data, generalID, new SetGeneralFunctor(&general, node["general"]));
            }
        } else {
            // TODO throw error or can it be defined here?
        }
    }
}

void emitGeneralization(YAML::Emitter& emitter, Generalization& generalization) {
    if (generalization.getElementType() == ElementType::GENERALIZATION) {
        emitter << YAML::BeginMap << YAML::Key << "generalization" << YAML::Value << YAML::BeginMap;
    }

    emitElement(emitter, generalization);

    if (generalization.getGeneral()) {
        emitter << YAML::Key << "general" << YAML::Value << generalization.getGeneral()->getID().string();
    }

    if (generalization.getElementType() == ElementType::GENERALIZATION) {
        emitter << YAML::EndMap << YAML::EndMap;
    }
}

void parseDataType(YAML::Node node, DataType& dataType, ParserMetaData& data) {
    parseClassifier(node, dataType, data);

    if (node["ownedAttribute"]) {
        if (node["ownedAttribute"].IsSequence()) {
            for (size_t i = 0; i < node["ownedAttribute"].size(); i++) {
                if (node["ownedAttribute"][i]["property"]) {
                    if (node["ownedAttribute"][i]["property"].IsMap()) {
                        Property& prop = data.m_manager->create<Property>();
                        parseProperty(node["ownedAttribute"][i]["property"], prop, data);
                        dataType.getOwnedAttribute().add(prop);
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
                        Operation& op = data.m_manager->create<Operation>();
                        parseOperation(node["ownedOperation"][i]["operation"], op, data);
                        dataType.getOwnedOperation().add(op);
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

void emitDataType(YAML::Emitter& emitter, DataType& dataType) {
    if (dataType.getElementType() == ElementType::DATA_TYPE) {
        emitter << YAML::BeginMap << YAML::Key << "dataType" << YAML::Value << YAML::BeginMap;
    }

    emitClassifier(emitter, dataType);
    
    if (!dataType.getOwnedAttribute().empty()) {
        emitter << YAML::Key << "ownedAttribute" << YAML::Value << YAML::BeginSeq;
        for (auto& attribute: dataType.getOwnedAttribute()) {
            emitProperty(emitter, attribute);
        }
        emitter << YAML::EndSeq;
    }

    if (!dataType.getOwnedOperation().empty()) {
        emitter << YAML::Key << "ownedOperation" << YAML::Value << YAML::BeginSeq;
        for (auto& operation : dataType.getOwnedOperation()) {
            emitOperation(emitter, operation);
        }
        emitter << YAML::EndSeq;
    }

    if (dataType.getElementType() == ElementType::DATA_TYPE) {
        emitter << YAML::EndMap << YAML::EndMap;
    }
}

void parsePrimitiveType(YAML::Node node, PrimitiveType& type, ParserMetaData& data) {
    parseDataType(node, type, data);
}

void emitPrimitiveType(YAML::Emitter& emitter, PrimitiveType& type) {
    if (type.getElementType() == ElementType::PRIMITIVE_TYPE) {
        emitter << YAML::BeginMap << YAML::Key << "primitiveType" << YAML::Value << YAML::BeginMap;
    }

    emitDataType(emitter, type);

    if (type.getElementType() == ElementType::PRIMITIVE_TYPE) {
        emitter << YAML::EndMap << YAML::EndMap;
    }
}

void parseStructuredClassifier(YAML::Node node, StructuredClassifier& clazz, ParserMetaData& data) {
    parseClassifier(node, clazz, data);

    if (node["ownedAttributes"]) {
        if (node["ownedAttributes"].IsSequence()) {
            for (size_t i=0; i<node["ownedAttributes"].size(); i++) {
                if (node["ownedAttributes"][i]["property"]) {
                    if (node["ownedAttributes"][i]["property"].IsMap()) {
                        Property& prop = data.m_manager->create<Property>();
                        parseProperty(node["ownedAttributes"][i]["property"], prop, data);
                        clazz.getOwnedAttributes().add(prop);
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

void emitStructuredClassifier(YAML::Emitter& emitter, StructuredClassifier& clazz) {

    emitClassifier(emitter, clazz);

    if (!clazz.getOwnedAttributes().empty()) {
        emitter << YAML::Key << "ownedAttributes" << YAML::Value << YAML::BeginSeq;
        for (auto& attribute : clazz.getOwnedAttributes()) {
            emitProperty(emitter, attribute);
        }
        emitter << YAML::EndSeq;
    }
}

void parseClass(YAML::Node node, Class& clazz, ParserMetaData& data) {
    parseStructuredClassifier(node, clazz, data);

    if (node["operations"]) {
        if (node["operations"].IsSequence()) {
            for (size_t i=0; i<node["operations"].size(); i++) {
                if (node["operations"][i]["operation"]) {
                    Operation& op = data.m_manager->create<Operation>();
                    parseOperation(node["operations"][i]["operation"], op, data);
                    clazz.getOperations().add(op);
                } else {
                    throw UmlParserException("Could not identify operation to parse, " + data.m_path.string() + " line " + to_string(node["operations"][i].Mark().line));
                }
            }
        } else {
            throw UmlParserException("Improper YAML node type for operations field, must be scalar, " + data.m_path.string() + " line " + to_string(node["attributes"].Mark().line));
        }
    }
}

void emitClass(YAML::Emitter& emitter, Class& clazz) {
    if (clazz.getElementType() == ElementType::CLASS) {
        emitter << YAML::BeginMap << YAML::Key << "class" << YAML::BeginMap;
    }

    emitStructuredClassifier(emitter, clazz);

    if (!clazz.getOperations().empty()) {
        emitter << YAML::Key << "operations" << YAML::Value << YAML::BeginSeq;
        for (auto& operation : clazz.getOperations()) {
            emitOperation(emitter, operation);
        }
        emitter << YAML::EndSeq;
    }

    if (clazz.getElementType() == ElementType::CLASS) {
        emitter << YAML::EndMap << YAML::EndMap;
    }
}

void parseBehavior(YAML::Node node, Behavior& bhv, ParserMetaData& data) {
    parseClass(node, bhv, data);

    if (node["parameters"]) {
        if (node["parameters"].IsSequence()) {
            for (size_t i=0; i<node["parameters"].size(); i++) {
                if (node["parameters"][i].IsMap()) {
                    if (node["parameters"][i]["parameter"]) {
                        Parameter& param = data.m_manager->create<Parameter>();
                        parseParameter(node["parameters"][i]["parameter"], param, data);
                        bhv.getParameters().add(param);
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

void emitBehavior(YAML::Emitter& emitter, Behavior& bhv) {
    emitClass(emitter, bhv);

    if (!bhv.getParameters().empty()) {
        emitter << YAML::Key << "parameters" << YAML::Value << YAML::BeginSeq;
        for (auto& param : bhv.getParameters()) {
            emitParameter(emitter, param);
        }
        emitter << YAML::EndSeq;
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
                    if (isValidID(body)) {
                        // TODO
                    } else {
                        // make literal string with value
                        LiteralString& bodyUML = data.m_manager->create<LiteralString>();
                        bodyUML.setValue(body);
                        bhv.getBodies().add(bodyUML);
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

void emitOpaqueBehavior(YAML::Emitter& emitter, OpaqueBehavior& bhv) {
    if (bhv.getElementType() == ElementType::OPAQUE_BEHAVIOR) {
        emitter << YAML::BeginMap << YAML::Key << "opaqueBehavior" << YAML::Value << YAML::BeginMap;
    }

    emitBehavior(emitter, bhv);

    if (!bhv.getBodies().empty()) {
        emitter << YAML::Key << "bodies" << YAML::Value << YAML::BeginSeq;
        for (auto& body : bhv.getBodies()) {
            emit(emitter, body);
        }
        emitter << YAML::EndSeq;
    }

    if (bhv.getElementType() == ElementType::OPAQUE_BEHAVIOR) {
        emitter << YAML::EndMap << YAML::EndMap;
    }
}

void parseProperty(YAML::Node node, Property& prop, ParserMetaData& data) {
    parseTypedElement(node, prop, data);
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
            if (node["defaultValue"]["expression"]) {
                if (node["defaultValue"]["expression"].IsMap()) {
                    Expression& newExp = data.m_manager->create<Expression>();
                    parseExpression(node["defaultValue"]["expression"], newExp, data);
                    prop.setDefaultValue(&newExp);
                } else {
                    throw UmlParserException("Invalid YAML node type for expression definition, must be map, " + data.m_path.string() + " line " + to_string(node["defaultValue"]["expression"].Mark().line));
                }
            } else if (node["defaultValue"]["literalBool"]) {
                if (node["defaultValue"]["literalBool"].IsMap()) {
                    LiteralBool& lb = data.m_manager->create<LiteralBool>();
                    parseLiteralBool(node["defaultValue"]["literalBool"], lb, data);
                    prop.setDefaultValue(&lb);
                } else {
                    throw UmlParserException("Improper YAML node type for Properties defaultValue field, " + data.m_path.string() + " line " + to_string(node["defaultValue"]["literalBool"].Mark().line));
                }
            } else if (node["defaultValue"]["literalInt"]) {
                if (node["defaultValue"]["literalInt"].IsMap()) {
                    LiteralInt& li = data.m_manager->create<LiteralInt>();
                    parseLiteralInt(node["defaultValue"]["literalInt"], li, data);
                    prop.setDefaultValue(&li);
                } else {
                    throw UmlParserException("Improper YAML node type for Properties defaultValue field, " + data.m_path.string() + " line " + to_string(node["defaultValue"]["literalInt"].Mark().line));
                }
            } else if (node["defaultValue"]["literalNull"]) {
                if (node["defaultValue"]["literalNull"].IsMap()) {
                    LiteralNull& ln = data.m_manager->create<LiteralNull>();
                    parseTypedElement(node["defaultValue"]["literalNull"], ln, data);
                    prop.setDefaultValue(&ln);
                } else {
                    throw UmlParserException("Improper YAML node type for Properties defaultValue field, " + data.m_path.string() + " line " + to_string(node["defaultValue"]["literalNull"].Mark().line));
                }
            } else if (node["defaultValue"]["literalReal"]) {
                if (node["defaultValue"]["literalReal"].IsMap()) {
                    LiteralReal& lr = data.m_manager->create<LiteralReal>();
                    parseLiteralReal(node["defaultValue"]["literalReal"], lr, data);
                    prop.setDefaultValue(&lr);
                } else {
                    throw UmlParserException("Improper YAML node type for Properties defaultValue field, " + data.m_path.string() + " line " + to_string(node["defaultValue"]["literalReal"].Mark().line));
                }
            } else if (node["defaultValue"]["literalString"]) {
                if (node["defaultValue"]["literalString"].IsMap()) {
                    LiteralString& ls = data.m_manager->create<LiteralString>();
                    parseLiteralString(node["defaultValue"]["literalString"], ls, data);
                    prop.setDefaultValue(&ls);
                } else {
                    throw UmlParserException("Improper YAML node type for Properties defaultValue field, " + data.m_path.string() + " line " + to_string(node["defaultValue"]["literalString"].Mark().line));
                }
            } else if (node["defaultValue"]["literalUnlimtedNatural"]) {
                if (node["defaultValue"]["literalUnlimitedNatural"]) {
                    LiteralUnlimitedNatural& ln = data.m_manager->create<LiteralUnlimitedNatural>();
                    parseLiteralUnlimitedNatural(node["defaultValue"]["literalUnlimitedNatural"], ln, data);
                    prop.setDefaultValue(&ln);
                } else {
                    throw UmlParserException("Improper YAML node type for Properties defaultValue field, " + data.m_path.string() + " line " + to_string(node["defaultValue"]["literalUnlimitedNatural"].Mark().line));
                }
            } else if (node["defaultValue"]["instanceValue"]) {
                if (node["defaultValue"]["instanceValue"].IsMap()) {
                    InstanceValue& iv = data.m_manager->create<InstanceValue>();
                    parseInstanceValue(node["defaultValue"]["instanceValue"], iv, data);
                    prop.setDefaultValue(&iv);
                } else {
                    throw UmlParserException("Improper YAML node type for Properties defaultValue field, " + data.m_path.string() + " line " + to_string(node["defaultValue"]["instanceValue"].Mark().line));
                }
            } else {
                throw UmlParserException("Unknown Value Specification for default value field, " + data.m_path.string() + " line " + to_string(node["defaultValue"].Mark().line));
            }
        }
    }
}

void emitProperty(YAML::Emitter& emitter, Property& prop) {
    if (prop.getElementType() == ElementType::PROPERTY) {
        emitter << YAML::BeginMap << YAML::Key << "property" << YAML::Value << YAML::BeginMap;
    }

    emitTypedElement(emitter, prop);
    emitMultiplicityElement(emitter, prop);

    if (prop.getAggregation() != AggregationKind::NONE) {
        string aggregationString;
        switch(prop.getAggregation()) {
            case AggregationKind::COMPOSITE : {
                aggregationString = "COMPOSITE";
                break;
            }
            case AggregationKind::SHARED : {
                aggregationString = "SHARED";
                break;
            }
        }
        emitter << YAML::Key << "aggregation" << YAML::Value << aggregationString;
    }

    if (prop.getDefaultValue()) {
        // TODO
    }

    if (prop.getElementType() == ElementType::PROPERTY) {
        emitter << YAML::EndMap << YAML::EndMap;
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

void emitParameter(YAML::Emitter& emitter, Parameter& el) {
    if (el.getElementType() == ElementType::PARAMETER) {
        emitter << YAML::BeginMap << YAML::Key << "parameter" << YAML::Value << YAML::BeginMap;
    }

    emitTypedElement(emitter, el);
    emitMultiplicityElement(emitter, el);

    if (el.getDirection() != ParameterDirectionKind::NONE) {
        string direction;
        switch(el.getDirection()) {
            case ParameterDirectionKind::IN : {
                direction = "IN";
                break;
            }
            case ParameterDirectionKind::INOUT : {
                direction = "INOUT";
                break;
            }
            case ParameterDirectionKind::OUT : {
                direction = "OUT";
                break;
            }
            case ParameterDirectionKind::RETURN : {
                direction = "RETURN";
                break;
            }
        }
        emitter << YAML::Key << "direction" << YAML::Value << direction;
    }

    if (el.getElementType() == ElementType::PARAMETER) {
        emitter << YAML::EndMap << YAML::EndMap;
    }
}

void parseOperation(YAML::Node node, Operation& op, ParserMetaData& data) {
    parseNamedElement(node, op, data);
    parseTemplateableElement(node, op, data);

    // TODO: maybe move all this to new function parseBehavioralFeature once the other ones are implemented
    if (node["methods"]) {
        if (node["methods"].IsSequence()) {
            for (size_t i=0; i<node["methods"].size(); i++) {
                if (node["methods"][i]["opaqueBehavior"]) {
                    OpaqueBehavior& bhv = data.m_manager->create<OpaqueBehavior>();
                    parseOpaqueBehavior(node["methods"][i]["opaqueBehavior"], bhv, data);
                    op.getMethods().add(bhv);
                } else {
                    throw UmlParserException("Invalid behavior type for operation methods, " + data.m_path.string() + " line " + to_string(node["methods"][i].Mark().line));
                } 
            }
        } else {
            throw UmlParserException("Improper YAML node type for methods, must be sequence, " + data.m_path.string() + " line " + to_string(node["methods"].Mark().line));
        }
    }

    if (node["ownedParameters"]) {
        // So how i'm currently interpreting UML, these parameters are owned by the operation, but are the same as the methods
        // TODO: Have a validator that checks that but don't think it is deal of parser to make sure that parameters are correct
        // TODO: maybe log a warning or something
        if (node["ownedParameters"].IsSequence()) {
            for (size_t i = 0; i < node["ownedParameters"].size(); i++) {
                if (node["ownedParameters"][i]["parameter"]) {
                    if (node["ownedParameters"][i]["parameter"].IsMap()) {
                        Parameter& p = data.m_manager->create<Parameter>();
                        parseParameter(node["ownedParameters"][i]["parameter"], p, data);
                        op.getOwnedParameters().add(p);
                    } else {
                        throw UmlParserException("Improper YAML Node type for parameter definition, must be map, " + data.m_path.string() + to_string(node["ownedParameters"][i]["parameter"].Mark().line));
                    }
                }
            }
        } else {
            throw UmlParserException("Improper YAML node type for ownedParameters field, must be a sequence, " + data.m_path.string() + to_string(node["ownedParameters"].Mark().line));
        }
    }
}

void emitOperation(YAML::Emitter& emitter, Operation& op) {
    if (op.getElementType() == ElementType::OPERATION) {
        emitter << YAML::BeginMap << YAML::Key << "operation" << YAML::Value << YAML::BeginMap;
    }

    emitNamedElement(emitter, op);

    if (!op.getMethods().empty()) {
        emitter << YAML::Key << "methods" << YAML::Value << YAML::BeginSeq;
        for (auto& method : op.getMethods()) {
            emit(emitter, method);
        }
        emitter << YAML::EndSeq;
    }

    if (!op.getOwnedParameters().empty()) {
        emitter << YAML::Key << "ownedParameters" << YAML::Value << YAML::BeginSeq;
        for (auto& parameter : op.getOwnedParameters()) {
            emitParameter(emitter, parameter);
        }
        emitter << YAML::EndSeq;
    }

    if (op.getElementType() == ElementType::OPERATION) {
        emitter << YAML::EndMap << YAML::EndMap;
    }
}

void parsePackage(YAML::Node node, Package& pckg, ParserMetaData& data) {

    parseNamedElement(node, pckg, data);
    parseTemplateableElement(node, pckg, data); 

    if (node["packageMerge"]) {
        if (node["packageMerge"].IsSequence()) {
            for (size_t i = 0; i < node["packageMerge"].size(); i++) {
                if (node["packageMerge"][i]["packageMerge"]) {
                    if (node["packageMerge"][i]["packageMerge"].IsMap()) {
                        PackageMerge& merge = data.m_manager->create<PackageMerge>();
                        parsePackageMerge(node["packageMerge"][i]["packageMerge"], merge, data);
                        pckg.getPackageMerge().add(merge);
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
                        Activity& activity = data.m_manager->create<Activity>();
                        // TODO parse activity
                        activity.setOwningPackage(&pckg);
                    } else if (node["packagedElements"][i]["class"]) {
                        Class& clazz = data.m_manager->create<Class>();
                        parseClass(node["packagedElements"][i]["class"], clazz, data);
                        pckg.getPackagedElements().add(clazz);
                    } else if (node["packagedElements"][i]["dataType"]) {
                        DataType& dataType = data.m_manager->create<DataType>();
                        parseDataType(node["packagedElements"][i]["dataType"], dataType, data);
                        pckg.getPackagedElements().add(dataType);
                    } else if (node["packagedElements"][i]["enumeration"]) {
                        Enumeration& enumeration = data.m_manager->create<Enumeration>();
                        parseEnumeration(node["packagedElements"][i]["enumeration"], enumeration, data);
                        pckg.getPackagedElements().add(enumeration);
                    } else if (node["packagedElements"][i]["expression"]) {
                        if (node["packagedElements"][i]["expression"].IsMap()) {
                            Expression& newExp = data.m_manager->create<Expression>();
                            parseExpression(node["packagedElements"][i]["expression"], newExp, data);
                            pckg.getPackagedElements().add(newExp);
                        } else {
                            throw UmlParserException("Invalid YAML node type for expression definition, must be map, " + data.m_path.string() + " line " + to_string(node["operands"][i]["expression"].Mark().line));
                        }
                    }else if (node["packagedElements"][i]["instanceSpecification"]) {
                        InstanceSpecification& inst = data.m_manager->create<InstanceSpecification>();
                        parseInstanceSpecification(node["packagedElements"][i]["instanceSpecification"], inst, data);
                        pckg.getPackagedElements().add(inst);
                    } else if (node["packagedElements"][i]["instanceValue"]) {
                        InstanceValue& instVal = data.m_manager->create<InstanceValue>();
                        parseInstanceValue(node["packagedElements"][i]["instanceValue"], instVal, data);
                        pckg.getPackagedElements().add(instVal);
                    } else if (node["packagedElements"][i]["literalBool"]) {
                        LiteralBool& lb = data.m_manager->create<LiteralBool>();
                        parseLiteralBool(node["packagedElements"][i]["literalBool"], lb, data);
                        pckg.getPackagedElements().add(lb);
                    } else if (node["packagedElements"][i]["literalInt"]) {
                        LiteralInt& li = data.m_manager->create<LiteralInt>();
                        parseLiteralInt(node["packagedElements"][i]["literalInt"], li, data);
                        pckg.getPackagedElements().add(li);
                    } else if (node["packagedElements"][i]["literalNull"]) {
                        LiteralNull& ln = data.m_manager->create<LiteralNull>();
                        parseTypedElement(node["packagedElements"][i]["literalNull"], ln, data);
                        pckg.getPackagedElements().add(ln); 
                    } else if (node["packagedElements"][i]["literalReal"]) {
                        LiteralReal& lr = data.m_manager->create<LiteralReal>();
                        parseLiteralReal(node["packagedElements"][i]["literalReal"], lr, data);
                        pckg.getPackagedElements().add(lr);
                    } else if (node["packagedElements"][i]["literalString"]) {
                        LiteralString& ls = data.m_manager->create<LiteralString>();
                        parseLiteralString(node["packagedElements"][i]["literalString"], ls, data);
                        pckg.getPackagedElements().add(ls);
                    } else if (node["packagedElements"][i]["literalUnlimitedNatural"]) {
                        LiteralUnlimitedNatural& ln = data.m_manager->create<LiteralUnlimitedNatural>();
                        parseLiteralUnlimitedNatural(node["packagedElements"][i]["literalUnlimitedNatural"], ln, data);
                        pckg.getPackagedElements().add(ln);
                    } else if (node["packagedElements"][i]["package"]) {
                        Package& package = data.m_manager->create<Package>();
                        parsePackage(node["packagedElements"][i]["package"], package, data);
                        package.setOwningPackage(&pckg);
                    } else if (node["packagedElements"][i]["primitiveType"]) {
                        PrimitiveType& type = data.m_manager->create<PrimitiveType>();
                        parsePrimitiveType(node["packagedElements"][i]["primitiveType"], type, data);
                        pckg.getPackagedElements().add(type);
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

void emitPackage(YAML::Emitter& emitter, Package& pckg) {
    if (pckg.getElementType() == ElementType::PACKAGE) {
        emitter << YAML::BeginMap << YAML::Key << "package" << YAML::Value << YAML::BeginMap;
    }

    emitNamedElement(emitter, pckg);

    for (auto const& pckgMerge : pckg.getPackageMerge()) {
        // TODO
    }

    if (!pckg.getPackagedElements().empty()) {
        emitter << YAML::Key << "packagedElements" << YAML::Value << YAML::BeginSeq;
        for (auto& el : pckg.getPackagedElements()) {
            emit(emitter, el);
        }
        emitter << YAML::EndSeq;
    }

    if (pckg.getElementType() == ElementType::PACKAGE) {
        emitter << YAML::EndMap << YAML::EndMap;
    }
}

void parseMultiplicityElement(YAML::Node node, MultiplicityElement& el, ParserMetaData& data) {
    if (node["lower"]) {
        if (node["lower"].IsScalar()) {
            LiteralInt& lower = data.m_manager->create<LiteralInt>();
            lower.setValue(node["lower"].as<int>());
            el.setLowerValue(&lower);
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
            LiteralInt& upper = data.m_manager->create<LiteralInt>();
            upper.setValue(node["upper"].as<int>());
            el.setUpperValue(&upper);
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

void emitMultiplicityElement(YAML::Emitter& emitter, MultiplicityElement& el) {
    if (el.getLowerValue()) {
        emitter << YAML::Key << "lower" << YAML::Value;
        emit(emitter, *el.getLowerValue());
    }

    if (el.getUpperValue()) {
        emitter << YAML::Key << "upper" << YAML::Value;
        emit(emitter, *el.getUpperValue());
    }
}

void SetClassifierFunctor::operator()(Element& el) const {
    if (el.isSubClassOf(ElementType::CLASSIFIER)) {
        dynamic_cast<InstanceSpecification*>(m_el)->setClassifier(&dynamic_cast<Classifier&>(el));
    } else {
        throw UmlParserException(m_el->getElementTypeString() + " id: " + m_el->getID().string() + 
                                 " assigned classifier is not a classifer! line " + to_string(m_node.Mark().line));
    }
}

void parseInstanceSpecification(YAML::Node node, InstanceSpecification& inst, ParserMetaData& data) {
    parseNamedElement(node, inst, data);

    if (node["classifier"]) {
        if (node["classifier"].IsScalar()) {
            string classifierID = node["classifier"].as<string>();
            if (isValidID(classifierID)) {
                ID id = ID::fromString(classifierID);
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
                        Slot& slot = data.m_manager->create<Slot>();
                        parseSlot(node["slots"][i]["slot"], slot, data);
                        inst.getSlots().add(slot);
                    }
                }
            }
        } else {
            throw UmlParserException("Invalid YAML node type for InstanceSpecification field slots, expected sequence, " + data.m_path.string() + " line " + to_string(node["slots"].Mark().line));
        }
    }
}

void emitInstanceSpecification(YAML::Emitter& emitter, InstanceSpecification& inst) {
    if (inst.getElementType() == ElementType::INSTANCE_SPECIFICATION) {
        emitter << YAML::BeginMap << YAML::Key << "instanceSpecification" << YAML::Value << YAML::BeginMap;
    }

    emitNamedElement(emitter, inst);

    if (inst.getClassifier()) {
        emitter << YAML::Key << "classifier" << YAML::Value << inst.getClassifier()->getID().string();
    }

    if (!inst.getSlots().empty()) {
        emitter << YAML::Key << "slots" << YAML::Value << YAML::BeginSeq;
        for (auto& slot : inst.getSlots()) {
            emitSlot(emitter, slot);
        }
    }

    if (inst.getElementType() == ElementType::INSTANCE_SPECIFICATION) {
        emitter << YAML::EndMap << YAML::EndMap;
    }
}

void SetDefiningFeatureFunctor::operator()(Element& el) const {
    if (el.isSubClassOf(ElementType::STRUCTURAL_FEATURE)) {
        dynamic_cast<Slot*>(m_el)->setDefiningFeature(&dynamic_cast<StructuralFeature&>(el));
    } else {
        throw UmlParserException(m_el->getElementTypeString() + " id: " + m_el->getID().string() + 
                                 " assigned definingFeature is not a structuralFeature! line " + to_string(m_node.Mark().line));
    }
}

void parseSlot(YAML::Node node, Slot& slot, ParserMetaData& data) {
    parseElement(node, slot, data);

    if (node["definingFeature"]) {
        if (node["definingFeature"].IsScalar()) {
            string stringID = node["definingFeature"].as<string>();
            if (isValidID(stringID)) {
                ID definingFeatureID = ID::fromString(stringID);
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
                if (node["values"][i]["expression"]) {
                    if (node["values"][i]["expression"].IsMap()) {
                        Expression& newExp = data.m_manager->create<Expression>();
                        parseExpression(node["values"][i]["expression"], newExp, data);
                        slot.getValues().add(newExp);
                    } else {
                        throw UmlParserException("Invalid YAML node type for expression definition, must be map, " + data.m_path.string() + " line " + to_string(node["values"][i]["expression"].Mark().line));
                    }
                } else if (node["values"][i]["literalBool"]) {
                    if (node["values"][i]["literalBool"].IsMap()) {
                        LiteralBool& lb = data.m_manager->create<LiteralBool>();
                        parseLiteralBool(node["values"][i]["literalBool"], lb, data);
                        slot.getValues().add(lb);
                    } else {
                        throw UmlParserException("Improper YAML node type for Expression values field, " + data.m_path.string() + " line " + to_string(node["values"][i]["literalBool"].Mark().line));
                    }
                } else if (node["values"][i]["literalInt"]) {
                    if (node["values"][i]["literalInt"].IsMap()) {
                        LiteralInt& li = data.m_manager->create<LiteralInt>();
                        parseLiteralInt(node["values"][i]["literalInt"], li, data);
                        slot.getValues().add(li);
                    } else {
                        throw UmlParserException("Improper YAML node type for LiteralInt values field, " + data.m_path.string() + " line " + to_string(node["values"][i]["literalInt"].Mark().line));
                    }
                } else if (node["values"][i]["literalReal"]) {
                    if (node["values"][i]["literalReal"].IsMap()) {
                        LiteralReal& lr = data.m_manager->create<LiteralReal>();
                        parseLiteralReal(node["values"][i]["literalReal"], lr, data);
                        slot.getValues().add(lr);
                    } else {
                        throw UmlParserException("Improper YAML node type for LiteralReal values field, " + data.m_path.string() + " line " + to_string(node["values"][i]["literalReal"].Mark().line));
                    }
                } else if (node["values"][i]["literalString"]) {
                    if (node["values"][i]["literalString"].IsMap()) {
                        LiteralString& ls = data.m_manager->create<LiteralString>();
                        parseLiteralString(node["values"][i]["literalString"], ls, data);
                        slot.getValues().add(ls);
                    } else {
                        throw UmlParserException("Improper YAML node type for Expression values field, " + data.m_path.string() + " line " + to_string(node["values"][i]["literalString"].Mark().line));
                    }
                } else if (node["values"][i]["literalNull"]) {
                    if (node["values"][i]["literalNull"].IsMap()) {
                        LiteralNull& ln = data.m_manager->create<LiteralNull>();
                        parseTypedElement(node["values"][i]["literalNull"], ln, data);
                        slot.getValues().add(ln);
                    } else {
                        throw UmlParserException("Improper YAML node type for LiteralNull values field, " + data.m_path.string() + " line " + to_string(node["values"][i]["literalNull"].Mark().line));
                    }
                } else if (node["values"][i]["literalUnlimitedNatural"]) {
                    if (node["values"][i]["literalUnlimitedNatural"].IsMap()) {
                        LiteralUnlimitedNatural& ln = data.m_manager->create<LiteralUnlimitedNatural>();
                        parseLiteralUnlimitedNatural(node["values"][i]["literalUnlimitedNatural"], ln, data);
                        slot.getValues().add(ln);
                    } else {
                        throw UmlParserException("Improper YAML node type for LiteralUnlimitedNatural values field, " + data.m_path.string() + " line " + to_string(node["values"][i]["literalUnlimitedNatural"].Mark().line));
                    }
                } else if (node["values"][i]["instanceValue"]) {
                    if (node["values"][i]["instanceValue"].IsMap()) {
                        InstanceValue& instVal = data.m_manager->create<InstanceValue>();
                        parseInstanceValue(node["values"][i]["instanceValue"], instVal, data);
                        slot.getValues().add(instVal);
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

void emitSlot(YAML::Emitter& emitter, Slot& slot) {
    if (slot.getElementType() == ElementType::SLOT) {
        emitter << YAML::BeginMap << YAML::Key << "slot" << YAML::Value << YAML::BeginMap;
    }

    emitElement(emitter, slot);

    if (slot.getDefiningFeature()) {
        emitter << YAML::Key << "definingFeature" << YAML::Value << slot.getDefiningFeature()->getID().string();
    }

    if (!slot.getValues().empty()) {
        emitter << YAML::Key << "values" << YAML::Value << YAML::BeginSeq;
        for (auto& val : slot.getValues()) {
            emit(emitter, val);
        }
        emitter << YAML::EndSeq;
    }

    if (slot.getElementType() == ElementType::SLOT) {
        emitter << YAML::EndMap << YAML::EndMap;
    }
}

void parseEnumeration(YAML::Node node, Enumeration& enumeration, ParserMetaData& data) {
    parseDataType(node, enumeration, data);

    if (node["ownedLiteral"]) {
        if (node["ownedLiteral"].IsSequence()) {
            for (size_t i = 0; i < node["ownedLiteral"].size(); i++) {
                if (node["ownedLiteral"][i]["enumerationLiteral"]) {
                    if (node["ownedLiteral"][i]["enumerationLiteral"].IsMap()) {
                        EnumerationLiteral& literal = data.m_manager->create<EnumerationLiteral>();
                        parseEnumerationLiteral(node["ownedLiteral"][i]["enumerationLiteral"], literal, data);
                        enumeration.getOwnedLiteral().add(literal);
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

void emitEnumeration(YAML::Emitter& emitter, Enumeration& enumeration) {
    if (enumeration.getElementType() == ElementType::ENUMERATION) {
        emitter << YAML::BeginMap << YAML::Key << "enumeration" << YAML::Value << YAML::BeginMap;
    }

    emitDataType(emitter, enumeration);

    if (!enumeration.getOwnedLiteral().empty()) {
        emitter << YAML::Key << "ownedLiteral" << YAML::BeginSeq;
        for (auto& literal : enumeration.getOwnedLiteral()) {
            emitEnumerationLiteral(emitter, literal);
        }
        emitter << YAML::EndSeq;
    }

    if (enumeration.getElementType() == ElementType::ENUMERATION) {
        emitter << YAML::EndMap << YAML::EndMap;
    }
}

void parseEnumerationLiteral(YAML::Node node, EnumerationLiteral& literal, ParserMetaData& data) {
    parseInstanceSpecification(node, literal, data);
}

void emitEnumerationLiteral(YAML::Emitter& emitter, EnumerationLiteral& literal) {
    if (literal.getElementType() == ElementType::ENUMERATION_LITERAL) {
        emitter << YAML::BeginMap << YAML::Key << "enumerationLiteral" << YAML::Value << YAML::BeginMap;
    }

    emitInstanceSpecification(emitter, literal);

    if (literal.getElementType() == ElementType::ENUMERATION_LITERAL) {
        emitter << YAML::EndMap << YAML::EndMap;
    }
}

void SetInstanceFunctor::operator()(Element& el) const {
    if (el.isSubClassOf(ElementType::INSTANCE_SPECIFICATION)) {
        dynamic_cast<InstanceValue*>(m_el)->setInstance(&dynamic_cast<InstanceSpecification&>(el));
    } else {
        throw UmlParserException(m_el->getElementTypeString() + " id: " + m_el->getID().string() + 
                                 " assigned instance is not an instanceSpecification! line " + to_string(m_node.Mark().line));
    }
}

void parseInstanceValue(YAML::Node node, InstanceValue& val, ParserMetaData& data) {
    parseTypedElement(node, val, data);

    if (node["instance"]) {
        if (node["instance"].IsScalar()) {
            string instID = node["instance"].as<string>();
            if (isValidID(instID)) {
                ID id = ID::fromString(instID);
                applyFunctor(data, id, new SetInstanceFunctor(&val, node["instance"]));
            } else {
                throw UmlParserException("Scalar YAML node for InstanceValue field instance is not a valid uuid4, " + data.m_path.string() + " line " + to_string(node["instance"].Mark().line));
            }
        } else {
            throw UmlParserException("Invalid YAML node type for InstanceValue field instance, expect scalar, " + data.m_path.string() + " line " + to_string(node["instance"].Mark().line));
        }
    }
}

void emitInstanceValue(YAML::Emitter& emitter, InstanceValue& val) {
    if (val.getElementType() == ElementType::INSTANCE_VALUE) {
        emitter << YAML::BeginMap << YAML::Key << "instanceValue" << YAML::Value << YAML::BeginMap;
    }

    emitTypedElement(emitter, val);

    if (val.getInstance()) {
        emitter << YAML::Key << "instance" << YAML::Value << val.getInstance()->getID().string();
    }

    if (val.getElementType() == ElementType::INSTANCE_VALUE) {
        emitter << YAML::EndMap << YAML::EndMap;
    }
}

void SetMergedPackageFunctor::operator()(Element& el) const {
    if (el.isSubClassOf(ElementType::PACKAGE)) {
        dynamic_cast<PackageMerge*>(m_el)->setMergedPackage(&dynamic_cast<Package&>(el));
    } else {
        throw UmlParserException(m_el->getElementTypeString() + " id: " + m_el->getID().string() + 
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
            if (isValidID(pckgString)) {
                ID pckgID = ID::fromString(pckgString);
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

void emitPackageMerge(YAML::Emitter& emitter, PackageMerge& merge) {
    if (merge.getElementType() == ElementType::PACKAGE_MERGE) {
        emitter << YAML::BeginMap << YAML::Key << "package" << YAML::Value << YAML::BeginMap;
    }

    emitElement(emitter, merge);

    if (merge.getMergedPackage()) {
        // TODO
    }

    if (merge.getElementType() == ElementType::PACKAGE_MERGE) {
        emitter << YAML::EndMap << YAML::EndMap;
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

void emitLiteralBool(YAML::Emitter& emitter, LiteralBool& lb) {
    if (lb.getElementType() == ElementType::LITERAL_BOOL) {
        emitter << YAML::BeginMap << YAML::Key << "literalBool" << YAML::Value << YAML::BeginMap;
    }

    emitTypedElement(emitter, lb);

    emitter << YAML::Key << "value" << YAML::Value << lb.getValue();

    if (lb.getElementType() == ElementType::LITERAL_BOOL) {
        emitter << YAML::EndMap << YAML::EndMap;
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

void emitLiteralInt(YAML::Emitter& emitter, LiteralInt& li) {
    if (li.getElementType() == ElementType::LITERAL_INT) {
        emitter << YAML::BeginMap << YAML::Key << "literalInt" << YAML::Value << YAML::BeginMap;
    }

    emitTypedElement(emitter, li);

    emitter << YAML::Key << "value" << YAML::Value << li.getValue();

    if (li.getElementType() == ElementType::LITERAL_INT) {
        emitter << YAML::EndMap << YAML::EndMap;
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

void emitLiteralReal(YAML::Emitter& emitter, LiteralReal& lr) {
    if (lr.getElementType() == ElementType::LITERAL_REAL) {
        emitter << YAML::BeginMap << YAML::Key << "literalReal" << YAML::Value << YAML::BeginMap;
    }

    emitTypedElement(emitter, lr);

    emitter << YAML::Key << "value" << YAML::Value << lr.getValue();

    if (lr.getElementType() == ElementType::LITERAL_REAL) {
        emitter << YAML::EndMap << YAML::EndMap;
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

void emitLiteralString(YAML::Emitter& emitter, LiteralString& ls) {
    if (ls.getElementType() == ElementType::LITERAL_STRING) {
        emitter << YAML::BeginMap << YAML::Key << "literalString" << YAML::Value << YAML::BeginMap;
    }

    emitTypedElement(emitter, ls);

    if (!ls.getValue().empty()) {
        emitter << YAML::Key << "value" << YAML::Value << ls.getValue();
    }

    if (ls.getElementType() == ElementType::LITERAL_STRING) {
        emitter << YAML::EndMap << YAML::EndMap;
    }
}

void parseLiteralUnlimitedNatural(YAML::Node node, LiteralUnlimitedNatural& ln, ParserMetaData& data) {
    parseTypedElement(node, ln, data);

    if (node["value"]) {
        if (node["value"].IsScalar()) {
            string val = node["value"].as<string>();
            if (val.compare("*") == 0) {
                ln.setInfinite();
            } else {
                ln.setNumberValue(node["value"].as<unsigned long>());
            }
        } else {
            throw UmlParserException("LiteralUnlimitedNatural value field must be a scalar!");
        }
    }
}

void emitLiteralUnlimitedNatural(YAML::Emitter& emitter, LiteralUnlimitedNatural& ln) {
    if (ln.getElementType() == ElementType::LITERAL_UNLIMITED_NATURAL) {
        emitter << YAML::BeginMap << YAML::Key << "literalUnlimitedNatural" << YAML::Value << YAML::BeginMap;
    }

    emitTypedElement(emitter, ln);

    if (ln.isInfinite()) {
        emitter << YAML::Key << "value" << YAML::Value << "*";
    } else {
        emitter << YAML::Key << "value" << YAML::Value << ln.getNumberValue();
    }

    if (ln.getElementType() == ElementType::LITERAL_UNLIMITED_NATURAL) {
        emitter << YAML::EndMap << YAML::EndMap;
    }
}

void parseExpression(YAML::Node node, Expression& exp, ParserMetaData& data) {
    parseTypedElement(node, exp, data);

    if (node["symbol"]) {
        if (node["symbol"].IsScalar()) {
            exp.setSymbol(node["symbol"].as<string>());
        } else {
            throw UmlParserException("Invalid YAML node type for Expression field symbol, must be scalar, " + data.m_path.string() + " line " + to_string(node["symbol"].Mark().line));
        }
    }

    if (node["operands"]) {
        if (node["operands"].IsSequence()) {
            for (size_t i = 0; i < node["operands"].size(); i++) {
                if (node["operands"][i]["expression"]) {
                    if (node["operands"][i]["expression"].IsMap()) {
                        Expression& newExp = data.m_manager->create<Expression>();
                        parseExpression(node["operands"][i]["expression"], newExp, data);
                        exp.getOperands().add(newExp);
                    } else {
                        throw UmlParserException("Invalid YAML node type for expression definition, must be map, " + data.m_path.string() + " line " + to_string(node["operands"][i]["expression"].Mark().line));
                    }
                } else if (node["operands"][i]["literalBool"]) {
                    if (node["operands"][i]["literalBool"].IsMap()) {
                        LiteralBool& lb = data.m_manager->create<LiteralBool>();
                        parseLiteralBool(node["operands"][i]["literalBool"], lb, data);
                        exp.getOperands().add(lb);
                    } else {
                        throw UmlParserException("Improper YAML node type for Expression operands field, " + data.m_path.string() + " line " + to_string(node["operands"][i]["literalBool"].Mark().line));
                    }
                } else if (node["operands"][i]["literalInt"]) {
                    if (node["operands"][i]["literalInt"].IsMap()) {
                        LiteralInt& li = data.m_manager->create<LiteralInt>();
                        parseLiteralInt(node["operands"][i]["literalInt"], li, data);
                        exp.getOperands().add(li);
                    } else {
                        throw UmlParserException("Improper YAML node type for Expression operands field, " + data.m_path.string() + " line " + to_string(node["operands"][i]["literalInt"].Mark().line));
                    }
                } else if (node["operands"][i]["literalReal"]) {
                    if (node["operands"][i]["literalReal"].IsMap()) {
                        LiteralReal& lr = data.m_manager->create<LiteralReal>();
                        parseLiteralReal(node["operands"][i]["literalReal"], lr, data);
                        exp.getOperands().add(lr);
                    } else {
                        throw UmlParserException("Improper YAML node type for Expression operands field, " + data.m_path.string() + " line " + to_string(node["operands"][i]["literalReal"].Mark().line));
                    }
                } else if (node["operands"][i]["literalString"]) {
                    if (node["operands"][i]["literalString"].IsMap()) {
                        LiteralString& ls = data.m_manager->create<LiteralString>();
                        parseLiteralString(node["operands"][i]["literalString"], ls, data);
                        exp.getOperands().add(ls);
                    } else {
                        throw UmlParserException("Improper YAML node type for Expression operands field, " + data.m_path.string() + " line " + to_string(node["operands"][i]["literalString"].Mark().line));
                    }
                } else if (node["operands"][i]["instanceValue"]) {
                    if (node["operands"][i]["instanceValue"].IsMap()) {
                        InstanceValue& iv = data.m_manager->create<InstanceValue>();
                        parseInstanceValue(node["operands"][i]["instanceValue"], iv, data);
                        exp.getOperands().add(iv);
                    } else {
                        throw UmlParserException("Improper YAML node type for Expression operands field, " + data.m_path.string() + " line " + to_string(node["operands"][i]["instanceValue"].Mark().line));
                    }
                } else {
                    throw UmlParserException("Unknown Value Specification for operand value field, " + data.m_path.string() + " line " + to_string(node["operands"][i].Mark().line));
                }
            }
        } else {
            throw UmlParserException("Invalid YAML node type for Expression field operands, must be sequence, " + data.m_path.string() + " line " + to_string(node["operands"].Mark().line));
        }
    }
}

void emitExpression(YAML::Emitter& emitter, Expression& exp) {
    if (exp.getElementType() == ElementType::EXPRESSION) {
        emitter << YAML::BeginMap << YAML::Key << "expression" << YAML::Value << YAML::BeginMap;
    }

    emitTypedElement(emitter, exp);

    if (!exp.getSymbol().empty()) {
        emitter << YAML::Key << "symbol" << YAML::Value << exp.getSymbol();
    }

    if (!exp.getOperands().empty()) {
        emitter << YAML::Key << "operands" << YAML::Value << YAML::BeginSeq;
        for (auto& operand : exp.getOperands()) {
            emit(emitter, operand);
        }
        emitter << YAML::EndSeq;
    }

    if (exp.getElementType() == ElementType::EXPRESSION) {
        emitter << YAML::EndMap << YAML::EndMap;
    }
}

void parseTemplateableElement(YAML::Node node, TemplateableElement& el, ParserMetaData& data) {
    if (node["templateSignature"]) {
        if (node["templateSignature"].IsMap()) {
            TemplateSignature& signature = data.m_manager->create<TemplateSignature>();
            parseTemplateSignature(node["templateSignature"], signature, data);
            el.setOwnedTemplateSignature(&signature);
        } else {
            throw UmlParserException("Invalid node type fore templateSignature, must be map " + data.m_path.string() + " line " + to_string(node["templateSignature"].Mark().line));
        }
    }

    if (node["templateBinding"]) {
        if (node["templateBinding"].IsMap()) {
            TemplateBinding& binding = data.m_manager->create<TemplateBinding>();
            parseTemplateBinding(node["templateBinding"], binding, data);
            el.setTemplateBinding(&binding);
        } else {
            throw UmlParserException("Invalid YAML node for templateBinding definition, " + data.m_path.string() + " line " + to_string(node["templateBinding"].Mark().line));
        }
    }
}

void AddTemplateParmeterFunctor::operator()(Element& el) const {
    if (el.isSubClassOf(ElementType::TEMPLATE_PARAMETER)) {
        dynamic_cast<TemplateSignature*>(m_el)->getParameter().add(dynamic_cast<TemplateParameter&>(el));
    } else {
        throw UmlParserException("Tried to add parameter to signature that wasn't a parameter! ");
    }
}

void parseTemplateSignature(YAML::Node node, TemplateSignature& signature, ParserMetaData& data) {
    parseElement(node, signature, data);

    if (node["ownedParameters"]) {
        if (node["ownedParameters"].IsSequence()) {
            for (size_t i = 0; i < node["ownedParameters"].size(); i++) {
                if (node["ownedParameters"][i]["templateParameter"]) {
                    if (node["ownedParameters"][i]["templateParameter"].IsMap()) {
                        TemplateParameter& parameter = data.m_manager->create<TemplateParameter>();
                        parseTemplateParameter(node["ownedParameters"][i]["templateParameter"], parameter, data);
                        signature.getOwnedParameter().add(parameter);
                    }
                } else {
                    throw UmlParserException("Invalid node type, templateParameter must be of node type map " + data.m_path.string() + to_string(node["ownedParameters"][i]["templateParameter"].Mark().line));
                }
            }
        } else {
            throw UmlParserException("Invalid node type, ownedParameters must be defined in Sequence Node type " + data.m_path.string() + " line " + to_string(node["ownedParameters"].Mark().line));
        }
    }

    if (node["parameters"]) {
        if (node["parameters"].IsSequence()) {
            for (size_t i = 0; i < node["parameters"].size(); i++) {
                if (node["parameters"][i].IsScalar()) {
                    if (isValidID(node["parameters"][i].as<string>())) {
                        applyFunctor(data, ID::fromString(node["parameters"][i].as<string>()), new AddTemplateParmeterFunctor(&signature, node["parameters"]));
                    }
                }
            }
        } else {
            throw UmlParserException("Invalid node type for template signature parameters, should be a sequence, " + data.m_path.string() + to_string(node["parameters"].Mark().line));
        }
    }
}

void SetParameteredElementFunctor::operator()(Element& el) const {
    if (el.isSubClassOf(ElementType::PARAMETERABLE_ELEMENT)) {
        dynamic_cast<TemplateParameter*>(m_el)->setParameteredElement(dynamic_cast<ParameterableElement*>(&el));
    } else {
        throw UmlParserException("Tried to assign non-parameterable element to TemplateParameter parameteredElement!");
    }
}

void parseTemplateParameter(YAML::Node node, TemplateParameter& parameter, ParserMetaData& data) {
    parseElement(node, parameter, data);

    if (node["ownedParameteredElement"]) {
        if (node["ownedParameteredElement"]["activity"]) {
            // TODO
        } else if (node["ownedParameteredElement"]["association"]) {
            // TODO
        } else if (node["ownedParameteredElement"]["class"]) {
            if (node["ownedParameteredElement"]["class"].IsMap()) {
                Class& clazz = data.m_manager->create<Class>();
                parseClass(node["ownedParameteredElement"]["class"], clazz, data);
                parameter.setOwnedParameteredElement(&clazz);
            } else {
                throw UmlParserException("class definition must be of node type Map! " + data.m_path.string() + " line " + to_string(node["ownedParameteredElement"]["class"].Mark().line));
            }
        } else if (node["ownedParameteredElement"]["dataType"]) {
            if (node["ownedParameteredElement"]["dataType"].IsMap()) {
                DataType& dataType = data.m_manager->create<DataType>();
                parseDataType(node["ownedParameteredElement"]["dataType"], dataType, data);
                parameter.setOwnedParameteredElement(&dataType);
            } else {
                throw UmlParserException("dataType definition must be of node type Map! " + data.m_path.string() + " line " + to_string(node["ownedParameteredElement"]["dataType"].Mark().line));
            }
        } else if (node["ownedParameteredElement"]["enumeration"]) {
            if (node["ownedParameteredElement"]["enumeration"].IsMap()) {
                Enumeration& enumeration = data.m_manager->create<Enumeration>();
                parseEnumeration(node["ownedParameteredElement"]["enumeration"], enumeration, data);
                parameter.setOwnedParameteredElement(&enumeration);
            } else {
                throw UmlParserException("enumeration definition must be of node type Map! " + data.m_path.string() + " line " + to_string(node["ownedParameteredElement"]["enumeration"].Mark().line));
            }
        } else if (node["ownedParameteredElement"]["enumerationLiteral"]) {
            if (node["ownedParameteredElement"]["enumerationLiteral"].IsMap()) {
                EnumerationLiteral& literal = data.m_manager->create<EnumerationLiteral>();
                parseEnumerationLiteral(node["ownedParameteredElement"]["enumerationLiteral"], literal, data);
                parameter.setOwnedParameteredElement(&literal);
            } else {
                throw UmlParserException("enumerationLiteral definition must be of node type Map! " + data.m_path.string() + " line " + to_string(node["ownedParameteredElement"]["enumerationLiteral"].Mark().line));
            }
        } else if (node["ownedParameteredElement"]["instanceSpecification"]) {
            if (node["ownedParameteredElement"]["instanceSpecification"].IsMap()) {
                InstanceSpecification& inst = data.m_manager->create<InstanceSpecification>();
                parseInstanceSpecification(node["ownedParameteredElement"]["instanceSpecification"], inst, data);
                parameter.setOwnedParameteredElement(&inst);
            } else {
                throw UmlParserException("instanceSpecification definition must be of node type Map! " + data.m_path.string() + " line " + to_string(node["ownedParameteredElement"]["instanceSpecification"].Mark().line));
            }
        } else if (node["ownedParameteredElement"]["instanceValue"]) {
            if (node["ownedParameteredElement"]["instanceValue"].IsMap()) {
                InstanceValue& inst = data.m_manager->create<InstanceValue>();
                parseInstanceValue(node["ownedParameteredElement"]["instanceValue"], inst, data);
                parameter.setOwnedParameteredElement(&inst);
            } else {
                throw UmlParserException("instanceValue definition must be of node type Map! " + data.m_path.string() + " line " + to_string(node["ownedParameteredElement"]["instanceValue"].Mark().line));
            }
        } else if (node["ownedParameteredElement"]["literalBool"]) {
            if (node["ownedParameteredElement"]["literalBool"].IsMap()) {
                LiteralBool& lb = data.m_manager->create<LiteralBool>();
                parseLiteralBool(node["ownedParameteredElement"]["literalBool"], lb, data);
                parameter.setOwnedParameteredElement(&lb);
            } else {
                throw UmlParserException("literalBool definition must be of node type Map! " + data.m_path.string() + " line " + to_string(node["ownedParameteredElement"]["literalBool"].Mark().line));
            }
        } else if (node["ownedParameteredElement"]["literalInt"]) {
            if (node["ownedParameteredElement"]["literalInt"].IsMap()) {
                LiteralInt& li = data.m_manager->create<LiteralInt>();
                parseLiteralInt(node["ownedParameteredElement"]["literalInt"], li, data);
                parameter.setOwnedParameteredElement(&li);
            } else {
                throw UmlParserException("literalInt definition must be of node type Map! " + data.m_path.string() + " line " + to_string(node["ownedParameteredElement"]["literalInt"].Mark().line));
            }
        } else if (node["ownedParameteredElement"]["literalNull"]) {
            if (node["ownedParameteredElement"]["literalNull"].IsMap()) {
                LiteralNull& ln = data.m_manager->create<LiteralNull>();
                parseTypedElement(node["ownedParameteredElement"]["literalNull"], ln, data);
                parameter.setOwnedParameteredElement(&ln);
            } else {
                throw UmlParserException("literalNull definition must be of node type Map! " + data.m_path.string() + " line " + to_string(node["ownedParameteredElement"]["literalNull"].Mark().line));
            }
        } else if (node["ownedParameteredElement"]["literalString"]) {
            if (node["ownedParameteredElement"]["literalString"].IsMap()) {
                LiteralString& ls = data.m_manager->create<LiteralString>();
                parseLiteralString(node["ownedParameteredElement"]["literalString"], ls, data);
                parameter.setOwnedParameteredElement(&ls);
            } else {
                throw UmlParserException("literalString definition must be of node type Map! " + data.m_path.string() + " line " + to_string(node["ownedParameteredElement"]["literalString"].Mark().line));
            }
        } else if (node["ownedParameteredElement"]["literalUnlimitedNatural"]) {
            if (node["ownedParameteredElement"]["literalUnlimitedNatural"].IsMap()) {
                LiteralUnlimitedNatural& lu = data.m_manager->create<LiteralUnlimitedNatural>();
                parseLiteralUnlimitedNatural(node["ownedParameteredElement"]["literalUnlimitedNatural"], lu, data);
                parameter.setOwnedParameteredElement(&lu);
            } else {
                throw UmlParserException("literalUnlimitedNatural definition must be of node type Map! " + data.m_path.string() + " line " + to_string(node["ownedParameteredElement"]["literalUnlimitedNatural"].Mark().line));
            }
        } else if (node["ownedParameteredElement"]["model"]) {
            if (node["ownedParameteredElement"]["model"].IsMap()) {
                Model& m = data.m_manager->create<Model>();
                parsePackage(node["ownedParameteredElement"]["model"], m, data);
                parameter.setOwnedParameteredElement(&m);
            } else {
                throw UmlParserException("model definition must be of node type Map! " + data.m_path.string() + " line " + to_string(node["ownedParameteredElement"]["model"].Mark().line));
            }
        } else if (node["ownedParameteredElement"]["opaqueBehavior"]) {
            if (node["ownedParameteredElement"]["opaqueBehavior"].IsMap()) {
                OpaqueBehavior& ob = data.m_manager->create<OpaqueBehavior>();
                parseOpaqueBehavior(node["ownedParameteredElement"]["opaqueBehavior"], ob, data);
                parameter.setOwnedParameteredElement(&ob);
            } else {
                throw UmlParserException("opaqueBehavior definition must be of node type Map! " + data.m_path.string() + " line " + to_string(node["ownedParameteredElement"]["opaqueBehavior"].Mark().line));
            }
        } else if (node["ownedParameteredElement"]["operation"]) {
            if (node["ownedParameteredElement"]["operation"].IsMap()) {
                Operation& op = data.m_manager->create<Operation>();
                parseOperation(node["ownedParameteredElement"]["operation"], op, data);
                parameter.setOwnedParameteredElement(&op);
            } else {
                throw UmlParserException("operation definition must be of node type Map! " + data.m_path.string() + " line " + to_string(node["ownedParameteredElement"]["operation"].Mark().line));
            }
        } else if (node["ownedParameteredElement"]["package"]) {
            if (node["ownedParameteredElement"]["package"].IsMap()) {
                Package& pckg = data.m_manager->create<Package>();
                parsePackage(node["ownedParameteredElement"]["package"], pckg, data);
                parameter.setOwnedParameteredElement(&pckg);
            } else {
                throw UmlParserException("package definition must be of node type Map! " + data.m_path.string() + " line " + to_string(node["ownedParameteredElement"]["package"].Mark().line));
            }
        } else if (node["ownedParameteredElement"]["parameter"]) {
            if (node["ownedParameteredElement"]["parameter"].IsMap()) {
                Parameter& param = data.m_manager->create<Parameter>();
                parseParameter(node["ownedParameteredElement"]["parameter"], param, data);
                parameter.setOwnedParameteredElement(&param);
            } else {
                throw UmlParserException("parameter definition must be of node type Map! " + data.m_path.string() + " line " + to_string(node["ownedParameteredElement"]["parameter"].Mark().line));
            }
        } else if (node["ownedParameteredElement"]["primitiveType"]) {
            if (node["ownedParameteredElement"]["primitiveType"].IsMap()) {
                PrimitiveType& prim = data.m_manager->create<PrimitiveType>();
                parsePrimitiveType(node["ownedParameteredElement"]["primitiveType"], prim, data);
                parameter.setOwnedParameteredElement(&prim);
            } else {
                throw UmlParserException("primitiveType definition must be of node type Map! " + data.m_path.string() + " line " + to_string(node["ownedParameteredElement"]["primitiveType"].Mark().line));
            }
        } else if (node["ownedParameteredElement"]["property"]) {
            if (node["ownedParameteredElement"]["property"].IsMap()) {
                Property& prop = data.m_manager->create<Property>();
                parseProperty(node["ownedParameteredElement"]["property"], prop, data);
                parameter.setOwnedParameteredElement(&prop);
            } else {
                throw UmlParserException("property definition must be of node type Map! " + data.m_path.string() + " line " + to_string(node["ownedParameteredElement"]["property"].Mark().line));
            }
        } 
    }

    if (node["parameteredElement"]) {
        if (node["parameteredElement"].IsScalar()) {
            if (isValidID(node["parameteredElement"].as<string>())) {
                applyFunctor(data, ID::fromString(node["parameteredElement"].as<string>()), new SetParameteredElementFunctor(&parameter, node["parameteredElement"]));
            } else {
                throw UmlParserException("Invalid id for parametered element, must be base64 url safe 28 character string! " + data.m_path.string());
            }
        } else {
            throw UmlParserException("Invalid YAML node type for parameteredElement, must be scalar " + data.m_path.string() + " line " + to_string(node["parameteredElement"].Mark().line));
        }
    }
}

void SetSignatureFunctor::operator()(Element& el) const {
    if (el.isSubClassOf(ElementType::TEMPLATE_SIGNATURE)) {
        dynamic_cast<TemplateBinding*>(m_el)->setSignature(dynamic_cast<TemplateSignature*>(&el));
    } else {
        throw UmlParserException("Tried to set binding signature to not signature, line " + to_string(m_node.Mark().line));
    }
}

void parseTemplateBinding(YAML::Node node, TemplateBinding& binding, ParserMetaData& data) {
    parseElement(node, binding, data);

    if (node["signature"]) {
        if (node["signature"].IsScalar()) {
            if (isValidID(node["signature"].as<string>())) {
                applyFunctor(data, ID::fromString(node["signature"].as<string>()), new SetSignatureFunctor(&binding, node["signature"]));
            } else {
                throw UmlParserException("TemplateBinding signature not a valid id, must be base64 url safe 28 character string " + data.m_path.string() + " line " + to_string(node["signature"].Mark().line));
            }
        } else {
            throw UmlParserException("Invalid yaml node type for templateBinding signature, must be scaler, " + data.m_path.string() + " line " + to_string(node["signature"].Mark().line));
        }
    }

    if (node["parameterSubstitution"]) {
        if (node["parameterSubstitution"].IsSequence()) {
            for (size_t i = 0; i < node["parameterSubstitution"].size(); i++) {
                if (node["parameterSubstitution"][i]["templateParameterSubstitution"]) {
                    if (node["parameterSubstitution"][i]["templateParameterSubstitution"].IsMap()) {
                        TemplateParameterSubstitution& sub = data.m_manager->create<TemplateParameterSubstitution>();
                        parseTemplateParameterSubstitution(node["parameterSubstitution"][i]["templateParameterSubstitution"], sub, data);
                        binding.getParameterSubstitution().add(sub);
                    } else {
                        throw UmlParserException("TODO");
                    }
                } else {
                    throw UmlParserException("ERROR, templateParameterSubstitution must be defined within parameterSubstitution sequence, " + data.m_path.string() + " line " + to_string(node["signature"].Mark().line));
                }
            }
        } else {
            throw UmlParserException("Invalid YAML node type, must be sequence, " + data.m_path.string() + " line " + to_string(node["parameterSubstitution"].Mark().line));
        }
    }
}

void SetFormalFunctor::operator()(Element& el) const {
    if (el.isSubClassOf(ElementType::TEMPLATE_PARAMETER)) {
        dynamic_cast<TemplateParameterSubstitution*>(m_el)->setFormal(dynamic_cast<TemplateParameter*>(&el));
    } else {
        throw UmlParserException("TemplateParameterSubstitution formal must be a templateParameter " + to_string(m_node.Mark().line));
    }
};

void parseTemplateParameterSubstitution(YAML::Node node, TemplateParameterSubstitution& sub, ParserMetaData& data) {
    parseElement(node, sub, data);

    if (node["formal"]) {
        if (node["formal"].IsScalar()) {
            if (isValidID(node["formal"].as<string>())) {
                applyFunctor(data, ID::fromString(node["formal"].as<string>()), new SetFormalFunctor(&sub, node["formal"]));
            }
        } else {
            throw UmlParserException("Invalid YAML node type, must be scalar for formal, " + data.m_path.string() + " line " + to_string(node["formal"].Mark().line));
        }
    }
    
    if (node["ownedActual"]) {
        // TODO
    }

    if (node["actual"]) {

    }
}

}

}
}