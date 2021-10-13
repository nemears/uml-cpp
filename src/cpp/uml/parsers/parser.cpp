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
#include "uml/slot.h"
#include "uml/packageMerge.h"
#include "uml/profileApplication.h"
#include "uml/templateSignature.h"
#include "uml/parameterableElement.h"
#include "uml/templateParameter.h"
#include "uml/templateParameterSubstitution.h"
#include "uml/dataType.h"
#include "uml/enumeration.h"
#include "uml/expression.h"
#include "uml/instanceSpecification.h"
#include "uml/instanceValue.h"
#include "uml/literalBool.h"
#include "uml/literalInt.h"
#include "uml/literalReal.h"
#include "uml/literalString.h"
#include "uml/literalUnlimitedNatural.h"
#include "uml/opaqueBehavior.h"
#include "uml/operation.h"
#include "uml/parameter.h"
#include "uml/primitiveType.h"
#include "uml/association.h"
#include "uml/extension.h"
#include "uml/generalization.h"
#include "uml/activity.h"
#include "uml/enumerationLiteral.h"
#include "uml/templateBinding.h"
#include "uml/dependency.h"
#include "uml/abstraction.h"
#include "uml/realization.h"
#include "uml/usage.h"
#include "uml/deployment.h"
#include "uml/artifact.h"
#include "uml/manifestation.h"
#include "uml/parsers/singletonFunctors.h"

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
    YAML::Node node = YAML::LoadFile(data.m_path.string());
    Element* ret = parseNode(node, data);
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
    emitToFile(*data.m_manager->getRoot(), data, data.m_path.string(), data.m_fileName);
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

SetDefaultValue::SetDefaultValue() {
    m_signature = &Property::m_defaultValue;
}

SetSpecific::SetSpecific() {
    m_signature = &Generalization::m_specific;
}

SetNestingClass::SetNestingClass() {
    m_signature = &Classifier::m_nestingClass;
}

OperationSetClass::OperationSetClass() {
    m_signature = &Operation::m_class;
}

PropertySetClass::PropertySetClass() {
    m_signature = &Property::m_class;
}

SetOwningPackage::SetOwningPackage() {
    m_signature = &PackageableElement::m_owningPackage;
}

PropertySetDataType::PropertySetDataType() {
    m_signature = &Property::m_dataType;
}

OperationSetDataType::OperationSetDataType() {
    m_signature = &Operation::m_dataType;
}

SetOwningElement::SetOwningElement() {
    m_signature = &Comment::m_owningElement;
}

PropertySetArtifact::PropertySetArtifact() {
    m_signature = &Property::m_artifact;
}

OperationSetArtifact::OperationSetArtifact() {
    m_signature = &Operation::m_artifact;
}

ArtifactSetArtifact::ArtifactSetArtifact() {
    m_signature = &Artifact::m_artifact;
}

ManifestationSetArtifact::ManifestationSetArtifact() {
    m_signature = &Manifestation::m_artifact;
}

InstanceSpecificationSetClassifier::InstanceSpecificationSetClassifier() {
    m_signature = &InstanceSpecification::m_classifier;
}

SetOwningInstance::SetOwningInstance() {
    m_signature = &Slot::m_owningInstance;
}

SetDefiningFeature::SetDefiningFeature() {
    m_signature = &Slot::m_definingFeature;
}

SetOwningSlot::SetOwningSlot() {
    m_signature = &ValueSpecification::m_owningSlot;
}

SetSpecification::SetSpecification() {
    m_signature = &InstanceSpecification::m_specification;
}

SetInstance::SetInstance() {
    m_signature = &InstanceValue::m_instance;
}

SetOwningInstanceSpec::SetOwningInstanceSpec() {
    m_signature = &ValueSpecification::m_owningInstanceSpec;
}

SetOwningAssociation::SetOwningAssociation() {
    m_signature = &Property::m_owningAssociation;
}

SetAssociation::SetAssociation() {
    m_signature = &Property::m_association;
}

SetType::SetType() {
    m_signature = &TypedElement::m_type;
}

SetOperation::SetOperation() {
    m_signature = &Parameter::m_operation;
}

BehaviorSetSpecification::BehaviorSetSpecification() {
    m_signature = &Behavior::m_specification;
}

SetBehavioredClassifier::SetBehavioredClassifier() {
    m_signature = &Behavior::m_behavioredClassifier;
}

SetBehavior::SetBehavior() {
    m_signature = &Parameter::m_behavior;
}

SetEnumeration::SetEnumeration() {
    m_signature = &EnumerationLiteral::m_enumeration;
}

SetExpression::SetExpression() {
    m_signature = &ValueSpecification::m_expression;
}

