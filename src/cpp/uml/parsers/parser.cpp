#include "uml/parsers/parser.h"
#include <fstream>
#include "uml/model.h"
#include "uml/umlManager.h"
#include "uml/literalNull.h"
#include "uml/stereotype.h"
#include "uml/extensionEnd.h"
#include "uml/profile.h"
#include "uml/profileApplication.h"
#include "uml/comment.h"

using namespace std;

namespace UML {
namespace Parsers {

UmlManager* parse(string path) {
    UmlManager* ret = new UmlManager;
    Element* root = ret->parse(path);
    ret->setRoot(root);
    return ret;
}

Element* parse(ParserMetaData& data) {
    YAML::Node node = YAML::LoadFile(data.m_path);
    Element* ret = parseNode(node, data);
    data.m_manager->setRoot(ret);
    return ret;
}

string emit(Element& el) {
    UmlManager m;
    EmitterMetaData data;
    data.m_manager = &m;
    data.m_strategy = EmitterStrategy::WHOLE;
    YAML::Emitter emitter;
    emit(emitter, el, data);
    return emitter.c_str();
}

void emit(EmitterMetaData& data) {
    emitToFile(*data.m_manager->getRoot(), data, data.m_path, data.m_fileName);
}

void emitToFile(Element& el, EmitterMetaData& data, string path, string fileName) {
    YAML::Emitter newEmitter;
    filesystem::path cPath = data.m_path;
    string cFile = data.m_fileName;
    data.m_path = path;
    data.m_fileName = fileName;
    newEmitter << YAML::BeginDoc;
    emit(newEmitter, el, data);
    newEmitter << YAML::EndDoc;
    ofstream file;
    file.open(path + '/' + fileName);
    file << newEmitter.c_str();
    file.close();
    data.m_path = cPath;
    data.m_fileName = cFile;
}

namespace {

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

    if (node["profile"]) {
        Profile& profile = data.m_manager->create<Profile>();
        parsePackage(node["profile"], profile, data);
        return &profile;
    }

    if (node["property"]) {
        Property& prop = data.m_manager->create<Property>();
        parseProperty(node["property"], prop, data);
        return &prop;
    }

    return 0;
}

Element* parseExternalAddToManager(ParserMetaData& data, string path) {
    if (filesystem::exists(data.m_path.parent_path() / path)) {
        filesystem::path cPath = data.m_path;
        data.m_path = cPath.parent_path() / path;;
        Element* ret = parse(data);
        data.m_manager->setPath(ret->getID(), data.m_path);
        data.m_path = cPath;
        return ret;
    } else {
        return 0;
    }
}

void emit(YAML::Emitter& emitter, Element& el, EmitterMetaData& data) {
    switch(el.getElementType()) {
        case ElementType::ASSOCIATION : {
            emitAssociation(emitter, dynamic_cast<Association&>(el), data);
            break;
        }
        case ElementType::CLASS : {
            emitClass(emitter, dynamic_cast<Class&>(el), data);
            break;
        }
        case ElementType::DATA_TYPE : {
            emitDataType(emitter, dynamic_cast<DataType&>(el), data);
            break;
        }
        case ElementType::ENUMERATION : {
            emitEnumeration(emitter, dynamic_cast<Enumeration&>(el), data);
            break;
        }
        case ElementType::EXPRESSION : {
            emitExpression(emitter, dynamic_cast<Expression&>(el), data);
            break;
        }
        case ElementType::EXTENSION : {
            emitExtension(emitter, dynamic_cast<Extension&>(el), data);
            break;
        }
        case ElementType::INSTANCE_SPECIFICATION : {
            emitInstanceSpecification(emitter, dynamic_cast<InstanceSpecification&>(el), data);
            break;
        }
        case ElementType::LITERAL_BOOL : {
            emitLiteralBool(emitter, dynamic_cast<LiteralBool&>(el), data);
            break;
        }
        case ElementType::LITERAL_INT : {
            emitLiteralInt(emitter, dynamic_cast<LiteralInt&>(el), data);
            break;
        }
        case ElementType::LITERAL_NULL : {
            emitter << YAML::BeginMap << YAML::Key << "literalNull" << YAML::Value << YAML::BeginMap;
            emitTypedElement(emitter, dynamic_cast<TypedElement&>(el), data);
            emitter << YAML::EndMap << YAML::EndMap;
            break;
        }
        case ElementType::LITERAL_REAL : {
            emitLiteralReal(emitter, dynamic_cast<LiteralReal&>(el), data);
            break;
        }
        case ElementType::LITERAL_STRING : {
            emitLiteralString(emitter, dynamic_cast<LiteralString&>(el), data);
            break;
        }
        case ElementType::LITERAL_UNLIMITED_NATURAL : {
            emitLiteralUnlimitedNatural(emitter, dynamic_cast<LiteralUnlimitedNatural&>(el), data);
            break;
        }
        case ElementType::MODEL : {
            emitModel(emitter, dynamic_cast<Model&>(el), data);
            break;
        }
        case ElementType::OPAQUE_BEHAVIOR : {
            emitOpaqueBehavior(emitter, dynamic_cast<OpaqueBehavior&>(el), data);
            break;
        }
        case ElementType::PACKAGE : {
            emitPackage(emitter, dynamic_cast<Package&>(el), data);
            break;
        }
        case ElementType::PRIMITIVE_TYPE : {
            emitPrimitiveType(emitter, dynamic_cast<PrimitiveType&>(el), data);
            break;
        }
        case ElementType::PROFILE : {
            emitter << YAML::BeginMap << YAML::Key << "profile" << YAML::Value << YAML::BeginMap;
            emitPackage(emitter, dynamic_cast<Profile&>(el), data);
            emitter << YAML::EndMap << YAML::EndMap;
            break;
        }
        default: {
            throw UmlParserException("Error emitting element, element type " + el.getElementTypeString() + " is abstract and cannot be emit", "");
            break;
        }
    }
}

void emitModel(YAML::Emitter& emitter, Model& model, EmitterMetaData& data) {
    if (model.getElementType() == ElementType::MODEL) {
        emitter << YAML::BeginMap << YAML::Key << "model" << YAML::Value << YAML::BeginMap;
    }

    emitPackage(emitter, model, data);

    if (model.getElementType() == ElementType::MODEL) {
        emitter << YAML::EndMap << YAML::EndMap;
    }
}

void AddAppliedStereotypeFunctor::operator()(Element& el) const {
    // will always be called after setClassifier functor
    if (el.isSubClassOf(ElementType::STEREOTYPE) && m_el->isSubClassOf(ElementType::INSTANCE_SPECIFICATION)) {
        m_stereotypedEl.getAppliedStereotypes().add(dynamic_cast<InstanceSpecification&>(*m_el));
    } else {
        throw UmlParserException("appliedStereotype instance classifier must be a stereotype!", "", m_node);
    }
}

void parseElement(YAML::Node node, Element& el, ParserMetaData& data) {
    if (node["id"]) {
        if (node["id"].IsScalar()) {
            string id = node["id"].as<string>();
            if (isValidID(id)) {
                el.setID(ID::fromString(id));
            } else {
                throw UmlParserException("Value for id is not a valid ID, must be base64 url safe encoded 28 character string, " , data.m_path.string() , node["id"]);
            }
        } else {
            throw UmlParserException("Improper YAML node type for id field, must be scalar, " , data.m_path.string(), node["id"]);
        }
    }

    // apply post processing here via functor
    data.elements.add(el);

    if (node["ownedComments"]) {
        if (node["ownedComments"].IsSequence()) {
            for (size_t i = 0; i < node["ownedComments"].size(); i++) {
                if (node["ownedComments"][i]["comment"]) {
                    if (node["ownedComments"][i]["comment"].IsMap()) {
                        Comment& comment = data.m_manager->create<Comment>();
                        parseComment(node["ownedComments"][i]["comment"], comment, data);
                        el.getOwnedComments().add(comment);
                    } else {
                        throw UmlParserException("Invalid yaml node type for comment, must be map!", data.m_path.string(), node["ownedComments"][i]["comment"]);
                    }
                } else {
                    throw UmlParserException("Invalid element type for ownedComment, must be a comment", data.m_path.string(), node["ownedComments"][i]);
                }
            }
        } else {
            throw UmlParserException("Invalid yaml node type for ownedComments, must be sequence!", data.m_path.string(), node["ownedComments"]);
        }
    }

    if (node["appliedStereotypes"]) {
        if (node["appliedStereotypes"].IsSequence()) {
            for (size_t i = 0; i < node["appliedStereotypes"].size(); i++) {
                if (node["appliedStereotypes"][i]["instanceSpecification"]) {
                    InstanceSpecification& inst = data.m_manager->create<InstanceSpecification>();
                    parseInstanceSpecification(node["appliedStereotypes"][i]["instanceSpecification"], inst, data);
                    applyFunctor(data, ID::fromString(node["appliedStereotypes"][i]["instanceSpecification"]["classifier"].as<string>()), new AddAppliedStereotypeFunctor(&inst, node["appliedStereotypes"][i]["instanceSpecification"]["classifier"], el));
                } else {
                    throw UmlParserException("Invalid uml element type for applied stereotype, must be an instancespecification", data.m_path.string(), node["appliedStereotypes"][i]);
                }
            }
        } else {
            throw UmlParserException("Invalid yaml node type for appliedStereotypes, must be a sequence!", data.m_path.string(), node["appliedStereotypes"]);
        }
    }
}

void emitElement(YAML::Emitter& emitter, Element& el, EmitterMetaData& data) {
    emitter << YAML::Key << "id" << YAML::Value << el.getID().string();

    if (!el.getOwnedComments().empty()) {
        emitter << YAML::Key << "ownedComments" << YAML::Value << YAML::BeginSeq;
        for (auto& comment : el.getOwnedComments()) {
            emitComment(emitter, comment, data);
        }
    }

    if (!el.getAppliedStereotypes().empty()) {
        emitter << YAML::Key << "appliedStereotypes" << YAML::Value << YAML::BeginSeq;
        for (auto& stereotypeInst : el.getAppliedStereotypes()) {
            emitInstanceSpecification(emitter, stereotypeInst, data);
        }
        emitter << YAML::EndSeq;
    }
}

void parseNamedElement(YAML::Node node, NamedElement& el, ParserMetaData& data) {

    parseElement(node, el, data);

    if (node["name"]) {
        if (node["name"].IsScalar()) {
            el.setName(node["name"].as<string>());
        } else {
            throw UmlParserException("Improper YAML node type for name field, must be scalar, " , data.m_path.string() , node["name"]);
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
            throw UmlParserException("Improper YAML node type for NamedElement field visibility, must be a scalar, " , data.m_path.string() , node["visibility"]);
        }
    }
}

void emitNamedElement(YAML::Emitter& emitter, NamedElement& el, EmitterMetaData& data) {
    emitElement(emitter, el, data);
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
        // need to cast to make polymorphic with stereotype as parameter
        if (el.isSubClassOf(ElementType::STEREOTYPE) && m_el->isSubClassOf(ElementType::EXTENSION_END)) {
            dynamic_cast<ExtensionEnd*>(m_el)->setType(&dynamic_cast<Stereotype&>(el));
        } else {
            dynamic_cast<TypedElement*>(m_el)->setType(&dynamic_cast<Type&>(el));
        }
    } else {
        throw UmlParserException(m_el->getElementTypeString() + " id: " + m_el->getID().string() + 
                                 " assigned type is not a typed Element! line " , "" , m_node);
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
                throw UmlParserException("ID for " + el.getElementTypeString() + " type field is invalid, " , data.m_path.string() , node["type"]);
            }
        } else {
            throw UmlParserException("Improper YAML node type for type field, must be scalar, " , data.m_path.string() , node["type"]);
        }
    }
}

