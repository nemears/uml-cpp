#ifndef CLASSIFIERH
#define CLASSIFIERH

#include "type.h"
#include "redefinableElement.h"
#include "sequence.h"
#include "namespace.h"
#include "templateableElement.h"

namespace UML {

    class Generalization;
    class Property;
    class Feature;
    class InstanceSpecification;

    /**
     * A Classifier represents a classification of instances according to their Features
     **/
    class Classifier: public Namespace, public Type , public RedefinableElement, public TemplateableElement {
        friend class UmlManager;
        friend class InstanceSpecification;
        protected:
            Sequence<Feature> m_features = Sequence<Feature>(this);
            Sequence<Property> m_attributes = Sequence<Property>(this);
            Sequence<Generalization> m_generalizations = Sequence<Generalization>(this);
            Sequence<Classifier> m_generals = Sequence<Classifier>(this);
            Sequence<NamedElement> m_inheritedMembers = Sequence<NamedElement>(this);
            void setManager(UmlManager* manager) override;
            void reindexName(std::string oldName, std::string newName) override;
            void referenceReindexed(ID oldID, ID newID) override;
            class AddAttributeFunctor : public TemplateAbstractSequenceFunctor<Property,Classifier> {
                public:
                    AddAttributeFunctor(Classifier* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Property& el) const override;
            };
            class RemoveAttributeFunctor : public TemplateAbstractSequenceFunctor<Property,Classifier> {
                public:
                    RemoveAttributeFunctor(Classifier* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Property& el) const override;
            };
            class CheckGeneralizationFunctor : public TemplateAbstractSequenceFunctor<Generalization,Classifier> {
                public:
                    CheckGeneralizationFunctor(Classifier* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Generalization& el) const override;
            };
            class AddGeneralizationFunctor : public TemplateAbstractSequenceFunctor<Generalization,Classifier> {
                public:
                    AddGeneralizationFunctor(Classifier* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Generalization& el) const override;
            };
            class RemoveGeneralizationFunctor : public TemplateAbstractSequenceFunctor<Generalization,Classifier> {
                public:
                    RemoveGeneralizationFunctor(Classifier* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Generalization& el) const override;
            };
            class AddGeneralFunctor : public TemplateAbstractSequenceFunctor<Classifier,Classifier> {
                public:
                    AddGeneralFunctor(Classifier* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Classifier& el) const override;
            };
            class RemoveGeneralFunctor : public TemplateAbstractSequenceFunctor<Classifier,Classifier> {
                public:
                    RemoveGeneralFunctor(Classifier* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Classifier& el) const override;
            };
            class AddFeatureFunctor : public TemplateAbstractSequenceFunctor<Feature,Classifier> {
                public:
                    AddFeatureFunctor(Classifier* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Feature& el) const override;
            };
            class RemoveFeatureFunctor : public TemplateAbstractSequenceFunctor<Feature,Classifier> {
                public:
                    RemoveFeatureFunctor(Classifier* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Feature& el) const override;
            };
            class AddInheritedMemberFunctor : public TemplateAbstractSequenceFunctor<NamedElement,Classifier> {
                public:
                    AddInheritedMemberFunctor(Classifier* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(NamedElement& el) const override;
            };
            class RemoveInheritedMemberFunctor : public TemplateAbstractSequenceFunctor<NamedElement,Classifier> {
                public:
                    RemoveInheritedMemberFunctor(Classifier* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(NamedElement& el) const override;
            };
            class ClassifierAddMemberFunctor : public TemplateAbstractSequenceFunctor<NamedElement,Classifier> {
                public:
                    ClassifierAddMemberFunctor(Classifier* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(NamedElement& el) const override;
            };
            class ClassifierRemoveMemberFunctor : public TemplateAbstractSequenceFunctor<NamedElement,Classifier> {
                public:
                    ClassifierRemoveMemberFunctor(Classifier* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(NamedElement& el) const override;
            };
            void restoreReleased(ID id, Element* released) override;
            void referencingReleased(ID id) override;
            Classifier();
        public:
            virtual ~Classifier();
            Classifier(const Classifier& clazz);
            std::string getName() override;
            void setName(const std::string& name) override;
            Sequence<Feature>& getFeatures();
            Sequence<Property>& getAttributes();
            /**
             * Generalizations to inherited classifier
             * the generalizations specific reference is this classifier
             **/
            Sequence<Generalization>& getGeneralizations();
            /**
             * this returns all classifiers this classifier inherits from
             **/
            Sequence<Classifier>& getGenerals();
            Sequence<NamedElement>& getInheritedMembers();
            ElementType getElementType() const override;
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