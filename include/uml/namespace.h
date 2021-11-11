#ifndef NAMESPACEH
#define NAMESPACEH

#include <iostream>
#include "namedElement.h"
#include "sequence.h"

namespace UML{
    /**
     * A Namespace is an Element in a model that contains a set of NamedElements that can be identified by name
     **/
    class Namespace : virtual public NamedElement {
        friend class UmlManager;
        protected:
            Set<NamedElement, Namespace> m_members = Set<NamedElement, Namespace>(this);
            Set<NamedElement, Namespace> m_ownedMembers = Set<NamedElement, Namespace>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Namespace();
        public:
            virtual ~Namespace();
            Namespace(const Namespace& nmspc);
            void setName(const std::string& name) override;
            Set<NamedElement, Namespace>& getMembers();
            Set<NamedElement, Namespace>& getOwnedMembers();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::NAMESPACE;
            };
    };
}
#endif