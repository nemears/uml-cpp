#ifndef _UML_CONSTRAINT_H_
#define _UML_CONSTRAINT_H_

#include "packageableElement.h"
#include "valueSpecification.h"
#include "uml/set/set.h"

namespace UML {

    typedef UmlPtr<ValueSpecification> ValueSpecificationPtr;

    class Constraint : public PackageableElement
    {

        friend class Namespace;
        template <typename SerializationPolicy, typename PersistencePolicy>
        friend class Manager;

    protected:
        CustomSingleton<Namespace, Constraint> m_context = CustomSingleton<Namespace, Constraint>(this);
        CustomSet<Element, Constraint> m_constrainedElements = CustomSet<Element, Constraint>(this);
        CustomSingleton<ValueSpecification, Constraint> m_specification = CustomSingleton<ValueSpecification, Constraint>(this);
        TypedSet<Namespace, Constraint> &getContextSingleton();
        void referenceReindexed(ID newID) override;
        void referenceErased(ID id) override;
        Constraint();

    public:
        virtual ~Constraint();
        NamespacePtr getContext() const;
        void setContext(Namespace *context);
        void setContext(Namespace &context);
        void setContext(NamespacePtr context);
        void setContext(ID id);
        Set<Element, Constraint> &getConstrainedElements();
        ValueSpecificationPtr getSpecification() const;
        void setSpecification(ValueSpecification *specification);
        void setSpecification(ValueSpecification &specification);
        void setSpecification(ID id);
        bool isSubClassOf(ElementType eType) const override;
        static ElementType elementType()
        {
            return ElementType::CONSTRAINT;
        }
    };
}

#endif