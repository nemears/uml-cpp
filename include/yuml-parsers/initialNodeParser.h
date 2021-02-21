#ifndef INITIALNODEPARSER_H
#define INITIALNODEPARSER_H

#include "activityNodeParser.h"
#include "uml/initialNode.h"

class InitialNodeParser : public ActivityNodeParser {
    public:
        Element* createElement() override;
        InitialNodeParser(map<boost::uuids::uuid, Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) :
            ActivityNodeParser(elements, postParsers),
            NamedElementParser(elements, postParsers)
            {
                keyword = "initialNode";
            };
        bool emit(YAML::Emitter& emitter, Element* el) override;
        static InitialNodeParser createNewParser();
};

#endif