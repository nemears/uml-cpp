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

namespace UML {

    class InstanceSpecification;

    typedef UmlPtr<RedefinableTemplateSignature> RedefinableTemplateSignaturePtr;

    /**
     * A Classifier represents a classification of instances according to their Features
     **/
    class Classifier: public Namespace, public Type , public RedefinableElement, public TemplateableElement {
        
        friend class UmlManager;
        friend class InstanceSpecification;
        friend class Generalization;
        friend class NamedElement;
        friend class RedefinableTemplateSignature;

        protected:
            Set<Feature, Classifier> m_features = Set<Feature, Classifier>(this);
            Set<Property, Classifier> m_attributes = Set<Property, Classifier>(this);
            Set<Generalization, Classifier> m_generalizations = Set<Generalization, Classifier>(this);
            Set<Classifier, Classifier> m_generals = Set<Classifier, Classifier>(this);
            Set<NamedElement, Classifier> m_inheritedMembers = Set<NamedElement, Classifier>(this);
            Set<GeneralizationSet, Classifier> m_powerTypeExtent = Set<GeneralizationSet, Classifier>(this);
            Singleton<RedefinableTemplateSignature, Classifier> m_classifierOwnedTemplateSignature = Singleton<RedefinableTemplateSignature, Classifier>(this);
            class AddGeneralizationFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    AddGeneralizationFunctor(Element* el) : SetFunctor(el) {};
            };
            class RemoveGeneralizationFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    RemoveGeneralizationFunctor(Element* el) : SetFunctor(el) {};
            };
            class AddGeneralFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    AddGeneralFunctor(Element* el) : SetFunctor(el) {};
            };
            class RemoveGeneralFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    RemoveGeneralFunctor(Element* el) : SetFunctor(el) {};
            };
            class AddOwnedMemberFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    AddOwnedMemberFunctor(Element* el) : SetFunctor(el) {};
            };
            class RemoveOwnedMemberFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    RemoveOwnedMemberFunctor(Element* el) : SetFunctor(el) {};
            };
            Set<RedefinableTemplateSignature, Classifier>& getOwnedTemplateSignatureSingleton();
            void referenceReindexed(ID oldID, ID newID) override;
            void reindexName(ID id, std::string newName) override;
            void referencingReleased(ID id) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            void init();
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