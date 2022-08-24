#include "uml/parsers/parserMetaData.h"
#include <algorithm>
#include "uml/uml-stable.h"

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
        delete data->postProcessFlag[el.getID()];
    }
    data->postProcessFlag.erase(el.getID());
}

void applyFunctor(ParserMetaData& data, ID relEl, AbstractPostProcessFunctor* functor) {
    if (data.elements.count(relEl)) {
        (*functor)(data.elements.get(relEl));
        delete functor;
    } else if (data.m_manager->count(relEl)) {
        (*functor)(data.m_manager->get<>(relEl));
        delete functor;
    } else {
        if (!data.postProcessFlag.count(relEl)) {
            data.postProcessFlag[relEl] = new vector<AbstractPostProcessFunctor*>;
        }
        data.postProcessFlag[relEl]->push_back(functor);
    }
}

}
}