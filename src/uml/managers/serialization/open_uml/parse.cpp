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

template <class T, class U, class S>
void parseSet(YAML::Node node, U& el, ParserData& data, string key, S& (U::*sequenceSignature)(), UmlPtr<T> (*parserSignature)(YAML::Node, ParserData&)) {
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
                (el.*sequenceSignature)().add((*parserSignature)(node[key][i], data));
            }
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
    else if (node["enumerationLiteral"]) {
        ret = createAndParseEnumerationLiteral(node["enumerationLiteral"], data);
    }
    // TODO
    else if (node["instanceSpecification"]) {
        ret = createAndParse<InstanceSpecification>(node["instanceSpecification"], data, 
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseInstanceSpecificationFeatures);
    }
    return ret;
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
    parseSet(node, el, data, "appliedStereotypes", &Element::getAppliedStereotypes, determineAndParseInstanceSpecification);
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

EnumerationLiteralPtr createAndParseEnumerationLiteral(YAML::Node node, ParserData& data) {
    return createAndParse<EnumerationLiteral>(node, data, 
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseInstanceSpecificationFeatures);
}

InstanceSpecificationPtr createAndParseInstanceSpecification(YAML::Node node, ParserData& data) {
    return createAndParse<InstanceSpecification>(node, data, 
                    parseElementFeatures,
                    parseNamedElementFeatures,
                    parseInstanceSpecificationFeatures);
}

InstanceSpecificationPtr determineAndParseInstanceSpecification(YAML::Node node, ParserData& data) {
    if (node["enumerationLiteral"]) {
        return createAndParseEnumerationLiteral(node["enumerationLiteral"], data);
    } else if (node["instanceSpecification"]) {
        return createAndParseInstanceSpecification(node["instanceSpecification"], data);
    }
}

}