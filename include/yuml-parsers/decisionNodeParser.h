#ifndef DECISIONNODEPARSER_H
#define DECISIONNODEPARSER_H

#include "activityNodeParser.h"
#include "uml/decisionNode.h"

class DecisionNodeParser : public ActivityNodeParser {
    public:
        Element* createElement() override;
        DecisionNodeParser(map<boost::uuids::uuid, Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) :
            ActivityNodeParser(elements, postParsers),
            NamedElementParser(elements, postParsers)
            {
                keyword = "decisionNode";
            };
        bool emit(YAML::Emitter& emitter, Element* el) override;
        static DecisionNodeParser createNewParser();
};

#endif