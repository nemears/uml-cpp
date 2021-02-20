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

bool TemplateParser::parseNowOrLater(boost::uuids::uuid laterId, boost::uuids::uuid myId, void(*funPtr)(Element*, Element*)) {
    // check if null
    // if null we make a flag for backwards parsing
    if((*elements)[laterId] == 0) {

        // check if struct created
        if ((*postProcessFlag)[laterId] == 0) {
            list<boost::uuids::uuid>* eList = new list<boost::uuids::uuid>;
            list<void(*)(Element*, Element*)>* fList = new list<void(*)(Element*, Element*)>;
            PostParser* postParser = new PostParser{*eList, *fList};
            (*postProcessFlag)[laterId] = postParser;
        } 

        // add flag with function pointer
        (*postProcessFlag)[laterId]->otherEls.push_back(myId);
        (*postProcessFlag)[laterId]->applyOnEl.push_back(funPtr);
        return false;
    } else {
        (*funPtr)((*elements)[myId], (*elements)[laterId]);
        return true;
    }
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