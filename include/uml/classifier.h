#ifndef _UML_CLASSIFIER_H_
#define _UML_CLASSIFIER_H_

#include "namespace.h"
#include "type.h"
#include "redefinableElement.h"
#include "templateableElement.h"
#include "property.h"
#include "generalization.h"
#include "generalizationSet.h"
#include "redefinableTemplateSignature.h"
#include "classifierTemplateParameter.h"

namespace UML {

    class InstanceSpecification;

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
            class AddGeneralizationPolicy {
                public:
                    void apply(Generalization& el, Classifier& m_el);
            };
            class RemoveGeneralizationPolicy {
                public:
                    void apply(Generalization& el, Classifier& m_el);
            };
            class AddGeneralPolicy {
                public:
                    void apply(Classifier& el, Classifier& m_el);
            };
            class RemoveGeneralPolicy {
                public:
                    void apply(Classifier& el, Classifier& m_el);
            };
            class AddOwnedMemberPolicy {
                public:
                    void apply(NamedElement& el, Classifier& me);
            };
            class RemoveOwnedMemberPolicy {
                public:
                    void apply(NamedElement& el, Classifier& me);
            };
            CustomSet<Feature, Classifier> m_features = CustomSet<Feature, Classifier>(this);
            CustomSet<Property, Classifier> m_attributes = CustomSet<Property, Classifier>(this);
            CustomSet<Generalization, Classifier, AddGeneralizationPolicy, RemoveGeneralizationPolicy> m_generalizations = CustomSet<Generalization, Classifier, AddGeneralizationPolicy, RemoveGeneralizationPolicy>(this);
            CustomSet<Classifier, Classifier, AddGeneralPolicy, RemoveGeneralPolicy> m_generals = CustomSet<Classifier, Classifier, AddGeneralPolicy, RemoveGeneralPolicy>(this);
            CustomSet<NamedElement, Classifier> m_inheritedMembers = CustomSet<NamedElement, Classifier>(this);
            CustomSet<GeneralizationSet, Classifier> m_powerTypeExtent = CustomSet<GeneralizationSet, Classifier>(this);
            CustomSingleton<RedefinableTemplateSignature, Classifier> m_classifierOwnedTemplateSignature = CustomSingleton<RedefinableTemplateSignature, Classifier>(this);
            CustomSingleton<ClassifierTemplateParameter, Classifier> m_classifierTemplateParameter = CustomSingleton<ClassifierTemplateParameter, Classifier>(this);
            CustomSet<NamedElement, Classifier, AddOwnedMemberPolicy, RemoveOwnedMemberPolicy> m_classifierOwnedMembers = CustomSet<NamedElement, Classifier, AddOwnedMemberPolicy, RemoveOwnedMemberPolicy>(this);
            TypedSet<RedefinableTemplateSignature, Classifier>& getOwnedTemplateSignatureSingleton();
            TypedSet<ClassifierTemplateParameter, Classifier>& getTemplateParameterSingleton();
            void referenceReindexed(ID newID) override;
            void restoreReferences() override;
            void restoreReference(Element* el) override;
            void referenceErased(ID id) override;
            Classifier();
        public:
            virtual ~Classifier();
            std::string getName() override;
            void setName(const std::string& name) override;
            Set<Feature, Classifier>& getFeatures();
            Set<Property, Classifier>& getAttributes();
            Set<Generalization, Classifier>& getGeneralizations();
            Set<Classifier, Classifier>& getGenerals();
            Set<NamedElement, Classifier>& getInheritedMembers();
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
            bool isSubClassOf(ElementType eType) const override;
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
#endif