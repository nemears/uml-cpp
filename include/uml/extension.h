#ifndef EXTENSION_H
#define EXTENSION_H

#include "association.h"

namespace UML {

    class ExtensionEnd;
    namespace Parsers {
        class SetOwnedEnd;
    }

    class Extension : public Association {

        friend class UmlManager;
        friend class Parsers::SetOwnedEnd;

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
                    void operator()(ID id) const override;
            };
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Extension();
        public:
            Extension(const Extension& rhs);
            virtual ~Extension();
            void setMetaClass(ElementType metaClass);
            ElementType getMetaClass();
            ExtensionEnd* getOwnedEnd();
            ExtensionEnd& getOwnedEndRef();
            ID getOwnedEndID() const;
            bool hasOwnedEnd() const;
            void setOwnedEnd(ExtensionEnd& end);
            void setOwnedEnd(ExtensionEnd* end);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::EXTENSION;
            };
    };
}

#endif