#pragma once 

#include "egm/egm-basic.h"

namespace UML {
    template <class>
    class StructuralFeature;

    template <class ManagerPolicy>
    class Property : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<Property, EGM::TemplateTypeList<StructuralFeature>>;
        protected:
            using ClassSingleton = EGM::Singleton<Class, Property>;
            ClassSingleton m_class = ClassSingleton(this);
        private:
            void init() {
                m_class.subsets(ManagerPolicy::m_featuringClassifier);
                m_class.subsets(ManagerPolicy::m_namespace);
                m_class.opposite(&ClassSingleton::ManagedType::getOwnedAttributes);
            }
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(Property);
            ClassSingleton& getClassSingleton() { return m_class; }
            using ClassImpl = typename ClassSingleton::ManagedType; 
            using ClassPtr = EGM::ManagedPtr<ClassImpl>;
            ClassPtr getClass() { return m_class.get(); }
            void setClass(ClassImpl& el) { m_class.set(el); }
            void setClass(ClassPtr el) { m_class.set(el); }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::Property> {
        static std::string name() { return "Property"; }
    };
    template <class Policy>
    static SetList sets(UML::Property<Policy>& el) {
        return SetList {
            make_set_pair("class", el.getClassSingleton())
        };
    }
}
