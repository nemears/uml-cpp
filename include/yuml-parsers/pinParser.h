#ifndef PINPARSER_H
#define PINPARSER_H

#include "objectNodeParser.h"
#include "multiplicityElementParser.h"

class PinParser : public ObjectNodeParser, public MultiplicityElementParser {
    protected:
        bool parseFeatures(YAML::Node node, Element* el) override;

    public:
        bool emit(YAML::Emitter& emitter, Element* el) override;
        PinParser(map<boost::uuids::uuid, Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) : 
            ObjectNodeParser(elements, postParsers), MultiplicityElementParser(elements, postParsers) {};
};

#endif