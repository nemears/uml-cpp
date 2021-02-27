#ifndef CALLBEHAVIORACTIONPARSER_H
#define CALLBEHAVIORACTIONPARSER_H

#include "actionParser.h"
#include "uml/callBehaviorAction.h"

class CallBehaviorActionParser : public ActionParser {
    protected:
        bool parseFeatures(YAML::Node node, Element* el) override;
    public:
        Element* createElement() override;
        bool emit(YAML::Emitter& emitter, Element* el) override;
        CallBehaviorActionParser(map<boost::uuids::uuid, Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) : 
            ActionParser(elements, postParsers), 
            NamedElementParser(elements, postParsers){
            this->keyword = "callBehaviorAction";
        };
};

#endif