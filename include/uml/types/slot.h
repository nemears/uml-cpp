#pragma once

#include "uml/set/singleton.h"
#include "uml/set/orderedSet.h"

namespace UML {

    class InstanceSpecification;
    class StructuralFeature;
    class ValueSpecification;
    typedef UmlPtr<StructuralFeature> StructuralFeaturePtr;
    typedef UmlPtr<InstanceSpecification> InstanceSpecificationPtr;

    class Slot : public Element {
        
        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class InstanceSpecification;
        
        protected:
            CustomSingleton<StructuralFeature, Slot> m_definingFeature = CustomSingleton<StructuralFeature, Slot>(this);
            CustomOrderedSet<ValueSpecification, Slot> m_values = CustomOrderedSet<ValueSpecification, Slot>(this);
            CustomSingleton<InstanceSpecification, Slot> m_owningInstance = CustomSingleton<InstanceSpecification, Slot>(this);
            void referenceErased(ID id) override;
            TypedSet<StructuralFeature, Slot>& getDefiningFeatureSingleton();
            TypedSet<InstanceSpecification, Slot>& getOwningInstanceSingleton();
            Slot();
        public:
            virtual ~Slot();
            OrderedSet<ValueSpecification, Slot>& getValues();
            StructuralFeaturePtr getDefiningFeature() const;
            void setDefiningFeature(StructuralFeature& definingFeature);
            void setDefiningFeature(StructuralFeature* definingFeature);
            void setDefiningFeature(ID id);
            InstanceSpecificationPtr getOwningInstance() const;
            void setOwningInstance(InstanceSpecification& inst);
            void setOwningInstance(InstanceSpecification* inst);
            void setOwningInstance(ID id);
            class NullDefiningFeatureException : public std::exception {
                public:
                    virtual const char* what() const throw() {
                        return "Tried to assign value to slot without setting definingFeature";
                    }
            } nullDefiningFeatureException;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::SLOT;
            };
    };
}