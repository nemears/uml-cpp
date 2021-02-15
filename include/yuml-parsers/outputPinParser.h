#ifndef OUTPUTPINPARSER_H
#define OUTPUTPINPARSER_H

#include "pinParser.h"

class OutputPinParser : public PinParser {
    public:
        OutputPinParser(map<boost::uuids::uuid, Element*>* elements) : PinParser(elements) {
            this->TypedElementParser::keyword = "outputPin";
        }
};

#endif