#ifndef EXTENSION_END_H
#define EXTENSION_END_H

#include "property.h"
#include "stereotype.h"

namespace UML {

    class Extension;

    class ExtensionEnd : public Property {

        friend class UmlManager;

        private:
            Singleton<Extension, ExtensionEnd> m_extension = Singleton<Extension, ExtensionEnd>(this);
            class RemoveExtensionProcedure : public AbstractSingletonProcedure<Extension, ExtensionEnd> {
                public:
                    RemoveExtensionProcedure(ExtensionEnd* me) : AbstractSingletonProcedure<Extension, ExtensionEnd>(me) {};
                    void operator()(Extension* el) const override;
            };
            class AddExtensionProcedure : public AbstractSingletonProcedure<Extension, ExtensionEnd> {
                public:
                    AddExtensionProcedure(ExtensionEnd* me) : AbstractSingletonProcedure<Extension, ExtensionEnd>(me) {};
                    void operator()(Extension* el) const override;
            };
            Singleton<Stereotype, ExtensionEnd> m_extensionType = Singleton<Stereotype, ExtensionEnd>(this);
            class RemoveExtensionTypeProcedure : public AbstractSingletonProcedure<Stereotype, ExtensionEnd> {
                public:
                    RemoveExtensionTypeProcedure(ExtensionEnd* me) : AbstractSingletonProcedure<Stereotype, ExtensionEnd>(me) {};
                    void operator()(Stereotype* el) const override;
            };
            class AddExtensionTypeProcedure : public AbstractSingletonProcedure<Stereotype, ExtensionEnd> {
                public:
                    AddExtensionTypeProcedure(ExtensionEnd* me) : AbstractSingletonProcedure<Stereotype, ExtensionEnd>(me) {};
                    void operator()(Stereotype* el) const override;
            };
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            ExtensionEnd();
        public:
            ExtensionEnd(const ExtensionEnd& end);
            virtual ~ExtensionEnd();
            Extension* getExtension();
            Extension& getExtensionRef();
            bool hasExtension() const;
            void setExtension(Extension& extension);
            void setExtension(Extension* extension);
            Stereotype* getType() override;
            Stereotype& getTypeRef();
            bool hasType() const;
            void setType(Stereotype& stereotype);
            void setType(Stereotype* stereotype);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::EXTENSION_END;
            };
    };
}

#endif