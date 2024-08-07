#pragma once

#include "namespace.h"
#include "type.h"
#include "redefinableElement.h"
#include "templateableElement.h"
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

        protected:
            struct GeneralizationPolicy {
                void elementAdded(Generalization& el, Classifier& me);
                void elementRemoved(Generalization& el, Classifier& me);
            };
            struct GeneralPolicy {
                void elementAdded(Classifier& el, Classifier& me);
                void elementRemoved(Classifier& el, Classifier& me);
            };
            struct OwnedMemberPolicy {
                void elementAdded(NamedElement& el, Classifier& me);
                void elementRemoved(NamedElement& el, Classifier& me);
            };
            ReadOnlySet<Feature, Classifier> m_features = ReadOnlySet<Feature, Classifier>(this);
            ReadOnlySet<Property, Classifier> m_attributes = ReadOnlySet<Property, Classifier>(this);
            Set<Generalization, Classifier, GeneralizationPolicy> m_generalizations = Set<Generalization, Classifier, GeneralizationPolicy>(this);
            Set<Classifier, Classifier, GeneralPolicy> m_generals = Set<Classifier, Classifier, GeneralPolicy>(this);
            ReadOnlySet<NamedElement, Classifier> m_inheritedMembers = ReadOnlySet<NamedElement, Classifier>(this);
            Set<GeneralizationSet, Classifier> m_powerTypeExtent = Set<GeneralizationSet, Classifier>(this);
            Singleton<RedefinableTemplateSignature, Classifier> m_classifierOwnedTemplateSignature = Singleton<RedefinableTemplateSignature, Classifier>(this);
            Singleton<ClassifierTemplateParameter, Classifier> m_classifierTemplateParameter = Singleton<ClassifierTemplateParameter, Classifier>(this);
            Set<NamedElement, Classifier, OwnedMemberPolicy> m_classifierOwnedMembers = Set<NamedElement, Classifier, OwnedMemberPolicy>(this);
            Singleton<RedefinableTemplateSignature, Classifier>& getOwnedTemplateSignatureSingleton();
            Singleton<ClassifierTemplateParameter, Classifier>& getTemplateParameterSingleton();
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Classifier();
        public:
            virtual ~Classifier();
            std::string getName() override;
            void setName(const std::string& name) override;
            ReadOnlySet<Feature, Classifier>& getFeatures();
            ReadOnlySet<Property, Classifier>& getAttributes();
            Set<Generalization, Classifier, GeneralizationPolicy>& getGeneralizations();
            Set<Classifier, Classifier, GeneralPolicy>& getGenerals();
            ReadOnlySet<NamedElement, Classifier>& getInheritedMembers();
            Set<GeneralizationSet, Classifier>& getPowerTypeExtent();
            RedefinableTemplateSignaturePtr getOwnedTemplateSignature() const;
            void setOwnedTemplateSignature(RedefinableTemplateSignature* signature);
            void setOwnedTemplateSignature(RedefinableTemplateSignature& signature);
            void setOwnedTemplateSignature(RedefinableTemplateSignaturePtr signature);
            void setOwnedTemplateSignature(ID id);
            ClassifierTemplateParameterPtr getTemplateParameter() const;
            void setTemplateParameter(ClassifierTemplateParameter* templateParameter);
            void setTemplateParameter(ClassifierTemplateParameter& templateParameter);
            void setTemplateParameter(ClassifierTemplateParameterPtr templateParameter);
            void setTemplateParameter(ID id);
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::CLASSIFIER;
            };
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
