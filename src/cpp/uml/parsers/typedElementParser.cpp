#include "uml/parsers/typedElementParser.h"
#include "uml/parsers/namedElementParser.h"

using namespace UML;
using namespace Parsers;

void UML::Parsers::SetTypeFunctor::operator()(Element& el) const {
    dynamic_cast<TypedElement*>(m_el)->setType(&dynamic_cast<Type&>(el));
}

void UML::Parsers::parseTypedElement(YAML::Node node, TypedElement& el, ParserMetaData& data) {

    parseNamedElement(node, el, data);

    if (node["type"]) {
        if (node["type"].IsScalar()) {
            string typeIDstring = node["type"].as<string>();
            if (isValidUUID4(typeIDstring)) {
                boost::uuids::uuid typeID = boost::lexical_cast<boost::uuids::uuid>(typeIDstring);
                applyFunctor(data, typeID, new SetTypeFunctor(&el));
            } else {
                // error
            }
        } else {
            // error
        }
    }
}