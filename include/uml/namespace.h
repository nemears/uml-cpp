#ifndef _UML_NAMESPACE_H_
#define _UML_NAMESPACE_H_

#include <iostream>
#include "namedElement.h"

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
            void reindexName(ID id, std::string newName) override;
            void referenceErased(ID id) override;
            void init();
            Namespace();
        public:
            virtual ~Namespace();
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