void emitTypedElement(YAML::Emitter& emitter, TypedElement& el, EmitterMetaData& data) {
    emitNamedElement(emitter, el, data);

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
                        throw UmlParserException("Improper YAML node type for Generalization definition, " , data.m_path.string() , node["generalizations"][i]["generalization"]);
                    }
                }
            }
        } else {
            throw UmlParserException("Improper YAML node type for Classifier field generalizations, must be sequence " , data.m_path.string() ,node["generalizations"]);
        }
    }
}

void emitClassifier(YAML::Emitter& emitter, Classifier& clazz, EmitterMetaData& data) {
    emitNamedElement(emitter, clazz, data);
    emitTemplateableElement(emitter, clazz, data);
    
    if (!clazz.getGeneralizations().empty()) {
        emitter << YAML::Key << "generalizations" << YAML::Value << YAML::BeginSeq;
        for (auto& generalization: clazz.getGeneralizations()) {
            emitGeneralization(emitter, generalization, data);
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

void emitGeneralization(YAML::Emitter& emitter, Generalization& generalization, EmitterMetaData& data) {
    if (generalization.getElementType() == ElementType::GENERALIZATION) {
        emitter << YAML::BeginMap << YAML::Key << "generalization" << YAML::Value << YAML::BeginMap;
    }

    emitElement(emitter, generalization, data);

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
                        throw UmlParserException("Improper YAML node type for property, must be map, " , data.m_path.string() , node["ownedAttribute"][i]["property"]);
                    }
                }
            }
        } else {
            throw UmlParserException("Improper YAML node type for dataType ownedAttribute, must be sequence, " ,data.m_path.string(), node["ownedAttribute"]);
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
                        throw UmlParserException("Improper YAML node type for operation, must be map, " , data.m_path.string() , node["ownedOperation"][i]["operation"]);
                    }
                } else {
                    throw UmlParserException("Improper UML node type for ownedOperation sequence, " , data.m_path.string() , node["ownedOperation"][i]);
                }
            }
        } else {
            throw UmlParserException("Improper YAML node type for dataType ownedOperation, must be sequence, " , data.m_path.string() , node["ownedOperation"]);
        }
    }
}

void emitDataType(YAML::Emitter& emitter, DataType& dataType, EmitterMetaData& data) {
    if (dataType.getElementType() == ElementType::DATA_TYPE) {
        emitter << YAML::BeginMap << YAML::Key << "dataType" << YAML::Value << YAML::BeginMap;
    }

    emitClassifier(emitter, dataType, data);
    
    if (!dataType.getOwnedAttribute().empty()) {
        emitter << YAML::Key << "ownedAttribute" << YAML::Value << YAML::BeginSeq;
        for (auto& attribute: dataType.getOwnedAttribute()) {
            emitProperty(emitter, attribute, data);
        }
        emitter << YAML::EndSeq;
    }

    if (!dataType.getOwnedOperation().empty()) {
        emitter << YAML::Key << "ownedOperation" << YAML::Value << YAML::BeginSeq;
        for (auto& operation : dataType.getOwnedOperation()) {
            emitOperation(emitter, operation, data);
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

void emitPrimitiveType(YAML::Emitter& emitter, PrimitiveType& type, EmitterMetaData& data) {
    if (type.getElementType() == ElementType::PRIMITIVE_TYPE) {
        emitter << YAML::BeginMap << YAML::Key << "primitiveType" << YAML::Value << YAML::BeginMap;
    }

    emitDataType(emitter, type, data);

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
                        throw UmlParserException("Improper YAML node type for property field, must be map, " , data.m_path.string() , node["ownedAttributes"][i]["property"]);
                    }
                }
            }
        } else {
            throw UmlParserException("Improper YAML node type for ownedAttributes field, must be sequence, " , data.m_path.string(), node["ownedAttributes"]);
        }
    }
}

void emitStructuredClassifier(YAML::Emitter& emitter, StructuredClassifier& clazz, EmitterMetaData& data) {

    emitClassifier(emitter, clazz, data);

    if (!clazz.getOwnedAttributes().empty()) {
        emitter << YAML::Key << "ownedAttributes" << YAML::Value << YAML::BeginSeq;
        for (auto& attribute : clazz.getOwnedAttributes()) {
            emitProperty(emitter, attribute, data);
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
                    throw UmlParserException("Could not identify operation to parse, ", data.m_path.string(), node["operations"][i]);
                }
            }
        } else {
            throw UmlParserException("Improper YAML node type for operations field, must be scalar, ", data.m_path.string(), node["attributes"]);
        }
    }
}

void emitClass(YAML::Emitter& emitter, Class& clazz, EmitterMetaData& data) {
    if (clazz.getElementType() == ElementType::CLASS) {
        emitter << YAML::BeginMap << YAML::Key << "class" << YAML::BeginMap;
    }

    emitStructuredClassifier(emitter, clazz, data);

    if (!clazz.getOperations().empty()) {
        emitter << YAML::Key << "operations" << YAML::Value << YAML::BeginSeq;
        for (auto& operation : clazz.getOperations()) {
            emitOperation(emitter, operation, data);
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
            throw UmlParserException("Improper YAML node type for parameters field, must be scalar,", data.m_path.string(), node["parameters"]);
        }
    }
}

void emitBehavior(YAML::Emitter& emitter, Behavior& bhv, EmitterMetaData& data) {
    emitClass(emitter, bhv, data);

    if (!bhv.getParameters().empty()) {
        emitter << YAML::Key << "parameters" << YAML::Value << YAML::BeginSeq;
        for (auto& param : bhv.getParameters()) {
            emitParameter(emitter, param, data);
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
                        throw UmlParserException("No literal string for body entry of a Map Node,", data.m_path.string(), node["bodies"][i]["literalString"]);
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
                    throw UmlParserException("Improper YAML node type for bodies sequence, must be scalar or map, ", data.m_path.string(), node["bodies"][i]);
                }
            }
        } else {
            throw UmlParserException("Improper YAML node type for bodies, must be sequence, ", data.m_path.string(), node["bodies"]);
        }
    }
}

void emitOpaqueBehavior(YAML::Emitter& emitter, OpaqueBehavior& bhv, EmitterMetaData& data) {
    if (bhv.getElementType() == ElementType::OPAQUE_BEHAVIOR) {
        emitter << YAML::BeginMap << YAML::Key << "opaqueBehavior" << YAML::Value << YAML::BeginMap;
    }

    emitBehavior(emitter, bhv, data);

    if (!bhv.getBodies().empty()) {
        emitter << YAML::Key << "bodies" << YAML::Value << YAML::BeginSeq;
        for (auto& body : bhv.getBodies()) {
            emit(emitter, body, data);
        }
        emitter << YAML::EndSeq;
    }

    if (bhv.getElementType() == ElementType::OPAQUE_BEHAVIOR) {
        emitter << YAML::EndMap << YAML::EndMap;
    }
}

ValueSpecification& determineAndParseValueSpecification(YAML::Node node, ParserMetaData& data) {
    if (node["expression"]) {
        if (node["expression"].IsMap()) {
            Expression& newExp = data.m_manager->create<Expression>();
            parseExpression(node["expression"], newExp, data);
            return newExp;
        } else {
            throw UmlParserException("Invalid YAML node type, ", data.m_path.string(), node["expression"]);
        }
    } else if (node["literalBool"]) {
        if (node["literalBool"].IsMap()) {
            LiteralBool& lb = data.m_manager->create<LiteralBool>();
            parseLiteralBool(node["literalBool"], lb, data);
            return lb;
        } else {
            throw UmlParserException("Improper YAML node type, ", data.m_path.string(), node["literalBool"]);
        }
    } else if (node["literalInt"]) {
        if (node["literalInt"].IsMap()) {
            LiteralInt& li = data.m_manager->create<LiteralInt>();
            parseLiteralInt(node["literalInt"], li, data);
            return li;
        } else {
            throw UmlParserException("Improper YAML node type, ", data.m_path.string(), node["literalInt"]);
        }
    } else if (node["literalNull"]) {
        if (node["literalNull"].IsMap()) {
            LiteralNull& ln = data.m_manager->create<LiteralNull>();
            parseTypedElement(node["literalNull"], ln, data);
            return ln;
        } else {
            throw UmlParserException("Improper YAML node type, ", data.m_path.string(), node["literalNull"]);
        }
    } else if (node["literalReal"]) {
        if (node["literalReal"].IsMap()) {
            LiteralReal& lr = data.m_manager->create<LiteralReal>();
            parseLiteralReal(node["literalReal"], lr, data);
            return lr;
        } else {
            throw UmlParserException("Improper YAML node type, ", data.m_path.string(), node["literalReal"]);
        }
    } else if (node["literalString"]) {
        if (node["literalString"].IsMap()) {
            LiteralString& ls = data.m_manager->create<LiteralString>();
            parseLiteralString(node["literalString"], ls, data);
            return ls;
        } else {
            throw UmlParserException("Improper YAML node type, ", data.m_path.string(), node["literalString"]);
        }
    } else if (node["literalUnlimtedNatural"]) {
        if (node["literalUnlimitedNatural"]) {
            LiteralUnlimitedNatural& ln = data.m_manager->create<LiteralUnlimitedNatural>();
            parseLiteralUnlimitedNatural(node["literalUnlimitedNatural"], ln, data);
            return ln;
        } else {
            throw UmlParserException("Improper YAML node type, ", data.m_path.string(), node["literalUnlimitedNatural"]);
        }
    } else if (node["instanceValue"]) {
        if (node["instanceValue"].IsMap()) {
            InstanceValue& iv = data.m_manager->create<InstanceValue>();
            parseInstanceValue(node["instanceValue"], iv, data);
            return iv;
        } else {
            throw UmlParserException("Improper YAML node type, ", data.m_path.string(), node["instanceValue"]);
        }
    } else {
        throw UmlParserException("Unknown Value Specification, ", data.m_path.string(), node);
    }
    ValueSpecification& dumb = data.m_manager->create<ValueSpecification>();
    dumb.setID(ID::nullID());
    return dumb;
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
            throw UmlParserException("Improper YAML node type for bodies, must be scalar, ", data.m_path.string(), node["aggregation"]);
        }
    }

    if (node["defaultValue"]) {
        if (node["defaultValue"].IsMap()) {
            prop.setDefaultValue(&determineAndParseValueSpecification(node["defaultValue"], data));
        } else {
            throw UmlParserException("Invalid yaml node type, must be Map!", data.m_path.string(), node["defaultValue"]);
        }
    }
}

