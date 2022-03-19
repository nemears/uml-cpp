#include "uml/parsers/parser.h"
#include <fstream>
#include "uml/uml-stable.h"

using namespace std;

namespace UML {
namespace Parsers {

ParserMetaData::ParserMetaData(UmlManager* manager) {
    m_manager = manager;
    if (!manager->m_path.empty()) {
        m_path = m_manager->m_path;
    }
}

/**
 * Template helper functions for parsing
 **/
template <class T = Element, class U = Element>
bool parseSingletonReference(YAML::Node node, ParserMetaData& data, std::string key, U& el, void (U::*elSignature)(T& el), void (U::*idSignature)(ID id)) {
    if (node[key]) {
        if (node[key].IsScalar()) {
            if (isValidID(node[key].as<std::string>())) {
                // ID
                ID id = ID::fromString(node[key].as<std::string>());
                if (data.m_manager->UmlManager::loaded(id) && data.m_strategy != ParserStrategy::INDIVIDUAL) {
                    try {
                        (el.*elSignature)(data.m_manager->get(id).as<T>());
                    } catch (DuplicateElementInSetException& e) {
                        // nothing let (that part) fail
                    }
                    catch (std::exception e) {
                        throw UmlParserException("Unexpected Uml error: " + std::string(e.what()), data.m_path.string(), node[key]);
                    }
                } else {
                    (el.*idSignature)(id);
                }
                return true;
            } else {
                // Path
                ElementPtr parsed = parseExternalAddToManager(data, node[key].as<std::string>());
                if (parsed) {
                    (el.*elSignature)(parsed->as<T>());
                } else {
                    throw UmlParserException("Could not identify valid file at path " + node[key].as<std::string>(), data.m_path.string(), node[key]);
                }
                return true;
                // throw UmlParserException("TODO, parse reference from path (seems a lil irrelevant)", data.m_path.string(), node[key]);
            }
        } else {
            throw UmlParserException("Invalid yaml node type for " + key + " entry, expected a scalar id", data.m_path.string(), node[key]);
        }
    }
    return false;
};

template <class T = Element, class U = Element, class S = Set<T,U>>
void parseSetReferences(YAML::Node node, ParserMetaData& data, std::string key, U& owner, S& (U::*signature)()) {
    if (node[key]) {
        if (node[key].IsSequence()) {
            for (size_t i = 0; i < node[key].size(); i++) {
                if (node[key][i].IsScalar()) {
                    if (isValidID(node[key][i].as<std::string>())) {
                        ID id = ID::fromString(node[key][i].as<std::string>());
                        if (data.m_manager->UmlManager::loaded(id) && data.m_strategy != ParserStrategy::INDIVIDUAL) {
                            try {
                                (owner.*signature)().add(data.m_manager->get(id).as<T>());
                            } catch (DuplicateElementInSetException e) {
                                // nothing
                            } catch (std::exception e) {
                                throw UmlParserException("Unexpected Uml error: " + std::string(e.what()), data.m_path.string(), node[key][i]);
                            }
                        } else {
                            (owner.*signature)().add(id);
                        }
                    }
                } else {
                    throw UmlParserException("Invalid yaml node type for " + key + " entry, expected a scalar id", data.m_path.string(), node);
                }
            }
        } else {
            throw UmlParserException("Invalid yaml node type for " + key + " entry, expected a sequence", data.m_path.string(), node[key]);
        }
    }
};

/**
 * This function is used to parse an element definition contained within a seperate 
 * file, and then add that element to the designated sequence. This should only be 
 * called with sequences that subset  Element::getOwnedElements
 * @param node, the node containing the element defenition file
 * @param data, the data for this parsing session
 * @param el, the element that owns the element being parsed
 * @param signature, the signature of the sequence to add the parsed element to
 **/
template <class T = Element, class U = Element, class S = Set<T,U>>
void parseAndAddToSequence(YAML::Node node, ParserMetaData& data, U& el, S& (U::* signature)()) {
    if (data.m_strategy == ParserStrategy::WHOLE) {
        ElementPtr packagedEl = parseExternalAddToManager(data, node.as<std::string>());
        if (!packagedEl) {
            throw UmlParserException("Could not identify YAML node for packaged elements", data.m_path.string(), node);
        }
        (el.*signature)().add(dynamic_cast<T&>(*packagedEl));
    } else {
        std::string path = node.as<std::string>();
        std::string idStr = path.substr(path.find_last_of("/") + 1, path.find_last_of("/") + 29);
        if (isValidID(idStr)) {
            ID id = ID::fromString(idStr);
            (el.*signature)().add(id);
        } else {
            throw UmlParserException("Invalid id for path, was the data specified as individual, that can only work on a mount!", data.m_path.string(), node);
        }
    }
}

template <class T = Element, class U = Element>
void parseAndSetSingleton(YAML::Node node, ParserMetaData& data, U& el, void (U::*idSignature)(ID id)) {
    if (data.m_strategy == ParserStrategy::INDIVIDUAL) {
        std::string path = node.as<std::string>();
        std::string idStr = path.substr(path.find_last_of("/") + 1, path.find_last_of("/") + 29);
        if (isValidID(idStr)) {
            ID id = ID::fromString(idStr);
            (el.*idSignature)(id);
        } else {
            throw UmlParserException("Invalid id for path", data.m_path.string(), node);
        }
    } else {
        throw UmlParserException("TODO alalla", data.m_path.string(), node);
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
template <class T = Element, class U = Element, class S = Set<T,U>>
void parseSequenceDefinitions(YAML::Node node, ParserMetaData& data, string key, U& owner, S& (U::*sequenceSignature)(), T& (*parserSignature)(YAML::Node, ParserMetaData&)) {
    if (node[key]) {
        if (node[key].IsSequence()) {
            for (size_t i = 0; i < node[key].size(); i++) {
                if (node[key][i].IsMap()) {
                    (owner.*sequenceSignature)().add((*parserSignature)(node[key][i], data));
                }
                else if (node[key][i].IsScalar()) {
                    parseAndAddToSequence<T,U,S>(node[key][i], data, owner, sequenceSignature);
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

template <class T = Element, class V = T> 
T& parseDefinition(YAML::Node node, ParserMetaData& data, string key, void (*parser)(YAML::Node, V&, ParserMetaData&)) {
    if (node[key].IsMap()) {
        T& ret = *data.m_manager->create<T>();
        parser(node[key], ret, data);
        return ret;
    } else {
        throw UmlParserException("Invalid yaml node type for " + key + " definition, it must be a map!", data.m_path.string(), node[key]);
    }
}

template <class T = Element, class U = Element>
void parseSingletonDefinition(YAML::Node node, ParserMetaData& data, std::string key, U& owner, T& (*parser)(YAML::Node, ParserMetaData&), void (U::*elSignature)(T&), void (U::*idSignature)(ID)) {
    if (node[key]) {
        if (node[key].IsMap()) {
            (owner.*elSignature)((*parser)(node[key], data));
        } else {
            parseAndSetSingleton(node[key], data, owner, idSignature);
        }
    }
};

UmlManager* parse(string path) {
    UmlManager* ret = new UmlManager;
    ElementPtr root = ret->parse(path);
    ret->setRoot(*root);
    return ret;
}

ElementPtr parse(ParserMetaData& data) {
    YAML::Node node = YAML::LoadFile(data.m_path.string());
    ElementPtr ret = parseNode(node, data);
    return ret;
}

ElementPtr parseString(string body, ParserMetaData& data) {
    YAML::Node node = YAML::Load(body);
    ElementPtr ret = parseNode(node, data);
    if (ret) {
        return ret;
    } else {
        throw UmlParserException("could not parse string representing an element!", "", node);
    }
}

ElementPtr parseYAML(YAML::Node node, ParserMetaData& data) {
    ElementPtr ret = parseNode(node, data);
    if (ret) {
        return ret;
    } else {
        throw UmlParserException("could not parse string representing an element!", "", node);
    }
}

EmitterMetaData getData(Element& el) {
    EmitterMetaData data;
    data.m_manager = el.m_manager;
    data.m_strategy = EmitterStrategy::INDIVIDUAL;
    return data;
}

string emit(Element& el) {
    YAML::Emitter emitter;
    emit(el, emitter);
    return emitter.c_str();
}

void emit(Element& el, YAML::Emitter& emitter) {
    EmitterMetaData data = getData(el);
    data.m_strategy = EmitterStrategy::WHOLE;
    emit(emitter, el, data);
}

string emitIndividual(Element& el) {
    EmitterMetaData data = getData(el);
    YAML::Emitter emitter;
    determineTypeAndEmit(emitter, el, data);
    return emitter.c_str();
}

void emitIndividual(Element& el, YAML::Emitter& emitter) {
    EmitterMetaData data = getData(el);
    determineTypeAndEmit(emitter, el, data);
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

ElementType elementTypeFromString(string eType) {
    if (eType.compare("ABSTRACTION") == 0) {
        return ElementType::ABSTRACTION;
    } else if (eType.compare("ACTION") == 0) {
        return ElementType::ACTION;
    } else if (eType.compare("ACTIVITY") == 0) {
        return ElementType::ACTIVITY;
    } else if (eType.compare("ACTIVITY_EDGE") == 0) {
        return ElementType::ACTIVITY_EDGE;
    } else if (eType.compare("ACTIVITY_FINAL_NODE") == 0) {
        return ElementType::ACTIVITY_FINAL_NODE;
    } else if (eType.compare("ACTIVITY_GROUP") == 0) {
        return ElementType::ACTIVITY_GROUP;
    } else if (eType.compare("ACTIVITY_NODE") == 0) {
        return ElementType::ACTIVITY_NODE;
    } else if (eType.compare("ACTIVITY_PARAMETER_NODE") == 0) {
        return ElementType::ACTIVITY_PARAMETER_NODE;
    } else if (eType.compare("ACTIVITY_PARTITION") == 0) {
        return ElementType::ACTIVITY_PARTITION;
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
    } else if (eType.compare("CONNECTOR") == 0) {
        return ElementType::CONNECTOR;
    } else if (eType.compare("CONNECTOR_END") == 0) {
        return ElementType::CONNECTOR_END;
    } else if (eType.compare("CONNECTABLE_ELEMENT") == 0) {
        return ElementType::CONNECTABLE_ELEMENT;
    } else if (eType.compare("CONTROL_FLOW") == 0) {
        return ElementType::CONTROL_FLOW;
    } else if (eType.compare("CREATE_OBJECT_ACTION") == 0) {
        return ElementType::CREATE_OBJECT_ACTION;
    } else if (eType.compare("DATA_STORE_NODE") == 0) {
        return ElementType::DATA_STORE_NODE;
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
    } else if (eType.compare("EXCEPTION_HANDLER") == 0) {
        return ElementType::EXCEPTION_HANDLER;
    } else if (eType.compare("EXECUTABLE_NODE") == 0) {
        return ElementType::EXECUTABLE_NODE;
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
    } else if (eType.compare("GENERALIZATION_SET") == 0) {
        return ElementType::GENERALIZATION_SET;
    } else if (eType.compare("INITITAL_NODE") == 0) {
        return ElementType::INITIAL_NODE;
    } else if (eType.compare("INPUT_PIN") == 0) {
        return ElementType::INPUT_PIN;
    } else if (eType.compare("INSTANCE_SPECIFICATION") == 0) {
        return ElementType::INSTANCE_SPECIFICATION;
    } else if (eType.compare("INSTANCE_VALUE") == 0) {
        return ElementType::INSTANCE_VALUE;
    } else if (eType.compare("INTERFACE_UML") == 0) {
        return ElementType::INTERFACE_UML;
    } else if (eType.compare("INTERFACE_REALIZATION") == 0) {
        return ElementType::INTERFACE_REALIZATION;
    } else if (eType.compare("INTERRUPTIBLE_ACTIVITY_REGION") == 0) {
        return ElementType::INTERRUPTIBLE_ACTIVITY_REGION;
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
    } else if (eType.compare("MANIFESTATION") == 0) {
        return ElementType::MANIFESTATION;
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
    } else if (eType.compare("PIN") == 0) {
        return ElementType::PIN;
    } else if (eType.compare("PORT") == 0) {
        return ElementType::PORT;
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
    throw UmlParserException("Could not identify element type by keyword: " + eType + '!', "");
}

void setNamespace(NamedElement& el, ID id) {
    el.m_namespace.addReadOnly(id);
}

void setOwner(Element& el, ID id) {
    el.m_owner->addReadOnly(id);
}

namespace {

void parseScope(YAML::Node node, ParserMetaData& data, Element* ret) {
    if (ret->isSubClassOf(ElementType::PACKAGEABLE_ELEMENT)) {
        if (parseSingletonReference(node, data, "owningPackage", ret->as<PackageableElement>(), &PackageableElement::setOwningPackage, &PackageableElement::setOwningPackage)) {
            return;
        }
    }
    if (ret->isSubClassOf(ElementType::PACKAGE_MERGE)) {
        if (parseSingletonReference(node, data, "receivingPackage", ret->as<PackageMerge>(), &PackageMerge::setReceivingPackage, &PackageMerge::setReceivingPackage)) {
            return;
        }
    }
    if (ret->isSubClassOf(ElementType::PROFILE_APPLICATION)) {
        if (parseSingletonReference(node, data, "applyingPackage", ret->as<ProfileApplication>(), &ProfileApplication::setApplyingPackage, &ProfileApplication::setApplyingPackage)) {
            return;
        }
    }
    if (ret->isSubClassOf(ElementType::GENERALIZATION)) {
        if (parseSingletonReference(node, data, "specific", ret->as<Generalization>(), &Generalization::setSpecific, &Generalization::setSpecific)) {
            return;
        }
    }
    if (ret->isSubClassOf(ElementType::CLASSIFIER)) {
        if (node["namespace"]) {
            if (node["namespace"].IsScalar()) {
                if (isValidID(node["namespace"].as<std::string>())) {
                    setNamespace(ret->as<NamedElement>(), ID::fromString(node["namespace"].as<std::string>()));
                    return;
                }
            }
        }
    }
    if (ret->isSubClassOf(ElementType::OPERATION)) {
        if (parseSingletonReference(node, data, "class", ret->as<Operation>(), &Operation::setClass, &Operation::setClass)) {
            return;
        }
        if (parseSingletonReference(node, data, "dataType", ret->as<Operation>(), &Operation::setDataType, &Operation::setDataType)) {
            return;
        }
        if (parseSingletonReference(node, data, "interface", ret->as<Operation>(), &Operation::setInterface, &Operation::setInterface)) {
            return;
        }
    }
    if (ret->isSubClassOf(ElementType::PROPERTY)) {
        if (parseSingletonReference(node, data, "class", ret->as<Property>(), &Property::setClass, &Property::setClass)) {
            return;
        }
        if (parseSingletonReference(node, data, "dataType", ret->as<Property>(), &Property::setDataType, &Property::setDataType)) {
            return;
        }
        if (parseSingletonReference(node, data, "owningAssociation", ret->as<Property>(), &Property::setOwningAssociation, &Property::setOwningAssociation)) {
            return;
        }
        if (parseSingletonReference(node, data, "interface", ret->as<Property>(), &Property::setInterface, &Property::setInterface)) {
            return;
        }
    }
    if (ret->isSubClassOf(ElementType::FEATURE)) {
        if (node["featuringClassifier"]) {
            if (node["featuringClassifier"].IsScalar() && isValidID(node["featuringClassifier"].as<std::string>())) {
                ID id = ID::fromString(node["featuringClassifier"].as<std::string>());
                ret->as<Feature>().setFeaturingClassifier(id);
                setNamespace(ret->as<NamedElement>(), id); // not guranteed in api but cant think of other case
                return;
            }
        }
    }
    if (ret->isSubClassOf(ElementType::VALUE_SPECIFICATION)) {
        if (node["owner"]) {
            if (node["owner"].IsScalar()) {
                if (isValidID(node["owner"].as<std::string>())) {
                    setOwner(*ret, ID::fromString(node["owner"].as<std::string>()));
                    // TODO i think we also need to check parameterable element here too
                    return;
                }
            }
        }
    }
    if (ret->isSubClassOf(ElementType::PARAMETER)) {
        if (parseSingletonReference(node, data, "operation", ret->as<Parameter>(), &Parameter::setOperation, &Parameter::setOperation)) {
            return;
        }
        if (node["namespace"]) {
            if (node["namespace"].IsScalar()) {
                if (isValidID(node["namespace"].as<std::string>())) {
                    setNamespace(ret->as<NamedElement>(), ID::fromString(node["namespace"].as<std::string>()));
                    return;
                }
            }
        }
    }
    if (ret->isSubClassOf(ElementType::SLOT)) {
        if (parseSingletonReference(node, data, "owningInstance", ret->as<Slot>(), &Slot::setOwningInstance, &Slot::setOwningInstance)) {
            return;
        }
    }
    if (ret->isSubClassOf(ElementType::ENUMERATION_LITERAL)) {
        if (parseSingletonReference(node, data, "enumeration", ret->as<EnumerationLiteral>(), &EnumerationLiteral::setEnumeration, &EnumerationLiteral::setEnumeration)) {
            return;
        }
    }
    if (ret->isSubClassOf(ElementType::TEMPLATE_SIGNATURE)) {
        if (parseSingletonReference(node, data, "template", ret->as<TemplateSignature>(), &TemplateSignature::setTemplate, &TemplateSignature::setTemplate)) {
            return;
        }
    }
    if (ret->isSubClassOf(ElementType::TEMPLATE_PARAMETER)) {
        if (parseSingletonReference(node, data, "signature", ret->as<TemplateParameter>(), &TemplateParameter::setSignature, &TemplateParameter::setSignature)) {
            return;
        }
    }
    if (ret->isSubClassOf(ElementType::TEMPLATE_BINDING)) {
        if (parseSingletonReference(node, data, "boundElement", ret->as<TemplateBinding>(), &TemplateBinding::setBoundElement, &TemplateBinding::setBoundElement)) {
            return;
        }
    }
    if (ret->isSubClassOf(ElementType::TEMPLATE_PARAMETER_SUBSTITUTION)) {
        if (parseSingletonReference(node, data, "templateBinding", ret->as<TemplateParameterSubstitution>(), &TemplateParameterSubstitution::setTemplateBinding, &TemplateParameterSubstitution::setTemplateBinding)) {
            return;
        }
    }
    if (ret->isSubClassOf(ElementType::MANIFESTATION)) {
        if (node["client"]) {
            if (node["client"].IsScalar()) {
                if (isValidID(node["client"].as<std::string>())) {
                    ID id = ID::fromString(node["client"].as<std::string>());
                    ret->as<Manifestation>().getClient().add(id);
                    setOwner(*ret, id);
                    return;
                }
            }
        }
    }
    if (ret->isSubClassOf(ElementType::DEPLOYMENT)) {
        if (parseSingletonReference(node, data, "location", ret->as<Deployment>(), &Deployment::setLocation, &Deployment::setLocation)) {
            return;
        }
    }
    if (ret->isSubClassOf(ElementType::STEREOTYPE)) {
        if (parseSingletonReference(node, data, "profile", ret->as<Stereotype>(), &Stereotype::setProfile, &Stereotype::setProfile)) {
            return;
        }
    }
    if (ret->isSubClassOf(ElementType::INTERFACE_REALIZATION)) {
        if (parseSingletonReference(node, data, "implementingClassifier", ret->as<InterfaceRealization>(), &InterfaceRealization::setImplementingClassifier, &InterfaceRealization::setImplementingClassifier)) {
            return;
        }
    }
    if (ret->isSubClassOf(ElementType::ACTIVITY_NODE)) {
        if (parseSingletonReference(node, data, "activity", ret->as<ActivityNode>(), &ActivityNode::setActivity, &ActivityNode::setActivity)) {
            return;
        }
    }
    if (ret->isSubClassOf(ElementType::ACTIVITY_EDGE)) {
        if (parseSingletonReference(node, data, "activity", ret->as<ActivityEdge>(), &ActivityEdge::setActivity, &ActivityEdge::setActivity)) {
            return;
        }
    }
    if (ret->isSubClassOf(ElementType::COMMENT)) {
        if (node["owner"]) {
            if (node["owner"].IsScalar()) {
                if (isValidID(node["owner"].as<std::string>())) {
                    setOwner(*ret, ID::fromString(node["owner"].as<std::string>()));
                    return;
                }
            }
        }
    }
    if (ret->isSubClassOf(ElementType::BEHAVIOR)) {
        if (node["namespace"]) {
            if (node["namespace"].IsScalar()) {
                if (isValidID(node["owner"].as<std::string>())) {
                    setNamespace(ret->as<NamedElement>(), ID::fromString(node["namespace"].as<std::string>()));
                    return;
                }
            }
        }
    }
    if (ret->isSubClassOf(ElementType::PARAMETERABLE_ELEMENT)) {
        if (parseSingletonReference(node, data, "owningTemplateParameter", ret->as<ParameterableElement>(), &ParameterableElement::setOwningTemplateParameter, &ParameterableElement::setOwningTemplateParameter)) {
            return;
        }
        if (node["owner"]) {
            if (node["owner"].IsScalar()) {
                if (isValidID(node["owner"].as<std::string>())) {
                    setOwner(*ret, ID::fromString(node["owner"].as<std::string>()));
                    // ret->as<ParameterableElement>().setTemplateParameter(ID::fromString(node["owner"].as<std::string>()));
                    return;
                }
            }
        }
    }
};

ElementPtr parseNode(YAML::Node node, ParserMetaData& data) {
    ElementPtr ret(0);

    if (node["activity"] && node["activity"].IsMap()) {
        ret = &parseDefinition<Activity>(node, data, "activity", parseActivity);
    }

    if (node["activityFinalNode"]) {
        ret = &parseDefinition<ActivityFinalNode>(node, data, "activityFinalNode", parseActivityNode);
    }

    if (node["activityParameterNode"]) {
        ret = &parseDefinition<ActivityParameterNode>(node, data, "activityParameterNode", parseActivityParameterNode);
    }

    if (node["association"]) {
        Association& association = *data.m_manager->create<Association>();
        parseAssociation(node["association"], association, data);
        ret = &association;
    }

    if (node["artifact"]) {
        if (node["artifact"].IsMap()) {
            Artifact& artifact = *data.m_manager->create<Artifact>();
            parseArtifact(node["artifact"], artifact, data);
            ret = &artifact;
        }
    }

    if (node["centralBufferNode"]) {
        ret = &parseDefinition<CentralBufferNode>(node, data, "centralBufferNode", parseObjectNode);
    }

    if (node["class"]) {
        if (node["class"].IsMap()) {
            ClassPtr clazz = data.m_manager->create<Class>();
            parseClass(node["class"], *clazz, data);
            ret = clazz;
        }
    }

    if (node["comment"]) {
        if (node["comment"].IsMap()) {
            Comment& comment = *data.m_manager->create<Comment>();
            parseComment(node["comment"], comment, data);
            ret = &comment;
        }
    }

    if (node["connector"]) {
        ret = &parseDefinition<Connector>(node, data, "connector", parseConnector);
    }

    if (node["connectorEnd"]) {
        ret = &parseDefinition<ConnectorEnd>(node, data, "connectorEnd", parseConnectorEnd);
    }

    if (node["controlFlow"]) {
        ret = &parseDefinition<ControlFlow>(node, data, "controlFlow", parseActivityEdge);
    }

    if (node["dataStoreNode"]) {
        ret = &parseDefinition<DataStoreNode>(node, data, "dataStoreNode", parseObjectNode);
    }

    if (node["dataType"]) {
        if (node["dataType"].IsMap()) {
            DataType& dataType = *data.m_manager->create<DataType>();
            parseDataType(node["dataType"], dataType, data);
            ret = &dataType;
        }
    }

    if (node["decisionNode"]) {
        ret = &parseDefinition<DecisionNode>(node, data, "decisionNode", parseDecisionNode);
    }

    if (node["dependency"]) {
        Dependency& dependency = *data.m_manager->create<Dependency>();
        parseDependency(node["dependency"], dependency, data);
        ret = &dependency;
    }

    if (node["deployment"]) {
        Deployment& deployment = *data.m_manager->create<Deployment>();
        parseDeployment(node["deployment"], deployment, data);
        ret = &deployment;
    }

    if (node["enumeration"]) {
        if (node["enumeration"].IsMap()) {
            Enumeration& enumeration = *data.m_manager->create<Enumeration>();
            parseEnumeration(node["enumeration"], enumeration, data);
            ret = &enumeration;
        }
    }

    if (node["enumerationLiteral"]) {
        EnumerationLiteral& enumerationLiteral = *data.m_manager->create<EnumerationLiteral>();
        parseEnumerationLiteral(node["enumerationLiteral"], enumerationLiteral, data);
        ret = &enumerationLiteral;
    }

    if (node["exceptionHandler"]) {
        ret = &parseDefinition<ExceptionHandler>(node, data, "exceptionHandler", parseExceptionHandler);
    }
    
    if (node["executableNode"]) {
        ret = &parseDefinition<ExecutableNode>(node, data, "executableNode", parseExecutableNode);
    }

    if (node["expression"]) {
        if (node["expression"].IsMap()) {
            Expression& exp = *data.m_manager->create<Expression>();
            parseExpression(node["expression"], exp, data);
            ret = &exp;
        }
    }

    if (node["extension"]) {
        if (node["extension"].IsMap()) {
            Extension& extension = *data.m_manager->create<Extension>();
            parseExtension(node["extension"], extension, data);
            ret = &extension;
        }
    }

    if (node["extensionEnd"]) {
        ExtensionEnd& extensionEnd = *data.m_manager->create<ExtensionEnd>();
        parseProperty(node["extensionEnd"], extensionEnd, data);
        ret = &extensionEnd;
    }

    if (node["flowFinalNode"]) {
        ret = &parseDefinition<FlowFinalNode>(node, data, "flowFinalNode", parseActivityNode);
    }

    if (node["forkNode"]) {
        ret = &parseDefinition<ForkNode>(node, data, "forkNode", parseActivityNode);
    }

    if (node["generalization"]) {
        Generalization& generalization = *data.m_manager->create<Generalization>();
        parseGeneralization(node["generalization"], generalization, data);
        ret = &generalization;
    }

    if (node["generalizationSet"]) {
        GeneralizationSet& generalizationSet = *data.m_manager->create<GeneralizationSet>();
        parseGeneralizationSet(node["generalizationSet"], generalizationSet, data);
        ret = &generalizationSet;
    }

    if (node["initialNode"]) {
        ret = &parseDefinition<InitialNode>(node, data, "initialNode", parseActivityNode);
    }

    if (node["instanceSpecification"]) {
        InstanceSpecification& inst = *data.m_manager->create<InstanceSpecification>();
        parseInstanceSpecification(node["instanceSpecification"], inst, data);
        ret = &inst;
    }

    if (node["instanceValue"]) {
        InstanceValue& instVal = *data.m_manager->create<InstanceValue>();
        parseInstanceValue(node["instanceValue"], instVal, data);
        ret = &instVal;
    }

    if (node["interface"]) {
        if (node["interface"].IsMap()) {
            ret = &parseDefinition<Interface>(node, data, "interface", parseInterface);
        }
    }

    if (node["interfaceRealization"]) {
        ret = &parseDefinition<InterfaceRealization>(node, data, "interfaceRealization", parseInterfaceRealization);
    }

    if (node["joinNode"]) {
        ret = &parseDefinition<JoinNode>(node, data, "joinNode", parseJoinNode);
    }

    if (node["literalBool"]) {
        LiteralBool& lb = *data.m_manager->create<LiteralBool>();
        parseLiteralBool(node["literalBool"], lb, data);
        ret = &lb;
    }

    if (node["literalInt"]) {
        LiteralInt& li = *data.m_manager->create<LiteralInt>();
        parseLiteralInt(node["literalInt"], li, data);
        ret = &li;
    }

    if (node["literalNull"]) {
        LiteralNull& ln = *data.m_manager->create<LiteralNull>();
        parseTypedElement(node["literalNull"], ln, data);
        ret = &ln;
    }

    if (node["literalReal"]) {
        LiteralReal& lr = *data.m_manager->create<LiteralReal>();
        parseLiteralReal(node["literalReal"], lr, data);
        ret = &lr;
    }

    if (node["literalString"]) {
        LiteralString& ls = *data.m_manager->create<LiteralString>();
        parseLiteralString(node["literalString"], ls, data);
        ret = &ls;
    }

    if (node["literalUnlimitedNatural"]) {
        LiteralUnlimitedNatural& ln = *data.m_manager->create<LiteralUnlimitedNatural>();
        parseLiteralUnlimitedNatural(node["literalUnlimitedNatural"], ln, data);
        ret = &ln;
    }

    if (node["manifestation"]) {
        Manifestation& manifestation = *data.m_manager->create<Manifestation>();
        parseManifestation(node["manifestation"], manifestation, data);
        ret = &manifestation;
    }

    if (node["mergeNode"]) {
        ret = &parseDefinition<MergeNode>(node, data, "mergeNode", parseActivityNode);
    }

    if (node["model"]) {
        Model& model = *data.m_manager->create<Model>();
        parsePackage(node["model"], model, data);
        ret = &model;
    }

    if (node["objectFlow"]) {
        ret = &parseDefinition<ObjectFlow>(node, data, "objectFlow", parseObjectFlow);
    }

    if (node["opaqueBehavior"]) {
        OpaqueBehavior& bhv = *data.m_manager->create<OpaqueBehavior>();
        parseOpaqueBehavior(node["opaqueBehavior"], bhv, data);
        ret = &bhv;
    }

    if (node["operation"]) {
        if (node["operation"].IsMap()) {
            Operation& op = *data.m_manager->create<Operation>();
            parseOperation(node["operation"], op, data);
            ret = &op;
        }
    }

    if (node["package"]) {
        PackagePtr pckg = data.m_manager->create<Package>();
        parsePackage(node["package"], *pckg, data);
        ret = pckg.ptr();
    }

    if (node["packageMerge"]) {
        PackageMerge& packageMerge = *data.m_manager->create<PackageMerge>();
        parsePackageMerge(node["packageMerge"], packageMerge, data);
        ret = &packageMerge;
    }

    if (node["parameter"]) {
        Parameter& param = *data.m_manager->create<Parameter>();
        parseParameter(node["parameter"], param, data);
        ret = &param;
    }

    if (node["port"]) {
        ret = & parseDefinition<Port>(node, data, "port", parsePort);
    }

    if (node["primitiveType"]) {
        PrimitiveType& type = *data.m_manager->create<PrimitiveType>();
        parsePrimitiveType(node["primitiveType"], type, data);
        ret = &type;
    }

    if (node["profile"] && node["profile"].IsMap()) {
        Profile& profile = *data.m_manager->create<Profile>();
        parsePackage(node["profile"], profile, data);
        ret = &profile;
    }

    if (node["profileApplication"]) {
        ProfileApplication& profileApplication = *data.m_manager->create<ProfileApplication>();
        parseProfileApplication(node["profileApplication"], profileApplication, data);
        ret = &profileApplication;
    }

    if (node["property"]) {
        Property& prop = *data.m_manager->create<Property>();
        parseProperty(node["property"], prop, data);
        ret = &prop;
    }

    if (node["realization"]) {
        Realization& realization = *data.m_manager->create<Realization>();
        parseDependency(node["realization"], realization, data);
        ret = &realization;
    }

    if (node["signal"]) {
        Signal& signal = *data.m_manager->create<Signal>();
        parseSignal(node["signal"], signal, data);
        ret = &signal;
    }

    if (node["slot"]) {
        Slot& slot = *data.m_manager->create<Slot>();
        parseSlot(node["slot"], slot, data);
        ret = &slot;
    }

    if (node["stereotype"]) {
        Stereotype& stereotype = *data.m_manager->create<Stereotype>();
        parseClass(node["stereotype"], stereotype, data);
        ret = &stereotype;
    }

    if (node["templateBinding"]) {
        if (node["templateBinding"].IsMap()) {
            TemplateBinding& binding = *data.m_manager->create<TemplateBinding>();
            parseTemplateBinding(node["templateBinding"], binding, data);
            ret = &binding;
        }
    }

    if (node["templateParameter"]) {
        TemplateParameter& parameter = *data.m_manager->create<TemplateParameter>();
        parseTemplateParameter(node["templateParameter"], parameter, data);
        ret = &parameter;
    }

    if (node["templateParameterSubstitution"]) {
        TemplateParameterSubstitution& templateParameterSubstitution = *data.m_manager->create<TemplateParameterSubstitution>();
        parseTemplateParameterSubstitution(node["templateParameterSubstitution"], templateParameterSubstitution, data);
        ret = &templateParameterSubstitution;
    }

    if (node["templateSignature"]) {
        TemplateSignature& templateSignature = *data.m_manager->create<TemplateSignature>();
        parseTemplateSignature(node["templateSignature"], templateSignature, data);
        ret = &templateSignature;
    }

    if (node["usage"]) {
        Usage& usage = *data.m_manager->create<Usage>();
        parseDependency(node["usage"], usage, data);
        ret = &usage;
    }

    if (ret && data.m_strategy == ParserStrategy::INDIVIDUAL) {
        parseScope(node, data, &*ret);
    }

    return ret;
}

ElementPtr parseExternalAddToManager(ParserMetaData& data, string path) {
    if (filesystem::exists(data.m_path.parent_path() / path)) {
        filesystem::path cPath = data.m_path;
        data.m_path = cPath.parent_path() / path;
        ElementPtr ret = parse(data);
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
    if (newPath.empty() || (/**newPath.parent_path().compare(data.m_path) == 0 &&**/ newPath.filename().compare(data.m_fileName) == 0)) {
        determineTypeAndEmit(emitter, el, data);
    } else {
        emitter << YAML::Value << newPath.filename().string();
        if (data.m_strategy == EmitterStrategy::WHOLE) {
            emitToFile(el, data, newPath.parent_path().string(), newPath.filename().string());
        }
    }
}

template <class T = Element, class V = T>
void emitDefinition(YAML::Emitter& emitter, EmitterMetaData& data, std::string name, V& el, void (*emitSig)(YAML::Emitter&, T&, EmitterMetaData&)) {
    if (V::elementType() != T::elementType()) {
        emitElementDefenition(emitter, V::elementType(), name, el, data);
        (*emitSig)(emitter, el, data);
        emitElementDefinitionEnd(emitter, V::elementType(), el);
    } else {
        (*emitSig)(emitter, el, data);
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
        case ElementType::ACTIVITY : {
            emitActivity(emitter, el.as<Activity>(), data);
            break;
        }
        case ElementType::ACTIVITY_FINAL_NODE : {
            emitElementDefenition(emitter, ElementType::ACTIVITY_FINAL_NODE, "activityFinalNode", el, data);
            emitActivityNode(emitter, el.as<ActivityNode>(), data);
            break;
        }
        case ElementType::ACTIVITY_PARAMETER_NODE : {
            emitActivityParameterNode(emitter, el.as<ActivityParameterNode>(), data);
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
        case ElementType::CENTRAL_BUFFER_NODE : {
            emitElementDefenition(emitter, ElementType::CENTRAL_BUFFER_NODE, "centralBufferNode", el.as<CentralBufferNode>(), data);
            emitObjectNode(emitter, el.as<CentralBufferNode>(), data);
            emitElementDefenitionEnd(emitter, ElementType::CENTRAL_BUFFER_NODE, el.as<CentralBufferNode>());
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
        case ElementType::CONNECTOR : {
            emitConnector(emitter, el.as<Connector>(), data);
            break;
        }
        case ElementType::CONNECTOR_END : {
            emitConnectorEnd(emitter, el.as<ConnectorEnd>(), data);
            break;
        }
        case ElementType::CONTROL_FLOW : {
            emitElementDefenition(emitter, ElementType::CONTROL_FLOW, "controlFlow", el, data);
            emitActivityEdge(emitter, el.as<ControlFlow>(), data);
            emitElementDefenitionEnd(emitter, ElementType::CONTROL_FLOW, el);
            break;
        }
        case ElementType::DATA_STORE_NODE : {
            emitElementDefenition(emitter, ElementType::DATA_STORE_NODE, "dataStoreNode", el, data);
            emitObjectNode(emitter, el.as<ObjectNode>(), data);
            emitElementDefenitionEnd(emitter, ElementType::DATA_STORE_NODE, el);
            break;
        }
        case ElementType::DATA_TYPE : {
            emitDataType(emitter, dynamic_cast<DataType&>(el), data);
            break;
        }
        case ElementType::DECISION_NODE : {
            emitDecisionNode(emitter, el.as<DecisionNode>(), data);
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
        case ElementType::EXCEPTION_HANDLER : {
            emitExceptionHandler(emitter, el.as<ExceptionHandler>(), data);
            break;
        }
        case ElementType::EXECUTABLE_NODE : {
            emitExecutableNode(emitter, el.as<ExecutableNode>(), data);
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
        case ElementType::FLOW_FINAL_NODE : {
            emitElementDefenition(emitter, ElementType::FLOW_FINAL_NODE, "flowFinalNode", el, data);
            emitActivityNode(emitter, el.as<ActivityNode>(), data);
            emitElementDefenitionEnd(emitter, ElementType::FLOW_FINAL_NODE, el);
            break;
        }
        case ElementType::FORK_NODE : {
            emitElementDefenition(emitter, ElementType::FORK_NODE, "forkNode", el, data);
            emitActivityNode(emitter, el.as<ActivityNode>(), data);
            emitElementDefenitionEnd(emitter, ElementType::FORK_NODE, el);
            break;
        }
        case ElementType::GENERALIZATION : {
            emitGeneralization(emitter, el.as<Generalization>(), data);
            break;
        }
        case ElementType::GENERALIZATION_SET : {
            emitGeneralizationSet(emitter, el.as<GeneralizationSet>(), data);
            break;
        }
        case ElementType::INITIAL_NODE : {
            emitElementDefenition(emitter, ElementType::INITIAL_NODE, "initialNode", el, data);
            emitActivityNode(emitter, el.as<ActivityNode>(), data);
            emitElementDefenitionEnd(emitter, ElementType::INITIAL_NODE, el);
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
        case ElementType::INTERFACE_UML : {
            emitInterface(emitter, el.as<Interface>(), data);
            break;
        }
        case ElementType::INTERFACE_REALIZATION : {
            emitInterfaceRealization(emitter, el.as<InterfaceRealization>(), data);
            break;
        }
        case ElementType::JOIN_NODE : {
            emitJoinNode(emitter, el.as<JoinNode>(), data);
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
        case ElementType::MERGE_NODE : {
            emitElementDefenition(emitter, ElementType::MERGE_NODE, "mergeNode", el, data);
            emitActivityNode(emitter, el.as<ActivityNode>(), data);
            emitElementDefenitionEnd(emitter, ElementType::MERGE_NODE, el);
            break;
        }
        case ElementType::MODEL : {
            emitModel(emitter, dynamic_cast<Model&>(el), data);
            break;
        }
        case ElementType::OBJECT_FLOW : {
            emitObjectFlow(emitter, el.as<ObjectFlow>(), data);
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
        case ElementType::PORT : {
            emitPort(emitter, el.as<Port>(), data);
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
        case ElementType::RECEPTION : {
            emitReception(emitter, el.as<Reception>(), data);
            break;
        }
        case ElementType::SIGNAL : {
            emitSignal(emitter, el.as<Signal>(), data);
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
        if (el.isSubClassOf(ElementType::STEREOTYPE)) {
            if (el.as<Stereotype>().getProfile()) {
                emitter << YAML::Key << "profile" << el.as<Stereotype>().getProfile().id().string();
                return;
            }
        }
        if (el.isSubClassOf(ElementType::PACKAGEABLE_ELEMENT)) {
            if (el.as<PackageableElement>().getOwningPackage()) {
                emitter << YAML::Key << "owningPackage" << el.as<PackageableElement>().getOwningPackage().id().string();
                return;
            }
        }
        if (el.isSubClassOf(ElementType::PROFILE_APPLICATION)) {
            if (el.as<ProfileApplication>().getApplyingPackage()) {
                emitter << YAML::Key << "applyingPackage" << YAML::Value << el.as<ProfileApplication>().getApplyingPackage().id().string();
                return;
            }
        }
        if (el.isSubClassOf(ElementType::PACKAGE_MERGE)) {
            if (el.as<PackageMerge>().getReceivingPackage()) {
                emitter << YAML::Key << "receivingPackage" << el.as<PackageMerge>().getReceivingPackage().id().string();
                return;
            }
        }
        if (el.isSubClassOf(ElementType::PROPERTY)) {
            if (el.as<Property>().getClass()) {
                emitter << YAML::Key << "class" << el.as<Property>().getClass().id().string();
                return;
            }
            if (el.as<Property>().getDataType()) {
                emitter << YAML::Key << "dataType" << YAML::Value << el.as<Property>().getDataType().id().string();
                return;
            }
            if (el.as<Property>().getOwningAssociation()) {
                emitter << YAML::Key << "owningAssociation" << YAML::Value << el.as<Property>().getOwningAssociation().id().string();
                return;
            }
            if (el.as<Property>().getInterface()) {
                emitter << YAML::Key << "interface" << YAML::Value << el.as<Property>().getInterface().id().string();
                return;
            }
        }
        if (el.isSubClassOf(ElementType::OPERATION)) {
            if (el.as<Operation>().getClass()) {
                emitter << YAML::Key << "class" << YAML::Value << el.as<Operation>().getClass().id().string();
                return;
            }
            if (el.as<Operation>().getDataType()) {
                emitter << YAML::Key << "dataType" << YAML::Value << el.as<Operation>().getDataType().id().string();
                return;
            }
            if (el.as<Operation>().getInterface()) {
                emitter << YAML::Key << "interface" << YAML::Value << el.as<Operation>().getInterface().id().string();
                return;
            }
        }
        if (el.isSubClassOf(ElementType::FEATURE)) {
            if (el.as<Feature>().getFeaturingClassifier()) {
                emitter << YAML::Key << "featuringClassifier" << YAML::Value << el.as<Feature>().getFeaturingClassifier().id().string();
                return;
            }
        }
        if (el.isSubClassOf(ElementType::GENERALIZATION)) {
            if (el.as<Generalization>().getSpecific()) {
                emitter << YAML::Key << "specific" << YAML::Value << el.as<Generalization>().getSpecific().id().string();
                return;
            }
        }
        if (el.isSubClassOf(ElementType::CLASSIFIER)) {
            if (el.as<Classifier>().getNamespace()) {
                emitter << YAML::Key << "namespace" << YAML::Value << el.as<Classifier>().getNamespace().id().string();
                return;
            }
        }
        if (el.isSubClassOf(ElementType::SLOT)) {
            if (el.as<Slot>().getOwningInstance()) {
                emitter << YAML::Key << "owningInstance" << YAML::Value << el.as<Slot>().getOwningInstance().id().string();
                return;
            }
        }
        if (el.isSubClassOf(ElementType::PARAMETER)) {
            if (el.as<Parameter>().getOperation()) {
                emitter << YAML::Key << "operation" << YAML::Value << el.as<Parameter>().getOperation().id().string();
                return;
            } else if (el.as<NamedElement>().getNamespace()) {
                emitter << YAML::Key << "namespace" << YAML::Value << el.as<NamedElement>().getNamespace().id().string();
                return;
            }
        }
        if (el.isSubClassOf(ElementType::ENUMERATION_LITERAL)) {
            if (el.as<EnumerationLiteral>().getEnumeration()) {
                emitter << YAML::Key << "enumeration" << YAML::Value << el.as<EnumerationLiteral>().getEnumeration().id().string();
                return;
            }
        }
        if (el.isSubClassOf(ElementType::TEMPLATE_SIGNATURE)) {
            if (el.as<TemplateSignature>().getTemplate()) {
                emitter << YAML::Key << "template" << YAML::Value << el.as<TemplateSignature>().getTemplate().id().string();
                return;
            }
        }
        if (el.isSubClassOf(ElementType::TEMPLATE_PARAMETER)) {
            if (el.as<TemplateParameter>().getSignature()) {
                emitter << YAML::Key << "signature" << YAML::Value << el.as<TemplateParameter>().getSignature().id().string();
                return;
            }
        }
        if (el.isSubClassOf(ElementType::TEMPLATE_BINDING)) {
            if (el.as<TemplateBinding>().getBoundElement()) {
                emitter << YAML::Key << "boundElement" << YAML::Value << el.as<TemplateBinding>().getBoundElement().id().string();
                return;
            }
        }
        if (el.isSubClassOf(ElementType::TEMPLATE_PARAMETER_SUBSTITUTION)) {
            if (el.as<TemplateParameterSubstitution>().getTemplateBinding()) {
                emitter << YAML::Key << "templateBinding" << YAML::Value << el.as<TemplateParameterSubstitution>().getTemplateBinding().id().string();
                return;
            }
        }
        if (el.isSubClassOf(ElementType::DEPLOYMENT)) {
            if (el.as<Deployment>().getLocation()) {
                emitter << YAML::Key << "location" << YAML::Value << el.as<Deployment>().getLocation().id().string();
                return;
            }
        }
        if (el.isSubClassOf(ElementType::MANIFESTATION)) {
            if (!el.as<Manifestation>().getClient().empty()) {
                emitter << YAML::Key << "client" << YAML::Value << el.as<Manifestation>().getClient().ids().front().string();
                return;
            }
        }
        if (el.isSubClassOf(ElementType::INTERFACE_REALIZATION)) {
            if (el.as<InterfaceRealization>().getImplementingClassifier()) {
                emitter << YAML::Key << "implementingClassifier" << el.as<InterfaceRealization>().getImplementingClassifier().id().string();
                return;
            }
        }
        if (el.isSubClassOf(ElementType::ACTIVITY_EDGE)) {
            if (el.as<ActivityEdge>().getActivity()) {
                emitter << YAML::Key << "activity" << el.as<ActivityEdge>().getActivity().id().string();
                return;
            }
        }
        if (el.isSubClassOf(ElementType::ACTIVITY_NODE)) {
            if (el.as<ActivityNode>().getActivity()) {
                emitter << YAML::Key << "activity" << el.as<ActivityNode>().getActivity().id().string();
                return;
            }
        }
        if (el.isSubClassOf(ElementType::COMMENT)) {
            if (el.getOwner()) {
                emitter << YAML::Key << "owner" << YAML::Value << el.getOwner().id().string();
                return;
            }
        }
        if (el.isSubClassOf(ElementType::PARAMETERABLE_ELEMENT)) {
            if (el.as<ParameterableElement>().getOwningTemplateParameter()) {
                emitter << YAML::Key << "owningTemplateParameter" << YAML::Value << el.as<ParameterableElement>().getOwningTemplateParameter().id().string();
                return;
            }
            else if (el.getOwner()) { // special case
                emitter << YAML::Key << "owner" << YAML::Value << el.getOwner().id().string();
                return;
            }
        }
    }
}

template <class T = Element, class U = Element, class S = Set<T,U>> 
void emitSequence(YAML::Emitter& emitter, string sequenceName, EmitterMetaData& data, U& el, S& (U::* sequenceMethod)()) {
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

template<class T = Element, class U =  Element, class S = Set<T,U>>
void emitSequenceReferences(YAML::Emitter& emitter, std::string sequenceName, EmitterMetaData& data, U& el, S& (U::* sequenceMethod)()) {
    if (!(el.*sequenceMethod)().empty()) {
        emitter << YAML::Key << sequenceName << YAML::Value << YAML::BeginSeq;
        for (ID id : (el.*sequenceMethod)().ids()) {
            emitter << YAML::Value << id.string();
        }
        emitter << YAML::EndSeq;
    }
}

template<class T = Element, class U = Element>
void emitSingletonDefinition(YAML::Emitter& emitter, std::string singletonName, EmitterMetaData& data, U& el, UmlPtr<T> (U::* accessor)() const) {
    if ((el.*accessor)()) {
        emitter << YAML::Key << singletonName << YAML::Value;
        if (data.m_strategy == EmitterStrategy::WHOLE) {
            emit(emitter, *(el.*accessor)(), data);
        } else {
            emitter << (el.*accessor)().id().string() + ".yml";
        }
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

Comment& determineAndParseOwnedComment(YAML::Node node, ParserMetaData& data) {
    if (node["comment"]) {
        Comment& comment = *data.m_manager->create<Comment>();
        parseComment(node["comment"], comment, data);
        return comment;
    }
    else {
        throw UmlParserException("Invalid uml definition for comment!", data.m_path.string(), node);
    }
}

InstanceSpecification& determinAndParseAppliedStereotype(YAML::Node node, ParserMetaData& data) {
    if (node["instanceSpecification"]) {
        InstanceSpecification& instance = *data.m_manager->create<InstanceSpecification>();
        parseInstanceSpecification(node["instanceSpecification"], instance, data);
        return instance;
    } else {
        throw UmlParserException("Invalid uml element identifier for appliedStereotype entry, can only be an InstanceSpecification!", data.m_path.string(), node);
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
    parseSequenceDefinitions<Comment, Element, Set<Comment,Element>>(node, data, "ownedComments", el, &Element::getOwnedComments, determineAndParseOwnedComment);
    parseSequenceDefinitions(node, data, "appliedStereotypes", el, &Element::getAppliedStereotypes, determinAndParseAppliedStereotype);
}

void emitElement(YAML::Emitter& emitter, Element& el, EmitterMetaData& data) {
    emitter << YAML::Key << "id" << YAML::Value << el.getID().string();
    emitSequence(emitter, "ownedComments", data, el, &Element::getOwnedComments);
    emitSequence(emitter, "appliedStereotypes", data, el, &Element::getAppliedStereotypes);
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

    // if (node["clientDependencies"]) {
    //     if (node["clientDependencies"].IsSequence()) {
    //         for (size_t i = 0; i < node["clientDependencies"].size(); i++) {
    //             if (node["clientDependencies"][i].IsScalar()) {
    //                 ID clientDependencyID = ID::fromString(node["clientDependencies"][i].as<string>());
    //                 // if (data.m_strategy == ParserStrategy::WHOLE) {
    //                 //     applyFunctor(data, clientDependencyID, new AddClientDepencyFunctor(&el, node["clientDependencies"][i]));
    //                 // } else {
    //                     if (data.m_manager->loaded(clientDependencyID)) {
    //                         el.getClientDependencies().add(data.m_manager->get(clientDependencyID).as<Dependency>());
    //                     } else {
    //                         el.getClientDependencies().add(clientDependencyID);
    //                     }
    //                 // }
    //             } else {
    //                 throw UmlParserException("Invalid yaml node type for NamedElement clientDependencies entry, must be a scalar!", data.m_path.string(), node["clientDependencies"][i]);
    //             }
    //         }
    //     } else {
    //         throw UmlParserException("Improper yaml node type for NamedElement field clientDependencies, must be a sequence!", data.m_path.string(), node["clientDependencies"]);
    //     }
    // }
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
}

void emitTypedElement(YAML::Emitter& emitter, TypedElement& el, EmitterMetaData& data) {
    emitNamedElement(emitter, el, data);

    if (el.getType()) {
        emitter << YAML::Key << "type" << YAML::Value << el.getType().id().string();
    }
}

Generalization& determineAndParseGeneralization(YAML::Node node, ParserMetaData& data) {
    if (node["generalization"]) {
        Generalization& gen = *data.m_manager->create<Generalization>();
        parseGeneralization(node["generalization"], gen, data);
        return gen;
    } else {
        throw UmlParserException("Invalid uml type for generalizations entry!", data.m_path.string(), node);
    }
}

void emitClassifier(YAML::Emitter& emitter, Classifier& clazz, EmitterMetaData& data) {
    emitNamedElement(emitter, clazz, data);
    emitTemplateableElement(emitter, clazz, data);
    emitParameterableElement(emitter, clazz, data);
    emitSequence(emitter, "generalizations", data, clazz, &Classifier::getGeneralizations);
    if (!clazz.getPowerTypeExtent().empty()) {
        emitter << YAML::Key << "powerTypeExtent" << YAML::Value << YAML::BeginSeq;
        for (const ID id : clazz.getPowerTypeExtent().ids()) {
            emitter << YAML::Value << id.string();
        }
        emitter << YAML::EndSeq;
    }
}

void emitGeneralization(YAML::Emitter& emitter, Generalization& generalization, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::GENERALIZATION, "generalization", generalization, data);

    emitElement(emitter, generalization, data);

    if (generalization.getGeneral()) {
        emitter << YAML::Key << "general" << YAML::Value << generalization.getGeneral().id().string();
    }
    
    if (!generalization.getGeneralizationSets().empty()) {
        emitter << YAML::Key << "generalizationSets" << YAML::Value << YAML::BeginSeq;
        for (const ID id : generalization.getGeneralizationSets().ids()) {
            emitter << YAML::Value << id.string();
        }
        emitter << YAML::EndSeq;
    }

    emitElementDefenitionEnd(emitter, ElementType::GENERALIZATION, generalization);
}

Property& determineAndParseOwnedAttribute(YAML::Node node, ParserMetaData& data) {
    if (node["property"]) {
        if (node["property"].IsMap()) {
            Property& prop = *data.m_manager->create<Property>();
            parseProperty(node["property"], prop, data);
            return prop;
        } else {
            throw UmlParserException("Invalid yaml node type for property definition, must be a map!", data.m_path.string(), node["property"]);
        }
    } else if (node["port"]) {
        if (node["port"].IsMap()) {
            Port& port = *data.m_manager->create<Port>();
            parsePort(node["port"], port, data);
            return port;
        } else {
            throw UmlParserException("Invalid yaml node type for port definition, must be a map!", data.m_path.string(), node["port"]);
        }
    } else {
        throw UmlParserException("Invalid uml definition for ownedAttribute, may be a property or port only!", data.m_path.string(), node);
    }
}

Operation& determineAndParseOwnedOperation(YAML::Node node, ParserMetaData& data) {
    if (node["operation"]) {
        Operation& op = *data.m_manager->create<Operation>();
        parseOperation(node["operation"], op, data);
        return op;
    } else {
        throw UmlParserException("Invalid uml definition for ownedOperation, may be an operation only!", data.m_path.string(), node);
    }
}

void parseDataType(YAML::Node node, DataType& dataType, ParserMetaData& data) {
    parseClassifier(node, dataType, data);
    parseSequenceDefinitions(node, data, "ownedAttribute", dataType, &DataType::getOwnedAttributes, determineAndParseOwnedAttribute);
    parseSequenceDefinitions(node, data, "ownedOperation", dataType, &DataType::getOwnedOperations, determineAndParseOwnedOperation);
}

void emitDataType(YAML::Emitter& emitter, DataType& dataType, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::DATA_TYPE, "dataType", dataType, data);
    emitClassifier(emitter, dataType, data);
    emitSequence(emitter, "ownedAttribute", data, dataType, &DataType::getOwnedAttributes);
    emitSequence(emitter, "ownedOperation", data, dataType, &DataType::getOwnedOperations);
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

Connector& determineAndParseConnector(YAML::Node node, ParserMetaData& data) {
    if (node["connector"]) {
        return parseDefinition<Connector>(node, data, "connector", &parseConnector);
    } else {
        throw UmlParserException("Invalid key for connector entry!", data.m_path.string(), node);
    }
}

void parseStructuredClassifier(YAML::Node node, StructuredClassifier& clazz, ParserMetaData& data) {
    parseClassifier(node, clazz, data);
    parseSequenceDefinitions(node, data, "ownedAttributes", clazz, &StructuredClassifier::getOwnedAttributes, determineAndParseOwnedAttribute);
    parseSequenceDefinitions(node, data, "ownedConnectors", clazz, &StructuredClassifier::getOwnedConnectors, determineAndParseConnector);
}

void emitStructuredClassifier(YAML::Emitter& emitter, StructuredClassifier& clazz, EmitterMetaData& data) {
    emitClassifier(emitter, clazz, data);
    emitSequence(emitter, "ownedAttributes", data, clazz, &StructuredClassifier::getOwnedAttributes);
    emitSequence(emitter, "ownedConnectors", data, clazz, &StructuredClassifier::getOwnedConnectors);
}

Classifier& determineAndParseClassifier(YAML::Node node, ParserMetaData& data) {
    if (node["activity"]) {
        return parseDefinition<Activity>(node, data, "activity", parseActivity);
    } else if (node["artifact"]) {
        if (node["artifact"].IsMap()) {
            Artifact& artifact = *data.m_manager->create<Artifact>();
            parseArtifact(node["artifact"], artifact, data);
            return artifact;
        }  else {
            throw UmlParserException("Invalid yaml node type for enumeration artifact", data.m_path.string(), node["artifact"]);
        }
    } else if (node["association"]) { // is this valid IG, seems weird
        if (node["association"].IsMap()) {
            Association& association = *data.m_manager->create<Association>();
            parseAssociation(node["association"], association, data);
            return association;
        } else {
            throw UmlParserException("Invalid yaml node type for association definition", data.m_path.string(), node["association"]);
        }
    } else if (node["class"]) {
        if (node["class"].IsMap()) {
            Class& nestedClazz = *data.m_manager->create<Class>();
            parseClass(node["class"], nestedClazz, data);
            return nestedClazz;
        } else {
            throw UmlParserException("Invalid yaml node type for class definition", data.m_path.string(), node["class"]);
        }
    } else if (node["dataType"]) {
        if (node["dataType"].IsMap()) {
            DataType& dataType = *data.m_manager->create<DataType>();
            parseDataType(node["dataType"], dataType, data);
            return dataType;
        }  else {
            throw UmlParserException("Invalid yaml node type for dataType definition", data.m_path.string(), node["dataType"]);
        }
    } else if (node["enumeration"]) {
        if (node["enumeration"].IsMap()) {
            Enumeration& enumeration = *data.m_manager->create<Enumeration>();
            parseEnumeration(node["enumeration"], enumeration, data);
            return enumeration;
        } else {
            throw UmlParserException("Invalid yaml node type for enumeration definition", data.m_path.string(), node["enumeration"]);
        }
    } else if (node["extension"]) {
        if (node["extension"].IsMap()) {
            Extension& extension = *data.m_manager->create<Extension>();
            parseExtension(node["extension"], extension, data);
            return extension;
        } else {
            throw UmlParserException("Invalide yaml node type for extension definition, must be a map!", data.m_path.string(), node["extension"]);
        }
    } else if (node["interface"]) {
        return parseDefinition<Interface>(node, data, "interface", parseInterface);
    } else if (node["opaqueBehavior"]) {
        if (node["opaqueBehavior"].IsMap()) {
            OpaqueBehavior& opaqueBehavior = *data.m_manager->create<OpaqueBehavior>();
            parseOpaqueBehavior(node["opaqueBehavior"], opaqueBehavior, data);
            return opaqueBehavior;
        } else {
            throw UmlParserException("Invalid yaml node type for opaqueBehavior definition", data.m_path.string(), node["opaqueBehavior"]);
        }
    } else if (node["primitiveType"]) {
        if (node["primitiveType"].IsMap()) {
            PrimitiveType& primitiveType = *data.m_manager->create<PrimitiveType>();
            parsePrimitiveType(node["primitiveType"], primitiveType, data);
            return primitiveType;
        } else {
            throw UmlParserException("Invalid yaml node type for primitiveType definition", data.m_path.string(), node["primitiveType"]);
        }
    } else if (node["signal"]) {
        if (node["signal"].IsMap()) {
            Signal& signal = *data.m_manager->create<Signal>();
            parseSignal(node["signal"], signal, data);
            return signal;
        } else {
            throw UmlParserException("Invalid yaml node type for signal definition", data.m_path.string(), node["signal"]);
        }
    } else {
        throw UmlParserException("invalid classifier definition for entry!", data.m_path.string(), node);
    }
}

Reception& determineAndParseOwnedReception(YAML::Node node, ParserMetaData& data) {
    if (node["reception"]) {
        if (node["reception"].IsMap()) {
            Reception& reception = *data.m_manager->create<Reception>();
            parseReception(node["reception"], reception, data);
            return reception;
        } else {
            throw UmlParserException("Invalid yaml node type for reception definition", data.m_path.string(), node["reception"]);
        }
    } else {
        throw UmlParserException("invalid ownedReceptions definition", data.m_path.string(), node);
    }
}

void parseClass(YAML::Node node, Class& clazz, ParserMetaData& data) {
    parseStructuredClassifier(node, clazz, data);
    parseBehavioredClassifier(node, clazz, data);
    parseSequenceDefinitions(node, data, "ownedOperations", clazz, &Class::getOwnedOperations, determineAndParseOwnedOperation);
    parseSequenceDefinitions(node, data, "nestedClassifiers", clazz, &Class::getNestedClassifiers, determineAndParseClassifier);
    parseSequenceDefinitions(node, data, "ownedReceptions", clazz, &Class::getOwnedReceptions, determineAndParseOwnedReception);
}

void emitClass(YAML::Emitter& emitter, Class& clazz, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::CLASS, "class", clazz, data);
    emitStructuredClassifier(emitter, clazz, data);
    emitBehavioredClassifier(emitter, clazz, data);
    emitSequence(emitter, "ownedOperations", data, clazz, &Class::getOwnedOperations);
    emitSequence(emitter, "nestedClassifiers", data, clazz, &Class::getNestedClassifiers);
    emitSequence(emitter, "ownedReceptions", data, clazz, &Class::getOwnedReceptions);
    emitElementDefenitionEnd(emitter, ElementType::CLASS, clazz);
}

Parameter& determineAndParseParameter(YAML::Node node, ParserMetaData& data) {
    if (node["parameter"]) {
        if (node["parameter"].IsMap()) {
            Parameter& param = *data.m_manager->create<Parameter>();
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
    parseSingletonReference(node, data, "specification", bhv, &Behavior::setSpecification, &Behavior::setSpecification);
}

void emitBehavior(YAML::Emitter& emitter, Behavior& bhv, EmitterMetaData& data) {
    emitClass(emitter, bhv, data);
    emitSequence(emitter, "parameters", data, bhv, &Behavior::getOwnedParameters);
    if (bhv.getSpecification()) {
        emitter << YAML::Key << "specification" << YAML::Value << bhv.getSpecification().id().string();
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
                        LiteralString& bodyUML = *data.m_manager->create<LiteralString>();
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
            Expression& newExp = *data.m_manager->create<Expression>();
            parseExpression(node["expression"], newExp, data);
            return newExp;
        } else {
            throw UmlParserException("Invalid YAML node type, ", data.m_path.string(), node["expression"]);
        }
    } else if (node["literalBool"]) {
        if (node["literalBool"].IsMap()) {
            LiteralBool& lb = *data.m_manager->create<LiteralBool>();
            parseLiteralBool(node["literalBool"], lb, data);
            return lb;
        } else {
            throw UmlParserException("Improper YAML node type, ", data.m_path.string(), node["literalBool"]);
        }
    } else if (node["literalInt"]) {
        if (node["literalInt"].IsMap()) {
            LiteralInt& li = *data.m_manager->create<LiteralInt>();
            parseLiteralInt(node["literalInt"], li, data);
            return li;
        } else {
            throw UmlParserException("Improper YAML node type, ", data.m_path.string(), node["literalInt"]);
        }
    } else if (node["literalNull"]) {
        if (node["literalNull"].IsMap()) {
            LiteralNull& ln = *data.m_manager->create<LiteralNull>();
            parseTypedElement(node["literalNull"], ln, data);
            return ln;
        } else {
            throw UmlParserException("Improper YAML node type, ", data.m_path.string(), node["literalNull"]);
        }
    } else if (node["literalReal"]) {
        if (node["literalReal"].IsMap()) {
            LiteralReal& lr = *data.m_manager->create<LiteralReal>();
            parseLiteralReal(node["literalReal"], lr, data);
            return lr;
        } else {
            throw UmlParserException("Improper YAML node type, ", data.m_path.string(), node["literalReal"]);
        }
    } else if (node["literalString"]) {
        if (node["literalString"].IsMap()) {
            LiteralString& ls = *data.m_manager->create<LiteralString>();
            parseLiteralString(node["literalString"], ls, data);
            return ls;
        } else {
            throw UmlParserException("Improper YAML node type, ", data.m_path.string(), node["literalString"]);
        }
    } else if (node["literalUnlimtedNatural"]) {
        if (node["literalUnlimitedNatural"]) {
            LiteralUnlimitedNatural& ln = *data.m_manager->create<LiteralUnlimitedNatural>();
            parseLiteralUnlimitedNatural(node["literalUnlimitedNatural"], ln, data);
            return ln;
        } else {
            throw UmlParserException("Improper YAML node type, ", data.m_path.string(), node["literalUnlimitedNatural"]);
        }
    } else if (node["instanceValue"]) {
        if (node["instanceValue"].IsMap()) {
            InstanceValue& iv = *data.m_manager->create<InstanceValue>();
            parseInstanceValue(node["instanceValue"], iv, data);
            return iv;
        } else {
            throw UmlParserException("Improper YAML node type, ", data.m_path.string(), node["instanceValue"]);
        }
    } else {
        throw UmlParserException("Unknown Value Specification, ", data.m_path.string(), node);
    }
    ValueSpecification& dumb = *data.m_manager->create<LiteralBool>();
    dumb.setID(ID::nullID());
    return dumb;
}

void emitProperty(YAML::Emitter& emitter, Property& prop, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::PROPERTY, "property", prop, data);

    emitTypedElement(emitter, prop, data);
    emitMultiplicityElement(emitter, prop, data);
    emitDeploymentTarget(emitter, prop, data);
    emitParameterableElement(emitter, prop, data);

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

    if (prop.getAssociation()) {
        emitter << YAML::Key << "association" << YAML::Value << prop.getAssociation().id().string();
    }

    emitElementDefenitionEnd(emitter, ElementType::PROPERTY, prop);
} 

void parseParameter(YAML::Node node, Parameter& el, ParserMetaData& data) {
    parseTypedElement(node, el, data);
    parseMultiplicityElement(node, el, data);

    if (node["direction"]) {
        if (node["direction"].IsScalar()) {
            if (node["direction"].as<string>().compare("IN") == 0) {
                el.setDirection(ParameterDirectionKind::IN_UML);
            } else if (node["direction"].as<string>().compare("INOUT") == 0) {
                el.setDirection(ParameterDirectionKind::INOUT);
            } else if (node["direction"].as<string>().compare("OUT") == 0) {
                el.setDirection(ParameterDirectionKind::OUT_UML);
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
            case ParameterDirectionKind::IN_UML : {
                direction = "IN";
                break;
            }
            case ParameterDirectionKind::INOUT : {
                direction = "INOUT";
                break;
            }
            case ParameterDirectionKind::OUT_UML : {
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
    parseBehavioralFeature(node, op, data);
    parseTemplateableElement(node, op, data);
}

void emitOperation(YAML::Emitter& emitter, Operation& op, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::OPERATION, "operation", op, data);
    emitBehavioralFeature(emitter, op, data);
    emitTemplateableElement(emitter, op, data);
    emitElementDefenitionEnd(emitter, ElementType::OPERATION, op);
}

PackageMerge& determineAndParsePackageMerge(YAML::Node node, ParserMetaData& data) {
    if (node["packageMerge"]) {
        if (node["packageMerge"].IsMap()) {
            PackageMerge& merge = *data.m_manager->create<PackageMerge>();
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
            ProfileApplication& profileApplication = *data.m_manager->create<ProfileApplication>();
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
            Abstraction& abstraction = *data.m_manager->create<Abstraction>();
            /** TODO: make own function and actually do opaqueExpressions**/
            parseDependency(node["abstraction"], abstraction, data);
            return abstraction;
        } else {
            throw UmlParserException("Invalid yaml node type for abstraction definition, must be a map!", data.m_path.string(), node["abstraction"]);
        }
    } else if (node["activity"]) {
        return parseDefinition<Activity>(node, data, "activity", parseActivity);
    } else if (node["artifact"]) {
        return parseDefinition<Artifact>(node, data, "artifact", parseArtifact);
    } else if (node["association"]) {
        return parseDefinition<Association>(node, data, "association", parseAssociation);
    } else if (node["class"]) {
        return parseDefinition<Class>(node, data, "class", parseClass);
    }  else if (node["dataType"]) {
        return parseDefinition<DataType>(node, data, "dataType", parseDataType);
    } else if (node["deployment"]) {
        return parseDefinition<Deployment>(node, data, "deployment", parseDeployment);
    } else if (node["dependency"]) {
        return parseDefinition<Dependency>(node, data, "dependency", parseDependency);
    } else if (node["enumeration"]) {
        return parseDefinition<Enumeration>(node, data, "enumeration", parseEnumeration);
    } else if (node["enumerationLiteral"]) {
        return parseDefinition<EnumerationLiteral>(node, data, "enumerationLiteral", parseEnumerationLiteral);
    } else if (node["expression"]) {
        return parseDefinition<Expression>(node, data, "expression", parseExpression);
    } else if (node["extension"]) {
        return parseDefinition<Extension>(node, data, "extension", parseExtension);
    } else if (node["generalizationSet"]) {
        return parseDefinition<GeneralizationSet>(node, data, "generalizationSet", parseGeneralizationSet);
    } else if (node["instanceSpecification"]) {
        return parseDefinition<InstanceSpecification>(node, data, "instanceSpecification", parseInstanceSpecification);
    } else if (node["instanceValue"]) {
        return parseDefinition<InstanceValue>(node, data, "instanceValue", parseInstanceValue);
    } else if (node["interface"]) {
        return parseDefinition<Interface>(node, data, "interface", parseInterface);
    } else if (node["literalBool"]) {
        return parseDefinition<LiteralBool>(node, data, "literalBool", parseLiteralBool);
    } else if (node["literalInt"]) {
        return parseDefinition<LiteralInt>(node, data, "literalInt", parseLiteralInt);
    } else if (node["literalNull"]) {
        return parseDefinition<LiteralNull>(node, data, "literalNull", parseTypedElement);
    } else if (node["literalReal"]) {
        return parseDefinition<LiteralReal>(node, data, "literalReal", parseLiteralReal);
    } else if (node["literalString"]) {
        return parseDefinition<LiteralString>(node, data, "literalString", parseLiteralString);
    } else if (node["literalUnlimitedNatural"]) {
        return parseDefinition<LiteralUnlimitedNatural>(node, data, "literalUnlimitedNatural", parseLiteralUnlimitedNatural);
    } else if (node["manifestation"]) {
        return parseDefinition<Manifestation>(node, data, "manifestation", parseManifestation);
    } else if (node["opaqueBehavior"]) {
        return parseDefinition<OpaqueBehavior>(node, data, "opaqueBehavior", parseOpaqueBehavior);
    } else if (node["package"]) {
        return parseDefinition<Package>(node, data, "package", parsePackage);
    } else if (node["primitiveType"]) {
        return parseDefinition<PrimitiveType>(node, data, "primitiveType", parseDataType);
    } else if (node["profile"]) {
        return parseDefinition<Profile>(node, data, "profile", parsePackage);
    } else if (node["realization"]) {
        return parseDefinition<Realization>(node, data, "realization", parseDependency);
    } else if (node["signal"]) {
        return parseDefinition<Signal>(node, data, "signal", parseSignal);
    } else if (node["usage"]) {
        return parseDefinition<Usage>(node, data, "usage", parseDependency);
    } else {
        throw UmlParserException("Invalid identifier for packagedElements, ", data.m_path.string(), node);
    }
}

Stereotype& determineAndParseStereotype(YAML::Node node, ParserMetaData& data) {
    if (node["stereotype"]) {
        if (node["stereotype"].IsMap()) {
            Stereotype& stereotype = *data.m_manager->create<Stereotype>();
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
    parseParameterableElement(node, pckg, data);
    parseSequenceDefinitions(node, data, "packageMerge", pckg, &Package::getPackageMerge, determineAndParsePackageMerge);
    parseSequenceDefinitions(node, data, "profileApplications", pckg, &Package::getProfileApplications, determineAndParseProfileApplication);
    parseSequenceDefinitions(node, data, "packagedElements", pckg, &Package::getPackagedElements, determineAndParsePackageableElement);
    parseSequenceDefinitions(node, data, "ownedStereotypes", pckg, &Package::getOwnedStereotypes, determineAndParseStereotype);
}

void emitPackage(YAML::Emitter& emitter, Package& pckg, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::PACKAGE, "package", pckg, data);
    emitNamedElement(emitter, pckg, data);
    emitTemplateableElement(emitter, pckg, data);
    emitParameterableElement(emitter, pckg, data);
    emitSequence(emitter, "packageMerge", data, pckg, &Package::getPackageMerge);
    emitSequence(emitter, "profileApplications", data, pckg, &Package::getProfileApplications);

    //special handling
    if (pckg.getPackagedElements().size() > pckg.getOwnedStereotypes().size()) {
        emitter << YAML::Key << "packagedElements" << YAML::Value << YAML::BeginSeq;
        if (data.m_strategy == EmitterStrategy::WHOLE) {
            for (auto& el : pckg.getPackagedElements()) {
                if (!el.isSubClassOf(ElementType::STEREOTYPE)) {
                    emit(emitter, el, data);
                }
            }
        } else {
            for (const ID id : pckg.getPackagedElements().ids()) {
                if (!pckg.getOwnedStereotypes().count(id)) {
                    emitter << YAML::Value << id.string() + ".yml";
                }
            }
        }
        emitter << YAML::EndSeq;
    }

    if (!pckg.getOwnedStereotypes().empty()) {
        emitter << YAML::Key << "ownedStereotypes" << YAML::Value << YAML::BeginSeq;
        if (data.m_strategy == EmitterStrategy::WHOLE) {
            for (auto& stereotype : pckg.getOwnedStereotypes()) {
                emit(emitter, stereotype, data);
            }
        } else {
            for (const ID id : pckg.getOwnedStereotypes().ids()) {
                emitter << YAML::Value << id.string() + ".yml";
            }
        }
        emitter << YAML::EndSeq;
    }
    // emitSequence(emitter, "packagedElements", data, pckg, &Package::getPackagedElements);

    emitElementDefenitionEnd(emitter, ElementType::PACKAGE, pckg);
}

void parseMultiplicityElement(YAML::Node node, MultiplicityElement& el, ParserMetaData& data) {
    if (node["lower"]) {
        if (node["lower"].IsScalar()) {
            LiteralInt& lower = *data.m_manager->create<LiteralInt>();
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
                LiteralUnlimitedNatural& upper = *data.m_manager->create<LiteralUnlimitedNatural>();
                upper.setInfinite();
                el.setUpperValue(&upper);
            } else {
                LiteralInt& upper = *data.m_manager->create<LiteralInt>();
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

Slot& determineAndParseSlot(YAML::Node node, ParserMetaData& data) {
    if (node["slot"]) {
        return parseDefinition<Slot>(node, data, "slot", parseSlot);
    } else {
        throw UmlParserException("Invalid element identifier for slot definition, it may only be a slot!", data.m_path.string(), node);
    }
}

void emitInstanceSpecification(YAML::Emitter& emitter, InstanceSpecification& inst, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::INSTANCE_SPECIFICATION, "instanceSpecification", inst, data);
    emitNamedElement(emitter, inst, data);
    emitDeploymentTarget(emitter, inst, data);
    emitParameterableElement(emitter, inst, data);
    if (!inst.getClassifiers().empty()) {
        emitter << YAML::Key << "classifiers" << YAML::Value << YAML::BeginSeq;
        for (const ID id : inst.getClassifiers().ids()) {
            emitter << YAML::Value << id.string();
        }
        emitter << YAML::EndSeq;
    }
    
    emitSequence(emitter, "slots", data, inst, &InstanceSpecification::getSlots);
    if (inst.getSpecification()) {
        emitter << YAML::Key << "specification" << YAML::Value;
        emit(emitter, *inst.getSpecification(), data);
    }
    emitElementDefenitionEnd(emitter, ElementType::INSTANCE_SPECIFICATION, inst);
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
        return parseDefinition<EnumerationLiteral>(node, data, "enumerationLiteral", parseEnumerationLiteral);
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

void emitInstanceValue(YAML::Emitter& emitter, InstanceValue& val, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::INSTANCE_VALUE, "instanceValue", val, data);
    emitTypedElement(emitter, val, data);
    if (val.getInstance()) {
        emitter << YAML::Key << "instance" << YAML::Value << val.getInstance()->getID().string();
    }
    emitElementDefenitionEnd(emitter, ElementType::INSTANCE_VALUE, val);
}

void emitPackageMerge(YAML::Emitter& emitter, PackageMerge& merge, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::PACKAGE_MERGE, "packageMerge", merge, data);

    emitElement(emitter, merge, data);

    if (merge.getMergedPackage()) {
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
    parseParameterableElement(node, lb, data);

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
    emitParameterableElement(emitter, lb, data);

    emitter << YAML::Key << "value" << YAML::Value << lb.getValue();

    emitElementDefenitionEnd(emitter, ElementType::LITERAL_BOOL, lb);
}

void parseLiteralInt(YAML::Node node, LiteralInt& li, ParserMetaData& data) {
    parseTypedElement(node, li, data);
    parseParameterableElement(node, li, data);

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
    emitParameterableElement(emitter, li, data);

    emitter << YAML::Key << "value" << YAML::Value << li.getValue();

    emitElementDefenitionEnd(emitter, ElementType::LITERAL_INT, li);
}

void parseLiteralReal(YAML::Node node, LiteralReal& lr, ParserMetaData& data) {
    parseTypedElement(node, lr, data);
    parseParameterableElement(node, lr, data);

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

TemplateBinding& determineAndParseTemplateBinding(YAML::Node node, ParserMetaData& data) {
    if (node["templateBinding"]) {
        return parseDefinition<TemplateBinding>(node, data, "templateBinding", parseTemplateBinding);
    } else {
        throw UmlParserException("Invalid element identifier, can only be a templateBinding!", data.m_path.string(), node);
    }
}

TemplateSignature& determineAndParseTemplateSignature(YAML::Node node, ParserMetaData& data) {
    if (node["templateSignature"]) {
        return parseDefinition<TemplateSignature>(node, data, "templateSignature", parseTemplateSignature);
    } else {
        throw UmlParserException("Invalid element identifier, can only be a templateSignature!", data.m_path.string(), node);
    }
}

void emitTemplateableElement(YAML::Emitter& emitter, TemplateableElement& el, EmitterMetaData& data) {
    if (el.getOwnedTemplateSignature()) {
        emitter << YAML::Key << "templateSignature" << YAML::Value;
        if (data.m_strategy == EmitterStrategy::WHOLE) {
            emit(emitter, *el.getOwnedTemplateSignature(), data);
        } else {
            emitter << el.getOwnedTemplateSignature().id().string() + ".yml";
        }
    }
    emitSequence(emitter, "templateBindings", data, el, &TemplateableElement::getTemplateBindings);
}

TemplateParameter& determineAndParseTemplateParameter(YAML::Node node, ParserMetaData& data) {
    if (node["templateParameter"]) {
        return parseDefinition<TemplateParameter>(node, data, "templateParameter", parseTemplateParameter);
    } else {
        throw UmlParserException("Invalid element identifier for templateParameter definition, it can only be a templateParameter!", data.m_path.string(), node);
    }
}

void parseTemplateSignature(YAML::Node node, TemplateSignature& signature, ParserMetaData& data) {
    parseElement(node, signature, data);
    parseSequenceDefinitions(node, data, "ownedParameters", signature, &TemplateSignature::getOwnedParameters, determineAndParseTemplateParameter);
    parseSetReferences<TemplateParameter, TemplateSignature>(node, data, "parameters", signature, &TemplateSignature::getParameters);
}

void emitTemplateSignature(YAML::Emitter& emitter, TemplateSignature& signature, EmitterMetaData& data) {

    emitElementDefenition(emitter, ElementType::TEMPLATE_SIGNATURE, "templateSignature", signature, data);

    emitElement(emitter, signature, data);
    emitSequence(emitter, "ownedParameters", data, signature, &TemplateSignature::getOwnedParameters);
    // special handling
    if (signature.getParameters().size() > signature.getOwnedParameters().size()) {
        emitter << YAML::Key << "parameters" << YAML::Value << YAML::BeginSeq;
        for (auto& param: signature.getParameters()) {
            if (!signature.getOwnedParameters().count(param.getID())) {
                emitter << param.getID().string();
            }
        }
        emitter << YAML::EndSeq;
    }

    emitElementDefenitionEnd(emitter, ElementType::TEMPLATE_SIGNATURE, signature);
}

ParameterableElement& determinAndParseParameterableElement(YAML::Node node, ParserMetaData& data) {
    if (node["activity"]) {
        // TODO
    } else if (node["association"]) {
        // TODO
    } else if (node["class"]) {
        if (node["class"].IsMap()) {
            Class& clazz = *data.m_manager->create<Class>();
            parseClass(node["class"], clazz, data);
            return clazz;
        } else {
            throw UmlParserException("class definition must be of node type Map! ", data.m_path.string(), node["class"]);
        }
    } else if (node["dataType"]) {
        if (node["dataType"].IsMap()) {
            DataType& dataType = *data.m_manager->create<DataType>();
            parseDataType(node["dataType"], dataType, data);
            return dataType;
        } else {
            throw UmlParserException("dataType definition must be of node type Map! ", data.m_path.string(), node["dataType"]);
        }
    } else if (node["enumeration"]) {
        if (node["enumeration"].IsMap()) {
            Enumeration& enumeration = *data.m_manager->create<Enumeration>();
            parseEnumeration(node["enumeration"], enumeration, data);
            return enumeration;
        } else {
            throw UmlParserException("enumeration definition must be of node type Map! ", data.m_path.string(), node["enumeration"]);
        }
    } else if (node["enumerationLiteral"]) {
        if (node["enumerationLiteral"].IsMap()) {
            EnumerationLiteral& literal = *data.m_manager->create<EnumerationLiteral>();
            parseEnumerationLiteral(node["enumerationLiteral"], literal, data);
            return literal;
        } else {
            throw UmlParserException("enumerationLiteral definition must be of node type Map! ", data.m_path.string(), node["enumerationLiteral"]);
        }
    } else if (node["instanceSpecification"]) {
        if (node["instanceSpecification"].IsMap()) {
            InstanceSpecification& inst = *data.m_manager->create<InstanceSpecification>();
            parseInstanceSpecification(node["instanceSpecification"], inst, data);
            return inst;
        } else {
            throw UmlParserException("instanceSpecification definition must be of node type Map! ", data.m_path.string(), node["instanceSpecification"]);
        }
    } else if (node["instanceValue"]) {
        if (node["instanceValue"].IsMap()) {
            InstanceValue& inst = *data.m_manager->create<InstanceValue>();
            parseInstanceValue(node["instanceValue"], inst, data);
            return inst;
        } else {
            throw UmlParserException("instanceValue definition must be of node type Map! ", data.m_path.string(), node["instanceValue"]);
        }
    } else if (node["literalBool"]) {
        if (node["literalBool"].IsMap()) {
            LiteralBool& lb = *data.m_manager->create<LiteralBool>();
            parseLiteralBool(node["literalBool"], lb, data);
            return lb;
        } else {
            throw UmlParserException("literalBool definition must be of node type Map! ", data.m_path.string(), node["literalBool"]);
        }
    } else if (node["literalInt"]) {
        if (node["literalInt"].IsMap()) {
            LiteralInt& li = *data.m_manager->create<LiteralInt>();
            parseLiteralInt(node["literalInt"], li, data);
            return li;
        } else {
            throw UmlParserException("literalInt definition must be of node type Map! ", data.m_path.string(), node["literalInt"]);
        }
    } else if (node["literalNull"]) {
        if (node["literalNull"].IsMap()) {
            LiteralNull& ln = *data.m_manager->create<LiteralNull>();
            parseTypedElement(node["literalNull"], ln, data);
            return ln;
        } else {
            throw UmlParserException("literalNull definition must be of node type Map! ", data.m_path.string(), node["literalNull"]);
        }
    } else if (node["literalString"]) {
        if (node["literalString"].IsMap()) {
            LiteralString& ls = *data.m_manager->create<LiteralString>();
            parseLiteralString(node["literalString"], ls, data);
            return ls;
        } else {
            throw UmlParserException("literalString definition must be of node type Map! ", data.m_path.string(), node["literalString"]);
        }
    } else if (node["literalUnlimitedNatural"]) {
        if (node["literalUnlimitedNatural"].IsMap()) {
            LiteralUnlimitedNatural& lu = *data.m_manager->create<LiteralUnlimitedNatural>();
            parseLiteralUnlimitedNatural(node["literalUnlimitedNatural"], lu, data);
            return lu;
        } else {
            throw UmlParserException("literalUnlimitedNatural definition must be of node type Map! ", data.m_path.string(), node["literalUnlimitedNatural"]);
        }
    } else if (node["model"]) {
        if (node["model"].IsMap()) {
            Model& m = *data.m_manager->create<Model>();
            parsePackage(node["model"], m, data);
            return m;
        } else {
            throw UmlParserException("model definition must be of node type Map! ", data.m_path.string(), node["model"]);
        }
    } else if (node["opaqueBehavior"]) {
        if (node["opaqueBehavior"].IsMap()) {
            OpaqueBehavior& ob = *data.m_manager->create<OpaqueBehavior>();
            parseOpaqueBehavior(node["opaqueBehavior"], ob, data);
            return ob;
        } else {
            throw UmlParserException("opaqueBehavior definition must be of node type Map! ", data.m_path.string(), node["opaqueBehavior"]);
        }
    } else if (node["operation"]) {
        if (node["operation"].IsMap()) {
            Operation& op = *data.m_manager->create<Operation>();
            parseOperation(node["operation"], op, data);
            return op;
        } else {
            throw UmlParserException("operation definition must be of node type Map! ", data.m_path.string(), node["operation"]);
        }
    } else if (node["package"]) {
        if (node["package"].IsMap()) {
            Package& pckg = *data.m_manager->create<Package>();
            parsePackage(node["package"], pckg, data);
            return pckg;
        } else {
            throw UmlParserException("package definition must be of node type Map! ", data.m_path.string(), node["package"]);
        }
    } else if (node["parameter"]) {
        if (node["parameter"].IsMap()) {
            Parameter& param = *data.m_manager->create<Parameter>();
            parseParameter(node["parameter"], param, data);
            return param;
        } else {
            throw UmlParserException("parameter definition must be of node type Map! ", data.m_path.string(), node["parameter"]);
        }
    } else if (node["primitiveType"]) {
        if (node["primitiveType"].IsMap()) {
            PrimitiveType& prim = *data.m_manager->create<PrimitiveType>();
            parsePrimitiveType(node["primitiveType"], prim, data);
            return prim;
        } else {
            throw UmlParserException("primitiveType definition must be of node type Map! ", data.m_path.string(), node["primitiveType"]);
        }
    } else if (node["property"]) {
        if (node["property"].IsMap()) {
            Property& prop = *data.m_manager->create<Property>();
            parseProperty(node["property"], prop, data);
            return prop;
        } else {
            throw UmlParserException("property definition must be of node type Map! ", data.m_path.string(), node["property"]);
        }
    } else {
        throw UmlParserException("not a parametered element! ", data.m_path.string(), node);
    }

    ParameterableElement& dumb = *data.m_manager->create<ParameterableElement>();
    dumb.setID(ID::nullID());
    return dumb;
}

void emitTemplateParameter(YAML::Emitter& emitter, TemplateParameter& parameter, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::TEMPLATE_PARAMETER, "templateParameter", parameter, data);

    emitElement(emitter, parameter, data);

    if (parameter.getOwnedDefault()) {
        emitter << YAML::Key << "ownedDefault" << YAML::Value;
        if (data.m_strategy == EmitterStrategy::WHOLE) {
            emit(emitter, *parameter.getOwnedDefault(), data);
        } else {
            emitter << parameter.getOwnedDefault().id().string();
        }
    }

    if (parameter.getDefault() && !parameter.getOwnedDefault()) {
        emitter << YAML::Key << "default" << YAML::Value << parameter.getDefault().id().string();
    }

    if (parameter.getOwnedParameteredElement()) {
        emitter << YAML::Key << "ownedParameteredElement" << YAML::Value;
        if (data.m_strategy == EmitterStrategy::WHOLE) {
            emit(emitter, parameter, data);
        } else {
            emitter << parameter.getOwnedParameteredElement().id().string() + ".yml";
        }
    }

    if (parameter.getParameteredElement() && !parameter.getOwnedParameteredElement()) {
        emitter << YAML::Key << "parameteredElement" << YAML::Value << parameter.getParameteredElement().id().string();
    }

    emitElementDefenitionEnd(emitter, ElementType::TEMPLATE_PARAMETER, parameter);
}

TemplateParameterSubstitution& determineAndParseTemplateParameterSubstitution(YAML::Node node, ParserMetaData& data) {
    if (node["templateParameterSubstitution"]) {
        return parseDefinition<TemplateParameterSubstitution>(node, data, "templateParameterSubstitution", parseTemplateParameterSubstitution);
    } else {
        throw UmlParserException("Invalid element identifier for templateParameterSubstitution definition, it can only be a templateParameterSubstitution!", data.m_path.string(), node);
    }
}

void emitTemplateBinding(YAML::Emitter& emitter, TemplateBinding& binding, EmitterMetaData& data) {
    // TODO change
    if (binding.getElementType() == ElementType::TEMPLATE_BINDING) {
        emitter << /*YAML::BeginMap << */YAML::Key << "templateBinding" << YAML::Value << YAML::BeginMap;
    }

    emitElement(emitter, binding, data);
    if (binding.getSignature()) {
        emitter << YAML::Key << "signature" << YAML::Value << binding.getSignature()->getID().string();
    }
    emitSequence(emitter, "parameterSubstitution", data, binding, &TemplateBinding::getParameterSubstitution);

    if (binding.getElementType() == ElementType::TEMPLATE_BINDING) {
        emitter << YAML::EndMap;// << YAML::EndMap;
    }
}

void emitTemplateParameterSubstitution(YAML::Emitter& emitter, TemplateParameterSubstitution& sub, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::TEMPLATE_PARAMETER_SUBSTITUTION, "templateParameterSubstitution", sub, data);

    emitElement(emitter, sub, data);

    if (sub.getFormal()) {
        emitter << YAML::Key << "formal" << YAML::Value << sub.getFormal().id().string();
    }

    if (sub.getOwnedActual()) {
        emitter << YAML::Key << "ownedActual" << YAML::Value;
        if (data.m_strategy == EmitterStrategy::WHOLE) {
            emit(emitter, *sub.getOwnedActual(), data);
        } else {
            emitter << sub.getOwnedActual().id().string();
        }
    }

    if (sub.getActual() && !sub.getOwnedActual()) {
        emitter << YAML::Key << "actual" << YAML::Value << sub.getActual().id().string();
    }

    emitElementDefenitionEnd(emitter, ElementType::TEMPLATE_PARAMETER_SUBSTITUTION, sub);
}

void parseAssociation(YAML::Node node, Association& association, ParserMetaData& data) {
    parseClassifier(node, association, data);
    parseSequenceDefinitions(node, data, "navigableOwnedEnds", association, &Association::getNavigableOwnedEnds, determineAndParseOwnedAttribute);
    parseSequenceDefinitions(node, data, "ownedEnds", association, &Association::getOwnedEnds, determineAndParseOwnedAttribute);
    parseSetReferences<Property, Association>(node, data, "memberEnds", association, &Association::getMemberEnds);
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

ExtensionEnd& determineAndParseOwnedEnd(YAML::Node node, ParserMetaData& data) {
    if (node["extensionEnd"]) {
        ExtensionEnd& extensionEnd = *data.m_manager->create<ExtensionEnd>();
        parseProperty(node["extensionEnd"], extensionEnd, data);
        return extensionEnd;
    } else {
        throw UmlParserException("Invalide uml element identifier, can only be an extensionEnd!", data.m_path.string(), node);
    }
}

void emitExtension(YAML::Emitter& emitter, Extension& extension, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::EXTENSION, "extension", extension, data);

    emitClassifier(emitter, extension, data);

    emitter << YAML::Key << "metaClass" << YAML::Value << Element::elementTypeToString(extension.getMetaClass());

    if (extension.getOwnedEnd()) {
        emitter << YAML::Key << "ownedEnd" << YAML::Value;
        if (data.m_strategy == EmitterStrategy::WHOLE) {
            emit(emitter, *extension.getOwnedEnd(), data);
        } else {
            emitter << extension.getOwnedEnd().id().string();
        }
    }

    emitElementDefenitionEnd(emitter, ElementType::EXTENSION, extension);
}

void emitProfileApplication(YAML::Emitter& emitter, ProfileApplication& application, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::PROFILE_APPLICATION, "profileApplication", application, data);

    emitElement(emitter, application, data);

    if (application.getAppliedProfile()) {
        filesystem::path path = data.getPath(application.getAppliedProfile().id());
        if (path.empty() || path == data.m_path / data.m_fileName) {
            emitter << YAML::Key << "appliedProfile" << YAML::Value << application.getAppliedProfile().id().string();
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

void parseDependency(YAML::Node node, Dependency& dependency, ParserMetaData& data) {
    parseNamedElement(node, dependency, data);
    parseSetReferences<NamedElement, Dependency>(node, data, "client", dependency, &Dependency::getClient);
    parseSetReferences<NamedElement, Dependency>(node, data, "supplier", dependency, &Dependency::getSupplier);
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

void parseDeployment(YAML::Node node, Deployment& deployment, ParserMetaData& data) {
    parseNamedElement(node, deployment, data);
    parseSetReferences<DeployedArtifact, Deployment>(node, data, "deployedArtifacts", deployment, &Deployment::getDeployedArtifacts);
}

void emitDeployment(YAML::Emitter& emitter, Deployment& deployment, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::DEPLOYMENT, "deployment", deployment, data);

    emitNamedElement(emitter, deployment, data);

    if (!deployment.getDeployedArtifacts().empty()) {
        emitter << YAML::Key << "deployedArtifacts" << YAML::Value << YAML::BeginSeq;
        for (const ID id : deployment.getDeployedArtifacts().ids()) {
            emitter << id.string();
        }
        emitter << YAML::EndSeq;
    }

    emitElementDefenitionEnd(emitter, ElementType::DEPLOYMENT, deployment);
}

Artifact& determineAndParseArtifact(YAML::Node node, ParserMetaData& data) {
    if (node["artifact"]) {
        return parseDefinition<Artifact>(node, data, "artifact", parseArtifact);
    } else {
        throw UmlParserException("Invalid element identifier for artifact definition, it can only be an artifact!", data.m_path.string(), node);
    }
}

Manifestation& determineAndParseManifestation(YAML::Node node, ParserMetaData& data) {
    if (node["manifestation"]) {
        return parseDefinition<Manifestation>(node, data, "manifestation", parseManifestation);
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
        return parseDefinition<Deployment>(node, data, "deployment", parseDeployment);
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
    if (node["activity"]) {
        return parseDefinition<Activity>(node, data, "activity", parseActivity);
    } else if (node["opaqueBehavior"]) {
        return parseDefinition<OpaqueBehavior>(node, data, "opaqueBehavior", parseOpaqueBehavior);
    } else {
        throw UmlParserException("Invalid element identifier for opaqueBehavior definition, must be an opaqueBehavior", data.m_path.string(), node); // TODO expand
    }
}

void emitBehavioredClassifier(YAML::Emitter& emitter, BehavioredClassifier& classifier, EmitterMetaData& data) {
    emitSequence(emitter, "ownedBehaviors", data, classifier, &BehavioredClassifier::getOwnedBehaviors);
    if (classifier.getClassifierBehavior()) {
        emitter << YAML::Key << "classifierBehavior" << YAML::Value << classifier.getClassifierBehavior().id().string();
    }
    emitSequence(emitter, "interfaceRealizations", data, classifier, &BehavioredClassifier::getInterfaceRealizations);
}

void emitManifestation(YAML::Emitter& emitter, Manifestation& manifestation, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::MANIFESTATION, "manifestation", manifestation, data);

    emitNamedElement(emitter, manifestation, data);

    if (manifestation.getUtilizedElement()) {
        emitter << YAML::Key << "utilizedElement" << YAML::Value << manifestation.getUtilizedElement()->getID().string();
    }

    emitElementDefenitionEnd(emitter, ElementType::MANIFESTATION, manifestation);
}

void parseParameterableElement(YAML::Node node, ParameterableElement& el, ParserMetaData& data) {
    parseSingletonReference(node, data, "templateParameter", el, &ParameterableElement::setTemplateParameter, &ParameterableElement::setTemplateParameter);
}

void emitParameterableElement(YAML::Emitter& emitter, ParameterableElement& el, EmitterMetaData& data) {
    if (el.getTemplateParameter()) {
        emitter << YAML::Key << "templateParameter" << YAML::Value << el.getTemplateParameter().id().string();
    }
}

void emitGeneralizationSet(YAML::Emitter& emitter, GeneralizationSet& generalizationSet, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::GENERALIZATION_SET, "generalizationSet", generalizationSet, data);
    emitNamedElement(emitter, generalizationSet, data);
    emitParameterableElement(emitter, generalizationSet, data);
    emitter << YAML::Key << "covering" << YAML::Value << generalizationSet.isCovering();
    emitter << YAML::Key << "disjoint" << YAML::Value << generalizationSet.isDisjoint();
    if (generalizationSet.getPowerType()) {
        emitter << YAML::Key << "powerType" << YAML::Value << generalizationSet.getPowerType().id().string();
    }
    if (!generalizationSet.getGeneralizations().empty()) {
        emitter << YAML::Key << "generalizations" << YAML::Value << YAML::BeginSeq;
        for (const ID id : generalizationSet.getGeneralizations().ids()) {
            emitter << YAML::Value << id.string();
        }
        emitter << YAML::EndSeq;
    }
    emitElementDefenitionEnd(emitter, ElementType::GENERALIZATION_SET, generalizationSet);
}

void parseTypedElement(YAML::Node node, TypedElement& el, ParserMetaData& data) {
    parseNamedElement(node, el, data);
    parseSingletonReference(node, data, "type", el, &TypedElement::setType, &TypedElement::setType);
}

void parseClassifier(YAML::Node node, Classifier& clazz, ParserMetaData& data) {
    parseNamedElement(node, clazz, data);
    parseTemplateableElement(node, clazz, data);
    parseParameterableElement(node, clazz, data);
    parseSequenceDefinitions(node, data, "generalizations", clazz, &Classifier::getGeneralizations, determineAndParseGeneralization);
    parseSetReferences<GeneralizationSet, Classifier>(node, data, "powerTypeExtent", clazz, &Classifier::getPowerTypeExtent);
}

void parseGeneralization(YAML::Node node, Generalization& general, ParserMetaData& data) {
    parseElement(node, general, data);
    parseSingletonReference(node, data, "general", general, &Generalization::setGeneral, &Generalization::setGeneral);
    parseSetReferences<GeneralizationSet, Generalization>(node, data, "generalizationSets", general, &Generalization::getGeneralizationSets);
}

void parsePackageMerge(YAML::Node node, PackageMerge& merge, ParserMetaData& data) {
    parseElement(node, merge, data);
    parseSingletonReference(node, data, "mergedPackage", merge, &PackageMerge::setMergedPackage, &PackageMerge::setMergedPackage);
}

void parseSlot(YAML::Node node, Slot& slot, ParserMetaData& data) {
    parseElement(node, slot, data);
    parseSingletonReference(node, data, "definingFeature", slot, &Slot::setDefiningFeature, &Slot::setDefiningFeature);
    parseSequenceDefinitions(node, data, "values", slot, &Slot::getValues, &determineAndParseValueSpecification);
}

void parseInstanceValue(YAML::Node node, InstanceValue& val, ParserMetaData& data) {
    parseTypedElement(node, val, data);
    parseSingletonReference(node, data, "instance", val, &InstanceValue::setInstance, &InstanceValue::setInstance);
}

void parseInstanceSpecification(YAML::Node node, InstanceSpecification& inst, ParserMetaData& data) {
    parseNamedElement(node, inst, data);
    parseDeploymentTarget(node, inst, data);
    parseParameterableElement(node, inst, data);
    parseSetReferences<Classifier, InstanceSpecification>(node, data, "classifiers", inst, &InstanceSpecification::getClassifiers);
    parseSequenceDefinitions(node, data, "slots", inst, &InstanceSpecification::getSlots, determineAndParseSlot);
    parseSingletonDefinition<ValueSpecification, InstanceSpecification>(node, data, "specification", inst, determineAndParseValueSpecification, &InstanceSpecification::setSpecification, &InstanceSpecification::setSpecification);
}

void parseProperty(YAML::Node node, Property& prop, ParserMetaData& data) {
    parseTypedElement(node, prop, data);
    parseMultiplicityElement(node, prop, data);
    parseDeploymentTarget(node, prop, data);
    parseParameterableElement(node, prop, data);

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

    parseSingletonDefinition(node, data, "defaultValue", prop, determineAndParseValueSpecification, &Property::setDefaultValue, &Property::setDefaultValue);
    parseSetReferences<Property, Property>(node, data, "redefinedProperties", prop, &Property::getRedefinedProperties);
    parseSingletonReference(node, data, "association", prop, &Property::setAssociation, &Property::setAssociation);
}

void parseTemplateableElement(YAML::Node node, TemplateableElement& el, ParserMetaData& data) {
    parseSingletonDefinition(node, data, "templateSignature", el, determineAndParseTemplateSignature, &TemplateableElement::setOwnedTemplateSignature, &TemplateableElement::setOwnedTemplateSignature);
    parseSequenceDefinitions(node, data, "templateBindings", el, &TemplateableElement::getTemplateBindings, determineAndParseTemplateBinding);
}

void parseTemplateParameter(YAML::Node node, TemplateParameter& parameter, ParserMetaData& data) {
    parseElement(node, parameter, data);
    parseSingletonDefinition(node, data, "ownedDefault", parameter, determinAndParseParameterableElement, &TemplateParameter::setOwnedDefault, &TemplateParameter::setOwnedDefault);
    parseSingletonReference(node, data, "default", parameter, &TemplateParameter::setDefault, &TemplateParameter::setDefault);
    parseSingletonDefinition(node, data, "ownedParameteredElement", parameter, determinAndParseParameterableElement, &TemplateParameter::setOwnedParameteredElement, &TemplateParameter::setOwnedParameteredElement);
    parseSingletonReference(node, data, "parameteredElement", parameter, &TemplateParameter::setParameteredElement, &TemplateParameter::setParameteredElement);
}

void parseTemplateBinding(YAML::Node node, TemplateBinding& binding, ParserMetaData& data) {
    parseElement(node, binding, data);
    parseSingletonReference(node, data, "signature", binding, &TemplateBinding::setSignature, &TemplateBinding::setSignature);
    parseSequenceDefinitions(node, data, "parameterSubstitution", binding, &TemplateBinding::getParameterSubstitution, determineAndParseTemplateParameterSubstitution);
}

void parseTemplateParameterSubstitution(YAML::Node node, TemplateParameterSubstitution& sub, ParserMetaData& data) {
    parseElement(node, sub, data);
    parseSingletonReference(node, data, "formal", sub, &TemplateParameterSubstitution::setFormal, &TemplateParameterSubstitution::setFormal);
    parseSingletonDefinition(node, data, "ownedActual", sub, determinAndParseParameterableElement, &TemplateParameterSubstitution::setOwnedActual, &TemplateParameterSubstitution::setOwnedActual);
    parseSingletonReference(node, data, "actual", sub, &TemplateParameterSubstitution::setActual, &TemplateParameterSubstitution::setActual);
}

void parseExtension(YAML::Node node, Extension& extension, ParserMetaData& data) {
    parseClassifier(node, extension, data);
    parseSingletonDefinition(node, data, "ownedEnd", extension, determineAndParseOwnedEnd, &Extension::setOwnedEnd, &Extension::setOwnedEnd);
    if (node["metaClass"]) {
        if (node["metaClass"].IsScalar()) {
            extension.setMetaClass(elementTypeFromString(node["metaClass"].as<string>()));
        } else {
            throw UmlParserException("Invalid yaml node type for extension MetaClass, must be scalar!", data.m_path.string(), node["metaClass"]);
        }
    }
}

void parseProfileApplication(YAML::Node node, ProfileApplication& application, ParserMetaData& data) {
    parseElement(node, application, data);
    parseSingletonReference(node, data, "appliedProfile", application, &ProfileApplication::setAppliedProfile, &ProfileApplication::setAppliedProfile);
}

void parseManifestation(YAML::Node node, Manifestation& manifestation, ParserMetaData& data) {
    parseNamedElement(node, manifestation, data);
    parseSingletonReference(node, data, "utilizedElement", manifestation, &Manifestation::setUtilizedElement, &Manifestation::setUtilizedElement);
}

InterfaceRealization& determineAndParseInterfaceRealization(YAML::Node node, ParserMetaData& data) {
    if (node["interfaceRealization"]) {
        return parseDefinition<InterfaceRealization>(node, data, "interfaceRealization", parseInterfaceRealization);
    } else {
        throw UmlParserException("Invalid key for interfaceRealization", data.m_path.string(), node);
    }
}

void parseBehavioredClassifier(YAML::Node node, BehavioredClassifier& classifier, ParserMetaData& data) {
    parseSequenceDefinitions(node, data, "ownedBehaviors", classifier, &BehavioredClassifier::getOwnedBehaviors, determineAndParseBehavior);
    parseSingletonReference(node, data, "classifierBehavior", classifier, &BehavioredClassifier::setClassifierBehavior, &BehavioredClassifier::setClassifierBehavior);
    parseSequenceDefinitions(node, data, "interfaceRealizations", classifier, &BehavioredClassifier::getInterfaceRealizations, determineAndParseInterfaceRealization);
}

void parseGeneralizationSet(YAML::Node node, GeneralizationSet& generalizationSet, ParserMetaData& data) {
    parseNamedElement(node, generalizationSet, data);
    parseParameterableElement(node, generalizationSet, data);

    if (node["covering"]) {
        if (node["covering"].IsScalar()) {
            generalizationSet.setCovering(node["covering"].as<bool>());
        } else {
            throw UmlParserException("Invalid yaml node type for covering field", data.m_path.string(), node["covering"]);
        }
    }

    if (node["disjoint"]) {
        if (node["disjoint"].IsScalar()) {
            generalizationSet.setDisjoint(node["disjoint"].as<bool>());
        } else {
            throw UmlParserException("Invalid yaml node type for disjoint field", data.m_path.string(), node["disjoint"]);
        }
    }

    parseSingletonReference(node, data, "powerType", generalizationSet, &GeneralizationSet::setPowerType, &GeneralizationSet::setPowerType);
    parseSetReferences<Generalization, GeneralizationSet>(node, data, "generalizations", generalizationSet, &GeneralizationSet::getGeneralizations);
}

ConnectorEnd& determineAndParseConnectorEnd(YAML::Node node, ParserMetaData& data) {
    if (node["connectorEnd"]) {
        return parseDefinition<ConnectorEnd>(node, data, "connectorEnd", parseConnectorEnd);
    } else {
        throw UmlParserException("Invalide definition for connectorEnd!", data.m_path.string(), node);
    }
}

void parseConnector(YAML::Node node, Connector& connector, ParserMetaData& data) {
    parseNamedElement(node, connector, data);
    parseSequenceDefinitions(node, data, "ends", connector, &Connector::getEnds, &determineAndParseConnectorEnd);
    parseSingletonReference(node, data, "type", connector, &Connector::setType, &Connector::setType);
    parseSetReferences<Behavior, Connector>(node, data, "contracts", connector, &Connector::getContracts);
    // todo kind
}

void emitConnector(YAML::Emitter& emitter, Connector& connector, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::CONNECTOR, "connector", connector, data);
    emitNamedElement(emitter, connector, data);
    emitSequence(emitter, "ends", data, connector, &Connector::getEnds);
    if (connector.getType()) {
        emitter << YAML::Key << "type" << YAML::Value << connector.getType().id().string();
    }
    if (!connector.getContracts().empty()) {
        emitter << YAML::Key << "contracts" << YAML::Value << YAML::BeginSeq;
        for (const ID id : connector.getContracts().ids()) {
            emitter << YAML::Value << id.string();
        }
        emitter << YAML::EndSeq;
    }
    emitElementDefenitionEnd(emitter, ElementType::CONNECTOR, connector);
}

void parseConnectorEnd(YAML::Node node, ConnectorEnd& end, ParserMetaData& data) {
    parseElement(node, end, data);
    parseMultiplicityElement(node, end, data);
    parseSingletonReference(node, data, "role", end, &ConnectorEnd::setRole, &ConnectorEnd::setRole);
}

void emitConnectorEnd(YAML::Emitter& emitter, ConnectorEnd& end, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::CONNECTOR_END, "connectorEnd", end, data);
    emitElement(emitter, end, data);
    emitMultiplicityElement(emitter, end, data);
    if (end.getRole()) {
        emitter << YAML::Key << "role" << YAML::Value << end.getRole().id().string();
    }
    emitElementDefenitionEnd(emitter, ElementType::CONNECTOR_END, end);
}

void parsePort(YAML::Node node, Port& port, ParserMetaData& data) {
    parseProperty(node, port, data);
    if (node["isBehavior"]) {
        if (node["isBehavior"].IsScalar()) {
            port.setIsBehavior(node["isBehavior"].as<bool>());
        } else {
            throw UmlParserException("Port field isBehavior must be a boolean scalar!", data.m_path.string(), node["isBehavior"]);
        }
    }
    if (node["isConjugated"]) {
        if (node["isConjugated"].IsScalar()) {
            port.setIsConjugated(node["isConjugated"].as<bool>());
        } else {
            throw UmlParserException("Port field isConjugated must be a boolean scalar!", data.m_path.string(), node["isConjugated"]);
        }
    }
    if (node["isService"]) {
        if (node["isService"].IsScalar()) {
            port.setIsService(node["isService"].as<bool>());
        } else {
            throw UmlParserException("Port field isService must be a boolean scalar!", data.m_path.string(), node["isService"]);
        }
    }
}

void emitPort(YAML::Emitter& emitter, Port& port, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::PORT, "port", port, data);
    emitProperty(emitter, port, data);
    if (port.isBehavior()) {
        emitter << YAML::Key << "isBehavior" << YAML::Value << true;
    }
    if (port.isConjugated()) {
        emitter << YAML::Key << "isConjugated" << YAML::Value << true;
    }
    if (!port.isService()) {
        emitter << YAML::Key << "isService" << YAML::Value << false;
    }
    emitElementDefenitionEnd(emitter, ElementType::PORT, port);
}

void parseInterface(YAML::Node node, Interface& interface_uml, ParserMetaData& data) {
    parseClassifier(node, interface_uml, data);
    parseSequenceDefinitions(node, data, "ownedAttributes", interface_uml, &Interface::getOwnedAttributes, determineAndParseOwnedAttribute);
    parseSequenceDefinitions(node, data, "ownedOperations", interface_uml, &Interface::getOwnedOperations, determineAndParseOwnedOperation);
    parseSequenceDefinitions(node, data, "nestedClassifiers", interface_uml, &Interface::getNestedClassifiers, determineAndParseClassifier);
}

void emitInterface(YAML::Emitter& emitter, Interface& interface_uml, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::INTERFACE_UML, "interface", interface_uml, data);
    emitClassifier(emitter, interface_uml, data);
    emitSequence(emitter, "ownedAttributes", data, interface_uml, &Interface::getOwnedAttributes);
    emitSequence(emitter, "ownedOperations", data, interface_uml, &Interface::getOwnedOperations);
    emitSequence(emitter, "nestedClassifiers", data, interface_uml, &Interface::getNestedClassifiers);
    emitElementDefenitionEnd(emitter, ElementType::INTERFACE_UML, interface_uml);
}

void parseInterfaceRealization(YAML::Node node, InterfaceRealization& realization, ParserMetaData& data) {
    parseNamedElement(node, realization, data);
    parseParameterableElement(node, realization, data);
    parseSingletonReference(node, data, "contract", realization, &InterfaceRealization::setContract, &InterfaceRealization::setContract);
}

void emitInterfaceRealization(YAML::Emitter& emitter, InterfaceRealization& realization, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::INTERFACE_REALIZATION, "interfaceRealization", realization, data);
    emitNamedElement(emitter, realization, data);
    if (realization.getContract()) {
        emitter << YAML::Key << "contract" << YAML::Value << realization.getContract().id().string();
    }
    emitElementDefenitionEnd(emitter, ElementType::INTERFACE_REALIZATION, realization);
}

void parseSignal(YAML::Node node, Signal& signal, ParserMetaData& data) {
    parseClassifier(node, signal, data);
    parseSequenceDefinitions(node, data, "ownedAttributes", signal, &Signal::getOwnedAttributes, determineAndParseOwnedAttribute);
}

void emitSignal(YAML::Emitter& emitter, Signal& signal, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::SIGNAL, "signal", signal, data);
    emitClassifier(emitter, signal, data);
    emitSequence(emitter, "ownedAttributes", data, signal, &Signal::getOwnedAttributes);
    emitElementDefenitionEnd(emitter, ElementType::SIGNAL, signal);
}

void parseBehavioralFeature(YAML::Node node, BehavioralFeature& feature, ParserMetaData& data) {
    parseNamedElement(node, feature, data);
    parseSetReferences<Behavior, BehavioralFeature>(node, data, "methods", feature, &BehavioralFeature::getMethods);
    parseSequenceDefinitions(node, data, "ownedParameters", feature, &BehavioralFeature::getOwnedParameters, determineAndParseParameter);
}

void emitBehavioralFeature(YAML::Emitter& emitter, BehavioralFeature& feature, EmitterMetaData& data) {
    emitNamedElement(emitter, feature, data);
    if (!feature.getMethods().empty()) {
        emitter << YAML::Key << "methods" << YAML::Value << YAML::BeginSeq;
        for (const ID id : feature.getMethods().ids()) {
            emitter << YAML::Value << id.string();
        }
        emitter << YAML::EndSeq;
    }
    emitSequence(emitter, "ownedParameters", data, feature, &BehavioralFeature::getOwnedParameters);
}

void parseReception(YAML::Node node, Reception& reception, ParserMetaData& data) {
    parseBehavioralFeature(node, reception, data);
    parseSingletonReference(node, data, "signal", reception, &Reception::setSignal, &Reception::setSignal);
}

void emitReception(YAML::Emitter& emitter, Reception& reception, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::RECEPTION, "reception", reception, data);
    emitBehavioralFeature(emitter, reception, data);
    if (reception.getSignal()) {
        emitter << YAML::Key << "signal" << YAML::Value << reception.getSignal().id().string();
    }
    emitElementDefenitionEnd(emitter, ElementType::RECEPTION, reception);
}

ActivityNode& determineAndParseActivityNode(YAML::Node node, ParserMetaData& data) {
    if (node["activityFinalNode"]) {
        return parseDefinition<ActivityFinalNode>(node, data, "activityFinalNode", parseActivityNode);
    } else if (node["activityParameterNode"]) {
        return parseDefinition<ActivityParameterNode>(node, data, "activityParameterNode", parseActivityParameterNode);
    } else if (node["centralBufferNode"]) {
        return parseDefinition<CentralBufferNode>(node, data, "centralBufferNode", parseObjectNode);
    } else if (node["dataStoreNode"]) {
        return parseDefinition<DataStoreNode>(node, data, "dataStoreNode", parseObjectNode);
    } else if (node["decisionNode"]) {
        return parseDefinition<DecisionNode>(node, data, "decisionNode", parseDecisionNode);
    } else if (node["executableNode"]) {
        return parseDefinition<ExecutableNode>(node, data, "executableNode", parseExecutableNode);
    } else if (node["flowFinalNode"]) {
        return parseDefinition<FlowFinalNode>(node, data, "flowFinalNode", parseActivityNode);
    } else if (node["forkNode"]) {
        return parseDefinition<ForkNode>(node, data, "forkNode", parseActivityNode);
    } else if (node["initialNode"]) {
        return parseDefinition<InitialNode>(node, data, "initialNode", parseActivityNode);
    } else if (node["inputPin"]) {
        throw UmlParserException("TODO inputPin", data.m_path.string(), node["inputPin"]);
    } else if (node["joinNode"]) {
        return parseDefinition<JoinNode>(node, data, "joinNode", parseJoinNode);
    } else if (node["mergeNode"]) {
        return parseDefinition<MergeNode>(node, data, "mergeNode", parseActivityNode);
    } else if (node["outputPin"]) {
        throw UmlParserException("TODO outputPin", data.m_path.string(), node["outputPin"]);
    } else {
        throw UmlParserException("Could not identify activity node", data.m_path.string(), node);
    }
}

ActivityEdge& determineAndParseActivityEdge(YAML::Node node, ParserMetaData& data) {
    if (node["controlFlow"]) {
        return parseDefinition<ControlFlow>(node, data, "controlFlow", parseActivityEdge);
    } else if (node["objectFlow"]) {
        return parseDefinition<ObjectFlow>(node, data, "objectFlow", parseObjectFlow);
    } else {
        throw UmlParserException("Invalid key for activity edge", data.m_path.string(), node);
    }
}

void parseActivity(YAML::Node node, Activity& activity, ParserMetaData& data) {
    parseBehavior(node, activity, data);
    parseSequenceDefinitions(node, data, "nodes", activity, &Activity::getNodes, determineAndParseActivityNode);
    parseSequenceDefinitions(node, data, "edges", activity, &Activity::getEdges, determineAndParseActivityEdge);
}

void emitActivity(YAML::Emitter& emitter, Activity& activity, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::ACTIVITY, "activity", activity, data);
    emitBehavior(emitter, activity, data);
    emitSequence(emitter, "nodes", data, activity, &Activity::getNodes);
    emitSequence(emitter, "edges", data, activity, &Activity::getEdges);
    emitElementDefenitionEnd(emitter, ElementType::ACTIVITY, activity);
}

void parseActivityNode(YAML::Node node, ActivityNode& activityNode, ParserMetaData& data) {
    parseNamedElement(node, activityNode, data);
    parseSetReferences<ActivityEdge, ActivityNode>(node, data, "incoming", activityNode, &ActivityNode::getIncoming);
    parseSetReferences<ActivityEdge, ActivityNode>(node, data, "outgoing", activityNode, &ActivityNode::getOutgoing);
}

void emitActivityNode(YAML::Emitter& emitter, ActivityNode& activityNode, EmitterMetaData& data) {
    emitNamedElement(emitter, activityNode, data);
    emitSequenceReferences(emitter, "incoming", data, activityNode, &ActivityNode::getIncoming);
    emitSequenceReferences(emitter, "outgoing", data, activityNode, &ActivityNode::getOutgoing);
}

void parseActivityEdge(YAML::Node node, ActivityEdge& edge, ParserMetaData& data) {
    parseNamedElement(node, edge, data);
    parseSingletonReference(node, data, "target", edge, &ActivityEdge::setTarget, &ActivityEdge::setTarget);
    parseSingletonReference(node, data, "source", edge, &ActivityEdge::setSource, &ActivityEdge::setSource);
    parseSingletonDefinition(node, data, "guard", edge, determineAndParseValueSpecification, &ActivityEdge::setGuard, &ActivityEdge::setGuard);
    parseSingletonDefinition(node, data, "weight", edge, determineAndParseValueSpecification, &ActivityEdge::setWeight, &ActivityEdge::setWeight);
}

void emitActivityEdge(YAML::Emitter& emitter, ActivityEdge& edge, EmitterMetaData& data) {
    emitNamedElement(emitter, edge, data);
    if (edge.getTarget()) {
        emitter << YAML::Key << "target" << YAML::Value << edge.getTarget().id().string();
    }
    if (edge.getSource()) {
        emitter << YAML::Key << "source" << YAML::Value << edge.getSource().id().string();
    }
}

void parseObjectFlow(YAML::Node node, ObjectFlow& flow, ParserMetaData& data) {
    parseActivityEdge(node, flow, data);
    parseSingletonDefinition<Behavior, ObjectFlow>(node, data, "transformation", flow, determineAndParseBehavior, &ObjectFlow::setTransformation, &ObjectFlow::setTransformation);
    parseSingletonDefinition<Behavior, ObjectFlow>(node, data, "selection", flow, determineAndParseBehavior, &ObjectFlow::setSelection, &ObjectFlow::setSelection);
}

void emitObjectFlow(YAML::Emitter& emitter, ObjectFlow& flow, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::OBJECT_FLOW, "objectFlow", flow, data);
    emitActivityEdge(emitter, flow, data);
    emitSingletonDefinition<Behavior, ObjectFlow>(emitter, "transformation", data, flow, &ObjectFlow::getTransformation);
    emitSingletonDefinition<Behavior, ObjectFlow>(emitter, "selection", data, flow, &ObjectFlow::getSelection);
    emitElementDefenitionEnd(emitter, ElementType::OBJECT_FLOW, flow);
}

void parseObjectNode(YAML::Node node, ObjectNode& objectNode, ParserMetaData& data) {
    parseTypedElement(node, objectNode, data);
    parseSetReferences<ActivityEdge, ActivityNode>(node, data, "incoming", objectNode, &ActivityNode::getIncoming);
    parseSetReferences<ActivityEdge, ActivityNode>(node, data, "outgoing", objectNode, &ActivityNode::getOutgoing);
    if (node["controlType"]) {
        if (node["controlType"].IsScalar()) {
            objectNode.setControlType(node["controlType"].as<bool>());
        }
    }
    if (node["ordering"]) {
        if (node["ordering"].IsScalar()) {
            std::string ordering = node["ordering"].as<std::string>();
            if (ordering.compare("unordered") == 0) {
                objectNode.setOrdering(ObjectNodeOrderingKind::UNORDERED);
            } else if (ordering.compare("ordered") == 0) {
                objectNode.setOrdering(ObjectNodeOrderingKind::ORDERED);
            } else if (ordering.compare("FIFO") == 0) {
                objectNode.setOrdering(ObjectNodeOrderingKind::FIFO);
            } else if (ordering.compare("LIFO") == 0) {
                objectNode.setOrdering(ObjectNodeOrderingKind::LIFO);
            } else {
                throw UmlParserException("Invalid object node ordering", data.m_path.string(), node["ordering"]);
            }
        }
    }
    parseSingletonDefinition<ValueSpecification, ObjectNode>(node, data, "upperBound", objectNode, determineAndParseValueSpecification, &ObjectNode::setUpperBound, &ObjectNode::setUpperBound);
    parseSingletonReference(node, data, "selection", objectNode, &ObjectNode::setSelection, &ObjectNode::setSelection);
}

void emitObjectNode(YAML::Emitter& emitter, ObjectNode& objectNode, EmitterMetaData& data) {
    emitTypedElement(emitter, objectNode, data);
    emitSequenceReferences<ActivityEdge, ActivityNode>(emitter, "incoming", data, objectNode, &ActivityNode::getIncoming);
    emitSequenceReferences<ActivityEdge, ActivityNode>(emitter, "outgoing", data, objectNode, &ActivityNode::getOutgoing);
    emitter << YAML::Key << "controlType" << YAML::Value;
    if (objectNode.isControlType()) {
        emitter << true;
    } else {
        emitter << false;
    }
    emitter << YAML::Key << "ordering" << YAML::Value;
    switch(objectNode.getOrdering()) {
        case ObjectNodeOrderingKind::UNORDERED : {
            emitter << "unordered";
            break;
        }
        case ObjectNodeOrderingKind::ORDERED : {
            emitter << "ordered";
            break;
        }
        case ObjectNodeOrderingKind::FIFO : {
            emitter << "FIFO";
            break;
        }
        case ObjectNodeOrderingKind::LIFO : {
            emitter << "LIFO";
            break;
        }
    }
    emitSingletonDefinition<ValueSpecification, ObjectNode>(emitter, "upperBound", data, objectNode, &ObjectNode::getUpperBound);
    if (objectNode.getSelection()) {
        emitter << YAML::Key << "selection" << YAML::Value << objectNode.getSelection().id().string();
    }
}

void parseActivityParameterNode(YAML::Node node, ActivityParameterNode& parameterNode, ParserMetaData& data) {
    parseObjectNode(node, parameterNode, data);
    parseSingletonReference(node, data, "parameter", parameterNode, &ActivityParameterNode::setParameter, &ActivityParameterNode::setParameter);
}

void emitActivityParameterNode(YAML::Emitter& emitter, ActivityParameterNode& parameterNode, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::ACTIVITY_PARAMETER_NODE, "activityParameterNode", parameterNode, data);
    emitObjectNode(emitter, parameterNode, data);
    if (parameterNode.getParameter()) {
        emitter << YAML::Key << "parameter" << YAML::Value << parameterNode.getParameter().id().string();
    }
    emitElementDefenitionEnd(emitter, ElementType::ACTIVITY_PARAMETER_NODE, parameterNode);
}

void parseDecisionNode(YAML::Node node, DecisionNode& decisionNode, ParserMetaData& data) {
    parseActivityNode(node, decisionNode, data);
    parseSingletonReference(node, data, "decisionInputFlow", decisionNode, &DecisionNode::setDecisionInputFlow, &DecisionNode::setDecisionInputFlow);
    parseSingletonReference(node, data, "decisionInput", decisionNode, &DecisionNode::setDecisionInput, &DecisionNode::setDecisionInput);
}

void emitDecisionNode(YAML::Emitter& emitter, DecisionNode& decisionNode, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::DECISION_NODE, "decisionNode", decisionNode, data);
    if (decisionNode.getDecisionInputFlow()) {
        emitter << YAML::Key << "decisionInputFlow" << YAML::Value << decisionNode.getDecisionInputFlow().id().string();
    }
    if (decisionNode.getDecisionInput()) {
        emitter << YAML::Key << "decisionInput" << YAML::Value << decisionNode.getDecisionInput().id().string();
    }
    emitElementDefenitionEnd(emitter, ElementType::DECISION_NODE, decisionNode);
}

void parseJoinNode(YAML::Node node, JoinNode& joinNode, ParserMetaData& data) {
    parseActivityNode(node, joinNode, data);
    parseSingletonDefinition(node, data, "joinSpec", joinNode, determineAndParseValueSpecification, &JoinNode::setJoinSpec, &JoinNode::setJoinSpec);
}

void emitJoinNode(YAML::Emitter& emitter, JoinNode& joinNode, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::JOIN_NODE, "joinNode", joinNode, data);
    emitActivityNode(emitter, joinNode, data);
    emitSingletonDefinition(emitter, "joinSpec", data, joinNode, &JoinNode::getJoinSpec);
    emitElementDefenitionEnd(emitter, ElementType::JOIN_NODE, joinNode);
}

ExceptionHandler& determineAndParseExceptionHandler(YAML::Node node, ParserMetaData& data) {
    if (node["exceptionHandler"]) {
        return parseDefinition<ExceptionHandler>(node, data, "exceptionHandler", parseExceptionHandler);
    } else {
        throw UmlParserException("could not identify exception handler", data.m_path.string(), node);
    }
}

void parseExecutableNode(YAML::Node node, ExecutableNode& executableNode, ParserMetaData& data) {
    parseActivityNode(node, executableNode, data);
    parseSequenceDefinitions(node, data, "handlers", executableNode, &ExecutableNode::getHandlers, determineAndParseExceptionHandler);
}

void emitExecutableNode(YAML::Emitter& emitter, ExecutableNode& executableNode, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::EXECUTABLE_NODE, "executableNode", executableNode, data);
    emitActivityNode(emitter, executableNode, data);
    emitSequence(emitter, "handlers", data, executableNode, &ExecutableNode::getHandlers);
}

void parseExceptionHandler(YAML::Node node, ExceptionHandler& handler, ParserMetaData& data) {
    parseElement(node, handler, data);
    parseSingletonReference(node, data, "handlerBody", handler, &ExceptionHandler::setHandlerBody, &ExceptionHandler::setHandlerBody);
    parseSingletonReference(node, data, "exceptionInput", handler, &ExceptionHandler::setExceptionInput, &ExceptionHandler::setExceptionInput);
    parseSetReferences<Classifier, ExceptionHandler>(node, data, "exceptionTypes", handler, &ExceptionHandler::getExceptionTypes);
}

void emitExceptionHandler(YAML::Emitter& emitter, ExceptionHandler& handler, EmitterMetaData& data) {
    emitElementDefenition(emitter, ElementType::EXCEPTION_HANDLER, "exceptionHandler", handler, data);
    if (handler.getHandlerBody()) {
        emitter << YAML::Key << "handlerBody" << YAML::Value << handler.getHandlerBody().id().string();
    }
    if (handler.getExceptionInput()) {
        emitter << YAML::Key << "exceptionInput" << YAML::Value << handler.getExceptionInput().id().string();
    }
    emitSequenceReferences(emitter, "exceptionTypes", data, handler, &ExceptionHandler::getExceptionTypes);
    emitElementDefenitionEnd(emitter, ElementType::EXCEPTION_HANDLER, handler);
}
    
}
}
}