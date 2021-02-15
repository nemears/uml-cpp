#ifndef INPUTPINPARSER_H
#define INPUTPINPARSER_H

#include "pinParser.h"
#include "uml/inputPin.h"

class InputPinParser : public PinParser {
    public:
        Element* createElement() override;
        InputPinParser(map<boost::uuids::uuid, Element*>* elements) : PinParser(elements), NamedElementParser(elements), ElementParser(elements) {
            this->TypedElementParser::keyword = "inputPin";
        }
};

#endif