void emitProperty(YAML::Emitter& emitter, Property& prop, EmitterMetaData& data) {
    if (prop.getElementType() == ElementType::PROPERTY) {
        emitter << YAML::BeginMap << YAML::Key << "property" << YAML::Value << YAML::BeginMap;
    }

    emitTypedElement(emitter, prop, data);
    emitMultiplicityElement(emitter, prop, data);

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
                throw UmlParserException("Invalid direction provided for Parameter, ", data.m_path.string(), node["direction"]);
            }
        } else {
            throw UmlParserException("Improper YAML node type for direction, must be scalar, ", data.m_path.string(), node["direction"]);
        }
    }
}

void emitParameter(YAML::Emitter& emitter, Parameter& el, EmitterMetaData& data) {
    if (el.getElementType() == ElementType::PARAMETER) {
        emitter << YAML::BeginMap << YAML::Key << "parameter" << YAML::Value << YAML::BeginMap;
    }

    emitTypedElement(emitter, el, data);
    emitMultiplicityElement(emitter, el, data);

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
                    throw UmlParserException("Invalid behavior type for operation methods, ", data.m_path.string(), node["methods"][i]);
                } 
            }
        } else {
            throw UmlParserException("Improper YAML node type for methods, must be sequence, ", data.m_path.string(), node["methods"]);
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
                        throw UmlParserException("Improper YAML Node type for parameter definition, must be map, ", data.m_path.string(), node["ownedParameters"][i]["parameter"]);
                    }
                }
            }
        } else {
            throw UmlParserException("Improper YAML node type for ownedParameters field, must be a sequence, ", data.m_path.string(), node["ownedParameters"]);
        }
    }
}

void emitOperation(YAML::Emitter& emitter, Operation& op, EmitterMetaData& data) {
    if (op.getElementType() == ElementType::OPERATION) {
        emitter << YAML::BeginMap << YAML::Key << "operation" << YAML::Value << YAML::BeginMap;
    }

    emitNamedElement(emitter, op, data);
    emitTemplateableElement(emitter, op, data);

    if (!op.getMethods().empty()) {
        emitter << YAML::Key << "methods" << YAML::Value << YAML::BeginSeq;
        for (auto& method : op.getMethods()) {
            emit(emitter, method, data);
        }
        emitter << YAML::EndSeq;
    }

    if (!op.getOwnedParameters().empty()) {
        emitter << YAML::Key << "ownedParameters" << YAML::Value << YAML::BeginSeq;
        for (auto& parameter : op.getOwnedParameters()) {
            emitParameter(emitter, parameter, data);
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
                        throw UmlParserException("Invalid YAML Node type for packageMerge definition with in Package, ", data.m_path.string(), node["packageMerge"][i]["packageMerge"]);
                    }
                } else {
                    throw UmlParserException("Invalid identifier in Package packageMerge list, ", data.m_path.string(), node["packageMerge"][i]["packageMerge"]);
                }
            }
        } else {
            throw UmlParserException("Invalid YAML Node type for Package field packageMerge, ", data.m_path.string(), node["packageMerge"]);
        }
    }

    if (node["profileApplications"]) {
        if (node["profileApplications"].IsSequence()) {
            for (size_t i = 0; i < node["profileApplications"].size(); i++) {
                if (node["profileApplications"][i]["profileApplication"]) {
                    if (node["profileApplications"][i]["profileApplication"].IsMap()) {
                        ProfileApplication& application = data.m_manager->create<ProfileApplication>();
                        parseProfileApplication(node["profileApplications"][i]["profileApplication"], application, data);
                        pckg.getProfileApplications().add(application);
                    } else {
                        throw UmlParserException("Invalid yaml node type, profileApplication must be a mao!", data.m_path.string(), node["profileApplications"][i]["profileApplication"]);
                    }
                } else {
                    throw UmlParserException("Invalid profileApplication type!", data.m_path.string(), node["profileApplications"][i]);
                }
            }
        } else {
            throw UmlParserException("Invalide yaml node type for profileApplications, must be a sequence!", data.m_path.string(), node["profileApplication"]);
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
                    } else if (node["packagedElements"][i]["association"]) {
                        if (node["packagedElements"][i]["association"].IsMap()) {
                            Association& association = data.m_manager->create<Association>();
                            parseAssociation(node["packagedElements"][i]["association"], association, data);
                            pckg.getPackagedElements().add(association);
                        } else {
                            throw UmlParserException("Invalid yaml node type, must be a map!", data.m_path.string(), node["packagedElements"][i]["association"]);
                        }
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
                            throw UmlParserException("Invalid YAML node type for expression definition, must be map, ", data.m_path.string(), node["operands"][i]["expression"]);
                        }
                    } else if (node["packagedElements"][i]["extension"]) {
                        if (node["packagedElements"][i]["extension"].IsMap()) {
                            Extension& extension = data.m_manager->create<Extension>();
                            parseExtension(node["packagedElements"][i]["extension"], extension, data);
                            pckg.getPackagedElements().add(extension);
                        } else {
                            throw UmlParserException("Invalide yaml node type for extension definition, must be a map!", data.m_path.string(), node["packagedElements"][i]["extension"]);
                        }
                    } else if (node["packagedElements"][i]["instanceSpecification"]) {
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
                    } else if (node["packagedElements"][i]["profile"]) {
                        if (node["packagedElements"][i]["profile"].IsMap()) {
                            Profile& profile = data.m_manager->create<Profile>();
                            parsePackage(node["packagedElements"][i]["profile"], profile, data);
                            pckg.getPackagedElements().add(profile);
                        }
                    } else {
                        throw UmlParserException("Invalid identifier for packagedElements, ", data.m_path.string(), node["packagedElements"][i]);
                    }
                // seperate file
                } else if (node["packagedElements"][i].IsScalar()) {
                    Element* packagedEl = parseExternalAddToManager(data, node["packagedElements"][i].as<string>());
                    if (packagedEl == 0) {
                        throw UmlParserException("Could not identify YAML node for packaged elements" , data.m_path.string(), node["packagedElements"][i]);
                    }
                    if (packagedEl->isSubClassOf(ElementType::PACKAGEABLE_ELEMENT)) {
                        pckg.getPackagedElements().add(dynamic_cast<PackageableElement&>(*packagedEl));
                    }
                } else {
                    throw UmlParserException("Invalid YAML node type for field packagedElements sequence, must be map, ", data.m_path.string(), node["packagedElements"][i]);
                }
            }
        } else {
            throw UmlParserException("Invalid YAML node type for field packagedElements, must be sequence, ", data.m_path.string(), node["packagedElements"]);
        }
    }

    // TODO update
    if (node["ownedStereotypes"]) {
        if (node["ownedStereotypes"].IsSequence()) {
            for (size_t i = 0; i < node["ownedStereotypes"].size(); i++) {
                if (node["ownedStereotypes"][i]["stereotype"]) {
                    Stereotype& s = data.m_manager->create<Stereotype>();
                    parseClass(node["ownedStereotypes"][i]["stereotype"], s, data);
                    pckg.getOwnedStereotypes().add(s);
                } else {
                    throw UmlParserException("Invalid uml element definition, must be stereotype!", data.m_path.string(), node["ownedStereotypes"][i]["stereotype"]);
                }
            }
        } else {
            throw UmlParserException("Invalid yaml node type for field ownedStereotypes, must be sequence, ", data.m_path.string(), node["ownedStereotypes"]);
        }
    }
}

void emitPackage(YAML::Emitter& emitter, Package& pckg, EmitterMetaData& data) {
    if (pckg.getElementType() == ElementType::PACKAGE) {
        emitter << YAML::BeginMap << YAML::Key << "package" << YAML::Value << YAML::BeginMap;
    }

    emitNamedElement(emitter, pckg, data);
    emitTemplateableElement(emitter, pckg, data);

    if (!pckg.getPackageMerge().empty()) {
        emitter << YAML::Key << "packageMerge" << YAML::Value << YAML::BeginSeq;
        for (auto& pckgMerge : pckg.getPackageMerge()) {
            emitPackageMerge(emitter, pckgMerge, data);
        }
        emitter << YAML::EndSeq;
    }

    if (!pckg.getProfileApplications().empty()) {
        emitter << YAML::Key << "profileApplications" << YAML::Value << YAML::BeginSeq;
        for (auto& application : pckg.getProfileApplications()) {
            emitProfileApplication(emitter, application, data);
        }
        emitter << YAML::EndSeq;
    }

    if (!pckg.getPackagedElements().empty()) {
        emitter << YAML::Key << "packagedElements" << YAML::Value << YAML::BeginSeq;
        switch (data.m_strategy) {
            case EmitterStrategy::WHOLE : {
                for (auto& el : pckg.getPackagedElements()) {
                    if (!el.isSubClassOf(ElementType::STEREOTYPE)) {
                        filesystem::path newPath = data.m_manager->getPath(el.getID());
                        if (newPath.empty() || (newPath.parent_path().compare(data.m_path) == 0 && newPath.filename().compare(data.m_fileName))) {
                            emit(emitter, el, data);
                        } else {
                            emitToFile(el, data, newPath.parent_path(), newPath.filename());
                        }
                    }
                }
                break;
            }
            case EmitterStrategy::COMPOSITE : {
                for (auto& el : pckg.getPackagedElements()) {
                    YAML::Emitter newEmitter;
                    filesystem::path cPath = data.m_path;
                    string cFile = data.m_fileName;
                    data.m_path = cPath / el.getID().string();
                    data.m_fileName = el.getID().string() + ".yml";
                    emit(newEmitter, el, data);
                    ofstream file;
                    file.open(data.m_path / el.getID().string() / (el.getID().string() + ".yml"));
                    file << newEmitter.c_str();
                    file.close();
                    data.m_path = cPath;
                    data.m_fileName = cFile;
                }
                break;
            }
        }
        
        emitter << YAML::EndSeq;
    }

    if (!pckg.getOwnedStereotypes().empty()) {
        emitter << YAML::Key << "ownedStereotypes" << YAML::Value << YAML::BeginSeq;
        for (auto& stereotype : pckg.getOwnedStereotypes()) {
            // TODO move to own function?
            emitter << YAML::BeginMap << YAML::Key << "stereotype" << YAML::Value << YAML::BeginMap;
            emitClass(emitter, stereotype, data);
            emitter << YAML::EndMap << YAML::EndMap;
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
            throw UmlParserException("Invalid YAML node type for field lower, must be scalar or map, ", data.m_path.string(), node["lower"]);
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
            throw UmlParserException("Invalid YAML node type for field upper, must be scalar or map, ", data.m_path.string(), node["upper"]);
        }
    }
}

