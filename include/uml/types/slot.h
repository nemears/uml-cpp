#pragma once

#include "uml/set/indexableSet.h"
#include "uml/set/singleton.h"
#include "uml/set/orderedSet.h"
#include "uml/types/element.h"

namespace UML {

    class InstanceSpecification;
    class StructuralFeature;
    class ValueSpecification;
    typedef UmlPtr<StructuralFeature> StructuralFeaturePtr;
    typedef UmlPtr<InstanceSpecification> InstanceSpecificationPtr;

    class Slot : public Element {
        
        friend class InstanceSpecification;
        friend struct ElementInfo<Slot>;
        
        protected:
            Singleton<StructuralFeature, Slot> m_definingFeature = Singleton<StructuralFeature, Slot>(this);
            IndexableOrderedSet<ValueSpecification, Slot> m_values = IndexableOrderedSet<ValueSpecification, Slot>(this);
            Singleton<InstanceSpecification, Slot> m_owningInstance = Singleton<InstanceSpecification, Slot>(this);
            Singleton<StructuralFeature, Slot>& getDefiningFeatureSingleton();
            Singleton<InstanceSpecification, Slot>& getOwningInstanceSingleton();
        public:
            Slot(std::size_t elementType, AbstractManager& manager);
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
            typedef TypeInfo<std::tuple<Element>, Slot> Info;
    };

    template <>
    struct ElementInfo<Slot> : public DefaultInfo {
        static const bool abstract = false;
        inline static const std::string name {"Slot"};
        static SetList sets(Slot& slot) {
            return SetList {
                makeSetPair("definingFeature", slot.m_definingFeature),
                makeSetPair("values", slot.m_values),
                makeSetPair("owningInstance", slot.m_owningInstance)
            };
        }
    };
}
