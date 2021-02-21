#ifndef JOINNODEPARSER_H
#define JOINNODEPARSER_H

#include "activityNodeParser.h"
#include "uml/joinNode.h"

class JoinNodeParser : public ActivityNodeParser {
    public:
        Element* createElement() override;
        JoinNodeParser(map<boost::uuids::uuid, Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) :
            ActivityNodeParser(elements, postParsers),
            NamedElementParser(elements, postParsers)
            {
                keyword = "joinNode";
            };
        bool emit(YAML::Emitter& emitter, Element* el) override;
        static JoinNodeParser createNewParser();
};

#endif