#include "yuml-parsers/templateParser.h"

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
    
    // backwards parsing
    if ((*postProcessFlag)[el->uuid]) {
        list<boost::uuids::uuid>::iterator elIt = (*postProcessFlag)[el->uuid]->otherEls.begin();
        for (auto const& fun : (*postProcessFlag)[el->uuid]->applyOnEl) {
            Element* me = (*elements)[el->uuid];
            Element* someoneWhoNeedsMe = (*elements)[(*elIt)];
            (*fun)(someoneWhoNeedsMe, me);
            ++elIt;
        }

        delete (*postProcessFlag)[el->uuid];
    }

    return el;
}

string TemplateParser::emitDocument(Element* el) {
    YAML::Emitter emitter;
    emitter << YAML::BeginDoc;
    if (emit(emitter, el)) {
        emitter << YAML::EndDoc;
        return emitter.c_str();
    } else {
        // TODO throw fatal emit error
        return NULL;
    }
}