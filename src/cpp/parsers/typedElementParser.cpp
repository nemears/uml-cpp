#include "headers/typedElementParser.h"

bool TypedElementParser::parseTag(InputParser * input, Element * el) {
    if (input->firstWord.compare("type") == 0) {

        // TODO check if overiding

        boost::uuids::uuid typeId = boost::lexical_cast<boost::uuids::uuid>(input->getTag());

        Type* type = (Type*)(*elements)[typeId];

        ((TypedElement*)el)->setType(type);

        return true;
    }

    return NamedElementParser::parseTag(input, el);
}