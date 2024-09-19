#pragma once

#include "namespace.h"
#include "type.h"
#include "redefinableElement.h"
#include "templateableElement.h"
#include "uml/set/indexableSet.h"
#include "uml/types/element.h"
#include "uml/types/namedElement.h"

namespace UML {

    class Generalization;
    class Feature;
    class RedefinableTemplateSignature;
    class ClassifierTemplateParameter;
    class Property;
    class GeneralizationSet;

    typedef UmlPtr<RedefinableTemplateSignature> RedefinableTemplateSignaturePtr;
    typedef UmlPtr<ClassifierTemplateParameter> ClassifierTemplateParameterPtr;

    /**
     * A Classifier represents a classification of instances according to their Features
     **/
    class Classifier: public Namespace, public Type , public RedefinableElement, public TemplateableElement {
        
        friend class InstanceSpecification;
        friend class Generalization;
        friend class NamedElement;
        friend class RedefinableTemplateSignature;
        friend class ClassifierTemplateParameter;
        friend class ElementInfo<Classifier>;

        protected:
            class GeneralizationPolicy : public IndexablePolicy {
                protected:
                    void elementAdded(Generalization& el, Classifier& me);
                    void elementRemoved(Generalization& el, Classifier& me);
            };
            class GeneralPolicy : public IndexablePolicy {
                protected:
                    void elementAdded(Classifier& el, Classifier& me);
                    void elementRemoved(Classifier& el, Classifier& me);
            };
            class OwnedMemberPolicy : public IndexablePolicy {
                protected:
                    void elementAdded(NamedElement& el, Classifier& me);
                    void elementRemoved(NamedElement& el, Classifier& me);
            };
            ReadOnlyIndexableSet<Feature, Classifier> m_features = ReadOnlyIndexableSet<Feature, Classifier>(this);
            ReadOnlyIndexableSet<Property, Classifier> m_attributes = ReadOnlyIndexableSet<Property, Classifier>(this);
            Set<Generalization, Classifier, GeneralizationPolicy> m_generalizations = Set<Generalization, Classifier, GeneralizationPolicy>(this);
            IndexableSet<Classifier, Classifier, GeneralPolicy> m_generals = IndexableSet<Classifier, Classifier, GeneralPolicy>(this);
            ReadOnlyIndexableSet<NamedElement, Classifier> m_inheritedMembers = ReadOnlyIndexableSet<NamedElement, Classifier>(this);
            Set<GeneralizationSet, Classifier> m_powerTypeExtent = Set<GeneralizationSet, Classifier>(this);
            Singleton<RedefinableTemplateSignature, Classifier> m_classifierOwnedTemplateSignature = Singleton<RedefinableTemplateSignature, Classifier>(this);
            Singleton<ClassifierTemplateParameter, Classifier> m_classifierTemplateParameter = Singleton<ClassifierTemplateParameter, Classifier>(this);
            Set<NamedElement, Classifier, OwnedMemberPolicy> m_classifierOwnedMembers = Set<NamedElement, Classifier, OwnedMemberPolicy>(this);
            Singleton<RedefinableTemplateSignature, Classifier>& getOwnedTemplateSignatureSingleton();
            Singleton<ClassifierTemplateParameter, Classifier>& getTemplateParameterSingleton();
            Classifier(std::size_t elementType, AbstractManager& manager);
        public:
            std::string getName() override;
            void setName(const std::string& name) override;
            ReadOnlyIndexableSet<Feature, Classifier>& getFeatures();
            ReadOnlyIndexableSet<Property, Classifier>& getAttributes();
            Set<Generalization, Classifier, GeneralizationPolicy>& getGeneralizations();
            IndexableSet<Classifier, Classifier, GeneralPolicy>& getGenerals();
            ReadOnlyIndexableSet<NamedElement, Classifier>& getInheritedMembers();
            Set<GeneralizationSet, Classifier>& getPowerTypeExtent();
            RedefinableTemplateSignaturePtr getOwnedTemplateSignature() const;
            void setOwnedTemplateSignature(RedefinableTemplateSignaturePtr signature);
            void setOwnedTemplateSignature(RedefinableTemplateSignature& signature);
            void setOwnedTemplateSignature(ID id);
            ClassifierTemplateParameterPtr getTemplateParameter() const;
            void setTemplateParameter(ClassifierTemplateParameter& templateParameter);
            void setTemplateParameter(ClassifierTemplateParameterPtr templateParameter);
            void setTemplateParameter(ID id);
            typedef TypeInfo<std::tuple<Namespace, Type, RedefinableElement, TemplateableElement>, Classifier> Info;
    };

    template<>
    struct ElementInfo<Classifier> : public DefaultInfo {
        inline static std::string name {"Classifier"};
        static SetList sets(Classifier& el) {
            return SetList {
                makeSetPair("features", el.m_features),
                makeSetPair("attributes", el.m_attributes),
                makeSetPair("generalizations", el.m_generalizations),
                // makeSetPair("generals", el.m_generals),
                makeSetPair("inheritedMembers", el.m_inheritedMembers),
                makeSetPair("powerTypeExtent", el.m_powerTypeExtent),
                makeSetPair("ownedTemplateSignature", el.m_classifierOwnedTemplateSignature),
                makeSetPair("templateParameter", el.m_classifierTemplateParameter),
                makeSetPair("ownedMembers", el.m_classifierOwnedMembers)
            };
        }
    };

    //Exceptions

    class InvalidGeneralizationException: public std::exception {
        private:
            std::string msg;
        public:
            InvalidGeneralizationException(std::string uuid) :
                msg("Invalid Generalization (uuid: " + uuid + ") was added to generalization set, only the Specific classifier may hold generalizations!")
                {};
            virtual const char* what() const throw() {
                return msg.c_str();
            };
    };

    class NoManagerException : public std::exception {
        private:
            std::string m_msg;
        public:
            NoManagerException() :
                m_msg("Cannot add general without Manager, because it creates Generalization that can't be tracked!")
                {};
            virtual const char* what() const throw() {
                return m_msg.c_str();
            };
    };
}
