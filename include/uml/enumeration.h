#ifndef ENUMERATION_H
#define ENUMERATION_H

#include "dataType.h"

namespace UML {

    class EnumerationLiteral;

    class Enumeration : public DataType {

        friend class UmlManager;
        friend class EnumerationLiteral;

        protected:
            OrderedSet<EnumerationLiteral, Enumeration> m_ownedLiterals = OrderedSet<EnumerationLiteral, Enumeration>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Set<EnumerationLiteral, Enumeration>& getOwnedLiteralsSet();
            void init();
            void copy(const Enumeration& rhs);
            Enumeration();
        public:
            Enumeration(const Enumeration& enumeration);
            OrderedSet<EnumerationLiteral, Enumeration>& getOwnedLiterals();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ENUMERATION;
            };
    };
}

#endif