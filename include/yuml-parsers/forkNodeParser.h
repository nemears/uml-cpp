#ifndef FORKNODEPARSER_H
#define FORKNODEPARSER_H

#include "activityNodeParser.h"
#include "uml/forkNode.h"

class ForkNodeParser : public ActivityNodeParser {
    public:
        Element* createElement() override;
        ForkNodeParser(map<boost::uuids::uuid, Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) :
            ActivityNodeParser(elements, postParsers),
            NamedElementParser(elements, postParsers)
            {
                keyword = "forkNode";
            };
        bool emit(YAML::Emitter& emitter, Element* el) override;
        static ForkNodeParser createNewParser();
};

#endif