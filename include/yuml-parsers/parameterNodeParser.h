#ifndef PARAMETERNODEPARSER_H
#define PARAMETERNODEPARSER_H

#include "objectNodeParser.h"
#include "uml/parameterNode.h"

class ParameterNodeParser : public ObjectNodeParser {
    protected:
        bool parseFeatures(YAML::Node node, Element* el) override;
    public:
        Element* createElement() override;
        bool emit(YAML::Emitter& emitter, Element* el) override;
        static ParameterNodeParser createNewParser();
        ParameterNodeParser(map<boost::uuids::uuid, Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) :
            ObjectNodeParser(elements, postParsers),
            NamedElementParser(elements, postParsers)
            {
                keyword = "parameterNode";
            };
        static void setParameterLater(Element* parameterNode, Element* parameter) {
            dynamic_cast<ParameterNode*>(parameterNode)->setParameter(dynamic_cast<Parameter*>(parameter));
        };
};

#endif