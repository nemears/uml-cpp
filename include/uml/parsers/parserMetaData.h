#ifndef PARSER_METADATA_H
#define PARSER_METADATA_H

#include "uml/sequence.h"
#include <map>

namespace UML{
    namespace Parsers {

        class AbstractPostProcessFunctor {
            protected:
                Element* m_el;
            public:
                AbstractPostProcessFunctor(Element* el) {
                    m_el = el;
                };
                virtual void operator()(Element& el) const = 0;
        };

        class ParserMetaData {
            protected:
                class ElementsFunctor : public AbstractSequenceFunctor {
                    private:
                        ParserMetaData* data;
                    public:
                        ElementsFunctor(Element* me, ParserMetaData* data) : AbstractSequenceFunctor(me) {
                            this->data = data;
                        };
                        void operator()(Element& el) const override;
                };
            public:
                ParserMetaData();
                Sequence<> elements;
                map<boost::uuids::uuid, vector<AbstractPostProcessFunctor*>*> postProcessFlag;
        };
    }
}

#endif