namespace {

/**
 * This function is used to parse an element definition contained within a seperate 
 * file, and then add that element to the designated sequence. This should only be 
 * called with sequences that subset  Element::getOwnedElements
 * @param node, the node containing the element defenition file
 * @param data, the data for this parsing session
 * @param el, the element that owns the element being parsed
 * @param signature, the signature of the sequence to add the parsed element to
 **/
template <class T = Element, class U = Element> void parseAndAddToSequence(YAML::Node node, ParserMetaData& data, U& el, Sequence<T>& (U::* signature)()) {
    if (data.m_strategy == ParserStrategy::WHOLE) {
        Element* packagedEl = parseExternalAddToManager(data, node.as<std::string>());
        if (packagedEl == 0) {
            throw UmlParserException("Could not identify YAML node for packaged elements", data.m_path.string(), node);
        }
        (el.*signature)().add(*dynamic_cast<T*>(packagedEl));
    } else {
        std::string path = node.as<std::string>();
        std::string idStr = path.substr(path.find_last_of("/") + 1, path.find_last_of("/") + 29);
        if (isValidID(idStr)) {
            ID id = ID::fromString(idStr);
            if (data.m_manager->loaded(id)) {
                (el.*signature)().add(data.m_manager->get<T>(id)); // Too slow? makes it easier
            } else {
                (el.*signature)().addByID(id);
            }
        } else {
            throw UmlParserException("Invalid id for path, was the data specified as individual, that can only work on a mount!", data.m_path.string(), node);
        }
    }
}

/**
 * This function is used to parse a sequence that subsets Element::getOwnedElements
 * @param node, the node of the element who's sequence we're parsing
 * @param data, the data for this parsing session
 * @param key, the key for the sequence to index the node
 * @param owner, the element that owns all elements in the sequence being parsed
 * @param sequenceSignature, the signature of the sequence we are adding to
 * @param parserSignature, the signature of the function we are using to parse it's children in WHOLE parser strategy mode
 **/
template <class T = Element, class U = Element> void parseSequenceDefinitions(YAML::Node node, ParserMetaData& data, string key, U& owner, Sequence<T>& (U::*sequenceSignature)(), T& (*parserSignature)(YAML::Node, ParserMetaData&)) {
    if (node[key]) {
        if (node[key].IsSequence()) {
            for (size_t i = 0; i < node[key].size(); i++) {
                if (node[key][i].IsMap()) {
                    (owner.*sequenceSignature)().add((*parserSignature)(node[key][i], data));
                }
                else if (node[key][i].IsScalar()) {
                    parseAndAddToSequence(node[key][i], data, owner, sequenceSignature);
                }
                else {
                    throw UmlParserException("Invalid yaml node type for " + key + " entry, must be a scalar or map!", data.m_path.string(), node[key][i]);
                }
            }
        }
        else {
            throw UmlParserException("Invalid YAML node type for field " + key + ", must be sequence, ", data.m_path.string(), node[key]);
        }
    }
}

template <class T = Element> T& parseDefinition(YAML::Node node, ParserMetaData& data, string key, void (*parser)(YAML::Node, T&, ParserMetaData&)) {
    if (node[key].IsMap()) {
        T& ret = data.m_manager->create<T>();
        parser(node[key], ret, data);
        return ret;
    } else {
        throw UmlParserException("Invalid yaml node type for " + key + " definition, it must be a map!", data.m_path.string(), node[key]);
    }
}

// TODO delete?
template <class T =Element> T& parseScalar(YAML::Node node, ParserMetaData& data) {
    if (data.m_strategy == ParserStrategy::WHOLE) {
        Element* packagedEl = parseExternalAddToManager(data, node.as<std::string>());
        if (packagedEl == 0) {
            throw UmlParserException("Could not identify YAML node for packaged elements" , data.m_path.string(), node);
        }
        return dynamic_cast<T&>(*packagedEl);
    } else {
        std::string path = node.as<std::string>();
        std::string idStr = path.substr(path.find_last_of("/") + 1, path.find_last_of("/") + 29);
        if (isValidID(idStr)) {
            ID id = ID::fromString(idStr);
            if (data.m_manager->loaded(id)) {
                return data.m_manager->get<T>(id);
            } else {
                throw UmlParserException("TODO: fix: if it isn't loaded it will recursively call untill all of lower tree is loaded, aka bad.", data.m_path.string(), node);
            }
        } else {
            throw UmlParserException("Invalid id for path, was the data specified as individual, that can only work on a mount!", data.m_path.string(), node);
        }
    }
}

// Helper function for parsing scope in parseNode
template <class T = Element, class U = Element> void parseSingleton(YAML::Node node, ParserMetaData& data, U& el, void (U::*setter)(T&), parseAndSetSingletonFunctor<T, U>& func) {
    ID id = ID::fromString(node.as<string>());
    if (data.m_manager->loaded(id)) {
        (el.*setter)(data.m_manager->get<T>(id));
    }
    else {
        func(node, data, el);
    }
}

Element* parseNode(YAML::Node node, ParserMetaData& data) {
    Element* ret = 0;

    if (node["association"]) {
        Association& association = data.m_manager->create<Association>();
        parseAssociation(node["association"], association, data);
        ret = &association;
    }

    if (node["artifact"]) {
        if (node["artifact"].IsMap()) {
            Artifact& artifact = data.m_manager->create<Artifact>();
            parseArtifact(node["artifact"], artifact, data);
            ret = &artifact;
        }
    }

    if (node["class"]) {
        if (node["class"].IsMap()) {
            Class& clazz = data.m_manager->create<Class>();
            parseClass(node["class"], clazz, data);
            ret = &clazz;
        }
    }

    if (node["comment"]) {
        if (node["comment"].IsMap()) {
            Comment& comment = data.m_manager->create<Comment>();
            parseComment(node["comment"], comment, data);
            ret = &comment;
        }
    }

    if (node["dataType"]) {
        if (node["dataType"].IsMap()) {
            DataType& dataType = data.m_manager->create<DataType>();
            parseDataType(node["dataType"], dataType, data);
            ret = &dataType;
        }
    }

    if (node["dependency"]) {
        Dependency& dependency = data.m_manager->create<Dependency>();
        parseDependency(node["dependency"], dependency, data);
        ret = &dependency;
    }

    if (node["enumeration"]) {
        if (node["enumeration"].IsMap()) {
            Enumeration& enumeration = data.m_manager->create<Enumeration>();
            parseEnumeration(node["enumeration"], enumeration, data);
            ret = &enumeration;
        }
    }

    if (node["enumerationLiteral"]) {
        EnumerationLiteral& enumerationLiteral = data.m_manager->create<EnumerationLiteral>();
        parseEnumerationLiteral(node["enumerationLiteral"], enumerationLiteral, data);
        ret = &enumerationLiteral;
    }

    if (node["expression"]) {
        if (node["expression"].IsMap()) {
            Expression& exp = data.m_manager->create<Expression>();
            parseExpression(node["expression"], exp, data);
            ret = &exp;
        }
    }

    if (node["generalization"]) {
        Generalization& generalization = data.m_manager->create<Generalization>();
        parseGeneralization(node["generalization"], generalization, data);
        ret = &generalization;
    }

    if (node["instanceSpecification"]) {
        InstanceSpecification& inst = data.m_manager->create<InstanceSpecification>();
        parseInstanceSpecification(node["instanceSpecification"], inst, data);
        ret = &inst;
    }

    if (node["instanceValue"]) {
        InstanceValue& instVal = data.m_manager->create<InstanceValue>();
        parseInstanceValue(node["instanceValue"], instVal, data);
        ret = &instVal;
    }

    if (node["literalBool"]) {
        LiteralBool& lb = data.m_manager->create<LiteralBool>();
        parseLiteralBool(node["literalBool"], lb, data);
        ret = &lb;
    }

    if (node["literalInt"]) {
        LiteralInt& li = data.m_manager->create<LiteralInt>();
        parseLiteralInt(node["literalInt"], li, data);
        ret = &li;
    }

    if (node["literalNull"]) {
        LiteralNull& ln = data.m_manager->create<LiteralNull>();
        parseTypedElement(node["literalNull"], ln, data);
        ret = &ln;
    }

    if (node["literalReal"]) {
        LiteralReal& lr = data.m_manager->create<LiteralReal>();
        parseLiteralReal(node["literalReal"], lr, data);
        ret = &lr;
    }

    if (node["literalString"]) {
        LiteralString& ls = data.m_manager->create<LiteralString>();
        parseLiteralString(node["literalString"], ls, data);
        ret = &ls;
    }

    if (node["literalUnlimitedNatural"]) {
        LiteralUnlimitedNatural& ln = data.m_manager->create<LiteralUnlimitedNatural>();
        parseLiteralUnlimitedNatural(node["literalUnlimitedNatural"], ln, data);
        ret = &ln;
    }

    if (node["manifestation"]) {
        Manifestation& manifestation = data.m_manager->create<Manifestation>();
        parseManifestation(node["manifestation"], manifestation, data);
        ret = &manifestation;
    }

    if (node["model"]) {
        Model& model = data.m_manager->create<Model>();
        parsePackage(node["model"], model, data);
        ret = &model;
    }

    if (node["opaqueBehavior"]) {
        OpaqueBehavior& bhv = data.m_manager->create<OpaqueBehavior>();
        parseOpaqueBehavior(node["opaqueBehavior"], bhv, data);
        ret = &bhv;
    }

    if (node["operation"]) {
        if (node["operation"].IsMap()) {
            Operation& op = data.m_manager->create<Operation>();
            parseOperation(node["operation"], op, data);
            ret = &op;
        }
    }

    if (node["package"]) {
        Package& pckg = data.m_manager->create<Package>();
        parsePackage(node["package"], pckg, data);
        ret = &pckg;
    }

    if (node["packageMerge"]) {
        PackageMerge& packageMerge = data.m_manager->create<PackageMerge>();
        parsePackageMerge(node["packageMerge"], packageMerge, data);
        ret = &packageMerge;
    }

    if (node["parameter"]) {
        Parameter& param = data.m_manager->create<Parameter>();
        parseParameter(node["parameter"], param, data);
        ret = &param;
    }

    if (node["primitiveType"]) {
        PrimitiveType& type = data.m_manager->create<PrimitiveType>();
        parsePrimitiveType(node["primitiveType"], type, data);
        ret = &type;
    }

    if (node["profile"]) {
        Profile& profile = data.m_manager->create<Profile>();
        parsePackage(node["profile"], profile, data);
        ret = &profile;
    }

    if (node["profileApplication"]) {
        ProfileApplication& profileApplication = data.m_manager->create<ProfileApplication>();
        parseProfileApplication(node["profileApplication"], profileApplication, data);
        ret = &profileApplication;
    }

    if (node["property"]) {
        Property& prop = data.m_manager->create<Property>();
        parseProperty(node["property"], prop, data);
        ret = &prop;
    }

    if (node["slot"]) {
        Slot& slot = data.m_manager->create<Slot>();
        parseSlot(node["slot"], slot, data);
        ret = &slot;
    }

    if (node["stereotype"]) {
        Stereotype& stereotype = data.m_manager->create<Stereotype>();
        parseClass(node["stereotype"], stereotype, data);
        ret = &stereotype;
    }

    if (ret && data.m_strategy == ParserStrategy::INDIVIDUAL) {
        if (node["owningPackage"]) {
            SetOwningPackage setOwningPackage;
            parseSingleton(node["owningPackage"], data, ret->as<PackageableElement>(), &PackageableElement::setOwningPackage, setOwningPackage);
        }
        if (node["receivingPackage"]) {
            ID receivingPackageID = ID::fromString(node["receivingPackage"].as<string>());
            if (data.m_manager->loaded(receivingPackageID)) {
                ret->as<PackageMerge>().setReceivingPackage(data.m_manager->get<Package>(receivingPackageID));
            } else {
                throw UmlParserException("TODO: fix this, just set id", data.m_path.string(), node["receivingPackage"]);
            }
        }
        if (node["applyingPackage"]) {
            ID applyingPackageID = ID::fromString(node["applyingPackage"].as<string>());
            if (data.m_manager->loaded(applyingPackageID)) {
                ret->as<ProfileApplication>().setApplyingPackage(data.m_manager->get<Package>(applyingPackageID));
            } else {
                throw UmlParserException("TODO: fix this, just set id", data.m_path.string(), node["applyingPackage"]);
            }
        }
        if (node["class"]) {
            if (node["class"].IsScalar()) {
                if (ret->isSubClassOf(ElementType::PROPERTY)) {
                    PropertySetClass setClass;
                    parseSingleton(node["class"], data, ret->as<Property>(), &Property::setClass, setClass);
                } else if (ret->isSubClassOf(ElementType::OPERATION)) {
                    OperationSetClass setClass;
                    parseSingleton(node["class"], data, ret->as<Operation>(), &Operation::setClass, setClass);
                }
            }
        }
        if (node["dataType"]) {
            if (node["dataType"].IsScalar()) {
                if (ret->isSubClassOf(ElementType::PROPERTY)) {
                    PropertySetDataType setDataType;
                    parseSingleton(node["dataType"], data, ret->as<Property>(), &Property::setDataType, setDataType);
                } else if (ret->isSubClassOf(ElementType::OPERATION)) {
                    OperationSetDataType setDataType;
                    parseSingleton(node["dataType"], data, ret->as<Operation>(), &Operation::setDataType, setDataType);
                }
            }
        }

        if (node["artifact"]) {
            if (node["artifact"].IsScalar()) {
                if (ret->isSubClassOf(ElementType::PROPERTY)) {
                    PropertySetArtifact setArtifact;
                    parseSingleton(node["artifact"], data, ret->as<Property>(), &Property::setArtifact, setArtifact);
                } else if (ret->isSubClassOf(ElementType::OPERATION)) {
                    OperationSetArtifact setArtifact;
                    parseSingleton(node["artifact"], data, ret->as<Operation>(), &Operation::setArtifact, setArtifact);
                } else if (ret->isSubClassOf(ElementType::MANIFESTATION)) {
                    ManifestationSetArtifact setArtifact;
                    parseSingleton(node["artifact"], data, ret->as<Manifestation>(), &Manifestation::setArtifact, setArtifact);
                }
            }
        }

        if (node["owningArtifact"]) {
            ArtifactSetArtifact setArtifact;
            parseSingleton(node["owningArtifact"], data, ret->as<Artifact>(), &Artifact::setArtifact, setArtifact);
        }

        if (node["owningProperty"]) {
            ID owningPropertyID = ID::fromString(node["owningProperty"].as<string>());
            if (data.m_manager->loaded(owningPropertyID)) {
                data.m_manager->get<Property>(owningPropertyID).setDefaultValue(ret->as<ValueSpecification>());
            } else {
                throw UmlParserException("TODO: fix this, just set id", data.m_path.string(), node["owningProperty"]);
            }
        }
        if (node["specific"]) {
            SetSpecific setSpecific;
            parseSingleton(node["specific"], data, ret->as<Generalization>(), &Generalization::setSpecific, setSpecific);
        }
        if (node["nestingClass"]) {
            SetNestingClass setNestingClass;
            parseSingleton(node["nestingClass"], data, ret->as<Classifier>(), &Classifier::setNestingClass, setNestingClass);
        }
        if (node["owningElement"]) {
            SetOwningElement setOwningElement;
            parseSingleton(node["owningElement"], data, ret->as<Comment>(), &Comment::setOwningElement, setOwningElement);
        }
        if (node["owningInstance"]) {
            SetOwningInstance setOwningInstance;
            parseSingleton(node["owningInstance"], data, ret->as<Slot>(), &Slot::setOwningInstance, setOwningInstance);
        }
        if (node["owningSlot"]) {
            SetOwningSlot setOwningSlot;
            parseSingleton(node["owningSlot"], data, ret->as<ValueSpecification>(), &ValueSpecification::setOwningSlot, setOwningSlot);
        }
        if (node["owningInstanceSpec"]) {
            SetOwningInstanceSpec setOwningInstanceSpec;
            parseSingleton(node["owningInstanceSpec"], data, ret->as<ValueSpecification>(), &ValueSpecification::setOwningInstanceSpec, setOwningInstanceSpec);
        }
        if (node["owningAssociation"]) {
            SetOwningAssociation setOwningAssociation;
            parseSingleton(node["owningAssociation"], data, ret->as<Property>(), &Property::setOwningAssociation, setOwningAssociation);
        }
        if (node["operation"]) {
            if (node["operation"].IsScalar()) {
                SetOperation setOperation;
                parseSingleton(node["operation"], data, ret->as<Parameter>(), &Parameter::setOperation, setOperation);
            }
        }
        if (node["behavioredClassifier"]) {
            SetBehavioredClassifier setBehavioredClassifier;
            parseSingleton(node["behavioredClassifier"], data, ret->as<Behavior>(), &Behavior::setBehavioredClassifier, setBehavioredClassifier);
        }
        if (node["behavior"]) {
            SetBehavior setBehavior;
            parseSingleton(node["behavior"], data, ret->as<Parameter>(), &Parameter::setBehavior, setBehavior);
        }
        if (node["enumeration"]) {
            if (node["enumeration"].IsScalar()) {
                SetEnumeration setEnumeration;
                parseSingleton(node["enumeration"], data, ret->as<EnumerationLiteral>(), &EnumerationLiteral::setEnumeration, setEnumeration);
            }
        }
        if (node["expression"]) {
            if (node["expression"].IsScalar()) {
                SetExpression setExpression;
                parseSingleton(node["expression"], data, ret->as<ValueSpecification>(), &ValueSpecification::setExpression, setExpression);
            }
        }
    }

    return ret;
}

Element* parseExternalAddToManager(ParserMetaData& data, string path) {
    if (filesystem::exists(data.m_path.parent_path() / path)) {
        filesystem::path cPath = data.m_path;
        data.m_path = cPath.parent_path() / path;
        Element* ret = parse(data);
        data.m_manager->setPath(ret->getID(), data.m_path.string());
        data.m_path = cPath;
        return ret;
    } else {
        return 0;
    }
}

void emit(YAML::Emitter& emitter, Element& el, EmitterMetaData& data) {
    filesystem::path newPath;
    switch (data.m_strategy) {
        case EmitterStrategy::WHOLE : {
            newPath = data.getPath(el.getID());
            break;
        }
        case EmitterStrategy::INDIVIDUAL : {
            newPath = data.m_path / (el.getID().string() + ".yml");
            break;
        }
    }
    if (newPath.empty() || (newPath.parent_path().compare(data.m_path) == 0 && newPath.filename().compare(data.m_fileName) == 0)) {
        determineTypeAndEmit(emitter, el, data);
    } else {
        emitter << YAML::Value << newPath.filename().string();
        if (data.m_strategy == EmitterStrategy::WHOLE) {
            emitToFile(el, data, newPath.parent_path().string(), newPath.filename().string());
        }
    }
}

void determineTypeAndEmit(YAML::Emitter& emitter, Element& el, EmitterMetaData& data) {
    emitter << YAML::BeginMap;
    emitScope(emitter, el, data);
    switch(el.getElementType()) {
        case ElementType::ABSTRACTION : {
            emitElementDefenition(emitter, ElementType::ABSTRACTION, "abstraction", el, data);
            emitDependency(emitter, el.as<Abstraction>(), data);
            emitElementDefenitionEnd(emitter, ElementType::ABSTRACTION, el);
            break;
        }
        case ElementType::ARTIFACT : {
            emitArtifact(emitter, el.as<Artifact>(), data);
            break;
        }
        case ElementType::ASSOCIATION : {
            emitAssociation(emitter, dynamic_cast<Association&>(el), data);
            break;
        }
        case ElementType::CLASS : {
            Class& clazz = el.as<Class>();
            emitClass(emitter, clazz, data);
            break;
        }
        case ElementType::COMMENT : {
            emitComment(emitter, el.as<Comment>(), data);
            break;
        }
        case ElementType::DATA_TYPE : {
            emitDataType(emitter, dynamic_cast<DataType&>(el), data);
            break;
        }
        case ElementType::DEPENDENCY : {
            emitDependency(emitter, el.as<Dependency>(), data);
            break;
        }
        case ElementType::DEPLOYMENT : {
            emitDeployment(emitter, el.as<Deployment>(), data);
            break;
        }
        case ElementType::ENUMERATION : {
            emitEnumeration(emitter, dynamic_cast<Enumeration&>(el), data);
            break;
        }
        case ElementType::ENUMERATION_LITERAL : {
            emitEnumerationLiteral(emitter, el.as<EnumerationLiteral>(), data);
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
        case ElementType::EXTENSION_END : {
            emitElementDefenition(emitter, ElementType::EXTENSION_END, "extensionEnd", el, data);
            emitProperty(emitter, el.as<ExtensionEnd>(), data);
            emitElementDefenitionEnd(emitter, ElementType::EXTENSION_END, el);
            break;
        }
        case ElementType::GENERALIZATION : {
            emitGeneralization(emitter, el.as<Generalization>(), data);
            break;
        }
        case ElementType::INSTANCE_SPECIFICATION : {
            InstanceSpecification& inst = el.as<InstanceSpecification>();
            emitInstanceSpecification(emitter, inst, data);
            break;
        }
        case ElementType::INSTANCE_VALUE: {
            InstanceValue& instanceValue = el.as<InstanceValue>();
            emitInstanceValue(emitter, instanceValue, data);
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
            emitElementDefenition(emitter, ElementType::LITERAL_NULL, "literalNull", el, data);
            emitTypedElement(emitter, dynamic_cast<TypedElement&>(el), data);
            emitElementDefenitionEnd(emitter, ElementType::LITERAL_NULL, el);
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
        case ElementType::MANIFESTATION : {
            emitManifestation(emitter, el.as<Manifestation>(), data);
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
        case ElementType::OPERATION : {
            emitOperation(emitter, el.as<Operation>(), data);
            break;
        }
        case ElementType::PACKAGE : {
            Package& pckg = el.as<Package>();
            emitPackage(emitter, pckg, data);
            break;
        }
        case ElementType::PACKAGE_MERGE : {
            PackageMerge& pckgMerge = el.as<PackageMerge>();
            emitPackageMerge(emitter, pckgMerge, data);
            break;
        }
        case ElementType::PARAMETER : {
            emitParameter(emitter, el.as<Parameter>(), data);
            break;
        }
        case ElementType::PRIMITIVE_TYPE : {
            emitPrimitiveType(emitter, dynamic_cast<PrimitiveType&>(el), data);
            break;
        }
        case ElementType::PROPERTY : {
            Property& property = el.as<Property>();
            emitProperty(emitter, property, data);
            break;
        }
        case ElementType::PROFILE : {
            Profile& profile = el.as<Profile>();
            emitElementDefenition(emitter, ElementType::PROFILE, "profile", profile, data);
            emitPackage(emitter, profile, data);
            emitElementDefenitionEnd(emitter, ElementType::PROFILE, profile);
            break;
        }
        case ElementType::PROFILE_APPLICATION : {
            ProfileApplication& profileApplication = el.as<ProfileApplication>();
            emitProfileApplication(emitter, profileApplication, data);
            break;
        }
        case ElementType::REALIZATION : {
            emitElementDefenition(emitter, ElementType::REALIZATION, "realization", el, data);
            emitDependency(emitter, el.as<Realization>(), data);
            emitElementDefenitionEnd(emitter, ElementType::REALIZATION, el);
            break;
        }
        case ElementType::SLOT : {
            emitSlot(emitter, el.as<Slot>(), data);
            break;
        }
        case ElementType::STEREOTYPE : {
            Stereotype& stereotype = el.as<Stereotype>();
            emitElementDefenition(emitter, ElementType::STEREOTYPE, "stereotype", el, data);
            emitClass(emitter, stereotype, data);
            emitElementDefenitionEnd(emitter, ElementType::STEREOTYPE, el);
            break;
        }
        case ElementType::TEMPLATE_BINDING : {
            emitTemplateBinding(emitter, el.as<TemplateBinding>(), data);
            break;
        }
        case ElementType::TEMPLATE_PARAMETER : {
            emitTemplateParameter(emitter, el.as<TemplateParameter>(), data);
            break;
        }
        case ElementType::TEMPLATE_PARAMETER_SUBSTITUTION : {
            emitTemplateParameterSubstitution(emitter, el.as<TemplateParameterSubstitution>(), data);
            break;
        }
        case ElementType::TEMPLATE_SIGNATURE : {
            emitTemplateSignature(emitter, el.as<TemplateSignature>(), data);
            break;
        }
        case ElementType::USAGE : {
            emitElementDefenition(emitter, ElementType::USAGE, "usage", el, data);
            emitDependency(emitter, el.as<Usage>(), data);
            emitElementDefenitionEnd(emitter, ElementType::USAGE, el);
            break;
        }
        default: {
            throw UmlParserException("Error emitting element, element type " + el.getElementTypeString() + " is abstract and cannot be emit", "");
            break;
        }
    }
    emitter << YAML::EndMap;
}

void emitScope(YAML::Emitter& emitter, Element& el, EmitterMetaData& data) {
    if (data.m_strategy == EmitterStrategy::INDIVIDUAL) {
        if (el.isSubClassOf(ElementType::PACKAGEABLE_ELEMENT)) {
            if (el.as<PackageableElement>().hasOwningPackage()) {
                emitter << YAML::Key << "owningPackage" << el.as<PackageableElement>().getOwningPackageID().string();
                return;
            }
        }
        if (el.isSubClassOf(ElementType::PROFILE_APPLICATION)) {
            if (el.as<ProfileApplication>().hasApplyingPackage()) {
                emitter << YAML::Key << "applyingPackage" << YAML::Value << el.as<ProfileApplication>().getApplyingPackageID().string();
                return;
            }
        }
        if (el.isSubClassOf(ElementType::PACKAGE_MERGE)) {
            if (el.as<PackageMerge>().hasReceivingPackage()) {
                emitter << YAML::Key << "receivingPackage" << el.as<PackageMerge>().getReceivingPackageID().string();
                return;
            }
        }
        if (el.isSubClassOf(ElementType::PROPERTY)) {
            if (el.as<Property>().hasClass()) {
                emitter << YAML::Key << "class" << el.as<Property>().getClassID().string();
                return;
            }
            if (el.as<Property>().hasDataType()) {
                emitter << YAML::Key << "dataType" << YAML::Value << el.as<Property>().getDataTypeID().string();
                return;
            }   
            if (el.as<Property>().hasArtifact()) {
                emitter << YAML::Key << "artifact" << YAML::Value << el.as<Property>().getArtifactID().string();
                return;
            }
            if (el.as<Property>().hasOwningAssociation()) {
                emitter << YAML::Key << "owningAssociation" << YAML::Value << el.as<Property>().getOwningAssociationID().string();
                return;
            }
        }
        if (el.isSubClassOf(ElementType::VALUE_SPECIFICATION)) {
            if (el.hasOwner()) {
                if (el.getOwnerRef().isSubClassOf(ElementType::PROPERTY)) {
                    if (el.getOwnerRef().as<Property>().getDefaultValueID() == el.getID()) {
                        emitter << YAML::Key << "owningProperty" << YAML::Value << el.getOwnerID().string();
                        return;
                    }
                }
                if (el.as<ValueSpecification>().hasOwningSlot()) {
                    emitter << YAML::Key << "owningSlot" << YAML::Value << el.as<ValueSpecification>().getOwningSlotID().string();
                    return;
                }
                if (el.as<ValueSpecification>().hasOwningInstanceSpec()) {
                    emitter << YAML::Key << "owningInstanceSpec" << YAML::Value << el.as<ValueSpecification>().getOwningInstanceSpecID().string();
                    return;
                }
                if (el.as<ValueSpecification>().hasExpression()) {
                    emitter << YAML::Key << "expression" << YAML::Value << el.as<ValueSpecification>().getExpressionID().string();
                    return;
                }
            }
        }
        if (el.isSubClassOf(ElementType::GENERALIZATION)) {
            if (el.as<Generalization>().hasSpecific()) {
                emitter << YAML::Key << "specific" << YAML::Value << el.as<Generalization>().getSpecificID().string();
                return;
            }
        }
        if (el.isSubClassOf(ElementType::CLASSIFIER)) {
            if (el.as<Classifier>().hasNestingClass()) {
                emitter << YAML::Key << "nestingClass" << YAML::Value << el.as<Classifier>().getNestingClassID().string();
                return;
            }
        }
        if (el.isSubClassOf(ElementType::OPERATION)) {
            if (el.as<Operation>().hasClass()) {
                emitter << YAML::Key << "class" << YAML::Value << el.as<Operation>().getClassID().string();
                return;
            }
            if (el.as<Operation>().hasDataType()) {
                emitter << YAML::Key << "dataType" << YAML::Value << el.as<Operation>().getDataTypeID().string();
                return;
            }
            if (el.as<Operation>().hasArtifact()) {
                emitter << YAML::Key << "artifact" << YAML::Value << el.as<Operation>().getArtifactID().string();
                return;
            }
        }
        if (el.isSubClassOf(ElementType::COMMENT)) {
            if (el.as<Comment>().hasOwningElement()) {
                emitter << YAML::Key << "owningElement" << YAML::Value << el.as<Comment>().getOwningElementID().string();
                return;
            }
        }
        if (el.isSubClassOf(ElementType::ARTIFACT)) {
            if (el.as<Artifact>().hasArtifact()) {
                emitter << YAML::Key << "owningArtifact" << YAML::Value << el.as<Artifact>().getArtifactID().string();
                return;
            }
        }
        if (el.isSubClassOf(ElementType::MANIFESTATION)) {
            if (el.as<Manifestation>().hasArtifact()) {
                emitter << YAML::Key << "artifact" << YAML::Value << el.as<Manifestation>().getArtifactID().string();
                return;
            }
        }
        if (el.isSubClassOf(ElementType::SLOT)) {
            if (el.as<Slot>().hasOwningInstance()) {
                emitter << YAML::Key << "owningInstance" << YAML::Value << el.as<Slot>().getOwningInstanceID().string();
                return;
            }
        }
        if (el.isSubClassOf(ElementType::PARAMETER)) {
            if (el.as<Parameter>().hasOperation()) {
                emitter << YAML::Key << "operation" << YAML::Value << el.as<Parameter>().getOperationID().string();
                return;
            }
            if (el.as<Parameter>().hasBehavior()) {
                emitter << YAML::Key << "behavior" << YAML::Value << el.as<Parameter>().getBehaviorID().string();
            }
        }
        if (el.isSubClassOf(ElementType::BEHAVIOR)) {
            if (el.as<Behavior>().hasBehavioredClassifier()) {
                emitter << YAML::Key << "behavioredClassifier" << YAML::Value << el.as<Behavior>().getBehavioredClassifierID().string();
                return;
            }
        }
        if (el.isSubClassOf(ElementType::ENUMERATION_LITERAL)) {
            if (el.as<EnumerationLiteral>().hasEnumeration()) {
                emitter << YAML::Key << "enumeration" << YAML::Value << el.as<EnumerationLiteral>().getEnumerationID().string();
            }
        }
    }
}

template <class T = Element, class U = Element> void emitSequence(YAML::Emitter& emitter, string sequenceName, EmitterMetaData& data, U& el, Sequence<T>& (U::* sequenceMethod)()) {
    if (!(el.*sequenceMethod)().empty()) {
        emitter << YAML::Key << sequenceName << YAML::Value << YAML::BeginSeq;
        if (data.m_strategy == EmitterStrategy::WHOLE) {
            for (auto& seqEl : (el.*sequenceMethod)()) {
                emit(emitter, seqEl, data);
            }
        } else {
            for (const ID id : (el.*sequenceMethod)().ids()) {
                emitter << YAML::Value << id.string() + ".yml";
            }
        }
        emitter << YAML::EndSeq;
    }
}

void emitElementDefenition(YAML::Emitter& emitter, ElementType eType, string yamlName, Element& el, EmitterMetaData& data) {
    if (el.getElementType() == eType) {
        emitter << YAML::Key << yamlName << YAML::Value << YAML::BeginMap;
    }
}

void emitElementDefenitionEnd(YAML::Emitter& emitter, ElementType eType, Element& el) {
    if (el.getElementType() == eType) {
        emitter << YAML::EndMap;
    }
}

void emitModel(YAML::Emitter& emitter, Model& model, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::MODEL, "model", model, data);

    emitPackage(emitter, model, data);

    emitElementDefenitionEnd(emitter, ElementType::MODEL, model);
}

void AddAppliedStereotypeFunctor::operator()(Element& el) const {
    // will always be called after setClassifier functor
    if (el.isSubClassOf(ElementType::STEREOTYPE) && m_el->isSubClassOf(ElementType::INSTANCE_SPECIFICATION)) {
        m_stereotypedEl.getAppliedStereotypes().add(dynamic_cast<InstanceSpecification&>(*m_el));
    } else {
        throw UmlParserException("appliedStereotype instance classifier must be a stereotype!", "", m_node);
    }
}

Comment& determineAndParseOwnedComment(YAML::Node node, ParserMetaData& data) {
    if (node["comment"]) {
        Comment& comment = data.m_manager->create<Comment>();
        parseComment(node["comment"], comment, data);
        return comment;
    }
    else {
        throw UmlParserException("Invalid uml definition for comment!", data.m_path.string(), node);
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

    parseSequenceDefinitions(node, data, "ownedComments", el, &Element::getOwnedComments, determineAndParseOwnedComment);

    // special handling with functor
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

    emitSequence(emitter, "ownedComments", data, el, &Element::getOwnedComments);

    emitSequence(emitter, "appliedStereotypes", data, el, &Element::getAppliedStereotypes);
}

void AddClientDepencyFunctor::operator()(Element& el) const {
    m_el->as<NamedElement>().getClientDependencies().add(el.as<Dependency>());
}

void AddSupplierDependencyFunctor::operator()(Element& el) const {
    m_el->as<NamedElement>().getSupplierDependencies().add(el.as<Dependency>());
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

    if (node["clientDependencies"]) {
        if (node["clientDependencies"].IsSequence()) {
            for (size_t i = 0; i < node["clientDependencies"].size(); i++) {
                if (node["clientDependencies"][i].IsScalar()) {
                    ID clientDependencyID = ID::fromString(node["clientDependencies"][i].as<string>());
                    if (data.m_strategy == ParserStrategy::WHOLE) {
                        applyFunctor(data, clientDependencyID, new AddClientDepencyFunctor(&el, node["clientDependencies"][i]));
                    } else {
                        if (data.m_manager->loaded(clientDependencyID)) {
                            el.getClientDependencies().add(data.m_manager->get<Dependency>(clientDependencyID));
                        } else {
                            el.getClientDependencies().addByID(clientDependencyID);
                        }
                    }
                } else {
                    throw UmlParserException("Invalid yaml node type for NamedElement clientDependencies entry, must be a scalar!", data.m_path.string(), node["clientDependencies"][i]);
                }
            }
        } else {
            throw UmlParserException("Improper yaml node type for NamedElement field clientDependencies, must be a sequence!", data.m_path.string(), node["clientDependencies"]);
        }
    }

    if (node["supplierDependencies"]) {
        if (node["supplierDependencies"].IsSequence()) {
            for (size_t i = 0; i < node["supplierDependencies"].size(); i++) {
                if (node["supplierDependencies"][i].IsScalar()) {
                    ID supplierDependencyID = ID::fromString(node["supplierDependencies"][i].as<string>());
                    if (data.m_strategy == ParserStrategy::WHOLE) {
                        applyFunctor(data, supplierDependencyID, new AddSupplierDependencyFunctor(&el, node["supplierDependencies"][i]));
                    } else {
                        if (data.m_manager->loaded(supplierDependencyID)) {
                            el.getSupplierDependencies().add(data.m_manager->get<Dependency>(supplierDependencyID));
                        } else {
                            el.getSupplierDependencies().addByID(supplierDependencyID);
                        }
                    }
                } else {
                    throw UmlParserException("Invalid yaml node type for NamedElement supplierDependencies entry, must be a scalar!", data.m_path.string(), node["supplierDependencies"][i]);
                }
            }
        } else {
            throw UmlParserException("Invalid yaml node type for NamedElement supplierDependencies, must be a sequence!", data.m_path.string(), node["supplierDependencies"]);
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
    if (!el.getClientDependencies().empty()) {
        bool emitClientDependenciesFlag = true;
        bool elIsDeploymentTargetFlag = false;
        bool elIsArtifactFlag = false;
        if (el.isSubClassOf(ElementType::DEPLOYMENT_TARGET)) {
            elIsDeploymentTargetFlag = true;
            if (el.as<DeploymentTarget>().getDeployments().size() == el.getClientDependencies().size()) {
                emitClientDependenciesFlag = false;
            }
        }
        if (el.isSubClassOf(ElementType::ARTIFACT)) {
            elIsArtifactFlag = true;
            if (el.as<Artifact>().getManifestations().size() == el.getClientDependencies().size()) {
                emitClientDependenciesFlag = false;
            }
        }
        if (emitClientDependenciesFlag) {
            emitter << YAML::Key << "clientDependencies" << YAML::Value << YAML::BeginSeq;
            for (const ID id : el.getClientDependencies().ids()) {
                if (elIsDeploymentTargetFlag) {
                    if (el.as<DeploymentTarget>().getDeployments().count(id)) {
                        continue;
                    }
                }
                if (elIsArtifactFlag) {
                    if (el.as<Artifact>().getManifestations().count(id)) {
                        continue;
                    }
                }
                emitter << YAML::Value << id.string();
            }
            emitter << YAML::EndSeq;
        }
    }

    if (!el.getSupplierDependencies().empty()) {
        emitter << YAML::Key << "supplierDependencies" << YAML::Value << YAML::BeginSeq;
        for (const ID id : el.getSupplierDependencies().ids()) {
            emitter << YAML::Value << id.string();
        }
        emitter << YAML::EndSeq;
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
            if (data.m_strategy == ParserStrategy::WHOLE) {
                string typeIDstring = node["type"].as<string>();
                if (isValidID(typeIDstring)) {
                    ID typeID = ID::fromString(typeIDstring);
                    applyFunctor(data, typeID, new SetTypeFunctor(&el, node["type"]));
                } else {
                    throw UmlParserException("ID for " + el.getElementTypeString() + " type field is invalid, ", data.m_path.string(), node["type"]);
                }
            } else {
                SetType setType;
                setType(node["type"], data, el);
            }
        } else {
            throw UmlParserException("Improper YAML node type for type field, must be scalar, " , data.m_path.string() , node["type"]);
        }
    }
}

void emitTypedElement(YAML::Emitter& emitter, TypedElement& el, EmitterMetaData& data) {
    emitNamedElement(emitter, el, data);

    if (el.hasType()) {
        emitter << YAML::Key << "type" << YAML::Value << el.getTypeID().string();
    }
}

Generalization& determineAndParseGeneralization(YAML::Node node, ParserMetaData& data) {
    if (node["generalization"]) {
        Generalization& gen = data.m_manager->create<Generalization>();
        parseGeneralization(node["generalization"], gen, data);
        return gen;
    } else {
        throw UmlParserException("Invalid uml type for generalizations entry!", data.m_path.string(), node);
    }
}

void parseClassifier(YAML::Node node, Classifier& clazz, ParserMetaData& data) {
    parseNamedElement(node, clazz, data);
    parseTemplateableElement(node, clazz, data);
    parseSequenceDefinitions(node, data, "generalizations", clazz, &Classifier::getGeneralizations, determineAndParseGeneralization);
}

void emitClassifier(YAML::Emitter& emitter, Classifier& clazz, EmitterMetaData& data) {
    emitNamedElement(emitter, clazz, data);
    emitTemplateableElement(emitter, clazz, data);
    emitSequence(emitter, "generalizations", data, clazz, &Classifier::getGeneralizations);
}

void SetGeneralFunctor::operator()(Element& el) const {
    if (el.isSubClassOf(ElementType::CLASSIFIER)) {
        dynamic_cast<Generalization*>(m_el)->setGeneral(&dynamic_cast<Classifier&>(el));
    }
}

void parseGeneralization(YAML::Node node, Generalization& general, ParserMetaData& data) {
    parseElement(node, general, data);
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
            throw UmlParserException("Cannot define general within generalization, generalization may own no elements!", data.m_path.string());
        }
    }
}

void emitGeneralization(YAML::Emitter& emitter, Generalization& generalization, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::GENERALIZATION, "generalization", generalization, data);

    emitElement(emitter, generalization, data);

    if (generalization.getGeneral()) {
        emitter << YAML::Key << "general" << YAML::Value << generalization.getGeneral()->getID().string();
    }

    emitElementDefenitionEnd(emitter, ElementType::GENERALIZATION, generalization);
}

Property& determineAndParseOwnedAttribute(YAML::Node node, ParserMetaData& data) {
    if (node["property"]) {
        if (node["property"].IsMap()) {
            Property& prop = data.m_manager->create<Property>();
            parseProperty(node["property"], prop, data);
            return prop;
        } else {
            throw UmlParserException("Invalid yaml node type for property definition, must be a map!", data.m_path.string(), node["property"]);
        }
    } else {
        throw UmlParserException("Invalid uml definition for ownedAttribute, may be a property only!", data.m_path.string(), node);
    }
}

Operation& determineAndParseOwnedOperation(YAML::Node node, ParserMetaData& data) {
    if (node["operation"]) {
        Operation& op = data.m_manager->create<Operation>();
        parseOperation(node["operation"], op, data);
        return op;
    } else {
        throw UmlParserException("Invalid uml definition for ownedOperation, may be an operation only!", data.m_path.string(), node);
    }
}

void parseDataType(YAML::Node node, DataType& dataType, ParserMetaData& data) {
    parseClassifier(node, dataType, data);
    parseSequenceDefinitions(node, data, "ownedAttribute", dataType, &DataType::getOwnedAttribute, determineAndParseOwnedAttribute);
    parseSequenceDefinitions(node, data, "ownedOperation", dataType, &DataType::getOwnedOperation, determineAndParseOwnedOperation);
}

void emitDataType(YAML::Emitter& emitter, DataType& dataType, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::DATA_TYPE, "dataType", dataType, data);
    emitClassifier(emitter, dataType, data);
    emitSequence(emitter, "ownedAttribute", data, dataType, &DataType::getOwnedAttribute);
    emitSequence(emitter, "ownedOperation", data, dataType, &DataType::getOwnedOperation);
    emitElementDefenitionEnd(emitter, ElementType::DATA_TYPE, dataType);
}

void parsePrimitiveType(YAML::Node node, PrimitiveType& type, ParserMetaData& data) {
    parseDataType(node, type, data);
}

void emitPrimitiveType(YAML::Emitter& emitter, PrimitiveType& type, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::PRIMITIVE_TYPE, "primitiveType", type, data);
    emitDataType(emitter, type, data);
    emitElementDefenitionEnd(emitter, ElementType::PRIMITIVE_TYPE, type);
}

