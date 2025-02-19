#pragma once

#include "egm/egm-basic.h"

namespace UML {
    template <class>
    class DirectedRelationship;

    template <class>
    class Classifier;

    template <class ManagerPolicy>
    class Generalization : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<Generalization, EGM::TemplateTypeList<DirectedRelationship>>;
        protected:
            struct GeneralPolicy;
            using GeneralSingleton = EGM::Singleton<Classifier, Generalization, GeneralPolicy>;
            using SpecificSingleton = EGM::Singleton<Classifier, Generalization>;
            GeneralSingleton m_general = GeneralSingleton(this);
            SpecificSingleton m_specific = SpecificSingleton(this);
        private:
            void init() {
                m_general.subsets(ManagerPolicy::m_targets);
                m_specific.subsets(ManagerPolicy::m_owner);
                m_specific.subsets(ManagerPolicy::m_sources);
                m_specific.opposite(&GeneralSingleton::ManagedType::getGeneralizations);
            }
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(Generalization);
            GeneralSingleton& getGeneralSingleton() { return m_general; }
            using ClassifierImpl = typename GeneralSingleton::ManagedType;
            using ClassifierPtr = EGM::ManagedPtr<ClassifierImpl>;
            ClassifierPtr getGeneral() { return m_general.get(); }
            void setGeneral(ClassifierImpl& el) { m_general.set(el); }
            void setGeneral(ClassifierPtr ptr) { m_general.set(ptr); }
            void setGeneral(EGM::ID id) { m_general.set(id); }

            SpecificSingleton& getSpecificSingleton() { return m_specific; }
            ClassifierPtr getSpecific() { return m_specific.get(); }
            void setSpecific(ClassifierImpl& el) { m_specific.set(el); }
            void setSpecific(ClassifierPtr ptr) { m_specific.set(ptr); }
            void setSpecific(EGM::ID id) { m_specific.set(id); }
        protected:
            using ClassifierImplNP = typename EGM::Singleton<Classifier, Generalization>::ManagedType;
            struct GeneralPolicy {
                template <class Clazz>
                void elementAdded(Clazz& el, Generalization& me) {
                    if (me.getSpecific() && !me.getSpecific()->getGenerals().contains(el)) {
                        me.getSpecific()->getGenerals().add(el);
                    }
                }
                template <class Clazz>
                void elementRemoved(Clazz& el, Generalization& me) {
                    if (me.getSpecific() && me.getSpecific()->getGenerals().contains(el)) {
                        me.getSpecific()->getGenerals().remove(el);
                    }
                }
            };

    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::Generalization> {
        static std::string name() { return "Generalization"; }
        template <class Policy>
        static SetList sets(UML::Generalization<Policy>& el) {
            return SetList {
                make_set_pair("general", el.getGeneralSingleton()),
                make_set_pair("specific", el.getSpecificSingleton())
            };
        }
    };
}
