#ifndef EXTENSION_H
#define EXTENSION_H

#include "association.h"

namespace UML {

    class ExtensionEnd;

    class Extension : public Association {

        friend class UmlManager;

        private:
            /**
             * NOTE: keeping it simple for now, uml suggests dealing with metaclasses as actual Classes stored within
             * every model, but obviously it will be more efficient to just keep track of it by enum right now.
             **/
            ElementType m_metaClass;
            Singleton<ExtensionEnd, Extension> m_ownedEnd = Singleton<ExtensionEnd, Extension>(this);
            bool m_setFlag = false;
            class RemoveOwnedEndProcedure : public AbstractSingletonProcedure<ExtensionEnd, Extension> {
                public:
                    RemoveOwnedEndProcedure(Extension* me) : AbstractSingletonProcedure<ExtensionEnd, Extension>(me) {};
                    void operator()(ExtensionEnd* el) const override;
            };
            class AddOwnedEndProcedure : public AbstractSingletonProcedure<ExtensionEnd, Extension> {
                public:
                    AddOwnedEndProcedure(Extension* me) : AbstractSingletonProcedure<ExtensionEnd, Extension>(me) {};
                    void operator()(ExtensionEnd* el) const override;
            };
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void referenceErased(ID id) override;
            Extension();
        public:
            Extension(const Extension& extension);
            virtual ~Extension();
            void setMetaClass(ElementType metaClass);
            ElementType getMetaClass();
            ExtensionEnd* getOwnedEnd();
            ExtensionEnd& getOwnedEndRef();
            bool hasOwnedEnd() const;
            void setOwnedEnd(ExtensionEnd& end);
            void setOwnedEnd(ExtensionEnd* end);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::EXTENSION;
            };
    };
}

#endif