void parseStructuredClassifier(YAML::Node node, StructuredClassifier& clazz, ParserMetaData& data) {
    parseClassifier(node, clazz, data);
    parseSequenceDefinitions(node, data, "ownedAttributes", clazz, &StructuredClassifier::getOwnedAttributes, determineAndParseOwnedAttribute);
}

void emitStructuredClassifier(YAML::Emitter& emitter, StructuredClassifier& clazz, EmitterMetaData& data) {
    emitClassifier(emitter, clazz, data);
    emitSequence(emitter, "ownedAttributes", data, clazz, &StructuredClassifier::getOwnedAttributes);
}

Classifier& determineAndParseClassifier(YAML::Node node, ParserMetaData& data) {
    if (node["activity"]) {
        // TODO
    } else if (node["artifact"]) {
        if (node["artifact"].IsMap()) {
            Artifact& artifact = data.m_manager->create<Artifact>();
            parseArtifact(node["artifact"], artifact, data);
            return artifact;
        }  else {
            throw UmlParserException("Invalid yaml node type for enumeration artifact", data.m_path.string(), node["artifact"]);
        }
    } else if (node["association"]) { // is this valid IG, seems weird
        if (node["association"].IsMap()) {
            Association& association = data.m_manager->create<Association>();
            parseAssociation(node["association"], association, data);
            return association;
        } else {
            throw UmlParserException("Invalid yaml node type for association definition", data.m_path.string(), node["association"]);
        }
    } else if (node["class"]) {
        if (node["class"].IsMap()) {
            Class& nestedClazz = data.m_manager->create<Class>();
            parseClass(node["class"], nestedClazz, data);
            return nestedClazz;
        } else {
            throw UmlParserException("Invalid yaml node type for class definition", data.m_path.string(), node["class"]);
        }
    } else if (node["dataType"]) {
        if (node["dataType"].IsMap()) {
            DataType& dataType = data.m_manager->create<DataType>();
            parseDataType(node["dataType"], dataType, data);
            return dataType;
        }  else {
            throw UmlParserException("Invalid yaml node type for dataType definition", data.m_path.string(), node["dataType"]);
        }
    } else if (node["enumeration"]) {
        if (node["enumeration"].IsMap()) {
            Enumeration& enumeration = data.m_manager->create<Enumeration>();
            parseEnumeration(node["enumeration"], enumeration, data);
            return enumeration;
        } else {
            throw UmlParserException("Invalid yaml node type for enumeration definition", data.m_path.string(), node["enumeration"]);
        }
    } else if (node["extension"]) {
        if (node["extension"].IsMap()) {
            Extension& extension = data.m_manager->create<Extension>();
            parseExtension(node["extension"], extension, data);
            return extension;
        } else {
            throw UmlParserException("Invalide yaml node type for extension definition, must be a map!", data.m_path.string(), node["extension"]);
        }
    } else if (node["opaqueBehavior"]) {
        if (node["opaqueBehavior"].IsMap()) {
            OpaqueBehavior& opaqueBehavior = data.m_manager->create<OpaqueBehavior>();
            parseOpaqueBehavior(node["opaqueBehavior"], opaqueBehavior, data);
            return opaqueBehavior;
        } else {
            throw UmlParserException("Invalid yaml node type for opaqueBehavior definition", data.m_path.string(), node["opaqueBehavior"]);
        }
    } else if (node["primitiveType"]) {
        if (node["primitiveType"].IsMap()) {
            PrimitiveType& primitiveType = data.m_manager->create<PrimitiveType>();
            parsePrimitiveType(node["primitiveType"], primitiveType, data);
            return primitiveType;
        } else {
            throw UmlParserException("Invalid yaml node type for primitiveType definition", data.m_path.string(), node["primitiveType"]);
        }
    } else {
        throw UmlParserException("invalid classifier definition for entry!", data.m_path.string(), node);
    }
}

