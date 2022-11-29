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
            CustomSingleton<Namespace, Constraint> m_context = CustomSingleton<Namespace, Constraint>(this);
            CustomSet<Element, Constraint> m_constrainedElements = CustomSet<Element, Constraint>(this);
            CustomSet<ValueSpecification, Constraint> m_specifications = CustomSet<ValueSpecification, Constraint>(this);
            TypedSet<Namespace, Constraint>& getContextSingleton();
            void referenceReindexed(ID newID) override;
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