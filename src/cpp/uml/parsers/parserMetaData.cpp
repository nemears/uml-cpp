#include "uml/parsers/parserMetaData.h"
#include <algorithm>

using namespace UML;
using namespace Parsers;

void UML::Parsers::ParserMetaData::ElementsFunctor::operator()(Element& el) const {
    if (data->postProcessFlag.count(el.getID())) {
        for (auto const& func: *(data->postProcessFlag[el.getID()])) {
            (*func)(el);
        }
        for (auto const& func: *(data->postProcessFlag[el.getID()])) {
            delete func;
        }
    }
    data->postProcessFlag.erase(el.getID());
}

UML::Parsers::ParserMetaData::ParserMetaData() {
    elements.addProcedures.push_back(new ElementsFunctor(0, this));
}