void parseClass(YAML::Node node, Class& clazz, ParserMetaData& data) {
    parseStructuredClassifier(node, clazz, data);
    parseBehavioredClassifier(node, clazz, data);
    parseSequenceDefinitions(node, data, "ownedOperations", clazz, &Class::getOwnedOperations, determineAndParseOwnedOperation);
    parseSequenceDefinitions(node, data, "nestedClassifiers", clazz, &Class::getNestedClassifiers, determineAndParseClassifier);
}

void emitClass(YAML::Emitter& emitter, Class& clazz, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::CLASS, "class", clazz, data);
    emitStructuredClassifier(emitter, clazz, data);
    emitBehavioredClassifier(emitter, clazz, data);
    emitSequence(emitter, "ownedOperations", data, clazz, &Class::getOwnedOperations);
    emitSequence(emitter, "nestedClassifiers", data, clazz, &Class::getNestedClassifiers);
    emitElementDefenitionEnd(emitter, ElementType::CLASS, clazz);
}

Parameter& determineAndParseParameter(YAML::Node node, ParserMetaData& data) {
    if (node["parameter"]) {
        if (node["parameter"].IsMap()) {
            Parameter& param = data.m_manager->create<Parameter>();
            parseParameter(node["parameter"], param, data);
            return param;
        } else {
            throw UmlParserException("Invalid yaml node type for parameter definition, must be a map!", data.m_path.string(), node["parameter"]);
        }
    } else {
        throw UmlParserException("Invalid uml element for parameter, can only be a parameter!", data.m_path.string(), node);
    }
}

