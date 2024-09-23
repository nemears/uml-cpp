#pragma once

#include "packageableElement.h"
#include "uml/set/set.h"

namespace UML {
    
    class ValueSpecification;
    typedef UmlPtr<ValueSpecification> ValueSpecificationPtr;

    class Constraint : public PackageableElement
    {

        friend class Namespace;
        friend struct ElementInfo<Constraint>;

    protected:
        Singleton<Namespace, Constraint> m_context = Singleton<Namespace, Constraint>(this);
        Set<Element, Constraint> m_constrainedElements = Set<Element, Constraint>(this);
        Singleton<ValueSpecification, Constraint> m_specification = Singleton<ValueSpecification, Constraint>(this);
        Singleton<Namespace, Constraint>& getContextSingleton();
    public:
        Constraint(std::size_t elementType, AbstractManager& manager);
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
        typedef TypeInfo<std::tuple<PackageableElement>, Constraint> Info;
    };

    template <>
    struct ElementInfo<Constraint> : public DefaultInfo {
        static const bool abstract = false;
        static constexpr std::string_view name{"Constraint"};
        static SetList sets(Constraint&  el) {
            return SetList{
                std::make_pair<std::string, AbstractSet*>("context", &el.m_context),
                std::make_pair<std::string, AbstractSet*>("constrainedElements", &el.m_constrainedElements),
                std::make_pair<std::string, AbstractSet*>("specification", &el.m_specification)
            };
        };
    };
}
