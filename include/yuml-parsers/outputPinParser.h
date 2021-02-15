#ifndef OUTPUTPINPARSER_H
#define OUTPUTPINPARSER_H

#include "pinParser.h"
#include "uml/outputPin.h"

class OutputPinParser : public PinParser {
    public:
        Element* createElement() override;
        OutputPinParser(map<boost::uuids::uuid, Element*>* elements) : PinParser(elements), NamedElementParser(elements), ElementParser(elements) {
            this->TypedElementParser::keyword = "outputPin";
        }
};

#endif