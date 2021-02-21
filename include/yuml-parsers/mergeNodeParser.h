#ifndef MERGENODEPARSER_H
#define MERGENODEPARSER_H

#include "activityNodeParser.h"
#include "uml/mergeNode.h"

class MergeNodeParser : public ActivityNodeParser {
    public:
        Element* createElement() override;
        MergeNodeParser(map<boost::uuids::uuid, Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) :
            ActivityNodeParser(elements, postParsers),
            NamedElementParser(elements, postParsers)
            {
                keyword = "mergeNode";
            };
        bool emit(YAML::Emitter& emitter, Element* el) override;
        static MergeNodeParser createNewParser();
};

#endif