#ifndef SLOT_H
#define SLOT_H

#include "uml/set.h"
#include "uml/singleton.h"
#include "yaml-cpp/yaml.h"

namespace UML {

    class InstanceSpecification;
    class StructuralFeature;
    class ValueSpecification;
    namespace Parsers {
        void parseSlot(YAML::Node node, Slot& slot, Parsers::ParserMetaData& data);
    }

    class Slot : public Element {
        friend class UmlManager;
        friend class InstanceSpecification;
        friend void Parsers::parseSlot(YAML::Node node, Slot& slot, Parsers::ParserMetaData& data);
        protected:
            Singleton<StructuralFeature, Slot> m_definingFeature = Singleton<StructuralFeature, Slot>(this);
            Set<ValueSpecification, Slot> m_values = Set<ValueSpecification, Slot>(this);
            Singleton<InstanceSpecification, Slot> m_owningInstance = Singleton<InstanceSpecification, Slot>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Set<StructuralFeature, Slot>& getDefiningFeatureSingleton();
            Set<InstanceSpecification, Slot>& getOwningInstanceSingleton();
            void init();
            void copy(const Slot& rhs);
            Slot();
        public:
            Slot(const Slot& rhs);
            virtual ~Slot();
            Set<ValueSpecification, Slot>& getValues();
            StructuralFeature* getDefiningFeature();
            StructuralFeature& getDefiningFeatureRef();
            ID getDefiningFeatureID() const;
            bool hasDefiningFeature() const;
            void setDefiningFeature(StructuralFeature& definingFeature);
            void setDefiningFeature(StructuralFeature* definingFeature);
            InstanceSpecification* getOwningInstance();
            InstanceSpecification& getOwningInstanceRef();
            ID getOwningInstanceID() const;
            bool hasOwningInstance() const;
            void setOwningInstance(InstanceSpecification& inst);
            void setOwningInstance(InstanceSpecification* inst);
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