#ifndef NAMEDELEMENTH
#define NAMEDELEMENTH

#include <string>
#include "element.h"
#include "singleton.h"
#include "set.h"
#include "singleton2.h"

namespace UML{

    template <class T> class Sequence;
    class Namespace;
    template <class T> class AbstractSequenceFunctor;
    class UmlManager;
    class Dependency;

    enum class VisibilityKind {
        PUBLIC,
        PROTECTED,
        PRIVATE,
        PACKAGE
    };

    /**
     * A NamedElement is an Element in a model that may have a name
     **/
    class NamedElement : virtual public Element {

        friend class UmlManager;
        friend class Namespace;

        protected:
            std::string m_name;
            std::string m_absoluteNamespace;
            Singleton2<Namespace, NamedElement> m_namespace = Singleton2<Namespace, NamedElement>(this);
            Set<Namespace, NamedElement> m_memberNamespace = Set<Namespace, NamedElement>(this);
            Set<Dependency, NamedElement> m_clientDependencies = Set<Dependency, NamedElement>(this);
            Set<Dependency, NamedElement> m_supplierDependencies =  Set<Dependency, NamedElement>(this);
            // visibility defaults to public, don't think there is a none value
            VisibilityKind m_visibility = VisibilityKind::PUBLIC;
            virtual void reindexName(std::string oldName, std::string newName);
            void updateQualifiedName(std::string absoluteNamespace);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Set<Namespace, NamedElement>& getNamespaceSingleton();
            NamedElement();
        public:
            virtual ~NamedElement();
            NamedElement(const NamedElement& el);
            virtual std::string getName();
            virtual void setName(const std::string &name);
            std::string getQualifiedName();
            Namespace* getNamespace();
            Namespace& getNamespaceRef();
            ID getNamespaceID() const;
            bool hasNamespace() const;
            void setNamespace(Namespace* nmspc);
            void setNamespace(Namespace& nmspc);
            Set<Namespace, NamedElement>& getMemberNamespace();
            Set<Dependency, NamedElement>& getClientDependencies();
            Set<Dependency, NamedElement>& getSupplierDependencies();
            VisibilityKind getVisibility();
            void setVisibility(VisibilityKind visibility);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::NAMED_ELEMENT;
            };
    };
}

#endif