#ifndef NAMEDELEMENTH
#define NAMEDELEMENTH

#include <string>
#include "element.h"

namespace UML{

    template <class T> class Sequence;
    class Namespace;
    template <class T> class AbstractSequenceFunctor;
    class UmlManager;

    enum class VisibilityKind {
        PUBLIC,
        PROTECTED,
        PRIVATE,
        PACKAGE
    };

    /**
     * A NamedElement is an Element in a model that may have a name
     **/
    class NamedElement : virtual public Element {
        friend class UmlManager;
        friend class Namespace;
        protected:
            std::string m_name;
            ID m_namespaceID;
            Namespace* m_namespacePtr;
            Sequence<Namespace>* m_memberNamespace;
            // visibility defaults to public, don't think there is a none value
            VisibilityKind m_visibility = VisibilityKind::PUBLIC;
            virtual void reindexName(std::string oldName, std::string newName);
            void reindexID(ID oldID, ID newID) override;
            void setManager(UmlManager* manager) override;
            void restoreReleased(ID id, Element* released) override;
            void referencingReleased(ID id) override;
        public:
            NamedElement();
            virtual ~NamedElement();
            NamedElement(const NamedElement& el);
            virtual std::string getName();
            virtual void setName(const std::string &name);
            Namespace* getNamespace();
            void setNamespace(Namespace* nmspc);
            Sequence<Namespace>& getMemberNamespace();
            VisibilityKind getVisibility();
            void setVisibility(VisibilityKind visibility);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::NAMED_ELEMENT;
            };
    };
}

#endif