void emitMultiplicityElement(YAML::Emitter& emitter, MultiplicityElement& el, EmitterMetaData& data) {
    if (el.getLowerValue()) {
        emitter << YAML::Key << "lower" << YAML::Value;
        emit(emitter, *el.getLowerValue(), data);
    }

    if (el.getUpperValue()) {
        emitter << YAML::Key << "upper" << YAML::Value;
        emit(emitter, *el.getUpperValue(), data);
    }
}

void SetClassifierFunctor::operator()(Element& el) const {
    if (el.isSubClassOf(ElementType::CLASSIFIER)) {
        dynamic_cast<InstanceSpecification*>(m_el)->setClassifier(&dynamic_cast<Classifier&>(el));
    } else {
        throw UmlParserException(m_el->getElementTypeString() + " id: " + m_el->getID().string() + 
                                 " assigned classifier is not a classifer! line ", "", m_node);
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
            throw UmlParserException("Invalid YAML node type for InstanceSpecification field classifier,", data.m_path.string(), node["classifier"]);
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
            throw UmlParserException("Invalid YAML node type for InstanceSpecification field slots, expected sequence, ", data.m_path.string(), node["slots"]);
        }
    }
}

void emitInstanceSpecification(YAML::Emitter& emitter, InstanceSpecification& inst, EmitterMetaData& data) {
    if (inst.getElementType() == ElementType::INSTANCE_SPECIFICATION) {
        emitter << YAML::BeginMap << YAML::Key << "instanceSpecification" << YAML::Value << YAML::BeginMap;
    }

    emitNamedElement(emitter, inst, data);

    if (inst.getClassifier()) {
        emitter << YAML::Key << "classifier" << YAML::Value << inst.getClassifier()->getID().string();
    }

    if (!inst.getSlots().empty()) {
        emitter << YAML::Key << "slots" << YAML::Value << YAML::BeginSeq;
        for (auto& slot : inst.getSlots()) {
            emitSlot(emitter, slot, data);
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
                                 " assigned definingFeature is not a structuralFeature! line ","", m_node);
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
            throw UmlParserException("Invalid YAML node type for Slot field definingFeature, expected scalar, ", data.m_path.string(), node["definingFeature"]);
        }
    }

    // TODO values
    if (node["values"]) {
        if (node["values"].IsSequence()) {
            for (size_t i = 0; i < node["values"].size(); i++) {
                slot.getValues().add(determineAndParseValueSpecification(node["values"][i], data));
            }
        } else {
            throw UmlParserException("Invalid YAML node type for Slot field values, expected Sequence, ", data.m_path.string(), node["values"]);
        }
    }
}

void emitSlot(YAML::Emitter& emitter, Slot& slot, EmitterMetaData& data) {
    if (slot.getElementType() == ElementType::SLOT) {
        emitter << YAML::BeginMap << YAML::Key << "slot" << YAML::Value << YAML::BeginMap;
    }

    emitElement(emitter, slot, data);

    if (slot.getDefiningFeature()) {
        emitter << YAML::Key << "definingFeature" << YAML::Value << slot.getDefiningFeature()->getID().string();
    }

    if (!slot.getValues().empty()) {
        emitter << YAML::Key << "values" << YAML::Value << YAML::BeginSeq;
        for (auto& val : slot.getValues()) {
            emit(emitter, val, data);
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
                        throw UmlParserException("Invalid YAML node type enumerationLiteral definition, should be map, ", data.m_path.string(), node["ownedLiteral"][i]["enumerationLiteral"]);
                    }
                } else {
                    throw UmlParserException("Invalid UML type in enumeration ownedLiteral list, must be literal,", data.m_path.string(), node["ownedLiteral"]);
                }
            }
        } else {
            throw UmlParserException("Invalid YAML node type for Enumeration field ownedLiteral, expected sequence, ", data.m_path.string(), node["ownedLiteral"]);
        }
    }
}

void emitEnumeration(YAML::Emitter& emitter, Enumeration& enumeration, EmitterMetaData& data) {
    if (enumeration.getElementType() == ElementType::ENUMERATION) {
        emitter << YAML::BeginMap << YAML::Key << "enumeration" << YAML::Value << YAML::BeginMap;
    }

    emitDataType(emitter, enumeration, data);

    if (!enumeration.getOwnedLiteral().empty()) {
        emitter << YAML::Key << "ownedLiteral" << YAML::BeginSeq;
        for (auto& literal : enumeration.getOwnedLiteral()) {
            emitEnumerationLiteral(emitter, literal, data);
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

void emitEnumerationLiteral(YAML::Emitter& emitter, EnumerationLiteral& literal, EmitterMetaData& data) {
    if (literal.getElementType() == ElementType::ENUMERATION_LITERAL) {
        emitter << YAML::BeginMap << YAML::Key << "enumerationLiteral" << YAML::Value << YAML::BeginMap;
    }

    emitInstanceSpecification(emitter, literal, data);

    if (literal.getElementType() == ElementType::ENUMERATION_LITERAL) {
        emitter << YAML::EndMap << YAML::EndMap;
    }
}

void SetInstanceFunctor::operator()(Element& el) const {
    if (el.isSubClassOf(ElementType::INSTANCE_SPECIFICATION)) {
        dynamic_cast<InstanceValue*>(m_el)->setInstance(&dynamic_cast<InstanceSpecification&>(el));
    } else {
        throw UmlParserException(m_el->getElementTypeString() + " id: " + m_el->getID().string() + 
                                 " assigned instance is not an instanceSpecification! line ", "", m_node);
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
                throw UmlParserException("Scalar YAML node for InstanceValue field instance is not a valid id, must be base64 url safe 28 character string, ", data.m_path.string(), node["instance"]);
            }
        } else {
            throw UmlParserException("Invalid YAML node type for InstanceValue field instance, expect scalar, ", data.m_path.string(), node["instance"]);
        }
    }
}

void emitInstanceValue(YAML::Emitter& emitter, InstanceValue& val, EmitterMetaData& data) {
    if (val.getElementType() == ElementType::INSTANCE_VALUE) {
        emitter << YAML::BeginMap << YAML::Key << "instanceValue" << YAML::Value << YAML::BeginMap;
    }

    emitTypedElement(emitter, val, data);

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
                                        " assigned mergedPackage that is not a Package! line ", "", m_node);
    }
}

void parsePackageMerge(YAML::Node node, PackageMerge& merge, ParserMetaData& data) {
    parseElement(node, merge, data);

    if (node["receivingPackage"]) {
        // This won't be called cause it is always defined from within receiving Package body
        throw UmlParserException("TODO", "" , node["receivingPackage"]);
    }

    if (node["mergedPackage"]) {
        if (node["mergedPackage"].IsScalar()) {
            string pckgString = node["mergedPackage"].as<string>();
            if (isValidID(pckgString)) {
                ID pckgID = ID::fromString(pckgString);
                applyFunctor(data, pckgID, new SetMergedPackageFunctor(&merge, node["mergedPackage"]));
            } else {
                Element* mergedPackage = parseExternalAddToManager(data, pckgString);
                if (mergedPackage == 0) {
                    throw UmlParserException("Could not parse external merged package!", data.m_path.string(), node["mergedPackage"]);
                }
                if (mergedPackage->isSubClassOf(ElementType::PACKAGE)) {
                    merge.setMergedPackage(dynamic_cast<Package*>(mergedPackage));
                } else {
                    throw UmlParserException("mergedPackage is not a package, ", data.m_path.string(), node["mergedPackage"]);
                }
            }
        } else {
            throw UmlParserException("Invalid YAML node type for PackageMerge field mergedPackage, expected scalar, ", data.m_path.string(), node["mergedPackage"]);
        }
    }
}

void emitPackageMerge(YAML::Emitter& emitter, PackageMerge& merge, EmitterMetaData& data) {
    if (merge.getElementType() == ElementType::PACKAGE_MERGE) {
        emitter << YAML::BeginMap << YAML::Key << "packageMerge" << YAML::Value << YAML::BeginMap;
    }

    emitElement(emitter, merge, data);

    if (merge.getMergedPackage() != 0) {
        filesystem::path path = data.m_manager->getPath(merge.getMergedPackage()->getID());
        if (path.empty() || path == data.m_path / data.m_fileName) {
            emitter << YAML::Key << "mergedPackage" << YAML::Value << merge.getMergedPackage()->getID().string();
        } else {
            emitToFile(*merge.getMergedPackage(), data, path.parent_path(), path.filename());
            if (data.m_path == path.parent_path()) {
                emitter << YAML::Key << "mergedPackage" << YAML::Value << path.filename();
            }
        }
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
            throw UmlParserException("Invalid YAML node type for LiteralBool field value, expected scalar, ", data.m_path.string(), node["value"]);
        }
    }
}

void emitLiteralBool(YAML::Emitter& emitter, LiteralBool& lb, EmitterMetaData& data) {
    if (lb.getElementType() == ElementType::LITERAL_BOOL) {
        emitter << YAML::BeginMap << YAML::Key << "literalBool" << YAML::Value << YAML::BeginMap;
    }

    emitTypedElement(emitter, lb, data);

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
            throw UmlParserException("Invalid YAML node type for LiteralInt field value, expected scalar, ", data.m_path.string(), node["value"]);
        }
    }
}

void emitLiteralInt(YAML::Emitter& emitter, LiteralInt& li, EmitterMetaData& data) {
    if (li.getElementType() == ElementType::LITERAL_INT) {
        emitter << YAML::BeginMap << YAML::Key << "literalInt" << YAML::Value << YAML::BeginMap;
    }

    emitTypedElement(emitter, li, data);

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
            throw UmlParserException("Invalid YAML node type for LiteralReal field value, expected scalar, ", data.m_path.string(), node["value"]);
        }
    }
}

void emitLiteralReal(YAML::Emitter& emitter, LiteralReal& lr, EmitterMetaData& data) {
    if (lr.getElementType() == ElementType::LITERAL_REAL) {
        emitter << YAML::BeginMap << YAML::Key << "literalReal" << YAML::Value << YAML::BeginMap;
    }

    emitTypedElement(emitter, lr, data);

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
            throw UmlParserException("Invalid YAML node type for LiteralString field value, expected scalar, ", data.m_path.string(), node["value"]);
        }
    }
}