void parseBehavior(YAML::Node node, Behavior& bhv, ParserMetaData& data) {
    parseClass(node, bhv, data);
    parseSequenceDefinitions(node, data, "parameters", bhv, &Behavior::getOwnedParameters, determineAndParseParameter);
    if (node["specification"]) {
        if (node["specification"].IsScalar()) {
            BehaviorSetSpecification setSpecification;
            parseSingleton(node["specification"], data, bhv, &Behavior::setSpecification, setSpecification);
        } else {
            throw UmlParserException("Invalid yaml node type for behavior specification field, must be scalar!", data.m_path.string(), node["specification"]);
        }
    }
}

void emitBehavior(YAML::Emitter& emitter, Behavior& bhv, EmitterMetaData& data) {
    emitClass(emitter, bhv, data);
    emitSequence(emitter, "parameters", data, bhv, &Behavior::getOwnedParameters);
    if (bhv.hasSpecification()) {
        emitter << YAML::Key << "specification" << YAML::Value << bhv.getSpecificationID().string();
    }
}

void parseOpaqueBehavior(YAML::Node node, OpaqueBehavior& bhv, ParserMetaData& data) {
    parseBehavior(node, bhv, data);

    // keep special handling for now
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
    emitElementDefenition(emitter, ElementType::OPAQUE_BEHAVIOR, "opaqueBehavior", bhv, data);
    emitBehavior(emitter, bhv, data);
    emitSequence(emitter, "bodies", data, bhv, &OpaqueBehavior::getBodies);
    emitElementDefenitionEnd(emitter, ElementType::OPAQUE_BEHAVIOR, bhv);
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
    parseDeploymentTarget(node, prop, data);

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
        } else if (node["defaultValue"].IsScalar()) {
            SetDefaultValue setDefaultValue;
            setDefaultValue(node["defaultValue"], data, prop);
        } else {
            throw UmlParserException("Invalid yaml node type for property default value entry, must be Map or scalar!", data.m_path.string(), node["defaultValue"]);
        }
    }

    if (node["redefinedProperties"]) {
        if (node["redefinedProperties"].IsSequence()) {
            for (size_t i = 0; i < node["redefinedProperties"].size(); i++) {
                if (node["redefinedProperties"][i].IsScalar()) {
                    if (isValidID(node["redefinedProperties"][i].as<string>())) {
                        ID redefinedID = ID::fromString(node["redefinedProperties"][i].as<string>());
                        prop.getRedefinedProperties().add(data.m_manager->get<Property>(redefinedID));
                    } else {
                        throw UmlParserException("Invalid uml id for redefinedProperties reference, must be a 28 character base64 url safe string!", data.m_path.string(), node["redefinedProperties"][i]);
                    }
                } else {
                    throw UmlParserException("Invalid yaml node type for redefinedProperties reference, must be a scalar!", data.m_path.string(), node["redefinedProperties"][i]);
                }
            }
        } else {
            throw UmlParserException("Invalid yaml node type for redefinedProperties specification, must be a sequence!", data.m_path.string(), node["redefinedProperties"]);
        }
    }

    if (node["association"]) {
        if (node["association"].IsScalar()) {
            SetAssociation setAssociation;
            setAssociation(node["association"], data, prop);
        } else {
            throw UmlParserException("Invalid yaml node type for association field, must be a scalar!", data.m_path.string(), node["redefinedProperties"]);
        }
    }
}

void emitProperty(YAML::Emitter& emitter, Property& prop, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::PROPERTY, "property", prop, data);

    emitTypedElement(emitter, prop, data);
    emitMultiplicityElement(emitter, prop, data);
    emitDeploymentTarget(emitter, prop, data);

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
        emitter << YAML::Key << "defaultValue" << YAML::Value;
        emit(emitter, *prop.getDefaultValue(), data);
    }

    if (!prop.getRedefinedProperties().empty()) {
        emitter << YAML::Key << "redefinedProperties" << YAML::Value << YAML::BeginSeq;
        for (auto& redefined: prop.getRedefinedProperties()) {
            emitter << YAML::Value << redefined.getID().string();
        }
        emitter << YAML::EndSeq;
    }

    if (prop.hasAssociation()) {
        emitter << YAML::Key << "association" << YAML::Value << prop.getAssociationID().string();
    }

    emitElementDefenitionEnd(emitter, ElementType::PROPERTY, prop);
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
    emitElementDefenition(emitter, ElementType::PARAMETER, "parameter", el, data);

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

    emitElementDefenitionEnd(emitter, ElementType::PARAMETER, el);
}

void parseOperation(YAML::Node node, Operation& op, ParserMetaData& data) {
    parseNamedElement(node, op, data);
    parseTemplateableElement(node, op, data);

    // TODO: maybe move all this to new function parseBehavioralFeature once the other ones are implemented
    if (node["methods"]) {
        if (node["methods"].IsSequence()) {
            for (size_t i=0; i<node["methods"].size(); i++) {
                if (node["methods"][i].IsScalar()) {
                    ID methodID = ID::fromString(node["methods"][i].as<string>());
                    if (data.m_manager->loaded(methodID)) {
                        op.getMethods().add(data.m_manager->get<Behavior>(methodID));
                    } else {
                        op.getMethods().addByID(methodID);
                    }
                } else {
                    throw UmlParserException("Invalid yaml node type for operation method entry, must be a scalar!", data.m_path.string(), node["methods"][i]);
                }
            }
        } else {
            throw UmlParserException("Improper YAML node type for methods, must be sequence, ", data.m_path.string(), node["methods"]);
        }
    }

    parseSequenceDefinitions(node, data, "ownedParameters", static_cast<BehavioralFeature&>(op), &BehavioralFeature::getOwnedParameters, determineAndParseParameter);
}

void emitOperation(YAML::Emitter& emitter, Operation& op, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::OPERATION, "operation", op, data);
    emitNamedElement(emitter, op, data);
    emitTemplateableElement(emitter, op, data);
    if (!op.getMethods().empty()) {
        emitter << YAML::Key << "methods" << YAML::Value << YAML::BeginSeq;
        for (const ID id : op.getMethods().ids()) {
            emitter << YAML::Value << id.string();
        }
        emitter << YAML::EndSeq;
    }
    emitSequence(emitter, "ownedParameters", data, static_cast<BehavioralFeature&>(op), &BehavioralFeature::getOwnedParameters);
    emitElementDefenitionEnd(emitter, ElementType::OPERATION, op);
}

PackageMerge& determineAndParsePackageMerge(YAML::Node node, ParserMetaData& data) {
    if (node["packageMerge"]) {
        if (node["packageMerge"].IsMap()) {
            PackageMerge& merge = data.m_manager->create<PackageMerge>();
            parsePackageMerge(node["packageMerge"], merge, data);
            return merge;
        } else {
            throw UmlParserException("Invalid yaml node type for packageMerge definition, can only be a map!", data.m_path.string(), node["packageMerge"]);
        }
    } else {
        throw UmlParserException("Invalid uml element for packageMerge definition, it can only be a packageMerge!", data.m_path.string(), node);
    }
}

ProfileApplication& determineAndParseProfileApplication(YAML::Node node, ParserMetaData& data) {
    if (node["profileApplication"]) {
        if (node["profileApplication"].IsMap()) {
            ProfileApplication& profileApplication = data.m_manager->create<ProfileApplication>();
            parseProfileApplication(node["profileApplication"], profileApplication, data);
            return profileApplication;
        }
        else {
            throw UmlParserException("Invalid yaml node type for profileApplication definition, must be a map!", data.m_path.string(), node["profileApplication"]);
        }
    } else {
        throw UmlParserException("Invalid uml element for profileApplication definition, can only be a profileApplication!", data.m_path.string(), node);
    }
}

PackageableElement& determineAndParsePackageableElement(YAML::Node node, ParserMetaData& data) {
    if (node["abstraction"]) {
        if (node["abstraction"].IsMap()) {
            Abstraction& abstraction = data.m_manager->create<Abstraction>();
            /** TODO: make own function and actually do opaqueExpressions**/
            parseDependency(node["abstraction"], abstraction, data);
            return abstraction;
        } else {
            throw UmlParserException("Invalid yaml node type for abstraction definition, must be a map!", data.m_path.string(), node["abstraction"]);
        }
    } else if (node["activity"]) {
        return data.m_manager->create<Activity>(); // TODO;
    } else if (node["artifact"]) {
        return parseDefinition(node, data, "artifact", parseArtifact);
    } else if (node["association"]) {
        return parseDefinition(node, data, "association", parseAssociation);
    } else if (node["class"]) {
        return parseDefinition(node, data, "class", parseClass);
    }  else if (node["dataType"]) {
        return parseDefinition(node, data, "dataType", parseDataType);
    } else if (node["deployment"]) {
        return parseDefinition(node, data, "deployment", parseDeployment);
    } else if (node["dependency"]) {
        return parseDefinition(node, data, "dependency", parseDependency);
    } else if (node["enumeration"]) {
        return parseDefinition(node, data, "enumeration", parseEnumeration);
    } else if (node["enumerationLiteral"]) {
        return parseDefinition(node, data, "enumerationLiteral", parseEnumerationLiteral);
    } else if (node["expression"]) {
        return parseDefinition(node, data, "expression", parseExpression);
    } else if (node["extension"]) {
        return parseDefinition(node, data, "extension", parseExtension);
    } else if (node["instanceSpecification"]) {
        return parseDefinition(node, data, "instanceSpecification", parseInstanceSpecification);
    } else if (node["instanceValue"]) {
        return parseDefinition(node, data, "instanceValue", parseInstanceValue);
    } else if (node["literalBool"]) {
        return parseDefinition(node, data, "literalBool", parseLiteralBool);
    } else if (node["literalInt"]) {
        return parseDefinition(node, data, "literalInt", parseLiteralInt);
    } else if (node["literalNull"]) {
        // special handling
        LiteralNull& ln = data.m_manager->create<LiteralNull>();
        parseTypedElement(node["literalNull"], ln, data);
        return ln;
    } else if (node["literalReal"]) {
        return parseDefinition(node, data, "literalReal", parseLiteralReal);
    } else if (node["literalString"]) {
        return parseDefinition(node, data, "literalString", parseLiteralString);
    } else if (node["literalUnlimitedNatural"]) {
        return parseDefinition(node, data, "literalUnlimitedNatural", parseLiteralUnlimitedNatural);
    } else if (node["manifestation"]) {
        return parseDefinition(node, data, "manifestation", parseManifestation);
    } else if (node["opaqueBehavior"]) {
        return parseDefinition(node, data, "opaqueBehavior", parseOpaqueBehavior);
    } else if (node["package"]) {
        return parseDefinition(node, data, "package", parsePackage);
    } else if (node["primitiveType"]) {
        return parseDefinition(node, data, "primitiveType", parsePrimitiveType);
    } else if (node["profile"]) {
        if (node["profile"].IsMap()) {
            Profile& profile = data.m_manager->create<Profile>();
            parsePackage(node["profile"], profile, data);
            return profile;
        } else {
            throw UmlParserException("Invalid yaml node type for profile definition, must be a map!", data.m_path.string(), node["profile"]);
        }
    } else if (node["realization"]) {
        if (node["realization"].IsMap()) {
            Realization& realization = data.m_manager->create<Realization>();
            /** TODO: switch to parseAbstraction when implemented**/
            parseDependency(node["realization"], realization, data);
            return realization;
        } else {
            throw UmlParserException("Invalid yaml node type for realization definition, must be a map!", data.m_path.string(), node["realization"]);
        }
    } else if (node["usage"]) {
        if (node["usage"].IsMap()) {
            Usage& usage = data.m_manager->create<Usage>();
            parseDependency(node["usage"], usage, data);
            return usage;
        } else {
            throw UmlParserException("Invalid yaml node type for usage definition, must be a map!", data.m_path.string(), node["usage"]);
        }
    } else {
        throw UmlParserException("Invalid identifier for packagedElements, ", data.m_path.string(), node);
    }
}

