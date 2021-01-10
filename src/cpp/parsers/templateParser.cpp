#include "headers/templateParser.h"

bool TemplateParser::parse(YAML::Node node) {
    cout << "[Info] keyword " << keyword << '\n';
    if (node[keyword]) {
        parseElement(node[keyword]);
        return true;
    }

    return false;
}

Element* TemplateParser::parseElement(YAML::Node node) {
    Element* el = createElement();
    (*elements)[el->uuid] = el;
    if (!parseFeatures(node, el)) {
        //Error
    }
    return el;
}