#pragma once

#include "element.h"
#include "uml/set/singleton.h"

namespace UML {

    template <class>
    class TemplateParameter;
    template <class>
    class TemplateParameterSubstitution;

    // typedef UmlPtr<TemplateParameter> TemplateParameterPtr;

    template <class ManagerPolicy>
    class ParameterableElement : public ManagerPolicy {

        template <class>
        friend class TemplateParameter;
        template <class>
        friend class TemplateParameterSubstitution;
        friend struct ElementInfo<ParameterableElement>;

        using ManagedTemplateParameter = TemplateParameter<typename ManagerPolicy::Manager::template GenBaseHierarchy<TemplateParameter>>;
        using TemplateParameterPtr = UmlPtr<ManagedTemplateParameter>;

        protected:
            Singleton<TemplateParameter, ParameterableElement> m_templateParameter = Singleton<TemplateParameter, ParameterableElement>(this);
            Singleton<TemplateParameter, ParameterableElement> m_owningTemplateParameter = Singleton<TemplateParameter, ParameterableElement>(this);
            Singleton<TemplateParameter, ParameterableElement>& getOwningTemplateParameterSingleton() {
                return m_owningTemplateParameter;
            }
            Singleton<TemplateParameter, ParameterableElement>& getTemplateParameterSingleton() {
                return m_templateParameter;
            }
            ParameterableElement(std::size_t elementType, AbstractManager& manager) : ManagerPolicy(elementType, manager) {
                m_templateParameter.opposite(&ManagedTemplateParameter::getParameteredElementSingleton);
                m_owningTemplateParameter.subsets(m_templateParameter);
                m_owningTemplateParameter.subsets(ManagerPolicy::m_owner);
                m_owningTemplateParameter.opposite(&ManagedTemplateParameter::getOwnedParameteredElementSingleton);
 
            }
        public:
            TemplateParameterPtr getOwningTemplateParameter() const {
                return m_owningTemplateParameter.get();
            }
            void setOwningTemplateParameter(TemplateParameterPtr parameter) {
                m_owningTemplateParameter.set(parameter);
            }
            void setOwningTemplateParameter(ManagedTemplateParameter& parameter) {
                m_owningTemplateParameter.set(parameter);
            }
            void setOwningTemplateParameter(ID id) {
                m_owningTemplateParameter.set(id);
            }
            TemplateParameterPtr getTemplateParameter() const {
                return m_templateParameter.get();
            }
            void setTemplateParameter(TemplateParameterPtr parameter) {
                m_templateParameter.set(parameter);
            }
            void setTemplateParameter(ManagedTemplateParameter& parameter) {
                m_templateParameter.set(parameter);
            }
            void setTemplateParameter(ID id) {
                m_templateParameter.set(id);
            }
            typedef TypeInfo<TemplateTypeList<Element>, ParameterableElement> Info;
    };

    template<class ManagerPolicy>
    struct ElementInfo<ParameterableElement<ManagerPolicy>> : public DefaultInfo {
        using ManagedType = ParameterableElement<ManagerPolicy>;
        static std::string name(__attribute__((unused)) AbstractElement& el) { return "ParameterableElement"; }
        static SetList sets(ManagedType& el) {
            return SetList {
                makeSetPair("templateParameter", el.m_templateParameter),
                makeSetPair("owningTemplateParameter", el.m_owningTemplateParameter)
            };
        }
    };
}
