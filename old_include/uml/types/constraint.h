#pragma once
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
        Singleton<Namespace, Constraint> m_context = Singleton<Namespace, Constraint>(this);
        Set<Element, Constraint> m_constrainedElements = Set<Element, Constraint>(this);
        Singleton<ValueSpecification, Constraint> m_specification = Singleton<ValueSpecification, Constraint>(this);
        Singleton<Namespace, Constraint>& getContextSingleton();
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