void emitLiteralString(YAML::Emitter& emitter, LiteralString& ls, EmitterMetaData& data) {
    if (ls.getElementType() == ElementType::LITERAL_STRING) {
        emitter << YAML::BeginMap << YAML::Key << "literalString" << YAML::Value << YAML::BeginMap;
    }

    emitTypedElement(emitter, ls, data);

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
            throw UmlParserException("LiteralUnlimitedNatural value field must be a scalar!", data.m_path.string(), node["value"]);
        }
    }
}

void emitLiteralUnlimitedNatural(YAML::Emitter& emitter, LiteralUnlimitedNatural& ln, EmitterMetaData& data) {
    if (ln.getElementType() == ElementType::LITERAL_UNLIMITED_NATURAL) {
        emitter << YAML::BeginMap << YAML::Key << "literalUnlimitedNatural" << YAML::Value << YAML::BeginMap;
    }

    emitTypedElement(emitter, ln, data);

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
            throw UmlParserException("Invalid YAML node type for Expression field symbol, must be scalar, ", data.m_path.string(), node["symbol"]);
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
                        throw UmlParserException("Invalid YAML node type for expression definition, must be map, ", data.m_path.string(), node["operands"][i]["expression"]);
                    }
                } else if (node["operands"][i]["literalBool"]) {
                    if (node["operands"][i]["literalBool"].IsMap()) {
                        LiteralBool& lb = data.m_manager->create<LiteralBool>();
                        parseLiteralBool(node["operands"][i]["literalBool"], lb, data);
                        exp.getOperands().add(lb);
                    } else {
                        throw UmlParserException("Improper YAML node type for Expression operands field, ", data.m_path.string(), node["operands"][i]["literalBool"]);
                    }
                } else if (node["operands"][i]["literalInt"]) {
                    if (node["operands"][i]["literalInt"].IsMap()) {
                        LiteralInt& li = data.m_manager->create<LiteralInt>();
                        parseLiteralInt(node["operands"][i]["literalInt"], li, data);
                        exp.getOperands().add(li);
                    } else {
                        throw UmlParserException("Improper YAML node type for Expression operands field, ", data.m_path.string(), node["operands"][i]["literalInt"]);
                    }
                } else if (node["operands"][i]["literalReal"]) {
                    if (node["operands"][i]["literalReal"].IsMap()) {
                        LiteralReal& lr = data.m_manager->create<LiteralReal>();
                        parseLiteralReal(node["operands"][i]["literalReal"], lr, data);
                        exp.getOperands().add(lr);
                    } else {
                        throw UmlParserException("Improper YAML node type for Expression operands field, ", data.m_path.string(), node["operands"][i]["literalReal"]);
                    }
                } else if (node["operands"][i]["literalString"]) {
                    if (node["operands"][i]["literalString"].IsMap()) {
                        LiteralString& ls = data.m_manager->create<LiteralString>();
                        parseLiteralString(node["operands"][i]["literalString"], ls, data);
                        exp.getOperands().add(ls);
                    } else {
                        throw UmlParserException("Improper YAML node type for Expression operands field, ", data.m_path.string(), node["operands"][i]["literalString"]);
                    }
                } else if (node["operands"][i]["instanceValue"]) {
                    if (node["operands"][i]["instanceValue"].IsMap()) {
                        InstanceValue& iv = data.m_manager->create<InstanceValue>();
                        parseInstanceValue(node["operands"][i]["instanceValue"], iv, data);
                        exp.getOperands().add(iv);
                    } else {
                        throw UmlParserException("Improper YAML node type for Expression operands field, ", data.m_path.string(), node["operands"][i]["instanceValue"]);
                    }
                } else {
                    throw UmlParserException("Unknown Value Specification for operand value field, ", data.m_path.string(), node["operands"][i]);
                }
            }
        } else {
            throw UmlParserException("Invalid YAML node type for Expression field operands, must be sequence, ", data.m_path.string(), node["operands"]);
        }
    }
}

void emitExpression(YAML::Emitter& emitter, Expression& exp, EmitterMetaData& data) {
    if (exp.getElementType() == ElementType::EXPRESSION) {
        emitter << YAML::BeginMap << YAML::Key << "expression" << YAML::Value << YAML::BeginMap;
    }

    emitTypedElement(emitter, exp, data);

    if (!exp.getSymbol().empty()) {
        emitter << YAML::Key << "symbol" << YAML::Value << exp.getSymbol();
    }

    if (!exp.getOperands().empty()) {
        emitter << YAML::Key << "operands" << YAML::Value << YAML::BeginSeq;
        for (auto& operand : exp.getOperands()) {
            emit(emitter, operand, data);
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
            throw UmlParserException("Invalid node type fore templateSignature, must be map ", data.m_path.string(), node["templateSignature"]);
        }
    }

    if (node["templateBinding"]) {
        if (node["templateBinding"].IsMap()) {
            TemplateBinding& binding = data.m_manager->create<TemplateBinding>();
            parseTemplateBinding(node["templateBinding"], binding, data);
            el.setTemplateBinding(&binding);
        } else {
            throw UmlParserException("Invalid YAML node for templateBinding definition, ", data.m_path.string(), node["templateBinding"]);
        }
    }
}

void emitTemplateableElement(YAML::Emitter& emitter, TemplateableElement& el, EmitterMetaData& data) {
    if (el.getOwnedTemplateSignature() != 0) {
        emitTemplateSignature(emitter, *el.getOwnedTemplateSignature(), data);
    }

    if (el.getTemplateBinding() != 0) {
        emitTemplateBinding(emitter, *el.getTemplateBinding(), data);
    }
}

void AddTemplateParmeterFunctor::operator()(Element& el) const {
    if (el.isSubClassOf(ElementType::TEMPLATE_PARAMETER)) {
        dynamic_cast<TemplateSignature*>(m_el)->getParameter().add(dynamic_cast<TemplateParameter&>(el));
    } else {
        throw UmlParserException("Tried to add parameter to signature that wasn't a parameter! ", "", m_node);
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
                    throw UmlParserException("Invalid node type, templateParameter must be of node type map ", data.m_path.string(), node["ownedParameters"][i]["templateParameter"]);
                }
            }
        } else {
            throw UmlParserException("Invalid node type, ownedParameters must be defined in Sequence Node type ", data.m_path.string(), node["ownedParameters"]);
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
            throw UmlParserException("Invalid node type for template signature parameters, should be a sequence, ", data.m_path.string(), node["parameters"]);
        }
    }
}

void emitTemplateSignature(YAML::Emitter& emitter, TemplateSignature& signature, EmitterMetaData& data) {
    if (signature.getElementType() == ElementType::TEMPLATE_SIGNATURE) {
        emitter << /*YAML::BeginMap <<*/ YAML::Key << "templateSignature" << YAML::Value << YAML::BeginMap;
    }

    emitElement(emitter, signature, data);

    if (!signature.getOwnedParameter().empty()) {
        emitter << YAML::Key << "ownedParameters" << YAML::Value << YAML::BeginSeq;
        for (auto& param: signature.getOwnedParameter()) {
            emitTemplateParameter(emitter, param, data);
        }
        emitter << YAML::EndSeq;
    }

    if (signature.getParameter().size() > signature.getOwnedParameter().size()) {
        emitter << YAML::Key << "parameters" << YAML::Value << YAML::BeginSeq;
        for (auto& param: signature.getParameter()) {
            if (!signature.getOwnedParameter().count(param.getID())) {
                emitter << param.getID().string();
            }
        }
        emitter << YAML::EndSeq;
    }

    if (signature.getElementType() == ElementType::TEMPLATE_SIGNATURE) {
        emitter << YAML::EndMap ;//<< YAML::EndMap;
    }
}

void SetParameteredElementFunctor::operator()(Element& el) const {
    if (el.isSubClassOf(ElementType::PARAMETERABLE_ELEMENT)) {
        dynamic_cast<TemplateParameter*>(m_el)->setParameteredElement(dynamic_cast<ParameterableElement*>(&el));
    } else {
        throw UmlParserException("Tried to assign non-parameterable element to TemplateParameter parameteredElement!", "", m_node);
    }
}

