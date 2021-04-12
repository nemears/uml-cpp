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
                if (data.elements.count(typeID)) {
                    if (data.elements.get(typeID)->isSubClassOf(ElementType::TYPE)) {
                        el.setType(dynamic_cast<Type*>(data.elements.get(typeID)));
                    } else {
                        // ERROR
                    }
                } else {
                    if (!data.postProcessFlag.count(typeID)) {
                        data.postProcessFlag[typeID] = new vector<AbstractPostProcessFunctor*>;
                    }
                    data.postProcessFlag[typeID]->push_back(new SetTypeFunctor(&el));
                }
            } else {
                // error
            }
        } else {
            // error
        }
    }
}