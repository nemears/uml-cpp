#ifndef ACTIONPARSER_H
#define ACTIONPARSER_H

#include "activityNodeParser.h"
#include "inputPinParser.h"
#include "outputPinParser.h"
#include "uml/action.h"

class ActionParser : public ActivityNodeParser {
    protected:
        bool parseFeatures(YAML::Node node, Element * el) override;
    
    public:
        Element* createElement() override;
        bool emit(YAML::Emitter& emitter, Element* el) override;
        ActionParser(map<boost::uuids::uuid, Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) : 
            ActivityNodeParser(elements, postParsers), 
            NamedElementParser(elements, postParsers){
            this->keyword = "action";
        };
        static ActionParser createNewParser();
};

#endif