Stereotype& determineAndParseStereotype(YAML::Node node, ParserMetaData& data) {
    if (node["stereotype"]) {
        if (node["stereotype"].IsMap()) {
            Stereotype& stereotype = data.m_manager->create<Stereotype>();
            parseClass(node["stereotype"], stereotype, data);
            return stereotype;
        } else {
            throw UmlParserException("Invalid yaml node type for stereotype definition, it must be a map!", data.m_path.string(), node["stereotype"]);
        }
    } else {
        throw UmlParserException("Invalid element identifier for stereotype, it may only be stereotype!", data.m_path.string(), node);
    }
}

void parsePackage(YAML::Node node, Package& pckg, ParserMetaData& data) {
    parseNamedElement(node, pckg, data);
    parseTemplateableElement(node, pckg, data); 
    parseSequenceDefinitions(node, data, "packageMerge", pckg, &Package::getPackageMerge, determineAndParsePackageMerge);
    parseSequenceDefinitions(node, data, "profileApplications", pckg, &Package::getProfileApplications, determineAndParseProfileApplication);
    parseSequenceDefinitions(node, data, "packagedElements", pckg, &Package::getPackagedElements, determineAndParsePackageableElement);
    parseSequenceDefinitions(node, data, "ownedStereotypes", pckg, &Package::getOwnedStereotypes, determineAndParseStereotype);
}

void emitPackage(YAML::Emitter& emitter, Package& pckg, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::PACKAGE, "package", pckg, data);
    emitNamedElement(emitter, pckg, data);
    emitTemplateableElement(emitter, pckg, data);
    emitSequence(emitter, "packageMerge", data, pckg, &Package::getPackageMerge);
    emitSequence(emitter, "profileApplications", data, pckg, &Package::getProfileApplications);

    // special handling
    if (!pckg.getPackagedElements().empty()) {
        emitter << YAML::Key << "packagedElements" << YAML::Value << YAML::BeginSeq;
        for (auto& el : pckg.getPackagedElements()) {
            if (!el.isSubClassOf(ElementType::STEREOTYPE)) {
                emit(emitter, el, data);
            }
        }
        emitter << YAML::EndSeq;
    }

    if (!pckg.getOwnedStereotypes().empty()) {
        emitter << YAML::Key << "ownedStereotypes" << YAML::Value << YAML::BeginSeq;
        for (auto& stereotype : pckg.getOwnedStereotypes()) {
            emit(emitter, stereotype, data);
        }
        emitter << YAML::EndSeq;
    }

    emitElementDefenitionEnd(emitter, ElementType::PACKAGE, pckg);
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
            if (node["upper"].as<string>().compare("*") == 0) {
                LiteralUnlimitedNatural& upper = data.m_manager->create<LiteralUnlimitedNatural>();
                upper.setInfinite();
                el.setUpperValue(&upper);
            } else {
                LiteralInt& upper = data.m_manager->create<LiteralInt>();
                upper.setValue(node["upper"].as<int>());
                el.setUpperValue(&upper);
            }
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

Slot& determineAndParseSlot(YAML::Node node, ParserMetaData& data) {
    if (node["slot"]) {
        return parseDefinition(node, data, "slot", parseSlot);
    } else {
        throw UmlParserException("Invalid element identifier for slot definition, it may only be a slot!", data.m_path.string(), node);
    }
}

void parseInstanceSpecification(YAML::Node node, InstanceSpecification& inst, ParserMetaData& data) {
    parseNamedElement(node, inst, data);
    parseDeploymentTarget(node, inst, data);

    if (node["classifier"]) {
        if (node["classifier"].IsScalar()) {
            string classifierID = node["classifier"].as<string>();
            if (isValidID(classifierID)) {
                if (data.m_strategy == ParserStrategy::WHOLE) {
                    ID id = ID::fromString(classifierID);
                    applyFunctor(data, id, new SetClassifierFunctor(&inst, node["classifier"]));
                } else {
                    InstanceSpecificationSetClassifier setClassifier;
                    setClassifier(node["classifier"], data, inst);
                }
            }
        } else {
            throw UmlParserException("Invalid YAML node type for InstanceSpecification field classifier,", data.m_path.string(), node["classifier"]);
        }
    }

    parseSequenceDefinitions(node, data, "slots", inst, &InstanceSpecification::getSlots, determineAndParseSlot);

    if (node["specification"]) {
        if (node["specification"].IsMap()) {
            inst.setSpecification(&determineAndParseValueSpecification(node["specification"], data));
        } else if (node["specification"].IsScalar()) {
            SetSpecification setSpecification;
            setSpecification(node["specification"], data, inst);
        } else {
            throw UmlParserException("Invalid yaml node type for specification field, must be a map or scalar!", data.m_path.string(), node["specification"]);
        }
    }
}

void emitInstanceSpecification(YAML::Emitter& emitter, InstanceSpecification& inst, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::INSTANCE_SPECIFICATION, "instanceSpecification", inst, data);
    emitNamedElement(emitter, inst, data);
    emitDeploymentTarget(emitter, inst, data);
    if (inst.getClassifier()) {
        emitter << YAML::Key << "classifier" << YAML::Value << inst.getClassifier()->getID().string();
    }
    emitSequence(emitter, "slots", data, inst, &InstanceSpecification::getSlots);
    if (inst.getSpecification()) {
        emitter << YAML::Key << "specification" << YAML::Value;
        emit(emitter, *inst.getSpecification(), data);
    }
    emitElementDefenitionEnd(emitter, ElementType::INSTANCE_SPECIFICATION, inst);
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
            if (data.m_strategy == ParserStrategy::WHOLE) {
                string stringID = node["definingFeature"].as<string>();
                if (isValidID(stringID)) {
                    ID definingFeatureID = ID::fromString(stringID);
                    applyFunctor(data, definingFeatureID, new SetDefiningFeatureFunctor(&slot, node["definingFeature"]));
                }
            } else {
                SetDefiningFeature setDefiningFeature;
                setDefiningFeature(node["definingFeature"], data, slot);
            }
        } else {
            throw UmlParserException("Invalid YAML node type for Slot field definingFeature, expected scalar, ", data.m_path.string(), node["definingFeature"]);
        }
    }
    parseSequenceDefinitions(node, data, "values", slot, &Slot::getValues, &determineAndParseValueSpecification);
}

void emitSlot(YAML::Emitter& emitter, Slot& slot, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::SLOT, "slot", slot, data);
    emitElement(emitter, slot, data);
    if (slot.getDefiningFeature()) {
        emitter << YAML::Key << "definingFeature" << YAML::Value << slot.getDefiningFeature()->getID().string();
    }
    emitSequence(emitter, "values", data, slot, &Slot::getValues);
    emitElementDefenitionEnd(emitter, ElementType::SLOT, slot);
}

EnumerationLiteral& determineAndParseEnumerationLiteral(YAML::Node node, ParserMetaData& data) {
    if (node["enumerationLiteral"]) {
        return parseDefinition(node, data, "enumerationLiteral", parseEnumerationLiteral);
    } else {
        throw UmlParserException("Invalid element identifier for enumerationLiteral definition, it may only be an enumerationLiteral!", data.m_path.string(), node);
    }
}

void parseEnumeration(YAML::Node node, Enumeration& enumeration, ParserMetaData& data) {
    parseDataType(node, enumeration, data);
    parseSequenceDefinitions(node, data, "ownedLiteral", enumeration, &Enumeration::getOwnedLiterals, determineAndParseEnumerationLiteral);
}

void emitEnumeration(YAML::Emitter& emitter, Enumeration& enumeration, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::ENUMERATION, "enumeration", enumeration, data);
    emitDataType(emitter, enumeration, data);
    emitSequence(emitter, "ownedLiteral", data, enumeration, &Enumeration::getOwnedLiterals);
    emitElementDefenitionEnd(emitter, ElementType::ENUMERATION, enumeration);
}

void parseEnumerationLiteral(YAML::Node node, EnumerationLiteral& literal, ParserMetaData& data) {
    parseInstanceSpecification(node, literal, data);
}

void emitEnumerationLiteral(YAML::Emitter& emitter, EnumerationLiteral& literal, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::ENUMERATION_LITERAL, "enumerationLiteral", literal, data);
    emitInstanceSpecification(emitter, literal, data);
    emitElementDefenitionEnd(emitter, ElementType::ENUMERATION_LITERAL, literal);
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
            if (data.m_strategy == ParserStrategy::WHOLE) {
                string instID = node["instance"].as<string>();
                if (isValidID(instID)) {
                    ID id = ID::fromString(instID);
                    applyFunctor(data, id, new SetInstanceFunctor(&val, node["instance"]));
                }
                else {
                    throw UmlParserException("Scalar YAML node for InstanceValue field instance is not a valid id, must be base64 url safe 28 character string, ", data.m_path.string(), node["instance"]);
                }
            } else {
                SetInstance setInstance;
                setInstance(node["instance"], data, val);
            }
        } else {
            throw UmlParserException("Invalid YAML node type for InstanceValue field instance, expect scalar, ", data.m_path.string(), node["instance"]);
        }
    }
}

void emitInstanceValue(YAML::Emitter& emitter, InstanceValue& val, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::INSTANCE_VALUE, "instanceValue", val, data);
    emitTypedElement(emitter, val, data);
    if (val.getInstance()) {
        emitter << YAML::Key << "instance" << YAML::Value << val.getInstance()->getID().string();
    }
    emitElementDefenitionEnd(emitter, ElementType::INSTANCE_VALUE, val);
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
    emitElementDefenition(emitter, ElementType::PACKAGE_MERGE, "packageMerge", merge, data);

    emitElement(emitter, merge, data);

    if (merge.getMergedPackage() != 0) {
        filesystem::path path = data.getPath(merge.getMergedPackage()->getID());
        if (path.empty() || path == data.m_path / data.m_fileName) {
            emitter << YAML::Key << "mergedPackage" << YAML::Value << merge.getMergedPackage()->getID().string();
        } else {
            emitToFile(*merge.getMergedPackage(), data, path.parent_path().string(), path.filename().string());
            if (data.m_path == path.parent_path()) {
                emitter << YAML::Key << "mergedPackage" << YAML::Value << path.filename().string();
            }
        }
    }

    emitElementDefenitionEnd(emitter, ElementType::PACKAGE_MERGE, merge);
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
    emitElementDefenition(emitter, ElementType::LITERAL_BOOL, "literalBool", lb, data);

    emitTypedElement(emitter, lb, data);

    emitter << YAML::Key << "value" << YAML::Value << lb.getValue();

    emitElementDefenitionEnd(emitter, ElementType::LITERAL_BOOL, lb);
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
    emitElementDefenition(emitter, ElementType::LITERAL_INT, "literalInt", li, data);

    emitTypedElement(emitter, li, data);

    emitter << YAML::Key << "value" << YAML::Value << li.getValue();

    emitElementDefenitionEnd(emitter, ElementType::LITERAL_INT, li);
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
    emitElementDefenition(emitter, ElementType::LITERAL_REAL, "literalReal", lr, data);

    emitTypedElement(emitter, lr, data);

    emitter << YAML::Key << "value" << YAML::Value << lr.getValue();

    emitElementDefenitionEnd(emitter, ElementType::LITERAL_REAL, lr);
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
    emitElementDefenition(emitter, ElementType::LITERAL_STRING, "literalString", ls, data);

    emitTypedElement(emitter, ls, data);

    if (!ls.getValue().empty()) {
        emitter << YAML::Key << "value" << YAML::Value << ls.getValue();
    }

    emitElementDefenitionEnd(emitter, ElementType::LITERAL_STRING, ls);
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
    emitElementDefenition(emitter, ElementType::LITERAL_UNLIMITED_NATURAL, "literalUnlimitedNatural", ln, data);

    emitTypedElement(emitter, ln, data);

    if (ln.isInfinite()) {
        emitter << YAML::Key << "value" << YAML::Value << "*";
    } else {
        emitter << YAML::Key << "value" << YAML::Value << ln.getNumberValue();
    }

    emitElementDefenitionEnd(emitter, ElementType::LITERAL_UNLIMITED_NATURAL, ln);
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
    parseSequenceDefinitions(node, data, "operands", exp, &Expression::getOperands, &determineAndParseValueSpecification);
}

