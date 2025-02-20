#pragma once

#include "egm/egm-basic.h"

namespace UML {
    template <class>
    class Namespace;

    template <class>
    class Type;

    template <class>
    class RedefinableElement;

    template <class>
    class Generalization;

    template <class>
    class Feature;

    template <class>
    class Property;

    template <class>
    class NamedElement;

    template <class ManagerPolicy>
    class Classifier : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<Classifier, EGM::TemplateTypeList<Namespace, Type, RedefinableElement>>;
        protected:
            struct GeneralizationsPolicy {
                template <class G>
                void elementAdded(G& el, Classifier& me) {
                    if (el.getGeneral() && !me.getGenerals().contains(el.getGeneral())) {
                        me.getGenerals().add(el.getGeneral());
                    }
                }
                template <class GeneralizationImpl>
                void elementRemoved(GeneralizationImpl& el, Classifier& me) {
                    if (el.getGeneral() && me.getGenerals().contains(el.getGeneral())) {
                        me.getGenerals().remove(el.getGeneral());
                    }
                }
            };
            using GeneralizationsSet = EGM::Set<Generalization, Classifier, GeneralizationsPolicy>;
            struct GeneralPolicy {
                void elementAdded(Classifier& el, Classifier& me) {
                    for (auto& generalization : me.getGeneralizations()) {
                        if (*generalization.getGeneral() == el) {
                            return;
                        }
                    }
                    constexpr std::size_t generalization_type = ManagerPolicy::manager::template ElementType<Generalization>::result; 
                    EGM::ManagedPtr<typename GeneralizationsSet::ManagedType> generalization = me.m_manager.create(generalization_type);
                    generalization->setGeneral(el);
                    me.getGeneralizations().add(generalization);
                }
                void elementRemoved(Classifier& el, Classifier& me) {
                    std::vector<EGM::ManagedPtr<typename GeneralizationsSet::ManagedType>> generalizations_to_erase;
                    for (auto& generalization : me.getGeneralizations()) {
                        if (generalization.getGeneral() && *generalization.getGeneral() == el) {
                            generalizations_to_erase.emplace_back(&generalization);
                        }
                    }
                    for (auto generalization : generalizations_to_erase) {
                        me.m_manager.erase(*generalization);
                    }
                }
            };
            using GeneralsSet = EGM::Set<Classifier, Classifier, GeneralPolicy>;
            using FeaturesSet = EGM::ReadOnlySet<Feature, Classifier>;
            using AttributesSet = EGM::ReadOnlySet<Property, Classifier>;
            using InheritedMembersSet = EGM::ReadOnlySet<NamedElement, Classifier>;
            GeneralizationsSet m_generalizations = GeneralizationsSet(this);
            GeneralsSet m_generals = GeneralsSet(this);
            FeaturesSet m_features = FeaturesSet(this);
            AttributesSet m_attributes = AttributesSet(this);
            InheritedMembersSet m_inheritedMembers = InheritedMembersSet(this);
        private:
            void init() {
                m_generalizations.subsets(ManagerPolicy::m_ownedElements);
                m_generalizations.opposite(&GeneralizationsSet::ManagedType::getSpecificSingleton);
                m_features.subsets(ManagerPolicy::m_ownedMembers);
                m_features.opposite(&FeaturesSet::ManagedType::getFeaturingClassifierSingleton);
                m_attributes.subsets(m_features);
                m_inheritedMembers.subsets(ManagerPolicy::m_members);
            }
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(Classifier);
            GeneralizationsSet& getGeneralizations() {
                return m_generalizations;
            }
            GeneralsSet& getGenerals() { return m_generals; }
            FeaturesSet& getFeatures() { return m_features; }
            AttributesSet& getAttributes() { return m_attributes; }
            InheritedMembersSet& getInheritedMembers() { return m_inheritedMembers; }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::Classifier> {
        static const bool abstract = true;
        static std::string name() { return "Classifier"; }
        template <class Policy>
        static SetList sets(UML::Classifier<Policy>& el) {
            return SetList {
                make_set_pair("generalizations", el.getGeneralizations()),
                make_set_pair("generals", el.getGenerals()),
                make_set_pair("features", el.getFeatures()),
                make_set_pair("attributes", el.getAttributes()),
                make_set_pair("inheritedMembers", el.getInheritedMembers())
            };
        }
    };
}
