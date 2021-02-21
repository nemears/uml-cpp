#ifndef FINALNODEPARSER_H
#define FINALNODEPARSER_H

#include "activityNodeParser.h"
#include "uml/finalNode.h"

class FinalNodeParser : public ActivityNodeParser {
    public:
        Element* createElement() override;
        FinalNodeParser(map<boost::uuids::uuid, Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) :
            ActivityNodeParser(elements, postParsers),
            NamedElementParser(elements, postParsers)
            {
                keyword = "finalNode";
            };
        bool emit(YAML::Emitter& emitter, Element* el) override;
        static FinalNodeParser createNewParser();
};

#endif