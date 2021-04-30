#ifndef NAMEDELEMENTH
#define NAMEDELEMENTH

#include <string>
#include "element.h"

namespace UML{

    template <class T = Element> class Sequence;
    class Namespace;
    class AbstractSequenceFunctor;

    /**
     * A NamedElement is an Element in a model that may have a name
     **/
    class NamedElement : virtual public Element {
        protected:
            std::string m_name;
            Namespace* m_namespace;
            Sequence<Namespace>* m_memberNamespace;
            virtual void reindexName(std::string oldName, std::string newName);
            void reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) override;
        public:
            NamedElement();
            ~NamedElement();
            NamedElement(const NamedElement& el);
            virtual std::string getName();
            virtual void setName(const std::string &name);
            Namespace* getNamespace();
            void setNamespace(Namespace* nmspc);
            Sequence<Namespace>& getMemberNamespace();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif