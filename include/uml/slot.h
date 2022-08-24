#ifndef _UML_SLOT_H_
#define _UML_SLOT_H_

#include "uml/set.h"
#include "uml/singleton.h"

namespace UML {

    class InstanceSpecification;
    class StructuralFeature;
    class ValueSpecification;
    typedef UmlPtr<StructuralFeature> StructuralFeaturePtr;
    typedef UmlPtr<InstanceSpecification> InstanceSpecificationPtr;

    class Slot : public Element {
        
        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;
        friend class InstanceSpecification;
        
        protected:
            Singleton<StructuralFeature, Slot> m_definingFeature = Singleton<StructuralFeature, Slot>(this);
            Set<ValueSpecification, Slot> m_values = Set<ValueSpecification, Slot>(this);
            Singleton<InstanceSpecification, Slot> m_owningInstance = Singleton<InstanceSpecification, Slot>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void referenceErased(ID id) override;
            void restoreReference(Element* el) override;
            Set<StructuralFeature, Slot>& getDefiningFeatureSingleton();
            Set<InstanceSpecification, Slot>& getOwningInstanceSingleton();
            void init();
            Slot();
        public:
            virtual ~Slot();
            Set<ValueSpecification, Slot>& getValues();
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

#endif