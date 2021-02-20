#ifndef OUTPUTPINPARSER_H
#define OUTPUTPINPARSER_H

#include "pinParser.h"
#include "uml/outputPin.h"

class OutputPinParser : public PinParser {
    public:
        Element* createElement() override;
        OutputPinParser(map<boost::uuids::uuid, Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) : 
            PinParser(elements, postParsers), 
            NamedElementParser(elements, postParsers), 
            ElementParser(elements, postParsers) {
            this->TypedElementParser::keyword = "outputPin";
        }
        bool emit(YAML::Emitter& emitter, Element* el) override;
        static OutputPinParser createNewParser();
};

#endif