void emitExpression(YAML::Emitter& emitter, Expression& exp, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::EXPRESSION, "expression", exp, data);

    emitTypedElement(emitter, exp, data);
    if (!exp.getSymbol().empty()) {
        emitter << YAML::Key << "symbol" << YAML::Value << exp.getSymbol();
    }
    emitSequence(emitter, "operands", data, exp, &Expression::getOperands);

    emitElementDefenitionEnd(emitter, ElementType::EXPRESSION, exp);
}

void parseTemplateableElement(YAML::Node node, TemplateableElement& el, ParserMetaData& data) {
    if (node["templateSignature"]) {
        if (node["templateSignature"].IsMap()) {
            if (node["templateSignature"]["templateSignature"]) {
                if (node["templateSignature"]["templateSignature"].IsMap()) {
                    TemplateSignature& signature = data.m_manager->create<TemplateSignature>();
                    parseTemplateSignature(node["templateSignature"]["templateSignature"], signature, data);
                    el.setOwnedTemplateSignature(&signature);
                } else {
                    /** TODO work for mount**/
                }
            } else {
                throw UmlParserException("Must specify templateSignature field before templateSignature definition", data.m_path.string(), node["templateSignature"]);
            }
        } else {
            el.setOwnedTemplateSignature(parseScalar<TemplateSignature>(node["templateSignature"], data));
            //throw UmlParserException("Invalid node type fore templateSignature, must be map ", data.m_path.string(), node["templateSignature"]);
        }
    }

    if (node["templateBinding"]) {
        if (node["templateBinding"].IsMap()) {
            if (node["templateBinding"]["templateBinding"]) {
                if (node["templateBinding"]["templateBinding"].IsMap()) {
                    TemplateBinding& binding = data.m_manager->create<TemplateBinding>();
                    parseTemplateBinding(node["templateBinding"]["templateBinding"], binding, data);
                    el.setTemplateBinding(&binding);
                } else if (node["templateBinding"]["templateBinding"].IsScalar()) {
                    /** TODO: make mount ready **/
                } else {
                    // TODO error
                }
            } else {
                throw UmlParserException("Must specify templateBinding field before templateBinding definition", data.m_path.string(), node["templateBinding"]);
            }
        } else {
            el.setTemplateBinding(parseScalar<TemplateBinding>(node["templateBinding"], data));
            //throw UmlParserException("Invalid YAML node for templateBinding definition, ", data.m_path.string(), node["templateBinding"]);
        }
    }
}

void emitTemplateableElement(YAML::Emitter& emitter, TemplateableElement& el, EmitterMetaData& data) {
    if (el.hasOwnedTemplateSignature()) {
        emitter << YAML::Key << "templateSignature" << YAML::Value;
        emit(emitter, el.getOwnedTemplateSignatureRef(), data);
    }

    if (el.hasTemplateBinding()) {
        emitter << YAML::Key << "templateBinding" << YAML::Value;
        emit(emitter, el.getTemplateBindingRef(), data);
    }
}

void AddTemplateParmeterFunctor::operator()(Element& el) const {
    if (el.isSubClassOf(ElementType::TEMPLATE_PARAMETER)) {
        dynamic_cast<TemplateSignature*>(m_el)->getParameter().add(dynamic_cast<TemplateParameter&>(el));
    } else {
        throw UmlParserException("Tried to add parameter to signature that wasn't a parameter! ", "", m_node);
    }
}

TemplateParameter& determineAndParseTemplateParameter(YAML::Node node, ParserMetaData& data) {
    if (node["templateParameter"]) {
        return parseDefinition(node, data, "templateParameter", parseTemplateParameter);
    } else {
        throw UmlParserException("Invalid element identifier for templateParameter definition, it can only be a templateParameter!", data.m_path.string(), node);
    }
}

void parseTemplateSignature(YAML::Node node, TemplateSignature& signature, ParserMetaData& data) {
    parseElement(node, signature, data);
    parseSequenceDefinitions(node, data, "ownedParameters", signature, &TemplateSignature::getOwnedParameter, determineAndParseTemplateParameter);
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

    emitElementDefenition(emitter, ElementType::TEMPLATE_SIGNATURE, "templateSignature", signature, data);

    emitElement(emitter, signature, data);
    emitSequence(emitter, "ownedParameters", data, signature, &TemplateSignature::getOwnedParameter);
    // special handling
    if (signature.getParameter().size() > signature.getOwnedParameter().size()) {
        emitter << YAML::Key << "parameters" << YAML::Value << YAML::BeginSeq;
        for (auto& param: signature.getParameter()) {
            if (!signature.getOwnedParameter().count(param.getID())) {
                emitter << param.getID().string();
            }
        }
        emitter << YAML::EndSeq;
    }

    emitElementDefenitionEnd(emitter, ElementType::TEMPLATE_SIGNATURE, signature);
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
    emitElementDefenition(emitter, ElementType::TEMPLATE_PARAMETER, "templateParameter", parameter, data);

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

    emitElementDefenitionEnd(emitter, ElementType::TEMPLATE_PARAMETER, parameter);
}

void SetSignatureFunctor::operator()(Element& el) const {
    if (el.isSubClassOf(ElementType::TEMPLATE_SIGNATURE)) {
        dynamic_cast<TemplateBinding*>(m_el)->setSignature(dynamic_cast<TemplateSignature*>(&el));
    } else {
        throw UmlParserException("Tried to set binding signature to not signature, line ", "", m_node);
    }
}

