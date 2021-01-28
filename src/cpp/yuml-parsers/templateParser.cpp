#include "yuml-parsers/templateParser.h"

bool TemplateParser::parse(YAML::Node node) {
    cout << "[Info] keyword " << keyword << '\n';
    if (node[keyword]) {
        parseElement(node[keyword]);
        return true;
    }

    return false;
}

UML::Element* TemplateParser::parseElement(YAML::Node node) {
    UML::Element* el = createElement();
    (*elements)[el->uuid] = el;
    if (!parseFeatures(node, el)) {
        //Error
    }
    return el;
}