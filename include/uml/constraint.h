#ifndef _UML_CONSTRAINT_H_
#define _UML_CONSTRAINT_H_

#include "packageableElement.h"
#include "valueSpecification.h"

namespace UML {

    // class ValueSpecification;

    class Constraint : public PackageableElement {

        friend class Namespace;
        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            Singleton<Namespace, Constraint> m_context = Singleton<Namespace, Constraint>(this);
            Set<Element, Constraint> m_constrainedElements = Set<Element, Constraint>(this);
            Set<ValueSpecification, Constraint> m_specifications = Set<ValueSpecification, Constraint>(this);
            Set<Namespace, Constraint>& getContextSingleton();
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void reindexName(ID id, std::string newName) override;
            void referenceErased(ID id) override;
            void init();
            Constraint();
        public:
            virtual ~Constraint();
            NamespacePtr getContext() const;
            void setContext(Namespace* context);
            void setContext(Namespace& context);
            void setContext(NamespacePtr context);
            void setContext(ID id);
            Set<Element, Constraint>& getConstrainedElements();
            Set<ValueSpecification, Constraint>& getSpecifications();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::CONSTRAINT;
            }
    };
}

#endif