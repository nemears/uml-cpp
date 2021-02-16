#ifndef INPUTPINPARSER_H
#define INPUTPINPARSER_H

#include "pinParser.h"
#include "uml/inputPin.h"

class InputPinParser : public PinParser {
    public:
        Element* createElement() override;
        InputPinParser(map<boost::uuids::uuid, Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) : 
            PinParser(elements, postParsers), 
            NamedElementParser(elements, postParsers), 
            ElementParser(elements, postParsers) {
            this->TypedElementParser::keyword = "inputPin";
        }
        static InputPinParser createNewParser();
};

#endif