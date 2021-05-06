#ifndef PARSER_METADATA_H
#define PARSER_METADATA_H

#include "yaml-cpp/yaml.h"
#include "uml/sequence.h"
#include <map>
#include <filesystem>

namespace UML{
    namespace Parsers {

        class AbstractPostProcessFunctor {
            protected:
                Element* m_el;
                YAML::Node m_node;
            public:
                AbstractPostProcessFunctor(Element* el, YAML::Node node) {
                    m_el = el;
                    m_node = node;
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
                std::filesystem::path m_path;
                ParserMetaData();
                Sequence<> elements;
                std::map<boost::uuids::uuid, std::vector<AbstractPostProcessFunctor*>*> postProcessFlag;
        };

        void applyFunctor(ParserMetaData& data, boost::uuids::uuid relEl, AbstractPostProcessFunctor* functor);
    }
}

#endif