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

    /**
     * A Classifier represents a classification of instances according to their Features
     **/
    class Classifier: public Namespace, public Type , public RedefinableElement, public TemplateableElement {
        friend class UmlManager;
        protected:
            Sequence<Feature> m_features = Sequence<Feature>(this);
            Sequence<Property> m_attributes = Sequence<Property>(this);
            Sequence<Generalization> m_generalizations = Sequence<Generalization>(this);
            Sequence<Classifier> m_generals = Sequence<Classifier>(this);
            Sequence<NamedElement> m_inheritedMembers = Sequence<NamedElement>(this);
            void setManager(UmlManager* manager) override;
            void reindexID(ID oldID, ID newID) override;
            void reindexName(std::string oldName, std::string newName) override;
            class AddAttributeFunctor : public AbstractSequenceFunctor {
                public:
                    AddAttributeFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveAttributeFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveAttributeFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class CheckGeneralizationFunctor : public AbstractSequenceFunctor {
                public:
                    CheckGeneralizationFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class AddGeneralizationFunctor : public AbstractSequenceFunctor {
                public:
                    AddGeneralizationFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveGeneralizationFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveGeneralizationFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class AddGeneralFunctor : public AbstractSequenceFunctor {
                public:
                    AddGeneralFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveGeneralFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveGeneralFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class AddFeatureFunctor : public AbstractSequenceFunctor {
                public:
                    AddFeatureFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveFeatureFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveFeatureFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class AddInheritedMemberFunctor : public AbstractSequenceFunctor {
                public:
                    AddInheritedMemberFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveInheritedMemberFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveInheritedMemberFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class ClassifierAddMemberFunctor : public AbstractSequenceFunctor {
                public:
                    ClassifierAddMemberFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class ClassifierRemoveMemberFunctor : public AbstractSequenceFunctor {
                public:
                    ClassifierRemoveMemberFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            void restoreReleased(ID id, Element* released) override;
            void referencingReleased(ID id) override;
        public:
            Classifier();
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