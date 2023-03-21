#include "uml/managers/serialization/open_uml/parse.h"

using namespace std;

namespace UML {

template <class T, class V = T>
void parseFeatures(YAML::Node node, T& el, ParserData& data, void (*parser)(YAML::Node, V&, ParserData&)) {
    parser(node, el, data);
}

template <class T, class V = T, class ... Funcs>
void parseFeatures(YAML::Node node, T& el, ParserData& data, void (*parser)(YAML::Node, V&, ParserData&), Funcs... funcs) {
    parseFeatures(node, el, data, parser);
    parseFeatures(node, el, data, funcs...);
}

template <class T, class ... Funcs>
UmlPtr<T> createAndParse(YAML::Node node, ParserData& data, Funcs... funcs) {
    if (node.IsMap()) {
        UmlPtr<T> ret = data.manager->create<T>();
        parseFeatures(node, *ret, data, funcs...);
        return ret;
    } else {
        throw SerializationError("Could not parse data for element of type " + 
                                    Element::elementTypeToString(T::elementType()) + 
                                    " because node specifying it was not a Map node at " + 
                                    getLineNumber(node));
    }
}

template <class T, class V = T>
void parseScope(YAML::Node node, T& el, ParserData& data, bool (*parser)(YAML::Node, V&, ParserData&)) {
    parser(node, el, data);
}

template <class T, class V = T, class ... Funcs>
void parseScope(YAML::Node node, T& el, ParserData& data, bool (*parser)(YAML::Node, V&, ParserData&), Funcs... funcs) {
    if (!parser(node, el, data)) {
        parseScope(node, el, data, funcs...);
    }
} 

template <class T, class U, class S>
void parseSet(YAML::Node node, U& el, ParserData& data, string key, S& (U::*sequenceSignature)()) {
    if (node[key]) {
        if (!node[key].IsSequence()) {
            throw SerializationError("Could not parse set " + key + "because it's data was not a sequence! " + getLineNumber(node[key]));
        }
        for (size_t i = 0; i < node[key].size(); i++) {
            if (node[key][i].IsScalar()) {
                if (data.mode == SerializationMode::WHOLE) {
                    // TODO either throw error or parse another path
                    throw SerializationError("TODO parse external paths, this may be because of a bad id in your file/data! " + 
                                                getLineNumber(node[key][i]));
                } else {
                    (el.*sequenceSignature)().add(ID::fromString(node[key][i].as<string>().substr(0, 29)));
                }
            } else if (node[key][i].IsMap()) {
                (el.*sequenceSignature)().add(parseNode(node[key][i], data));
            }
        }
    }
}

template <class T, class U>
bool parseSingleton(YAML::Node node, U& el, ParserData& data, string key, void (U::*idMutator)(ID), void (U::*elMutator)(T&)) {
    if (node[key]) {
        if (node[key].IsMap()) {
            // assuming this is an owned singleton and we are parsing a project serialized in whole
            (el.*elMutator)(parseNode(node[key], data)->as<T>());
        } else if (node[key].IsScalar()) {
            (el.*idMutator)(ID::fromString(node[key].as<string>()));
        } else {
            throw SerializationError("Singleton " + key + " for element type " + Element::elementTypeToString(U::elementType()) + " could not be serialized because it was not a map or scalar " + getLineNumber(node[key]));
        }
        return true;
    }
    return false;
}

template <class T>
void parseBoolean(YAML::Node node, T& el, string key, void (T::*mutator)(bool)) {
    if (node[key]) {
        if (!node[key].IsScalar()) {
            throw SerializationError("Could not parse boolean value for property " + key + " for element type " + Element::elementTypeToString(T::elementType()) + " " + getLineNumber(node[key]));
        }
        (el.*mutator)(node[key].as<bool>());
    }
}

template <class T>
void parseInt(YAML::Node node, T& el, string key, void (T::*mutator)(int)) {
    if (node[key]) {
        if (!node[key].IsScalar()) {
            throw SerializationError("Could not parse integer value for property " + key + " for element type " + Element::elementTypeToString(T::elementType()) + " " + getLineNumber(node[key]));
        }
        (el.*mutator)(node[key].as<int>());
    }
}

string getLineNumber(YAML::Node node) {
    return "line number " + to_string(node.Mark().line);
}

ElementPtr parseNode(YAML::Node node, ParserData& data) {
    ElementPtr ret;
    if (node["abstraction"]) {
        ret = createAndParse<Abstraction>(node["abstraction"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseParameterableElementFeatures,
                    parseDependencyFeatures //,
                    // parseAbstractionFeatures
                    );
        parseScope(node, ret->as<Abstraction>(), data,
                    parsePackageableElementScope,
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    }
    // TODO
    else if (node["class"] && node["class"].IsMap()) {
        ret = createAndParse<Class>(node["class"], data,
                    parseElementFeatures,
                    parseNamespaceFeatures,
                    parseNamedElementFeatures,
                    parseTemplateableElementFeatures,
                    parseParameterableElementFeatures,
                    parseClassifierFeatures,
                    parseStructuredClassifierFeatures,
                    parseClassFeatures);
        parseScope(node, ret->as<Class>(), data, 
                    parsePackageableElementScope, 
                    parseParameterableElementScope,
                    parseNamedElementScope, 
                    parseElementScope);
    } else if (node["comment"]) {
        ret = createAndParse<Comment>(node["comment"], data,
                    parseElementFeatures,
                    parseCommentFeatures);
        parseScope(node, ret->as<Comment>(), data, parseElementScope);
    } else if (node["constraint"]) {
        ret = createAndParse<Constraint>(node["constraint"], data, 
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseParameterableElementFeatures,
                    parseConstraintFeatures);
        parseScope(node, ret->as<Constraint>(), data,
                    parseConstraintScope,
                    parsePackageableElementScope,
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["dataType"] && node["dataType"].IsMap()) {
        ret = createAndParse<DataType>(node["dataType"], data, 
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseNamespaceFeatures,
                    parseTemplateableElementFeatures,
                    parseParameterableElementFeatures,
                    parseClassifierFeatures,
                    parseDataTypeFeatures);
        parseScope(node, ret->as<DataType>(), data, 
                    parsePackageableElementScope, 
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["dependency"]) {
        ret = createAndParse<Dependency>(node["dependency"], data, 
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseParameterableElementFeatures,
                    parseDependencyFeatures);
        parseScope(node, ret->as<Dependency>(), data,
                    parsePackageableElementScope,
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["elementImport"]) {
        ret = createAndParse<ElementImport>(node["elementImport"], data,
                    parseElementFeatures,
                    parseElementImportFeatures);
        parseScope(node, ret->as<ElementImport>(), data, parseElementImportScope, parseElementScope);
    } else if (node["enumeration"] && node["enumeration"].IsMap()) {
        ret = createAndParse<Enumeration>(node["enumeration"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseNamespaceFeatures,
                    parseTemplateableElementFeatures,
                    parseParameterableElementFeatures,
                    parseClassifierFeatures,
                    parseDataTypeFeatures,
                    parseEnumerationFeatures);
        parseScope(node, ret->as<Enumeration>(), data,
                    parsePackageableElementScope,
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["enumerationLiteral"]) {
        ret = createAndParse<EnumerationLiteral>(node["enumerationLiteral"], data, 
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseParameterableElementFeatures,
                    parseInstanceSpecificationFeatures);
        parseScope(node, ret->as<EnumerationLiteral>(), data, 
                    parseEnumerationLiteralScope, 
                    parsePackageableElementScope, 
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    }
    // TODO
    else if (node["instanceSpecification"]) {
        ret = createAndParse<InstanceSpecification>(node["instanceSpecification"], data, 
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseParameterableElementFeatures,
                    parseInstanceSpecificationFeatures);
        parseScope(node, ret->as<InstanceSpecification>(), data, 
                    parsePackageableElementScope, 
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    }
    // TODO
    else if (node["package"]) {
        ret = createAndParse<Package>(node["package"], data, 
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseNamespaceFeatures,
                    parseTemplateableElementFeatures,
                    parseParameterableElementFeatures,
                    parsePackageFeatures);
        parseScope(node, ret->as<Package>(), data, 
                    parsePackageableElementScope, 
                    parseParameterableElementScope, 
                    parseNamedElementScope, 
                    parseElementScope);
    } else if (node["packageImport"]) {
        ret = createAndParse<PackageImport>(node["packageImport"], data,
                    parseElementFeatures,
                    parsePackageImportFeatures);
        parseScope(node, ret->as<PackageImport>(), data, 
                    parsePackageImportScope,
                    parseElementScope);
    } else if (node["primitivType"]) {
        ret = createAndParse<PrimitiveType>(node["primitiveType"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseNamespaceFeatures,
                    parseTemplateableElementFeatures,
                    parseParameterableElementFeatures,
                    parseClassifierFeatures,
                    parseDataTypeFeatures);
        parseScope(node, ret->as<PrimitiveType>(), data,
                    parsePackageableElementScope,
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["property"]) {
        ret = createAndParse<Property>(node["property"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseTypedElementFeatures,
                    parseMultiplicityElementFeatures,
                    parseParameterableElementFeatures);
        parseScope(node, ret->as<Property>(), data, 
                    parsePropertyScope, 
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["realization"]) {
        ret = createAndParse<Realization>(node["realization"], data, 
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseParameterableElementFeatures,
                    parseDependencyFeatures);
                    // TODO abstraction features
        parseScope(node, ret->as<Realization>(), data,
                    parsePackageableElementScope,
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else if (node["templateBinding"]) {
        ret = createAndParse<TemplateBinding>(node["templateBinding"], data,
                    parseElementFeatures,
                    parseTemplateBindingFeatures);
        parseScope(node, ret->as<TemplateBinding>(), data, parseTemplateBindingScope, parseElementScope);
    } else if (node["templateParameter"]) {
        ret = createAndParse<TemplateParameter>(node["templateParameter"], data,
                    parseElementFeatures,
                    parseTemplateParameterFeatures);
        parseScope(node, ret->as<TemplateParameter>(), data,
                    parseTemplateParameterScope,
                    parseElementScope);
    } else if (node["templateParameterSubstitution"]) {
        ret = createAndParse<TemplateParameterSubstitution>(node["templateParameterSubstitution"], data,
                        parseElementFeatures,
                        parseTemplateParameterSubstitutionFeatures);
        parseScope(node, ret->as<TemplateParameterSubstitution>(), data, parseTemplateParameterSubstitutionScope, parseElementScope);
    } else if (node["templateSignature"]) {
        ret = createAndParse<TemplateSignature>(node["templateSignature"], data,
                    parseElementFeatures,
                    parseTemplateSignatureFeatures);
        parseScope(node, ret->as<TemplateSignature>(), data,
                    parseTemplateSignatureScope,
                    parseElementScope);
    } else if (node["usage"]) {
        ret = createAndParse<Usage>(node, data, 
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseParameterableElementFeatures,
                    parseDependencyFeatures);
        parseScope(node, ret->as<Usage>(), data,
                    parsePackageableElementScope,
                    parseParameterableElementScope,
                    parseNamedElementScope,
                    parseElementScope);
    } else {
        throw SerializationError("Could not identify an element type to parse in data provided for parsing " + getLineNumber(node));
    }
    return ret;
}

void parseClassFeatures(YAML::Node node, Class& clazz, ParserData& data) {
    parseSet<Classifier>(node, clazz, data, "nestedClassifiers", &Class::getNestedClassifiers);
    parseSet<Property>(node, clazz, data, "ownedAttributes", &Class::getOwnedAttributes);
    parseSet<Operation>(node, clazz, data, "ownedOperations", &Class::getOwnedOperations);
    parseSet<Reception>(node, clazz, data, "ownedReceptions", &Class::getOwnedReceptions);
}

void parseClassifierFeatures(YAML::Node node, Classifier& classifier, ParserData& data) {
    parseSet<Generalization>(node, classifier, data, "generalizations", &Classifier::getGeneralizations);
}

void parseCommentFeatures(YAML::Node node, Comment& comment, ParserData& data) {
    parseSet<Element>(node, comment, data, "annotatedElements", &Comment::getAnnotatedElements);
    if (node["body"]) {
        if (!node["body"].IsScalar()) {
            comment.setBody(node["body"].as<string>());
        }
    }
}

void parseConstraintFeatures(YAML::Node node, Constraint& constraint, ParserData& data) {
    parseSet<Element>(node, constraint, data, "constrainedElements", &Constraint::getConstrainedElements);
    parseSingleton<ValueSpecification>(node, constraint, data, "specification", &Constraint::setSpecification, &Constraint::setSpecification);
}

void parseDataTypeFeatures(YAML::Node node, DataType& dataType, ParserData& data) {
    parseSet<Property>(node, dataType, data, "ownedAttributes", &DataType::getOwnedAttributes);
    parseSet<Operation>(node, dataType, data, "ownedOperations", &DataType::getOwnedOperations);
}

void parseDependencyFeatures(YAML::Node node, Dependency& dependency, ParserData& data) {
    parseSet<NamedElement>(node, dependency, data, "suppliers", &Dependency::getSuppliers);
    parseSet<NamedElement>(node, dependency, data, "clients", &Dependency::getClients);
}

void parseElementFeatures(YAML::Node node, Element& el, ParserData& data) {
    if (node["id"]) {
        if (!node["id"].IsScalar()) {
            throw SerializationError("Could not parse id of element because it is not a scalar value " + getLineNumber(node["id"]));
        }
        string idString = node["id"].as<string>();
        if (!isValidID(idString)) {
            throw SerializationError("Could not parse id of element because it is not a valid id. A valid id is a 28 character url safe base64 encoded string " + getLineNumber(node["id"]));
        }
        el.setID(idString);
    }
    parseSet<InstanceSpecification>(node, el, data, "appliedStereotypes", &Element::getAppliedStereotypes);
}

template <class T>
void parseVisibilty(YAML::Node node, T& elWithVisibility) {
    if (node["visibility"]) {
        if (!node["visibility"].IsScalar()) {
            throw SerializationError("could not parse visibility, node must be a scalar!" + getLineNumber(node["visibility"]));
        }
        string visibilityValue = node["visibility"].as<string>();
        if (visibilityValue == "package") {
            elWithVisibility.setVisibility(VisibilityKind::PACKAGE);
        } else if (visibilityValue == "private") {
            elWithVisibility.setVisibility(VisibilityKind::PRIVATE);
        } else if (visibilityValue == "protected") {
            elWithVisibility.setVisibility(VisibilityKind::PROTECTED);
        }
    }
}

void parseElementImportFeatures(YAML::Node node, ElementImport& elementImport, ParserData& data) {
    parseSingleton<PackageableElement>(node, elementImport, data, "importedElement", &ElementImport::setImportedElement, &ElementImport::setImportedElement);
    if (node["alias"]) {
        if (!node["alias"].IsScalar()) {
            throw SerializationError("Could not parse elementImport alias because it was not a string! " + getLineNumber(node["alias"]));
        }
        elementImport.setAlias(node["alias"].as<string>());
    }
    parseVisibilty(node, elementImport);
}

void parseEnumerationFeatures(YAML::Node node, Enumeration& enumeration, ParserData& data) {
    parseSet<EnumerationLiteral>(node, enumeration, data, "ownedLiterals", &Enumeration::getOwnedLiterals);
}

void parseInstanceSpecificationFeatures(YAML::Node node, InstanceSpecification& inst, ParserData& data) {

}

void parseMultiplicityElementFeatures(YAML::Node node, MultiplicityElement& multiplicityElement, ParserData& data) {
    parseBoolean(node, multiplicityElement, "isOrdered", &MultiplicityElement::setIsOrdered);
    parseBoolean(node, multiplicityElement, "isUnique", &MultiplicityElement::setIsUnique);
    parseSingleton<ValueSpecification>(node, multiplicityElement, data, "lowerValue", &MultiplicityElement::setLowerValue, &MultiplicityElement::setLowerValue);
    parseInt(node, multiplicityElement, "lower", &MultiplicityElement::setLower);
    parseSingleton<ValueSpecification>(node, multiplicityElement, data, "upperValue", &MultiplicityElement::setUpperValue, &MultiplicityElement::setUpperValue);
    parseInt(node, multiplicityElement, "upper", &MultiplicityElement::setUpper);
}

void parseNamedElementFeatures(YAML::Node node, NamedElement& el, ParserData& data) {
    if (node["name"]) {
        if (!node["name"].IsScalar()) {
            throw SerializationError("data for name is not in a valid format! It must be a scalar string! " + getLineNumber(node["name"]));
        }
        el.setName(node["name"].as<string>());
    }
    parseVisibilty(node, el);
    parseSet<Dependency>(node, el, data, "clientDependencies", &NamedElement::getClientDependencies);
}

void parseNamespaceFeatures(YAML::Node node, Namespace& nmspc, ParserData& data) {
    parseSet<ElementImport>(node, nmspc, data, "elementImports", &Namespace::getElementImports);
    parseSet<PackageImport>(node, nmspc, data, "packageImports", &Namespace::getPackageImports);
    parseSet<Constraint>(node, nmspc, data, "ownedRules", &Namespace::getOwnedRules);
}

void parsePackageFeatures(YAML::Node node, Package& pckg, ParserData& data) {
    parseSet<InstanceSpecification>(node, pckg, data, "packagedElements", &Package::getPackagedElements);
}

void parsePackageImportFeatures(YAML::Node node, PackageImport& packageImport, ParserData& data) {
    parseSingleton<Package>(node, packageImport, data, "importedPackage", &PackageImport::setImportedPackage, &PackageImport::setImportedPackage);
    parseVisibilty(node, packageImport);
}

void parseParameterableElementFeatures(YAML::Node node, ParameterableElement& parameterableElement, ParserData& data) {
    parseSingleton<TemplateParameter>(node, parameterableElement, data, "templateParameter", &ParameterableElement::setTemplateParameter, &ParameterableElement::setTemplateParameter);
}

void parseStructuredClassifierFeatures(YAML::Node node, StructuredClassifier& structuredClassifier, ParserData& data) {
    parseSet<Connector>(node, structuredClassifier, data, "ownedConnectors", &StructuredClassifier::getOwnedConnectors);
}

void parseTemplateableElementFeatures(YAML::Node node, TemplateableElement& templateableElement, ParserData& data) {
    parseSingleton<TemplateSignature>(node, templateableElement, data, "ownedTemplateSignature", &TemplateableElement::setOwnedTemplateSignature, &TemplateableElement::setOwnedTemplateSignature);
    parseSet<TemplateBinding>(node, templateableElement, data, "templateBindings", &TemplateableElement::getTemplateBindings);
}

void parseTemplateBindingFeatures(YAML::Node node, TemplateBinding& templateBinding, ParserData& data) {
    parseSet<TemplateParameterSubstitution>(node, templateBinding, data, "parameterSubstitutions", &TemplateBinding::getParameterSubstitutions);
    parseSingleton<TemplateSignature>(node, templateBinding, data, "signature", &TemplateBinding::setSignature, &TemplateBinding::setSignature);
}

void parseTemplateParameterFeatures(YAML::Node node, TemplateParameter& templateParameter, ParserData& data) {
    parseSingleton<ParameterableElement>(node, templateParameter, data, "ownedParameteredElement", &TemplateParameter::setOwnedParameteredElement, &TemplateParameter::setOwnedParameteredElement);
    parseSingleton<ParameterableElement>(node, templateParameter, data, "parameteredElement", &TemplateParameter::setParameteredElement, &TemplateParameter::setParameteredElement);
    parseSingleton<ParameterableElement>(node, templateParameter, data, "ownedDefault", &TemplateParameter::setOwnedDefault, &TemplateParameter::setOwnedDefault);
    parseSingleton<ParameterableElement>(node, templateParameter, data, "default", &TemplateParameter::setDefault, &TemplateParameter::setDefault);
}

void parseTemplateParameterSubstitutionFeatures(YAML::Node node, TemplateParameterSubstitution& templateParameterSubstitution, ParserData& data) {
    parseSingleton<TemplateParameter>(node, templateParameterSubstitution, data, "formal", &TemplateParameterSubstitution::setFormal, &TemplateParameterSubstitution::setFormal);
    parseSingleton<ParameterableElement>(node, templateParameterSubstitution, data, "ownedActual", &TemplateParameterSubstitution::setOwnedActual, &TemplateParameterSubstitution::setOwnedActual);
    parseSingleton<ParameterableElement>(node, templateParameterSubstitution, data, "actual", &TemplateParameterSubstitution::setActual, &TemplateParameterSubstitution::setActual);
}

void parseTemplateSignatureFeatures(YAML::Node node, TemplateSignature& templateSignature, ParserData& data) {
    parseSet<TemplateParameter>(node, templateSignature, data, "parameters", &TemplateSignature::getParameters);
    parseSet<TemplateParameter>(node, templateSignature, data, "ownedParameters", &TemplateSignature::getOwnedParameters);
}

void parseTypedElementFeatures(YAML::Node node, TypedElement& typedElement, ParserData& data) {
    parseSingleton<Type>(node, typedElement, data, "type", &TypedElement::setType, &TypedElement::setType);
}

bool parseConstraintScope(YAML::Node node, Constraint& constraint, ParserData& data) {
    return parseSingleton<Namespace>(node, constraint, data, "context", &Constraint::setContext, &Constraint::setContext);
}

bool parseElementScope(YAML::Node node, Element& el, ParserData& data) {
    if (node["owner"]) {
        el.setOwner(ID::fromString(node["owner"].as<string>()));
        return true;
    }
    return false;
}

bool parseElementImportScope(YAML::Node node, ElementImport& elementImport, ParserData& data) {
    return parseSingleton<Namespace>(node, elementImport, data, "importingNamespace", &ElementImport::setImportingNamespace, &ElementImport::setImportingNamespace);
}

bool parseEnumerationLiteralScope(YAML::Node node, EnumerationLiteral& literal, ParserData& data) {
    if (node["enumeration"]) {
        if (!node["enumeration"].IsScalar()) {
            throw SerializationError("Could not parse enumeration literal's owning eneruation, must be a scalar " + getLineNumber(node["enumeration"]));
        }
        literal.setEnumeration(ID::fromString(node["enumeration"].as<string>()));
        return true;
    }
    return false;
}

bool parseNamedElementScope(YAML::Node node, NamedElement& namedElement, ParserData& data) {
    if (node["namespace"]) {
        namedElement.setNamespace(ID::fromString(node["namespace"].as<string>()));
        return true;
    }
    return false;
}

bool parsePackageableElementScope(YAML::Node node, PackageableElement& el, ParserData& data) {
    return parseSingleton<Package>(node, el, data, "owningPackage", &PackageableElement::setOwningPackage, &PackageableElement::setOwningPackage);
}

bool parsePackageImportScope(YAML::Node node, PackageImport& packageImport, ParserData& data) {
    return parseSingleton<Namespace>(node, packageImport, data, "importingNamespace", &PackageImport::setImportingNamespace, &PackageImport::setImportingNamespace);
}

bool parseParameterableElementScope(YAML::Node node, ParameterableElement& parameterableElement, ParserData& data) {
    return parseSingleton<TemplateParameter>(node, parameterableElement, data, "owningTemplateParameter", &ParameterableElement::setOwningTemplateParameter, &ParameterableElement::setOwningTemplateParameter);
}

bool parsePropertyScope(YAML::Node node, Property& property, ParserData& data) {
    if (node["class"] && node["class"].IsScalar()) {
        property.setClass(ID::fromString(node["class"].as<string>()));
        return true;
    } else if (node["dataType"] && node["dataType"].IsScalar()) {
        property.setDataType(ID::fromString(node["dataType"].as<string>()));
        return true;
    } else if (node["interface"] && node["interface"].IsScalar()) {
        property.setInterface(ID::fromString(node["interface"].as<string>()));
    }
    return false;
}

bool parseTemplateBindingScope(YAML::Node node, TemplateBinding& templateBinding, ParserData& data) {
    return parseSingleton<TemplateableElement>(node, templateBinding, data, "boundElement", &TemplateBinding::setBoundElement, &TemplateBinding::setBoundElement);
}

bool parseTemplateParameterScope(YAML::Node node, TemplateParameter& templateParameter, ParserData& data) {
    return parseSingleton<TemplateSignature>(node, templateParameter, data, "signature", &TemplateParameter::setSignature, &TemplateParameter::setSignature);
}

bool parseTemplateParameterSubstitutionScope(YAML::Node node, TemplateParameterSubstitution& templateParameterSubstitution, ParserData& data) {
    return parseSingleton<TemplateBinding>(node, templateParameterSubstitution, data, "templateBinding", &TemplateParameterSubstitution::setTemplateBinding, &TemplateParameterSubstitution::setTemplateBinding);
}

bool parseTemplateSignatureScope(YAML::Node node, TemplateSignature& templateSignature, ParserData& data) {
    return parseSingleton<TemplateableElement>(node, templateSignature, data, "template", &TemplateSignature::setTemplate, &TemplateSignature::setTemplate);
}

}