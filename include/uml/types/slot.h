#pragma once

#include "uml/set/indexableSet.h"
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
            Singleton<StructuralFeature, Slot> m_definingFeature = Singleton<StructuralFeature, Slot>(this);
            IndexableOrderedSet<ValueSpecification, Slot> m_values = IndexableOrderedSet<ValueSpecification, Slot>(this);
            Singleton<InstanceSpecification, Slot> m_owningInstance = Singleton<InstanceSpecification, Slot>(this);
            void referenceErased(ID id) override;
            Singleton<StructuralFeature, Slot>& getDefiningFeatureSingleton();
            Singleton<InstanceSpecification, Slot>& getOwningInstanceSingleton();
            Slot();
        public:
            virtual ~Slot();
            IndexableOrderedSet<ValueSpecification, Slot>& getValues();
            StructuralFeaturePtr getDefiningFeature() const;
            void setDefiningFeature(StructuralFeature& definingFeature);
            void setDefiningFeature(StructuralFeaturePtr definingFeature);
            void setDefiningFeature(ID id);
            InstanceSpecificationPtr getOwningInstance() const;
            void setOwningInstance(InstanceSpecification& inst);
            void setOwningInstance(InstanceSpecificationPtr inst);
            void setOwningInstance(ID id);
            class NullDefiningFeatureException : public std::exception {
                public:
                    virtual const char* what() const throw() {
                        return "Tried to assign value to slot without setting definingFeature";
                    }
            } nullDefiningFeatureException;
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::SLOT;
            };
    };
}