ParameterableElement& determinAndParseParameterableElement(YAML::Node node, ParserMetaData& data) {
    if (node["activity"]) {
        // TODO
    } else if (node["association"]) {
        // TODO
    } else if (node["class"]) {
        if (node["class"].IsMap()) {
            Class& clazz = data.m_manager->create<Class>();
            parseClass(node["class"], clazz, data);
            return clazz;
        } else {
            throw UmlParserException("class definition must be of node type Map! ", data.m_path.string(), node["class"]);
        }
    } else if (node["dataType"]) {
        if (node["dataType"].IsMap()) {
            DataType& dataType = data.m_manager->create<DataType>();
            parseDataType(node["dataType"], dataType, data);
            return dataType;
        } else {
            throw UmlParserException("dataType definition must be of node type Map! ", data.m_path.string(), node["dataType"]);
        }
    } else if (node["enumeration"]) {
        if (node["enumeration"].IsMap()) {
            Enumeration& enumeration = data.m_manager->create<Enumeration>();
            parseEnumeration(node["enumeration"], enumeration, data);
            return enumeration;
        } else {
            throw UmlParserException("enumeration definition must be of node type Map! ", data.m_path.string(), node["enumeration"]);
        }
    } else if (node["enumerationLiteral"]) {
        if (node["enumerationLiteral"].IsMap()) {
            EnumerationLiteral& literal = data.m_manager->create<EnumerationLiteral>();
            parseEnumerationLiteral(node["enumerationLiteral"], literal, data);
            return literal;
        } else {
            throw UmlParserException("enumerationLiteral definition must be of node type Map! ", data.m_path.string(), node["enumerationLiteral"]);
        }
    } else if (node["instanceSpecification"]) {
        if (node["instanceSpecification"].IsMap()) {
            InstanceSpecification& inst = data.m_manager->create<InstanceSpecification>();
            parseInstanceSpecification(node["instanceSpecification"], inst, data);
            return inst;
        } else {
            throw UmlParserException("instanceSpecification definition must be of node type Map! ", data.m_path.string(), node["instanceSpecification"]);
        }
    } else if (node["instanceValue"]) {
        if (node["instanceValue"].IsMap()) {
            InstanceValue& inst = data.m_manager->create<InstanceValue>();
            parseInstanceValue(node["instanceValue"], inst, data);
            return inst;
        } else {
            throw UmlParserException("instanceValue definition must be of node type Map! ", data.m_path.string(), node["instanceValue"]);
        }
    } else if (node["literalBool"]) {
        if (node["literalBool"].IsMap()) {
            LiteralBool& lb = data.m_manager->create<LiteralBool>();
            parseLiteralBool(node["literalBool"], lb, data);
            return lb;
        } else {
            throw UmlParserException("literalBool definition must be of node type Map! ", data.m_path.string(), node["literalBool"]);
        }
    } else if (node["literalInt"]) {
        if (node["literalInt"].IsMap()) {
            LiteralInt& li = data.m_manager->create<LiteralInt>();
            parseLiteralInt(node["literalInt"], li, data);
            return li;
        } else {
            throw UmlParserException("literalInt definition must be of node type Map! ", data.m_path.string(), node["literalInt"]);
        }
    } else if (node["literalNull"]) {
        if (node["literalNull"].IsMap()) {
            LiteralNull& ln = data.m_manager->create<LiteralNull>();
            parseTypedElement(node["literalNull"], ln, data);
            return ln;
        } else {
            throw UmlParserException("literalNull definition must be of node type Map! ", data.m_path.string(), node["literalNull"]);
        }
    } else if (node["literalString"]) {
        if (node["literalString"].IsMap()) {
            LiteralString& ls = data.m_manager->create<LiteralString>();
            parseLiteralString(node["literalString"], ls, data);
            return ls;
        } else {
            throw UmlParserException("literalString definition must be of node type Map! ", data.m_path.string(), node["literalString"]);
        }
    } else if (node["literalUnlimitedNatural"]) {
        if (node["literalUnlimitedNatural"].IsMap()) {
            LiteralUnlimitedNatural& lu = data.m_manager->create<LiteralUnlimitedNatural>();
            parseLiteralUnlimitedNatural(node["literalUnlimitedNatural"], lu, data);
            return lu;
        } else {
            throw UmlParserException("literalUnlimitedNatural definition must be of node type Map! ", data.m_path.string(), node["literalUnlimitedNatural"]);
        }
    } else if (node["model"]) {
        if (node["model"].IsMap()) {
            Model& m = data.m_manager->create<Model>();
            parsePackage(node["model"], m, data);
            return m;
        } else {
            throw UmlParserException("model definition must be of node type Map! ", data.m_path.string(), node["model"]);
        }
    } else if (node["opaqueBehavior"]) {
        if (node["opaqueBehavior"].IsMap()) {
            OpaqueBehavior& ob = data.m_manager->create<OpaqueBehavior>();
            parseOpaqueBehavior(node["opaqueBehavior"], ob, data);
            return ob;
        } else {
            throw UmlParserException("opaqueBehavior definition must be of node type Map! ", data.m_path.string(), node["opaqueBehavior"]);
        }
    } else if (node["operation"]) {
        if (node["operation"].IsMap()) {
            Operation& op = data.m_manager->create<Operation>();
            parseOperation(node["operation"], op, data);
            return op;
        } else {
            throw UmlParserException("operation definition must be of node type Map! ", data.m_path.string(), node["operation"]);
        }
    } else if (node["package"]) {
        if (node["package"].IsMap()) {
            Package& pckg = data.m_manager->create<Package>();
            parsePackage(node["package"], pckg, data);
            return pckg;
        } else {
            throw UmlParserException("package definition must be of node type Map! ", data.m_path.string(), node["package"]);
        }
    } else if (node["parameter"]) {
        if (node["parameter"].IsMap()) {
            Parameter& param = data.m_manager->create<Parameter>();
            parseParameter(node["parameter"], param, data);
            return param;
        } else {
            throw UmlParserException("parameter definition must be of node type Map! ", data.m_path.string(), node["parameter"]);
        }
    } else if (node["primitiveType"]) {
        if (node["primitiveType"].IsMap()) {
            PrimitiveType& prim = data.m_manager->create<PrimitiveType>();
            parsePrimitiveType(node["primitiveType"], prim, data);
            return prim;
        } else {
            throw UmlParserException("primitiveType definition must be of node type Map! ", data.m_path.string(), node["primitiveType"]);
        }
    } else if (node["property"]) {
        if (node["property"].IsMap()) {
            Property& prop = data.m_manager->create<Property>();
            parseProperty(node["property"], prop, data);
            return prop;
        } else {
            throw UmlParserException("property definition must be of node type Map! ", data.m_path.string(), node["property"]);
        }
    } else {
        throw UmlParserException("not a parametered element! ", data.m_path.string(), node);
    }

    ParameterableElement& dumb = data.m_manager->create<ParameterableElement>();
    dumb.setID(ID::nullID());
    return dumb;
}

void parseTemplateParameter(YAML::Node node, TemplateParameter& parameter, ParserMetaData& data) {
    parseElement(node, parameter, data);

    if (node["default"]) {
        if (node["default"].IsScalar()) {
            if (isValidID(node["default"].as<string>())) {
                // TODO
            } else {
                throw UmlParserException("Invalid id, must be 28 character base64 urlsafe encoded string!", data.m_path.string(), node["default"]);
            }
        } else {
            throw UmlParserException("Invalid yaml node type, must be scalar!", data.m_path.string(), node["default"]);
        }
    }

    if (node["ownedParameteredElement"]) {
        if (node["ownedParameteredElement"].IsMap()) {
            parameter.setOwnedParameteredElement(&determinAndParseParameterableElement(node["ownedParameteredElement"], data));
        } else {
            throw UmlParserException("Invalid yaml node type, must be map! ", data.m_path.string(), node["ownedParameteredElement"]);
        }
    }

    if (node["parameteredElement"]) {
        if (node["parameteredElement"].IsScalar()) {
            if (isValidID(node["parameteredElement"].as<string>())) {
                applyFunctor(data, ID::fromString(node["parameteredElement"].as<string>()), new SetParameteredElementFunctor(&parameter, node["parameteredElement"]));
            } else {
                throw UmlParserException("Invalid id for parametered element, must be base64 url safe 28 character string! ", data.m_path.string(), node["parameteredElement"]);
            }
        } else {
            throw UmlParserException("Invalid YAML node type for parameteredElement, must be scalar ", data.m_path.string(), node["parameteredElement"]);
        }
    }
}

void emitTemplateParameter(YAML::Emitter& emitter, TemplateParameter& parameter, EmitterMetaData& data) {
    if (parameter.getElementType() == ElementType::TEMPLATE_PARAMETER) {
        emitter << YAML::BeginMap << YAML::Key << "templateParameter" << YAML::Value << YAML::BeginMap;
    }

    emitElement(emitter, parameter, data);

    if (parameter.getOwnedDefault()) {
        emitter << YAML::Key << "ownedDefault" << YAML::Value;
        emit(emitter, *parameter.getOwnedDefault(), data);
    }

    if (parameter.getDefault() && !parameter.getOwnedDefault()) {
        emitter << YAML::Key << "default" << YAML::Value << parameter.getDefault()->getID().string();
    }

    if (parameter.getOwnedParameteredElement() != 0) {
        emitter << YAML::Key << "ownedParameteredElement" << YAML::Value;
        emit(emitter, parameter, data);
    }

    if (parameter.getParameteredElement() != 0 && parameter.getOwnedParameteredElement() == 0) {
        emitter << YAML::Key << "parameteredElement" << YAML::Value << parameter.getParameteredElement()->getID().string();
    }

    if (parameter.getElementType() == ElementType::TEMPLATE_PARAMETER) {
        emitter << YAML::EndMap << YAML::EndMap;
    }
}

void SetSignatureFunctor::operator()(Element& el) const {
    if (el.isSubClassOf(ElementType::TEMPLATE_SIGNATURE)) {
        dynamic_cast<TemplateBinding*>(m_el)->setSignature(dynamic_cast<TemplateSignature*>(&el));
    } else {
        throw UmlParserException("Tried to set binding signature to not signature, line ", "", m_node);
    }
}

void parseTemplateBinding(YAML::Node node, TemplateBinding& binding, ParserMetaData& data) {
    parseElement(node, binding, data);

    if (node["signature"]) {
        if (node["signature"].IsScalar()) {
            if (isValidID(node["signature"].as<string>())) {
                applyFunctor(data, ID::fromString(node["signature"].as<string>()), new SetSignatureFunctor(&binding, node["signature"]));
            } else {
                throw UmlParserException("TemplateBinding signature not a valid id, must be base64 url safe 28 character string ", data.m_path.string(), node["signature"]);
            }
        } else {
            throw UmlParserException("Invalid yaml node type for templateBinding signature, must be scaler, ", data.m_path.string(), node["signature"]);
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
                        throw UmlParserException("TODO", data.m_path.string(), node["parameterSubstitution"][i]["templateParameterSubstitution"]);
                    }
                } else {
                    throw UmlParserException("ERROR, templateParameterSubstitution must be defined within parameterSubstitution sequence, ", data.m_path.string(), node["signature"]);
                }
            }
        } else {
            throw UmlParserException("Invalid YAML node type, must be sequence, ", data.m_path.string(), node["parameterSubstitution"]);
        }
    }
}

void emitTemplateBinding(YAML::Emitter& emitter, TemplateBinding& binding, EmitterMetaData& data) {
    if (binding.getElementType() == ElementType::TEMPLATE_BINDING) {
        emitter << /*YAML::BeginMap << */YAML::Key << "templateBinding" << YAML::Value << YAML::BeginMap;
    }

    emitElement(emitter, binding, data);

    if (binding.getSignature() != 0) {
        emitter << YAML::Key << "signature" << YAML::Value << binding.getSignature()->getID().string();
    }

    if (!binding.getParameterSubstitution().empty()) {
        emitter << YAML::Key << "parameterSubstitution" << YAML::BeginSeq;
        for (auto& sub: binding.getParameterSubstitution()) {
            emitTemplateParameterSubstitution(emitter, sub, data);
        }
        emitter << YAML::EndSeq;
    }

    if (binding.getElementType() == ElementType::TEMPLATE_BINDING) {
        emitter << YAML::EndMap;// << YAML::EndMap;
    }
}

void SetFormalFunctor::operator()(Element& el) const {
    if (el.isSubClassOf(ElementType::TEMPLATE_PARAMETER)) {
        dynamic_cast<TemplateParameterSubstitution*>(m_el)->setFormal(dynamic_cast<TemplateParameter*>(&el));
    } else {
        throw UmlParserException("TemplateParameterSubstitution formal must be a templateParameter ", "", m_node);
    }
};

void SetActualFunctor::operator()(Element& el) const {
    if (el.isSubClassOf(ElementType::PARAMETERABLE_ELEMENT)) {
        dynamic_cast<TemplateParameterSubstitution*>(m_el)->setActual(dynamic_cast<ParameterableElement*>(&el));
    } else {
        throw UmlParserException("TemplateParameterSubstitution actual must be a parameterableElement", "", m_node);
    }
}

