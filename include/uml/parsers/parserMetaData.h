#ifndef PARSER_METADATA_H
#define PARSER_METADATA_H

#include "yaml-cpp/yaml.h"
#include "uml/sequence.h"
#include <unordered_map>
#include <filesystem>

namespace UML{

    class UmlManager;

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
                ParserMetaData(UmlManager* manager);
                Sequence<> elements;
                std::unordered_map<ID, std::vector<AbstractPostProcessFunctor*>*> postProcessFlag;
                UmlManager* m_manager;
        };

        void applyFunctor(ParserMetaData& data, ID relEl, AbstractPostProcessFunctor* functor);
    }
}

#endif