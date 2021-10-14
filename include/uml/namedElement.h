#ifndef NAMEDELEMENTH
#define NAMEDELEMENTH

#include <string>
#include "element.h"
#include "singleton.h"

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
            Singleton<Namespace, NamedElement> m_namespace = Singleton<Namespace, NamedElement>(this);
            class RemoveNamespaceProcedures : public AbstractSingletonProcedure<Namespace, NamedElement> {
                public:
                    RemoveNamespaceProcedures(NamedElement* me) : AbstractSingletonProcedure<Namespace, NamedElement>(me) {};
                    void operator()(Namespace* el) const override;
            };
            class AddNamespaceProcedures : public AbstractSingletonProcedure<Namespace, NamedElement> {
                public:
                    AddNamespaceProcedures(NamedElement* me) : AbstractSingletonProcedure<Namespace, NamedElement>(me) {};
                    void operator()(Namespace* el) const override;
                    void operator()(ID id) const override;
            };
            Sequence<Namespace>* m_memberNamespace;
            Sequence<Dependency>* m_clientDependencies;
            Sequence<Dependency>* m_supplierDependencies;
            // visibility defaults to public, don't think there is a none value
            VisibilityKind m_visibility = VisibilityKind::PUBLIC;
            virtual void reindexName(std::string oldName, std::string newName);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            NamedElement();
        public:
            virtual ~NamedElement();
            NamedElement(const NamedElement& el);
            virtual std::string getName();
            virtual void setName(const std::string &name);
            Namespace* getNamespace();
            Namespace& getNamespaceRef();
            ID getNamespaceID() const;
            bool hasNamespace() const;
            void setNamespace(Namespace* nmspc);
            void setNamespace(Namespace& nmspc);
            Sequence<Namespace>& getMemberNamespace();
            Sequence<Dependency>& getClientDependencies();
            Sequence<Dependency>& getSupplierDependencies();
            VisibilityKind getVisibility();
            void setVisibility(VisibilityKind visibility);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::NAMED_ELEMENT;
            };
    };
}

#endif