void parseTemplateParameterSubstitution(YAML::Node node, TemplateParameterSubstitution& sub, ParserMetaData& data) {
    parseElement(node, sub, data);

    if (node["formal"]) {
        if (node["formal"].IsScalar()) {
            if (isValidID(node["formal"].as<string>())) {
                applyFunctor(data, ID::fromString(node["formal"].as<string>()), new SetFormalFunctor(&sub, node["formal"]));
            } else {
                throw UmlParserException("Invalid id, must be 28 character base64 urlsafe encoded string!", data.m_path.string(), node["actual"]);
            }
        } else {
            throw UmlParserException("Invalid YAML node type, must be scalar for formal, ", data.m_path.string(), node["formal"]);
        }
    }
    
    if (node["ownedActual"]) {
        if (node["ownedActual"].IsMap()) {
            sub.setOwnedActual(&determinAndParseParameterableElement(node["ownedActual"], data));
        } else {
            throw UmlParserException("invalid yaml node type, must be map!", data.m_path.string(), node["ownedActual"]);
        }
    }

    if (node["actual"]) {
        if (node["actual"].IsScalar()) {
            if (isValidID(node["actual"].as<string>())) {
                applyFunctor(data, ID::fromString(node["actual"].as<string>()), new SetActualFunctor(&sub, node["actual"]));
            } else {
                throw UmlParserException("Invalid id, must be 28 character base64 urlsafe encoded string!", data.m_path.string(), node["actual"]);
            }
        } else {
            throw UmlParserException("Invalid yaml node type, must be scalar!", data.m_path.string(), node["actual"]);
        }
    }
}

void emitTemplateParameterSubstitution(YAML::Emitter& emitter, TemplateParameterSubstitution& sub, EmitterMetaData& data) {
    if (sub.getElementType() == ElementType::TEMPLATE_PARAMETER_SUBSTITUTION) {
        emitter << YAML::BeginMap << YAML::Key << "templateParameterSubstitution" << YAML::BeginMap;
    }

    emitElement(emitter, sub, data);

    if (sub.getFormal() != 0) {
        emitter << YAML::Key << "formal" << YAML::Value << sub.getFormal()->getID().string();
    }

    if (sub.getOwnedActual() != 0) {
        emitter << YAML::Key << "ownedActual" << YAML::Value;
        emit(emitter, *sub.getOwnedActual(), data);
    }

    if (sub.getActual() != 0 && sub.getOwnedActual() == 0) {
        emitter << YAML::Key << "actual" << YAML::Value << sub.getActual()->getID().string();
    }

    if (sub.getElementType() == ElementType::TEMPLATE_PARAMETER_SUBSTITUTION) {
        emitter << YAML::EndMap << YAML::EndMap;
    }
}

void AddMemberEndFunctor::operator()(Element& el) const {
    if (el.isSubClassOf(ElementType::PROPERTY)) {
        dynamic_cast<Association*>(m_el)->getMemberEnds().add(dynamic_cast<Property&>(el));
    } else {
        throw UmlParserException("Invalid element added to association member ends, must be a property!", "", m_node);
    }
}

void parseAssociation(YAML::Node node, Association& association, ParserMetaData& data) {
    parseClassifier(node, association, data);

    if (node["navigableOwnedEnds"]) {
        if (node["navigableOwnedEnds"].IsSequence()) {
            for (size_t i = 0; i < node["navigableOwnedEnds"].size(); i++) {
                if (node["navigableOwnedEnds"][i]["property"]) {
                    if (node["navigableOwnedEnds"][i]["property"].IsMap()) {
                        Property& property = data.m_manager->create<Property>();
                        parseProperty(node["navigableOwnedEnds"][i]["property"], property, data);
                        association.getNavigableOwnedEnds().add(property);
                    } else {
                        throw UmlParserException("Invalid yaml node type, must be map!", data.m_path.string(), node["navigableOwnedEnds"][i]["property"]);
                    }
                }
            }
        } else {
            throw UmlParserException("Invalid yaml node type, must be sequence!", data.m_path.string(), node["navigableOwnedEnds"]);
        }
    }

    if (node["ownedEnds"]) {
        if (node["ownedEnds"].IsSequence()) {
            for (size_t i = 0; i < node["ownedEnds"].size(); i++) {
                if (node["ownedEnds"][i]["property"]) {
                    if (node["ownedEnds"][i]["property"].IsMap()) {
                        Property& property = data.m_manager->create<Property>();
                        parseProperty(node["ownedEnds"][i]["property"], property, data);
                        association.getOwnedEnds().add(property);
                    } else {
                        throw UmlParserException("Invalid yaml node type, must be map!", data.m_path.string(), node["navigableOwnedEnds"][i]["property"]);
                    }
                }
            }
        } else {
            throw UmlParserException("Invalid yaml node type, must be sequence!", data.m_path.string(), node["navigableOwnedEnds"]);
        }
    }

    if (node["memberEnds"]) {
        if (node["memberEnds"].IsSequence()) {
            for (size_t i = 0; i < node["memberEnds"].size(); i++) {
                if (node["memberEnds"][i].IsScalar()) {
                    if (isValidID(node["memberEnds"][i].as<string>())) {
                        applyFunctor(data, ID::fromString(node["memberEnds"][i].as<string>()), new AddMemberEndFunctor(&association, node["memberEnds"][i]));
                    }
                } else {
                    throw UmlParserException("Invalid yaml node type, must be scalar!", data.m_path.string(), node["memberEnds"][i]);
                }
            }
        } else {
            throw UmlParserException("Invalid yaml node type, must be sequence!", data.m_path.string(), node["memberEnds"]);
        }
    } 
}

void emitAssociation(YAML::Emitter& emitter, Association& association, EmitterMetaData& data) {
    if (association.getElementType() == ElementType::ASSOCIATION) {
        emitter << YAML::BeginMap << YAML::Key << "association" << YAML::Value << YAML::BeginMap;
    }

    emitClassifier(emitter, association, data);

    if (!association.getNavigableOwnedEnds().empty()) {
        emitter << YAML::Key << "navigableOwnedEnds" << YAML::Value << YAML::BeginSeq;
        for (auto& end : association.getNavigableOwnedEnds()) {
            emitProperty(emitter, end, data); // todo go to emit, make polymorphic
        }
        emitter << YAML::EndSeq;
    }

    if (!association.getOwnedEnds().size() > association.getNavigableOwnedEnds().size()) {
        emitter << YAML::Key << "ownedEnds" << YAML::Value << YAML::BeginSeq;
        for (auto& end : association.getOwnedEnds()) {
            if (!association.getNavigableOwnedEnds().count(end.getID())) {
                emitProperty(emitter, end, data);
            }
        }
        emitter << YAML::EndSeq;
    }

    if (association.getMemberEnds().size() > association.getOwnedEnds().size()) {
        emitter << YAML::Key << "memberEnds" << YAML::Value << YAML::BeginSeq;
        for (auto& end : association.getMemberEnds()) {
            if (!association.getOwnedEnds().count(end.getID())) {
                emitter << YAML::Value << end.getID().string();
            }
        }
        emitter << YAML::EndSeq;
    }

    if (association.getElementType() == ElementType::ASSOCIATION) {
        emitter << YAML::EndMap << YAML::EndMap;
    }
}

void parseExtension(YAML::Node node, Extension& extension, ParserMetaData& data) {
    parseClassifier(node, extension, data);

    if (node["ownedEnd"]) {
        if (node["ownedEnd"].IsMap()) {
            if (node["ownedEnd"]["extensionEnd"]) {
                if (node["ownedEnd"]["extensionEnd"].IsMap()) {
                    ExtensionEnd& end = data.m_manager->create<ExtensionEnd>();
                    parseProperty(node["ownedEnd"]["extensionEnd"], end, data);
                    extension.setOwnedEnd(&end);
                } else {
                    throw UmlParserException("Invalid yaml node type for extension ownedEnd extensionEnd definition, must be map!", data.m_path.string(), node["ownedEnd"]["extensionEnd"]);
                }
            }
        } else {
            throw UmlParserException("Invalid yaml nodeType for extension ownedEnd, must be map!", data.m_path.string(), node["ownedEnd"]);
        }
    }

    if (node["metaClass"]) {
        if (node["metaClass"].IsScalar()) {
            extension.setMetaClass(elementTypeFromString(node["metaClass"].as<string>()));
        } else {
            throw UmlParserException("Invalid yaml node type for extension MetaClass, must be scalar!", data.m_path.string(), node["metaClass"]);
        }
    }
}

