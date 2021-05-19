#include "uml/parsers/parserMetaData.h"
#include <algorithm>

using namespace std;

namespace UML {
namespace Parsers {

void ParserMetaData::ElementsFunctor::operator()(Element& el) const {
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

ParserMetaData::ParserMetaData() {
    elements.addProcedures.push_back(new ElementsFunctor(0, this));
}

void applyFunctor(ParserMetaData& data, boost::uuids::uuid relEl, AbstractPostProcessFunctor* functor) {
    if (data.elements.count(relEl)) {
        (*functor)(*data.elements.get(relEl));
    } else {
        if (!data.postProcessFlag.count(relEl)) {
            data.postProcessFlag[relEl] = new vector<AbstractPostProcessFunctor*>;
        }
        data.postProcessFlag[relEl]->push_back(functor);
    }
}

}
}