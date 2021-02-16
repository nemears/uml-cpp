#ifndef TYPEDELEMENTPARSER
#define TYPEDELEMENTPARSER
#include "namedElementParser.h"
#include "uml/element.h"
#include "uml/typedElement.h"
#include "uml/primitiveType.h"

class TypedElementParser : virtual public NamedElementParser {
    protected:
        bool parseFeatures(YAML::Node, Element* el) override;
        bool parseTypeFeatures(YAML::Node, Element* el);
        bool emitType(YAML::Emitter& emitter, Element* el);

    public:
        TypedElementParser(map<boost::uuids::uuid, Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) : 
            NamedElementParser(elements, postParsers){
            // this->elements = elements;
        };

        bool emit(YAML::Emitter& emitter, Element* el) override;
};
#endif