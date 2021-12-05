#ifndef CLASSIFIERH
#define CLASSIFIERH

#include "type.h"
#include "redefinableElement.h"
#include "namespace.h"
#include "templateableElement.h"

namespace UML {

    class Generalization;
    class Property;
    class Feature;
    class InstanceSpecification;
    class GeneralizationSet;
    namespace Parsers {
        class SetNestingClass;
    }

    /**
     * A Classifier represents a classification of instances according to their Features
     **/
    class Classifier: public Namespace, public Type , public RedefinableElement, public TemplateableElement {
        
        friend class UmlManager;
        friend class InstanceSpecification;
        friend class Generalization;
        friend class Parsers::SetNestingClass;

        protected:
            Set<Feature, Classifier> m_features = Set<Feature, Classifier>(this);
            Set<Property, Classifier> m_attributes = Set<Property, Classifier>(this);
            Set<Generalization, Classifier> m_generalizations = Set<Generalization, Classifier>(this);
            Set<Classifier, Classifier> m_generals = Set<Classifier, Classifier>(this);
            Set<NamedElement, Classifier> m_inheritedMembers = Set<NamedElement, Classifier>(this);
            // Set<GeneralizationSet, Classifier> m_powerTypeExtent = Set<GeneralizationSet, Classifier>(this);
            // Singleton<Class, Classifier> m_nestingClass = Singleton<Class, Classifier>(this);
            void reindexName(std::string oldName, std::string newName) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void referencingReleased(ID id) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            void init();
            void copy(const Classifier& rhs);
            Classifier();
        public:
            virtual ~Classifier();
            Classifier(const Classifier& rhs);
            std::string getName() override;
            void setName(const std::string& name) override;
            Set<Feature, Classifier>& getFeatures();
            Set<Property, Classifier>& getAttributes();
            Set<Generalization, Classifier>& getGeneralizations();
            Set<Classifier, Classifier>& getGenerals();
            Set<NamedElement, Classifier>& getInheritedMembers();
            // Sequence<GeneralizationSet>& getPowerTypeExtent();
            // // TODO move NestingClass to protected
            // Class* getNestingClass();
            // Class& getNestingClassRef();
            // ID getNestingClassID() const;
            // bool hasNestingClass() const;
            // void setNestingClass(Class* clazz);
            // void setNestingClass(Class& clazz);
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