TemplateParameterSubstitution& determineAndParseTemplateParameterSubstitution(YAML::Node node, ParserMetaData& data) {
    if (node["templateParameterSubstitution"]) {
        return parseDefinition(node, data, "templateParameterSubstitution", parseTemplateParameterSubstitution);
    } else {
        throw UmlParserException("Invalid element identifier for templateParameterSubstitution definition, it can only be a templateParameterSubstitution!", data.m_path.string(), node);
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
    parseSequenceDefinitions(node, data, "parameterSubstitution", binding, &TemplateBinding::getParameterSubstitution, determineAndParseTemplateParameterSubstitution);
}

void emitTemplateBinding(YAML::Emitter& emitter, TemplateBinding& binding, EmitterMetaData& data) {
    // TODO change
    if (binding.getElementType() == ElementType::TEMPLATE_BINDING) {
        emitter << /*YAML::BeginMap << */YAML::Key << "templateBinding" << YAML::Value << YAML::BeginMap;
    }

    emitElement(emitter, binding, data);
    if (binding.getSignature() != 0) {
        emitter << YAML::Key << "signature" << YAML::Value << binding.getSignature()->getID().string();
    }
    emitSequence(emitter, "parameterSubstitution", data, binding, &TemplateBinding::getParameterSubstitution);

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
    emitElementDefenition(emitter, ElementType::TEMPLATE_PARAMETER_SUBSTITUTION, "templateParameterSubstitution", sub, data);

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

    emitElementDefenitionEnd(emitter, ElementType::TEMPLATE_PARAMETER_SUBSTITUTION, sub);
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
    parseSequenceDefinitions(node, data, "navigableOwnedEnds", association, &Association::getNavigableOwnedEnds, determineAndParseOwnedAttribute);
    parseSequenceDefinitions(node, data, "ownedEnds", association, &Association::getOwnedEnds, determineAndParseOwnedAttribute);
    
    if (node["memberEnds"]) {
        if (node["memberEnds"].IsSequence()) {
            for (size_t i = 0; i < node["memberEnds"].size(); i++) {
                if (node["memberEnds"][i].IsScalar()) {
                    if (data.m_strategy == ParserStrategy::WHOLE) {
                        if (isValidID(node["memberEnds"][i].as<string>())) {
                            applyFunctor(data, ID::fromString(node["memberEnds"][i].as<string>()), new AddMemberEndFunctor(&association, node["memberEnds"][i]));
                        } else {
                            throw UmlParserException("Invalid ID for member end entry, must be a 28 character url safe 64bit encoded string!", data.m_path.string(), node["memberEnds"][i]);
                        }
                    } else {
                        if (data.m_manager->loaded(ID::fromString(node["memberEnds"][i].as<string>()))) {
                            association.getMemberEnds().add(data.m_manager->get<Property>(ID::fromString(node["memberEnds"][i].as<string>())));
                        } else {
                            association.getMemberEnds().addByID(ID::fromString(node["memberEnds"][i].as<string>()));
                        }
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
    emitElementDefenition(emitter, ElementType::ASSOCIATION, "association", association, data);

    emitClassifier(emitter, association, data);
    emitSequence(emitter, "navigableOwnedEnds", data, association, &Association::getNavigableOwnedEnds);
    // special handling
    if (!association.getOwnedEnds().size() > association.getNavigableOwnedEnds().size() && !association.getOwnedEnds().empty()) {
        emitter << YAML::Key << "ownedEnds" << YAML::Value << YAML::BeginSeq;
        if (data.m_strategy == EmitterStrategy::WHOLE) {
            for (auto& end : association.getOwnedEnds()) {
                if (!association.getNavigableOwnedEnds().count(end.getID())) {
                    emit(emitter, end, data);
                }
            }
        } else {
            for (const ID id : association.getOwnedEnds().ids()) {
                if (!association.getNavigableOwnedEnds().count(id)) {
                    emitter << YAML::Value << id.string() + ".yml";
                }
            }
        }
        emitter << YAML::EndSeq;
    }

    if (association.getMemberEnds().size() > association.getOwnedEnds().size() && !association.getMemberEnds().empty()) {
        emitter << YAML::Key << "memberEnds" << YAML::Value << YAML::BeginSeq;
        for (const ID id : association.getMemberEnds().ids()) {
            if (!association.getOwnedEnds().count(id)) {
                emitter << YAML::Value << id.string();
            }
        }
        emitter << YAML::EndSeq;
    }

    emitElementDefenitionEnd(emitter, ElementType::ASSOCIATION, association);
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
    if (eType.compare("ABSTRACTION") == 0) {
        return ElementType::ABSTRACTION;
    } else if (eType.compare("ACTION") == 0) {
        return ElementType::ACTION;
    } else if (eType.compare("ACTIVITY") == 0) {
        return ElementType::ACTIVITY;
    } else if (eType.compare("ACTIVITY_EDGE") == 0) {
        return ElementType::ACTIVITY_EDGE;
    } else if (eType.compare("ACTIVITY_NODE") == 0) {
        return ElementType::ACTIVITY_NODE;
    } else if (eType.compare("ARTIFACT") == 0) {
        return ElementType::ARTIFACT;
    } else if (eType.compare("ASSOCIATION") == 0) {
        return ElementType::ASSOCIATION;
    } else if (eType.compare("BEHAVIOR") == 0) {
        return ElementType::BEHAVIOR;
    } else if (eType.compare("BEHAVIORAL_FEATURE") == 0) {
        return ElementType::BEHAVIORAL_FEATURE;
    } else if (eType.compare("BEHAVIORED_CLASSIFIER") == 0 ) {
        return ElementType::BEHAVIORED_CLASSIFIER;
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
    } else if (eType.compare("DEPENDENCY") == 0) {
        return ElementType::DEPENDENCY;
    } else if (eType.compare("DEPLOYED_ARTIFACT") == 0) {
        return ElementType::DEPLOYED_ARTIFACT;
    } else if (eType.compare("DEPLOYMENT") == 0) {
        return ElementType::DEPLOYMENT;
    } else if (eType.compare("DEPLOYMENT_TARGET") == 0) {
        return ElementType::DEPLOYMENT_TARGET;
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
    } else if (eType.compare("REALIZATION") == 0) {
        return ElementType::REALIZATION;
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
    } else if (eType.compare("USAGE") == 0) {
        return ElementType::USAGE;
    } else if (eType.compare("VALUE_SPECIFICATION") == 0) {
        return ElementType::VALUE_SPECIFICATION;
    } 
    throw UmlParserException("Could not identify entity type by keyword: " + eType + '!', "");
}

void emitExtension(YAML::Emitter& emitter, Extension& extension, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::EXTENSION, "extension", extension, data);

    emitClassifier(emitter, extension, data);

    emitter << YAML::Key << "metaClass" << YAML::Value << Element::elementTypeToString(extension.getMetaClass());

    if (extension.getOwnedEnd() != 0) {
        emitter << YAML::Key << "ownedEnd" << YAML::Value;
        emit(emitter, *extension.getOwnedEnd(), data);
    }

    emitElementDefenitionEnd(emitter, ElementType::EXTENSION, extension);
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
    emitElementDefenition(emitter, ElementType::PROFILE_APPLICATION, "profileApplication", application, data);

    emitElement(emitter, application, data);

    if (application.getAppliedProfile() != 0) {
        filesystem::path path = data.getPath(application.getAppliedProfile()->getID());
        if (path.empty() || path == data.m_path / data.m_fileName) {
            emitter << YAML::Key << "appliedProfile" << YAML::Value << application.getAppliedProfile()->getID().string();
        } else {
            //emit(emitter, *application.getAppliedProfile(), data);
            if (data.m_path == path.parent_path()) {
                emitter << YAML::Key << "appliedProfile" << YAML::Value << path.filename().string();
            }
        }
    }

    emitElementDefenitionEnd(emitter, ElementType::PROFILE_APPLICATION, application);
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
    emitElementDefenition(emitter, ElementType::COMMENT, "comment", comment, data);

    emitElement(emitter, comment, data);

    if (!comment.getBody().empty()) {
        emitter << YAML::Key << "body" << YAML::Value << comment.getBody();
    }

    emitElementDefenitionEnd(emitter, ElementType::COMMENT, comment);
}

void AddClientFunctor::operator()(Element& el) const {
    if (!m_el->as<Dependency>().getClient().count(el.getID())) {
        m_el->as<Dependency>().getClient().add(el.as<NamedElement>());
    }
}

void AddSupplierFunctor::operator()(Element& el) const {
    if (!m_el->as<Dependency>().getSupplier().count(el.getID())) {
        m_el->as<Dependency>().getSupplier().add(el.as<NamedElement>());
    }
}

void parseDependency(YAML::Node node, Dependency& dependency, ParserMetaData& data) {
    parseNamedElement(node, dependency, data);

    if (node["client"]) {
        if (node["client"].IsSequence()) {
            for (size_t i = 0; i < node["client"].size(); i++) {
                if (node["client"][i].IsScalar()) {
                    if (isValidID(node["client"][i].as<string>())) {
                        ID clientID = ID::fromString(node["client"][i].as<string>());
                        if (data.m_strategy == ParserStrategy::WHOLE) {
                            applyFunctor(data, clientID, new AddClientFunctor(&dependency, node["client"][i]));
                        } else {
                            if (data.m_manager->loaded(clientID)) {
                                dependency.getClient().add(data.m_manager->get<NamedElement>(clientID));
                            } else {
                                dependency.getClient().addByID(clientID);
                            }
                        }
                    } else {
                        throw UmlParserException("Invalid ID, must be a base64 url safe 28 character string!", data.m_path.string(), node["client"][i]);
                    }
                } else {
                    throw UmlParserException("Invalid yaml node type for dependency client entry, must be a scalar!", data.m_path.string(), node["client"][i]);
                }
            }
        } else {
            throw UmlParserException("Invalid yaml node type for dependency client, must be a sequence!", data.m_path.string(), node["client"]);
        }
    }

    if (node["supplier"]) {
        if (node["supplier"].IsSequence()) {
            for (size_t i = 0; i < node["supplier"].size(); i++) {
                if (node["supplier"][i].IsScalar()) {
                    if (isValidID(node["supplier"][i].as<string>())) {
                        ID supplierID = ID::fromString(node["supplier"][i].as<string>());
                        if (data.m_strategy == ParserStrategy::WHOLE) {
                            applyFunctor(data, supplierID, new AddSupplierFunctor(&dependency, node["supplier"][i]));
                        } else {
                            if (data.m_manager->loaded(supplierID)) {
                                dependency.getSupplier().add(data.m_manager->get<NamedElement>(supplierID));
                            } else {
                                dependency.getSupplier().addByID(supplierID);
                            }
                        }
                    } else {
                        throw UmlParserException("Invalid ID, must be a base64 url safe 28 character string!", data.m_path.string(), node["supplier"][i]);
                    }
                } else {
                    throw UmlParserException("Invalid yaml node type for dependency client entry, must be a scalar!", data.m_path.string(), node["supplier"][i]);
                }
            }
        } else {
            throw UmlParserException("Invalid yaml node type for dependency supplier, must be a sequence!", data.m_path.string(), node["supplier"]);
        }
    }
}

void emitDependency(YAML::Emitter& emitter, Dependency& dependency, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::DEPENDENCY, "dependency", dependency, data);

    emitNamedElement(emitter, dependency, data);

    if (!dependency.getClient().empty()) {
        emitter << YAML::Key << "client" << YAML::Value << YAML::BeginSeq;
        for (const ID id : dependency.getClient().ids()) {
            emitter << id.string();
        }
        emitter << YAML::EndSeq;
    }

    if (!dependency.getSupplier().empty()) {
        emitter << YAML::Key << "supplier" << YAML::Value << YAML::BeginSeq;
        for (const ID id : dependency.getSupplier().ids()) {
            emitter << id.string();
        }
        emitter << YAML::EndSeq;
    }

    emitElementDefenitionEnd(emitter, ElementType::DEPENDENCY, dependency);
}

void AddDeployedArtifactFunctor::operator()(Element& el) const {
    m_el->as<Deployment>().getDeployedArtifact().add(el.as<DeployedArtifact>());
}

void parseDeployment(YAML::Node node, Deployment& deployment, ParserMetaData& data) {
    
    parseNamedElement(node, deployment, data);

    if (node["deployedArtifacts"]) {
        if (node["deployedArtifacts"].IsSequence()) {
            for (size_t i = 0; i < node["deployedArtifacts"].size(); i++) {
                if (node["deployedArtifacts"][i].IsScalar()) {
                    applyFunctor(data, ID::fromString(node["deployedArtifacts"][i].as<string>()), new AddDeployedArtifactFunctor(&deployment, node["deployedArtifacts"][i]));
                } else {
                    throw UmlParserException("Invalid yaml node type for deployment deployedArtifacts reference, must be a scalar!", data.m_path.string(), node["deployedArtifacts"][i]);
                }
            }
        } else {
            throw UmlParserException("Invalid yaml node type for deployment deployedArtifacts field, must be a sequence!", data.m_path.string(), node["deployedArtifacts"]);
        }
    }
}

void emitDeployment(YAML::Emitter& emitter, Deployment& deployment, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::DEPLOYMENT, "deployment", deployment, data);

    emitNamedElement(emitter, deployment, data);

    if (!deployment.getDeployedArtifact().empty()) {
        emitter << YAML::Key << "deployedArtifacts" << YAML::Value << YAML::BeginSeq;
        for (const ID id : deployment.getDeployedArtifact().ids()) {
            emitter << id.string();
        }
        emitter << YAML::EndSeq;
    }

    emitElementDefenitionEnd(emitter, ElementType::DEPLOYMENT, deployment);
}

Artifact& determineAndParseArtifact(YAML::Node node, ParserMetaData& data) {
    if (node["artifact"]) {
        return parseDefinition(node, data, "artifact", parseArtifact);
    } else {
        throw UmlParserException("Invalid element identifier for artifact definition, it can only be an artifact!", data.m_path.string(), node);
    }
}

Manifestation& determineAndParseManifestation(YAML::Node node, ParserMetaData& data) {
    if (node["manifestation"]) {
        return parseDefinition(node, data, "manifestation", parseManifestation);
    } else {
        throw UmlParserException("Invalid element identifier for manifestation definition, it can only be a manifestation!", data.m_path.string(), node);
    }
}

void parseArtifact(YAML::Node node, Artifact& artifact, ParserMetaData& data) {
    parseClassifier(node, artifact, data);
    parseSequenceDefinitions(node, data, "ownedAttributes", artifact, &Artifact::getOwnedAttributes, determineAndParseOwnedAttribute);
    parseSequenceDefinitions(node, data, "ownedOperations", artifact, &Artifact::getOwnedOperations, determineAndParseOwnedOperation);
    parseSequenceDefinitions(node, data, "nestedArtifacts", artifact, &Artifact::getNestedArtifacts, determineAndParseArtifact);
    parseSequenceDefinitions(node, data, "manifestations", artifact, &Artifact::getManifestations, determineAndParseManifestation);
}

void emitArtifact(YAML::Emitter& emitter, Artifact& artifact, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::ARTIFACT, "artifact", artifact, data);
    emitClassifier(emitter, artifact, data);
    emitSequence(emitter, "ownedAttributes", data, artifact, &Artifact::getOwnedAttributes);
    emitSequence(emitter, "ownedOperations", data, artifact, &Artifact::getOwnedOperations);
    emitSequence(emitter, "nestedArtifacts", data, artifact, &Artifact::getNestedArtifacts);
    emitSequence(emitter, "manifestations", data, artifact, &Artifact::getManifestations);
    emitElementDefenitionEnd(emitter, ElementType::ARTIFACT, artifact);
}

Deployment& determineAndParseDeployment(YAML::Node node, ParserMetaData& data) {
    if (node["deployment"]) {
        return parseDefinition(node, data, "deployment", parseDeployment);
    } else {
        throw UmlParserException("Invalid element identifier for deployment, it can only be a deployment!", data.m_path.string(), node);
    }
}

void parseDeploymentTarget(YAML::Node node, DeploymentTarget& target, ParserMetaData& data) {
    parseSequenceDefinitions(node, data, "deployments", target, &DeploymentTarget::getDeployments, determineAndParseDeployment);
 }

void emitDeploymentTarget(YAML::Emitter& emitter, DeploymentTarget& target, EmitterMetaData& data) {
    emitSequence(emitter, "deployments", data, target, &DeploymentTarget::getDeployments);
}

Behavior& determineAndParseBehavior(YAML::Node node, ParserMetaData& data) {
    if (node["opaqueBehavior"]) {
        return parseDefinition(node, data, "opaqueBehavior", parseOpaqueBehavior);
    } else {
        throw UmlParserException("Invalid element identifier for opaqueBehavior definition, must be an opaqueBehavior", data.m_path.string(), node); // TODO expand
    }
}

void parseBehavioredClassifier(YAML::Node node, BehavioredClassifier& classifier, ParserMetaData& data) {
    parseSequenceDefinitions(node, data, "ownedBehaviors", classifier, &BehavioredClassifier::getOwnedBehaviors, determineAndParseBehavior);
    
    if (node["classifierBehavior"]) {
        if (node["classifierBehavior"].IsScalar()) {
            classifier.setClassifierBehavior(&classifier.getOwnedBehaviors().get(ID::fromString(node["classifierBehavior"].as<string>())));
        } else {
            throw UmlParserException("Invalid yaml node type for classifierBehavior reference, must be a scalar!", data.m_path.string(), node["classifierBehavior"]);
        }
    }
}

void emitBehavioredClassifier(YAML::Emitter& emitter, BehavioredClassifier& classifier, EmitterMetaData& data) {
    emitSequence(emitter, "ownedBehaviors", data, classifier, &BehavioredClassifier::getOwnedBehaviors);

    if (classifier.getClassifierBehavior() != 0) {
        emitter << YAML::Key << "classifierBehavior" << YAML::Value << classifier.getClassifierBehavior()->getID().string();
    }
}

void SetUtilizedElementFunctor::operator()(Element& el) const {
    m_el->as<Manifestation>().setUtilizedElement(&el.as<PackageableElement>());
}

void parseManifestation(YAML::Node node, Manifestation& manifestation, ParserMetaData& data) {
    parseNamedElement(node, manifestation, data);
    if (node["utilizedElement"]) {
        if (node["utilizedElement"].IsScalar()) {
            applyFunctor(data, ID::fromString(node["utilizedElement"].as<string>()), new SetUtilizedElementFunctor(&manifestation, node["utilizedElement"]));
        } else {
            throw UmlParserException("Invalid yaml node type for manifestation utilized element field, must be a scalar!", data.m_path.string(), node["utilizedElement"]);
        }
    }
}

void emitManifestation(YAML::Emitter& emitter, Manifestation& manifestation, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::MANIFESTATION, "manifestation", manifestation, data);

    emitNamedElement(emitter, manifestation, data);

    if (manifestation.getUtilizedElement() != 0) {
        emitter << YAML::Key << "utilizedElement" << YAML::Value << manifestation.getUtilizedElement()->getID().string();
    }

    emitElementDefenitionEnd(emitter, ElementType::MANIFESTATION, manifestation);
}

}

}
}