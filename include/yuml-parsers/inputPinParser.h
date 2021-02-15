#ifndef INPUTPINPARSER_H
#define INPUTPINPARSER_H

#include "pinParser.h"

class InputPinParser : public PinParser {
    public:
        InputPinParser(map<boost::uuids::uuid, Element*>* elements) : PinParser(elements) {
            this->TypedElementParser::keyword = "inputPin";
        }
};

#endif