#include "yuml-parsers/namespaceParser.h"

using namespace UML;

Element* NamespaceParser::createElement() {
    return new Namespace;
}

bool NamespaceParser::parseFeatures(YAML::Node node, Element* el) {
    bool ret = NamedElementParser::parseFeatures(node, el);

    // add all children that are members
    for (auto const& child : el->getOwnedElements()) {
        if (child->isSubClassOf(ElementType::NAMED_ELEMENT)) {
            dynamic_cast<Namespace*>(el)->getMembers().add(*dynamic_cast<NamedElement*>(child));
            dynamic_cast<NamedElement*>(child)->setNamespace(dynamic_cast<Namespace*>(el));
        }
    }

    return ret;
}

bool NamespaceParser::emit(YAML::Emitter& emitter, Element* el) {

    if (el->getElementType() == ElementType::NAMESPACE) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "namespace";
        emitter << YAML::BeginMap;
    }

    bool ret = NamedElementParser::emit(emitter, el);

    if (el->getElementType() == ElementType::NAMESPACE) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }
    return ret;
}

NamespaceParser NamespaceParser::createNewParser() {
    return NamespaceParser(new map<boost::uuids::uuid, Element*>, new map<boost::uuids::uuid, PostParser*>);
}