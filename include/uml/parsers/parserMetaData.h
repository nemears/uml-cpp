#ifndef PARSER_METADATA_H
#define PARSER_METADATA_H

#include "yaml-cpp/yaml.h"
#include "uml/sequence.h"
#include <unordered_map>
#include <filesystem>

namespace UML{

    class UmlManager;

    namespace Parsers {

        enum class ParserStrategy {
            WHOLE,
            INDIVIDUAL
        };

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
                class ElementsFunctor : public TemplateAbstractSequenceFunctor<Element,Element> {
                    private:
                        ParserMetaData* data;
                    public:
                        ElementsFunctor(Element* me, ParserMetaData* data) : TemplateAbstractSequenceFunctor(me) {
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
                ParserStrategy m_strategy = ParserStrategy::WHOLE;
        };

        void applyFunctor(ParserMetaData& data, ID relEl, AbstractPostProcessFunctor* functor);
    }
}

#endif