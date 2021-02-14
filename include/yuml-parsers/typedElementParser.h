#ifndef TYPEDELEMENTPARSER
#define TYPEDELEMENTPARSER
#include "namedElementParser.h"
#include "uml/element.h"
#include "uml/typedElement.h"
#include "uml/primitiveType.h"

class TypedElementParser : virtual public NamedElementParser {
    protected:
        bool parseFeatures(YAML::Node, UML::Element* el) override;
        bool parseTypeFeatures(YAML::Node, UML::Element* el);

    public:
        TypedElementParser(map<boost::uuids::uuid, UML::Element*>* elements) : NamedElementParser(elements){
            // this->elements = elements;
        };

        bool emit(YAML::Emitter& emitter, Element* el) override;
};
#endif