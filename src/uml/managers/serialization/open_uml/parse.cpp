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
    if (parser(node, el, data)) {
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
void parseSingleton(YAML::Node node, U& el, ParserData& data, string key, void (U::*idMutator)(ID), void (U::*elMutator)(T&)) {
    if (node[key]) {
        if (node[key].IsMap()) {
            // assuming this is an owned singleton and we are parsing a project serialized in whole
            (el.*elMutator)(parseNode(node[key], data)->as<T>());
        } else if (node[key].IsScalar()) {
            (el.*idMutator)(ID::fromString(node[key].as<string>()));
        } else {
            throw SerializationError("Singleton " + key + " for element type " + Element::elementTypeToString(U::elementType()) + " could not be serialized because it was not a map or scalar " + getLineNumber(node[key]));
        }
    }
}

string getLineNumber(YAML::Node node) {
    return "line number " + to_string(node.Mark().line);
}

ElementPtr parseNode(YAML::Node node, ParserData& data) {
    ElementPtr ret;
    if (node["abstraction"]) {
        // TODO
    }
    // TODO
    else if (node["dataType"] && node["dataType"].IsMap()) {
        ret = createAndParse<DataType>(node["dataType"], data, 
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseDataTypeFeatures);
        parseScope(node, ret->as<DataType>(), data, parsePackageableElementScope, parseElementScope);
    return ret;
    } else if (node["enumerationLiteral"]) {
        ret = createAndParse<DataType>(node["dataType"], data, 
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseDataTypeFeatures);
        parseScope(node, ret->as<EnumerationLiteral>(), data, parsePackageableElementScope, parseElementScope);
    }
    // TODO
    else if (node["instanceSpecification"]) {
        ret = createAndParse<InstanceSpecification>(node["instanceSpecification"], data, 
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseInstanceSpecificationFeatures);
        parseScope(node, ret->as<InstanceSpecification>(), data, parseElementScope);
    }
    // TODO
    else if (node["package"]) {
        ret = createAndParse<Package>(node["package"], data, 
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parsePackageFeatures);
        parseScope(node, ret->as<Package>(), data, parsePackageableElementScope, parseElementScope);
    } else if (node["property"]) {
        ret = createAndParse<Property>(node["property"], data,
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseTypedElementFeatures);
        parseScope(node, ret->as<Property>(), data, parsePropertyScope, parseElementScope);
    }
    return ret;
}

void parseDataTypeFeatures(YAML::Node node, DataType& dataType, ParserData& data) {
    parseSet<Property>(node, dataType, data, "ownedAttributes", &DataType::getOwnedAttributes);
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

void parseInstanceSpecificationFeatures(YAML::Node node, InstanceSpecification& inst, ParserData& data) {

}

void parseNamedElementFeatures(YAML::Node node, NamedElement& el, ParserData& data) {
    if (node["name"]) {
        if (!node["name"].IsScalar()) {
            throw SerializationError("data for name is not in a valid format! It must be a scalar string! " + getLineNumber(node["name"]));
        }
        el.setName(node["name"].as<string>());
    }
}

void parsePackageFeatures(YAML::Node node, Package& pckg, ParserData& data) {
    parseSet<InstanceSpecification>(node, pckg, data, "packagedElements", &Package::getPackagedElements);
}

void parseTypedElementFeatures(YAML::Node node, TypedElement& typedElement, ParserData& data) {
    parseSingleton<Type>(node, typedElement, data, "type", &TypedElement::setType, &TypedElement::setType);
}

bool parseElementScope(YAML::Node node, Element& el, ParserData& data) {
    if (node["owner"]) {
        el.setOwner(ID::fromString(node["owner"].as<string>()));
        return true;
    }
    return false;
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

bool parsePackageableElementScope(YAML::Node node, PackageableElement& el, ParserData& data) {
    if (node["owningPackage"]) {
        if (!node["owningPackage"].IsScalar()) {
            throw SerializationError("Could not parse owning package, must be a scalar " + getLineNumber(node["owningPackage"]));
        }
        el.setOwningPackage(ID::fromString(node["owningPackage"].as<string>()));
        return true;
    }
    return false;
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

}