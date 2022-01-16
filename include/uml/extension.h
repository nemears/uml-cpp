#ifndef EXTENSION_H
#define EXTENSION_H

#include "association.h"

namespace UML {

    class ExtensionEnd;

    class Extension : public Association {

        friend class UmlManager;
        friend class ExtensionEnd;

        private:
            /**
             * NOTE: keeping it simple for now, uml suggests dealing with metaclasses as actual Classes stored within
             * every model, but obviously it will be more efficient to just keep track of it by enum right now.
             **/
            ElementType m_metaClass;
            Singleton<ExtensionEnd, Extension> m_ownedEnd = Singleton<ExtensionEnd, Extension>(this);
            bool m_setFlag = false;
            Set<ExtensionEnd, Extension>& getOwnedEndSingleton();
            void init();
            void copy(const Extension& rhs);
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
            void setOwnedEnd(ID id);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::EXTENSION;
            };
    };
}

#endif