ElementType elementTypeFromString(string eType) {
    if (eType.compare("ACTION") == 0) {
        return ElementType::ACTION;
    } else if (eType.compare("ACTIVITY") == 0) {
        return ElementType::ACTIVITY;
    } else if (eType.compare("ACTIVITY_EDGE") == 0) {
        return ElementType::ACTIVITY_EDGE;
    } else if (eType.compare("ACTIVITY_NODE") == 0) {
        return ElementType::ACTIVITY_NODE;
    } else if (eType.compare("ASSOCIATION") == 0) {
        return ElementType::ASSOCIATION;
    } else if (eType.compare("BEHAVIOR") == 0) {
        return ElementType::BEHAVIOR;
    } else if (eType.compare("BEHAVIORAL_FEATURE") == 0) {
        return ElementType::BEHAVIORAL_FEATURE;
    } else if (eType.compare("CALL_BEHAVIOR_ACTION") == 0) {
        return ElementType::CALL_BEHAVIOR_ACTION;
    } else if (eType.compare("CLASS") == 0) {
        return ElementType::CLASS;
    } else if (eType.compare("CLASSIFIER") == 0) {
        return ElementType::CLASSIFIER;
    } else if (eType.compare("COMMENT") == 0) {
        return ElementType::COMMENT;
    } else if (eType.compare("CONNECTABLE_ELEMENT") == 0) {
        return ElementType::CONNECTABLE_ELEMENT;
    } else if (eType.compare("CONTROL_FLOW") == 0) {
        return ElementType::CONTROL_FLOW;
    } else if (eType.compare("CREATE_OBJECT_ACTION") == 0) {
        return ElementType::CREATE_OBJECT_ACTION;
    } else if (eType.compare("DATA_TYPE") == 0) {
        return ElementType::DATA_TYPE;
    } else if (eType.compare("DECISION_NODE") == 0) {
        return ElementType::DECISION_NODE;
    } else if (eType.compare("DIRECTED_RELATIONSHIP") == 0) {
        return ElementType::DIRECTED_RELATIONSHIP;
    } else if (eType.compare("ELEMENT") == 0) {
        return ElementType::ELEMENT;
    } else if (eType.compare("ENUMERATION") == 0) {
        return ElementType::ENUMERATION;
    } else if (eType.compare("ENUMERATION_LITERAL") == 0) {
        return ElementType::ENUMERATION_LITERAL;
    } else if (eType.compare("EXPRESSION") == 0) {
        return ElementType::EXPRESSION;
    } else if (eType.compare("EXTENSION") == 0) {
        return ElementType::EXTENSION;
    } else if (eType.compare("EXTENSION_END") == 0) {
        return ElementType::EXTENSION_END;
    } else if (eType.compare("FEATURE") == 0) {
        return ElementType::FEATURE;
    } else if (eType.compare("FINAL_NODE") == 0) {
        return ElementType::FINAL_NODE;
    } else if (eType.compare("FORK_NODE") == 0) {
        return ElementType::FORK_NODE;
    } else if (eType.compare("GENERALIZATION") == 0) {
        return ElementType::GENERALIZATION;
    } else if (eType.compare("INITITAL_NODE") == 0) {
        return ElementType::INITIAL_NODE;
    } else if (eType.compare("INPUT_PIN") == 0) {
        return ElementType::INPUT_PIN;
    } else if (eType.compare("INSTANCE_SPECIFICATION") == 0) {
        return ElementType::INSTANCE_SPECIFICATION;
    } else if (eType.compare("INSTANCE_VALUE") == 0) {
        return ElementType::INSTANCE_VALUE;
    } else if (eType.compare("JOIN_NODE") == 0) {
        return ElementType::JOIN_NODE;
    } else if (eType.compare("LITERAL_BOOL") == 0) {
        return ElementType::LITERAL_BOOL;
    } else if (eType.compare("LITERAL_INT") == 0) {
        return ElementType::LITERAL_INT;
    } else if (eType.compare("LITERAL_NULL") == 0) {
        return ElementType::LITERAL_NULL;
    } else if (eType.compare("LITERAL_REAL") == 0) {
        return ElementType::LITERAL_REAL;
    } else if (eType.compare("LITERAL_SPECIFICATION") == 0) {
        return ElementType::LITERAL_SPECIFICATION;
    } else if (eType.compare("LITERAL_STRING") == 0) {
        return ElementType::LITERAL_STRING;
    } else if (eType.compare("LITERAL_UNLIMITED_NATURAL") == 0) {
        return ElementType::LITERAL_UNLIMITED_NATURAL;
    } else if (eType.compare("MERGE_NODE") == 0) {
        return ElementType::MERGE_NODE;
    } else if (eType.compare("MODEL") == 0) {
        return ElementType::MODEL;
    } else if (eType.compare("MULTIPLICITY_ELEMENT") == 0) {
        return ElementType::MULTIPLICITY_ELEMENT;
    } else if (eType.compare("NAMED_ELEMENT") == 0) {
        return ElementType::NAMED_ELEMENT;
    } else if (eType.compare("NAMESPACE") == 0) {
        return ElementType::NAMESPACE;
    } else if (eType.compare("OBJECT_FLOW") == 0) {
        return ElementType::OBJECT_FLOW;
    } else if (eType.compare("OBJECT_NODE") == 0) {
        return ElementType::OBJECT_NODE;
    } else if (eType.compare("OPAQUE_BEHAVIOR") == 0) {
        return ElementType::OPAQUE_BEHAVIOR;
    } else if (eType.compare("OPERATION") == 0) {
        return ElementType::OPERATION;
    } else if (eType.compare("OUTPUT_PIN") == 0) {
        return ElementType::OUTPUT_PIN;
    } else if (eType.compare("PACKAGE") == 0) {
        return ElementType::PACKAGE;
    } else if (eType.compare("PACKAGEABLE_ELEMENT") == 0) {
        return ElementType::PACKAGEABLE_ELEMENT;
    } else if (eType.compare("PACKAGE_MERGE") == 0) {
        return ElementType::PACKAGE_MERGE;
    } else if (eType.compare("PARAMETER") == 0) {
        return ElementType::PARAMETER;
    } else if (eType.compare("PARAMETERABLE_ELEMENT") == 0) {
        return ElementType::PARAMETERABLE_ELEMENT;
    } else if (eType.compare("PARAMETER_NODE") == 0) {
        return ElementType::PARAMETER_NODE;
    } else if (eType.compare("PIN") == 0) {
        return ElementType::PIN;
    } else if (eType.compare("PRIMITIVE_TYPE") == 0) {
        return ElementType::PRIMITIVE_TYPE;
    } else if (eType.compare("PROFILE") == 0) {
        return ElementType::PROFILE;
    } else if (eType.compare("PROFILE_APPLICATION") == 0) {
        return ElementType::PROFILE_APPLICATION;
    } else if (eType.compare("PROPERTY") == 0) {
        return ElementType::PROPERTY;
    } else if (eType.compare("REDEFINABLE_ELEMENT") == 0) {
        return ElementType::REDEFINABLE_ELEMENT;
    } else if (eType.compare("RELATIONSHIP") == 0) {
        return ElementType::RELATIONSHIP;
    } else if (eType.compare("SLOT") == 0) {
        return ElementType::SLOT;
    } else if (eType.compare("STEREOTYPE") == 0) {
        return ElementType::STEREOTYPE;
    } else if (eType.compare("STRUCTURAL_FEATURE") == 0) {
        return ElementType::STRUCTURAL_FEATURE;
    } else if (eType.compare("STRUCTURED_CLASSIFIER") == 0) {
        return ElementType::STRUCTURED_CLASSIFIER;
    } else if (eType.compare("TEMPLATEABLE_ELEMENT") == 0) {
        return ElementType::TEMPLATEABLE_ELEMENT;
    } else if (eType.compare("TEMPLATE_BINDING") == 0) {
        return ElementType::TEMPLATE_BINDING;
    } else if (eType.compare("TEMPLATE_PARAMETER") == 0) {
        return ElementType::TEMPLATE_PARAMETER;
    } else if (eType.compare("TEMPLATE_PARAMETER_SUBSTITUTION") == 0) {
        return ElementType::TEMPLATE_PARAMETER_SUBSTITUTION;
    } else if (eType.compare("TEMPLATE_SIGNATURE") == 0) {
        return ElementType::TEMPLATE_SIGNATURE;
    } else if (eType.compare("TYPE") == 0) {
        return ElementType::TYPE;
    } else if (eType.compare("TYPED_ELEMENT") == 0) {
        return ElementType::TYPED_ELEMENT;
    } else if (eType.compare("VALUE_SPECIFICATION") == 0) {
        return ElementType::VALUE_SPECIFICATION;
    } 
    throw UmlParserException("Could not identify entity type by keyword: " + eType + '!', "");
}

void emitExtension(YAML::Emitter& emitter, Extension& extension, EmitterMetaData& data) {
    if (extension.getElementType() == ElementType::EXTENSION) {
        emitter << YAML::BeginMap << YAML::Key << "extension" << YAML::Value << YAML::BeginMap;
    }

    emitClassifier(emitter, extension, data);

    emitter << YAML::Key << "metaClass" << YAML::Value << Element::elementTypeToString(extension.getMetaClass());

    if (extension.getOwnedEnd() != 0) {
        emitter << YAML::Key << "ownedEnd" << YAML::Value << YAML::BeginMap << YAML::Key << "extensionEnd" << YAML::Value << YAML::BeginMap;
        emitProperty(emitter, *extension.getOwnedEnd(), data);
        emitter << YAML::EndMap << YAML::EndMap;
    }

    if (extension.getElementType() == ElementType::EXTENSION) {
        emitter << YAML::EndMap << YAML::EndMap;
    }
}

void SetAppliedProfileFunctor::operator()(Element& el) const {
    if (el.isSubClassOf(ElementType::PROFILE)) {
        dynamic_cast<ProfileApplication*>(m_el)->setAppliedProfile(&dynamic_cast<Profile&>(el));
    } else {
        throw UmlParserException("Tried to set applied profile to non profile", "", m_node);
    }
}

void parseProfileApplication(YAML::Node node, ProfileApplication& application, ParserMetaData& data) {
    parseElement(node, application, data);

    if (node["appliedProfile"]) {
        if (node["appliedProfile"].IsScalar()) {
            string profileString = node["appliedProfile"].as<string>();
            if (isValidID(profileString)) {
                applyFunctor(data, ID::fromString(profileString), new SetAppliedProfileFunctor(&application, node["appliedProfile"]));
            } else {
                Element* profile = parseExternalAddToManager(data, profileString);
                if (profile == 0) {
                    throw UmlParserException("Could not parse external profile!", data.m_path.string(), node["appliedProfile"]);
                }
                if (profile->isSubClassOf(ElementType::PROFILE)) {
                    application.setAppliedProfile(dynamic_cast<Profile*>(profile));
                } else {
                    throw UmlParserException("File for applied profile is not root element type profile", data.m_path.string(), node["appliedProfile"]);
                }
            }
        }
    }
}

void emitProfileApplication(YAML::Emitter& emitter, ProfileApplication& application, EmitterMetaData& data) {
    if (application.getElementType() == ElementType::PROFILE_APPLICATION) {
        emitter << YAML::BeginMap << YAML::Key << "profileApplication" << YAML::Value << YAML::BeginMap;
    }

    emitElement(emitter, application, data);

    if (application.getAppliedProfile() != 0) {
        filesystem::path path = data.m_manager->getPath(application.getAppliedProfile()->getID());
        if (path.empty() || path == data.m_path / data.m_fileName) {
            emitter << YAML::Key << "appliedProfile" << YAML::Value << application.getAppliedProfile()->getID().string();
        } else {
            emitToFile(*application.getAppliedProfile(), data, path.parent_path(), path.filename());
            if (data.m_path == path.parent_path()) {
                emitter << YAML::Key << "appliedProfile" << YAML::Value << path.filename();
            }
        }
    }

    if (application.getElementType() == ElementType::PROFILE_APPLICATION) {
        emitter << YAML::EndMap << YAML::EndMap;
    }
}

void parseComment(YAML::Node node, Comment& comment, ParserMetaData& data) {
    parseElement(node, comment, data);

    if (node["body"]) {
        if (node["body"].IsScalar()) {
            comment.setBody(node["body"].as<string>());
        } else {
            throw UmlParserException("Invalid yaml node type for comment body, must be scalar!", data.m_path.string(), node["body"]);
        }
    }
}

void emitComment(YAML::Emitter& emitter, Comment& comment, EmitterMetaData& data) {
    if (comment.getElementType() == ElementType::COMMENT) {
        emitter << YAML::BeginMap << YAML::Key << "comment" << YAML::Value << YAML::BeginMap;
    }

    emitElement(emitter, comment, data);

    if (!comment.getBody().empty()) {
        emitter << YAML::Key << "body" << YAML::Value << comment.getBody();
    }

    if (comment.getElementType() == ElementType::COMMENT) {
        emitter << YAML::EndMap << YAML::EndMap;
    }
}

}

}
}